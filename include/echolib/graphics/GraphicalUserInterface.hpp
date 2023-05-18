#pragma once
#include "main.h"
#include <algorithm>
#include <cstddef>
#include <iostream>
#include <list>
#include <array>
#include "echolib/Auton.hpp"




class GraphicalUserInterface {

    public:
        GraphicalUserInterface(std::string iTitle, int startTab = 0){
            title = iTitle;

            if(startTab <0 || startTab >3){
                startTab=0;
            }

            LV_IMG_DECLARE(Echo_Idle_Screen_Gold);
            Idle_Screen=Echo_Idle_Screen_Gold;

            selectedTab = startTab;
            pros::Task autonomousTask([this] { this->asyncAutonomousTask(); } );
            pros::Task displayTask([this] { this->asyncDisplayTask(); } );
        }

        struct GraphData{
            double *dataUnit;
            std::string dataName;
        };

        struct MotorData{
            int motorPort;
            std::string motorName;
        };

        void setTitle(std::string iTitle); //Complete
        void setTab(int tab); //Complete
        void setIdleImage(const lv_img_dsc_t image); //Complete

        void addAutons(std::vector<Auton> autons);//Complete
        void executeSelectedAuton();//Complete
        void executeAuton(int autonomousIndex);//Complete

        void useGraphData(std::vector<GraphData> iGraphData);//Complete

        void printDiagnosticsData(int row, std::string data);//Complete
        void clearDiagnosticsData();//Complete

        void addTemperatureData(std::vector<MotorData> iMotorData);//Complete

    private:
        const static int AUTONOMOUS_SELECTOR_TAB = 0, GRAPHING_UTILITY_TAB = 1, 
                     DIAGNOSTICS_TAB = 2, TEMPERATURE_GAUGE_TAB = 3; 
        const static int SCROLL_UP = 4, SCROLL_DOWN = 5;
        
        inline static int selectedTab=0;
        inline static int selectedAutonomousIndex = 0;
        inline static bool menuIndexChanged, justScrolled;

        inline static const char* autonomousNames[12] = {"Autonomous Undefined",
                                        "Autonomous Undefined",
                                        "Autonomous Undefined",
                                        "Autonomous Undefined",
                                        "Autonomous Undefined",
                                        "Autonomous Undefined",
                                        "Autonomous Undefined",
                                        "Autonomous Undefined",
                                        "Autonomous Undefined",
                                        "Autonomous Undefined",
                                        "Autonomous Undefined",
                                        "Autonomous Undefined"};

        inline static const char* autonomousDescs[12] = {"Autonomous Undefined",
                                                "Autonomous Undefined",
                                                "Autonomous Undefined",
                                                "Autonomous Undefined",
                                                "Autonomous Undefined",
                                                "Autonomous Undefined",
                                                "Autonomous Undefined",
                                                "Autonomous Undefined",
                                                "Autonomous Undefined",
                                                "Autonomous Undefined",
                                                "Autonomous Undefined",
                                                "Autonomous Undefined"};

        inline static std::function<void()> autonomousCallbacks[12] = {nullptr,
                                                        nullptr,
                                                        nullptr,
                                                        nullptr,
                                                        nullptr,
                                                        nullptr,
                                                        nullptr,
                                                        nullptr,
                                                        nullptr,
                                                        nullptr,
                                                        nullptr,
                                                        nullptr };

        //MENU BUTTONS
        lv_obj_t * titleLabel;
            int idleTimout = 10000;

        inline static lv_obj_t * autonomousSelectorButton;
            lv_obj_t * autonomousButtonMatrix;
            inline static lv_obj_t * autonomousName;
            inline static lv_obj_t * autonomousDesc;
            inline static const char * autonomousButtons[] = {"1", "2", "3", "\n",
                                    "4", "5", "6", "\n",
                                    "7", "8", "9", "\n",
                                    "10", "11", "12",""};
            lv_obj_t * executeAutonomousButton;
            lv_obj_t * elapsedTimeLabel;
            inline static double elapsedAutonomousTime;
            inline static double autonomousStartTime;
            inline static bool autonomousRunning = false;
            inline static bool executionButtonPressed = false;

        inline static   lv_obj_t * graphingUtilityButton;
            bool graphingUtilityInitialized = false;
            lv_obj_t * chart;
            std::vector<GraphData> graphData;
            std::array<lv_chart_series_t *, 4> seriesVector;
            std::array<std::array<double, 20>, 4> seriesPointsVector;

        inline static  lv_obj_t * diagnosticsButton;
            bool diagnosticsInitialized = false;
            lv_obj_t * diagnosticPage;
            lv_obj_t * diagnosticDataLabel;
            std::vector<std::string> diagnosticData;

        inline static lv_obj_t * temperatureGaugeButton;
            bool temperatureInitialized=false;
            lv_obj_t * upScrollButton;
            lv_obj_t * downScrollButton;
            lv_obj_t * tempGauge;
            std::vector <lv_obj_t> temperatureButtonMatrix;
            std::vector <MotorData> motorData;
            inline static int startIndex=0;
            std::vector <lv_obj_t*> tempLabels;
            std::vector <lv_obj_t*> tempColorBoxes;
            std::vector <pros::Motor> motors;
        
        inline static lv_obj_t * backButton;
            inline static bool splashScreenEnabled = false;
            lv_img_dsc_t Idle_Screen;



        //STYLES
        lv_style_t buttonBackgroundStyle;
        lv_style_t buttonReleasedStyle;
        lv_style_t buttonPressedStyle;
        lv_style_t buttonToggleStyle;
        lv_style_t chartStyle;
        lv_style_t pageStyle;
        lv_style_t scrollBarStyle;
        lv_style_t gaugeStyle;
        lv_style_t style_txt;

        //COLORS
        lv_color_t DARK_BUT_NOT_BLACK;
        lv_color_t NOT_QUITE_BLACK;
        lv_color_t KINDA_GRAY;
        lv_color_t WANNABE_WHITE;
        lv_color_t CHARTREUSE;
        lv_color_t CRIMSON;
        lv_color_t MARIGOLD;
        lv_color_t ORANGE;
        lv_color_t HOT_PINK;
        lv_color_t AQUA;
        lv_color_t LAVENDER;
        lv_color_t BLURPLE;
        lv_color_t OLIVE;
        
        //Fields
        std::string title;

        //Helper Functions
        lv_obj_t * drawRectangle( int x, int y, int width, int height, int radius, int borderWidth, lv_color_t infillColor);
        lv_obj_t * drawRectangle( int x, int y, int width, int height, int radius, int borderWidth, lv_color_t infillColor, lv_color_t gradColor, lv_color_t outlineColor);

        lv_obj_t * printText(int x, int y,const char* text);
        lv_obj_t * printTempText(int x, int y,const char* text);
        lv_obj_t * printTextCenteredAt(int x, int y,const char* text);
        lv_obj_t * printTextCenteredAt(int x, int y,const char* text,lv_color_t color);

        void printTemperatureBlock(int xPos, int yPos, lv_color_t infillColor, int id);

        static lv_res_t tabButtonCallback(lv_obj_t * obj);
        static lv_res_t scrollButtonCallback(lv_obj_t * obj);
        static lv_res_t backButtonCallback(lv_obj_t * obj);
        static lv_res_t autonomousButtonMatrixCallback(lv_obj_t * buttonMatrix, const char *txt);
        static lv_res_t executeAutonomousButtonCallback(lv_obj_t * button);

        void defineColors();
        void createBackground();
        void setupStartTab();
        void createStyles();
        void createButtons();

        void refreshDisplay();
        
        //Menu Functions
        void graphicsInit();

        void autonomousSelectorTabInit();
        void autonomousSelectorTabUpdate();

        void graphingUtilityTabInit();
        void graphingUtilityTabUpdate();

        void diagnosticsTabInit();
        void diagnosticsTabUpdate();

        void temperatureGaugeTabInit();
        void temperatureGaugeTabUpdate();

        void asyncDisplayTask();
        void asyncAutonomousTask();
     
};

    