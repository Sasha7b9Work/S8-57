#pragma once
#pragma warning(push, 0)
#include <wx/frame.h>
#pragma warning(pop)


class ConsoleSCPI : public wxFrame
{
public:
    static void Open(wxFrame *parent);

    static void AddLine(const wxString &line);

private:
    ConsoleSCPI(wxFrame *parent);

    void OnSize(wxSizeEvent &);
    void OnTextEnter(wxCommandEvent &);
};
