#include "defines.h"
#include "GUI/Dialogs/TesterDialog.h"
#include "GUI/ConsoleSCPI.h"


enum
{
    ID_BUTTON_BEGIN,
    ID_BUTTON_END
};


TesterDialog::TesterDialog() : wxDialog(nullptr, wxID_ANY, wxT("Тестер"))
{
    wxButton *btnBegin = new wxButton(this, ID_BUTTON_BEGIN, wxT("Начать"));
    Connect(ID_BUTTON_BEGIN, wxEVT_BUTTON, wxCommandEventHandler(TesterDialog::OnButtonBegin));

    wxButton *btnEnd = new wxButton(this, ID_BUTTON_END, wxT("Завершить"));
    Connect(ID_BUTTON_END, wxEVT_BUTTON, wxCommandEventHandler(TesterDialog::OnButtonEnd));

    wxBoxSizer *hBox = new wxBoxSizer(wxHORIZONTAL);
    hBox->Add(btnBegin);
    hBox->Add(btnEnd);
    SetSizer(hBox);
}


void TesterDialog::OnButtonBegin(wxCommandEvent &)
{
    ConsoleSCPI::Self()->StartTest();
}


void TesterDialog::OnButtonEnd(wxCommandEvent &)
{
    ConsoleSCPI::Self()->StopTest();
}
