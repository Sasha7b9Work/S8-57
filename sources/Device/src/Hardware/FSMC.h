#pragma once


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef void (*pFuncTester)();


class Message;


class FSMC
{
public:
    static void Init();

    static void WriteToPanel(Message *message);
    /// Записать в панель команду, состоящую из двух байт
    static void WriteToPanel2bytes(uint8 byte0, uint8 byte1);

    static void WriteToFPGA8(uint8 *address, uint8 value);

    static void WriteToFPGA16(uint8 *address, uint16 value);

    static uint8 ReadFromFPGA(uint8 *address);
    /// Возвращает true, если идёт обмен с панелью
    static bool InterchangeWithPanel();
    /// Установить функцию, которую нужно выполнить единоразово после завершения обмена с панелью
    static void RunFunctionAfterInteractionWitchPanel(pFuncTester func);
private:

    static void WriteToPanel(uint8 *data, uint length);

    enum ModeFSMC
    {
        ModeNone,
        ModePanelWrite,
        ModePanelRead,
        ModeFPGA
    };

    static void ReadByte();

    static void ConfigureForFPGA();
    /// Расставляет биты значенния data по ножкам D0...D7
    static void SetOutData(uint8 data);
    /// Возвращает значение с ножек D0...D7
    static uint8 GetOutData();
    /// Возвращает состояние PAN_0, PAN_1
    static uint8 ReadPAN();

    static ModeFSMC mode;

    static bool interchangeWithPanel;
    /// Если не равно нулю, нужно выполнить эту функцию после завершения обмена с панелью
    static pFuncTester funcAfterInteractionWithPanel;
};
