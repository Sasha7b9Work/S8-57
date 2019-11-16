#include "defines.h"
#include "GUI/Dialogs/Dialog.h"


enum
{
    ID_SPINCTRL_DONW,
    ID_SPINCTRL_UP,
    ID_BUTTON_OK,
    ID_BUTTON_CANCEL,
    ID_RADIOBUTTON_DIRECT,
    ID_RADIOBUTTON_BACK
};


Dialog::Dialog(const wxString &title) : wxDialog(nullptr, wxID_ANY, title)
{
    Connect(wxEVT_MOVE, wxMoveEventHandler(Dialog::OnMove));

    wxButton *btnOk = new wxButton(this, ID_BUTTON_OK, wxT("Принять"), wxDefaultPosition, BUTTON_SIZE);
    Connect(ID_BUTTON_OK, wxEVT_BUTTON, wxCommandEventHandler(Dialog::OnButtonApply));
    wxButton *btnCancel = new wxButton(this, ID_BUTTON_CANCEL, wxT("Отменить"), wxDefaultPosition, BUTTON_SIZE);
    Connect(ID_BUTTON_CANCEL, wxEVT_BUTTON, wxCommandEventHandler(Dialog::OnButtonCancel));

    Bind(wxEVT_KEY_DOWN, &Dialog::OnKeyDown, this);
    Bind(wxEVT_KEY_UP, &Dialog::OnKeyDown, this);

    Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(Dialog::OnKeyDown));
    Connect(wxEVT_KEY_UP, wxKeyEventHandler(Dialog::OnKeyDown));

    wxBoxSizer *vBox = new wxBoxSizer(wxVERTICAL);
    panelBox = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *hBox = new wxBoxSizer(wxHORIZONTAL);

    vBox->Add(panelBox);
    hBox->Add(btnOk, 1, wxALIGN_CENTER);
    hBox->AddSpacer(20);
    hBox->Add(btnCancel, 1, wxALIGN_CENTER);
    vBox->AddSpacer(10);
    vBox->Add(hBox, 0, wxALIGN_CENTER);

    SetSizer(vBox);
}


Dialog::~Dialog()
{
}


wxPanel *Dialog::CreatePanelLevels()
{
    wxPanel *panel = new wxPanel(this);

    new wxStaticBox(panel, wxID_ANY, wxT("Уровни"), wxDefaultPosition, wxSize(130, 75));

    return panel;
}


wxPanel *Dialog::CreatePanelPolarity() 
{
    wxPanel *panel = new wxPanel(this);
    new wxStaticBox(panel, wxID_ANY, wxT("Полярность"), wxDefaultPosition, wxSize(90, 75));

    int y = 25, x = 5;

    rbPolarityDirect = new wxRadioButton(panel, ID_RADIOBUTTON_DIRECT, wxT("Прямая"), wxPoint(x, y));
    Connect(ID_RADIOBUTTON_DIRECT, wxEVT_RADIOBUTTON, wxCommandEventHandler(Dialog::OnControlEvent));
    rbPolarityDirect->SetValue(true);

    rbPolarityBack = new wxRadioButton(panel, ID_RADIOBUTTON_BACK, wxT("Обратная"), wxPoint(x, y + 25));
    Connect(ID_RADIOBUTTON_BACK, wxEVT_RADIOBUTTON, wxCommandEventHandler(Dialog::OnControlEvent));

    return panel;
}


void Dialog::SetBoxSizer(wxBoxSizer *sizer, wxSize size)
{
    panelBox->Add(sizer);

    size.y += 50;

    SetClientSize(size);

    Centre();

    SendAdditionForm();

    SetFocus();
}


void Dialog::OnMove(wxMoveEvent &)
{
}


void Dialog::OnButtonApply(wxCommandEvent &)
{
    Destroy();
}


void Dialog::OnButtonCancel(wxCommandEvent &)
{
    Destroy();
}


void Dialog::OnControlEvent(wxCommandEvent &)
{
    SendAdditionForm();
}


void Dialog::OnKeyDown(wxKeyEvent &)
{
}
