#pragma once
#pragma warning(push, 0)
#include <wx/wx.h>
#pragma warning(pop)


class TesterDialog : public wxDialog
{
public:
    TesterDialog();

private:
    void OnButtonBegin(wxCommandEvent &);
    void OnButtonEnd(wxCommandEvent &);
};
