/**
 * @file lv_demo_widgets.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "lv_demo_widgets.h"
#include "app_main.h"
#if LV_USE_DEMO_WIDGETS

#if LV_MEM_CUSTOM == 0 && LV_MEM_SIZE < (38ul * 1024ul)
    #error Insufficient memory for lv_demo_widgets. Please set LV_MEM_SIZE to at least 38KB (38ul * 1024ul).  48KB is recommended.
#endif

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/
typedef enum {
    DISP_SMALL,
    DISP_MEDIUM,
    DISP_LARGE,
} disp_size_t;

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/
#include "FreeRTOS.h"
#include "timers.h"

uint32_t seed;
void srand(uint32_t new_seed){
	seed = new_seed;
}

uint32_t rand(){
	seed = 1664525 * seed + 1013904223;
	return (uint32_t)(seed & 0xFFFF);
}

volatile uint32_t rect_x, rect_y;
void x_inc_event_handler(lv_event_t *event)
{
	lv_event_code_t code = lv_event_get_code(event);
	lv_obj_t *rect = lv_event_get_user_data(event);
    if(code == LV_EVENT_CLICKED) {
    	srand(lv_tick_get());
    	uint16_t rect_x_y = rand() & 0xFFFF;
    	rect_x = rect_x_y & 0xFF;
    	rect_y = rect_x_y >> 8;
    	lv_obj_set_pos(rect, rect_x, rect_y);
    }
}

extern uint16_t pic_start[];
extern uint16_t pic_end[];

#define IMG_OBJ_W  (128)
#define IMG_OBJ_H  (99 / 3)
#define FRAMES (25 * 3)
uint16_t camera_buffer_simulate[2][IMG_OBJ_W*IMG_OBJ_H];
#define LV_IMAGE_VAL \
	(lv_img_dsc_t){ \
	    .header.always_zero = 0, \
	    .header.w = IMG_OBJ_W, \
	    .header.h = IMG_OBJ_H, \
	    .data_size = IMG_OBJ_W * IMG_OBJ_H * 2, \
	    .header.cf = LV_IMG_CF_TRUE_COLOR, \
	    .data = NULL,  \
	}; \

#define DECLARE_DEF_LV_IMAGE(name, n) \
	name [n] = LV_IMAGE_VAL; \
	name ##_obj[n] = lv_img_create(cont); \
	lv_img_set_src(name ##_obj[n], &(name [n])); \
	lv_obj_set_pos(name ##_obj[n], 100, 100 + n * (IMG_OBJ_H)); \
	lv_obj_set_style_pad_all(name ##_obj[n], 0, 0); \
	lv_obj_add_flag(name ##_obj[n], LV_OBJ_FLAG_OVERFLOW_VISIBLE); \
	lv_obj_set_size(name ##_obj[n], IMG_OBJ_W, IMG_OBJ_H); \
	lv_obj_add_flag(name ##_obj[n], LV_OBJ_FLAG_IGNORE_LAYOUT | LV_LAYER_TYPE_NONE); \
	lv_obj_set_style_bg_color(name ##_obj[n], lv_color_make(0xFF, 0, 0), 0);


static TimerHandle_t xTimer;
void start_event_handler(lv_event_t *event)
{
	lv_event_code_t code = lv_event_get_code(event);
	lv_obj_t *label = lv_event_get_user_data(event);
    if(code == LV_EVENT_CLICKED) {
    	lv_label_set_text(label, "Stop");
    	xTimerStart(xTimer, 0);
    }
}

#include "gui_guider.h"
#include "events_init.h"
#include "custom.h"
lv_ui guider_ui;

lv_timer_t *g_pTmrFanRotate;
lv_timer_t *g_pTmrPgsBar;
lv_chart_series_t* g_pSeriesAry[1];

lv_chart_series_t* g_pACCSeriesAry[3]; // x, y, z

void ShowHealthWaveform(float health) {
	lv_ui* ui = &guider_ui;
	float stdHealth = (health + APP_HEALTH_ABS_MAX) / (2.0f * APP_HEALTH_ABS_MAX);

    uint32_t uHealth = (uint32_t)(stdHealth * 100.0f + 0.5f);

    lv_chart_set_next_value(ui->Main_chart_1, g_pSeriesAry[0], uHealth);
    
    uint8_t buffer[8+3+1+3+1] = "Health: "; // one sign bit, 2 int, 1., 3 decimal
    uint8_t buffer_acc[11] = "Acc (m/s^2)"; // one sign bit, 2 int, 1., 3 decimal

    extern void format_decimal(uint8_t *buf, float value);
    format_decimal(buffer+7, health);

    if(!ui->is_showing_acc)
    {
    	lv_label_set_text(ui->Main_chart_Label, (const char*)buffer);
    }
    else{
    	lv_label_set_text(ui->Main_chart_Label, (const char*)buffer_acc);
	}
}

void ShowAccFrom(float* acc_value) {
	lv_ui* ui = &guider_ui;
	float x = (acc_value[0] + APP_ACC_ABS_MAX) / (2.0f * APP_ACC_ABS_MAX);
	float y = (acc_value[1] + APP_ACC_ABS_MAX) / (2.0f * APP_ACC_ABS_MAX);
	float z = (acc_value[2] + APP_ACC_ABS_MAX) / (2.0f * APP_ACC_ABS_MAX);

    lv_chart_set_next_value(ui->Main_chart_acc, g_pACCSeriesAry[0], (uint32_t)(x * 100.0f + 0.5f));
    lv_chart_set_next_value(ui->Main_chart_acc, g_pACCSeriesAry[1], (uint32_t)(y * 100.0f + 0.5f));
    lv_chart_set_next_value(ui->Main_chart_acc, g_pACCSeriesAry[2], (uint32_t)(z * 100.0f + 0.5f));
}

void ShowProgress(int val)
{
	lv_obj_t *pBar;
	lv_ui* ui = &guider_ui;

	pBar = ui->Model_Training_barModelTrainProgress;
	lv_bar_set_value(pBar, g_app.featureNdx, LV_ANIM_OFF);
}

void OnTmr_FanRotate(lv_timer_t* pTmr) 
{

    if (g_app.fanState == kFanOff) {
        return;
    }
 //   uint16_t angle = lv_img_get_angle(pFan);
    
    if (g_app.fanState == kFanErr) {
  //  	lv_img_set_src(pFan,&_motor_abnormal_on_alpha_160x160);
    } else {
  //  	lv_img_set_src(pFan,&_motor_normal_on_alpha_160x160);
    }
 //   angle = (angle + 150) % 3600;
  //  lv_img_set_angle(pFan, angle);

    if (g_app.isNewFeature) {
        g_app.isNewFeature = 0;
        if (g_app.appState == kAppPredicting){
            ShowHealthWaveform(g_app.health);
            ShowAccFrom(g_app.x_y_z);
        }

    }
}

void SetupFanRotating(void)
{
   // lv_img_set_pivot(guider_ui.Main_img_status, 80,80);
    g_pTmrFanRotate = lv_timer_create(OnTmr_FanRotate, 100, guider_ui.Main_labelMainCurFunc);
	//g_pTmrFanRotate = lv_timer_create(OnTmr_FanRotate, 100, guider_ui.Main_img_status);
}

void OnTmr_PgsBar(lv_timer_t* pTmr) 
{
    lv_ui* ui = &guider_ui;

    lv_bar_set_value(ui->Model_Training_barModelTrainProgress, g_app.featureNdx, LV_ANIM_OFF);
}

void SetupPgsBar(void)
{
    g_pTmrPgsBar = lv_timer_create(OnTmr_PgsBar, 100, guider_ui.Model_Training_barModelTrainProgress);
}



void SetChartInitData(void)
{
    lv_ui* ui = &guider_ui;   
	lv_chart_series_t * Main_chart_1_0 = lv_chart_add_series(ui->Main_chart_1, lv_color_make(0x00, 0x00, 0x00), LV_CHART_AXIS_PRIMARY_Y);
	// lv_chart_series_t * Main_chart_1_1 = lv_chart_add_series(ui->Main_chart_1, lv_color_make(0xcc, 0x2d, 0x2d), LV_CHART_AXIS_PRIMARY_Y);
    // lv_chart_series_t * Main_chart_1_2 = lv_chart_add_series(ui->Main_chart_1, lv_color_make(0x37, 0xa5, 0x18), LV_CHART_AXIS_PRIMARY_Y);
    g_pSeriesAry[0] = Main_chart_1_0;

    lv_chart_set_point_count(ui->Main_chart_1, 20);
    lv_chart_set_div_line_count(ui->Main_chart_1, 5, 4);
    
}

void SetAccChartInitData(void)
{
    lv_ui* ui = &guider_ui;
	lv_chart_series_t * Main_chart_1_0 = lv_chart_add_series(ui->Main_chart_acc, lv_color_make(0x00, 0x00, 0x00), LV_CHART_AXIS_PRIMARY_Y);
	lv_chart_series_t * Main_chart_1_1 = lv_chart_add_series(ui->Main_chart_acc, lv_color_make(0xcc, 0x2d, 0x2d), LV_CHART_AXIS_PRIMARY_Y);
    lv_chart_series_t * Main_chart_1_2 = lv_chart_add_series(ui->Main_chart_acc, lv_color_make(0x37, 0xa5, 0x18), LV_CHART_AXIS_PRIMARY_Y);
    g_pACCSeriesAry[0] = Main_chart_1_0;
    g_pACCSeriesAry[1] = Main_chart_1_1;
    g_pACCSeriesAry[2] = Main_chart_1_2;
    lv_chart_set_point_count(ui->Main_chart_acc, 20);
    lv_chart_set_div_line_count(ui->Main_chart_acc, 5, 4);
}

void SetupMainScrUserLogic(void)
{
    g_app.appState = kAppPredicting;
    SetupFanRotating();
    SetChartInitData();
    SetAccChartInitData();
}

uint32_t get_slider_value(uint32_t value_type){
    switch(value_type){
        // gamma
        case 0:
            return lv_slider_get_value(guider_ui.Model_Training_SliderModelTrainGamma);
        case 1:
            return lv_slider_get_value(guider_ui.Model_Training_SliderModelTrainNu);
        default:
            break;
    }
    return 0;
}

void lv_demo_widgets(void)
{
    setup_ui(&guider_ui);
    events_init(&guider_ui);
    custom_init(&guider_ui);
    
    SetupMainScrUserLogic();
}
#endif
