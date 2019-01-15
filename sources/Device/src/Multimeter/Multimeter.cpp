#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "Multimeter.h"
#include "Display/Display_Primitives.h"
#include "Display/Painter.h"
#include "Settings/Settings.h"
#include "Hardware/Hardware.h"
#include "Hardware/Timer.h"
#include "Device.h"
#include <cstring>
#endif


using namespace Display::Primitives;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
char                Multimeter::buffer[11] = {0};
UART_HandleTypeDef  Multimeter::handlerUART;
/// В этот буфер записывается информация в обработчике прерывания приёма
static uint8 bufferUART[10];
/// В этом буфере готовая к выводу информация
#define SIZE_OUT 15
static char out[SIZE_OUT];

//static char recv[11] = {0};
//static char trans[11] = {0};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Multimeter::SetMeasure(const uint8 buf[10])
{
    std::memcpy(buffer, buf, 10);
    buffer[2] |= 0x30;
    buffer[3] |= 0x30;
    buffer[4] |= 0x30;
    buffer[5] |= 0x30;
    buffer[6] |= 0x30;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Multimeter::ChangeMode()
{
    std::memset(buffer, '8', 10); //-V512
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Multimeter::Init()
{
    handlerUART.Instance = USART3;
    handlerUART.Init.BaudRate = 9600;
    handlerUART.Init.WordLength = UART_WORDLENGTH_8B;
    handlerUART.Init.StopBits = UART_STOPBITS_1;
    handlerUART.Init.Parity = UART_PARITY_NONE;
    handlerUART.Init.Mode = UART_MODE_TX_RX;
    handlerUART.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    handlerUART.Init.OverSampling = UART_OVERSAMPLING_16;

    if (HAL_UART_Init(&handlerUART) != HAL_OK)
    {
        ERROR_HANDLER();
    }

    uint8 send[4] = {0x02, 'V', '0', 0x0a};

    HAL_NVIC_SetPriority(USART3_IRQn, 0, 1);
    HAL_NVIC_EnableIRQ(USART3_IRQn);

    HAL_UART_Transmit(&handlerUART, send, 4, 10);

    if (HAL_UART_Receive_IT(&handlerUART, bufferUART, 10) != HAL_OK)
    {
        ERROR_HANDLER();
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Multimeter::DeInit()
{

}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Multimeter::ChangeAVP()
{
    ChangeMode();
    char send[4] = {0x02, 'Z', MULTI_AVP == AVP::On ? '1' : '0', 0x0a};
    HAL_UART_Transmit(&handlerUART, (uint8*)send, 4, 100);
    HAL_UART_Receive_IT(&handlerUART, bufferUART, 10);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Multimeter::Update()
{
    if(Device::CurrentMode() != Device::Mode::Multimeter)
    {
        return;
    }
    
    uint8 range = 0;
    if(MULTI_MEASURE == Measure::VoltageDC)        { range = MULTI_RANGE_DC; }
    else if(MULTI_MEASURE == Measure::VoltageAC)   { range = (uint8)MULTI_RANGE_AC; }
    else if(MULTI_MEASURE == Measure::Resistance)  { range = (uint8)MULTI_RANGE_RESISTANCE; }
    else
    {
        // больше выборов нету
    }

    uint8 send[4] = {0x02, (uint8)MULTI_MEASURE.Symbol(), (uint8)(range + 0x30), 0x0a};
  //  trans[0] = (char)send[1];
  //  trans[1] = (char)send[2];
  //  trans[2] = '\0';
    HAL_UART_Transmit(&handlerUART, send, 4, 100);
    HAL_UART_Receive_IT(&handlerUART, bufferUART, 10);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Multimeter::Display::Draw()
{
    struct Func
    {
        pFuncVV func;
        Func(pFuncVV f) : func(f) {};
    };

    static const Func funcs[Multimeter::Measure::Number] =
    {
        PrepareConstantVoltage,
        PrepareVariableVoltage,
        PrepareConstantCurrent,
        PrepareVariableCurrent,
        PrepareResistance,
        PrepareTestDiode,
        PrepareRing
    };

    Painter::BeginScene(Color::BACK);

    std::memset(out, 0, SIZE_OUT);

    Measure meas = Measure::ForSymbol(buffer[7]);
    if(meas == Measure::Number)
    {
        meas = MULTI_MEASURE;
    }

    funcs[meas].func();

    //Painter::DrawBigText(30, 30, 5, out, buffer[0] == '8' ? Color::GRAY_50 : Color::FILL);


    Color::SetCurrent(Color::FILL);

    Menu::Draw();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Multimeter::Display::PrepareConstantVoltage()
{
    out[0] = buffer[1];
    out[1] = buffer[2];
    out[5] = buffer[5];
    out[6] = buffer[6];
    out[7] = ' ';
    out[8] = 'V';
    out[9] = '=';

    switch(MULTI_RANGE_DC)
    {
        case Multimeter::RangeDC::_2V:
            out[2] = '.';
            out[3] = buffer[3];
            out[4] = buffer[4];
            break;
        case Multimeter::RangeDC::_20V:
            out[2] = buffer[3];
            out[3] = '.';
            out[4] = buffer[4];
            break;
        case Multimeter::RangeDC::_500V:
            out[2] = buffer[3];
            out[3] = buffer[4];
            out[4] = '.';
            break;
    };
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Multimeter::Display::PrepareVariableVoltage()
{
    out[0] = buffer[2];
    out[4] = buffer[5];
    out[5] = buffer[6];
    out[6] = ' ';
    out[7] = 'V';
    out[8] = '~';

    switch (MULTI_RANGE_AC)
    {
        case Multimeter::RangeAC::_2V:
            out[1] = '.';
            out[2] = buffer[3];
            out[3] = buffer[4];
            break;
        case Multimeter::RangeAC::_20V:
            out[1] = buffer[3];
            out[2] = '.';
            out[3] = buffer[4];
            break;
        case Multimeter::RangeAC::_400V:
            out[1] = buffer[3];
            out[2] = buffer[4];
            out[3] = '.';
            break;
    };
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Multimeter::Display::PrepareConstantCurrent()
{
    out[0] = (char)buffer[1];
    out[1] = buffer[2];
    out[2] = '.';
    out[3] = buffer[3];
    out[4] = buffer[4];
    out[5] = buffer[5];
    out[6] = buffer[6];
    out[7] = ' ';
    out[8] = 'A';
    out[9] = '=';
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Multimeter::Display::PrepareVariableCurrent()
{
    out[0] = buffer[2];
    out[1] = '.';
    out[2] = buffer[3];
    out[3] = buffer[4];
    out[4] = buffer[5];
    out[5] = buffer[6];
    out[6] = ' ';
    out[7] = 'A';
    out[8] = '~';
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Multimeter::Display::PrepareResistance()
{
    out[0] = buffer[2];
    out[4] = buffer[5];
    out[5] = buffer[6];
    out[6] = ' ';
    out[7] = 'к';
    out[8] = 'О';
    out[9] = 'м';

    switch(MULTI_RANGE_RESISTANCE)
    {
        case Multimeter::RangeResistance::_2k:
            out[1] = '.';
            out[2] = buffer[3];
            out[3] = buffer[4];
            break;
        case Multimeter::RangeResistance::_20k:
            out[1] = buffer[3];
            out[2] = '.';
            out[3] = buffer[4];
            break;
        case Multimeter::RangeResistance::_200k:
            out[1] = buffer[3];
            out[2] = buffer[4];
            out[3] = '.';
            break;
        case Multimeter::RangeResistance::_10M:
            out[1] = buffer[3];
            out[2] = '.';
            out[3] = buffer[4];
            out[7] = 'M';
            break;
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Multimeter::Display::PrepareTestDiode()
{
    out[0] = (char)buffer[1];
    out[1] = buffer[2];
    out[2] = '.';
    out[3] = buffer[3];
    out[4] = buffer[4];
    out[5] = buffer[5];
    out[6] = buffer[6];
    out[7] = ' ';
    out[8] = 'V';
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Multimeter::Display::PrepareRing()
{
    out[0] = buffer[2];
    out[1] = '.';
    out[2] = buffer[3];
    out[3] = buffer[4];
    out[4] = buffer[5];
    out[5] = buffer[6];
    out[6] = ' ';
    out[7] = 'к';
    out[8] = 'О';
    out[9] = 'м';
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *)
{
  //  memcpy(recv, bufferUART + 1, 8);
  //  recv[9] = '\0';

    Multimeter::SetMeasure(bufferUART);

    HAL_UART_Receive_IT(&Multimeter::handlerUART, bufferUART, 10);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
    if(huart == &Multimeter::handlerUART)
    {
        huart = &Multimeter::handlerUART;
    }
}
