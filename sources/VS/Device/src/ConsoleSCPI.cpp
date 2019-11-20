#include "defines.h"
#include "ConsoleSCPI.h"
#pragma warning(push, 0)
#include <wx/wx.h>
#pragma warning(pop)


static wxTextCtrl *text = nullptr;
static wxTextCtrl *line = nullptr;


static ConsoleSCPI *console = nullptr;



ConsoleSCPI::ConsoleSCPI() : wxFrame(nullptr, wxID_ANY, wxT("SCPI"))
{
    text = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, { 600, 300 }, wxTE_MULTILINE | wxTE_READONLY);
    line = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize);
    line->SetFocus();

    Bind(wxEVT_SIZE, &ConsoleSCPI::OnSize, this);

    Show();
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
