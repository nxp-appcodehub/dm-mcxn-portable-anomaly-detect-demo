/*
* Copyright 2024 NXP
* NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
* accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
* terms, then you may not retain, install, activate or otherwise use the software.
*/

#include "lvgl.h"
#include <stdio.h>
#include "gui_guider.h"
#include "events_init.h"
#include "custom.h"

#undef lv_obj_set_pos
#undef lv_obj_set_size

void setup_scr_Main(lv_ui *ui){

	//Write codes Main
	ui->Main = lv_obj_create(NULL);
	lv_obj_set_scrollbar_mode(ui->Main, LV_SCROLLBAR_MODE_OFF);
	//Set style for Main. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	//lv_obj_set_style_bg_color(ui->Main, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->Main, lv_color_hex(0xdde4e6), LV_PART_MAIN|LV_STATE_DEFAULT);
	//lv_obj_set_style_bg_grad_color(ui->Main, lv_color_make(0x2F, 0x92, 0xDA), LV_PART_MAIN|LV_STATE_DEFAULT);
	//lv_obj_set_style_bg_grad_dir(ui->Main, LV_GRAD_DIR_VER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->Main, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes Main_labelMainCurFunc
	ui->Main_labelMainCurFunc = lv_label_create(ui->Main);
	lv_obj_set_pos(ui->Main_labelMainCurFunc, 15, 5);
	lv_obj_set_size(ui->Main_labelMainCurFunc, 300, 30);
	//lv_obj_align(ui->Main_labelMainCurFunc, LV_ALIGN_TOP_LEFT, 0, 0);
	lv_obj_set_scrollbar_mode(ui->Main_labelMainCurFunc, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->Main_labelMainCurFunc, "Motor Anomaly Detect");

	//Set style for Main_labelMainCurFunc. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->Main_labelMainCurFunc, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->Main_labelMainCurFunc, lv_color_make(0xda, 0xf6, 0xfd), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->Main_labelMainCurFunc, lv_color_make(0x2F, 0xCA, 0xDA), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->Main_labelMainCurFunc, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->Main_labelMainCurFunc, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->Main_labelMainCurFunc, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->Main_labelMainCurFunc, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->Main_labelMainCurFunc, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->Main_labelMainCurFunc, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->Main_labelMainCurFunc, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->Main_labelMainCurFunc, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->Main_labelMainCurFunc, lv_color_make(0x0d, 0x30, 0x08), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->Main_labelMainCurFunc, &lv_font_montserratMedium_20, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->Main_labelMainCurFunc, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->Main_labelMainCurFunc, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->Main_labelMainCurFunc, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->Main_labelMainCurFunc, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->Main_labelMainCurFunc, 8, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->Main_labelMainCurFunc, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	
	//Train button code - TRAIN
	ui->Main_btnMainSwScr = lv_btn_create(ui->Main);
	lv_obj_set_pos(ui->Main_btnMainSwScr, 229, 260);
	lv_obj_set_size(ui->Main_btnMainSwScr, 75, 40);
	lv_obj_set_scrollbar_mode(ui->Main_btnMainSwScr, LV_SCROLLBAR_MODE_OFF);

	//Set style for TRAIN Button. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->Main_btnMainSwScr, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->Main_btnMainSwScr, lv_color_hex(0x69CA00), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->Main_btnMainSwScr, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->Main_btnMainSwScr, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->Main_btnMainSwScr, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->Main_btnMainSwScr, lv_color_hex(0xb9b7b7), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_side(ui->Main_btnMainSwScr, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->Main_btnMainSwScr, lv_color_make(0x2F, 0xCA, 0xDA), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->Main_btnMainSwScr, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	//Button Shadow
	lv_obj_set_style_shadow_width(ui->Main_btnMainSwScr, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->Main_btnMainSwScr, lv_color_hex(0x5f605e), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->Main_btnMainSwScr, 180, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->Main_btnMainSwScr, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->Main_btnMainSwScr, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->Main_btnMainSwScr, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->Main_btnMainSwScr, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->Main_btnMainSwScr, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->Main_btnMainSwScr, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->Main_btnMainSwScr, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	ui->Main_btnMainSwScr_label = lv_label_create(ui->Main_btnMainSwScr);
	lv_label_set_text(ui->Main_btnMainSwScr_label, "Train");
	lv_obj_set_style_pad_all(ui->Main_btnMainSwScr, 0, LV_STATE_DEFAULT);
	lv_obj_align(ui->Main_btnMainSwScr_label, LV_ALIGN_CENTER, 0, 0);

	//Write codes Main_img_status
/*	ui->Main_img_status = lv_img_create(ui->Main);
	lv_obj_set_pos(ui->Main_img_status, 308, 44);
	lv_obj_set_size(ui->Main_img_status, 160, 160);
	lv_obj_set_scrollbar_mode(ui->Main_img_status, LV_SCROLLBAR_MODE_OFF);

	//Set style for Main_img_status. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_img_recolor(ui->Main_img_status, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor_opa(ui->Main_img_status, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_opa(ui->Main_img_status, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->Main_img_status, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->Main_img_status,&_motor_normal_on_alpha_160x160);
	lv_img_set_pivot(ui->Main_img_status, 80,80);
	lv_img_set_angle(ui->Main_img_status, 0);*/

	//Write codes Main_chart_1
	//Health monitor chart
	ui->Main_chart_1 = lv_chart_create(ui->Main);
	lv_obj_set_pos(ui->Main_chart_1, 215, 70);
	lv_obj_set_size(ui->Main_chart_1, 244, 160);
	lv_obj_set_scrollbar_mode(ui->Main_chart_1, LV_SCROLLBAR_MODE_OFF);

	//Set style for Main_chart_1. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->Main_chart_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->Main_chart_1, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->Main_chart_1, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->Main_chart_1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->Main_chart_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->Main_chart_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->Main_chart_1, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->Main_chart_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->Main_chart_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->Main_chart_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->Main_chart_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->Main_chart_1, lv_color_make(0xe8, 0xe8, 0xe8), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->Main_chart_1, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->Main_chart_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_color(ui->Main_chart_1, lv_color_make(0xe8, 0xe8, 0xe8), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_width(ui->Main_chart_1, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_opa(ui->Main_chart_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_chart_set_type(ui->Main_chart_1, LV_CHART_TYPE_LINE);
	lv_chart_set_range(ui->Main_chart_1,LV_CHART_AXIS_PRIMARY_Y, 0, 100);
	lv_chart_set_div_line_count(ui->Main_chart_1, 5, 4);
	lv_chart_set_point_count(ui->Main_chart_1, 7);
    
    ui->Main_chart_Label = lv_label_create(ui->Main);
    lv_obj_set_pos(ui->Main_chart_Label, 215, 50);
	lv_obj_set_size(ui->Main_chart_Label, 200, 32);
    lv_label_set_text(ui->Main_chart_Label, "Health: 0.00");
    
    //extern const LV_ATTRIBUTE_LARGE_CONST lv_img_dsc_t svm_toy;
    //memset((void*)svm_toy.data, 0x0, 160*160*sizeof(lv_color_t));
    ui->Main_svm_toy = lv_img_create(ui->Main);
	lv_obj_set_pos(ui->Main_svm_toy, 30, 70);
	lv_obj_set_size(ui->Main_svm_toy, 160, 160);

	ui->Main_svm_toy_x_label = lv_label_create(ui->Main);
	lv_obj_set_size(ui->Main_svm_toy_x_label, 300, 32);
	lv_obj_set_pos(ui->Main_svm_toy_x_label, 30, 50);
	lv_label_set_long_mode(ui->Main_svm_toy_x_label, LV_LABEL_LONG_WRAP);
	lv_label_set_text(ui->Main_svm_toy_x_label, "Freq of Max FFT Ampl");
	lv_obj_set_style_text_align(ui->Main_svm_toy_x_label, LV_TEXT_ALIGN_LEFT, 0);

	ui->Main_svm_toy_y_label = lv_label_create(ui->Main);
	lv_obj_set_size(ui->Main_svm_toy_y_label, 16, 32 * 3);
	lv_obj_set_pos(ui->Main_svm_toy_y_label, 10, 70 + 64);
	lv_label_set_long_mode(ui->Main_svm_toy_y_label, LV_LABEL_LONG_WRAP);
	lv_label_set_text(ui->Main_svm_toy_y_label, "RMS");
	lv_obj_set_style_text_align(ui->Main_svm_toy_y_label, LV_TEXT_ALIGN_CENTER, 0);

	//Write codes - README button
	ui->Main_btn_ReadMe = lv_btn_create(ui->Main);
	lv_obj_set_pos(ui->Main_btn_ReadMe, 29, 260);
	lv_obj_set_size(ui->Main_btn_ReadMe, 92, 40);
	lv_obj_set_scrollbar_mode(ui->Main_btn_ReadMe, LV_SCROLLBAR_MODE_OFF);

	//Set style for Main_btnMainSwScr. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->Main_btn_ReadMe, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->Main_btn_ReadMe, lv_color_hex(0x0EAFE0), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->Main_btn_ReadMe, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->Main_btn_ReadMe, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->Main_btn_ReadMe, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->Main_btn_ReadMe, lv_color_hex(0xb9b7b7), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_side(ui->Main_btn_ReadMe, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->Main_btn_ReadMe, lv_color_make(0x2F, 0xCA, 0xDA), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->Main_btn_ReadMe, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	//Button Shadow
	lv_obj_set_style_shadow_width(ui->Main_btn_ReadMe, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->Main_btn_ReadMe, lv_color_hex(0x5f605e), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->Main_btn_ReadMe, 180, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->Main_btn_ReadMe, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->Main_btn_ReadMe, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->Main_btn_ReadMe, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->Main_btn_ReadMe, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->Main_btn_ReadMe, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->Main_btn_ReadMe, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->Main_btn_ReadMe, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	ui->Main_btn_ReadMe_label = lv_label_create(ui->Main_btn_ReadMe);
	lv_label_set_text(ui->Main_btn_ReadMe_label, "Readme");
	lv_obj_set_style_pad_all(ui->Main_btn_ReadMe_label, 0, LV_STATE_DEFAULT);
	lv_obj_align(ui->Main_btn_ReadMe_label, LV_ALIGN_CENTER, 0, 0);
	ui->is_ReadMe_clicked = false;

	//SHOW Accelerometer button
	ui->Main_btn_Show_acc = lv_btn_create(ui->Main);
	lv_obj_set_pos(ui->Main_btn_Show_acc, 134, 260);
	lv_obj_set_size(ui->Main_btn_Show_acc, 80, 40);
	lv_obj_set_scrollbar_mode(ui->Main_btn_Show_acc, LV_SCROLLBAR_MODE_OFF);

	//Set style for Main_btnMainSwScr. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->Main_btn_Show_acc, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->Main_btn_Show_acc, lv_color_hex(0x0EAFE0), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->Main_btn_Show_acc, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->Main_btn_Show_acc, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->Main_btn_Show_acc, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->Main_btn_Show_acc, lv_color_hex(0xb9b7b7), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_side(ui->Main_btn_Show_acc, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->Main_btn_Show_acc, lv_color_make(0x2F, 0xCA, 0xDA), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->Main_btn_Show_acc, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	//Button Shadow
	lv_obj_set_style_shadow_width(ui->Main_btn_Show_acc, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->Main_btn_Show_acc, lv_color_hex(0x5f605e), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->Main_btn_Show_acc, 180, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->Main_btn_Show_acc, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->Main_btn_Show_acc, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->Main_btn_Show_acc, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->Main_btn_Show_acc, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->Main_btn_Show_acc, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->Main_btn_Show_acc, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->Main_btn_Show_acc, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	ui->Main_btn_Show_acc_label = lv_label_create(ui->Main_btn_Show_acc);
	lv_label_set_text(ui->Main_btn_Show_acc_label, "Accel");
	lv_obj_set_style_pad_all(ui->Main_btn_Show_acc_label, 0, LV_STATE_DEFAULT);
	lv_obj_align(ui->Main_btn_Show_acc_label, LV_ALIGN_CENTER, 0, 0);


	//STOP Sequence button
	ui->Main_btnStopSeq = lv_btn_create(ui->Main);
	lv_obj_set_pos(ui->Main_btnStopSeq, 319, 260);
	lv_obj_set_size(ui->Main_btnStopSeq, 75, 40);
	lv_obj_set_scrollbar_mode(ui->Main_btnStopSeq, LV_SCROLLBAR_MODE_OFF);

	//Write style for screen_btn_4, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_radius(ui->Main_btnStopSeq, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->Main_btnStopSeq, lv_color_hex(0xFCA600), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->Main_btnStopSeq, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->Main_btnStopSeq, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->Main_btnStopSeq, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->Main_btnStopSeq, lv_color_hex(0xb9b7b7), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_side(ui->Main_btnStopSeq, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->Main_btnStopSeq, lv_color_make(0x2F, 0xCA, 0xDA), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->Main_btnStopSeq, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	//Button Shadow
	lv_obj_set_style_shadow_width(ui->Main_btnStopSeq, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->Main_btnStopSeq, lv_color_hex(0x5f605e), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->Main_btnStopSeq, 180, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->Main_btnStopSeq, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->Main_btnStopSeq, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->Main_btnStopSeq, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->Main_btnStopSeq, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->Main_btnStopSeq, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->Main_btnStopSeq, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->Main_btnStopSeq, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	//Label for Stop Button
	ui->Main_btnStopSeq_label = lv_label_create(ui->Main_btnStopSeq);
	lv_label_set_text(ui->Main_btnStopSeq_label, "Stop");
	lv_obj_set_style_pad_all(ui->Main_btnStopSeq_label, 0, LV_STATE_DEFAULT);
	lv_obj_align(ui->Main_btnStopSeq_label, LV_ALIGN_CENTER, 0, 0);
	ui->is_demo_mode_STOP = false; /* init ui->is_demo_mode_STOP as false*/


	//Write codes Main_chart_acc
	//Accelerometer chart
	ui->Main_chart_acc = lv_chart_create(ui->Main);
	lv_obj_set_pos(ui->Main_chart_acc, 215, 70);
	lv_obj_set_size(ui->Main_chart_acc, 244, 160);
	lv_obj_set_scrollbar_mode(ui->Main_chart_acc, LV_SCROLLBAR_MODE_OFF);

	//Set style for Main_chart_1. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->Main_chart_acc, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->Main_chart_acc, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->Main_chart_acc, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->Main_chart_acc, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->Main_chart_acc, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->Main_chart_acc, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->Main_chart_acc, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->Main_chart_acc, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->Main_chart_acc, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->Main_chart_acc, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->Main_chart_acc, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->Main_chart_acc, lv_color_make(0xe8, 0xe8, 0xe8), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->Main_chart_acc, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->Main_chart_acc, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_color(ui->Main_chart_acc, lv_color_make(0xe8, 0xe8, 0xe8), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_width(ui->Main_chart_acc, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_opa(ui->Main_chart_acc, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_chart_set_type(ui->Main_chart_acc, LV_CHART_TYPE_LINE);
	lv_chart_set_range(ui->Main_chart_acc,LV_CHART_AXIS_PRIMARY_Y, 0, 100);
	lv_chart_set_div_line_count(ui->Main_chart_acc, 5, 4);
	lv_chart_set_point_count(ui->Main_chart_acc, 7);
	lv_obj_add_flag(ui->Main_chart_acc, LV_OBJ_FLAG_HIDDEN);
	ui->is_showing_acc = false;

	//Write codes Main_ddlist_1
	ui->Main_ddlist_1 = lv_dropdown_create(ui->Main);
	lv_dropdown_set_options(ui->Main_ddlist_1, "all\nx\ny\nz");
	lv_obj_set_pos(ui->Main_ddlist_1, 370, 35);
	lv_obj_set_size(ui->Main_ddlist_1, 90, 30);

	//Write style for Main_ddlist_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_text_color(ui->Main_ddlist_1, lv_color_hex(0x0D3055), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->Main_ddlist_1, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->Main_ddlist_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->Main_ddlist_1, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->Main_ddlist_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->Main_ddlist_1, lv_color_hex(0xe1e6ee), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_side(ui->Main_ddlist_1, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->Main_ddlist_1, 8, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->Main_ddlist_1, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->Main_ddlist_1, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->Main_ddlist_1, 3, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->Main_ddlist_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->Main_ddlist_1, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->Main_ddlist_1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->Main_ddlist_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style state: LV_STATE_CHECKED for &style_Main_ddlist_1_extra_list_selected_checked
	static lv_style_t style_Main_ddlist_1_extra_list_selected_checked;
	ui_init_style(&style_Main_ddlist_1_extra_list_selected_checked);

	lv_style_set_border_width(&style_Main_ddlist_1_extra_list_selected_checked, 1);
	lv_style_set_border_opa(&style_Main_ddlist_1_extra_list_selected_checked, 255);
	lv_style_set_border_color(&style_Main_ddlist_1_extra_list_selected_checked, lv_color_hex(0xe1e6ee));
	lv_style_set_border_side(&style_Main_ddlist_1_extra_list_selected_checked, LV_BORDER_SIDE_FULL);
	lv_style_set_radius(&style_Main_ddlist_1_extra_list_selected_checked, 3);
	lv_style_set_bg_opa(&style_Main_ddlist_1_extra_list_selected_checked, 255);
	lv_style_set_bg_color(&style_Main_ddlist_1_extra_list_selected_checked, lv_color_hex(0x00a1b5));
	lv_style_set_bg_grad_dir(&style_Main_ddlist_1_extra_list_selected_checked, LV_GRAD_DIR_NONE);
	lv_obj_add_style(lv_dropdown_get_list(ui->Main_ddlist_1), &style_Main_ddlist_1_extra_list_selected_checked, LV_PART_SELECTED|LV_STATE_CHECKED);

	//Write style state: LV_STATE_DEFAULT for &style_Main_ddlist_1_extra_list_main_default
	static lv_style_t style_Main_ddlist_1_extra_list_main_default;
	ui_init_style(&style_Main_ddlist_1_extra_list_main_default);

	lv_style_set_max_height(&style_Main_ddlist_1_extra_list_main_default, 90);
	lv_style_set_text_color(&style_Main_ddlist_1_extra_list_main_default, lv_color_hex(0x0D3055));
	lv_style_set_text_font(&style_Main_ddlist_1_extra_list_main_default, &lv_font_montserratMedium_12);
	lv_style_set_text_opa(&style_Main_ddlist_1_extra_list_main_default, 255);
	lv_style_set_border_width(&style_Main_ddlist_1_extra_list_main_default, 1);
	lv_style_set_border_opa(&style_Main_ddlist_1_extra_list_main_default, 255);
	lv_style_set_border_color(&style_Main_ddlist_1_extra_list_main_default, lv_color_hex(0xe1e6ee));
	lv_style_set_border_side(&style_Main_ddlist_1_extra_list_main_default, LV_BORDER_SIDE_FULL);
	lv_style_set_radius(&style_Main_ddlist_1_extra_list_main_default, 3);
	lv_style_set_bg_opa(&style_Main_ddlist_1_extra_list_main_default, 255);
	lv_style_set_bg_color(&style_Main_ddlist_1_extra_list_main_default, lv_color_hex(0xffffff));
	lv_style_set_bg_grad_dir(&style_Main_ddlist_1_extra_list_main_default, LV_GRAD_DIR_NONE);
	lv_obj_add_style(lv_dropdown_get_list(ui->Main_ddlist_1), &style_Main_ddlist_1_extra_list_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style state: LV_STATE_DEFAULT for &style_Main_ddlist_1_extra_list_scrollbar_default
	static lv_style_t style_Main_ddlist_1_extra_list_scrollbar_default;
	ui_init_style(&style_Main_ddlist_1_extra_list_scrollbar_default);

	lv_style_set_radius(&style_Main_ddlist_1_extra_list_scrollbar_default, 3);
	lv_style_set_bg_opa(&style_Main_ddlist_1_extra_list_scrollbar_default, 255);
	lv_style_set_bg_color(&style_Main_ddlist_1_extra_list_scrollbar_default, lv_color_hex(0x00ff00));
	lv_style_set_bg_grad_dir(&style_Main_ddlist_1_extra_list_scrollbar_default, LV_GRAD_DIR_NONE);
	lv_obj_add_style(lv_dropdown_get_list(ui->Main_ddlist_1), &style_Main_ddlist_1_extra_list_scrollbar_default, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->Main_ddlist_1, LV_OBJ_FLAG_HIDDEN);

	//XYZ labels
	//Write codes green_legend
	ui->green_legend = lv_label_create(ui->Main);
	lv_label_set_text(ui->green_legend, "");
	lv_label_set_long_mode(ui->green_legend, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->green_legend, 260+120, 235);
	lv_obj_set_size(ui->green_legend, 10, 10);

	//Write style for green_legend, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->green_legend, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->green_legend, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->green_legend, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->green_legend, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->green_legend, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->green_legend, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->green_legend, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->green_legend, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->green_legend, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->green_legend, lv_color_hex(0x37a518), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->green_legend, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->green_legend, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->green_legend, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->green_legend, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->green_legend, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->green_legend, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes red_legend
	ui->red_legend = lv_label_create(ui->Main);
	lv_label_set_text(ui->red_legend, "");
	lv_label_set_long_mode(ui->red_legend, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->red_legend, 300+110, 235);
	lv_obj_set_size(ui->red_legend, 10, 10);

	//Write style for red_legend, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->red_legend, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->red_legend, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->red_legend, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->red_legend, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->red_legend, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->red_legend, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->red_legend, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->red_legend, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->red_legend, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->red_legend, lv_color_hex(0xcc2d2d), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->red_legend, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->red_legend, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->red_legend, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->red_legend, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->red_legend, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->red_legend, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes black_legend
	ui->black_legend = lv_label_create(ui->Main);
	lv_label_set_text(ui->black_legend, "");
	lv_label_set_long_mode(ui->black_legend, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->black_legend, 340+100, 235);
	lv_obj_set_size(ui->black_legend, 10, 10);

	//Write style for black_legend, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->black_legend, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->black_legend, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->black_legend, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->black_legend, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->black_legend, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->black_legend, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->black_legend, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->black_legend, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->black_legend, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->black_legend, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->black_legend, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->black_legend, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->black_legend, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->black_legend, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->black_legend, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->black_legend, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes Accel_chart_X_label
	ui->Accel_chart_X_label = lv_label_create(ui->Main);
	lv_label_set_text(ui->Accel_chart_X_label, "X");
	lv_label_set_long_mode(ui->Accel_chart_X_label, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->Accel_chart_X_label, 270+120, 235);
	lv_obj_set_size(ui->Accel_chart_X_label, 15, 15);

	//Write style for Accel_chart_X_label, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->Accel_chart_X_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->Accel_chart_X_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->Accel_chart_X_label, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->Accel_chart_X_label, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->Accel_chart_X_label, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->Accel_chart_X_label, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->Accel_chart_X_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->Accel_chart_X_label, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->Accel_chart_X_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->Accel_chart_X_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->Accel_chart_X_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->Accel_chart_X_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->Accel_chart_X_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->Accel_chart_X_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes Accel_chart_Y_label
	ui->Accel_chart_Y_label = lv_label_create(ui->Main);
	lv_label_set_text(ui->Accel_chart_Y_label, "Y");
	lv_label_set_long_mode(ui->Accel_chart_Y_label, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->Accel_chart_Y_label, 310+110, 235);
	lv_obj_set_size(ui->Accel_chart_Y_label, 15, 15);

	//Write style for Accel_chart_Y_label, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->Accel_chart_Y_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->Accel_chart_Y_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->Accel_chart_Y_label, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->Accel_chart_Y_label, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->Accel_chart_Y_label, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->Accel_chart_Y_label, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->Accel_chart_Y_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->Accel_chart_Y_label, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->Accel_chart_Y_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->Accel_chart_Y_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->Accel_chart_Y_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->Accel_chart_Y_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->Accel_chart_Y_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->Accel_chart_Y_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes Accel_chart_Z_label
	ui->Accel_chart_Z_label = lv_label_create(ui->Main);
	lv_label_set_text(ui->Accel_chart_Z_label, "Z");
	lv_label_set_long_mode(ui->Accel_chart_Z_label, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->Accel_chart_Z_label, 350+100, 235);
	lv_obj_set_size(ui->Accel_chart_Z_label, 15, 15);

	//Write style for Accel_chart_Z_label, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->Accel_chart_Z_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->Accel_chart_Z_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->Accel_chart_Z_label, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->Accel_chart_Z_label, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->Accel_chart_Z_label, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->Accel_chart_Z_label, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->Accel_chart_Z_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->Accel_chart_Z_label, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->Accel_chart_Z_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->Accel_chart_Z_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->Accel_chart_Z_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->Accel_chart_Z_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->Accel_chart_Z_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->Accel_chart_Z_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Hide Accelerometer axis labels and legends initially
	lv_obj_add_flag(ui->Accel_chart_X_label, LV_OBJ_FLAG_HIDDEN);
	lv_obj_add_flag(ui->Accel_chart_Y_label, LV_OBJ_FLAG_HIDDEN);
	lv_obj_add_flag(ui->Accel_chart_Z_label, LV_OBJ_FLAG_HIDDEN);
	lv_obj_add_flag(ui->green_legend, LV_OBJ_FLAG_HIDDEN);
	lv_obj_add_flag(ui->red_legend, LV_OBJ_FLAG_HIDDEN);
	lv_obj_add_flag(ui->black_legend, LV_OBJ_FLAG_HIDDEN);

	//Write codes Main_ta_1
	//Shows Message when Readme button is pressed
	ui->Main_ta_1 = lv_textarea_create(ui->Main);
	lv_textarea_set_text(ui->Main_ta_1, "a) The 'Heat Map' represents the two feature values used for SVM training (both normalized to the range of 0-1). "
			"\nThe horizontal axis represents the frequency corresponding to the maximum amplitude after performing FFT on the data. "
			"\nThe vertical axis represents RMS, which is the average energy of the training data."
			"\n\nb) The 'Health value' is the output result of the model (-1, 1), and the result is normalized to the range of -10 to 10."
			"A larger numerical value indicates a more normal system state.\n\n"
			"c) The Accelerometer value: Green->X, Red->Y, Black->Z\n\n");
	lv_textarea_set_placeholder_text(ui->Main_ta_1, "");
	lv_textarea_set_password_bullet(ui->Main_ta_1, "*");
	lv_textarea_set_password_mode(ui->Main_ta_1, false);
	lv_textarea_set_one_line(ui->Main_ta_1, false);
	lv_textarea_set_accepted_chars(ui->Main_ta_1, "");
	lv_textarea_set_max_length(ui->Main_ta_1, 32);
	lv_obj_set_pos(ui->Main_ta_1, 90, 110);
	lv_obj_set_size(ui->Main_ta_1, 320, 130);

	//Write style for Main_ta_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_text_color(ui->Main_ta_1, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->Main_ta_1, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->Main_ta_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->Main_ta_1, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->Main_ta_1, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->Main_ta_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->Main_ta_1, lv_color_hex(0x51a4e0), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->Main_ta_1, LV_GRAD_DIR_HOR, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->Main_ta_1, lv_color_hex(0x2087d4), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_main_stop(ui->Main_ta_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_stop(ui->Main_ta_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->Main_ta_1, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->Main_ta_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->Main_ta_1, lv_color_hex(0xe6e6e6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_side(ui->Main_ta_1, LV_BORDER_SIDE_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->Main_ta_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->Main_ta_1, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->Main_ta_1, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->Main_ta_1, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->Main_ta_1, 4, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style for Main_ta_1, Part: LV_PART_SCROLLBAR, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->Main_ta_1, 255, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->Main_ta_1, lv_color_hex(0x2195f6), LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->Main_ta_1, LV_GRAD_DIR_NONE, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->Main_ta_1, 0, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
	lv_obj_scroll_to_y(ui->Main_ta_1, 0, LV_ANIM_OFF);
	lv_obj_add_flag(ui->Main_ta_1, LV_OBJ_FLAG_HIDDEN); // first not be shown


    //lv_img_set_src(ui->Main_svm_toy,&svm_toy);
	//Init events for screen
    ui->is_in_MainScr = true;
	events_init_Main(ui);
}
