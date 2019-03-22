#include "log.h"
#include "DecoderPanel.h"
#include "Hardware/FSMC.h"
#include "Display/Display.h"
#include "Display/Painter.h"
#include "Display/Text.h"
#include "Hardware/Keyboard.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef bool(*pFuncBU8)(uint8);
/// Выполняемая функция
static pFuncBU8 curFunc;
/// Текущий байт выполняемой функции
static int step = 0;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Обработка запроса на изображение экрана
static bool FuncScreen(uint8);

static bool EmptyFunc(uint8) { return true; }

static bool InButtonPress(uint8);

static bool BeginScene(uint8);

static bool EndScene(uint8);

static bool SetColor(uint8);

static bool FillRegion(uint8);

static bool DrawText(uint8);

static bool DrawBigText(uint8);

static bool SetPalette(uint8);

static bool DrawRectangle(uint8);

static bool DrawVLine(uint8);

static bool SetFont(uint8);

static bool SetPoint(uint8);

static bool DrawLine(uint8);

static bool DrawHLine(uint8);

static bool DrawTesterPoints(uint8);

static bool DrawVPointLine(uint8);

static bool DrawHPointLine(uint8);
/// Эту функцию надо вызывать после выполнения последнего шага
static void FinishCommand();


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Decoder::AddData(uint8 data)
{
    static const struct StructFunc
    {
        pFuncBU8 func;
        StructFunc(pFuncBU8 f) : func(f) {};
    }
    command[Command::Size] =
    {
        EmptyFunc,
        InButtonPress,
        BeginScene,
        EndScene,
        SetColor,
        FillRegion,
        DrawText,
        SetPalette,
        DrawRectangle,
        DrawVLine,
        DrawHLine,
        SetFont,
        SetPoint,
        DrawLine,
        DrawTesterPoints,
        DrawBigText,
        FuncScreen,
        DrawVPointLine,
        DrawHPointLine
    };

    if (step == 0)
    {
        if (data < Command::Size)
        {
            curFunc = command[data].func;
            if (curFunc == 0)
            {
                LOG_ERROR("Не обнаржен обработчик");
            }
        }
        else
        {
            FinishCommand();
            return;     // Простенькая защита от ошибок
        }
    }

    if (curFunc(data))
    {
        FinishCommand();
    }
    else
    {
        step++;
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static bool InButtonPress(uint8)
{
    if (step == 0)
    {
        return false;
    }
    if (step == 1)
    {
        return false;
    }
    if (step == 2)
    {
    }
    return true;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static bool BeginScene(uint8 data)
{
    if (step == 0)
    {
        return false;
    }
    if (step == 1)
    {
        Painter::BeginScene((Color)data);
    }
    return true;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static bool DrawTesterPoints(uint8 data)
{
    // Здесь хранится текущий принимаемый байт. Всего их будет 2400
    static int numPoint = 0;
    static Color color = Color::FILL;
    static uint8 mode = 0;

    static uint8 buffer[TESTER_NUM_POINTS * 3] __attribute__((aligned(2)));

    if(step == 0)
    {
        numPoint = 0;
    }
    else if(step == 1)
    {
        mode = data;
    }
    else if(step == 2)
    {
        color = Color(data);
    }
    else
    {
        if (numPoint < TESTER_NUM_POINTS)   // Если первые точки, то это иксы - ложим их в младшие байты полуслов
        {
            uint16 *pointer = (uint16 *)buffer; //-V1032
            pointer[numPoint++] = data;
        }
        else
        {
            buffer[numPoint + TESTER_NUM_POINTS] = data;
            numPoint++;
        }

        if(numPoint == TESTER_NUM_POINTS * 2)
        {
            Painter::DrawTesterData(mode, color, (uint16 *)buffer, buffer + TESTER_NUM_POINTS * 2);
            return true;
        }
    }
    return false;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static bool EndScene(uint8)
{
    Painter::EndScene();
    return true;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static bool SetColor(uint8 data)
{
    if (step == 0)
    {
        return false;
    }
    if (step == 1)
    {
        Painter::SetColor((Color)data);
    }
    return true;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static bool FuncScreen(uint8 data)
{
    if (step == 0)
    {
        return false;
    }

    if (step == 1)
    {
        Painter::SendRow(data);
    }

    return true;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static bool FillRegion(uint8 data)
{
    static int x;
    static int y;
    static int width;
    static int height;
    
    bool result = false;

    switch (step)
    {
        case 0:                                     break;
        case 1:     x = data;                       break;
        case 2:     x += (((int)data) << 8);        break;
        case 3:     y = data;                       break;
        case 4:     width = data;                   break;
        case 5:     width += (((int)data) << 8);    break;
        case 6:     height = (int)data;
            Painter::FillRegion(x, y, width, height);
            result = true;
            break;
        default:
            result = true;
            break;
    }
    
    return result;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static bool DrawRectangle(uint8 data)
{
    static int x;
    static int y;
    static int width;
    static int height;
    
    bool result = false;

    switch (step)
    {
        case 0:                                     break;
        case 1:     x = data;                       break;
        case 2:     x += (((int)data) << 8);        break;
        case 3:     y = data;                       break;
        case 4:     width = data;                   break;
        case 5:     width += (((int)data) << 8);    break;
        case 6:     height = (int)data;
            Painter::DrawRectangle(x, y, width, height);
            result = true;
            break;
        default:
            result = true;
            break;
    }
    
    return result;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static bool DrawVLine(uint8 data)
{
    static int x;
    static int y0;
    static int y1;
    
    bool result = false;

    switch (step)
    {
        case 0:                             break;
        case 1: x = data;                   break;
        case 2: x += (((int)data) << 8);    break;
        case 3: y0 = data;                  break;
        case 4: y1 = data;
            Painter::DrawVLine(x, y0, y1);
            result = true;
            break;
        default:
            result = true;
            break;
    }
    
    return result;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static bool DrawHLine(uint8 data)
{
    __IO static int y;  /// \todo эти штуки __IO вставлены потому, что без них c оптимизацией экран ничего не хочет выводить. Надо потом разобраться
    __IO static int x0;
    __IO static int x1;
    
    bool result = false;

    switch (step)
    {
        case 0:                             break;
        case 1:     y = data;               break;
        case 2:     x0 = data;              break;
        case 3:     x0 += (int)data << 8;   break;
        case 4:     x1 = data;              break;
        case 5:     x1 += (int)data << 8;
            Painter::DrawHLine(y, x0, x1);
            result = true;
            break;
        default:
            result = true;
            break;
    }
    
    return result;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static bool DrawVPointLine(uint8 data)
{
    static int y;
    static int x;
    static int delta;
    static int count;

    bool result = false;

    switch (step)
    {
    case 0:                         break;
    case 1: x = data;               break;
    case 2: x += ((int)data) << 8;  break;
    case 3: y = data;               break;
    case 4: delta = data;           break;
    case 5: count = data;
        Painter::DrawVPointLine(x, y, delta, count);
        result = true;
        break;
    default:
        result = true;
        break;
    }

    return result;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static bool DrawHPointLine(uint8 data)
{
    static int y;
    static int x;
    static int delta;
    static int count;

    bool result = false;

    switch (step)
    {
    case 0:                         break;
    case 1: x = data;               break;
    case 2: x += ((int)data) << 8;  break;
    case 3: y = data;               break;
    case 4: delta = data;           break;
    case 5: count = data;
        Painter::DrawHPointLine(x, y, delta, count);
        result = true;
        break;
    default:
        result = true;
        break;
    }

    return result;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static bool DrawLine(uint8 data)
{
    __IO static int x0;
    __IO static int y0;
    __IO static int x1;
    
    bool result = false;

    switch (step)
    {
        case 0:                         break;
        case 1: x0 = data;              break;
        case 2: x0 += ((int)data << 8); break;
        case 3: y0 = data;              break;
        case 4: x1 = data;              break;
        case 5: x1 += ((int)data << 8); break;
        case 6: Painter::DrawLine(x0, y0, x1, data);
            result = true;
            break;
        default:
            result = true;
            break;
    }

    return result;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static bool SetPoint(uint8 data)
{
    static int x = 0;
    
    bool result = false;

    switch (step)
    {
        case 0:                         break;
        case 1: x = data;               break;
        case 2: x += ((int)data << 8);  break;
        case 3: Painter::SetPoint(x, data);
            result = true;
            break;
        default:
            result = true;
            break;
    }

    return result;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static bool DrawText(uint8 data)
{
    static int x;
    static int y;
    static int numSymbols;
    static int readingSymbols;
    static char *buffer;

    switch (step)
    {
        case 0:                             break;
        case 1:     x = data;               break;
        case 2:     x += (int)data << 8;    break;
        case 3:
            y = data;
            if (y > 239)
            {
                y -= 256;
            }
            break;
        case 4:
            numSymbols = data;
            readingSymbols = 0;
            buffer = new char[(uint)(numSymbols + 1)];
            break;
        default:
            buffer[readingSymbols++] = (char)data;
            if (readingSymbols == numSymbols)
            {
                buffer[readingSymbols] = 0;
                Painter::DrawText(x, y, buffer);
                delete [] buffer;
                return true;
            }
            break;
    }
    return false;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static bool DrawBigText(uint8 data)
{
    static int x;
    static int y;
    static uint8 size;
    static int numSymbols;
    static int readingSymbols;
    static char *buffer;

    switch (step)
    {
    case 0:                             break;
    case 1:     x = data;               break;
    case 2:     x += (int)data << 8;    break;
    case 3:     y = data;               break;
    case 4:     size = data;            break;
    case 5:
        numSymbols = data;
        readingSymbols = 0;
        buffer = new char[(uint)(numSymbols + 1)];
        break;
    default:
        buffer[readingSymbols++] = (char)data;
        if (readingSymbols == numSymbols)
        {
            buffer[readingSymbols] = 0;
            Text::DrawBigText(x, y, size, buffer);
            delete[] buffer;
            return true;
        }
        break;
    }
    return false;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static bool SetPalette(uint8 data)
{
    static uint8 numColor;
    static uint valueColor;
    
    bool result = false;

    switch (step)
    {
        case 0:                                 break;
        case 1: numColor = data;                break;
        case 2: valueColor = data;              break;
        case 3: valueColor |= (uint)data << 8;  break;
        case 4: valueColor |= (uint)data << 16; break;
        case 5: valueColor |= (uint)data << 24;
            Painter::SetColorValue(Color(numColor), valueColor);
            result = true;
            break;
        default:
            result = true;
            break;
    }
    
    return result;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static bool SetFont(uint8 data)
{
    if (step == 0)
    {
        return false;
    }

    Font::SetCurrent((Font::Type::E)data);

    return true;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void FinishCommand()
{
    step = 0;
    curFunc = 0;
}
