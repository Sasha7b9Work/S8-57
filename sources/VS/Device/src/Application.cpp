#include "defines.h"
#include "Settings/Settings.h"

#pragma warning(push)
#pragma warning(disable:4018 4189 4355 4365 4459 4571 4625 4668 4774 5026 5027)
#include "Application.h"
#include <SDL.h>
#pragma warning(pop)

#undef main



extern void update();
extern void init();



enum
{
    FILE_SIZE = wxID_HIGHEST + 1,
    FILE_QUIT = wxID_EXIT,
    GENERATOR_TUNE
};

enum
{
    TIMER_ID = 10,
    TIMER_LONG_ID
};


wxIMPLEMENT_APP_NO_MAIN(Application);


int main(int argc, char **argv)
{
    setlocale(LC_ALL, "Russian");

    FreeConsole();

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0U) //-V2517
    {
        LOG_ERROR("SDL_Init Error: %s", SDL_GetError());
    }

    return wxEntry(argc, argv);
}



bool Application::OnInit()
{
    if (!wxApp::OnInit())
    {
        return false;
    }

    init();

    return true;
}


int Application::OnExit()
{
    set.Save();

    return wxApp::OnExit();
}



Frame::Frame(const wxString& title)
    : wxFrame(NULL, wxID_ANY, title)
{
    SetIcon(wxICON(sample));

    wxMenu *fileMenu = new wxMenu;
    wxMenu *generatorMenu = new wxMenu;

    //fileMenu->Append(File_Size, "&Size", "Resize screen");

    fileMenu->Append(FILE_QUIT, "Выход\tAlt-X", "Закрывает окно программы");

    generatorMenu->Append(GENERATOR_TUNE, "Настроить");

    wxMenuBar *menuBar = new wxMenuBar();
    menuBar->Append(fileMenu, "Файл");
    menuBar->Append(generatorMenu, "Генератор");

    SetMenuBar(menuBar);

    CreateStatusBar(2);
    SetStatusText("Welcome to wxWidgets!");

    Bind(wxEVT_MENU, &Frame::OnSize, this, FILE_SIZE);
    Bind(wxEVT_MENU, &Frame::OnQuit, this, FILE_QUIT);
    Bind(wxEVT_MENU, &Frame::OnGeneratorTune, this, GENERATOR_TUNE);
    Bind(wxEVT_TIMER, &Frame::OnTimer, this, TIMER_ID);
    Bind(wxEVT_TIMER, &Frame::OnTimerLong, this, TIMER_LONG_ID);

    timer.SetOwner(this, TIMER_ID);

    timer.Start(0);

    timerLongPress.SetOwner(this, TIMER_LONG_ID);
}


void Frame::OnTimer(wxTimerEvent&)
{
    update();

    HandlerEvents();

    DrawFPS();
}


void Frame::DrawFPS()
{
    static int count = 0;
    static unsigned int prevTime = 0;

    count++;

    if (SDL_GetTicks() - prevTime > 1000)
    {
        float fps = static_cast<float>(count) / (SDL_GetTicks() - prevTime) * 1000.0F;

        char buffer[100];
        sprintf(buffer, "fps %f", fps);

        SetStatusText(buffer);

        prevTime = SDL_GetTicks();
        count = 0;
    }
}


void Frame::OnSize(wxCommandEvent&)
{

}


void Frame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    Close(true);
}


void Frame::OnGeneratorTune(wxCommandEvent &)
{

}


void Frame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
    wxMessageBox(wxString::Format
    (
        "Welcome to %s!\n"
        "\n"
        "This is the minimal wxWidgets sample\n"
        "running under %s.",
        wxVERSION_STRING,
        wxGetOsDescription()
    ),
        "About wxWidgets minimal sample",
        wxOK | wxICON_INFORMATION,
        this);
}
