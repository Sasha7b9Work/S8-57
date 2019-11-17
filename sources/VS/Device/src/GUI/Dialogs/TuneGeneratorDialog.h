#pragma once
#include "GUI/Dialogs/Dialog.h"


class TuneGeneratorDialog : public Dialog
{
public:
    TuneGeneratorDialog();

    void OnChangeParameter(wxCommandEvent &);

    static float frequency[2];
    static float amplitude[2];
    static float offset[2];

protected:
    virtual void ApplyParameters();
    virtual void CancelParameters();
};
