#include "defines.h"
#include "ConsoleSCPI.h"
#pragma warning(push, 0)
#include <wx/wx.h>
#pragma warning(pop)


static wxTextCtrl *text = nullptr;
static wxTextCtrl *line = nullptr;


static ConsoleSCPI *TheConsole = nullptr;


ConsoleSCPI::ConsoleSCPI(wxFrame *parent) : wxFrame(parent, wxID_ANY, wxT("SCPI"))
{
    text = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, { 600, 300 }, wxTE_MULTILINE | wxTE_READONLY);
    line = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize);
    line->SetFocus();

    Bind(wxEVT_SIZE, &ConsoleSCPI::OnSize, this);

    Show();

    TheConsole = this;
}


void ConsoleSCPI::OnSize(wxSizeEvent &)
{
    wxPoint clientOrigin = GetClientAreaOrigin();

    wxSize clientSize = GetClientSize();

    wxSize sizeText = clientSize;
    sizeText.y -= line->GetSize().y;

    text->SetSize(sizeText);

    line->SetPosition({ clientOrigin.x, clientSize.y - line->GetSize().y });
    line->SetSize({ text->GetSize().x, line->GetSize().y });
}


void ConsoleSCPI::Open(wxFrame *parent)
{
    if (TheConsole)
    {
        if (TheConsole->IsShown())
        {
            TheConsole->Hide();
            text->Clear();
            line->Clear();
        }
        else
        {
            TheConsole->Show();
        }
    }
    else
    {
        TheConsole = new ConsoleSCPI(parent);
    }
}
