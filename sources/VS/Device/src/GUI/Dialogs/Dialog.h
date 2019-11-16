#pragma once
#pragma warning(push, 0)
#include <wx/wx.h>
#pragma warning(pop)


class Dialog : public wxDialog
{
public:
    Dialog(const wxString &title);
    virtual ~Dialog();

    void OnControlEvent(wxCommandEvent &);

    static const int WIDTH_PANEL = 220;

protected:

    void SetBoxSizer(wxBoxSizer *sizer, wxSize size);

private:
    wxBoxSizer *panelBox = nullptr;

    void OnMove(wxMoveEvent &);

    void OnButtonApply(wxCommandEvent &);

    void OnButtonCancel(wxCommandEvent &);

    void OnKeyDown(wxKeyEvent &);
};
