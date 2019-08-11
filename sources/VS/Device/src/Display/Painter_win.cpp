#include "defines.h"
#pragma warning(push)
#pragma warning(disable:4018 4076 4091 4189 4365 4459 4571 4625 4668 5026 5027)
#include "Application.h"


#define uint    unsigned int
#define int8    signed char
#define uint8   unsigned char
#define int16   signed short
#define uint16  unsigned short
#define uint    unsigned int
#define uchar   unsigned char
#define pString const char * const

#include "Display/Painter.h"
#include "Keyboard/BufferButtons.h"

#include <SDL.h>

#include <wx/display.h>

#pragma warning(pop)


#undef uint   
#undef int8   
#undef uint8  
#undef int16  
#undef uint16 
#undef uint   
#undef uchar  
#undef pString


#include "defines.h"
#include "Keyboard/Keyboard.h"
#include "Menu/Menu.h"
#include "Utils/Math.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
SDL_Renderer *renderer = nullptr;
static SDL_Window *window = nullptr;
static SDL_Texture *texture = nullptr;

/// Здесь хранятся указатели на кнопки
static wxButton *buttons[Key::Number] = { nullptr };
/// Цвета
static uint colors[256];

static bool needStartTimerLong = false;
static bool needStopTimerLong = false;
/// Здесь имя нажатой кнопки
static Key::E pressedKey = Key::None;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Создаёт окно приложения. Возвращает хэндл виджета для отрисовки
static HANDLE CreateFrame();
/// Установить размер и оптимальную позицию для окна приложения
static void SetPositionAndSize(Frame *frame);
/// Получить разрешение максимального имеющегося в системе монитора
static wxRect GetMaxDisplay();
/// Создаёт все кнопки
static void CreateButtons(Frame *frame);
/// Создаёт одну кнопку
static void CreateButton(Key::E key, Frame *frame, const wxPoint &pos, const wxSize &size, pString title);
/// Создаёт кнопки для меню канала
static void CreateButtonsChannel(Frame *frame, const char *title, int x, int y, Key::E keyChannel, Key::E keyRangeLess, Key::E keyRangeMore, Key::E keyRShiftLess, Key::E keyRShiftMore);
/// Создаёт кнопки группы синхронизации
static void CreateButtonsTrig(Frame *frame, int x, int y);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Painter::Init()
{
    HANDLE handle = CreateFrame();

    window = SDL_CreateWindowFrom(handle);

    if (window == nullptr)
    {
        std::cout << "SDL_CreateWindowFrom() Error: " << SDL_GetError() << std::endl;
    }
    else
    {
        std::cout << "Create SDL window is ok" << std::endl;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::BeginScene(Color color)
{
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_RENDERER_ACCELERATED, Display::WIDTH, Display::HEIGHT);

    SDL_SetRenderTarget(renderer, texture);
    Color::SetCurrent(color);
    SDL_RenderClear(renderer);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::EndScene()
{
    SDL_SetRenderTarget(renderer, NULL);

    SDL_Rect rect = {0, 0, Frame::WIDTH, Frame::HEIGHT};

    if (texture)
    {
        SDL_RenderCopy(renderer, texture, NULL, &rect); //-V2001
    }

    SDL_RenderPresent(renderer);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void SetPositionAndSize(Frame *frame)
{
    wxSize size = {Frame::WIDTH + 9, Frame::HEIGHT + 320 };
    
    frame->SetSize(size);
    frame->SetMinSize(size);
    frame->SetMaxSize(size);

    wxRect rect = GetMaxDisplay();

    frame->SetPosition({ rect.width / 2 - size.GetWidth() / 2, rect.height / 2 - size.GetHeight() / 2 });
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static wxRect GetMaxDisplay()
{
    wxRect result = {0, 0, 0, 0};

    for (uint i = 0; i < wxDisplay::GetCount(); i++)
    {
        wxDisplay display(i);

        wxRect rect = display.GetClientArea();
        if (rect.width > result.width)
        {
            result.width = rect.width;
            result.height = rect.height;
        }
    }

    return result;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static HANDLE CreateFrame()
{
    Frame *frame = new Frame("");

    SetPositionAndSize(frame);

    wxBoxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);

    wxButton *button = new wxButton(frame, wxID_ANY, "", {10, 10}, {Frame::WIDTH, Frame::HEIGHT});
    button->SetMaxSize({Frame::WIDTH, Frame::HEIGHT});

    sizer->Add(button);

    frame->SetSizer(sizer);

    CreateButtons(frame);

    frame->Show(true);

    return button->GetHandle();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void CreateButtons(Frame *frame)
{
    // Рисуем кнопки меню и функциональные

    Key::E keys[2][5] = 
    {
        { Key::F1,       Key::F2,      Key::F3,     Key::F4,      Key::F5 },
        { Key::Function, Key::Display, Key::Memory, Key::Measure, Key::Service }
    };

    int x0 = 37;
    int y0 = Frame::HEIGHT + 10;

    int dX = 68;
    int dY = 5;

    int width = 58;
    int height = 25;

    wxSize size = {width, height};

    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            Key::E key = keys[j][i];
            CreateButton(key, frame, {x0 + (width + dX) * i, y0 + (height + dY) * j}, size, Key(key).Name());
        }
    }

    // Рисуем кнопки управления

    dX = 5;

    int x00 = 145;

    width = height = 25;
    x0 = x00 + 160;
    y0 = Frame::HEIGHT + 100;

    size.SetWidth(width);
    size.SetHeight(height);

    CreateButton(Key::Enter, frame, {x0, y0}, size, "E");
    CreateButton(Key::Left, frame, {x0 - dX - width, y0}, size, "L");
    CreateButton(Key::Right, frame, {x0 + dX + width, y0}, size, "R");
    CreateButton(Key::Up, frame, {x0, y0 - height - dY}, size, "U");
    CreateButton(Key::Down, frame, {x0, y0 + height + dY}, size, "D");

    // Кнопки времени

    width = 51;
    x0 = x00 + 5;

    y0 = Frame::HEIGHT + 100;

    size.SetWidth(width);

    CreateButton(Key::TBaseLess, frame, {x0, y0}, size, "с");
    CreateButton(Key::TBaseMore, frame, {x0 + width + dY, y0}, size, "мс");
    y0 += height + dY;
    CreateButton(Key::TShiftLess, frame, {x0, y0}, size, "<-");
    CreateButton(Key::TShiftMore, frame, {x0 + width + dY, y0}, size, "->");

    int x = x00 + 5 + (2 * width + dX) / 2 - width / 2;

    CreateButton(Key::Time, frame, {x, y0 - height - dY - height - dY}, size, "Развёртка");

    // Кнопки канала A

    int y = Frame::HEIGHT + 200;

    CreateButtonsChannel(frame, "Канал 1", 5 + x00, y, Key::ChannelA, Key::RangeLessA, Key::RangeMoreA, Key::RShiftLessA, Key::RShiftMoreA);

    // Кнопки канала B

    CreateButtonsChannel(frame, "Канал 1", 120 + x00, y, Key::ChannelB, Key::RangeLessB, Key::RangeMoreB, Key::RShiftLessB, Key::RShiftMoreB);

    CreateButtonsTrig(frame, 235 + x00, y - 130);

    CreateButton(Key::Start, frame, { 230 + x00, Frame::HEIGHT + 198 }, { 80, 25 }, "ПУСК/СТОП");
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void CreateButton(Key::E key, Frame *frame, const wxPoint &pos, const wxSize &size, pString title)
{
    wxButton *button = new wxButton(frame, (wxWindowID)key, title, pos, size);

    button->Connect((wxWindowID)key, wxEVT_LEFT_DOWN, wxCommandEventHandler(Frame::OnDown));
    button->Connect((wxWindowID)key, wxEVT_LEFT_UP, wxCommandEventHandler(Frame::OnUp));

    buttons[key] = button;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void CreateButtonsChannel(Frame *frame, const char *title, int x, int y, Key::E keyChannel, Key::E keyRangeLess, Key::E keyRangeMore, Key::E keyRShiftLess, Key::E keyRShiftMore)
{
    int width = 45;
    int height = 20;

    int dX = 5;
    int dY = 5;

    wxSize size = {width, height};

    CreateButton(keyRangeLess, frame, {x, y}, size, "мВ");
    CreateButton(keyRangeMore, frame, {x, y + height + dY}, size, "В");

    CreateButton(keyRShiftMore, frame, {x + width + 2 * dX, y}, size, "+");
    CreateButton(keyRShiftLess, frame, {x + width + 2 * dX, y + height + dY}, size, "-");

    size.SetHeight(25);
    size.SetWidth(width + width + dX * 2);

    wxPoint pos = {x, y - dY - size.GetHeight()};

    CreateButton(keyChannel, frame, pos, size, title);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void CreateButtonsTrig(Frame *frame, int x, int y)
{
    int width = 45;
    int height = 20;

    wxSize size = { width, height };

    CreateButton(Key::Trig, frame, { x, y }, size, "СИНХР");
    CreateButton(Key::TrigLevMore, frame, { x, y + 30 }, size, "больше");
    CreateButton(Key::TrigLevLess, frame, { x, y + 60 }, size, "меньше");
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Frame::OnDown(wxCommandEvent &event)
{
    Key::E key = (Key::E)event.GetId();

    std::cout << "down " << Key(key).Name() << std::endl;
    event.Skip();

    BufferButtons::Push(KeyEvent(key, TypePress::Press));

    needStartTimerLong = true;

    pressedKey = key;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Frame::OnUp(wxCommandEvent &event)
{
    Key::E key = (Key::E)event.GetId();

    std::cout << "up   " << Key(key).Name() << std::endl;
    event.Skip();

    BufferButtons::Push(KeyEvent(key, TypePress::Release));

    needStopTimerLong = true;

    pressedKey = Key::None;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Frame::OnTimerLong(wxTimerEvent&)
{
    BufferButtons::Push(KeyEvent(pressedKey, TypePress::Long));

    pressedKey = Key::None;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Frame::HandlerEvents()
{
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        SDL_PumpEvents();
        switch (event.type)
        {
        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_ESCAPE)
            {
                Close(true);
            }
            break;
        }
    }

    if (needStartTimerLong)
    {
        timerLongPress.StartOnce(500);
        needStartTimerLong = false;
    }

    if (needStopTimerLong)
    {
        timerLongPress.Stop();
        needStopTimerLong = false;
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::DrawTesterData(uint8, Color, const uint16 *, const uint8 *)
{

}
