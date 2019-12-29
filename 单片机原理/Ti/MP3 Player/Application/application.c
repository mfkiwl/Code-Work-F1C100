#include "application.h"

//全局变量
uint32_t SysClock;                 //系统时钟
uint8_t idx;                       //用于索引接收到的数值
uint8_t remote_code[4];            //红外键值
uint8_t irdata[33];                //用于记录两个下降沿之间的时间
volatile uint32_t tcount = 0;      //定时器计数值
volatile bool receiveComplete = 0; //接收完成标志位
bool startflag = 0;                //表示开始接收
uint32_t uid;                      //uid

SemaphoreHandle_t InfraredReceive_Handle = NULL; //红外接收信号量
SemaphoreHandle_t BUZZER_Handle = NULL;          //蜂鸣器信号量
EventGroupHandle_t Command_Handle = NULL;        //命令事件组
TimerHandle_t Swtmr1_Handle = NULL;              //软件定时器

//BSP初始化
void BSP_Init()
{

    SysClock = SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN | SYSCTL_USE_PLL | SYSCTL_CFG_VCO_480), SysClockFreq);

    FPUEnable();
    FPULazyStackingEnable();

    TFT_Init();
    BUZZER_Init();
    USART0_Init();
    PN532_Init();
		Show_BMP(0, 0, "nfc.bmp");
		TFT_ShowString(20,300,"Please verify your card!",BLACK,WHITE);
    while (uid != 0xF394AF02)
    {
        uid = PN532_Readuid();
        SysCtlDelay(100 * SysClock / 3000);
    }
    BUZZER_Beep();
    Show_BMP(0, 0, "logo.bmp");
    USART6_Init();
    PLAYER_Init();

    I2C0GPIOBEnable();
    ACCELERATOR_Init();
    ADC0_Init();
    TOUCH_Init();
    LED_Init();
    PCA9557_Init();
    OLED_Init();
    INFRARED_Init();
    TimerIntInitial();
    SysCtlDelay(3 * SysClock / 3);
    TFT_CLEAR(0);
    FreeRTOS_Init();
    IntMasterEnable();
}

//实现流水灯效果
static void LEDTask(void *pvParameters)
{
    while (1)
    {
        int i = 0, t = 0;
        for (;;)
        {
            i++;
            if (i > 8)
            {
                i = 1;
            }
            LED_Open(i);
            vTaskDelay(100);
            LED_Close(i);
        }
    }
}

//bmp轮播
static void BMPDisplay(void *pvParameters)
{
    uint8_t index = 1;
    char s[5];

    while (1)
    {
        sprintf(s, "%d.bmp", index++);
        taskENTER_CRITICAL();
        Show_BMP(0, 0, s);
        taskEXIT_CRITICAL();
        vTaskDelay(5000);
        if (index == 5)
            index = 1;
    }
}

//红外控制
static void INFRARED(void *pvParameters)
{
    uint8_t res;
    while (1)
    {
        xSemaphoreTake(InfraredReceive_Handle, portMAX_DELAY);
        receiveComplete = 0;
        res = Ir_Server();

        switch (res)
        {
        case irKEY1:
            xEventGroupSetBits(Command_Handle, KEY1_EVENT);
            break;
        case irKEY2:
            xEventGroupSetBits(Command_Handle, KEY2_EVENT);
            break;
        case irKEY3:
            xEventGroupSetBits(Command_Handle, KEY3_EVENT);
            break;
        case irKEY4:
            xEventGroupSetBits(Command_Handle, KEY4_EVENT);
            break;
        case irKEY5:
            xEventGroupSetBits(Command_Handle, KEY5_EVENT);
            break;
        default:
            break;
        }
    }
}

//触摸控制
static void GETPoint(void *pvParameters)
{
    TFT_ShowWheel();
    Get_Point();
}

//命令响应
static void Command_Control(void *pvParameters)
{
    EventBits_t r_event;
    while (1)
    {
        r_event = xEventGroupWaitBits(Command_Handle, KEY1_EVENT | KEY2_EVENT | KEY3_EVENT | KEY4_EVENT | KEY5_EVENT, pdTRUE, pdFALSE, portMAX_DELAY);
        xSemaphoreGive(BUZZER_Handle);
        switch (r_event)
        {
        case KEY1_EVENT: //previous
            PREVIOUS();
            break;
        case KEY2_EVENT: //next
            NEXT();
            break;
        case KEY3_EVENT: //play/pause
            PLAY_OR_PAUSE();
            break;
        case KEY4_EVENT: //add_volume
            ADD_VOLUME();
            break;
        case KEY5_EVENT: //sub_volume
            SUB_VOLUME();
            break;
        default:
            break;
        }

    }
}

//SD时钟控制
static void SDTicket(void *pvParameters)
{
    disk_timerproc();
}

//重力小球
static void GravityBall(void *parameter)
{
    int ball_x = 0;
    int ball_y = 0;

    int Gx = 0;
    int Gy = 0;

    KFP KFP_X = {0.02, 0, 0, 0, 0.001, 0.643};
    KFP KFP_Y = {0.02, 0, 0, 0, 0.001, 0.643};

    while (1)
    {
        taskENTER_CRITICAL();
        TFT_DrawRectangle(180 + 2 * ball_y / 10, 100 + 2 * ball_x / 10, 10, 10, BLACK);
        taskEXIT_CRITICAL();
        Gx = ACCELERATOR_Read('X');
        Gy = ACCELERATOR_Read('Y');

        ball_x = kalmanFilter(&KFP_X, (float)Gx);
        ball_y = kalmanFilter(&KFP_Y, (float)Gy);
        taskENTER_CRITICAL();
        TFT_DrawRectangle(180 + 2 * ball_y / 10, 100 + 2 * ball_x / 10, 10, 10, RED);
        taskEXIT_CRITICAL();
        vTaskDelay(20);

    }
}

//ADC读取
static void ADCRead(void *parameter)
{
    uint16_t adc_value;

    char s[10];

    float v, t;
    float K_V, K_T;
    static uint8_t volume = 0;
    uint8_t temp;
    KFP KFP_T = {0.02, 0, 0, 0, 0.001, 0.543};
    KFP KFP_V = {0.02, 0, 0, 0, 0.001, 0.543};
    while (1)
    {
        for (int i = 0; i < 10; i++)
        {
            adc_value = ADC0_Read(0);
            t = (1.8663 - ((adc_value / 4096.0) * 3.3)) * 1000 / 11.69;
            adc_value = ADC0_Read(4);
            v = adc_value / 4096.0 * 3.3 * 1000;
            K_T = kalmanFilter(&KFP_T, t);
            K_V = kalmanFilter(&KFP_V, v);
        }
        temp = (uint8_t)(30 * K_V / 3200.0);
        if(temp != volume)
        {
            volume = temp;
            SET_VOLUME(temp);
        }
        taskENTER_CRITICAL();
        OLED_ShowStr(5, 1, "T:", 2);
        num2char(s, K_T, 2, 2);
        OLED_ShowStr(25, 1, s, 2);
				sprintf(s,"V:%d  ",volume);
        OLED_ShowStr(5, 4, s, 2);
        taskEXIT_CRITICAL();

        vTaskDelay(100);
    }
}

//蜂鸣器
static void BEEZZERBeep(void *parameter)
{
    while (1)
    {
        xSemaphoreTake(BUZZER_Handle, portMAX_DELAY);
        taskENTER_CRITICAL();
        BUZZER_Beep();
        taskEXIT_CRITICAL();
    }
}

//数码管
static void DIGITALTube(void *parameter)
{
    while (1)
    {
        setnumber(1, '1');
        vTaskDelay(1000);
        setnumber(2, '2');
        vTaskDelay(1000);
        setnumber(3, '3');
        vTaskDelay(1000);
        setnumber(4, '4');
        vTaskDelay(1000);
    }
}

//FreeRTOS初始化
void FreeRTOS_Init()
{
    taskENTER_CRITICAL();
    InfraredReceive_Handle = xSemaphoreCreateBinary();
    BUZZER_Handle = xSemaphoreCreateBinary();
    Command_Handle = xEventGroupCreate();
    Swtmr1_Handle = xTimerCreate((const char *)"SDTicket", (TickType_t)10, (UBaseType_t)pdTRUE, (void *)1, (TimerCallbackFunction_t)SDTicket);
    xTimerStart(Swtmr1_Handle, 0);

    xTaskCreate(LEDTask, (const portCHAR *)"Led", 128, NULL,
                tskIDLE_PRIORITY + 10, NULL);
    xTaskCreate(GravityBall, (const portCHAR *)"Ball", 128, NULL,
                tskIDLE_PRIORITY + 10, NULL);
    xTaskCreate(GETPoint, (const portCHAR *)"Touch", 128, NULL,
                tskIDLE_PRIORITY + 10, NULL);
    xTaskCreate(INFRARED, (const portCHAR *)"Infrared", 128, NULL,
                tskIDLE_PRIORITY + 10, NULL);
    xTaskCreate(Command_Control, (const portCHAR *)"Control", 128, NULL,
                tskIDLE_PRIORITY + 10, NULL);
    xTaskCreate(BMPDisplay, (const portCHAR *)"BmpDisplay", 128, NULL,
                tskIDLE_PRIORITY + 10, NULL);
    xTaskCreate(ADCRead, (const portCHAR *)"ADC", 128, NULL,
                tskIDLE_PRIORITY + 10, NULL);
    xTaskCreate(BEEZZERBeep, (const portCHAR *)"Buzzer", 128, NULL,
                tskIDLE_PRIORITY + 10, NULL);
    xTaskCreate(DIGITALTube, (const portCHAR *)"DigitalTube", 128, NULL,
                tskIDLE_PRIORITY + 10, NULL);

    vTaskStartScheduler();
    taskEXIT_CRITICAL();
}

//Hard Fault 处理
void LED_BLINK(void)
{
    LED_Open(1);
    SysCtlDelay(50000000 / 3);
    LED_Close(1);
    SysCtlDelay(50000000 / 3);
}