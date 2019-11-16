#include "defines.h"
#include "GUI/Dialogs/TuneGeneratorDialog.h"
#include "Utils/StringUtils.h"


enum
{
    ID_FREQUENCY,
    ID_AMPLITUDE,
    ID_OFFSET
};


static float oldFrequency = 1e3F;
static float oldAmplitude = 1.0F;
static float oldOffset = 0.0F;


static wxTextCtrl *tcFrequency = nullptr;
static wxTextCtrl *tcAmplitude = nullptr;
static wxTextCtrl *tcOffset = nullptr;


float TuneGeneratorDialog::frequency = oldFrequency;
float TuneGeneratorDialog::amplitude = oldAmplitude;
float TuneGeneratorDialog::offset = oldOffset;


static wxPanel *CreatePanelParameters(wxDialog *dlg)
{
    wxPanel *panel = new wxPanel(dlg);

    int y = 10, x = 10, dY = 26;
    int dX = 80;

    tcFrequency = new wxTextCtrl(panel, ID_FREQUENCY, SU::DoubleToString(oldFrequency), wxPoint(x, y), wxSize(75, 20));
    dlg->Connect(ID_FREQUENCY, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(TuneGeneratorDialog::OnChangeParameter));
    new wxStaticText(panel, wxID_ANY, wxT("Частота"), { x + dX, y + 3 });

    y += dY;
    tcAmplitude = new wxTextCtrl(panel, ID_AMPLITUDE, SU::DoubleToString(oldAmplitude), wxPoint(x, y), wxSize(75, 20));
    dlg->Connect(ID_AMPLITUDE, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(TuneGeneratorDialog::OnChangeParameter));
    new wxStaticText(panel, wxID_ANY, wxT("Амплитуда"), { x + dX, y + 3 });

    y += dY;
    tcOffset = new wxTextCtrl(panel, ID_OFFSET, SU::DoubleToString(oldOffset), wxPoint(x, y), wxSize(75, 20));
    dlg->Connect(ID_OFFSET, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(TuneGeneratorDialog::OnChangeParameter));
    new wxStaticText(panel, wxID_ANY, wxT("Смещение"), { x + dX, y + 3 });

    return panel;
}


TuneGeneratorDialog::TuneGeneratorDialog() : Dialog(wxT("Настройки генератора"))
{
    wxBoxSizer *vBox = new wxBoxSizer(wxVERTICAL);

    vBox->Add(CreatePanelParameters(this));

    SetBoxSizer(vBox, { 175, 80 });
}


/// Присвоить значеение параметру parameter из tc, если оно действительное
static void SetIfValid(float *parameter, wxTextCtrl *tc)
{
    double value = SU::StringToDouble(tc->GetValue());
    if (value != SU::ERROR_VALUE_DOUBLE)
    {
        *parameter = static_cast<float>(value);
    }
}


void TuneGeneratorDialog::OnChangeParameter(wxCommandEvent &)
{
    oldFrequency = frequency;
    oldAmplitude = amplitude;
    oldOffset = offset;

    SetIfValid(&frequency, tcFrequency);

    SetIfValid(&amplitude, tcAmplitude);

    SetIfValid(&offset, tcOffset);
}


void TuneGeneratorDialog::ApplyParameters()
{

}


void TuneGeneratorDialog::CancelParameters()
{
    frequency = oldFrequency;
    amplitude = oldAmplitude;
    offset = oldOffset;
}
