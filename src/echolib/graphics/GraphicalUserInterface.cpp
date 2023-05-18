#include "echolib/graphics/GraphicalUserInterface.hpp"
#include "display/lv_objx/lv_btnm.h"
#include "display/lv_objx/lv_chart.h"
#include "display/lv_objx/lv_cont.h"
#include "display/lv_objx/lv_label.h"
#include "echolib/Auton.hpp"
#include "display/lv_core/lv_obj.h"
#include "display/lv_core/lv_style.h"
#include "display/lv_fonts/lv_font_builtin.h"
#include "display/lv_misc/lv_color.h"
#include "display/lv_objx/lv_btn.h"
#include "display/lv_objx/lv_img.h"
#include "display/lv_objx/lv_imgbtn.h"
#include "pros/rtos.hpp"
#include <iostream>
#include <iomanip>
#include <limits>
#include <ostream>
#include <stdexcept>
#include <sstream>
#include <string>


//Display Drawing Helper Functions
lv_obj_t * GraphicalUserInterface::drawRectangle( int x, int y, int width, int height, int radius, int borderWidth, lv_color_t infillColor) {
  lv_obj_t * obj1 = lv_obj_create(lv_scr_act(), NULL);

  lv_style_t *style1 = (lv_style_t *)malloc( sizeof( lv_style_t ));
  lv_style_copy(style1, &lv_style_plain_color);    /*Copy a built-in style to initialize the new style*/
  style1->body.empty = 0;
  style1->body.main_color = infillColor;
	style1->body.grad_color=infillColor;
	style1->body.border.color = infillColor;
  style1->body.border.width = borderWidth;
	style1->body.radius=radius;
  style1->body.border.part = LV_BORDER_FULL;

  lv_obj_set_style(obj1, style1);
  lv_obj_set_pos(obj1, x, y);
  lv_obj_set_size(obj1, width, height);

  return obj1;
}

lv_obj_t * GraphicalUserInterface::drawRectangle( int x, int y, int width, int height, int radius, int borderWidth, lv_color_t infillColor, lv_color_t gradColor, lv_color_t outlineColor) {
  lv_obj_t * obj1 = lv_obj_create(lv_scr_act(), NULL);

  lv_style_t *style1 = (lv_style_t *)malloc( sizeof( lv_style_t ));
  lv_style_copy(style1, &lv_style_plain_color);    /*Copy a built-in style to initialize the new style*/
  style1->body.empty = 0;
  style1->body.main_color = infillColor;
	style1->body.grad_color=gradColor;
	style1->body.border.color = outlineColor;
  style1->body.border.width = borderWidth;
	style1->body.radius=radius;
  style1->body.border.part = LV_BORDER_FULL;

  lv_obj_set_style(obj1, style1);
  lv_obj_set_pos(obj1, x, y);
  lv_obj_set_size(obj1, width, height);

  return obj1;
}

lv_obj_t * GraphicalUserInterface::printText(int x, int y,const char* text){
  lv_obj_t * obj1 = lv_label_create(lv_scr_act(), NULL);

  lv_label_set_text(obj1, text);
  lv_obj_set_pos(obj1, x, y);

  return obj1;
}

lv_obj_t * GraphicalUserInterface::printTempText(int x, int y,const char* text){
  lv_obj_t * obj1 = lv_label_create(lv_scr_act(), NULL);

  static lv_style_t style_txt;
    lv_style_copy(&style_txt, &lv_style_plain);
    style_txt.text.font = &lv_font_dejavu_10;
    style_txt.text.color = WANNABE_WHITE;

  lv_label_set_text(obj1, text);
  lv_label_set_style(obj1, &style_txt);
  lv_obj_set_pos(obj1, x, y);

  return obj1;
}

lv_obj_t * GraphicalUserInterface::printTextCenteredAt(int x, int y,const char* text){
    lv_obj_t * obj1 = lv_label_create(lv_scr_act(), NULL);

    lv_label_set_text(obj1, text);
    lv_obj_align(obj1,NULL,LV_ALIGN_CENTER, x, y);

    return obj1;
}

lv_obj_t * GraphicalUserInterface::printTextCenteredAt(int x, int y,const char* text,lv_color_t color){
    static lv_style_t errorStyle;
    lv_style_copy(&errorStyle,&lv_style_plain);
    errorStyle.text.color = color;
    errorStyle.text.font = &lv_font_dejavu_20;

    lv_obj_t * obj1 = lv_label_create(lv_scr_act(), NULL);
    lv_obj_set_style(obj1, &errorStyle);

    lv_label_set_text(obj1, text);
    lv_obj_align(obj1,NULL,LV_ALIGN_CENTER, 0, 0);

    return obj1;
}

//User Functions
void GraphicalUserInterface::setTitle(std::string iTitle){
    title = iTitle;
    refreshDisplay();
}

void GraphicalUserInterface::setTab(int tab){
    selectedTab = tab;
    menuIndexChanged = true;
}

void GraphicalUserInterface::setIdleImage(lv_img_dsc_t image){
    Idle_Screen = image;
    refreshDisplay();
}


void GraphicalUserInterface::addAutons(std::vector<Auton> autons){
    for(int i=0; i<autons.size();i++){
        autonomousNames[i] = autons.at(i).autonomousName;
        autonomousDescs[i] = autons.at(i).autonomousDesc;
        autonomousCallbacks[i] = autons.at(i).autonomousCallback;
    }
    refreshDisplay();
}

void GraphicalUserInterface::executeSelectedAuton(){
    executeAuton(selectedAutonomousIndex);
}

void GraphicalUserInterface::executeAuton(int autonomousIndex){
    if(autonomousCallbacks[autonomousIndex] == nullptr)
        return;
    autonomousRunning = true;
    autonomousStartTime = pros::millis();
    autonomousCallbacks[autonomousIndex]();
    autonomousRunning=false;
}

 void GraphicalUserInterface::useGraphData(std::vector<GraphData> iGraphData){
    graphData = iGraphData;
    graphingUtilityInitialized=true;
}

void GraphicalUserInterface::printDiagnosticsData(int row, std::string data){
    if(diagnosticData.size()<row+1){
        diagnosticData.resize(row+1);
    }

    diagnosticData.at(row) = data;
    diagnosticsInitialized = true;
}

void GraphicalUserInterface::clearDiagnosticsData(){
    diagnosticData.clear();
}

void GraphicalUserInterface::addTemperatureData(std::vector<MotorData> iMotorData){
    motorData = iMotorData;
    temperatureInitialized = true;
}

//Callbacks
lv_res_t GraphicalUserInterface::tabButtonCallback(lv_obj_t * obj){
    lv_imgbtn_set_state(autonomousSelectorButton,LV_BTN_STATE_REL);
    lv_imgbtn_set_state(graphingUtilityButton,LV_BTN_STATE_REL);
    lv_imgbtn_set_state(diagnosticsButton,LV_BTN_STATE_REL);
    lv_imgbtn_set_state(temperatureGaugeButton,LV_BTN_STATE_REL);

    int tab = lv_obj_get_free_num(obj);

    if(tab == AUTONOMOUS_SELECTOR_TAB){
        lv_imgbtn_set_state(autonomousSelectorButton,LV_BTN_STATE_TGL_REL);
    }else if(tab == GRAPHING_UTILITY_TAB){
        lv_imgbtn_set_state(graphingUtilityButton,LV_BTN_STATE_TGL_REL);
    }else if(tab == DIAGNOSTICS_TAB){
        lv_imgbtn_set_state(diagnosticsButton,LV_BTN_STATE_TGL_REL);
    }else if(tab == TEMPERATURE_GAUGE_TAB){
        lv_imgbtn_set_state(temperatureGaugeButton,LV_BTN_STATE_TGL_REL);
    }

    selectedTab = tab;
    menuIndexChanged = true;

    return LV_RES_OK;
}

lv_res_t GraphicalUserInterface::scrollButtonCallback(lv_obj_t * obj){
    int tab = lv_obj_get_free_num(obj);

    if(tab == SCROLL_UP){
        startIndex--;
    }else if(tab == SCROLL_DOWN){
        startIndex++;
    }

    return LV_RES_OK;
}

lv_res_t GraphicalUserInterface::backButtonCallback(lv_obj_t * obj){
  splashScreenEnabled = !splashScreenEnabled;

  if(splashScreenEnabled){
    lv_obj_set_pos(backButton, 0, 0);
  }else{
    lv_obj_set_pos(backButton, 5, 209);
  }

  menuIndexChanged=true;
  
  return LV_RES_OK;
}

lv_res_t GraphicalUserInterface::autonomousButtonMatrixCallback(lv_obj_t * buttonMatrix, const char *txt){
    std::stringstream strValue;
    int intValue;

    strValue << txt;
    strValue >> intValue;

    intValue -= 1;

    selectedAutonomousIndex=intValue;

    lv_label_set_text(autonomousName, autonomousNames[selectedAutonomousIndex]);
    lv_label_set_text(autonomousDesc, autonomousDescs[selectedAutonomousIndex]);


    return LV_RES_OK;
}

lv_res_t GraphicalUserInterface::executeAutonomousButtonCallback(lv_obj_t * button){
    pros::delay(1500);
    executionButtonPressed = true;
    return LV_RES_OK;
}

//Initialization Helper Functions
void GraphicalUserInterface::defineColors(){
    DARK_BUT_NOT_BLACK = LV_COLOR_MAKE(35, 39, 42);
    NOT_QUITE_BLACK = LV_COLOR_MAKE(44, 47, 51);
    KINDA_GRAY = LV_COLOR_MAKE(48, 53, 56);
    WANNABE_WHITE = LV_COLOR_MAKE(143, 149, 145);
    CHARTREUSE = LV_COLOR_MAKE(67,181,129);
    CRIMSON = LV_COLOR_MAKE(220,20,60);
    MARIGOLD = LV_COLOR_MAKE(248,195,0);
    ORANGE = LV_COLOR_MAKE(245, 119, 49);
    HOT_PINK = LV_COLOR_MAKE(255, 20, 147);
    AQUA = LV_COLOR_MAKE(0, 255, 255);
    LAVENDER = LV_COLOR_MAKE(166, 82, 187);
    BLURPLE = LV_COLOR_MAKE(114, 137, 218);
    OLIVE = LV_COLOR_MAKE(128,128,0);
}

void GraphicalUserInterface::createBackground(){
    drawRectangle(0, 0, 480, 240, 0, 1, DARK_BUT_NOT_BLACK); //Outline 480 x 240
    drawRectangle(50, 21, 440, 219, 5, 1, NOT_QUITE_BLACK); //Background
    drawRectangle(5, 21, 40, 3, 2, 1, WANNABE_WHITE); //Border

    titleLabel = lv_label_create(lv_scr_act(), NULL);
        lv_label_set_text(titleLabel, title.c_str());
        lv_obj_set_pos(titleLabel, 4, 2);
}

void GraphicalUserInterface::setupStartTab(){
    if(selectedTab==AUTONOMOUS_SELECTOR_TAB){
        lv_imgbtn_set_state(autonomousSelectorButton,LV_BTN_STATE_TGL_REL);
    }else if(selectedTab==GRAPHING_UTILITY_TAB){
        lv_imgbtn_set_state(graphingUtilityButton,LV_BTN_STATE_TGL_REL);
    }else if(selectedTab==DIAGNOSTICS_TAB){
        lv_imgbtn_set_state(diagnosticsButton,LV_BTN_STATE_TGL_REL);
    }else if(selectedTab==TEMPERATURE_GAUGE_TAB){
        lv_imgbtn_set_state(temperatureGaugeButton,LV_BTN_STATE_TGL_REL);
    }

    menuIndexChanged = true;
}

void GraphicalUserInterface::createStyles(){
     /*Create a new style for the button matrix back ground*/
    lv_style_copy(&buttonBackgroundStyle, &lv_style_plain);
        buttonBackgroundStyle.body.main_color = DARK_BUT_NOT_BLACK;
        buttonBackgroundStyle.body.grad_color = DARK_BUT_NOT_BLACK;
        buttonBackgroundStyle.body.padding.hor = 0;
        buttonBackgroundStyle.body.padding.ver = 0;
        buttonBackgroundStyle.body.padding.inner = 0;

    /*Create 2 button styles*/
    lv_style_copy(&buttonReleasedStyle, &lv_style_btn_rel);
        buttonReleasedStyle.body.main_color = DARK_BUT_NOT_BLACK;
        buttonReleasedStyle.body.grad_color = DARK_BUT_NOT_BLACK;
        buttonReleasedStyle.body.border.color = WANNABE_WHITE;
        buttonReleasedStyle.body.border.width = 1;
        buttonReleasedStyle.body.border.opa = LV_OPA_50;
        buttonReleasedStyle.body.radius = 5;
        buttonReleasedStyle.text.color = WANNABE_WHITE;

    lv_style_copy(&buttonPressedStyle, &lv_style_btn_pr);
        buttonPressedStyle.body.main_color = KINDA_GRAY;
        buttonPressedStyle.body.grad_color = KINDA_GRAY;
        buttonPressedStyle.text.color = WANNABE_WHITE;
        buttonPressedStyle.body.border.color = WANNABE_WHITE;

    lv_style_copy(&buttonToggleStyle, &lv_style_btn_pr);
        buttonToggleStyle.body.main_color = KINDA_GRAY;
        buttonToggleStyle.body.grad_color = KINDA_GRAY;
        buttonToggleStyle.text.color = WANNABE_WHITE;
        buttonToggleStyle.body.border.color = CHARTREUSE;

    lv_style_copy(&chartStyle, &lv_style_pretty);
        chartStyle.body.main_color = DARK_BUT_NOT_BLACK;
        chartStyle.body.grad_color = DARK_BUT_NOT_BLACK;
        chartStyle.body.border.color =WANNABE_WHITE;
        chartStyle.body.border.width = 2;
        chartStyle.body.border.opa = 100;
        chartStyle.line.color = WANNABE_WHITE;
        chartStyle.line.opa = 100;
        chartStyle.line.rounded = false;

    lv_style_copy(&pageStyle, &lv_style_pretty);
        pageStyle.body.main_color = NOT_QUITE_BLACK;
        pageStyle.body.grad_color = NOT_QUITE_BLACK;
        pageStyle.body.border.color = WANNABE_WHITE;
        pageStyle.body.border.width = 1;
        pageStyle.body.border.opa = LV_OPA_50;
        pageStyle.body.radius = 3;
        pageStyle.text.color = WANNABE_WHITE;

    lv_style_copy(&scrollBarStyle, &lv_style_pretty);
        scrollBarStyle.body.main_color = WANNABE_WHITE;
        scrollBarStyle.body.grad_color = WANNABE_WHITE;
        scrollBarStyle.body.border.color = WANNABE_WHITE;
        scrollBarStyle.body.border.width = 1;
        scrollBarStyle.body.border.opa = LV_OPA_50;
        scrollBarStyle.body.radius = 3;
        scrollBarStyle.text.color = WANNABE_WHITE;

    lv_style_copy(&gaugeStyle, &lv_style_pretty_color);
        gaugeStyle.body.main_color = LV_COLOR_BLUE; /*Line color at the beginning*/
        gaugeStyle.body.grad_color = LV_COLOR_RED; /*Line color at the end*/
        gaugeStyle.body.padding.hor = 10; /*Scale line length*/
        gaugeStyle.body.padding.inner = 8 ; /*Scale label padding*/
        gaugeStyle.body.border.color = WANNABE_WHITE; /*Needle middle circle color*/
        gaugeStyle.line.width = 3;
        gaugeStyle.text.color = WANNABE_WHITE;
        gaugeStyle.line.color = LV_COLOR_RED; /*Line color after the critical value*/
}

void GraphicalUserInterface::createButtons(){
    LV_IMG_DECLARE(Autonomous_Button_Pressed);
    LV_IMG_DECLARE(Autonomous_Button_Released);
    LV_IMG_DECLARE(Autonomous_Button_Toggled_Pressed);
    LV_IMG_DECLARE(Autonomous_Button_Toggled_Released);

    LV_IMG_DECLARE(Diagnostics_Button_Pressed);
    LV_IMG_DECLARE(Diagnostics_Button_Released);
    LV_IMG_DECLARE(Diagnostics_Button_Toggled_Pressed);
    LV_IMG_DECLARE(Diagnostics_Button_Toggled_Released);

    LV_IMG_DECLARE(Sensors_Button_Pressed);
    LV_IMG_DECLARE(Sensors_Button_Released);
    LV_IMG_DECLARE(Sensors_Button_Toggled_Pressed);
    LV_IMG_DECLARE(Sensors_Button_Toggled_Released);

    LV_IMG_DECLARE(Temperature_Button_Pressed);
    LV_IMG_DECLARE(Temperature_Button_Released);
    LV_IMG_DECLARE(Temperature_Button_Toggled_Pressed);
    LV_IMG_DECLARE(Temperature_Button_Toggled_Released);

    LV_IMG_DECLARE(Back_Button_Pressed);
    LV_IMG_DECLARE(Back_Button_Released);

    autonomousSelectorButton = lv_imgbtn_create(lv_scr_act(), nullptr);
        lv_imgbtn_set_action(autonomousSelectorButton, LV_BTN_ACTION_CLICK, tabButtonCallback);
        lv_imgbtn_set_src(autonomousSelectorButton, LV_BTN_STATE_REL, &Autonomous_Button_Released);
        lv_imgbtn_set_src(autonomousSelectorButton, LV_BTN_STATE_PR, &Autonomous_Button_Pressed);
        lv_imgbtn_set_src(autonomousSelectorButton, LV_BTN_STATE_TGL_REL, &Autonomous_Button_Toggled_Released);
        lv_imgbtn_set_src(autonomousSelectorButton, LV_BTN_STATE_TGL_PR, &Autonomous_Button_Toggled_Pressed);
        lv_imgbtn_set_toggle(autonomousSelectorButton, true);
        lv_obj_set_pos(autonomousSelectorButton, 5, 29);
        lv_obj_set_free_num(autonomousSelectorButton, AUTONOMOUS_SELECTOR_TAB);

    graphingUtilityButton = lv_imgbtn_create(lv_scr_act(), nullptr);
        lv_imgbtn_set_action(graphingUtilityButton, LV_BTN_ACTION_CLICK, tabButtonCallback);
        lv_imgbtn_set_src(graphingUtilityButton, LV_BTN_STATE_REL, &Diagnostics_Button_Released);
        lv_imgbtn_set_src(graphingUtilityButton, LV_BTN_STATE_PR, &Diagnostics_Button_Pressed);
        lv_imgbtn_set_src(graphingUtilityButton, LV_BTN_STATE_TGL_REL, &Diagnostics_Button_Toggled_Released);
        lv_imgbtn_set_src(graphingUtilityButton, LV_BTN_STATE_TGL_PR, &Diagnostics_Button_Toggled_Pressed);
        lv_imgbtn_set_toggle(graphingUtilityButton, true);
        lv_obj_set_pos(graphingUtilityButton, 5,74);
        lv_obj_set_free_num(graphingUtilityButton,GRAPHING_UTILITY_TAB);

    diagnosticsButton =lv_imgbtn_create(lv_scr_act(), nullptr);
        lv_imgbtn_set_action(diagnosticsButton, LV_BTN_ACTION_CLICK, tabButtonCallback);
        lv_imgbtn_set_src(diagnosticsButton, LV_BTN_STATE_REL, &Sensors_Button_Released);
        lv_imgbtn_set_src(diagnosticsButton, LV_BTN_STATE_PR, &Sensors_Button_Pressed);
        lv_imgbtn_set_src(diagnosticsButton, LV_BTN_STATE_TGL_REL, &Sensors_Button_Toggled_Released);
        lv_imgbtn_set_src(diagnosticsButton, LV_BTN_STATE_TGL_PR, &Sensors_Button_Toggled_Pressed);
        lv_imgbtn_set_toggle(diagnosticsButton, true);
        lv_obj_set_pos(diagnosticsButton, 5, 119);
        lv_obj_set_free_num(diagnosticsButton, DIAGNOSTICS_TAB);

    temperatureGaugeButton =lv_imgbtn_create(lv_scr_act(), nullptr);
        lv_imgbtn_set_action(temperatureGaugeButton, LV_BTN_ACTION_CLICK, tabButtonCallback);
        lv_imgbtn_set_src(temperatureGaugeButton, LV_BTN_STATE_REL, &Temperature_Button_Released);
        lv_imgbtn_set_src(temperatureGaugeButton, LV_BTN_STATE_PR, &Temperature_Button_Pressed);
        lv_imgbtn_set_src(temperatureGaugeButton, LV_BTN_STATE_TGL_REL, &Temperature_Button_Toggled_Released);
        lv_imgbtn_set_src(temperatureGaugeButton, LV_BTN_STATE_TGL_PR, &Temperature_Button_Toggled_Pressed);
        lv_imgbtn_set_toggle(temperatureGaugeButton, true);
        lv_obj_set_pos(temperatureGaugeButton, 5, 164);
        lv_obj_set_free_num(temperatureGaugeButton,TEMPERATURE_GAUGE_TAB);

    backButton =lv_imgbtn_create(lv_scr_act(), nullptr);
        lv_imgbtn_set_action(backButton, LV_BTN_ACTION_CLICK, backButtonCallback);
        lv_imgbtn_set_src(backButton, LV_BTN_STATE_REL, &Back_Button_Released);
        lv_imgbtn_set_src(backButton, LV_BTN_STATE_PR, &Back_Button_Pressed);
        lv_imgbtn_set_src(backButton, LV_BTN_STATE_TGL_REL, &Idle_Screen);
        lv_imgbtn_set_src(backButton, LV_BTN_STATE_TGL_PR, &Idle_Screen);
        lv_imgbtn_set_toggle(backButton, true);

    if(splashScreenEnabled){
        lv_obj_set_pos(backButton, 0, 0);
        lv_imgbtn_set_state(backButton,LV_BTN_STATE_TGL_REL);
    }else{
        lv_obj_set_pos(backButton, 5, 209);
        lv_imgbtn_set_state(backButton,LV_BTN_STATE_REL);
    }
}

void GraphicalUserInterface::refreshDisplay(){
    lv_scr_act();
    graphicsInit();
}

//Initialization Functions
void GraphicalUserInterface::graphicsInit(){
    defineColors();

    createBackground();

    createButtons();

    createStyles();

    setupStartTab();
}

void GraphicalUserInterface::autonomousSelectorTabInit(){
    autonomousButtonMatrix = lv_btnm_create(lv_scr_act(), NULL);
        lv_btnm_set_map(autonomousButtonMatrix, autonomousButtons);
        lv_btnm_set_toggle(autonomousButtonMatrix, true, selectedAutonomousIndex);
        lv_btnm_set_action(autonomousButtonMatrix, autonomousButtonMatrixCallback);
        lv_obj_set_size(autonomousButtonMatrix, 207, 205);
        lv_obj_set_pos(autonomousButtonMatrix, 59, 30);
        lv_btnm_set_style(autonomousButtonMatrix, LV_BTNM_STYLE_BG, &buttonBackgroundStyle);
        lv_btnm_set_style(autonomousButtonMatrix, LV_BTNM_STYLE_BTN_REL, &buttonReleasedStyle);
        lv_btnm_set_style(autonomousButtonMatrix, LV_BTNM_STYLE_BTN_PR, &buttonPressedStyle);
        lv_btnm_set_style(autonomousButtonMatrix, LV_BTNM_STYLE_BTN_TGL_REL, &buttonToggleStyle);
        lv_btnm_set_style(autonomousButtonMatrix, LV_BTNM_STYLE_BTN_TGL_PR, &buttonToggleStyle);

    autonomousName = lv_label_create(lv_scr_act(), NULL);
        lv_label_set_long_mode(autonomousName, LV_LABEL_LONG_BREAK);
        lv_label_set_align(autonomousName,LV_LABEL_ALIGN_CENTER);
        lv_label_set_text(autonomousName, autonomousNames[selectedAutonomousIndex]);
        lv_obj_set_width(autonomousName, 195);
        lv_obj_align(autonomousName, NULL, LV_ALIGN_IN_TOP_MID, 131, 30);

    autonomousDesc = lv_label_create(lv_scr_act(), NULL);
        lv_label_set_long_mode(autonomousDesc, LV_LABEL_LONG_BREAK);
        lv_label_set_style(autonomousDesc, &buttonPressedStyle);
        lv_label_set_align(autonomousDesc,LV_LABEL_ALIGN_CENTER);
        lv_label_set_text(autonomousDesc, autonomousDescs[selectedAutonomousIndex]);
        lv_obj_set_height(autonomousDesc, 195);//195
        lv_obj_set_width(autonomousDesc, 170);//195
        lv_obj_align(autonomousDesc, NULL, LV_ALIGN_IN_TOP_MID, 131, 60);

    drawRectangle(276, 50, 3, 180, 3, 1, WANNABE_WHITE);

   /*Create a normal button*/
    executeAutonomousButton = lv_btn_create(lv_scr_act(), NULL);
        lv_btn_set_action(executeAutonomousButton, LV_BTN_ACTION_CLICK, executeAutonomousButtonCallback);
        lv_btn_set_toggle(executeAutonomousButton, false);
        lv_btn_set_style(executeAutonomousButton, LV_BTN_STYLE_REL, &buttonReleasedStyle);
        lv_btn_set_style(executeAutonomousButton, LV_BTN_STYLE_PR, &buttonPressedStyle);
        lv_label_set_style(executeAutonomousButton, &buttonReleasedStyle);
        lv_obj_set_size(executeAutonomousButton, 60, 40);
        lv_obj_align(executeAutonomousButton, NULL, LV_ALIGN_IN_TOP_MID, 80, 235-46);
        lv_obj_t * label = lv_label_create(executeAutonomousButton, NULL);
        lv_label_set_text(label, "Run");
    
    elapsedTimeLabel = lv_label_create(lv_scr_act(), NULL);
        lv_label_set_style(elapsedTimeLabel, &buttonPressedStyle);
        lv_label_set_text(elapsedTimeLabel, "Time: 0 s");
        lv_obj_align(elapsedTimeLabel, NULL, LV_ALIGN_IN_TOP_MID, 160, 235-36);
        lv_label_set_long_mode(elapsedTimeLabel, LV_LABEL_LONG_SCROLL);
}

void GraphicalUserInterface::graphingUtilityTabInit(){
     if(!graphingUtilityInitialized)
        return;
    /*Create a chart*/
    static lv_color_t graph_colors[] = {CRIMSON,LV_COLOR_PURPLE,MARIGOLD,HOT_PINK};

    chart = lv_chart_create(lv_scr_act(), NULL);
    lv_obj_set_size(chart, 389, 175);
    lv_obj_set_style(chart, &chartStyle);
    lv_obj_set_pos(chart,59,30);
    lv_chart_set_type(chart, LV_CHART_TYPE_LINE); /*Show lines and points too*/
    lv_chart_set_point_count(chart,20);
    lv_chart_set_div_line_count(chart, 10, 3);
    lv_chart_set_series_width(chart, 2); /*Line width and point radious*/
    lv_chart_set_range(chart, 0, 110);

    for(int i=0;i<4;i++){
        if(graphData.size() > i){
            drawRectangle(59 +100*i, 212, 20, 20, 2, 1, graph_colors[i],graph_colors[i], WANNABE_WHITE);
            printText(84 + 100*i, 212, graphData.at(i).dataName.c_str());
            for(int j=0; j < seriesPointsVector.at(i).size();j++){
                seriesPointsVector.at(i)[j] = *graphData.at(i).dataUnit;
            }
            seriesVector.at(i) = lv_chart_add_series(chart, graph_colors[i]);
        }
    }

    for(int i=0;i<12;i++){
        int number= 110-i*10;
        std::stringstream strs;
        strs << number;
        std::string temp_str = strs.str();
        printText(451, 25 + i*15, temp_str.c_str());
    }
    
    lv_chart_refresh(chart); 
}

void GraphicalUserInterface::diagnosticsTabInit(){
     if(!diagnosticsInitialized)
        return;
    std::string displayData = "";
    if(diagnosticData.size()!=0){
        for(int i=0; i<diagnosticData.size();i++){
            displayData += diagnosticData.at(i) + "\n";
        }
    }
    /*Create a page*/
    diagnosticPage = lv_page_create(lv_scr_act(), NULL);
        lv_obj_set_size(diagnosticPage, 410, 200);
        lv_obj_align(diagnosticPage, NULL, LV_ALIGN_CENTER, 25, 11);
        lv_page_set_style(diagnosticPage, LV_PAGE_STYLE_BG, &pageStyle); /*Set the scrollbar style*/
        lv_page_set_style(diagnosticPage, LV_PAGE_STYLE_SB, &scrollBarStyle); /*Set the scrollbar style*/
        lv_page_set_sb_mode(diagnosticPage, LV_SB_MODE_AUTO); /*Show scroll bars is scrolling is possible*/
    /*Create a label on the page*/
    diagnosticDataLabel = lv_label_create(diagnosticPage, NULL);
        lv_label_set_long_mode(diagnosticDataLabel, LV_LABEL_LONG_BREAK); /*Automatically break long lines*/
        lv_obj_set_width(diagnosticDataLabel, lv_page_get_scrl_width(diagnosticPage)); /*Set the width. Lines will break here*/
        lv_label_set_text(diagnosticDataLabel, displayData.c_str());
}

void GraphicalUserInterface::temperatureGaugeTabInit(){
     if(!temperatureInitialized)
        return;
    LV_IMG_DECLARE(Scroll_Up_Button_Released);
    LV_IMG_DECLARE(Scroll_Up_Button_Pressed);
    LV_IMG_DECLARE(Scroll_Down_Button_Released);
    LV_IMG_DECLARE(Scroll_Down_Button_Pressed);
    
    static lv_color_t needle_colors[] = {CHARTREUSE, MARIGOLD, ORANGE, HOT_PINK, AQUA, LAVENDER, BLURPLE, OLIVE};

    tempGauge = lv_gauge_create(lv_scr_act(), NULL);
    lv_gauge_set_style(tempGauge, &gaugeStyle);
    lv_gauge_set_needle_count(tempGauge, 8, needle_colors);
    lv_gauge_set_range(tempGauge, 10, 80);
    lv_gauge_set_scale(tempGauge, 280, 36, 8);
    lv_obj_align(tempGauge, NULL, LV_ALIGN_CENTER, 0, 20);
    lv_obj_set_size(tempGauge, 207, 205);
    lv_obj_set_pos(tempGauge, 59, 40);

    lv_gauge_set_value(tempGauge, 0, 10);
    lv_gauge_set_value(tempGauge, 1, 20);
    lv_gauge_set_value(tempGauge, 2, 30);
    lv_gauge_set_value(tempGauge, 3, 40);
    lv_gauge_set_value(tempGauge, 4, 50);
    lv_gauge_set_value(tempGauge, 5, 60);
    lv_gauge_set_value(tempGauge, 6, 70);
    lv_gauge_set_value(tempGauge, 7, 80);

    for(int i=0;i<=7;i++){
        tempColorBoxes.push_back(drawRectangle(290, 30 + 26*i, 20, 20, 5, 1, needle_colors[(i + startIndex)%8],needle_colors[(i + startIndex)%8], WANNABE_WHITE));
        tempLabels.push_back(printText(320, 30 + 26*i, motorData.at((i + startIndex)%8).motorName.c_str()));
    }

    for(int i=0;i<motorData.size();i++){
        pros::Motor m(motorData.at(i).motorPort);
        motors.push_back(m);
    }

    upScrollButton = lv_imgbtn_create(lv_scr_act(), nullptr);
        lv_imgbtn_set_action(upScrollButton, LV_BTN_ACTION_CLICK, scrollButtonCallback);
        lv_imgbtn_set_src(upScrollButton, LV_BTN_STATE_REL, &Scroll_Up_Button_Pressed);
        lv_imgbtn_set_src(upScrollButton, LV_BTN_STATE_PR, &Scroll_Up_Button_Pressed);
        lv_imgbtn_set_toggle(upScrollButton, false);
        lv_obj_set_pos(upScrollButton, 450, 21);
        lv_imgbtn_set_state(upScrollButton,LV_BTN_STATE_REL);
        lv_obj_set_free_num(upScrollButton, SCROLL_UP);

     downScrollButton = lv_imgbtn_create(lv_scr_act(), nullptr);
        lv_imgbtn_set_action(downScrollButton, LV_BTN_ACTION_CLICK, scrollButtonCallback);
        lv_imgbtn_set_src(downScrollButton, LV_BTN_STATE_REL, &Scroll_Down_Button_Released);
        lv_imgbtn_set_src(downScrollButton, LV_BTN_STATE_PR, &Scroll_Down_Button_Pressed);
        lv_imgbtn_set_toggle(downScrollButton, false);
        lv_obj_set_pos(downScrollButton, 450, 130);
        lv_imgbtn_set_state(downScrollButton,LV_BTN_STATE_REL);
        lv_obj_set_free_num(downScrollButton, SCROLL_DOWN);
}

//Update Functions
void GraphicalUserInterface::autonomousSelectorTabUpdate(){
    if(autonomousRunning){
        elapsedAutonomousTime = (pros::millis() - autonomousStartTime)/1000.0;

        std::stringstream ss;
        ss << std::fixed << std::setprecision(2) << elapsedAutonomousTime;
        std::string formattedOutput = ss.str();

        std::string elapsedTimeStr = "Time: " + formattedOutput + " s";
        lv_label_set_text(elapsedTimeLabel, elapsedTimeStr.c_str());
    }
}

void GraphicalUserInterface::graphingUtilityTabUpdate(){
    if(!graphingUtilityInitialized)
        return;
    for(int i=0; i < seriesPointsVector.at(0).size()-1;i++){
        for(int j=0; j<seriesPointsVector.size(); j++){
            seriesPointsVector.at(j)[i] = seriesPointsVector.at(j)[i+1];
        }
    }

    for(int j=0; j<seriesPointsVector.size(); j++){
        seriesPointsVector.at(j)[seriesPointsVector.at(j).size()-1] = *graphData.at(j).dataUnit;
    }

  for(int i=0; i < seriesPointsVector.at(0).size(); i++){
    for(int j=0; j<seriesPointsVector.size(); j++){
        seriesVector.at(j)->points[i] = seriesPointsVector.at(j).at(i);
    }
  }

  lv_chart_refresh(chart);
}

void GraphicalUserInterface::diagnosticsTabUpdate(){
    if(!diagnosticsInitialized)
        return;
   std::string displayData = "";
    if(diagnosticData.size()!=0){
        for(int i=0; i<diagnosticData.size();i++){
            displayData += diagnosticData.at(i) + "\n";
        }
    }

    lv_label_set_text(diagnosticDataLabel, displayData.c_str());
}

void GraphicalUserInterface::temperatureGaugeTabUpdate(){
    if(!temperatureInitialized)
        return;
    static lv_color_t needle_colors[] = {CHARTREUSE, MARIGOLD, ORANGE, HOT_PINK, AQUA, LAVENDER, BLURPLE, OLIVE};

    

     for(int i=0;i<=7;i++){
        lv_obj_del(tempLabels.at(i));
        lv_obj_del(tempColorBoxes.at(i));

        tempColorBoxes.at(i) = drawRectangle(290, 30 + 26*i, 20, 20, 5, 1, needle_colors[(i + startIndex)%8],needle_colors[(i + startIndex)%8], WANNABE_WHITE);
        tempLabels.at(i) = printText(320, 30 + 26*i, motorData.at((i + startIndex)%motorData.size()).motorName.c_str());

        
        lv_gauge_set_value(tempGauge, i, motors.at(i).get_temperature());
    }
}


//Asynchronous Display Task
void GraphicalUserInterface::asyncDisplayTask(){
    refreshDisplay();
    while(true){
        if(menuIndexChanged){
            lv_obj_clean(lv_scr_act());
            graphicsInit();
            if(!splashScreenEnabled){
                if(selectedTab==AUTONOMOUS_SELECTOR_TAB){
                    autonomousSelectorTabInit();
                }else if(selectedTab==GRAPHING_UTILITY_TAB){
                    graphingUtilityTabInit();
                }else if(selectedTab==DIAGNOSTICS_TAB){
                    diagnosticsTabInit();
                }else if(selectedTab==TEMPERATURE_GAUGE_TAB){
                    temperatureGaugeTabInit();
                }
            }
            menuIndexChanged=false;
        }
        if(!splashScreenEnabled){
            if(selectedTab==GRAPHING_UTILITY_TAB){
                graphingUtilityTabUpdate();
            }else if(selectedTab==DIAGNOSTICS_TAB){
                diagnosticsTabUpdate();
            }else if(selectedTab==TEMPERATURE_GAUGE_TAB){
                temperatureGaugeTabUpdate();
            }

            if(executionButtonPressed){
                executionButtonPressed=false;
                executeSelectedAuton();
            }
        }

        pros::delay(200);
    }
}




void GraphicalUserInterface::asyncAutonomousTask(){
    while(true){
        autonomousSelectorTabUpdate();
        pros::delay(50);
    }
}