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
    /// Послать форму для ознакомительной отрисовки
    virtual void SendAdditionForm() = 0;

    void SetBoxSizer(wxBoxSizer *sizer, wxSize size);

    /// Создаёт панель полярности сигнала
    wxPanel *CreatePanelPolarity();
    wxRadioButton *rbPolarityDirect = nullptr;
    wxRadioButton *rbPolarityBack = nullptr;

    /// Создаёт панель уровней сигнала
    wxPanel *CreatePanelLevels();

private:
    wxBoxSizer *panelBox = nullptr;

    void OnMove(wxMoveEvent &);

    void OnButtonApply(wxCommandEvent &);

    void OnButtonCancel(wxCommandEvent &);

    void OnKeyDown(wxKeyEvent &);
};
