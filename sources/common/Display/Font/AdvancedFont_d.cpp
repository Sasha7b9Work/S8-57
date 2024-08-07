// 2023/11/22 13:38:24 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "AdvancedFont_d.h"
#include "fontGOST28.inc"
#include "fontGOST72bold.inc"


namespace DAdvancedFont
{
    struct NativeSymbol
    {
        uint8 width;        // ������ ������� � ��������
        uint8 height;       // ������ ������� � ��������
        uint8 firstRow;     // ����� ������ �������� ������. ������ � ������ ���� �������� � data
        //    uint8 data;     // ������ ���� ������ ������ �������

            // ���������� ���������� ���� � ������
        int BytesInRow();
        // ���������� ��������� �� ������ ���� ������
        uint8 *GetRow(int row);
        // ���������� ��������� �� ������ ���� ������
        uint8 *Data();

        bool BitIsExist(int row, int bit);
    };

    // ���������� ���������
    struct HeaderFont
    {
        uint16       offsets[256];  // �������� 256 �������� �������. 0 ��������, ��� ������ �����������
        NativeSymbol symbol;        // ������ ������ � ������� ��� �������� 256

        // ���������� ��������� �� ������, ���� �� ������������ � ������� � nullptr � �������� ������
        NativeSymbol *GetSymbol(uint8 num);

        static HeaderFont *Sefl();
    };


    DTypeFont::E currentType = DTypeFont::None;


    static const unsigned char *font = nullptr;
}


void DAdvancedFont::Set(DTypeFont::E t)
{
    currentType = t;
    
    if(currentType == DTypeFont::_GOST28)
    {
        font = fontGOST28;
    }
    else if(currentType == DTypeFont::_GOST72bold)
    {
        font = fontGOST72bold;
    }
    else
    {
        font = nullptr;
    }
}


bool DAdvancedFont::RowNotEmpty(uint8 s, int r)
{
    HeaderFont *header = HeaderFont::Sefl();

    NativeSymbol *symbol = header->GetSymbol(s);

    if (symbol)
    {
        if (r < symbol->firstRow)
        {
            return false;
        }

        uint8 *row = symbol->GetRow(r);

        if (row)
        {
            for (int i = 0; i < symbol->BytesInRow(); i++)
            {
                if (row[i] != 0)
                {
                    return true;
                }
            }
        }
    }

    return false;
}


uint8 DAdvancedFont::GetWidth(uint8 num)
{
    NativeSymbol *symbol = HeaderFont::Sefl()->GetSymbol(num);

    return symbol ? symbol->width : 0U;
}

uint8 DAdvancedFont::GetHeight(uint8 num)
{
    NativeSymbol *symbol = HeaderFont::Sefl()->GetSymbol(num);

    return symbol ? symbol->height : 0U;
}


uint8 DAdvancedFont::GetHeight()
{
    uint8 result = 0;

    for (int i = 0; i < 256; i++)
    {
        NativeSymbol *symbol = HeaderFont::Sefl()->GetSymbol(static_cast<uint8>(i));

        if (symbol && symbol->height > result)
        {
            result = symbol->height;
        }
    }

    return result;
}


int DAdvancedFont::NativeSymbol::BytesInRow()
{
    int result = width / 8;

    if (width % 8)
    {
        result++;
    }

    return result;
}


uint8 *DAdvancedFont::NativeSymbol::GetRow(int row)
{
    if (row > height - 1)
    {
        return nullptr;
    }

    if (row < firstRow)
    {
        return nullptr;
    }

    return Data() + (row - firstRow) * BytesInRow();
}


DAdvancedFont::NativeSymbol *DAdvancedFont::HeaderFont::GetSymbol(uint8 num)
{
    HeaderFont *header = HeaderFont::Sefl();

    if (header->offsets[num] == 0)
    {
        return nullptr;
    }

    uint8 *offset = reinterpret_cast<uint8 *>(header) + header->offsets[num];

    return reinterpret_cast<NativeSymbol *>(offset);
}


DAdvancedFont::HeaderFont *DAdvancedFont::HeaderFont::Sefl()
{
    return reinterpret_cast<HeaderFont *>(const_cast<uint8 *>(font)); //-V2567
}


uint8 *DAdvancedFont::NativeSymbol::Data()
{
    return reinterpret_cast<uint8 *>(this) + sizeof(*this);
}


bool DAdvancedFont::BitIsExist(uint8 s, int row, int bit)
{
    NativeSymbol *symbol = HeaderFont::Sefl()->GetSymbol(s);

    return symbol ? symbol->BitIsExist(row, bit) : false;
}


bool DAdvancedFont::NativeSymbol::BitIsExist(int r, int b)
{
    uint8 *row = GetRow(r);

    if (row == nullptr)
    {
        return false;
    }

    while (b > 7)       // ������������� � �����, ����������� ��� ���
    {
        row++;
        b -= 8;
    }

    return ((*row) & (1 << (7 - b))) != 0;
}
