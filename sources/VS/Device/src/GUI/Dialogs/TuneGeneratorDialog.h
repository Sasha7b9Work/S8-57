#pragma once
#include "GUI/Dialogs/Dialog.h"


class TuneGeneratorDialog : public Dialog
{
public:
    TuneGeneratorDialog();

    void OnChangeParameter(wxCommandEvent &);

    static double frequency;
    static double amplitude;
    static double offset;

protected:
    virtual void ApplyParameters();
    virtual void CancelParameters();
};
