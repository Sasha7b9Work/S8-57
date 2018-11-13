#pragma once
#include "Display/Colors.h"
#include "Hardware/stm32/stm746.h"
#include "Hardware/Controls.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct TS_StateTypeDef
{
    uint8  touchDetected;
    uint16 touchX[5];
    uint16 touchY[5];
    uint8  touchWeight[5];
    uint   SensorId;
};

// Touch
#define SDA_GPIO_PIN        GPIO_PIN_13
#define SDA_GPIO_PORT       GPIOD
#define SCL_GPIO_PIN        GPIO_PIN_12
#define SCL_GPIO_PORT       GPIOD
#define LCD_RST_GPIO_Port   GPIOD
#define LCD_RST_Pin         GPIO_PIN_11
#define GT811_VERSION_VALUE 0x2010   /* GT811 firmware version */
/* Maximum border values of the touchscreen pad */
#define GT811_MAX_WIDTH     ((uint16_t)1024)    /* Touchscreen pad max width   */
#define GT811_MAX_HEIGHT    ((uint16_t)600)     /* Touchscreen pad max height  */
#define GT811_CMD_WR		0XBA	/* GT811 Write address  */
#define GT811_CONFIG_REG    0x6A2	  /* GT811 configuration start address  */
#define GT811_VERSION       0x717    /* GT811 version register addresses */
#define I2C_MEMADD_16BIT
#define SET_SDA()           HAL_GPIO_WritePin(SDA_GPIO_PORT,SDA_GPIO_PIN,GPIO_PIN_SET)  
#define SET_CLK()           HAL_GPIO_WritePin(SCL_GPIO_PORT,SCL_GPIO_PIN,GPIO_PIN_SET)
#define CLR_SDA()           HAL_GPIO_WritePin(SDA_GPIO_PORT,SDA_GPIO_PIN,GPIO_PIN_RESET)
#define CLR_CLK()           HAL_GPIO_WritePin(SCL_GPIO_PORT,SCL_GPIO_PIN,GPIO_PIN_RESET)
#define READ_SDA()          HAL_GPIO_ReadPin(SDA_GPIO_PORT,SDA_GPIO_PIN)
#define LCD_INT_Pin         GPIO_PIN_7
#define GT811_READ_XY_REG   0x721  	/* GT811 touch message start address  */

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Keyboard
{
public:
    static void Init();

    static void Draw();

    //----------------------------------------------------------------------------------------------------------------------------------- TIM4 ---
    class TIM4_
    {
        friend class Keyboard;
    public:
        static void ElapsedCallback(void *htim);
    };


private:

    static void DrawButton(int x, int y, int width, int height, Color color, const char *title);

    static void Update();
    /// Установка каллбэка для клавиатуры
    static void SetCallback(void(*func)());

    static void FillCommand(Control control, TypePress typePress);

    //--------------------------------------------------------------------------------------------------------------------------------------------
    class TOUCH
    {
    friend class Keyboard;
    public:

        static void Init();

        static void GetState(TS_StateTypeDef *state);
    private:

        static uint16 GT811_ReadID();

        static uint8 I2C_WriteReg(uint8 I2c_Addr, uint16 reg, uint8 *buf, uint8 len);

        static void I2C_ReadReg(uint8 I2c_Addr, uint16 reg, uint8 *buf, uint8 len);

        static void I2C_Start();

        static void I2C_Send_Byte(uint8 txd);

        static uint8 I2C_Wait_Ack();

        static void I2C_Stop();

        static uint8 I2C_Read_Byte(uint8 ack);
    };
};
