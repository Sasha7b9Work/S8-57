#include "defines.h"
#include "Keyboard.h"
#include "Hardware/Controls.h"
#include "Display/Painter.h"
#include "Hardware/stm32/Timer7XX.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Время последнего нажатия кнопки
static uint timeLastPress = 0;
static int pointer = 0;
static StructControl commands[10];
static void(*callbackKeyboard)() = 0;
static TIM_HandleTypeDef handleTIM4;
#define TIME_UPDATE 2
static uint8 TS_flag = 0;
TS_StateTypeDef TS_state = { 0 };




struct StructButton
{
    const char *title;
    Control control;
    Color color;
    int x;
    int y;
};
static int x0 = 1;
static int y0 = 243;
static int x1 = 323;
static int y1 = 0;

static int selX = 0; // Если кнопка нажата, то
static int selY = 0; // здесь её координаты

#define DELTA_BTN   2

#define WIDTH_BTN1  62
#define HEIGHT_BTN1 27
#define ROWS1       2
#define COLS1       5

#define WIDTH_BTN2  61
#define HEIGHT_BTN2 31
#define ROWS2       9
#define COLS2       3

static StructButton strBtn1[ROWS1][COLS1] =
{
    {{"F1",      B_1},        {"F2",      B_2},       {"F3",     B_3},       {"F4",        B_4},        {"F5",     B_5}},
    {{"ФУНКЦИЯ", K_Function}, {"ДИСПЛЕЙ", K_Display}, {"СЕРВИС", K_Service}, {"ИЗМЕРЕНИЯ", K_Measures}, {"ПАМЯТЬ", K_Memory}}
};

static const Color green = Color::GREEN_25;
static const Color blue = Color::BLUE_25;
static const Color red = Color::RED_25;

static StructButton bNone        = {"",          K_None };
static StructButton bTBaseMore   = {"нС",        B_TBaseMore,   green };
static StructButton bTBaseLess   = {"С",         B_TBaseLess,   green };
static StructButton bTime        = {"РАЗВЕРТКА", K_Time,        Color::GRAY_25 };
static StructButton bTShiftLess  = {"Вр влево",  B_TShiftLess,  green };
static StructButton bTShiftMore  = {"Вр вправо", B_TShiftMore,  green };
static StructButton bStart       = {"ПУСК/СТОП", K_Start,       Color::GRAY_25};
static StructButton bChannelA    = {"КАНАЛ 1",   K_ChannelA,    Color::GRAY_25 };
static StructButton bRangeLessA  = {"мВ",        B_RangeLessA,  red };
static StructButton bRangeMoreA  = {"В",         K_RangeMoreA,  red };
static StructButton bRShiftMoreA = {"Вверх",     B_RShiftMoreA, red };
static StructButton bRShiftLessA = {"Вниз",      B_RShiftLessA, red };
static StructButton bChannelB    = {"КАНАЛ 2",   K_ChannelB,    Color::GRAY_25 };
static StructButton bRangeLessB  = {"мВ",        B_RangeLessB,  red };
static StructButton bRangeMoreB  = {"В",         B_RangeMoreB,  red };
static StructButton bRShiftMoreB = {"Вверх",     B_RShiftMoreB, red };
static StructButton bRShiftLessB = {"Вниз",      B_RShiftLessB, red };
static StructButton bTrig        = {"Синхр",     K_Trig,        Color::GRAY_25 };
static StructButton bTrigLevMore = {"С вверх",   B_TrigLevMore, blue };
static StructButton bTrigLevLess = {"С вниз",    B_TrigLevLess, blue };

static StructButton strBtn2[ROWS2][COLS2] =
{
    {bChannelA,    bTrig,        bChannelB},
    {bRangeMoreA,  bTrigLevMore, bRangeMoreB},
    {bRangeLessA,  bTrigLevLess, bRangeLessB},
    {bRShiftMoreA, bTime,        bRShiftMoreB},
    {bRShiftLessA, bTBaseMore,   bRShiftLessB},
    {bTShiftLess,  bTBaseLess,   bTShiftMore},
    {bNone,        {"Вверх", B_Up,    blue}, bNone},
    {{"Влево", B_Left,  blue}, {"Ввод",  B_Enter, blue}, {"Вправо", B_Right,  blue}},
    {bNone,        {"Вниз",  B_Down,  blue}, bStart}
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Keyboard::Init()
{
    SetCallback(&Update);

    HAL_NVIC_SetPriority(TIM4_IRQn, 0, 1);

    HAL_NVIC_EnableIRQ(TIM4_IRQn);

    handleTIM4.Instance = TIM4;
    handleTIM4.Init.Period = TIME_UPDATE * 10 - 1;
    handleTIM4.Init.Prescaler = (uint)((SystemCoreClock / 2) / 10000) - 1;
    handleTIM4.Init.ClockDivision = 0;
    handleTIM4.Init.CounterMode = TIM_COUNTERMODE_UP;

    HAL_TIM_Base_Init(&handleTIM4);

    HAL_TIM_Base_Start_IT(&handleTIM4);

    TOUCH::Init();

    for (int i = 0; i < ROWS1; i++)
    {
        for (int j = 0; j < COLS1; j++)
        {
            StructButton &button = strBtn1[i][j];
            button.x = x0 + j * (WIDTH_BTN1 + DELTA_BTN);
            button.y = y0 + i * (HEIGHT_BTN1 + DELTA_BTN);
            button.color = i == 0 ? Color::GRAY_10 : Color::GRAY_25;
        }
    }

    for (int i = 0; i < ROWS2; i++)
    {
        for (int j = 0; j < COLS2; j++)
        {
            strBtn2[i][j].x = x1 + j * (WIDTH_BTN2 + DELTA_BTN);
            int y = y1 + i * (HEIGHT_BTN2 + DELTA_BTN);
            if (i > 5)
            {
                y += 4;
            }
            strBtn2[i][j].y = y;
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Keyboard::Draw()
{
    for (int i = 0; i < ROWS1; i++)
    {
        for (int j = 0; j < COLS1; j++)
        {
            const char *title = strBtn1[i][j].title;
            if (title[0])
            {
                DrawButton(strBtn1[i][j].x, strBtn1[i][j].y, WIDTH_BTN1, HEIGHT_BTN1, strBtn1[i][j].color, title);
            }
        }
    }

    for (int i = 0; i < ROWS2; i++)
    {
        for (int j = 0; j < COLS2; j++)
        {
            const char *title = strBtn2[i][j].title;
            if (title[0])
            {
                DrawButton(strBtn2[i][j].x, strBtn2[i][j].y, WIDTH_BTN2, HEIGHT_BTN2, strBtn2[i][j].color, title);
            }
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Keyboard::DrawButton(int x, int y, int width, int height, Color color, const char *title)
{
    if (selX == x && selY == y)
    {
        Painter::FillRegion(x, y, width, height, Color::FILL);
        Painter::DrawStringInCenterRect(x, y, width, height, title, Color::BACK);
    }
    else
    {
        Painter::DrawRectangle(x, y, width, height, Color::FILL);
        Painter::FillRegion(x + 1, y + 1, width - 2, height - 2, color);
        if (title)
        {
            Painter::DrawStringInCenterRect(x, y, width, height, title, Color::FILL);
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Keyboard::SetCallback(void(*func)())
{
    callbackKeyboard = func;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Keyboard::Update()
{
    if (TIME_MS - timeLastPress < 20)
    {
        return;
    }

    if (TS_flag == 1)
    {
        TOUCH::GetState(&TS_state);
        if (TS_state.touchDetected != 0)
        {
            int x = TS_state.touchX[0] >> 1;
            int y = TS_state.touchY[0] >> 1;

            for (int i = 0; i < ROWS1; i++)
            {
                for (int j = 0; j < COLS1; j++)
                {
                    if (x > strBtn1[i][j].x && x < (strBtn1[i][j].x + WIDTH_BTN1) &&
                        y > strBtn1[i][j].y && y < (strBtn1[i][j].y + HEIGHT_BTN1))
                    {
                        selX = strBtn1[i][j].x;
                        selY = strBtn1[i][j].y;
                        timeLastPress = TIME_MS;
                    }
                }
            }
            for (int i = 0; i < ROWS2; i++)
            {
                for (int j = 0; j < COLS2; j++)
                {
                    if (x > strBtn2[i][j].x && x < (strBtn2[i][j].x + WIDTH_BTN2) &&
                        y > strBtn2[i][j].y && y < (strBtn2[i][j].y + HEIGHT_BTN2))
                    {
                        selX = strBtn2[i][j].x;
                        selY = strBtn2[i][j].y;
                        timeLastPress = TIME_MS;
                    }
                }
            }
        }
        else
        {
            for (int i = 0; i < ROWS1; i++)
            {
                for (int j = 0; j < COLS1; j++)
                {
                    if (selX == strBtn1[i][j].x && selY == strBtn1[i][j].y)
                    {
                        FillCommand(strBtn1[i][j].control, Release);
                        selX = -1;
                    }
                }
            }
            for (int i = 0; i < ROWS2; i++)
            {
                for (int j = 0; j < COLS2; j++)
                {
                    if (selX == strBtn2[i][j].x && selY == strBtn2[i][j].y)
                    {
                        FillCommand(strBtn2[i][j].control, Release);
                        selX = -1;
                    }
                }
            }
        }

        TS_flag = 0;
    }
    else if (selX != -1)
    {
        for (int i = 0; i < ROWS1; i++)
        {
            for (int j = 0; j < COLS1; j++)
            {
                if (selX == strBtn1[i][j].x && selY == strBtn1[i][j].y)
                {
                    FillCommand(strBtn1[i][j].control, Release);
                    selX = -1;
                }
            }
        }
    }
    else
    {
        selX = -1;
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Keyboard::FillCommand(Control control, TypePress typePress)
{
    commands[pointer].key = control;
    commands[pointer++].typePress = typePress;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Keyboard::TOUCH::Init()
{
    // Инициализируем I2C
    GPIO_InitTypeDef GPIO_InitStruct;

    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;

    GPIO_InitStruct.Pin = SDA_GPIO_PIN;
    HAL_GPIO_Init(SDA_GPIO_PORT, &GPIO_InitStruct);
    HAL_GPIO_WritePin(SDA_GPIO_PORT, SDA_GPIO_PIN, GPIO_PIN_SET);

    GPIO_InitStruct.Pin = SCL_GPIO_PIN;
    HAL_GPIO_Init(SCL_GPIO_PORT, &GPIO_InitStruct);
    HAL_GPIO_WritePin(SCL_GPIO_PORT, SCL_GPIO_PIN, GPIO_PIN_SET);

    // Инициализируем GT811

    /*Configure GPIO pin : PtPin */
    GPIO_InitStruct.Pin = LCD_INT_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    /* EXTI interrupt init*/
    HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

    /* reset GT811 */
    HAL_GPIO_WritePin(LCD_RST_GPIO_Port, LCD_RST_Pin, GPIO_PIN_RESET);
    HAL_Delay(200);
    HAL_GPIO_WritePin(LCD_RST_GPIO_Port, LCD_RST_Pin, GPIO_PIN_SET);
    HAL_Delay(200);

    /* if Version is correct, send the configuration parameters */
    if (GT811_ReadID() == GT811_VERSION_VALUE)
    {
        /* touch screen configuration parameter (touch screen manufacturers provide) */
        uint8_t GTP_CFG_DATA[] =
        {
            0x12,0x10,0x0E,0x0C,0x0A,0x08,0x06,0x04,0x02,0x00,0x05,0x55,0x15,0x55,0x25,0x55,0x35,0x55,0x45,0x55,
            0x55,0x55,0x65,0x55,0x75,0x55,0x85,0x55,0x95,0x55,0xA5,0x55,0xB5,0x55,0xC5,0x55,0xD5,0x55,0xE5,0x55,
            0xF5,0x55,0x1B,0x03,0x00,0x00,0x00,0x13,0x13,0x13,0x0F,0x0F,0x0A,0x50,0x30,0x05,0x03,0x64,0x05,0xe0,
            0x01,0x20,0x03,0x00,0x00,0x32,0x2C,0x34,0x2E,0x00,0x00,0x04,0x14,0x22,0x04,0x00,0x00,0x00,0x00,0x00,
            0x20,0x14,0xEC,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0C,0x30,0x25,0x28,0x14,0x00,
            0x00,0x00,0x00,0x00,0x00,0x01,
        };

        /* config  */
        GTP_CFG_DATA[62] = GT811_MAX_WIDTH >> 8;
        GTP_CFG_DATA[61] = GT811_MAX_WIDTH & 0xff;
        GTP_CFG_DATA[60] = GT811_MAX_HEIGHT >> 8;
        GTP_CFG_DATA[59] = GT811_MAX_HEIGHT & 0xff;

        I2C_WriteReg(GT811_CMD_WR, GT811_CONFIG_REG, (uint8_t *)GTP_CFG_DATA, sizeof(GTP_CFG_DATA));
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
uint16 Keyboard::TOUCH::GT811_ReadID()
{
    uint8_t value[2];
    I2C_ReadReg(GT811_CMD_WR, GT811_VERSION, value, 2);
    return (uint16)(value[0] * 0x100 + value[1]);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Keyboard::TOUCH::I2C_ReadReg(uint8 I2c_Addr, uint16 reg, uint8 *buf, uint8 len)
{
    uint8_t i;
    I2C_Start();
    I2C_Send_Byte(I2c_Addr);
    I2C_Wait_Ack();

#ifdef I2C_MEMADD_16BIT
    I2C_Send_Byte((uint8)(reg >> 8));   	     //Data Addr high
    I2C_Wait_Ack();
#endif

    I2C_Send_Byte((uint8)(reg & 0xFF));   	   //Data Addr low
    I2C_Wait_Ack();
    I2C_Stop();

    I2C_Start();
    I2C_Send_Byte((uint8)(I2c_Addr + 1));
    I2C_Wait_Ack();

    for (i = 0; i < len; i++)
    {
        buf[i] = I2C_Read_Byte((uint8)(i == (len - 1) ? 0 : 1));
    }
    I2C_Stop();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void SDA_IN()
{
    GPIO_InitTypeDef GPIO_InitStruct;

    GPIO_InitStruct.Pin = SDA_GPIO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
    HAL_GPIO_Init(SDA_GPIO_PORT, &GPIO_InitStruct);

}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void delay_us(uint32_t value)
{
    uint32_t i;
    i = value * 250;
    while (i--);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
uint8 Keyboard::TOUCH::I2C_Wait_Ack()
{
    uint16_t ucErrTime = 0;

    SDA_IN();
    SET_SDA();
    delay_us(1);
    SET_CLK();
    delay_us(1);

    while (READ_SDA())
    {
        ucErrTime++;
        if (ucErrTime > 250)
        {
            I2C_Stop();
            return 1;
        }
    }
    CLR_CLK();
    return 0;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
uint8 Keyboard::TOUCH::I2C_WriteReg(uint8 I2c_Addr, uint16 reg, uint8 *buf, uint8 len)
{
    uint8_t i;
    uint8_t ret = 0;
    I2C_Start();

    I2C_Send_Byte(I2c_Addr);   //Slaver Addr
    I2C_Wait_Ack();

#ifdef I2C_MEMADD_16BIT
    I2C_Send_Byte((uint8)(reg >> 8));   	     //Data Addr high
    I2C_Wait_Ack();
#endif

    I2C_Send_Byte((uint8)(reg & 0xFF));   	   //Data Addr low
    I2C_Wait_Ack();

    for (i = 0; i < len; i++)
    {
        I2C_Send_Byte(buf[i]);
        ret = I2C_Wait_Ack();
        if (ret)break;
    }
    I2C_Stop();
    return ret;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void SDA_OUT()
{
    GPIO_InitTypeDef GPIO_InitStruct;

    GPIO_InitStruct.Pin = SDA_GPIO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
    HAL_GPIO_Init(SDA_GPIO_PORT, &GPIO_InitStruct);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void I2C_NAck(void)
{
    SDA_OUT();

    SET_SDA();
    delay_us(1);

    SET_CLK();
    delay_us(1);

    CLR_CLK();
    delay_us(1);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void I2C_Ack(void)
{
    SDA_OUT();
    CLR_CLK();

    CLR_SDA();
    delay_us(1);

    SET_CLK();
    delay_us(1);

    CLR_CLK();
    delay_us(1);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
uint8 Keyboard::TOUCH::I2C_Read_Byte(uint8 ack)
{
    unsigned char i, receive = 0;
    SDA_IN();         //SDA set in
    for (i = 0; i < 8; i++)
    {
        CLR_CLK();
        delay_us(1);
        SET_CLK();
        receive <<= 1;
        if (READ_SDA())receive++;
        delay_us(1);
    }
    CLR_CLK();
    if (!ack)I2C_NAck();   //sent nACK
    else I2C_Ack();        //sent ACK   

    return receive;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Keyboard::TIM4_::ElapsedCallback(void *htim)
{
    if ((TIM_HandleTypeDef *)htim == &handleTIM4 && callbackKeyboard)
    {
        callbackKeyboard();
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Keyboard::TOUCH::I2C_Send_Byte(uint8 txd)
{
    uint8_t t;

    SDA_OUT();
    CLR_CLK();       //Put low CLK to send data

    for (t = 0; t < 8; t++)
    {
        if (txd & 0x80)
            SET_SDA();
        else
            CLR_SDA();

        txd <<= 1;
        delay_us(1);

        SET_CLK();
        delay_us(1);
        CLR_CLK();
        delay_us(1);
    }

    SET_SDA();
    delay_us(1);
    CLR_CLK();
    delay_us(1);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Keyboard::TOUCH::GetState(TS_StateTypeDef *TS_State)
{
    uint8_t RegBuf[34];

    /* Read touch message */
    I2C_ReadReg(GT811_CMD_WR, GT811_READ_XY_REG, RegBuf, sizeof(RegBuf));

    /* get touch massage */
    TS_State->SensorId = (uint)(RegBuf[0x00] >> 6);
    TS_State->touchDetected = (uint8)(RegBuf[0x00] & 0x1F);

    if (TS_State->touchDetected != 0)
    {
        //Touch point 1 coordinates
        TS_State->touchY[0] = (uint16)(GT811_MAX_HEIGHT - (((uint16_t)RegBuf[0x02] << 8) + RegBuf[0x03]));
        TS_State->touchX[0] = (uint16)((((uint16_t)RegBuf[0x04] << 8) + RegBuf[0x05]));
        TS_State->touchWeight[0] = RegBuf[0x06];
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Keyboard::TOUCH::I2C_Stop()
{
    SDA_OUT();

    CLR_SDA();
    delay_us(1);

    SET_CLK();
    delay_us(1);

    SET_SDA();
    delay_us(1);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Keyboard::TOUCH::I2C_Start()
{
    SDA_OUT();

    SET_SDA();
    delay_us(1);

    SET_CLK();
    delay_us(1);

    CLR_SDA();
    delay_us(1);

    CLR_CLK();
    delay_us(1);
}


#ifdef __cplusplus
extern "C" {
#endif

    //------------------------------------------------------------------------------------------------------------------------------------------------
    void TIM4_IRQHandler()
    {
        if ((TIM4->SR & TIM_SR_UIF) == TIM_SR_UIF)
        {
            if ((TIM4->DIER & TIM_DIER_UIE) == TIM_DIER_UIE)
            {
                TIM4->SR = ~TIM_DIER_UIE;
                Keyboard::TIM4_::ElapsedCallback(&handleTIM4);
            }
        }
    }

    //------------------------------------------------------------------------------------------------------------------------------------------------
    void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
    {
        if (GPIO_Pin == GPIO_PIN_7)
        {
            TS_flag = 1;
        }
    }

#ifdef __cplusplus
}
#endif
