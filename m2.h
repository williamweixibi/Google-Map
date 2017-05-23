#pragma once
#include <string>
#include <iostream>
#include <cfloat> // for FLT_MAX
#include <chrono>
#include <thread>
#include <cstdlib>
#include <vector>
#include <unordered_map>
#include "graphics.h"
#include "m1.h"
#include "m2.h"
#include "OSMDatabaseAPI.h"
#include "streetGraph.h"
#include "StreetsDatabaseAPI.h"

void draw_arrow_function(double offset, double multi);
void search_engine(void);
void distance_calculator(void);
void helper_scalefunction(void);
void act_on_scale_fit(void (*drawscreen_ptr) (void));
void helper_level(float offset);
void drawscreen();
float *CrossProduct(float *a, float *b);
void draw_streetarrowpath(void);
void drawdestinations(double offset, double multi, double font, double radius, vector<unsigned>anyPath);
void act_on_adjust(void (*drawscreen_ptr) (void));
// Draws the map. You can assume your load_map (string map_name)
// function is called before this function in the unit tests.
// Your main () program should do the same.
void draw_streetName(void);
//void act_on_adjust(void (*drawscreen_ptr) (void));
void act_on_command(void (*drawscreen_ptr) (void)) ;
void act_on_help(void (*drawscreen_ptr) (void)); 
//use this to calculate the street naem angle of eeach segments
double setRoadNameAngleID(unsigned ID);
void drawtextonly(void);
t_point LatLon_to_XY(LatLon location);
// Callbacks for event-driven window handling.
void act_on_new_button_func(void (*drawscreen_ptr) (void));
void act_on_button_press(float x, float y, t_event_buttonPressed event);
void act_on_mouse_move(float x, float y);
void act_on_keypress(char c, int keysym);
void act_on_search_street(void (*drawscreen_ptr) (void));
void act_on_find_intersection(void (*drawscreen_ptr) (void));
void act_on_clear(void (*drawscreen_ptr) (void));
void act_on_station(void (*drawscreen_ptr) (void));
void act_on_switch_map(void (*drawscreen_ptr) (void));
void act_on_feature(void (*drawscreen_ptr) (void));
void act_on_POI(void (*drawscreen_ptr) (void));
void act_on_find_path_m3(void (*drawscreen_ptr) (void));
//convert coordinatae systems
t_point LatLon_to_XY(LatLon location);
// A handy delay function for the animation example
void delay(long milliseconds);

// State variables for the example showing entering lines and rubber banding
// and the new button example.

static t_point rubber_pt; // Last point to which we rubber-banded.
static std::vector<t_point> line_pts; // Stores the points entered by user clicks.



////check if the street name exists, if unknown, return false

bool check_street_valid(string streetName);

//check if a feaure is a closed polygon
bool check_is_closed(unsigned feature_id);

//draw each individual street segments
//put in zoom level high maybe?


vector <long double> findCoordinates(void);

//to change the bound box, +/- num to bottom, left, right, up coordinates

t_bound_box adjust_t_bound_box(t_bound_box origin);

//calculate the angle we shoudl orient the street name
double setRoadNameAngle(t_point& from, t_point& to);
//double setRoadNameAngle (LatLon & from, LatLon & to);

//draw the street name using draw text from graphics library
//should only show up when zoom level very high
//void draw_streetName(double angle, string name, t_point center, double segmentLength );
void draw_streetName(double angle, string streetName, t_point center, double segmentLength);

string myToUpper(string lowerCase);

void draw_map();


//draw all type highway, use oragne
//size varies accroding to zoomlevel
void draw_highway(int width,float radius);


//ues this to draw main road. Main road will be yellow to indicarte the street blocks
void draw_mainroad(int width,float radius);

//draw side road, set size accrodingly. Ratio of width = highway  : minroad : sideroad = 3:2:1 
//function to draw segment by find their connected intersections and curve points
void draw_sideroad(int width,float radius);

//draw segments provided by a street ID, link together
void draw_segment(unsigned segment_id,float radius);
//function to draw streets by name
//function to draw streets by name
void draw_street(vector<unsigned> street_ids);

void resetVisibleWorld(t_point resetMid, t_point offset, bool redraw);
t_point findCentreFeature(vector<t_point>vertices, int vertexCount);



//draw the POI name in a box for high zoom levels
void draw_POI_name(t_point POI_position, unsigned int POI_id);

//draw the hopsitpal POIs, use the symbol + ato represent red cross
void draw_hospital(vector<unsigned> POI_ids);



//use this funciton to label type finanical, like bank, atm ,etc
//we use symbol "$"

void draw_financials(vector<unsigned> POI_ids);


//use this function to label type schools, including kindergarten to uni
//we use the symbol "A+" to give them some hope

void draw_schools(vector<unsigned> POI_ids);


//yay food! we use this function to label all resturants, food courts, cafe, anywhere  you can eat
void draw_food(vector<unsigned> POI_ids);

//draws a circle for general points of interest
void draw_point_of_interest(unsigned POI_id, float rad, color_types select1, color_types select2);


//we set zoom levels and will show different types of POI in different zoom levels
//this is used to filter POI by type
void draw_POI_by_type(string type);


//used to draw feature name
void draw_featureName(unsigned int featureID);


//draw buildings as polygons, only show in high zoom levels
//use this to draw building, lake , beach, unknown, island, lake, river, etc all types
void draw_all_types_features(unsigned int Feature_id, color_types selected_color);

//we defined all the functions above to FILTER WHAT TO DRAW AND layering by calling different functions
void draw_feature(int zoomLevel);


//add intersetion info, latlon, name, streets names, used to find
void highlight_intersections(vector<unsigned> intersection_id);

//use this to highlight feaures entered by user
void highlight_feature(vector<unsigned> feature_id);
void highlight_find_path_m3(vector<unsigned> feature_id);
void printPath();
void highlightPOI(vector<unsigned> POI_id);
void draw_click_POI(t_point position);
void highlightPath(t_point position);
void help_helperfunction(void);
void highlight_helperfunction(void);
void transport_helperfunction(void);
void highlight_path_helperfunction(void);
void level_12_helper_function(void);
void level_11_helper_function(void);
void level_10_helper_function(void);
void level_9_helper_function(void);
void level_8_helper_function(void);
void level_7_helper_function(void);
void level_6_helper_function(void);
void level_5_helper_function(void);
void level_4_helper_function(void);
void level_3_helper_function(void);
void level_2_helper_function(void);
void level_1_helper_function(void);
void level_0_helper_function(void);