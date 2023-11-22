// 2023/11/22 13:39:38 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/Memory/ROM.h"
#include "Settings/SettingsNRST.h"


/*
    Принцип хранения такой.
    Сначала заполняется первый сектор.
    Когда сектор полностью заполнен (определяется по тому, что в нём не осталось места для хранения структуры рамзером как последняя сохранённая),
    сохраняемая структура записывается в начало второго сектора.
    Когда заполняется полностью второй сектор, стирается первый сектор и запись производится в него. Стирается второй сектор
*/


struct Packet
{
    uint size;
    
    // Возвращает адрес следующего за этим пакета
    Packet *Next();
    
    // Адрес первого байта в пакете
    uint Begin() { return reinterpret_cast<uint>(this);  } //-V2571
    
    // Возвращает адрес первого байта за этим пакетом
    uint End()   { return reinterpret_cast<uint>(Next()); } //-V2571
    
    // Возвращает true, если в пакете нет данных (пакет находится за последним записанным пакетом)
    bool IsEmpty() const;
    
    // Попытка записать в пакет структуру с данными
    bool SaveSettings(SettingsNRST *nrst);
};


struct SectorNRST
{
    // Возвращает указатель на сохранённую структуру если таковая имеется и nullptr в противном случае
    SettingsNRST *GetSaved() const;
    
    // Возвращает указатель на пакет, установленный на начало сектора
    Packet *CreatePacket() const;
    
    // Возвращает указатель на последний записанный в секторе пакет
    Packet *LastPacket() const;
    
    // Попытка записать в сектор структуру с данными
    bool SaveSettings(SettingsNRST *) const;

    const Sector &sector;
};


static const SectorNRST sector1 = { HAL_ROM::sectors[Sector::_03_NRST_1] };
static const SectorNRST sector2 = { HAL_ROM::sectors[Sector::_04_NRST_2] };


SettingsNRST *ROM::NRST::GetSaved()
{
    if(sector2.GetSaved())          // Если во втором секторе сохранены настройки
    {
        return sector2.GetSaved();  // то их и возвращаем
    }

    return sector1.GetSaved();      // иначе возвращаем настрйоки из первого сектора, если таковые имеются
}


void ROM::NRST::Save(SettingsNRST *nrst)
{
    if(!sector1.SaveSettings(nrst))         // Сначала пытаемся записать в первый сектор
    {
        if(!sector2.SaveSettings(nrst))     // Потом пытаемся записать во второй сектор
        {
            sector1.sector.Erase();
            sector1.SaveSettings(nrst);
            sector2.sector.Erase();
        }
    }
}


SettingsNRST *SectorNRST::GetSaved() const
{
    Packet *last = LastPacket();

    return (last && last->IsEmpty()) ? nullptr : reinterpret_cast<SettingsNRST *>(last);
}


Packet *SectorNRST::CreatePacket() const
{
    return reinterpret_cast<Packet *>(sector.address); //-V2571
}


Packet *SectorNRST::LastPacket() const
{
    Packet *packet = CreatePacket();

    while(!packet->IsEmpty())
    {
        if(packet->End() >= sector.End() - 4)   // Проверяем, что пакет не находится на границе сектора. Откидываем четыре байта, в которых поместится информация, что следующий пакет пустой
        {
            return packet;
        }

        if(packet->Next()->IsEmpty())           // Если следующий пакет - пустой, то мы нашли наш пакет
        {
            return packet;
        }

        packet = packet->Next();
    }

    return nullptr;
}


bool Packet::IsEmpty() const
{
    return (size == 0xFFFFFFFFU);
}


bool SectorNRST::SaveSettings(SettingsNRST *nrst) const
{
    Packet *last = LastPacket();                            // Находим последний сохранённый в секторе пакет

    if(last == nullptr)                                     // Если пакетов ещё нету
    {
        last = reinterpret_cast<Packet *>(sector.address);  // то он будет в самом начале сектора //-V2571

        return last->SaveSettings(nrst);                    // В него и сохраняем
    }

    return last->Next()->SaveSettings(nrst);                // Если сохранён хотя бы один пакет - делаем попытку записи за ним
}


bool Packet::SaveSettings(SettingsNRST *nrst)
{
    const Sector &sector = Sector::Get(Begin());

    if((Begin() < sector.address)   ||                      // Проверяем на выход за начало сектора
       !IsEmpty()                   ||                      // Проверямем на то, чтобы по этому адресу ничего не было записано уже
       (sizeof(SettingsNRST) + Begin() >= sector.End()))    // Проверяем на выход за конец сектора
    {
        return false;
    }

    nrst->size = sizeof(SettingsNRST);

    HAL_ROM::WriteBufferBytes(Begin(), nrst, sizeof(SettingsNRST));

    return true;
}


Packet *Packet::Next()
{
    if(size == 0xFFFFFFFFU)
    {
        return nullptr;
    }

    return reinterpret_cast<Packet *>(Begin() + size); //-V2571
}


void ROM::NRST::Erase()
{
    sector1.sector.Erase();
    sector2.sector.Erase();
}
