#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/Memory/ROM.h"
#include "Settings/SettingsNRST.h"


/*
    ѕринцип хранени€ такой.
    —начала заполн€етс€ первый сектор.
     огда сектор полностью заполнен (определ€етс€ по тому, что в нЄм не осталось места дл€ хранени€ структуры рамзером как поселдн€€ сохранЄнна),
    сохран€ема€ структура записываетс€ в начало второго сектора.
     огда заполн€етс€ полностью второй сектор, стираетс€ первый сектор и запись производитс€ в него. —тираетс€ второй сектор
*/


struct Packet
{
    uint size;
    // ¬озвращает адрес следующего за этим пакета
    Packet *Next();
    // јдрес первого байта в пакете
    uint Begin() { return reinterpret_cast<uint>(this);  }
    // ¬озвращает адрес первого байта за этим пакетом
    uint End()   { return reinterpret_cast<uint>(Next()); }
    // ¬озвращает true, если в пакете нет данных (пакет находитс€ за последним записанным пакетом)
    bool IsEmpty() const;
    /// ѕопытка записать в пакет структуру с данными
    bool SaveSettings(SettingsNRST *nrst);
};


struct SectorNRST
{
    // ¬озвращает указатель на сохранЄнную структуру если такова€ имеетс€ и nullptr в противном случае
    SettingsNRST *GetSaved() const;
    // ¬озвращает указатель на пакет, установленный на начало сектора
    Packet *CreatePacket() const;
    // ¬озвращает указатель на последний записанный в секторе пакет
    Packet *LastPacket() const;
    /// ѕопытка записать в сектор структуру с данными
    bool SaveSettings(SettingsNRST *) const;

    const Sector &sector;
};


static const SectorNRST sector1 = { HAL_ROM::sectors[Sector::_03_NRST_1] };
static const SectorNRST sector2 = { HAL_ROM::sectors[Sector::_04_NRST_2] };


SettingsNRST *ROM::NRST::GetSaved()
{
    if(sector2.GetSaved())          // ≈сли во втором секторе сохранены настройки
    {
        return sector2.GetSaved();  // то их и возвращаем
    }

    return sector1.GetSaved();      // иначе возвращаем настрйоки из первого сектора, если таковые имеютс€
}


void ROM::NRST::Save(SettingsNRST *nrst)
{
    if(!sector1.SaveSettings(nrst))         // —начала пытаемс€ записать в первый сектор
    {
        if(!sector2.SaveSettings(nrst))     // ѕотом пытаемс€ записать во второй сектор
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

    return last->IsEmpty() ? nullptr : reinterpret_cast<SettingsNRST *>(last);
}


Packet *SectorNRST::CreatePacket() const
{
    return reinterpret_cast<Packet *>(sector.address);
}


Packet *SectorNRST::LastPacket() const
{
    Packet *packet = CreatePacket();

    while(!packet->IsEmpty())
    {
        if(packet->End() >= sector.End() - 4)   // ѕровер€ем, что пакет не находитс€ на границе сектора. ќткидываем четыре байта, в которых поместитс€ информаци€, что следующий пакет пустой
        {
            return packet;
        }

        if(packet->Next()->IsEmpty())           // ≈сли следующий пакет - пустой, то мы нашли наш пакет
        {
            return packet;
        }

        packet = packet->Next();
    }

    return nullptr;
}


bool Packet::IsEmpty() const
{
    return (size == 0xFFFFFFFF);
}


bool SectorNRST::SaveSettings(SettingsNRST *nrst) const
{
    Packet *last = LastPacket();

    if(last == nullptr)
    {
        return last->SaveSettings(nrst);
    }

    return last->Next()->SaveSettings(nrst);
}


bool Packet::SaveSettings(SettingsNRST *nrst)
{
    const Sector &sector = Sector::Get(Begin());

    if((Begin() < sector.address)   ||                      // ѕровер€ем на выход за начало сектора
       !IsEmpty()                   ||                      // ѕровер€мем на то, чтобы по этому адресу ничего не было записано уже
       (sizeof(SettingsNRST) + Begin() >= sector.End()))    // ѕровер€ем на выход за конец сектора
    {
        return false;
    }

    nrst->size = sizeof(SettingsNRST);

    HAL_ROM::WriteBufferBytes(Begin(), nrst, sizeof(SettingsNRST));

    return true;
}


Packet *Packet::Next()
{
    if(size == 0xFFFFFFFF)
    {
        return nullptr;
    }

    return reinterpret_cast<Packet *>(Begin() + size);
}
