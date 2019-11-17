#include "defines.h"
#include "GUI/Dialogs/TuneGeneratorDialog.h"
#include "Settings/Settings.h"
#include "Application.h"
#pragma warning(push, 0)
#include <SDL.h>
#include <wx/config.h>
#include <wx/file.h>
#include <wx/fileconf.h>
#pragma warning(pop)

#undef main



extern void update();
extern void init();


#define FILE_CONFIG wxT("config.ini")


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

    locale.Init(locale.GetSystemLanguage());

    LoadSettings();

    return true;
}


int Application::OnExit()
{
    set.Save();

    SaveSettings();

    return wxApp::OnExit();
}


void Application::SaveSettings()
{
    wxString wsFile(FILE_CONFIG);
    wxFileConfig *pConfig = new wxFileConfig(wxEmptyString, wxEmptyString, wsFile, wxEmptyString, wxCONFIG_USE_LOCAL_FILE | wxCONFIG_USE_RELATIVE_PATH);
    wxConfigBase::Set(pConfig);

    pConfig->SetPath(wxT("Cenerator/A"));

    pConfig->Write(wxT("frequency"), (float)TuneGeneratorDialog::frequency);
    pConfig->Write(wxT("amplitude"), (float)TuneGeneratorDialog::amplitude);
    pConfig->Write(wxT("offset"), (float)TuneGeneratorDialog::offset);

    pConfig->SetPath(wxT("../B"));
    pConfig->Write(wxT("frequency"), (float)TuneGeneratorDialog::frequency);
    pConfig->Write(wxT("amplitude"), (float)TuneGeneratorDialog::amplitude);
    pConfig->Write(wxT("offset"), (float)TuneGeneratorDialog::offset);

    delete wxConfigBase::Set((wxConfigBase *)nullptr);
}


void Application::LoadSettings()
{
    wxString wsFile(FILE_CONFIG);   
    wxFileConfig *pConfig = new wxFileConfig(wxEmptyString, wxEmptyString, wsFile, wxEmptyString, wxCONFIG_USE_LOCAL_FILE | wxCONFIG_USE_RELATIVE_PATH);
    wxConfigBase::Set(pConfig);

    wxConfigBase *config = wxConfigBase::Get(false);

    wxString entry;
    entry = wxT("frequency");
    long index = 1;

    config->SetPath(wxT("Cenerator/A"));

    config->Read(wxT("frequency"), &TuneGeneratorDialog::frequency, TuneGeneratorDialog::frequency);
    config->Read(wxT("amplitude"), &TuneGeneratorDialog::amplitude, TuneGeneratorDialog::amplitude);
    config->Read(wxT("offset"), &TuneGeneratorDialog::offset, TuneGeneratorDialog::offset);

    config->SetPath(wxT("../B"));
    config->Read(wxT("frequency"), &TuneGeneratorDialog::frequency, TuneGeneratorDialog::frequency);
    config->Read(wxT("amplitude"), &TuneGeneratorDialog::amplitude, TuneGeneratorDialog::amplitude);
    config->Read(wxT("offset"), &TuneGeneratorDialog::offset, TuneGeneratorDialog::offset);
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
    TuneGeneratorDialog dialog;

    dialog.ShowModal();
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


