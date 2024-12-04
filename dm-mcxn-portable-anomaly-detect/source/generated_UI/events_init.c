/*
* Copyright 2024 NXP
* NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
* accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
* terms, then you may not retain, install, activate or otherwise use the software.
*/

#include "events_init.h"
#include <stdio.h>
#include "lvgl.h"
#include "app_main.h"
#include "motor_control.h"

void events_init(lv_ui *ui)
{
}
extern lv_timer_t *g_pTmrFanRotate;
extern lv_timer_t *g_pTmrPgsBar;


// define a simple function to format the value
void format_int(uint8_t *buf, uint32_t value, uint32_t max_bits){
    switch(max_bits){
        case 4:
            *buf++ = (value / 1000) + '0';
            value %= 1000;
        case 3:
            *buf++ = (value / 100) + '0';
            value %= 100;
        case 2:
            *buf++ = (value / 10) + '0';
            value %= 10;
        case 1:
            *buf++ = (value) + '0';
        default:
            break;
    }
}


void format_nu(uint8_t *buf, uint32_t value){
    // the nu must always be 0-1
    buf[0] = '0';
    buf[1] = '.';
    format_int(buf+2, value, 2);
}

void format_gamma(uint8_t *buf, uint32_t value){
    format_int(buf, value, 4);
}

extern void SetupPgsBar(void);

/* press "Train" button in main screen */
static void Main_btnMainSwScr_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_CLICKED:
	{
		lv_obj_t * act_scr = lv_scr_act();
		lv_disp_t * d = lv_obj_get_disp(act_scr);
		if (d->prev_scr == NULL && (d->scr_to_load == NULL || d->scr_to_load == act_scr))
		{
			if (guider_ui.Model_Training_del == true) {
			    if (g_pTmrFanRotate)
			        lv_timer_del(g_pTmrFanRotate);
			    g_pTmrFanRotate = NULL;
				setup_scr_Model_Training(&guider_ui);
                lv_slider_set_value(guider_ui.Model_Training_SliderModelTrainNu, g_app.nu * 100.0f, LV_ANIM_OFF);
                lv_slider_set_value(guider_ui.Model_Training_SliderModelTrainGamma, g_app.gamma, LV_ANIM_OFF);                              
                lv_obj_t *pBar = guider_ui.Model_Training_barModelTrainProgress;
                lv_bar_set_range(pBar, 0, APP_FEATURE_CNT);
                lv_bar_set_value(pBar, 0, LV_ANIM_OFF);
                
                
			}
			lv_scr_load_anim(guider_ui.Model_Training, LV_SCR_LOAD_ANIM_NONE, 100, 100, true);
            SetupPgsBar();
			guider_ui.Main_del = true;
            guider_ui.is_in_MainScr = false;
		}

		/* Train button, SM transitions to demo_stop mode temporarily. */
		set_demo_mode(demo_stop);
		All_Motors_OFF(); /* Stop all the motors before starting to run */

		//Stop button - shadow effect
		//lv_obj_set_style_shadow_width(guider_ui.Main_btnStopSeq, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
		//lv_obj_set_style_shadow_spread(guider_ui.Main_btnStopSeq, 2, LV_PART_MAIN|LV_STATE_DEFAULT);

	}
		break;
	default:
		break;
	}
}

//Readme button event
static void Main_btn_ReadMe_event_handler(lv_event_t *e)
{	// press "Train" button in main screen
	lv_event_code_t code = lv_event_get_code(e);
	lv_ui *ui = lv_event_get_user_data(e);
	switch (code)
	{
	case LV_EVENT_CLICKED:
	{
		if(ui->is_ReadMe_clicked){
			lv_obj_add_flag(ui->Main_ta_1, LV_OBJ_FLAG_HIDDEN);
			ui->is_ReadMe_clicked = false;

			//Readme button - Shadow effect ON
			lv_obj_set_style_shadow_width(ui->Main_btn_ReadMe, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
			lv_obj_set_style_shadow_spread(ui->Main_btn_ReadMe, 0, LV_PART_MAIN|LV_STATE_DEFAULT);


		}else{
			lv_obj_clear_flag(ui->Main_ta_1, LV_OBJ_FLAG_HIDDEN);
			ui->is_ReadMe_clicked = true;

			//Readme button - Shadow effect OFF
			lv_obj_set_style_shadow_width(ui->Main_btn_ReadMe, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
			lv_obj_set_style_shadow_spread(ui->Main_btn_ReadMe, 2, LV_PART_MAIN|LV_STATE_DEFAULT);

		}
		break;
	}
	default:
		break;
	}
}

/* Show Acc button event */
static void Main_btn_Show_acc_event_handler(lv_event_t *e)
{	// press "Train" button in main screen
	lv_event_code_t code = lv_event_get_code(e);
	lv_obj_t* btn = lv_event_get_target(e);
	lv_obj_t* btn_label = lv_obj_get_child(btn, 0);
	lv_ui *ui = lv_event_get_user_data(e);
	switch (code)
	{
		case LV_EVENT_CLICKED:
		{
			if(ui->is_showing_acc){
				lv_label_set_text(btn_label, "Accel");
				// hidden the acc and show the chart
				lv_obj_add_flag(ui->Main_chart_acc, LV_OBJ_FLAG_HIDDEN);
				lv_obj_add_flag(ui->Main_ddlist_1, LV_OBJ_FLAG_HIDDEN);
				//lv_obj_add_flag(ui->Main_chart_img_label, LV_OBJ_FLAG_HIDDEN);
				lv_obj_clear_flag(ui->Main_chart_1, LV_OBJ_FLAG_HIDDEN);
				//lv_obj_clear_flag(ui->Main_chart_Label, LV_OBJ_FLAG_HIDDEN);
				lv_label_set_text(ui->Main_chart_Label, "Acc (m/s^2)");

				//labels
				lv_obj_add_flag(ui->Accel_chart_X_label, LV_OBJ_FLAG_HIDDEN);
				lv_obj_add_flag(ui->Accel_chart_Y_label, LV_OBJ_FLAG_HIDDEN);
				lv_obj_add_flag(ui->Accel_chart_Z_label, LV_OBJ_FLAG_HIDDEN);
				lv_obj_add_flag(ui->green_legend, LV_OBJ_FLAG_HIDDEN);
				lv_obj_add_flag(ui->red_legend, LV_OBJ_FLAG_HIDDEN);
				lv_obj_add_flag(ui->black_legend, LV_OBJ_FLAG_HIDDEN);

				//Accel button - Shadow Effect ON
				lv_obj_set_style_shadow_width(ui->Main_btn_Show_acc, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
				lv_obj_set_style_shadow_spread(ui->Main_btn_Show_acc, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

				// set the chart acc hidden
				ui->is_showing_acc = false;

			}else{
				lv_label_set_text(btn_label, "Health");
				// hidden the chart1, chart1 label and show the acc
				lv_obj_add_flag(ui->Main_chart_1, LV_OBJ_FLAG_HIDDEN);
				//lv_obj_add_flag(ui->Main_chart_Label, LV_OBJ_FLAG_HIDDEN);
				lv_obj_clear_flag(ui->Main_chart_acc, LV_OBJ_FLAG_HIDDEN);
				lv_obj_clear_flag(ui->Main_ddlist_1, LV_OBJ_FLAG_HIDDEN);
				//lv_obj_clear_flag(ui->Main_chart_img_label, LV_OBJ_FLAG_HIDDEN);
				lv_label_set_text(ui->Main_chart_Label, "Health: ");

				//labels
				lv_obj_clear_flag(ui->Accel_chart_X_label, LV_OBJ_FLAG_HIDDEN);
				lv_obj_clear_flag(ui->Accel_chart_Y_label, LV_OBJ_FLAG_HIDDEN);
				lv_obj_clear_flag(ui->Accel_chart_Z_label, LV_OBJ_FLAG_HIDDEN);
				lv_obj_clear_flag(ui->green_legend, LV_OBJ_FLAG_HIDDEN);
				lv_obj_clear_flag(ui->red_legend, LV_OBJ_FLAG_HIDDEN);
				lv_obj_clear_flag(ui->black_legend, LV_OBJ_FLAG_HIDDEN);

				//Accel button - Shadow effect OFF
				lv_obj_set_style_shadow_width(ui->Main_btn_Show_acc, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
				lv_obj_set_style_shadow_spread(ui->Main_btn_Show_acc, 2, LV_PART_MAIN|LV_STATE_DEFAULT);

				// set the chart acc visible
				ui->is_showing_acc = true;
			}
			break;

		}
		default:
			break;
	}
}

static void Main_ddlist_1_event_handler (lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	lv_ui *ui = lv_event_get_user_data(e);
	uint16_t id = 0;

	// x, y, z
	lv_chart_series_t* ser[4];
	lv_obj_t* chart = ui->Main_chart_acc;
	switch (code) {
		case LV_EVENT_VALUE_CHANGED:
		{

			id = lv_dropdown_get_selected(ui->Main_ddlist_1);
			lv_chart_series_t *next = NULL;
			for(int i=1;i<4;i++){
				ser[i] = lv_chart_get_series_next(chart, next);
				next = ser[i];
			}
			switch(id) {
				// show all the three chart
				case 0:
				{
					// show all
					lv_chart_hide_series(chart, ser[1], false);
					lv_chart_hide_series(chart, ser[2], false);
					lv_chart_hide_series(chart, ser[3], false);
					break;
				}
				case 1: // 0
				{
					// show cur
					lv_chart_hide_series(chart, ser[1], false);
					// mask the other 2
					lv_chart_hide_series(chart, ser[2], true);
					lv_chart_hide_series(chart, ser[3], true);
					break;
				}
				case 2:
				{
					// show cur
					lv_chart_hide_series(chart, ser[2], false);
					// mask the other 2
					lv_chart_hide_series(chart, ser[1], true);
					lv_chart_hide_series(chart, ser[3], true);
					break;
				}
				case 3:
				{
					// show cur
					lv_chart_hide_series(chart, ser[3], false);
					// mask the other 2
					lv_chart_hide_series(chart, ser[1], true);
					lv_chart_hide_series(chart, ser[2], true);
					break;
				}

				default:
					break;
				}
			break;
		}
		default:
			break;
		}
}

/* press "Stop" button in main screen */
static void Main_btnStopSeq_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	lv_ui *ui = lv_event_get_user_data(e);

	switch (code)
	{
	case LV_EVENT_CLICKED:
	{
		if (!ui->is_demo_mode_STOP) /*false*/
		{
			/* Stop button, SM transitions to demo_stop mode. */
			set_demo_mode(demo_stop);
			All_Motors_OFF(); /* Stop all the motors before starting to run */
			lv_label_set_text(ui->Main_btnStopSeq_label, "Run");

			ui->is_demo_mode_STOP = true;
		}
		else{
			/* Same is executed after the train routine automatically */
			set_demo_mode(demo_run);
			lv_label_set_text(ui->Main_btnStopSeq_label, "Stop");

			ui->is_demo_mode_STOP = false;
		}
	}
		break;
	default:
		break;
	}
}


void events_init_Main(lv_ui *ui)
{
	lv_obj_add_event_cb(ui->Main_btnMainSwScr, Main_btnMainSwScr_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->Main_btn_ReadMe, Main_btn_ReadMe_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->Main_btn_Show_acc, Main_btn_Show_acc_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->Main_ddlist_1, Main_ddlist_1_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->Main_btnStopSeq, Main_btnStopSeq_event_handler, LV_EVENT_ALL, ui);

}

extern void SetupMainScrUserLogic(void);

// "Return" button in Train screen is clicked
static void Model_Training_btnModelTrainReturn_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_CLICKED:
	{
		lv_obj_t * act_scr = lv_scr_act();
		lv_disp_t * d = lv_obj_get_disp(act_scr);
		if (d->prev_scr == NULL && (d->scr_to_load == NULL || d->scr_to_load == act_scr))
		{
			if (guider_ui.Main_del == true) {
			    if (g_pTmrPgsBar)
			        lv_timer_del(g_pTmrPgsBar);
			    g_pTmrPgsBar = NULL;
				setup_scr_Main(&guider_ui);

			}

			lv_scr_load_anim(guider_ui.Main, LV_SCR_LOAD_ANIM_NONE, 100, 100, true);
            SetupMainScrUserLogic();
			guider_ui.Model_Training_del = true;
            
        	/* Stop all the motors */
			All_Motors_OFF(); /* Stop all the motors before starting to run */
        	set_demo_mode(demo_run);

		}
	}
		break;
	default:
		break;
	}
}

void events_init_Model_Training(lv_ui *ui)
{
	lv_obj_add_event_cb(ui->Model_Training_btnModelTrainReturn, Model_Training_btnModelTrainReturn_event_handler, LV_EVENT_ALL, ui);
}

/* When user presses the Start Button on training Screen */
static void OnTrainingStart(lv_event_t *e) {
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
		case LV_EVENT_CLICKED:
			g_app.appState = kAppCollecting;
            g_app.featureNdx = 0;
            g_app.nu = lv_slider_get_value(guider_ui.Model_Training_SliderModelTrainNu) / 100.0f;
            g_app.gamma = lv_slider_get_value(guider_ui.Model_Training_SliderModelTrainGamma);

        	/* Start ML Training step */
        	set_demo_mode(demo_train);

			break;
        default:
            break;
	};
}

void events_init_Start(lv_ui *ui)
{
	lv_obj_add_event_cb(ui->Model_Training_btnModelTrainStart, OnTrainingStart, LV_EVENT_ALL, ui);
}

static void OnClickedParamSet(lv_event_t *e) {
	lv_event_code_t code = lv_event_get_code(e);
    lv_ui* ui = lv_event_get_user_data(e);
    
	switch (code)
	{
		case LV_EVENT_CLICKED:
			if(ui->Model_Training_Cont_Hidden) { 
                lv_obj_clear_flag(ui->Model_Training_Cont, LV_OBJ_FLAG_HIDDEN);
                uint8_t text_buf[9] = "Nu: ";
                format_nu(text_buf+4, lv_slider_get_value(ui->Model_Training_SliderModelTrainNu));
                lv_label_set_text(ui->Model_Training_SliderModelTrainNu_Label, (const char*)text_buf);   
                uint8_t text_buf2[14] = "Gam: ";
                format_gamma(text_buf2+5, lv_slider_get_value(ui->Model_Training_SliderModelTrainGamma));
                lv_label_set_text(ui->Model_Training_SliderModelTrainGamma_Label, (const char*)text_buf2);                  
            }
            else lv_obj_add_flag(ui->Model_Training_Cont, LV_OBJ_FLAG_HIDDEN);
            ui->Model_Training_Cont_Hidden ^= 1;

			break;
        default:
            break;
	};
}

void events_param_set(lv_ui *ui)
{
	lv_obj_add_event_cb(ui->Model_Training_btnModelTrainParamSet, OnClickedParamSet, LV_EVENT_ALL, ui);
}

void format_decimal(uint8_t *buf, float value){
    uint32_t sign_bit = 0;
    if(value < 0){
        buf[sign_bit] = '-';
        sign_bit = 1;   
        value = -value;
    }
    format_int(buf+sign_bit, value, 2);
    buf[sign_bit+2] = '.';
    format_int(buf+sign_bit+2+1, ((value - (uint32_t)value)*1000), 3);
}


static void OnSliderNuValueChanged(lv_event_t *e) {
	lv_event_code_t code = lv_event_get_code(e);
    lv_ui* ui = lv_event_get_user_data(e);
    
    uint8_t text_buf[9] = "Nu: ";
	switch (code)
	{
		case LV_EVENT_VALUE_CHANGED:
			format_nu(text_buf+4, lv_slider_get_value(ui->Model_Training_SliderModelTrainNu));
            g_app.nu = lv_slider_get_value(ui->Model_Training_SliderModelTrainNu) / 100.0f;
            lv_label_set_text(ui->Model_Training_SliderModelTrainNu_Label, (const char*)text_buf);
            break;
        default:
            break;
	};
}
void events_slider_nu_value_changed(lv_ui *ui)
{
    lv_obj_add_event_cb(ui->Model_Training_SliderModelTrainNu, OnSliderNuValueChanged, LV_EVENT_ALL, ui);
}

static void OnSliderGammaValueChanged(lv_event_t *e) {
	lv_event_code_t code = lv_event_get_code(e);
    lv_ui* ui = lv_event_get_user_data(e);
    
    uint8_t text_buf[14] = "Gam: ";
	switch (code)
	{
		case LV_EVENT_VALUE_CHANGED:
            g_app.gamma = lv_slider_get_value(ui->Model_Training_SliderModelTrainGamma);
			format_gamma(text_buf+5, lv_slider_get_value(ui->Model_Training_SliderModelTrainGamma));
            lv_label_set_text(ui->Model_Training_SliderModelTrainGamma_Label, (const char*)text_buf);
            break;
        default:
            break;
	};
}
void events_slider_gamma_value_changed(lv_ui *ui)
{
   lv_obj_add_event_cb(ui->Model_Training_SliderModelTrainGamma, OnSliderGammaValueChanged, LV_EVENT_ALL, ui); 
}
