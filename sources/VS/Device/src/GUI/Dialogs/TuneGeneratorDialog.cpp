#include "defines.h"
#include "GUI/Dialogs/TuneGeneratorDialog.h"
#include "Utils/StringUtils.h"


enum
{
    ID_FREQUENCY_0,
    ID_AMPLITUDE_0,
    ID_OFFSET_0,

    ID_FREQUENCY_1,
    ID_AMPLITUDE_1,
    ID_OFFSET_1
};


static float storeFrequency[2] = { 0.0F, 0.0F };
static float storeAmplitude[2] = { 0.0F, 0.0F };
static float storeOffset[2] = { 0.0F, 0.0F };

static wxTextCtrl *tcFrequency[2] = { nullptr, nullptr };
static wxTextCtrl *tcAmplitude[2] = { nullptr, nullptr };
static wxTextCtrl *tcOffset[2] = { nullptr, nullptr };


float TuneGeneratorDialog::frequency[2] = { 1e3F, 1e3F };
float TuneGeneratorDialog::amplitude[2] = { 1.0F, 1.0F };
float TuneGeneratorDialog::offset[2] = { 0.0F, 0.0F };


static wxPanel *CreatePanelParameters(wxDialog *dlg, int ch)
{
    wxPanel *panel = new wxPanel(dlg);

    wxString name = wxString::Format("Канал %d", ch + 1);

    new wxStaticBox(panel, wxID_ANY, name, wxDefaultPosition, { 160, 105 });

    int y = 20, x = 10, dY = 26;
    int dX = 80;

    tcFrequency[ch] = new wxTextCtrl(panel, ch == 0 ? ID_FREQUENCY_0 : ID_FREQUENCY_1, SU::DoubleToString(TuneGeneratorDialog::frequency[ch]), wxPoint(x, y), wxSize(75, 20));
    dlg->Connect(ch == 0 ? ID_FREQUENCY_0 : ID_FREQUENCY_1, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(TuneGeneratorDialog::OnChangeParameter));
    new wxStaticText(panel, wxID_ANY, wxT("Частота"), { x + dX, y + 3 });

    y += dY;
    tcAmplitude[ch] = new wxTextCtrl(panel, ch == 0 ? ID_AMPLITUDE_0 : ID_AMPLITUDE_1, SU::DoubleToString(TuneGeneratorDialog::amplitude[ch]), wxPoint(x, y), wxSize(75, 20));
    dlg->Connect(ch == 0 ? ID_AMPLITUDE_0 : ID_AMPLITUDE_1, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(TuneGeneratorDialog::OnChangeParameter));
    new wxStaticText(panel, wxID_ANY, wxT("Амплитуда"), { x + dX, y + 3 });

    y += dY;
    tcOffset[ch] = new wxTextCtrl(panel, ch == 0 ? ID_OFFSET_0 : ID_OFFSET_1, SU::DoubleToString(TuneGeneratorDialog::offset[ch]), wxPoint(x, y), wxSize(75, 20));
    dlg->Connect(ch == 0 ? ID_OFFSET_0 : ID_OFFSET_1, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(TuneGeneratorDialog::OnChangeParameter));
    new wxStaticText(panel, wxID_ANY, wxT("Смещение"), { x + dX, y + 3 });

    return panel;
}


TuneGeneratorDialog::TuneGeneratorDialog() : Dialog(wxT("Настройки генератора"))
{
    wxBoxSizer *hBox = new wxBoxSizer(wxHORIZONTAL);

    hBox->Add(CreatePanelParameters(this, 0));
    hBox->Add(CreatePanelParameters(this, 1));

    SetBoxSizer(hBox, { 320, 105 });

    std::memcpy(storeFrequency, frequency, sizeof(float) * 2);
    std::memcpy(storeAmplitude, amplitude, sizeof(float) * 2);
    std::memcpy(storeOffset, offset, sizeof(float) * 2);
}


/// Присвоить значеение параметру parameter из tc, если оно действительное
static void SetIfValid(float *parameter, wxTextCtrl *tc)
{
    double value = 0.0;
    if (SU::StringToDouble(&value, tc->GetValue()))
    {
        *parameter = static_cast<float>(value);
    }
}


void TuneGeneratorDialog::OnChangeParameter(wxCommandEvent &)
{
    SetIfValid(&frequency[0], tcFrequency[0]);

    SetIfValid(&amplitude[0], tcAmplitude[0]);

    SetIfValid(&offset[0], tcOffset[0]);

    SetIfValid(&frequency[1], tcFrequency[1]);

    SetIfValid(&amplitude[1], tcAmplitude[1]);

    SetIfValid(&offset[1], tcOffset[1]);
}


void TuneGeneratorDialog::ApplyParameters()
{

}


void TuneGeneratorDialog::CancelParameters()
{
    std::memcpy(frequency, storeFrequency, sizeof(float) * 2);
    std::memcpy(amplitude, storeAmplitude, sizeof(float) * 2);
    std::memcpy(offset, storeOffset, sizeof(float) * 2);
}
