#include "defines.h"
#include "Transceiver.h"
#include "Decoder_p.h"
#include "Hardware/Timer.h"
#include <cstring>


/// Удалить из буфера переданные данные
void DiscardTransmittedData();
/// Инициализировать шину данных на приём
void InitDataPinsOnRecive();
/// Инициализировать шину данных на передачу
void InitDataPinsOnTransmit();
/// Засылает данные в устройство, если таковые имеются
void TransmitData();
/// Установить данные на шину
void SetData(uint8 data);


static uint8 buffer[1024];

static uint _bytesInBuffer = 0;


void DeInitPins()
{
    DeInit_FL0();

    Receiver::InitDataPins();
}


void Receiver::InitDataPins()
{
    GPIO_InitTypeDef gpio;
    gpio.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;   // D0...D7
    gpio.Mode = GPIO_MODE_INPUT;
    gpio.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOE, &gpio);
}


void InitDataPins()
{
    GPIO_InitTypeDef gpio;
    gpio.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;   // D0...D7
    gpio.Mode = GPIO_MODE_OUTPUT_PP;
    HAL_GPIO_Init(GPIOE, &gpio);
}


void Transceiver::Send(const uint8 *data, uint size)
{
    std::memcpy(&buffer[_bytesInBuffer], data, size);
    _bytesInBuffer += size;
}


bool Transceiver::Receive()
{


//    if (mode == ModeDevice::Send)
//    {
//        PDecoder::AddData((uint8)GPIOE->IDR);       // Читаем и обрабатываем байт данных
//        
//        PORT_READY->BSRR = PIN_READY;               // Устанавливаем признак, что данные приняты - "1" на READY
//        
//        while (PORT_MODE1->IDR & PIN_MODE1) {};     // Ждём сигнал подтверждения. Пока MODE1 находтися в "1" - устройство не дождалось подтверждения //-V712
//
//        PORT_READY->BSRR = (uint)PIN_READY << 16U;  // И убираем сигнал готовности, устанавливая READY в "0"
//
//        return true;
//    }

    return false;
}


void TransmitData()
{

}


void DiscardTransmittedData()
{
    if (_bytesInBuffer > 0)
    {
        _bytesInBuffer--;                        // Удаляем переданные данные из буфера

        std::memmove(&buffer[0], &buffer[1], _bytesInBuffer);
    }
}


void Set_READY(State::E state)
{
    HAL_GPIO_WritePin(READY, (state == State::Active) ? GPIO_PIN_SET : GPIO_PIN_RESET);
}


void Set_FL0(State::E state)
{
    HAL_GPIO_WritePin(FL0, (state == State::Active) ? GPIO_PIN_SET : GPIO_PIN_RESET);
}


void SetData(uint8 data)
{
    static const uint16 pins[8] = {GPIO_PIN_0, GPIO_PIN_1, GPIO_PIN_2, GPIO_PIN_3, GPIO_PIN_4, GPIO_PIN_5, GPIO_PIN_6, GPIO_PIN_7};

    for (int i = 0; i < 8; i++)
    {
        HAL_GPIO_WritePin(GPIOE, pins[i], static_cast<GPIO_PinState>(data & 0x01));
        data >>= 1;
    }
}


void Init_FL0_OUT()
{
    GPIO_InitTypeDef gpio;
    gpio.Pin = PIN_FL0;
    gpio.Mode = GPIO_MODE_OUTPUT_PP;
    HAL_GPIO_Init(PORT_FL0, &gpio);
}


void DeInit_FL0()
{
    GPIO_InitTypeDef gpio;
    gpio.Pin = PIN_FL0;
    gpio.Mode = GPIO_MODE_INPUT;
    gpio.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(PORT_FL0, &gpio);     // FL0 инициализируем на прослушивание - чтобы не мешать работе шины
}


ModeDevice::E Mode_Device()
{
    //                  MODE    0  1
    static const ModeDevice::E modes [2][2] =
    {
        {ModeDevice::Disabled, ModeDevice::Send},
        {ModeDevice::Receive,  ModeDevice::Forbidden}
    };

    //int m0 = HAL_GPIO_ReadPin(MODE0);
    int m0 = (PORT_MODE0->IDR & PIN_MODE0) ? 1 : 0;
    
    //int m1 = HAL_GPIO_ReadPin(MODE1);
    int m1 = (PORT_MODE1->IDR & PIN_MODE1) ? 1 : 0;

    return modes[m0][m1];
}
