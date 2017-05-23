#include <string>
#include <sstream>
#include <cfloat> // for FLT_MAX
#include <chrono>
#include <thread>
#include <cstdlib>
#include <vector>
#include <unordered_map>
#include <map>
#include <math.h>
#include <cctype>
#include <algorithm>
#include <readline/readline.h>
#include <readline/history.h>
#include <iostream>

#define LEVEL_TOTAL 13
#define LVL8 0.000075
#define LVL9 0.005
#define LVL10 0.05
#define LVL11 0.00
#define PI 3.1415926535897932384626433

#define SIDE_WIDTH2 1
#define SIDE_WIDTH3 1
#define SIDE_WIDTH4 2
#define SIDE_WIDTH5 3
#define SIDE_WIDTH6 3
#define SIDE_WIDTH7 4
#define SIDE_WIDTH8 8
#define SIDE_WIDTH9 12
#define SIDE_WIDTH10 36
#define SIDE_WIDTH11 55
#define SIDE_WIDTH12 72
    
#define LVL5RADIUS 0
#define WLVL5RADIUS 0.000001
#define HLVL5RADIUS 0.0001

#define LVL6RADIUS 0
#define WLVL6RADIUS 0.000001
#define HLVL6RADIUS 0.000065

#define LVL7RADIUS 0.000000225
#define WLVL7RADIUS 0.000001
#define HLVL7RADIUS 0.00005

#define LVL8RADIUS 0.000034
#define WLVL8RADIUS 0.000040
#define HLVL8RADIUS 0.000055

#define LVL9RADIUS 0.00003
#define WLVL9RADIUS 0.000048
#define HLVL9RADIUS 0.000058

#define LVL10RADIUS 0.0000565
#define WLVL10RADIUS 0.000058
#define HLVL10RADIUS 0.000073


#define LVL11RADIUS 0.000063
#define WLVL11RADIUS 0.000065
#define HLVL11RADIUS 0.00007

#define LVL12RADIUS 0.000038
#define WLVL12RADIUS 0.000042
#define HLVL12RADIUS 0.00005












#define LVL10RRADIUS 0.00005
#define LVL11RRADIUS 0.000048




    

//We kept the user click, (for everything, POI, stations, intersections, feactures) red, until
//user press "clear" button. So we need these global vectors to store where user clicks

    //for the size of draw POI name's bound text box
    const double POI_text_width =0.00005 * 5;
     const double POI_testHeight = 0.00005 * 1.5;
    
    //draw a circle at each intersection of 2 street lines to fix ugly streets when zoomed in
    const double FIX_UGLY_ROAD = 0.0000018;
    
    //highlightPath, bound box size consts
    const double HIGHLIGHT_PATH_BOX = 0.05;
    //highligh path text box
    const double HIGHLIGHT_INTERSECTION_BOX = 0.03;
    
    
    //fount size, a enum of arrays. array indices == zoom_level!!!
    enum type_font_size
    {
        ZOOM_FONT0 = 2, 
        ZOOM_FONT1,ZOOM_FONT2,ZOOM_FONT3,ZOOM_FONT4,ZOOM_FONT5,
        ZOOM_FONT6,ZOOM_FONT7,ZOOM_FONT8,ZOOM_FONT9,ZOOM_FONT10,ZOOM_FONT11,ZOOM_FONT12
    };
    
    const enum type_font_size font_size[] = 
    {
    ZOOM_FONT0 , 
    ZOOM_FONT1,ZOOM_FONT2,ZOOM_FONT3,ZOOM_FONT4,ZOOM_FONT5,
    ZOOM_FONT6,ZOOM_FONT7,ZOOM_FONT8,ZOOM_FONT9,ZOOM_FONT10,ZOOM_FONT11,ZOOM_FONT12
    }; 
    
    //road width for 7 levels
    //ZOOM level =0
    enum  ROAD_WIDTH0{MAIN_WIDTH0 = 1, HIGH_WIDTH0 = 2};
    //zoom level  = 1
    enum ROAD_WIDTH1{MAIN_WIDTH1 = 2, HIGH_WIDTH1 = 3};
    //zoom level =2
    enum ROAD_WIDTH2{MAIN_WIDTH2 = 2, HIGH_WIDTH2 = 3};
    //zoom level = 3
    enum ROAD_WIDTH3{MAIN_WIDTH3 = 3, HIGH_WIDTH3 = 4};    
    //ZOOM LEVEL = 4
    enum ROAD_WIDTH4{MAIN_WIDTH4 = 3, HIGH_WIDTH4 = 4};
    //ZOOM LEVEL = 5
    enum ROAD_WIDTH5{MAIN_WIDTH5 = 4, HIGH_WIDTH5 = 6};        
    //zoom level = 6
    enum ROAD_WIDTH6{MAIN_WIDTH6 = 4, HIGH_WIDTH6 = 6}; 
    //zoom level = 7
    enum ROAD_WIDTH7{MAIN_WIDTH7 = 5, HIGH_WIDTH7 = 7};   
    //zoom level = 8
    enum ROAD_WIDTH8{MAIN_WIDTH8 = 10, HIGH_WIDTH8 = 14};
    //zoom level = 9
    enum ROAD_WIDTH9{MAIN_WIDTH9 = 20, HIGH_WIDTH9 = 25};
        //zoom level = 10
    enum ROAD_WIDTH10{MAIN_WIDTH10 = 40, HIGH_WIDTH10 = 50};
        //zoom level = 11
    enum ROAD_WIDTH11{MAIN_WIDTH11 = 72, HIGH_WIDTH11 = 80};
        //zoom level = 12
    enum ROAD_WIDTH12{MAIN_WIDTH12 = 80, HIGH_WIDTH12 = 90};



    
//the radius of POI
 const double RAD_POI = 0.00005;

 //POI's text box, all text box with offset 0.01
 const double ZERO_1_NAME_BOX = 0.01;


 //360 degree!!
 const int FULL_CIRCLE_DEG = 360;

 //small borer of all feature functions
 const int FEATURE_BORDER_WIDTH = 2;

 //to search nearby hilighted stuff
 const double NEARBY_INTERSECTION = 0.0001;
 const double NEARBY_POI = 0.00006;

 //0.00003
 const double HIGHLIGHT_POI_BOX_3 = 0.00003;

 //0.000004
 const double TEXTBOX_04 = 0.00004;
//enum set font sizes at each zoom level
  //  textZOOM0[]{}
    
    