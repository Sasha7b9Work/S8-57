#include "defines.h"
#include "ConsoleSCPI.h"
#include "SCPI/SCPI.h"

#pragma warning(push, 0)
#include <wx/wx.h>
#pragma warning(pop)


enum
{
    ID_LINE
};


static wxTextCtrl *text = nullptr;
static wxTextCtrl *line = nullptr;


static ConsoleSCPI *TheConsole = nullptr;


ConsoleSCPI::ConsoleSCPI(wxFrame *parent) : wxFrame(parent, wxID_ANY, wxT("SCPI"))
{
    text = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, { 600, 300 }, wxTE_MULTILINE | wxTE_READONLY);
    AddLine("String 1");
    AddLine("������ 2");

    line = new wxTextCtrl(this, ID_LINE, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
    line->SetFocus();

    wxFont font = line->GetFont();
    font.SetWeight(wxFONTWEIGHT_BOLD);
    line->SetFont(font);
    text->SetFont(font);

    Bind(wxEVT_SIZE, &ConsoleSCPI::OnSize, this);
    line->Bind(wxEVT_TEXT_ENTER, &ConsoleSCPI::OnTextEnter, this, ID_LINE);

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


void ConsoleSCPI::OnTextEnter(wxCommandEvent &)
{
    String txt("%s\x0d", static_cast<const char *>(line->GetLineText(0).mb_str()));

    SCPI::AddNewData(txt.CString(), std::strlen(txt.CString()));

    line->Clear();
}


void ConsoleSCPI::AddLine(const wxString &str)
{
    text->WriteText(str);
    text->WriteText(wxT("\n"));
}