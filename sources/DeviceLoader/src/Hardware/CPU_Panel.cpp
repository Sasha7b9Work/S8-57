#include <usbh_core.h>
#include "Hardware/Controls.h"
#include "Display/Display.h"
#include "Hardware/CPU.h"
#include "Hardware/Timer.h"
#include <stm32f4xx_hal.h>
#include <string.h>


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define MAX_DATA            20

#define LED_CHANA_ENABLE    129
#define LED_CHANA_DISABLE   1
#define LED_CHANB_ENABLE    130
#define LED_CHANB_DISABLE   2
#define LED_TRIG_ENABLE     131
#define LED_TRIG_DISABLE    3
#define POWER_OFF           4

static uint16 firstPos = 0;
static uint16 lastPos = 0;
static uint16 dataTransmitted[MAX_DATA] = {0x00};
volatile static bool isRunning = true;
static PanelButton pressedButton = B_Empty;

static SPI_HandleTypeDef handleSPI =
{
    SPI1,
    {
        SPI_MODE_SLAVE,                 // Init.Mode    
        SPI_DIRECTION_2LINES,           // Init.Direction
        SPI_DATASIZE_8BIT,              // Init.DataSize
        SPI_POLARITY_HIGH,              // Init.CLKPolarity
        SPI_PHASE_1EDGE,                // Init.CLKPhase
        SPI_NSS_SOFT,                   // Init.NSS
        SPI_BAUDRATEPRESCALER_2,        // Init.BaudRatePrescaler
        SPI_FIRSTBIT_MSB,               // Init.FirstBit
        SPI_TIMODE_DISABLED,            // Init.TIMode
        SPI_CRCCALCULATION_DISABLED,    // Init.CRCCalculation
        7                               // InitCRCPolynomial
    },
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, HAL_UNLOCKED, HAL_SPI_STATE_RESET, 0
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static PanelButton ButtonIsPress(uint16 command)
{
    PanelButton button = B_Empty;

    static uint timePrevPressButton = 0;

    if (command < (B_NumButtons | 0x80) && command > (B_Empty | 0x80))
    {
        if(TIME_MS - timePrevPressButton > 100)
        {
            button = (PanelButton)(command & 0x7f);
            timePrevPressButton = TIME_MS;
        }
    }

    return button;
}


bool CPU::Panel::ProcessingCommandFromPIC(uint16 command)
{
    if (command != 0)
    {
        pressedButton = ButtonIsPress(command);
    }

    return true;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void CPU::Panel::TransmitData(uint16 data)
{
    if(lastPos == MAX_DATA)
    {
    }
    else
    {
        if (data == 0x04)
        {
            uint time = TIME_MS;
            while (TIME_MS - time < 200) {};
        }
        dataTransmitted[lastPos++] = data;
    }
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
uint16 CPU::Panel::NextData()
{
    if (lastPos > 0)
    {
        uint16 data = dataTransmitted[firstPos++];
        if(firstPos == lastPos)
        {
            firstPos = lastPos = 0;
        }
        return data;
    }
    return 0;
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
void CPU::Panel::Disable()
{
    isRunning = false;
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
void CPU::Panel::Enable()
{
    isRunning = true;
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
/*
    SPI1
    56  - PG0 - программный NSS 
    41  - PA5 - SCK
    42  - PA6 - MISO
    135 - PB5 - MOSI
*/

//----------------------------------------------------------------------------------------------------------------------------------------------------
static uint8 dataSPIfromPanel;

//----------------------------------------------------------------------------------------------------------------------------------------------------
void HAL_GPIO_EXTI_Callback(uint16_t pin)
{
    // Прерывание на SPI от панели управления
    if (pin == GPIO_PIN_6)
    {
        HAL_SPI_Receive_IT(&handleSPI, &dataSPIfromPanel, 1);
    }
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef* hSPI)
{
    if (!CPU::Panel::ProcessingCommandFromPIC(dataSPIfromPanel))
    {
        HAL_SPI_DeInit(hSPI);
        HAL_SPI_Init(hSPI);
    }

    SPI1->DR = 0;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
PanelButton CPU::Panel::PressedButton()
{
    return pressedButton;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void CPU::Panel::SPI_IRQHandler()
{
    HAL_SPI_IRQHandler(&handleSPI);
}
