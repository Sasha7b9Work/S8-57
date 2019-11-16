#pragma once
#include "GUI/Dialogs/Dialog.h"


class TuneGeneratorDialog : public Dialog
{
public:
    TuneGeneratorDialog();

    void OnChangeParameter(wxCommandEvent &);

    static float frequency;
    static float amplitude;
    static float offset;

protected:
    virtual void ApplyParameters();
    virtual void CancelParameters();
};
