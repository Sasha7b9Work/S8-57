#pragma once
#pragma warning(push, 0)
#include <wx/frame.h>
#pragma warning(pop)


class ConsoleSCPI : public wxFrame
{
public:

    static ConsoleSCPI *Self();

    void SwitchVisibility();

    void AddLine(const wxString &line);
    void AddText(const wxString &text);

private:
    ConsoleSCPI(wxFrame *parent);

    void OnSize(wxSizeEvent &);
    void OnTextEnter(wxCommandEvent &);
    void OnClose(wxCloseEvent &);
};
