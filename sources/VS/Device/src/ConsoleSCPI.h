#pragma once
#pragma warning(push, 0)
#include <wx/frame.h>
#pragma warning(pop)


class ConsoleSCPI : public wxFrame
{
public:
    ConsoleSCPI();

private:
    void OnSize(wxSizeEvent &);
};
