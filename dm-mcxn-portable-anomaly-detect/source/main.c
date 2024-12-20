/*
 * Copyright 2022, 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "board.h"
#include "clock_config.h"
#include "pin_mux.h"

/* Debug console */
#include "fsl_debug_console.h"

/* SDK drivers */
#include "fsl_cache_lpcac.h"

/* FreeRTOS */
#include "FreeRTOS.h"
#include "task.h"

/* LVGL */
#include "lvgl.h"
#include "app_lvgl.h"

#define _MAIN_C_
#include "app_main.h"

#include "arm_math.h"
#include "arm_const_structs.h"
#include "svm_test.h"
#include "fsl_flash.h"
#include "fsl_flash_ffr.h"
#include "motor_control.h"

static flash_config_t s_flashDriver;
#ifdef __ARMCC_VERSION
extern uint8_t Image$$ER_m_svm$$Base[];
#define m_svm_start (uint32_t)Image$$ER_m_svm$$Base

#elif defined(__GNUC__)


extern uint8_t __base_m_svm_model[];
#define m_svm_start __base_m_svm_model
//#define m_svm_start (uint32_t)__base_m_svm_model  //Remove warnings
#endif


/*******************************************************************************
 * Code
 ******************************************************************************/
void BOARD_I2C_ReleaseBus(void);

uint32_t get_flash_address(){
    return m_svm_start;
}

uint32_t save_model_to_flash(void* model, uint32_t model_len){
    taskENTER_CRITICAL();
    // total 8K sector erase, the model must be little than this
    FLASH_Erase(&s_flashDriver, m_svm_start, 0x2000, kFLASH_ApiEraseKey);
    
    for(int i = 0; i < model_len; i += 256){
        FLASH_Program(&s_flashDriver, (uint32_t)(m_svm_start + i), (uint8_t*)model + i, 256);
    }
    taskEXIT_CRITICAL();
    return m_svm_start;
}

void app_gui_demo_init(void);

static void app_initialization_task(void *parameters);

extern uint32_t svm_model_valid(void* model_ptr);
__WEAK void* get_ad_model(void) {
    return svm_model_valid((void*)m_svm_start) ? (void*)m_svm_start : NULL;
}

__WEAK void* ad_train(float features[][APP_FEATURE_DIM], int featureCnt) {
    return svm_train_mode(features, featureCnt);
}

volatile float g_modelRetPlaceHolder;
__WEAK float ad_predict(void* pvModel, float feature[APP_FEATURE_DIM]) {
    return svm_model_pre(pvModel, feature);
}

volatile float g_rmsThreshold = 0.0f;
__WEAK uint8_t ad_is_on(void* pvModel, float feature[APP_FEATURE_CNT]) {
    if (feature[0] >= g_rmsThreshold)
        return 1;
    return 0;
}

int $Sub$$_sys_open(const char *name, int openmode)
{
    return 1;
}

/*******************************************************************************
 * Definitions
 ******************************************************************************/
//Touch sensor on TFT Display
#define TOUCH_I2C_RELEASE_SDA_PORT  PORT4
#define TOUCH_I2C_RELEASE_SCL_PORT  PORT4
#define TOUCH_I2C_RELEASE_SDA_GPIO  GPIO4
#define TOUCH_I2C_RELEASE_SDA_PIN   0U
#define TOUCH_I2C_RELEASE_SCL_GPIO  GPIO4
#define TOUCH_I2C_RELEASE_SCL_PIN   1U
#define TOUCH_I2C_RELEASE_BUS_COUNT 100U

//3-axis Accelerometer sensor
#define ACCEL_I2C_RELEASE_SDA_PORT  PORT1
#define ACCEL_I2C_RELEASE_SCL_PORT  PORT1
#define ACCEL_I2C_RELEASE_SDA_GPIO  GPIO1
#define ACCEL_I2C_RELEASE_SDA_PIN   0U
#define ACCEL_I2C_RELEASE_SCL_GPIO  GPIO1
#define ACCEL_I2C_RELEASE_SCL_PIN   1U
#define ACCEL_I2C_RELEASE_BUS_COUNT 100U

#define ALERT_LED_RED_GPIO   (GPIO3)
#define ALERT_LED_RED_PIN   (20u)

#define ALERT_LED_GREEN_GPIO   (GPIO3)
#define ALERT_LED_GREEN_PIN   (21u)

#define INIT_LED_PINS(led, output) \
    GPIO_PinWrite(ALERT_LED_## led ##_GPIO, ALERT_LED_## led ##_PIN, output); \
    ALERT_LED_## led ##_GPIO->PDDR |= (1U << ALERT_LED_## led ##_PIN)                         /*!< Enable target LED_RED */

#define LED_ON(led) \
    GPIO_PinWrite(ALERT_LED_## led ##_GPIO, ALERT_LED_## led ##_PIN, LOGIC_LED_ON);

#define LED_OFF(led) \
    GPIO_PinWrite(ALERT_LED_## led ##_GPIO, ALERT_LED_## led ##_PIN, LOGIC_LED_OFF);

//RST pin on MikroE slot
#define MIKROE_RST_ON()      GPIO_PortSet(BOARD_MIKROE_NATIVE_RST_GPIO, 1U << BOARD_MIKROE_NATIVE_RST_PIN)

#ifdef __ARMCC_VERSION
extern uint8_t Image$$RW_m_sramx_start$$Limit[];
extern uint8_t Image$$RW_m_sramx_start$$Base[];

#define SVM_pArena Image$$RW_m_sramx_start$$Base

#elif defined(__GNUC__)
extern uint8_t __base_SRAMX[];
extern uint8_t __top_SRAMX[];
#define SVM_pArena __base_SRAMX
#endif


lv_img_dsc_t svm_toy = {
  .header.always_zero = 0,
  .header.w = 160,
  .header.h = 160,
  .data_size = 160 * 160 * LV_COLOR_SIZE,
  .header.cf = LV_IMG_CF_TRUE_COLOR,
  .data = NULL,
};

static void i2c_release_bus_delay(void)
{
    SDK_DelayAtLeastUs(100U, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
}

void BOARD_I2C_ReleaseBus(void)
{
    uint8_t i = 0;
    gpio_pin_config_t pin_config;
    port_pin_config_t i2c_pin_config = {0};

    /* Config pin mux as gpio */
    i2c_pin_config.pullSelect = kPORT_PullUp;
    i2c_pin_config.mux        = kPORT_MuxAlt0; //GPIO

    pin_config.pinDirection = kGPIO_DigitalOutput;
    pin_config.outputLogic  = 1U;

    CLOCK_EnableClock(kCLOCK_Port1); /* I2C Resource for Sensors - Mikroe. */
    CLOCK_EnableClock(kCLOCK_Port4); /* I2C Resource for Touch Sensor on TFT Display. */
    CLOCK_EnableClock(kCLOCK_Gpio1);
    CLOCK_EnableClock(kCLOCK_Gpio4);

    //Set PORT and PIN config - Touch Sensor
    PORT_SetPinConfig(TOUCH_I2C_RELEASE_SCL_PORT, TOUCH_I2C_RELEASE_SCL_PIN, &i2c_pin_config);
    PORT_SetPinConfig(TOUCH_I2C_RELEASE_SCL_PORT, TOUCH_I2C_RELEASE_SDA_PIN, &i2c_pin_config);
    GPIO_PinInit(TOUCH_I2C_RELEASE_SCL_GPIO, TOUCH_I2C_RELEASE_SCL_PIN, &pin_config);
    GPIO_PinInit(TOUCH_I2C_RELEASE_SDA_GPIO, TOUCH_I2C_RELEASE_SDA_PIN, &pin_config);

    //Set PORT and PIN config - Precision Accelerometer Sensor
    PORT_SetPinConfig(ACCEL_I2C_RELEASE_SCL_PORT, ACCEL_I2C_RELEASE_SCL_PIN, &i2c_pin_config);
    PORT_SetPinConfig(ACCEL_I2C_RELEASE_SCL_PORT, ACCEL_I2C_RELEASE_SDA_PIN, &i2c_pin_config);
    GPIO_PinInit(ACCEL_I2C_RELEASE_SCL_GPIO, ACCEL_I2C_RELEASE_SCL_PIN, &pin_config);
    GPIO_PinInit(ACCEL_I2C_RELEASE_SDA_GPIO, ACCEL_I2C_RELEASE_SDA_PIN, &pin_config);

    /* Drive SDA low first to simulate a start */
    GPIO_PinWrite(TOUCH_I2C_RELEASE_SDA_GPIO, TOUCH_I2C_RELEASE_SDA_PIN, 0U);
    GPIO_PinWrite(ACCEL_I2C_RELEASE_SDA_GPIO, ACCEL_I2C_RELEASE_SDA_PIN, 0U);
    i2c_release_bus_delay();

    /* Send 9 pulses on SCL and keep SDA high */
    for (i = 0; i < 9; i++)
    {
        GPIO_PinWrite(TOUCH_I2C_RELEASE_SCL_GPIO, TOUCH_I2C_RELEASE_SCL_PIN, 0U);
        GPIO_PinWrite(ACCEL_I2C_RELEASE_SCL_GPIO, ACCEL_I2C_RELEASE_SCL_PIN, 0U);
        i2c_release_bus_delay();

        GPIO_PinWrite(TOUCH_I2C_RELEASE_SDA_GPIO, TOUCH_I2C_RELEASE_SDA_PIN, 1U);
        GPIO_PinWrite(ACCEL_I2C_RELEASE_SDA_GPIO, ACCEL_I2C_RELEASE_SDA_PIN, 1U);
        i2c_release_bus_delay();

        GPIO_PinWrite(TOUCH_I2C_RELEASE_SCL_GPIO, TOUCH_I2C_RELEASE_SCL_PIN, 1U);
        GPIO_PinWrite(ACCEL_I2C_RELEASE_SCL_GPIO, ACCEL_I2C_RELEASE_SCL_PIN, 1U);
        i2c_release_bus_delay();
    }

    /* Send stop */
    GPIO_PinWrite(TOUCH_I2C_RELEASE_SCL_GPIO, TOUCH_I2C_RELEASE_SCL_PIN, 0U);
    GPIO_PinWrite(ACCEL_I2C_RELEASE_SCL_GPIO, ACCEL_I2C_RELEASE_SCL_PIN, 0U);
    i2c_release_bus_delay();

    GPIO_PinWrite(TOUCH_I2C_RELEASE_SDA_GPIO, TOUCH_I2C_RELEASE_SDA_PIN, 0U);
    GPIO_PinWrite(ACCEL_I2C_RELEASE_SDA_GPIO, ACCEL_I2C_RELEASE_SDA_PIN, 0U);
    i2c_release_bus_delay();

    GPIO_PinWrite(TOUCH_I2C_RELEASE_SCL_GPIO, TOUCH_I2C_RELEASE_SCL_PIN, 1U);
    GPIO_PinWrite(ACCEL_I2C_RELEASE_SCL_GPIO, ACCEL_I2C_RELEASE_SCL_PIN, 1U);
    i2c_release_bus_delay();

    GPIO_PinWrite(TOUCH_I2C_RELEASE_SDA_GPIO, TOUCH_I2C_RELEASE_SDA_PIN, 1U);
    GPIO_PinWrite(ACCEL_I2C_RELEASE_SDA_GPIO, ACCEL_I2C_RELEASE_SDA_PIN, 1U);
    i2c_release_bus_delay();
}


int main(void) {
    L1CACHE_EnableCodeCache();

    CLOCK_SetClkDiv(kCLOCK_DivFlexcom4Clk, 1u);
    CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);

    /* attach FRO 12M to FLEXCOMM2 */
    CLOCK_SetClkDiv(kCLOCK_DivFlexcom2Clk, 1u);
    CLOCK_AttachClk(kFRO12M_to_FLEXCOMM2);

    /* attach FRO 12M to FLEXCOMM3 for Sensor I2C*/
    CLOCK_SetClkDiv(kCLOCK_DivFlexcom3Clk, 1u);
    CLOCK_AttachClk(kFRO12M_to_FLEXCOMM3);

    CLOCK_SetClkDiv(kCLOCK_DivFlexioClk, 1u);
    CLOCK_AttachClk(kPLL0_to_FLEXIO);
    BOARD_InitBootClocks();

    BOARD_I2C_ReleaseBus(); /* Sends 9 CLKs to I2C devices to release I2C bus. */
    BOARD_InitBootPins();

    /* Set P1_3 (RST on ME SLot) High */
    //MIKROE_NATIVE_RST_INIT(); //Init as a output
    MIKROE_RST_ON();

    BOARD_InitDebugConsole();
    
    // Init the LED for show
    CLOCK_EnableClock(kCLOCK_Gpio3);
    INIT_LED_PINS(RED, LOGIC_LED_OFF);
    INIT_LED_PINS(GREEN, LOGIC_LED_OFF);
    LED_RED_INIT(0);
    LED_GREEN_INIT(0);
    LED_GREEN_ON();
    FLASH_Init(&s_flashDriver);
    FFR_Init(&s_flashDriver);

    /* Balanced and Un-balanced Motors control init */
    ESC_Motor_Init();
    flexPWM_init();

    PRINTF("Anomaly detection demo.\r\n");
    if (xTaskCreate(app_initialization_task, "INIT", 512, NULL, 3, NULL) != pdPASS) {
        PRINTF("Failed to create init taskj.\r\n");

        goto dead_loop;
    }

    vTaskStartScheduler();

dead_loop:
    for (;;) {
        /* -- */
    }

    return 0;
}

volatile float g_dbgFeature[APP_FEATURE_DIM] = {0.7f, 0.4f};
#include <imu_ops.h>
#include "ringbuffer.h"
uint16_t ring_buffer[1];//[2 * 3 * APP_FEATURE_CNT];
ringbuffer_t ringbuffer_handler;

void delay_ms(uint32_t ms){
	vTaskDelay(ms);
}

volatile float g_rmsDiv = APP_FEATURE_RMS_DIVIDER;
static void app_sensor_task(void* parameters) 
{
    TickType_t tick_start;
    TickType_t tick_end;
    TickType_t delay_target;
    int cnt = 0;
	
	if(IMU_Init()){
		PRINTF("IMU Sensor init failed! \r\n");
		while(1);
	}
	PRINTF("IMU Sensor init sucess! \r\n");
	ringbuffer_init(&ringbuffer_handler, (uint8_t*)ring_buffer, sizeof(ring_buffer));

    arm_rfft_instance_q15 s;
    arm_rfft_init_q15(&s, APP_FFT_LEN, 0, 1/*bit reversal*/);

    int16_t rawBuf[APP_SENSOR_TASK_READ_SIZE/6][3];
    int taskPeriod_ms = APP_SENSOR_TASK_READ_ITEM * APP_SAMP_PERIOD_MS / 2 + 1 ;
    for (;;) 
    {
        int fifoCnt = IMU_Get_Fifo_Cnt();
        int readSize = 0;

        tick_start = xTaskGetTickCount();

        // calc how many bytes to read
        int maxItemCnt = APP_SENSOR_TASK_READ_ITEM;
        int itemToRead = APP_FFT_LEN - g_app.rfftInFillCnt;
        if (itemToRead > maxItemCnt) itemToRead = maxItemCnt;

        int bytesToRead = itemToRead * 6;

        int16_t msbVal;
        if (fifoCnt >= bytesToRead) {
            readSize = IMU_ReadSensorData(rawBuf[0], fifoCnt, bytesToRead);
            for (int i=0; i<readSize / 6; i++) {
                for (int j=0; j<3; j++) {
                    msbVal = rawBuf[i][j];
                    //msbVal; // /= APP_FFT_MSB_PRE_DIV;
                    g_app.currentMeans[j] += msbVal;
                    g_app.rfftInBuf[j][g_app.rfftInFillCnt] = msbVal;
                }
                g_app.rfftInFillCnt++;
            }
        }

        if (g_app.rfftInFillCnt == APP_FFT_LEN) {
            float rmsAry[3];
            float top1FreqAry[3];
            // calc dc offset EMAs for Ax, Ay, Az
            for (int j=0; j<3; j++) {
                // calc current mean
                g_app.currentMeans[j] /= APP_FFT_LEN;
                g_app.dcEMAs[j] = (
                    g_app.dcEMAs[j] * APP_EMA_DECAY_FACTOR_X100 + 
                    g_app.currentMeans[j] * (100 - APP_EMA_DECAY_FACTOR_X100)) / 100;
                if (cnt == 0)
                    g_app.dcEMAs[j] = g_app.currentMeans[j];
                
                // the rfft will populate the inBuf, and also the substract, so do a backend to it first
                int16_t rfftInBackend[APP_FFT_LEN - APP_FFT_STRIDE];
                memcpy(rfftInBackend, g_app.rfftInBuf[j] + APP_FFT_STRIDE, sizeof(int16_t) * (APP_FFT_LEN - APP_FFT_STRIDE));
                // subtract EMA
                rmsAry[j] = 0.0f;
                float ftmp;
                float rfftIn_max = FLT_MIN;
                for (int i=0; i<APP_FFT_LEN; i++) {
                    g_app.rfftInBuf[j][i] -= g_app.dcEMAs[j];
                    ftmp = (float)(g_app.rfftInBuf[j][i]) / g_rmsDiv;
                    rmsAry[j] += ftmp * ftmp;

                    if(rfftIn_max <= g_app.rfftInBuf[j][i]){
                    	rfftIn_max = g_app.rfftInBuf[j][i];
                    }
                }
                g_app.x_y_z[j] = g_app.rfftInBuf[j][APP_FFT_LEN-1] / rfftIn_max;
                arm_sqrt_f32(rmsAry[j] / APP_FFT_LEN, rmsAry + j);
                // do FFT in q15
                arm_rfft_q15(&s, g_app.rfftInBuf[j], g_app.rfftOutBuf[j]);
                // copy back, we have chance to copy only once, merge to line:215, but will need a 3-buffer, forget it.
                memcpy(g_app.rfftInBuf[j] + APP_FFT_STRIDE, rfftInBackend, sizeof(int16_t) * (APP_FFT_LEN - APP_FFT_STRIDE));
                
                // find top1 freq
                int16_t maxFreqNdx = 0, maxFreqVal = -32768;
                int16_t stmp;
                for (int i=2*2; i<APP_FFT_LEN; i+=2/*skip imag part*/) {
                    stmp = g_app.rfftOutBuf[j][i];
                    if (stmp < 0)
                        stmp = -stmp;
                    if (stmp > maxFreqVal) {
                        maxFreqVal = stmp;
                        maxFreqNdx = i >> 1;    // skip imag part
                    }
                }
                top1FreqAry[j] = (float)(APP_SAMP_RATE_HZ / 2) * maxFreqNdx / (APP_FFT_LEN / 2);
                g_app.currentMeans[j] = 0;
            }
            g_dbgFeature[1] = /*rmsAry[0];*/(rmsAry[0] + rmsAry[1] + rmsAry[2]) / 3.0f;
            g_dbgFeature[0] = top1FreqAry[0] /*(top1FreqAry[0] + top1FreqAry[1] + top1FreqAry[2]) / 3.0f*/ /
                    (APP_SAMP_RATE_HZ / 2); 
            xQueueSend(g_app.qh_NewSample, (void*)g_dbgFeature, portMAX_DELAY);						
            // slide the data to left
            for (int j=0; j<3; j++) {
                memcpy(g_app.rfftInBuf[j], g_app.rfftInBuf[j]+APP_FFT_STRIDE, 
                    (APP_FFT_LEN - APP_FFT_STRIDE) * APP_FFT_DATATYPE_LEN);
                for(int i=0;i<(APP_FFT_LEN - APP_FFT_STRIDE);i++){
                    g_app.currentMeans[j] += g_app.rfftInBuf[j][i];
                }
            }  
            g_app.rfftInFillCnt = APP_FFT_LEN - APP_FFT_STRIDE;
            cnt++;
						
        }
		

        tick_end = xTaskGetTickCount();

        if (tick_end - tick_start >= pdMS_TO_TICKS(taskPeriod_ms)) {
            /* The task takes too long to finish, use minimum delay target */
            delay_target = 1;
        } else {
            delay_target = pdMS_TO_TICKS(taskPeriod_ms) - (tick_end - tick_start);
        }

        //Motor stimulus routine.
        motor_control_task();

        vTaskDelay(delay_target);
    }
}

extern void ShowHealthWaveform(float health);
extern void ShowProgress(int val);
float g_features[APP_FEATURE_CNT][APP_FEATURE_DIM];
volatile uint8_t g_dbgShowNdx; // 0 = health, 1 = rms, 2 = top1
static void app_algo_task(void* parameters)
{
    float feature[APP_FEATURE_DIM];
    g_app.featureNdx = 0;
    int fanState;
    int deSlideAcc = 0;
    for (;;) {
        
        xQueueReceive(g_app.qh_NewSample, feature, portMAX_DELAY);
        g_app.isNewFeature = 1;

        switch (g_app.appState) /* ML Learning & Execution*/
        {
        case kAppCollecting:

            deSlideAcc += APP_FFT_STRIDE;
            if (deSlideAcc >= APP_FFT_LEN)
            {
                deSlideAcc -= APP_FFT_LEN;
                memcpy(g_features + g_app.featureNdx, feature, APP_FEATURE_SIZE);
                g_app.featureNdx++;
                // ShowProgress(g_app.featureNdx);
                if (g_app.featureNdx == APP_FEATURE_CNT) {
                    g_app.pADModel = ad_train(g_features, APP_FEATURE_CNT); 
                    deSlideAcc = 0;
                    if (g_app.pADModel) {
                        g_app.appState = kAppWaitForReturn;
                        svm_toy.data = SVM_pArena;
                        // update the svm_toy heatmap
                        draw_svm_toy_map(g_app.pADModel, (void*)(svm_toy.data), 160, 160);
                        svm_features_t features = {
                            .col = APP_FEATURE_CNT,
                            .row = APP_FEATURE_DIM,
                            .p_feature = (float*)g_features,
                        };
                        draw_points_on_svm_toy((void*)(svm_toy.data), 160, 160, &features, lv_color_make(128, 128, 128), false);
                    }
                }
            }
            break;

        case kAppPredicting:

            if (g_app.pADModel) {
                fanState = g_app.fanState;
                float ret = 0; 
                if (ad_is_on(g_app.pADModel, feature)) {
                    // draw the new one into the svm_toy
                    svm_features_t features = {
                        .col = 1,
                        .row = APP_FEATURE_DIM,
                        .p_feature = (float*)feature,
                    };
                    if(svm_toy.data == NULL){
                        svm_toy.data = SVM_pArena;
                        draw_svm_toy_map(g_app.pADModel, (void*)(svm_toy.data), 160, 160);
                        draw_svm_toy_update();
                    }
                    if(svm_toy.data){
                        draw_points_on_svm_toy((void*)(svm_toy.data), 160, 160, &features, lv_color_make(255, 255, 255), true);
                        // refresh inside the draw points, do not need update manually.
                        draw_svm_toy_update();
                    }
                        
                    ret = ad_predict(g_app.pADModel, feature);
                    float retSign = -1.0f;
                    if(ret > 0){
                        retSign = 1.0f;
                        ret *= 50;
                    } else {
                        ret *= 2;
                    }
                    
                    if (g_dbgShowNdx == 0)
                        g_app.health = (g_app.health + ret + 0.5 * retSign) * 0.85f;
                    else if (g_dbgShowNdx == 1)
                        g_app.health = feature[0] * 10;
                    else if (g_dbgShowNdx == 2)
                        g_app.health = feature[1] * 10;
                    if (g_app.health > APP_HEALTH_ABS_MAX)
                        g_app.health = APP_HEALTH_ABS_MAX;
                    if (g_app.health < -APP_HEALTH_ABS_MAX)
                        g_app.health = -APP_HEALTH_ABS_MAX;
                    if (g_app.fanState == kFanOn) {
                        if (g_app.health < APP_ABNORMAL_LEVEL - APP_SWITCH_BAND)
                            g_app.fanState = kFanErr;
                    } else if (g_app.fanState == kFanErr) {
                        if (g_app.health > APP_ABNORMAL_LEVEL + APP_SWITCH_BAND)
                            g_app.fanState = kFanOn;
                    } else {
                    	g_app.fanState = kFanOn;
                    }
                    if(fanState == kFanErr){
                        LED_OFF(GREEN);
                        LED_ON(RED);

                        LED_GREEN_OFF();
                        LED_RED_ON();

                    }else{
                        LED_OFF(RED);
                        LED_ON(GREEN);

                        LED_RED_OFF();
                        LED_GREEN_ON();
                    }
                                 
                } else {
                    g_app.fanState = kFanOff;
                }
            }
        default:
            break;
        }
    }

}

static void app_initialization_task(void *parameters) {
    // app_lvgl_init() creates lvgl task
    if (app_lvgl_init() != 0) {
        PRINTF("Failed to init LVGL.\r\n");

        goto delete_exit;
    }

    app_gui_demo_init();

    g_app.pADModel = get_ad_model();
    g_app.qh_NewSample = xQueueCreate(1, APP_FEATURE_SIZE);
    g_app.health = APP_HEALTH_ABS_MAX * 0.5f;
    g_app.gamma = 50;
    g_app.nu = 0.1f;
    
    if (xTaskCreate(app_sensor_task, "SENS", 1024, NULL, 3, NULL) != pdPASS) {
        PRINTF("Failed to create sensor task.\r\n");
        while (1) {}
    }

    if (xTaskCreate(app_algo_task, "ALGO", 0x1000, NULL, 3, NULL) != pdPASS) {
        PRINTF("Failed to create sensor task.\r\n");
        while (1) {}
    }

delete_exit:
    vTaskDelete(NULL);
}

