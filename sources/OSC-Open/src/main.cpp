#include "Hardware/CPU.h"
#include "Panel.h"

int main()
{
    CPU::Init();
    Panel::Init();
    
    while(1)
    {
        Panel::Update();
    }
}
