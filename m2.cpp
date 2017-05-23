#include "easygl_constants.h"
#include "graphics.h"
#include "m1.h"
#include "m2.h"
#include "m3.h"
#include "streetGraph.h"
#include "StreetsDatabaseAPI.h"
#include "parser.h"
#include "constantM2.h"
#include "graphics.h"

t_bound_box screen_bound;
bool to_help = false;
bool resetWorld_Feature = false;
bool keep_highlight = true;
bool transport_on = false;
bool show_info = false;
bool find_Path = false;
extern bool scale_fit;
extern bool zoom_switch;

string keypressed;
bool pressed = false;

unsigned int station_click = 0;
vector<unsigned> path;
extern int zoom_level;

///extern vector<unsigned> paths;
int segfault = 0;

vector<t_point> resetWorld_Points;
unsigned int findPath_click = 0;
unsigned int findClick = 0;
vector<t_point> click_intersectionFindPath;
vector<t_point> click_POI_findPath;
vector<vector<unsigned>> m3_intersection;

vector<t_point> click_intersection;
vector<t_point> click_POI;
vector<vector<unsigned>> searched_street;
vector<vector<unsigned>> searched_intersection;
vector<vector<unsigned>> searched_feature;
vector<vector<unsigned>> searched_POI;

extern t_point initial_one;
extern t_point initial_two;

vector<long double> coordinates;

float level_0_width;
float level_0_height;
float level_1_width;
float level_1_height;
float level_2_width;
float level_2_height;
float level_3_width;
float level_3_height;
float level_4_width;
float level_4_height;
float level_5_width;
float level_5_height;
float level_6_width;
float level_6_height;
float level_7_width;
float level_7_height;
float level_8_width;
float level_8_height;
float level_9_width;
float level_9_height;
float level_10_width;
float level_10_height;
float level_11_width;
float level_11_height;
float level_12_width;
float level_12_height;

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

void distance_calculator(void) {
    t_bound_box world_frame(initial_one, initial_two);
    level_0_width = world_frame.get_width();
    level_0_height = world_frame.get_height();
    level_1_width = world_frame.get_width()/(float)pow(ZOOM_FACTOR,1);
    level_1_height = world_frame.get_height()/(float)pow(ZOOM_FACTOR,1);
    
    level_2_width = world_frame.get_width()/(float)pow(ZOOM_FACTOR,2);
    level_2_height = world_frame.get_height()/(float)pow(ZOOM_FACTOR,2);
    
    level_3_width = world_frame.get_width()/(float)pow(ZOOM_FACTOR,3);
    level_3_height = world_frame.get_height()/(float)pow(ZOOM_FACTOR,3);
    
    level_4_width = world_frame.get_width()/(float)pow(ZOOM_FACTOR,4);
    level_4_height = world_frame.get_height()/(float)pow(ZOOM_FACTOR,4);
    
    level_5_width = world_frame.get_width()/(float)pow(ZOOM_FACTOR,5);
    level_5_height = world_frame.get_height()/(float)pow(ZOOM_FACTOR,5);
    
    level_6_width = world_frame.get_width()/(float)pow(ZOOM_FACTOR,6);
    level_6_height = world_frame.get_height()/(float)pow(ZOOM_FACTOR,6);
    
    level_7_width = world_frame.get_width()/(float)pow(ZOOM_FACTOR,7);
    level_7_height = world_frame.get_height()/(float)pow(ZOOM_FACTOR,7);
    
    level_8_width = world_frame.get_width()/(float)pow(ZOOM_FACTOR,8);
    level_8_height = world_frame.get_height()/(float)pow(ZOOM_FACTOR,8);
    
    level_9_width = world_frame.get_width()/(float)pow(ZOOM_FACTOR,9);
    level_9_height = world_frame.get_height()/(float)pow(ZOOM_FACTOR,9);
    
    level_10_width = world_frame.get_width()/(float)pow(ZOOM_FACTOR,10);
    level_10_height = world_frame.get_height()/(float)pow(ZOOM_FACTOR,10);
    
    level_11_width = world_frame.get_width()/(float)pow(ZOOM_FACTOR,11);
    level_11_height = world_frame.get_height()/(float)pow(ZOOM_FACTOR,11);
    
    level_12_width = world_frame.get_width()/(float)pow(ZOOM_FACTOR,12);
    level_12_height = world_frame.get_height()/(float)pow(ZOOM_FACTOR,12);
    /*
    cout<<"Level 0 Width: "<<level_0_width<<" & "<<"level 0 Height: "<<level_0_height<<endl;
    cout<<"Level 1 Width: "<<level_1_width<<" & "<<"level 1 Height: "<<level_1_height<<endl;
    cout<<"Level 2 Width: "<<level_2_width<<" & "<<"level 2 Height: "<<level_2_height<<endl;
    cout<<"Level 3 Width: "<<level_3_width<<" & "<<"level 3 Height: "<<level_3_height<<endl;
    cout<<"Level 4 Width: "<<level_4_width<<" & "<<"level 4 Height: "<<level_4_height<<endl;
    cout<<"Level 5 Width: "<<level_5_width<<" & "<<"level 5 Height: "<<level_5_height<<endl;
    cout<<"Level 6 Width: "<<level_6_width<<" & "<<"level 6 Height: "<<level_6_height<<endl;
    cout<<"Level 7 Width: "<<level_7_width<<" & "<<"level 7 Height: "<<level_7_height<<endl;
    cout<<"Level 8 Width: "<<level_8_width<<" & "<<"level 8 Height: "<<level_8_height<<endl;
    cout<<"Level 9 Width: "<<level_9_width<<" & "<<"level 9 Height: "<<level_9_height<<endl;
    cout<<"Level 10 Width: "<<level_10_width<<" & "<<"level 10 Height: "<<level_10_height<<endl;
    cout<<"Level 11 Width: "<<level_11_width<<" & "<<"level 11 Height: "<<level_11_height<<endl;
    cout<<"Level 12 Width: "<<level_12_width<<" & "<<"level 12 Height: "<<level_12_height<<endl;
    */
}

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

void draw_map() {
    init_graphics("Map Of City", NEWMAPGREY);
    coordinates = findCoordinates();
    set_visible_world(coordinates[0] * singleton->get_cosAvg(), coordinates[1], coordinates[2] * singleton->get_cosAvg(), coordinates[3]);

    initial_one.x = coordinates[0] * singleton->get_cosAvg();
    initial_one.y = coordinates[1];
    initial_two.x = coordinates[2] * singleton->get_cosAvg();
    initial_two.y = coordinates[3];
    distance_calculator();

    update_message("Interactive graphics example.");
    
    create_button("Window", "Find", act_on_find_intersection);
    create_button("Find", "Street", act_on_search_street);
    create_button("Street", "Attraction", act_on_feature);
    create_button("Attraction", "POI", act_on_POI);
    create_button("POI", "Transport Off", act_on_station);
    create_button("Transport Off", "Clear", act_on_clear);
    create_button("Clear", "Switch", act_on_switch_map);
    create_button("Switch", "Command", act_on_command);
    create_button("Command", "Path Off", act_on_find_path_m3);
    create_button("Command", "Help", act_on_help);
    
    /*
    create_button("Window", "Landmarks", NULL);
    create_button("Landmarks", "Transport", NULL);
    create_button("Transport", "Feature", NULL);
    create_button("Feature", "POI", NULL);
    create_button("POI", "Street", NULL);
    create_button("Street", "Intersection", NULL);
    create_button("Intersection", "Pathfinding", NULL);
    create_button("Pathfinding", "Tour Plan", NULL);
    create_button("Tour Plan", "Switch Map", NULL);
    create_button("Switch Map", "Clear", NULL);
    create_button("Clear", "Help", NULL);
    */
    set_keypress_input(true);
    set_mouse_move_input(true);
    drawscreen();
    event_loop(act_on_button_press, NULL, act_on_keypress, drawscreen);
    close_graphics();
}

void drawscreen() {
    set_draw_mode(DRAW_NORMAL); // Should set this if your program does any XOR drawing in callbacks.
    clearscreen(); /* Should precede drawing for all drawscreens*/
    draw_streetName();

    if (zoom_level == 0) {
        level_0_helper_function();
    } else if (zoom_level == 1) {
        level_1_helper_function();
    } else if (zoom_level == 2) {
        level_2_helper_function();
    } else if (zoom_level == 3) {
        level_3_helper_function();
    } else if (zoom_level == 4) {
        level_4_helper_function();
    } else if (zoom_level == 5) {
        level_5_helper_function();
    } else if (zoom_level == 6) {
        level_6_helper_function();
    } else if (zoom_level == 7) {
        level_7_helper_function();
    } else if (zoom_level == 8) {
        level_8_helper_function();
    } else if (zoom_level == 9) {
        level_9_helper_function();
    } else if (zoom_level == 10) {
        level_10_helper_function();
    } else if (zoom_level == 11) {
        level_11_helper_function();
    } else if (zoom_level == 12) {
        level_12_helper_function();
    }
    transport_helperfunction();
    highlight_helperfunction();
    highlight_path_helperfunction();
    help_helperfunction();
    newfunction();
    //search_engine();
    
}


////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

void level_0_helper_function(void) {
    t_bound_box mapBorder = get_visible_world();
    LatLon bottomLeft, topRight;
    bottomLeft.lat = mapBorder.bottom();
    bottomLeft.lon = mapBorder.left();
    topRight.lat = mapBorder.top();
    topRight.lon = mapBorder.right();
    draw_feature(zoom_level); //NO BUILDING
    draw_mainroad(MAIN_WIDTH0, LVL6RADIUS);
    draw_highway(HIGH_WIDTH0, LVL6RADIUS);
    setcolor(BLACK);
    settextrotation(0);
    setfontsize(60);
    long double x = 10000000000000;
    //drawtext(mapBorder.get_xcenter(), mapBorder.get_ycenter(), city, x, x);
    drawdestinations(0.001, 1, 0, LVL6RADIUS, path);
    //path is command line, paths is the path of clicking intersections!
    drawdestinations(0.001, 1, 0, LVL6RADIUS, paths);
}

void level_1_helper_function(void) {
    t_bound_box mapBorder = get_visible_world();
    LatLon bottomLeft, topRight;
    bottomLeft.lat = mapBorder.bottom();
    bottomLeft.lon = mapBorder.left();
    topRight.lat = mapBorder.top();
    topRight.lon = mapBorder.right();
    draw_feature(zoom_level); //show feature name without building
    draw_mainroad(MAIN_WIDTH1, LVL6RADIUS);
    draw_highway(HIGH_WIDTH1, LVL6RADIUS);

    setcolor(BLACK);
    settextrotation(0);
    setfontsize(60);
    long double x = 10000000000000;
    //drawtext(mapBorder.get_xcenter(), mapBorder.get_ycenter(), city, x, x);
    drawdestinations(0.001, 1, 0, LVL6RADIUS, path);
    //path is command line, paths is the path of clicking intersections!
    drawdestinations(0.001, 1, 0, LVL6RADIUS, paths);
}

void level_2_helper_function(void) {
    t_bound_box mapBorder = get_visible_world();
    LatLon bottomLeft, topRight;
    bottomLeft.lat = mapBorder.bottom();
    bottomLeft.lon = mapBorder.left();
    topRight.lat = mapBorder.top();
    topRight.lon = mapBorder.right();
    draw_feature(zoom_level);
    draw_mainroad(MAIN_WIDTH2, LVL6RADIUS);
    draw_highway(HIGH_WIDTH2, LVL6RADIUS);
    setcolor(BLACK);
    settextrotation(0);
    setfontsize(60);
    long double x = 10000000000000;
    //drawtext(mapBorder.get_xcenter(), mapBorder.get_ycenter(), city, x, x);
    drawdestinations(0.001, 0.8, 0, LVL6RADIUS, path);
    //path is command line, paths is the path of clicking intersections!
    drawdestinations(0.001, 0.8, 0, LVL6RADIUS, paths);
}

void level_3_helper_function(void) {
    t_bound_box mapBorder = get_visible_world();
    LatLon bottomLeft, topRight;
    bottomLeft.lat = mapBorder.bottom();
    bottomLeft.lon = mapBorder.left();
    topRight.lat = mapBorder.top();
    topRight.lon = mapBorder.right();
    draw_feature(zoom_level);
    draw_sideroad(SIDE_WIDTH3, LVL6RADIUS);
    draw_mainroad(MAIN_WIDTH3, LVL6RADIUS);
    draw_highway(HIGH_WIDTH3, LVL6RADIUS);
    drawdestinations(0.001, 0.8, 0, LVL6RADIUS, path);
    //path is command line, paths is the path of clicking intersections!
    drawdestinations(0.001, 0.8, 0, LVL6RADIUS, paths);
}

void level_4_helper_function(void) {
    t_bound_box mapBorder = get_visible_world();
    LatLon bottomLeft, topRight;
    bottomLeft.lat = mapBorder.bottom();
    bottomLeft.lon = mapBorder.left();
    topRight.lat = mapBorder.top();
    topRight.lon = mapBorder.right();
    draw_feature(zoom_level);
    draw_sideroad(SIDE_WIDTH4, LVL6RADIUS);
    draw_mainroad(MAIN_WIDTH4, LVL6RADIUS);
    draw_highway(HIGH_WIDTH4, LVL6RADIUS);
    drawdestinations(0.001, 0.6, 0, LVL6RADIUS, path);
    //path is command line, paths is the path of clicking intersections!
    drawdestinations(0.001, 0.6, 0, LVL6RADIUS, paths);
}

void level_5_helper_function(void) {
    t_bound_box mapBorder = get_visible_world();
    LatLon bottomLeft, topRight;
    bottomLeft.lat = mapBorder.bottom();
    bottomLeft.lon = mapBorder.left();
    topRight.lat = mapBorder.top();
    topRight.lon = mapBorder.right();
    draw_feature(zoom_level);
    draw_sideroad(SIDE_WIDTH5, LVL5RADIUS);
    draw_mainroad(MAIN_WIDTH5, WLVL5RADIUS);
    draw_highway(HIGH_WIDTH5, HLVL5RADIUS);
    drawdestinations(0.001, 0.6, 0, LVL6RADIUS, path);
    //path is command line, paths is the path of clicking intersections!
    drawdestinations(0.001, 0.6, 0, LVL6RADIUS, paths);
}

void level_6_helper_function(void) {
    t_bound_box mapBorder = get_visible_world();
    LatLon bottomLeft, topRight;
    bottomLeft.lat = mapBorder.bottom();
    bottomLeft.lon = mapBorder.left();
    topRight.lat = mapBorder.top();
    topRight.lon = mapBorder.right();
    draw_feature(zoom_level);
    draw_sideroad(SIDE_WIDTH6, LVL6RADIUS);
    draw_mainroad(MAIN_WIDTH6, WLVL6RADIUS);
    draw_highway(HIGH_WIDTH6, HLVL6RADIUS);
    draw_POI_by_type("hospital");
    draw_POI_by_type("college");
    draw_POI_by_type("language_school");
    draw_POI_by_type("english_school");
    draw_POI_by_type("music_school");
    draw_POI_by_type("preschool");
    draw_POI_by_type("school");
    draw_POI_by_type("university");
    draw_POI_by_type("kindergarten");
    drawdestinations(0.001, 0.4, 10, LVL6RADIUS, path);
    //path is command line, paths is the path of clicking intersections!
    drawdestinations(0.001, 0.4, 10, LVL6RADIUS, paths);
}

void level_7_helper_function(void) {
    t_bound_box mapBorder = get_visible_world();
    LatLon bottomLeft, topRight;
    bottomLeft.lat = mapBorder.bottom();
    bottomLeft.lon = mapBorder.left();
    topRight.lat = mapBorder.top();
    topRight.lon = mapBorder.right();
    draw_feature(zoom_level);
    draw_sideroad(SIDE_WIDTH7, LVL7RADIUS);
    draw_mainroad(MAIN_WIDTH7, WLVL7RADIUS);
    draw_highway(HIGH_WIDTH7, HLVL7RADIUS);
    draw_POI_by_type("hospital");
    draw_POI_by_type("college");
    draw_POI_by_type("language_school");
    draw_POI_by_type("english_school");
    draw_POI_by_type("music_school");
    draw_POI_by_type("preschool");
    draw_POI_by_type("school");
    draw_POI_by_type("university");
    draw_POI_by_type("kindergarten");
    draw_POI_by_type("bakery");
    draw_POI_by_type("cafe");
    draw_POI_by_type("fast_food");
    draw_POI_by_type("food_court");
    draw_POI_by_type("ice_cream");
    draw_POI_by_type("internet_cafe");
    draw_POI_by_type("restaurant");
    draw_POI_by_type("sweets");
    draw_POI_by_type("bank");
    draw_POI_by_type("atm");
    draw_POI_by_type("financial");
    draw_POI_by_type("hotel");
    drawdestinations(0.001, 0.4, 10, LVL7RADIUS, path);
    //path is command line, paths is the path of clicking intersections!
    drawdestinations(0.001, 0.4, 10, LVL7RADIUS, paths);
}

void level_8_helper_function(void) {
    t_bound_box mapBorder = get_visible_world();
    LatLon bottomLeft, topRight;
    bottomLeft.lat = mapBorder.bottom();
    bottomLeft.lon = mapBorder.left();
    topRight.lat = mapBorder.top();
    topRight.lon = mapBorder.right();
    draw_feature(zoom_level);
    draw_sideroad(SIDE_WIDTH8, LVL8RADIUS);
    draw_mainroad(MAIN_WIDTH8, WLVL8RADIUS);
    draw_highway(HIGH_WIDTH8, HLVL8RADIUS);
    
    unordered_map<string, vector<unsigned>> all_types_POI;
    all_types_POI = singleton ->get_Map_POItype_Point_ID();
    for (auto iter = all_types_POI.begin(); iter != all_types_POI.end(); iter++) {
        draw_POI_by_type(iter->first);
    }
    drawdestinations(0.001, 0.2, 12, LVL8RADIUS, path);
    //path is command line, paths is the path of clicking intersections!
    drawdestinations(0.001, 0.2, 12, LVL8RADIUS, paths);
    draw_streetName();
}

void level_9_helper_function(void)
{
    t_bound_box mapBorder = get_visible_world();
    LatLon bottomLeft, topRight;
    bottomLeft.lat = mapBorder.bottom();
    bottomLeft.lon = mapBorder.left();
    topRight.lat = mapBorder.top();
    topRight.lon = mapBorder.right();
    draw_feature(zoom_level);
    draw_sideroad(SIDE_WIDTH9, LVL9RADIUS);
    draw_mainroad(MAIN_WIDTH9, WLVL9RADIUS);
    draw_highway(HIGH_WIDTH9, HLVL9RADIUS);

    unordered_map<string, vector<unsigned>> all_types_POI;
    all_types_POI = singleton ->get_Map_POItype_Point_ID();
    for (auto iter = all_types_POI.begin(); iter != all_types_POI.end(); iter++)
    {
        draw_POI_by_type(iter->first);
    }
    drawdestinations(0.001, 0.2, 12, LVL9RADIUS, path);
    //path is command line, paths is the path of clicking intersections!
    drawdestinations(0.001, 0.2, 12, LVL9RADIUS, paths);
        draw_streetName();
}

void level_10_helper_function(void) {
    t_bound_box mapBorder = get_visible_world();
    LatLon bottomLeft, topRight;
    bottomLeft.lat = mapBorder.bottom();
    bottomLeft.lon = mapBorder.left();
    topRight.lat = mapBorder.top();
    topRight.lon = mapBorder.right();
    draw_feature(zoom_level);

    draw_sideroad(SIDE_WIDTH10, LVL10RRADIUS);
    draw_mainroad(MAIN_WIDTH10, WLVL10RADIUS);
    draw_highway(HIGH_WIDTH10, HLVL10RADIUS);
    
    unordered_map<string, vector<unsigned>> all_types_POI;
    all_types_POI = singleton ->get_Map_POItype_Point_ID();
    for (auto iter = all_types_POI.begin(); iter != all_types_POI.end(); iter++) {
        draw_POI_by_type(iter->first);
    }
   drawdestinations(0.001, 0.1, 14, LVL10RADIUS, path);
   //path is command line, paths is the path of clicking intersections!
    drawdestinations(0.001, 0.1, 14, LVL10RADIUS, paths);
    draw_streetName();
}

void level_11_helper_function(void) {
    t_bound_box mapBorder = get_visible_world();
    LatLon bottomLeft, topRight;
    bottomLeft.lat = mapBorder.bottom();
    bottomLeft.lon = mapBorder.left();
    topRight.lat = mapBorder.top();
    topRight.lon = mapBorder.right();
    draw_feature(zoom_level);
    draw_sideroad(SIDE_WIDTH11, LVL11RRADIUS);
    draw_mainroad(MAIN_WIDTH11, WLVL11RADIUS);
    draw_highway(HIGH_WIDTH11, HLVL11RADIUS);
    
    unordered_map<string, vector<unsigned>> all_types_POI;
    all_types_POI = singleton ->get_Map_POItype_Point_ID();
    for (auto iter = all_types_POI.begin(); iter != all_types_POI.end(); iter++) {
        draw_POI_by_type(iter->first);
    }
    drawdestinations(0.001, 0.1, 14, LVL11RADIUS, path);
    //path is command line, paths is the path of clicking intersections!
    drawdestinations(0.001, 0.1, 14, LVL11RADIUS, paths);
    draw_streetName();
}

void level_12_helper_function(void) {
    t_bound_box mapBorder = get_visible_world();
    LatLon bottomLeft, topRight;
    bottomLeft.lat = mapBorder.bottom();
    bottomLeft.lon = mapBorder.left();
    topRight.lat = mapBorder.top();
    topRight.lon = mapBorder.right();
    draw_feature(zoom_level);
    draw_sideroad(SIDE_WIDTH12, LVL12RADIUS);
    draw_mainroad(MAIN_WIDTH12, WLVL12RADIUS);
    draw_highway(HIGH_WIDTH12, HLVL12RADIUS);
    
    unordered_map<string, vector<unsigned>> all_types_POI;
    all_types_POI = singleton ->get_Map_POItype_Point_ID();
    for (auto iter = all_types_POI.begin(); iter != all_types_POI.end(); iter++) {
        draw_POI_by_type(iter->first);
    }
    drawdestinations(0.001, 0.05, 16, LVL12RADIUS, path);
    //path is command line, paths is the path of clicking intersections!
    drawdestinations(0.001, 0.05, 16, LVL12RADIUS, paths);
    draw_streetName();
}

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

void highlight_path_helperfunction(void) {
    if ((find_Path == true) && (findClick == 2)) {
        if (click_intersectionFindPath.size() > 2) {
            cout << "Error: more than 2 intersections!" << endl;
            find_Path = false;
            click_intersectionFindPath.clear();
            paths.clear();
        } else if(click_intersectionFindPath.size() == 2){
            unsigned inter1 = singleton->get_hash_Intersection_Position_toID(click_intersectionFindPath[0]);
            unsigned inter2 = singleton->get_hash_Intersection_Position_toID(click_intersectionFindPath[1]);
            paths = find_path_between_intersections(inter1, inter2);
            if (!paths.empty()) {
                highlight_find_path_m3(paths);
            } else {
                cout << "No valid path found" << endl;

            }
        }
    } else if (find_Path == false) {
        if (!click_intersectionFindPath.empty())
            click_intersectionFindPath.clear();

        if (!paths.empty())
            paths.clear();
    }
}

void transport_helperfunction(void) {
    if (transport_on) {
        vector<vector<unsigned long long>> routes;
        routes = singleton->get_all_transport_lines();
        for (unsigned i = 0; i < routes.size(); i++) {
            vector<unsigned long long> all_stops;
            all_stops = routes[i];
            for (unsigned j = 0; j < all_stops.size() - 1; j++) {
                const OSMNode* stop1 = singleton ->get_OSMID_Point_OSMNode(all_stops[j]);
                const OSMNode* stop2 = singleton ->get_OSMID_Point_OSMNode(all_stops[j + 1]);
                LatLon stop1_position = stop1 ->coords();
                LatLon stop2_position = stop2 ->coords();

                t_point stop1_in_xy, stop2_in_xy;
                stop1_in_xy.x = stop1_position.lon * (singleton ->get_cosAvg());
                stop1_in_xy.y = stop1_position.lat;
                stop2_in_xy.x = stop2_position.lon * (singleton ->get_cosAvg());
                stop2_in_xy.y = stop2_position.lat;

                setlinewidth(4);
                setlinestyle(DASHED);
                setcolor(PURPLE);
                drawline(stop1_in_xy, stop2_in_xy);
            }
        }

        //to turn on the subway lines!
        vector<t_point> transportation = singleton ->get_Locations_Of_All_Stations();
        for (unsigned i = 0; i < transportation.size(); i++) {
            t_point position = transportation[i];
            setcolor(LIGHTSKYBLUE);
            fillrect(position.x - RAD_POI, position.y - RAD_POI, position.x + RAD_POI, position.y + RAD_POI);

            string name = singleton->get_Name_of_All_Stations(i);
            if (name != "\0") {
                t_bound_box textBox((position.x) - NEARBY_INTERSECTION, (position.y) + RAD_POI, (position.x) + NEARBY_INTERSECTION
                        , (position.y) + NEARBY_INTERSECTION);
                setcolor(SADDLEBROWN);
                setfontsize(font_size[zoom_level]);
                settextrotation(0);
                drawtext_in(textBox, name);
            }
        }
    }
}

void highlight_helperfunction(void) {
    if (keep_highlight) {
        for (auto iter = searched_feature.begin(); iter != searched_feature.end(); iter++) {
            highlight_feature(*iter);
        }

        for (auto iter = searched_street.begin(); iter != searched_street.end(); iter++) {
            draw_street(*iter);
        }

        for (auto iter = searched_intersection.begin(); iter != searched_intersection.end(); iter++) {
            highlight_intersections(*iter);
        }

        for (auto iter = searched_POI.begin(); iter != searched_POI.end(); iter++) {
            highlightPOI(*iter);
        }

        for (auto iter = click_intersection.begin(); iter != click_intersection.end(); iter++) {
            highlightPath(*iter);
        }

        for (auto iter = click_POI.begin(); iter != click_POI.end(); iter++) {
            draw_click_POI(*iter);
        }
        show_info = false;
    } else {
        if (click_intersection.size() > 0) {
            click_intersection.clear();
        }
        if (click_POI.size() > 0) {
            click_POI.clear();
        }
        if (searched_street.size() > 0) {
            for (auto iter = searched_street.begin(); iter != searched_street.end(); iter++)
                (*iter).clear();
            searched_street.clear();
        }
        if (searched_intersection.size() > 0) {
            for (auto iter = searched_intersection.begin(); iter != searched_intersection.end(); iter++)
                (*iter).clear();
            searched_intersection.clear();
        }
        if (searched_feature.size() > 0) {
            for (auto iter = searched_feature.begin(); iter != searched_feature.end(); iter++)
                (*iter).clear();
            searched_feature.clear();
        }
        if (searched_POI.size() > 0) {
            for (auto iter = searched_POI.begin(); iter != searched_POI.end(); iter++)
                (*iter).clear();
            searched_POI.clear();
        }

        keep_highlight = true;
        show_info = false;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

void help_helperfunction(void)
{
    if (to_help == true)
    {
        helper_level(1);
    } else {
        zoom_switch = false;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

string test;
vector<string>global_string_input;
#include <X11/keysym.h>
void act_on_keypress (char c, int keysym) 
{
        string s(1, c);
        //test = s; 
        global_string_input.push_back(s);
        //cout<<"Key Value: "<<keysym<<endl; 
        if(keysym==65288)
        {
            global_string_input.pop_back();
            global_string_input.pop_back();
        }
        for(int i=0;i<global_string_input.size();i++)
        {
            if(i==0)
            {
                test = "";
            }
            else
            {
                test += global_string_input[i];
            }
        }
        test[0]=toupper(test[0]);
        for(int j=0;j<test.size();j++)
        {
            if(test[j]==' ')
            {
                test[j+1]=toupper(test[j+1]);
            }
        }
        search_engine();
        drawscreen();
}

//must set the area want to draw into
//fill the rectangele with another rectangle acting as border
//to set the box, for every zoom level (75% of the screen width)

void search_engine()
{
        t_bound_box search_box = get_visible_world();
        float left_offset = search_box.get_width()*(float)0.05;
        float left_coordinate = search_box.left()+left_offset;
        float right_offset = search_box.get_width()*(float)0.25;
        float right_coordinate = search_box.right()-right_offset;
        float top_offset = search_box.get_height()*(float)0.05;
        float top_coordinate = search_box.top()-top_offset;
        float bottom_offset = search_box.get_height()*(float)0.87;
        float bottom_coordinate = search_box.bottom()+bottom_offset;

        //outer border
        float left_offset2 = search_box.get_width()*(float)0.045;
        float left_coordinate2 = search_box.left()+left_offset2;
        float right_offset2 = search_box.get_width()*(float)0.245;
        float right_coordinate2 = search_box.right()-right_offset2;
        float top_offset2 = search_box.get_height()*(float)0.04;
        float top_coordinate2 = search_box.top()-top_offset2;
        float bottom_offset2 = search_box.get_height()*(float)0.86;
        float bottom_coordinate2 = search_box.bottom()+bottom_offset2;

        setcolor(BLACK);
        fillrect(left_coordinate2,bottom_coordinate2,right_coordinate2,top_coordinate2);

        setcolor(WHITE);
        fillrect(left_coordinate,bottom_coordinate,right_coordinate,top_coordinate);

        t_bound_box input_center(left_coordinate,bottom_coordinate,right_coordinate,top_coordinate);
        setcolor(BLACK);
        setfontsize(40);
        drawtext_in(input_center,test);
}

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

void helper_level(float offset)
{
        zoom_switch = true;
        t_bound_box screen_temp = get_visible_world();
        setcolor(WHITE);
        set_visible_world(screen_temp);
        fillrect(screen_temp);
        string temp = "Help Menu";
        string temp1 = "Welcome to";
        string temp2 = "Zoom In: Enlarge Scale";
        string temp3 = "Zoom Out: Diminish Scale";
        string temp4 = "Zoom Fit: Fit City to Screen";
        string temp5 = "Window: Create Rectangle to Scale In";
        string temp6 = "Find: Type Street Names in terminal to find intersection";
        string temp7 = "Attraction: Search for tourism attractions";
        string temp8 = "POI: Search Point of Interest";
        string temp9 = "Transportation: Show Public Transportation Routes and Locations";
        string temp10 = "Clear: Clear All Search and Highlight";
        string temp11 = "Switch: Switch to a different City";
        string temp12 = "Command: Enable autp-completion search engine for intersections";
        string temp13 = "Adjust: Zoom into Point of Interest";

    setcolor(BLACK);
    settextrotation(0);
    setfontsize(60);
    long double x = 10000000000000;
    drawtext(screen_temp.get_xcenter(), screen_temp.get_ycenter() + 0.14 * offset, temp1, x, x);
    setfontsize(50);
    drawtext(screen_temp.get_xcenter(), screen_temp.get_ycenter() + 0.1 * offset, temp, x, x);
    setfontsize(10);
    drawtext(screen_temp.get_xcenter(), screen_temp.get_ycenter() + 0.07 * offset, temp2, x, x);
    setfontsize(10);
    drawtext(screen_temp.get_xcenter(), screen_temp.get_ycenter() + 0.05 * offset, temp3, x, x);
    setfontsize(10);
    drawtext(screen_temp.get_xcenter(), screen_temp.get_ycenter() + 0.03 * offset, temp4, x, x);
    setfontsize(10);
    drawtext(screen_temp.get_xcenter(), screen_temp.get_ycenter() + 0.01 * offset, temp5, x, x);
    setfontsize(10);
    drawtext(screen_temp.get_xcenter(), screen_temp.get_ycenter() - 0.01 * offset, temp6, x, x);
    setfontsize(10);
    drawtext(screen_temp.get_xcenter(), screen_temp.get_ycenter() - 0.03 * offset, temp7, x, x);
    setfontsize(10);
    drawtext(screen_temp.get_xcenter(), screen_temp.get_ycenter() - 0.05 * offset, temp8, x, x);
    setfontsize(10);
    drawtext(screen_temp.get_xcenter(), screen_temp.get_ycenter() - 0.07 * offset, temp9, x, x);
    setfontsize(10);
    drawtext(screen_temp.get_xcenter(), screen_temp.get_ycenter() - 0.09 * offset, temp10, x, x);
    setfontsize(10);
    drawtext(screen_temp.get_xcenter(), screen_temp.get_ycenter() - 0.11 * offset, temp11, x, x);
    setfontsize(10);
    drawtext(screen_temp.get_xcenter(), screen_temp.get_ycenter() - 0.13 * offset, temp12, x, x);
    setfontsize(10);
    drawtext(screen_temp.get_xcenter(), screen_temp.get_ycenter() - 0.15 * offset, temp13, x, x);
}

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

bool check_street_valid(string streetName) {
    if (streetName.empty() || streetName == "<unknown>") {
        return false;
    } else if (find_street_ids_from_name(streetName).size() == 0) {
        return false;
    } else {
        return true;
    }
}

void draw_highway(int zoomLevel, float radius) {
    vector<unsigned> highway = singleton -> get_Highway();
    for (auto iter = highway.begin(); iter != highway.end(); iter++) {
        setlinestyle(SOLID);
        setcolor(HIGHWAYPURPLE);
        setlinewidth(zoomLevel);
        draw_segment(*iter, radius);
    }
}

void draw_mainroad(int zoomLevel, float radius) {
    vector<unsigned> mainroad = singleton -> get_mainroad();
    for (auto iter = mainroad.begin(); iter != mainroad.end(); iter++) {
        setlinestyle(SOLID);
        setlinewidth(zoomLevel);
        setcolor(SANDYELLOW);
        draw_segment(*iter, radius);
    }
}

void draw_sideroad(int zoomLevel, float radius) {
    vector<unsigned> sideroad = singleton -> get_sideroad();
    for (auto iter = sideroad.begin(); iter != sideroad.end(); iter++) {
        setlinestyle(SOLID);
        setcolor(WHITE);
        setlinewidth(zoomLevel);
        draw_segment(*iter, radius);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

void draw_POI_name(t_point POI_position, unsigned int POI_id) {
    double textWidth = POI_text_width;
    double testHeight = POI_testHeight;
    t_bound_box textBox(t_point((POI_position.x) - textWidth / 2, (POI_position.y) + RAD_POI), textWidth, testHeight);
    settextrotation(0);
    setcolor(BLACK);
    setfontsize(font_size[zoom_level]); //use a enum array of consts in the contsM2.h
    string name = singleton->get_hash_POIID_Point_POIname(POI_id);
    if (name != "<noname>") {
        drawtext(textBox.get_center(), name, textBox.get_width(), textBox.get_height());
    }

}

double setRoadNameAngleID(unsigned ID) {
    double angle;
    //StreetSegmentsEnds endpoints = getStreetSegmentEnds(ID);
    //argument is segmentID
    unsigned long from = singleton->get_Hash_SegmentID_Point_StreetSegmentInfo(ID).from;
    unsigned long to = singleton->get_Hash_SegmentID_Point_StreetSegmentInfo(ID).to;
    t_point secondpoint = singleton->get_intersectionPositionIn_xy_coordinate(from);
    t_point firstpoint = singleton->get_intersectionPositionIn_xy_coordinate(to);

    if ((secondpoint.x - firstpoint.x) == 0) {
        return 0;
    }
    angle = atan((secondpoint.y - firstpoint.y) / (secondpoint.x - firstpoint.x))*180 / PI;
    //in case angle is negative
    while (angle < 0)
        angle += FULL_CIRCLE_DEG;

    return angle;
}

double setRoadNameAngle(t_point& firstpoint, t_point & secondpoint) {
    double angle;

    if ((secondpoint.x - firstpoint.x) == 0) {
        return 0;
    }
    angle = atan((secondpoint.y - firstpoint.y) / (secondpoint.x - firstpoint.x))*180 / PI;
    //in case angle is negative
    while (angle < 0)
        angle += FULL_CIRCLE_DEG;

    return angle;
}

//to change the bound box, +/- num to bottom, left, right, up coordinates

void draw_streetName(void) 
{
    //iterate through every street segment
    unsigned int max = singleton->get_segment_numbers();
    //iterate through every segment ID
    for (unsigned int i = 0; i < max; i++) 
    {
        //find the XY coordinates from start and end intersections segmentID
        unsigned long from = singleton->get_Hash_SegmentID_Point_StreetSegmentInfo(i).from;
        unsigned long to = singleton->get_Hash_SegmentID_Point_StreetSegmentInfo(i).to;
        t_point start = singleton->get_intersectionPositionIn_xy_coordinate(from);
        t_point end = singleton->get_intersectionPositionIn_xy_coordinate(to);
        //get the streetID from every segmentID
        unsigned StreetID = singleton->get_Hash_SegmentID_Point_StreetSegmentInfo(i).streetID;
        //find the name for that StreetID
        string segment_name = getStreetName(StreetID);
        //return diagonal of the segment
        //assign the number of curve points for a given segmentID
        unsigned curve_points = singleton->get_Hash_SegmentID_Point_StreetSegmentInfo(i).curvePointCount;

        if (segment_name != "<unknown>") {
            //curve the text
            float angle = setRoadNameAngleID(i);
            settextrotation(angle);
            //if it is a straight line
            if (curve_points == 0) 
            {
                //find the length of the street
                float length = find_street_segment_length(i);
                //at level 8
                if (zoom_level == 8) 
                {
                    t_point mid_coord;
                    mid_coord.x = (start.x + end.x) / 2;
                    mid_coord.y = (start.y + end.y) / 2;
                    setcolor(BLACK);
                    setfontsize(7);
                    drawtext(mid_coord, segment_name, length / 100000 - LVL8, length / 100000 - LVL8);
                } 
                else if (zoom_level == 9) 
                {
                    t_point mid_coord;
                    mid_coord.x = (start.x + end.x) / 2;
                    mid_coord.y = (start.y + end.y) / 2;
                    setcolor(BLACK);
                    setfontsize(9);
                    drawtext(mid_coord, segment_name, length / 10000 - LVL9, length / 10000 - LVL9);
                } 
                else if (zoom_level == 10) 
                {
                    t_point mid_coord;
                    mid_coord.x = (start.x + end.x) / 2;
                    mid_coord.y = (start.y + end.y) / 2;
                    setcolor(BLACK);
                    setfontsize(11);
                    drawtext(mid_coord, segment_name, length / 1000 - LVL10, length / 1000 - LVL10);

                } 
                else if (zoom_level == 11) 
                {
                    t_point mid_coord;
                    mid_coord.x = (start.x + end.x) / 2;
                    mid_coord.y = (start.y + end.y) / 2;
                    setcolor(BLACK);
                    setfontsize(12);
                    drawtext(mid_coord, segment_name, length / 100000, length / 100000);
                } else if (zoom_level == 12) {
                    t_point mid_coord;
                    mid_coord.x = (start.x + end.x) / 2;
                    mid_coord.y = (start.y + end.y) / 2;
                    setcolor(BLACK);
                    setfontsize(16);
                    drawtext(mid_coord, segment_name, length / 100000, length / 100000);
                }
            } else if (curve_points == 1) {

                //get the segment info of a street segmentID
                StreetSegmentInfo temp = singleton->get_Hash_SegmentID_Point_StreetSegmentInfo(i);
                //get the curve points in XY coordinate coordinates
                vector <t_point> curve_in_segment = singleton->get_curvePoints_in_xy_coordinate(i);
                t_point mid_coordone;
                mid_coordone.x = start.x + curve_in_segment[0].x / 2;
                mid_coordone.y = start.y + curve_in_segment[0].y / 2;
                t_point mid_coordtwo;
                mid_coordtwo.x = curve_in_segment[0].x + end.x / 2;
                mid_coordtwo.y = curve_in_segment[0].y + end.y / 2;

                double angle = setRoadNameAngle(start, curve_in_segment[0]);
                double angle1 = setRoadNameAngle(curve_in_segment[0], end);


                LatLon pt1;
                LatLon pt2;
                LatLon pt3;

                pt1.lat = start.y;
                pt1.lon = start.x / (singleton->get_cosAvg());

                pt2.lat = curve_in_segment[0].y;
                pt2.lon = curve_in_segment[0].x / (singleton->get_cosAvg());

                pt3.lat = end.y;
                pt3.lon = end.x / (singleton->get_cosAvg());

                double length1 = find_distance_between_two_points(pt1, pt2);
                double length2 = find_distance_between_two_points(pt2, pt3);

                setcolor(BLACK);

                setfontsize(font_size[zoom_level]);
                if (zoom_level == 8) 
                {
                    setfontsize(7);
                    settextrotation(angle);
                    drawtext(mid_coordone, segment_name, length1 / 100000 - LVL8, length1 / 100000 - LVL8);
                    settextrotation(angle1);
                    drawtext(mid_coordtwo, segment_name, length2 / 100000 - LVL8, length2 / 100000 - LVL8);
                } else if (zoom_level == 9) {
                    setfontsize(9);
                    settextrotation(angle);
                    drawtext(mid_coordone, segment_name, length1 / 10000 - LVL9, length1 / 10000 - LVL9);
                    settextrotation(angle1);
                    drawtext(mid_coordtwo, segment_name, length2 / 10000 - LVL9, length2 / 10000 - LVL9);
                } else if (zoom_level == 10) {
                    setfontsize(11);
                    settextrotation(angle);
                    drawtext(mid_coordone, segment_name, length1 / 1000 - LVL10, length1 / 1000 - LVL10);
                    settextrotation(angle1);
                    drawtext(mid_coordtwo, segment_name, length2 / 1000 - LVL10, length2 / 1000 - LVL10);
                } else if (zoom_level == 11) {

                    setfontsize(12);
                    settextrotation(angle);
                    drawtext(mid_coordone, segment_name, length1 / 1000000, length1 / 1000000);
                    settextrotation(angle1);
                    drawtext(mid_coordtwo, segment_name, length2 / 1000000, length2 / 1000000);

                } else if (zoom_level == 12) {
                    setfontsize(16);
                    settextrotation(angle);
                    drawtext(mid_coordone, segment_name, length1 / 1000000, length1 / 1000000);
                    settextrotation(angle1);
                    drawtext(mid_coordtwo, segment_name, length2 / 1000000, length2 / 1000000);
                }

            } else {

                //get the segment info of a street segmentID
                StreetSegmentInfo temp = singleton->get_Hash_SegmentID_Point_StreetSegmentInfo(i);
                //get the curve points in XY coordinate coordinates
                vector <t_point> curve_in_segment = singleton->get_curvePoints_in_xy_coordinate(i);
                for (unsigned int i = 0; i < curve_in_segment.size() - 1; i++) {
                    double angle = setRoadNameAngle(curve_in_segment[i], curve_in_segment[i + 1]);
                    settextrotation(angle);
                    LatLon pt1;
                    LatLon pt2;

                    pt1.lat = (curve_in_segment[i]).y;
                    pt1.lon = (curve_in_segment[i]).x / (singleton->get_cosAvg());

                    pt2.lat = (curve_in_segment[i + 1]).y;
                    pt2.lon = (curve_in_segment[i + 1]).x / (singleton->get_cosAvg());

                    t_point mid_coord2;
                    mid_coord2.x = ((curve_in_segment[i]).x + (curve_in_segment[i + 1]).x) / 2;
                    mid_coord2.y = ((curve_in_segment[i]).y + (curve_in_segment[i + 1]).y) / 2;

                    double curve_length = find_distance_between_two_points(pt1, pt2);

                    if (zoom_level == 8) {
                        setcolor(BLACK);
                        setfontsize(7);
                        drawtext(mid_coord2, segment_name, curve_length / 100000 - LVL8 * 2, curve_length / 100000 - LVL8 * 2);
                    } else if (zoom_level == 9) {
                        setcolor(BLACK);
                        setfontsize(9);
                        drawtext(mid_coord2, segment_name, curve_length / 10000 - LVL9 * 2, curve_length / 10000 - LVL9 * 2);
                    } 
                    else if (zoom_level == 10) 
                    {
                        setcolor(BLACK);
                        setfontsize(11);
                        drawtext(mid_coord2, segment_name, curve_length / 1000 - LVL10 * 2, curve_length / 1000 - LVL10 * 2);

                    } 
                    else if (zoom_level == 11) 
                    {
                        setcolor(BLACK);
                        setfontsize(12);
                        drawtext(mid_coord2, segment_name, curve_length / 10000000, curve_length / 10000000);
                    } 
                    else if (zoom_level == 12) 
                    {
                        //set the colour to black
                        setcolor(BLACK);
                        setfontsize(16);
                        drawtext(mid_coord2, segment_name, curve_length / 1000000, curve_length / 1000000);
                    }
                }
            }
        }
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

void draw_segment(unsigned segment_id, float radius) 
{
    StreetSegmentInfo this_segment = singleton->get_Hash_SegmentID_Point_StreetSegmentInfo(segment_id);
    //get the intersectionID of START and END
    unsigned int intersection1 = this_segment.from;
    unsigned int intersection2 = this_segment.to;
    //get the position of two intersections in xy_coordinate, using id as key
    t_point from_position = singleton->get_intersectionPositionIn_xy_coordinate(intersection1);
    t_point to_position = singleton->get_intersectionPositionIn_xy_coordinate(intersection2);
    if (zoom_level == 0) {
        fillarc(from_position, radius, 0, FULL_CIRCLE_DEG);
        fillarc(to_position, radius, 0, FULL_CIRCLE_DEG);
        //find the number of curve points of a segemnt
        unsigned curve_points = singleton->get_Hash_SegmentID_Point_StreetSegmentInfo(segment_id).curvePointCount;
        //return a vector containing the XY coordinate of every curve point
        vector <t_point> curve_in_segment = singleton->get_curvePoints_in_xy_coordinate(segment_id);
        if (curve_points == 0) 
        {
            //if straight line, just draw from start to finish
            drawline(from_position, to_position);
            //fix the ugly  segments
            fillarc(from_position, radius, 0, FULL_CIRCLE_DEG);
            fillarc(to_position, radius, 0, FULL_CIRCLE_DEG);
            //see if we ned to draw the arrow foe oneway street
            if (zoom_level > 6) {
                if (this_segment.oneWay) 
                {
                    t_bound_box arrowBox((from_position.x) - 0.03, (from_position.y) - 0.03, (from_position.x) + 0.03, (from_position.y) + 0.03);
                    setcolor(SEGMENTDARKGREY);
                    double angle = setRoadNameAngle(from_position, to_position);
                    settextrotation(angle);
                    drawtext_in(arrowBox, "->");
                }
            }
        } else {
            t_point current_point = from_position;
            //iterate through every curve straight line that make up of a segment
            for (auto i = curve_in_segment.begin(); i != curve_in_segment.end(); i++) 
            {
                //draw the mini straight line segment
                fillarc(current_point, radius, 0, FULL_CIRCLE_DEG);
                drawline(current_point, *i);
                //draw an arrow ->
                current_point = *i;
                //fix ugly segments
                fillarc(current_point, radius, 0, FULL_CIRCLE_DEG);
            }
            //draw the last segment to the end point
            drawline(current_point, to_position);
            //fix ugly segments
            fillarc(current_point, radius, 0, FULL_CIRCLE_DEG);
            fillarc(to_position, radius, 0, FULL_CIRCLE_DEG);
        }
        //draw the last segment to the end point
    } else if (zoom_level == 1) {
        fillarc(from_position, radius, 0, FULL_CIRCLE_DEG);
        fillarc(to_position, radius, 0, FULL_CIRCLE_DEG);
        //find the number of curve points of a segemnt
        unsigned curve_points = singleton->get_Hash_SegmentID_Point_StreetSegmentInfo(segment_id).curvePointCount;
        //return a vector containing the XY coordinate of every curve point
        vector <t_point> curve_in_segment = singleton->get_curvePoints_in_xy_coordinate(segment_id);
        if (curve_points == 0) 
        {
            //if straight line, just draw from start to finish
            drawline(from_position, to_position);
            //fix the ugly  segments
            fillarc(from_position, radius, 0, FULL_CIRCLE_DEG);
            fillarc(to_position, radius, 0, FULL_CIRCLE_DEG);
            //see if we ned to draw the arrow foe oneway street
            if (zoom_level > 6) {
                if (this_segment.oneWay) 
                {
                    t_bound_box arrowBox((from_position.x) - 0.03, (from_position.y) - 0.03, (from_position.x) + 0.03, (from_position.y) + 0.03);
                    setcolor(SEGMENTDARKGREY);
                    double angle = setRoadNameAngle(from_position, to_position);
                    settextrotation(angle);
                    drawtext_in(arrowBox, "->");
                }
            }
        } else {
            t_point current_point = from_position;
            //iterate through every curve straight line that make up of a segment
            for (auto i = curve_in_segment.begin(); i != curve_in_segment.end(); i++) 
            {
                //draw the mini straight line segment
                fillarc(current_point, radius, 0, FULL_CIRCLE_DEG);
                drawline(current_point, *i);
                //draw an arrow ->
                current_point = *i;
                //fix ugly segments
                fillarc(current_point, radius, 0, FULL_CIRCLE_DEG);
            }
            //draw the last segment to the end point
            drawline(current_point, to_position);
            //fix ugly segments
            fillarc(current_point, radius, 0, FULL_CIRCLE_DEG);
            fillarc(to_position, radius, 0, FULL_CIRCLE_DEG);
        }
        //draw the last segment to the end point
    } else if (zoom_level == 2) {
        fillarc(from_position, radius, 0, FULL_CIRCLE_DEG);
        fillarc(to_position, radius, 0, FULL_CIRCLE_DEG);
        //find the number of curve points of a segemnt
        unsigned curve_points = singleton->get_Hash_SegmentID_Point_StreetSegmentInfo(segment_id).curvePointCount;
        //return a vector containing the XY coordinate of every curve point
        vector <t_point> curve_in_segment = singleton->get_curvePoints_in_xy_coordinate(segment_id);
        if (curve_points == 0) 
        {
            //if straight line, just draw from start to finish
            drawline(from_position, to_position);
            //fix the ugly  segments
            fillarc(from_position, radius, 0, FULL_CIRCLE_DEG);
            fillarc(to_position, radius, 0, FULL_CIRCLE_DEG);
            //see if we ned to draw the arrow foe oneway street
            if (zoom_level > 6) {
                if (this_segment.oneWay) 
                {
                    t_bound_box arrowBox((from_position.x) - 0.03, (from_position.y) - 0.03, (from_position.x) + 0.03, (from_position.y) + 0.03);
                    setcolor(SEGMENTDARKGREY);
                    double angle = setRoadNameAngle(from_position, to_position);
                    settextrotation(angle);
                    drawtext_in(arrowBox, "->");
                }
            }
        } else {
            t_point current_point = from_position;
            //iterate through every curve straight line that make up of a segment
            for (auto i = curve_in_segment.begin(); i != curve_in_segment.end(); i++) 
            {
                //draw the mini straight line segment
                fillarc(current_point, radius, 0, FULL_CIRCLE_DEG);
                drawline(current_point, *i);
                //draw an arrow ->
                current_point = *i;
                //fix ugly segments
                fillarc(current_point, radius, 0, FULL_CIRCLE_DEG);
            }
            //draw the last segment to the end point
            drawline(current_point, to_position);
            //fix ugly segments
            fillarc(current_point, radius, 0, FULL_CIRCLE_DEG);
            fillarc(to_position, radius, 0, FULL_CIRCLE_DEG);
        }
        //draw the last segment to the end point
    } else if (zoom_level == 3) {
        fillarc(from_position, radius, 0, FULL_CIRCLE_DEG);
        fillarc(to_position, radius, 0, FULL_CIRCLE_DEG);
        //find the number of curve points of a segemnt
        unsigned curve_points = singleton->get_Hash_SegmentID_Point_StreetSegmentInfo(segment_id).curvePointCount;
        //return a vector containing the XY coordinate of every curve point
        vector <t_point> curve_in_segment = singleton->get_curvePoints_in_xy_coordinate(segment_id);
        if (curve_points == 0) 
        {
            //if straight line, just draw from start to finish
            drawline(from_position, to_position);
            //fix the ugly  segments
            fillarc(from_position, radius, 0, FULL_CIRCLE_DEG);
            fillarc(to_position, radius, 0, FULL_CIRCLE_DEG);
            //see if we ned to draw the arrow foe oneway street
            if (zoom_level > 6) {
                if (this_segment.oneWay) 
                {
                    t_bound_box arrowBox((from_position.x) - 0.03, (from_position.y) - 0.03, (from_position.x) + 0.03, (from_position.y) + 0.03);
                    setcolor(SEGMENTDARKGREY);
                    double angle = setRoadNameAngle(from_position, to_position);
                    settextrotation(angle);
                    drawtext_in(arrowBox, "->");
                }
            }
        } else {
            t_point current_point = from_position;
            //iterate through every curve straight line that make up of a segment
            for (auto i = curve_in_segment.begin(); i != curve_in_segment.end(); i++) 
            {
                //draw the mini straight line segment
                fillarc(current_point, radius, 0, FULL_CIRCLE_DEG);
                drawline(current_point, *i);
                //draw an arrow ->
                current_point = *i;
                //fix ugly segments
                fillarc(current_point, radius, 0, FULL_CIRCLE_DEG);
            }
            //draw the last segment to the end point
            drawline(current_point, to_position);
            //fix ugly segments
            fillarc(current_point, radius, 0, FULL_CIRCLE_DEG);
            fillarc(to_position, radius, 0, FULL_CIRCLE_DEG);
        }
        //draw the last segment to the end point
    } else if (zoom_level == 4) {
        fillarc(from_position, radius, 0, FULL_CIRCLE_DEG);
        fillarc(to_position, radius, 0, FULL_CIRCLE_DEG);
        //find the number of curve points of a segemnt
        unsigned curve_points = singleton->get_Hash_SegmentID_Point_StreetSegmentInfo(segment_id).curvePointCount;
        //return a vector containing the XY coordinate of every curve point
        vector <t_point> curve_in_segment = singleton->get_curvePoints_in_xy_coordinate(segment_id);
        if (curve_points == 0) 
        {
            //if straight line, just draw from start to finish
            drawline(from_position, to_position);
            //fix the ugly  segments
            fillarc(from_position, radius, 0, FULL_CIRCLE_DEG);
            fillarc(to_position, radius, 0, FULL_CIRCLE_DEG);
            //see if we ned to draw the arrow foe oneway street
            if (zoom_level > 6) {
                if (this_segment.oneWay) 
                {
                    t_bound_box arrowBox((from_position.x) - 0.03, (from_position.y) - 0.03, (from_position.x) + 0.03, (from_position.y) + 0.03);
                    setcolor(SEGMENTDARKGREY);
                    double angle = setRoadNameAngle(from_position, to_position);
                    settextrotation(angle);
                    drawtext_in(arrowBox, "->");
                }
            }
        } else {
            t_point current_point = from_position;
            //iterate through every curve straight line that make up of a segment
            for (auto i = curve_in_segment.begin(); i != curve_in_segment.end(); i++) 
            {
                //draw the mini straight line segment
                fillarc(current_point, radius, 0, FULL_CIRCLE_DEG);
                drawline(current_point, *i);
                //draw an arrow ->
                current_point = *i;
                //fix ugly segments
                fillarc(current_point, radius, 0, FULL_CIRCLE_DEG);
            }
            //draw the last segment to the end point
            drawline(current_point, to_position);
            //fix ugly segments
            fillarc(current_point, radius, 0, FULL_CIRCLE_DEG);
            fillarc(to_position, radius, 0, FULL_CIRCLE_DEG);
        }
        //draw the last segment to the end point
    } else if (zoom_level == 5) {
        fillarc(from_position, radius, 0, FULL_CIRCLE_DEG);
        fillarc(to_position, radius, 0, FULL_CIRCLE_DEG);
        //find the number of curve points of a segemnt
        unsigned curve_points = singleton->get_Hash_SegmentID_Point_StreetSegmentInfo(segment_id).curvePointCount;
        //return a vector containing the XY coordinate of every curve point
        vector <t_point> curve_in_segment = singleton->get_curvePoints_in_xy_coordinate(segment_id);
        if (curve_points == 0) 
        {
            //if straight line, just draw from start to finish
            drawline(from_position, to_position);
            //fix the ugly  segments
            fillarc(from_position, radius, 0, FULL_CIRCLE_DEG);
            fillarc(to_position, radius, 0, FULL_CIRCLE_DEG);
            //see if we ned to draw the arrow foe oneway street
            if (zoom_level > 6) {
                if (this_segment.oneWay) 
                {
                    t_bound_box arrowBox((from_position.x) - 0.03, (from_position.y) - 0.03, (from_position.x) + 0.03, (from_position.y) + 0.03);
                    setcolor(SEGMENTDARKGREY);
                    double angle = setRoadNameAngle(from_position, to_position);
                    settextrotation(angle);
                    drawtext_in(arrowBox, "->");
                }
            }
        } else {
            t_point current_point = from_position;
            //iterate through every curve straight line that make up of a segment
            for (auto i = curve_in_segment.begin(); i != curve_in_segment.end(); i++) 
            {
                //draw the mini straight line segment
                fillarc(current_point, radius, 0, FULL_CIRCLE_DEG);
                drawline(current_point, *i);
                //draw an arrow ->
                current_point = *i;
                //fix ugly segments
                fillarc(current_point, radius, 0, FULL_CIRCLE_DEG);
            }
            //draw the last segment to the end point
            drawline(current_point, to_position);
            //fix ugly segments
            fillarc(current_point, radius, 0, FULL_CIRCLE_DEG);
            fillarc(to_position, radius, 0, FULL_CIRCLE_DEG);
        }
        //draw the last segment to the end point
    } else if (zoom_level == 6) {
        fillarc(from_position, radius, 0, FULL_CIRCLE_DEG);
        fillarc(to_position, radius, 0, FULL_CIRCLE_DEG);
        //find the number of curve points of a segemnt
        unsigned curve_points = singleton->get_Hash_SegmentID_Point_StreetSegmentInfo(segment_id).curvePointCount;
        //return a vector containing the XY coordinate of every curve point
        vector <t_point> curve_in_segment = singleton->get_curvePoints_in_xy_coordinate(segment_id);
        if (curve_points == 0) 
        {
            //if straight line, just draw from start to finish
            drawline(from_position, to_position);
            //fix the ugly  segments
            fillarc(from_position, radius, 0, FULL_CIRCLE_DEG);
            fillarc(to_position, radius, 0, FULL_CIRCLE_DEG);
            //see if we ned to draw the arrow foe oneway street
            if (zoom_level > 6) {
                if (this_segment.oneWay) 
                {
                    t_bound_box arrowBox((from_position.x) - 0.03, (from_position.y) - 0.03, (from_position.x) + 0.03, (from_position.y) + 0.03);
                    setcolor(SEGMENTDARKGREY);
                    double angle = setRoadNameAngle(from_position, to_position);
                    settextrotation(angle);
                    drawtext_in(arrowBox, "->");
                }
            }
        } else {
            t_point current_point = from_position;
            //iterate through every curve straight line that make up of a segment
            for (auto i = curve_in_segment.begin(); i != curve_in_segment.end(); i++) 
            {
                //draw the mini straight line segment
                fillarc(current_point, radius, 0, FULL_CIRCLE_DEG);
                drawline(current_point, *i);
                //draw an arrow ->
                current_point = *i;
                //fix ugly segments
                fillarc(current_point, radius, 0, FULL_CIRCLE_DEG);
            }
            //draw the last segment to the end point
            drawline(current_point, to_position);
            //fix ugly segments
            fillarc(current_point, radius, 0, FULL_CIRCLE_DEG);
            fillarc(to_position, radius, 0, FULL_CIRCLE_DEG);
        }
        //draw the last segment to the end point
    } else if (zoom_level == 7) {
        fillarc(from_position, radius, 0, FULL_CIRCLE_DEG);
        fillarc(to_position, radius, 0, FULL_CIRCLE_DEG);
        //find the number of curve points of a segemnt
        unsigned curve_points = singleton->get_Hash_SegmentID_Point_StreetSegmentInfo(segment_id).curvePointCount;
        //return a vector containing the XY coordinate of every curve point
        vector <t_point> curve_in_segment = singleton->get_curvePoints_in_xy_coordinate(segment_id);
        if (curve_points == 0) 
        {
            //if straight line, just draw from start to finish
            drawline(from_position, to_position);
            //fix the ugly  segments
            fillarc(from_position, radius, 0, FULL_CIRCLE_DEG);
            fillarc(to_position, radius, 0, FULL_CIRCLE_DEG);
            //see if we ned to draw the arrow foe oneway street
            if (zoom_level > 6) {
                if (this_segment.oneWay) 
                {
                    t_bound_box arrowBox((from_position.x) - 0.03, (from_position.y) - 0.03, (from_position.x) + 0.03, (from_position.y) + 0.03);
                    setcolor(SEGMENTDARKGREY);
                    double angle = setRoadNameAngle(from_position, to_position);
                    settextrotation(angle);
                    drawtext_in(arrowBox, "->");
                }
            }
        } else {
            t_point current_point = from_position;
            //iterate through every curve straight line that make up of a segment
            for (auto i = curve_in_segment.begin(); i != curve_in_segment.end(); i++) 
            {
                //draw the mini straight line segment
                fillarc(current_point, radius, 0, FULL_CIRCLE_DEG);
                drawline(current_point, *i);
                //draw an arrow ->
                current_point = *i;
                //fix ugly segments
                fillarc(current_point, radius, 0, FULL_CIRCLE_DEG);
            }
            //draw the last segment to the end point
            drawline(current_point, to_position);
            //fix ugly segments
            fillarc(current_point, radius, 0, FULL_CIRCLE_DEG);
            fillarc(to_position, radius, 0, FULL_CIRCLE_DEG);
        }
        //draw the last segment to the end point
        //draw the last segment to the end point
    } else if (zoom_level == 8) {
        fillarc(from_position, radius, 0, FULL_CIRCLE_DEG);
        fillarc(to_position, radius, 0, FULL_CIRCLE_DEG);
        //find the number of curve points of a segemnt
        unsigned curve_points = singleton->get_Hash_SegmentID_Point_StreetSegmentInfo(segment_id).curvePointCount;
        //return a vector containing the XY coordinate of every curve point
        vector <t_point> curve_in_segment = singleton->get_curvePoints_in_xy_coordinate(segment_id);
        if (curve_points == 0) 
        {
            //if straight line, just draw from start to finish
            drawline(from_position, to_position);
            //fix the ugly  segments
            fillarc(from_position, radius, 0, FULL_CIRCLE_DEG);
            fillarc(to_position, radius, 0, FULL_CIRCLE_DEG);
            //see if we ned to draw the arrow foe oneway street
            if (zoom_level > 6) {
                if (this_segment.oneWay) 
                {
                    t_bound_box arrowBox((from_position.x) - 0.03, (from_position.y) - 0.03, (from_position.x) + 0.03, (from_position.y) + 0.03);
                    setcolor(SEGMENTDARKGREY);
                    double angle = setRoadNameAngle(from_position, to_position);
                    settextrotation(angle);
                    drawtext_in(arrowBox, "->");
                }
            }
        } else {
            t_point current_point = from_position;
            //iterate through every curve straight line that make up of a segment
            for (auto i = curve_in_segment.begin(); i != curve_in_segment.end(); i++) 
            {
                //draw the mini straight line segment
                fillarc(current_point, radius, 0, FULL_CIRCLE_DEG);
                drawline(current_point, *i);
                //draw an arrow ->
                current_point = *i;
                //fix ugly segments
                fillarc(current_point, radius, 0, FULL_CIRCLE_DEG);
            }
            //draw the last segment to the end point
            drawline(current_point, to_position);
            //fix ugly segments
            fillarc(current_point, radius, 0, FULL_CIRCLE_DEG);
            fillarc(to_position, radius, 0, FULL_CIRCLE_DEG);
        }
        //draw the last segment to the end point
    } else if (zoom_level == 9) {
        fillarc(from_position, radius, 0, FULL_CIRCLE_DEG);
        fillarc(to_position, radius, 0, FULL_CIRCLE_DEG);
        //find the number of curve points of a segemnt
        unsigned curve_points = singleton->get_Hash_SegmentID_Point_StreetSegmentInfo(segment_id).curvePointCount;
        //return a vector containing the XY coordinate of every curve point
        vector <t_point> curve_in_segment = singleton->get_curvePoints_in_xy_coordinate(segment_id);
        if (curve_points == 0) 
        {
            //if straight line, just draw from start to finish
            drawline(from_position, to_position);
            //fix the ugly  segments
            fillarc(from_position, radius, 0, FULL_CIRCLE_DEG);
            fillarc(to_position, radius, 0, FULL_CIRCLE_DEG);
            //see if we ned to draw the arrow foe oneway street
            if (zoom_level > 6) {
                if (this_segment.oneWay) 
                {
                    t_bound_box arrowBox((from_position.x) - 0.03, (from_position.y) - 0.03, (from_position.x) + 0.03, (from_position.y) + 0.03);
                    setcolor(SEGMENTDARKGREY);
                    double angle = setRoadNameAngle(from_position, to_position);
                    settextrotation(angle);
                    drawtext_in(arrowBox, "->");
                }
            }
        } else {
            t_point current_point = from_position;
            //iterate through every curve straight line that make up of a segment
            for (auto i = curve_in_segment.begin(); i != curve_in_segment.end(); i++) 
            {
                //draw the mini straight line segment
                fillarc(current_point, radius, 0, FULL_CIRCLE_DEG);
                drawline(current_point, *i);
                //draw an arrow ->
                current_point = *i;
                //fix ugly segments
                fillarc(current_point, radius, 0, FULL_CIRCLE_DEG);
            }
            //draw the last segment to the end point
            drawline(current_point, to_position);
            //fix ugly segments
            fillarc(current_point, radius, 0, FULL_CIRCLE_DEG);
            fillarc(to_position, radius, 0, FULL_CIRCLE_DEG);
        }
        //draw the last segment to the end point
    } else if (zoom_level == 10) {
        fillarc(from_position, radius, 0, FULL_CIRCLE_DEG);
        fillarc(to_position, radius, 0, FULL_CIRCLE_DEG);
        //find the number of curve points of a segemnt
        unsigned curve_points = singleton->get_Hash_SegmentID_Point_StreetSegmentInfo(segment_id).curvePointCount;
        //return a vector containing the XY coordinate of every curve point
        vector <t_point> curve_in_segment = singleton->get_curvePoints_in_xy_coordinate(segment_id);
        if (curve_points == 0) 
        {
            //if straight line, just draw from start to finish
            drawline(from_position, to_position);
            //fix the ugly  segments
            fillarc(from_position, radius, 0, FULL_CIRCLE_DEG);
            fillarc(to_position, radius, 0, FULL_CIRCLE_DEG);
            //see if we ned to draw the arrow foe oneway street
            if (zoom_level > 6) {
                if (this_segment.oneWay) 
                {
                    t_bound_box arrowBox((from_position.x) - 0.03, (from_position.y) - 0.03, (from_position.x) + 0.03, (from_position.y) + 0.03);
                    setcolor(SEGMENTDARKGREY);
                    double angle = setRoadNameAngle(from_position, to_position);
                    settextrotation(angle);
                    drawtext_in(arrowBox, "->");
                }
            }
        } else {
            t_point current_point = from_position;
            //iterate through every curve straight line that make up of a segment
            for (auto i = curve_in_segment.begin(); i != curve_in_segment.end(); i++) 
            {
                //draw the mini straight line segment
                fillarc(current_point, radius, 0, FULL_CIRCLE_DEG);
                drawline(current_point, *i);
                //draw an arrow ->
                current_point = *i;
                //fix ugly segments
                fillarc(current_point, radius, 0, FULL_CIRCLE_DEG);
            }
            //draw the last segment to the end point
            drawline(current_point, to_position);
            //fix ugly segments
            fillarc(current_point, radius, 0, FULL_CIRCLE_DEG);
            fillarc(to_position, radius, 0, FULL_CIRCLE_DEG);
        }
        //draw the last segment to the end point
    } else if (zoom_level == 11) {
        fillarc(from_position, radius, 0, FULL_CIRCLE_DEG);
        fillarc(to_position, radius, 0, FULL_CIRCLE_DEG);
        //find the number of curve points of a segemnt
        unsigned curve_points = singleton->get_Hash_SegmentID_Point_StreetSegmentInfo(segment_id).curvePointCount;
        //return a vector containing the XY coordinate of every curve point
        vector <t_point> curve_in_segment = singleton->get_curvePoints_in_xy_coordinate(segment_id);
        if (curve_points == 0) 
        {
            //if straight line, just draw from start to finish
            drawline(from_position, to_position);
            //fix the ugly  segments
            fillarc(from_position, radius, 0, FULL_CIRCLE_DEG);
            fillarc(to_position, radius, 0, FULL_CIRCLE_DEG);
            //see if we ned to draw the arrow foe oneway street
            if (zoom_level > 6) {
                if (this_segment.oneWay) 
                {
                    t_bound_box arrowBox((from_position.x) - 0.03, (from_position.y) - 0.03, (from_position.x) + 0.03, (from_position.y) + 0.03);
                    setcolor(SEGMENTDARKGREY);
                    double angle = setRoadNameAngle(from_position, to_position);
                    settextrotation(angle);
                    drawtext_in(arrowBox, "->");
                }
            }
        } else {
            t_point current_point = from_position;
            //iterate through every curve straight line that make up of a segment
            for (auto i = curve_in_segment.begin(); i != curve_in_segment.end(); i++) 
            {
                //draw the mini straight line segment
                fillarc(current_point, radius, 0, FULL_CIRCLE_DEG);
                drawline(current_point, *i);
                //draw an arrow ->
                current_point = *i;
                //fix ugly segments
                fillarc(current_point, radius, 0, FULL_CIRCLE_DEG);
            }
            //draw the last segment to the end point
            drawline(current_point, to_position);
            //fix ugly segments
            fillarc(current_point, radius, 0, FULL_CIRCLE_DEG);
            fillarc(to_position, radius, 0, FULL_CIRCLE_DEG);
        }
        //draw the last segment to the end point
    } else if (zoom_level == 12) {
        fillarc(from_position, radius, 0, FULL_CIRCLE_DEG);
        fillarc(to_position, radius, 0, FULL_CIRCLE_DEG);
        //find the number of curve points of a segemnt
        unsigned curve_points = singleton->get_Hash_SegmentID_Point_StreetSegmentInfo(segment_id).curvePointCount;
        //return a vector containing the XY coordinate of every curve point
        vector <t_point> curve_in_segment = singleton->get_curvePoints_in_xy_coordinate(segment_id);
        if (curve_points == 0) 
        {
            //if straight line, just draw from start to finish
            drawline(from_position, to_position);
            //fix the ugly  segments
            fillarc(from_position, radius, 0, FULL_CIRCLE_DEG);
            fillarc(to_position, radius, 0, FULL_CIRCLE_DEG);
            //see if we ned to draw the arrow foe oneway street
            if (zoom_level > 6) {
                if (this_segment.oneWay) 
                {
                    t_bound_box arrowBox((from_position.x) - 0.03, (from_position.y) - 0.03, (from_position.x) + 0.03, (from_position.y) + 0.03);
                    setcolor(SEGMENTDARKGREY);
                    double angle = setRoadNameAngle(from_position, to_position);
                    settextrotation(angle);
                    drawtext_in(arrowBox, "->");
                }
            }
        } else {
            t_point current_point = from_position;
            //iterate through every curve straight line that make up of a segment
            for (auto i = curve_in_segment.begin(); i != curve_in_segment.end(); i++) 
            {
                //draw the mini straight line segment
                fillarc(current_point, radius, 0, FULL_CIRCLE_DEG);
                drawline(current_point, *i);
                //draw an arrow ->
                current_point = *i;
                //fix ugly segments
                fillarc(current_point, radius, 0, FULL_CIRCLE_DEG);
            }
            //draw the last segment to the end point
            drawline(current_point, to_position);
            //fix ugly segments
            fillarc(current_point, radius, 0, FULL_CIRCLE_DEG);
            fillarc(to_position, radius, 0, FULL_CIRCLE_DEG);
        }
        //draw the last segment to the end point
        //draw the last segment to the end point
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

void draw_hospital(vector<unsigned> POI_ids) {
    for (auto iter = POI_ids.begin(); iter != POI_ids.end(); iter++) {
        t_point POI_position = singleton -> get_PointOfInterest_in_xy_coordinate(*iter);
        setcolor(RED);
        fillarc(POI_position, RAD_POI, 0, FULL_CIRCLE_DEG);
        setcolor(WHITE);
        t_bound_box textBox((POI_position.x) - ZERO_1_NAME_BOX, (POI_position.y) - ZERO_1_NAME_BOX,
                (POI_position.x) + ZERO_1_NAME_BOX, (POI_position.y) + ZERO_1_NAME_BOX);
        settextrotation(0);
        setfontsize(font_size[zoom_level]);
        drawtext_in(textBox, "+");
        draw_POI_name(POI_position, (*iter));

    }

}

//use this funciton to label type finanical, like bank, atm ,etc
//we use symbol "$"

void draw_financials(vector<unsigned> POI_ids) {
    for (auto iter = POI_ids.begin(); iter != POI_ids.end(); iter++) {
        t_point POI_position = singleton -> get_PointOfInterest_in_xy_coordinate(*iter);
        setcolor(DOLLARGREEN);
        fillarc(POI_position, RAD_POI, 0, FULL_CIRCLE_DEG);
        setcolor(DOLLARGOLD);
        t_bound_box textBox((POI_position.x) - ZERO_1_NAME_BOX, (POI_position.y) - ZERO_1_NAME_BOX,
                (POI_position.x) + ZERO_1_NAME_BOX, (POI_position.y) + ZERO_1_NAME_BOX);
        settextrotation(0);
        setfontsize(font_size[zoom_level]);
        drawtext_in(textBox, "$");
        draw_POI_name(POI_position, (*iter));
    }

}

//use this function to label type schools, including kindergarten to uni
//we use the symbol "A+" to give them some hope

void draw_schools(vector<unsigned> POI_ids) {
    for (auto iter = POI_ids.begin(); iter != POI_ids.end(); iter++) {
        t_point POI_position = singleton -> get_PointOfInterest_in_xy_coordinate(*iter);
        setcolor(LAKECOLOR);
        fillarc(POI_position, RAD_POI, 0, FULL_CIRCLE_DEG);
        setcolor(RED);
        t_bound_box textBox((POI_position.x) - ZERO_1_NAME_BOX, (POI_position.y) - ZERO_1_NAME_BOX,
                (POI_position.x) + ZERO_1_NAME_BOX, (POI_position.y) + ZERO_1_NAME_BOX);
        settextrotation(0);
        setfontsize(font_size[zoom_level]);
        drawtext_in(textBox, "A+");
        draw_POI_name(POI_position, (*iter));
    }

}

//yay food! we use this function to label all resturants, food courts, cafe, anywhere  you can eat

void draw_food(vector<unsigned> POI_ids) {
    for (auto iter = POI_ids.begin(); iter != POI_ids.end(); iter++) {
        t_point POI_position = singleton -> get_PointOfInterest_in_xy_coordinate(*iter);
        setcolor(FOODYELLOW);
        fillarc(POI_position, RAD_POI, 0, FULL_CIRCLE_DEG);
        setcolor(WHITE);
        t_bound_box textBox((POI_position.x) - LVL8, (POI_position.y) - LVL8, (POI_position.x) + LVL8, (POI_position.y) + LVL8);
        settextrotation(0);
        setfontsize(font_size[zoom_level]);
        drawtext_in(textBox, "^_^");
        draw_POI_name(POI_position, (*iter));
    }
}

void draw_hotel(vector<unsigned> POI_ids) {
    for (auto iter = POI_ids.begin(); iter != POI_ids.end(); iter++) {
        t_point POI_position = singleton -> get_PointOfInterest_in_xy_coordinate(*iter);
        setcolor(HOTELBLUE);
        fillarc(POI_position, RAD_POI, 0, FULL_CIRCLE_DEG);
        setcolor(WHITE);
        t_bound_box textBox((POI_position.x) - LVL8, (POI_position.y) - LVL8, (POI_position.x) + LVL8, (POI_position.y) + LVL8);
        settextrotation(0);
        setfontsize(font_size[zoom_level]);
        drawtext_in(textBox, "♋");
        draw_POI_name(POI_position, (*iter));
    }
}
//draws a circle for general points of interest

void draw_point_of_interest(unsigned POI_id, float rad, color_types select1, color_types select2) {
    t_point POI_position = singleton -> get_PointOfInterest_in_xy_coordinate(POI_id);
    setcolor(select2);
    fillarc(POI_position, rad, 0, FULL_CIRCLE_DEG);
    setcolor(select1);
    fillarc(POI_position, rad / 2, 0, FULL_CIRCLE_DEG);
}

//we set zoom levels and will show different types of POI in different zoom levels
//this is used to filter POI by type

void draw_POI_by_type(string type) {
    vector<unsigned> POI_ids = singleton ->get_Hash_POItype_Point_ID(type);
    if (POI_ids.size() > 0) {
        if (type == "hospital")
            draw_hospital(POI_ids);
        else if (type == "bank" || type == "atm" || type == "financial")
            draw_financials(POI_ids);
        else if (type == "college" || type == "language_school" || type == "english_school"
                || type == "music_school" || type == "preschool" || type == "school"
                || type == "university" || type == "kindergarten")
            draw_schools(POI_ids);
        else if (type == "bakery" || type == "cafe" || type == "fast_food" ||
                type == "food_court" || type == "ice_cream" || type == "internet_cafe"
                || type == "restaurant" || type == "sweets")
            draw_food(POI_ids);
        else if (type == "hotel")
            draw_hotel(POI_ids);
        else {
            for (auto iter = POI_ids.begin(); iter != POI_ids.end(); iter++) {
                draw_point_of_interest(*iter, TEXTBOX_04, MEDIUMPURPLE, WHITE);
            }
        }
    }
}


//draw buildings as polygons, only show in high zoom levels
//use this to draw building, lake , beach, unknown, island, lake, river, etc all types

void draw_feature_by_id(unsigned int Feature_id, color_types selected_color) {
    vector<t_point> all_points;
    setcolor(selected_color);
    setlinewidth(2);
    setlinestyle(SOLID);

    all_points = singleton -> get_Feature_Points_In_xycoordinate(Feature_id);
    unsigned int npoint = all_points.size();
    bool isClose = singleton ->check_is_closed(all_points);

    if (isClose) {
        t_point point_array[npoint];
        for (unsigned int i = 0; i < npoint - 1; i++) {
            t_point point1 = all_points[i];
            point_array[i].x = point1.x;
            point_array[i].y = point1.y;
            t_point point2 = all_points[i + 1];
            drawline(point1, point2);
        }
        point_array[npoint - 1].x = all_points[npoint - 1].x;
        point_array[npoint - 1].y = all_points[npoint - 1].y;
        setcolor(selected_color);

        fillpoly(point_array, npoint);
    } else {
        for (unsigned int i = 0; i < npoint - 1; i++) {
            t_point point1 = all_points[i];
            t_point point2 = all_points[i + 1];
            drawline(point1, point2);
        }
    }

}

void draw_feature(int zoomLevel) {
    //we defined all the functions above to FILTER WHAT TO DRAW AND layering by calling different functions
    color_types selected_color;
    for (auto i = singleton ->area_Point_Close_Feature.rbegin(); i != singleton ->area_Point_Close_Feature.rend(); ++i) {
        //auto close_featureID = iter.equal_range(iter->first);
        //for (unsigned i = 0; i < close_featureID.size(); i++) {
        FeatureType type = singleton->get_Hash_FeatureID_Point_Type(i->second);
        if (type == Building) {
            if (zoom_level > 5) {
                selected_color = BUILDINGGREY;
                draw_feature_by_id(i->second, selected_color);
            }
        } else if (type == Park) {
            selected_color = GRASSGREEN;
            draw_feature_by_id(i->second, selected_color);
        } else if (type == Lake) {
            selected_color = LAKECOLOR;
            draw_feature_by_id(i->second, selected_color);
        } else if (type == Beach) {
            selected_color = SANDYELLOW;
            draw_feature_by_id(i->second, selected_color);
        } else if (type == Island) {
            selected_color = KHAKI;
            draw_feature_by_id(i->second, selected_color);
        } else if (type == Shoreline) {
            selected_color = CORAL;
            draw_feature_by_id(i->second, selected_color);
        } else if (type == Unknown) {
            selected_color = BISQUE;
            draw_feature_by_id(i->second, selected_color);
        } else if (type == Greenspace) {
            selected_color = GRASSGREEN;
            draw_feature_by_id(i->second, selected_color);
        } else if (type == Golfcourse) {
            selected_color = GOLFGREEN;
            draw_feature_by_id(i->second, selected_color);
        }
        if (type == River) {
            selected_color = LAKECOLOR;
            draw_feature_by_id(i->second, selected_color);
            continue;
        }
    }

    //then draw open features
    vector<unsigned> open_featureIDs = singleton ->get_open_features();
    for (unsigned j = 0; j < open_featureIDs.size(); j++) {
        FeatureType type = singleton->get_Hash_FeatureID_Point_Type(j);
        if (type == Building) {
            if (zoom_level > 5) {
                selected_color = BUILDINGGREY;
                draw_feature_by_id(j, selected_color);
            }
        } else if (type == Park) {
            selected_color = GRASSGREEN;
            draw_feature_by_id(j, selected_color);
        } else if (type == Lake) {
            selected_color = LAKECOLOR;
            draw_feature_by_id(j, selected_color);
        } else if (type == Beach) {
            selected_color = SANDYELLOW;
            draw_feature_by_id(j, selected_color);

        } else if (type == Island) {
            selected_color = KHAKI;
            draw_feature_by_id(j, selected_color);
        } else if (type == Shoreline) {
            selected_color = CORAL;
            draw_feature_by_id(j, selected_color);
        } else if (type == Unknown) {
            selected_color = BUILDINGGREY;
            draw_feature_by_id(j, selected_color);
        } else if (type == Greenspace) {
            selected_color = GRASSGREEN;
            draw_feature_by_id(j, selected_color);
        } else if (type == Golfcourse) {
            selected_color = GOLFGREEN;
            draw_feature_by_id(j, selected_color);
        }
        if (type == River) {
            selected_color = LAKECOLOR;
            draw_feature_by_id(j, selected_color);
            continue;
        }
    }


}

//function to draw streets by name

void draw_street(vector<unsigned> street_ids) {//set visible world, add street name, oneway, list intersection names in terminal
    //label intersection names, use another color for the circles))
    //this street exists, then draw this street
    for (auto iter = street_ids.begin(); iter != street_ids.end(); iter++) {
        vector<unsigned> segments;
        segments = find_street_street_segments(*iter);
        setlinestyle(SOLID);
        vector<unsigned> segID = singleton->get_Hash_IntersectionID_Point_StreetSegmentID(*iter);

        //draw segments on this street and connect them
        for (auto s_iter = segments.begin(); s_iter != segments.end(); s_iter++) {
            setcolor(RED);
            draw_segment(*s_iter, 0);


        }
        //print th estreet info in terminal
        //too much, can't do it gg

    }
}

void highlight_intersections(vector<unsigned> intersection_id) {//add intersetion info, latlon, name, streets names, used to find
    setcolor(RED);
    for (auto iter = intersection_id.begin(); iter != intersection_id.end(); iter++) {
        t_point position;
        position = singleton ->get_intersectionPositionIn_xy_coordinate(*iter);
        fillarc(position, FIX_UGLY_ROAD, 0, FULL_CIRCLE_DEG);
        t_bound_box textBox((position.x) - HIGHLIGHT_INTERSECTION_BOX, (position.y) - HIGHLIGHT_INTERSECTION_BOX,
                (position.x) + HIGHLIGHT_INTERSECTION_BOX, (position.y) + HIGHLIGHT_INTERSECTION_BOX);
        setcolor(BLACK);
        settextrotation(0);
        string sentence;
        LatLon thisInter_Position;
        thisInter_Position = singleton->get_IntersectionID_To_Position((*iter));

        string intersection_name = getIntersectionName(*iter);
        sentence = intersection_name + " ";

        drawtext_in(textBox, intersection_name);

        if (show_info == true)
            cout << "Intersection at " << intersection_name << thisInter_Position << endl;
    }

}


//use this to highlight feaures entered by user

void highlight_feature(vector<unsigned> feature_id) { //set visble, print name, type
    t_point midPoint;
    for (auto iter = feature_id.begin(); iter != feature_id.end(); iter++) {
        vector<t_point> all_points;
        setlinewidth(2.5);
        setlinestyle(DASHED);
        setcolor(RED);
        all_points = singleton -> get_Feature_Points_In_xycoordinate(*iter);

        unsigned int numOfPoint = all_points.size();

        for (unsigned int i = 0; i < numOfPoint - 1; i++) {
            t_point point1 = all_points[i];
            t_point point2 = all_points[i + 1];
            drawline(point1, point2);

        }
    }

}
//used for the find POI button, will higlihgt and print info fo the selected one in the visible world

void highlightPOI(vector<unsigned> POI_id) {//find poi 
    string POI_name = singleton ->get_hash_POIID_Point_POIname(POI_id[0]);
    for (auto iter = POI_id.begin(); iter != POI_id.end(); iter++) {
        t_point position = singleton ->get_PointOfInterest_in_xy_coordinate(*iter);
        setcolor(PLUM);
        fillarc(position, NEARBY_POI, 0, FULL_CIRCLE_DEG);
        setcolor(KHAKI);
        fillrect(position.x - HIGHLIGHT_POI_BOX_3, position.y - HIGHLIGHT_POI_BOX_3,
                position.x + HIGHLIGHT_POI_BOX_3, position.y + HIGHLIGHT_POI_BOX_3);

        double textWidth = TEXTBOX_04 * 5;
        double testHeight = TEXTBOX_04 * 1.5;
        t_bound_box textBox(t_point((position.x) - textWidth / 2, (position.y) + RAD_POI), textWidth, testHeight);

        setcolor(PLUM);
        fillrect(textBox);

        setcolor(RED);
        setlinewidth(FEATURE_BORDER_WIDTH);
        drawrect(textBox);

        settextrotation(0);
        setcolor(BLACK);
        setfontsize(font_size[zoom_level]);

        drawtext(textBox.get_center(), POI_name, textBox.get_width(), textBox.get_height());
    }

}


// will higlihgt and print info fo the selected one in the visible world
//use information box to display all information

void draw_click_POI(t_point position) {//click. add text , ino box 
    bool found = false;
    unsigned POI_id;
    for (unsigned i = 0; i < singleton->get_POI_numbers() && !found; i++) {
        t_point this_position = singleton ->get_PointOfInterest_in_xy_coordinate(i);
        if (this_position.x == position.x && this_position.y == position.y) {
            POI_id = i;
            found = true;
        }
    }
    string POI_name = singleton ->get_hash_POIID_Point_POIname(POI_id);
    setcolor(RED);
    fillarc(position, RAD_POI, 0, FULL_CIRCLE_DEG);
    setcolor(KHAKI);
    fillrect(position.x - HIGHLIGHT_POI_BOX_3, position.y - HIGHLIGHT_POI_BOX_3,
            position.x + HIGHLIGHT_POI_BOX_3, position.y + HIGHLIGHT_POI_BOX_3);

    double textWidth = POI_text_width;
    double testHeight = POI_testHeight;
    t_bound_box textBox(t_point((position.x) - textWidth / 2, (position.y) + RAD_POI), textWidth, testHeight);

    setcolor(LIGHTPINK);
    fillrect(textBox);

    setcolor(BLUE);
    setlinewidth(FEATURE_BORDER_WIDTH);
    drawrect(textBox);

    settextrotation(0);
    setcolor(RED);
    setfontsize(font_size[zoom_level]);



    string POI_Type = singleton->get_Hash_POIID_Point_POItype(POI_id);
    string wholeInformation = POI_name + " Type: " + POI_Type;
    drawtext(textBox.get_center(), wholeInformation, textBox.get_width(), textBox.get_height());
    //cout<<"POI type"<<POI_Type<<endl;
}


//used to click and higlihght, print name of street, used to highlight intersecdtion on the street

void highlightPath(t_point position) {//ysed to click and higlihght, add name, used to hilight intersecdtion on the street

    LatLon mousePosition;
    mousePosition.lat = position.y;
    mousePosition.lon = position.x / (singleton->get_cosAvg());
    unsigned closeIntersection;
    bool found = false;
    for (unsigned i = 0; i < singleton ->get_intersection_number() && !found; i++) {
        t_point this_position = singleton ->get_intersectionPositionIn_xy_coordinate(i);
        if (this_position.x == position.x && this_position.y == position.y) {
            closeIntersection = i;
            found = true;
        }
    }


    //draw fill the road with color
    //choose Yellow
    vector<unsigned> highlightSegments;
    highlightSegments = singleton->get_Hash_IntersectionID_Point_StreetSegmentID(closeIntersection);
    setcolor(PATHORANGE);

    for (unsigned i = 0; i < highlightSegments.size(); i++) {
        setcolor(RED);

        StreetSegmentInfo this_segment;
        this_segment = singleton->get_Hash_SegmentID_Point_StreetSegmentInfo(highlightSegments[i]);

        //get the ids of two intersections that this segment connects
        unsigned int intersection1, intersection2;
        intersection1 = this_segment.from;
        intersection2 = this_segment.to;

        //get the position of two intersections in xy_coordinate, using id as key
        t_point from_position, to_position;
        from_position = singleton->get_intersectionPositionIn_xy_coordinate(intersection1);
        to_position = singleton->get_intersectionPositionIn_xy_coordinate(intersection2);

        //check if there are curve points exist
        vector<t_point> curve_in_segment;
        curve_in_segment = singleton->get_curvePoints_in_xy_coordinate(highlightSegments[i]);

        setlinestyle(SOLID);
        if (curve_in_segment.size() == 0) {//this segment is a straight line, draw directly
            setcolor(RED);
            drawline(from_position, to_position);
            fillarc(from_position, zoom_level*FIX_UGLY_ROAD, 0, FULL_CIRCLE_DEG);
            fillarc(to_position, zoom_level*FIX_UGLY_ROAD, 0, FULL_CIRCLE_DEG);
        } else {
            //has curve points, find these curve points
            //draw lines by connecting these points and two intersection points
            setcolor(RED);
            t_point current_point = from_position;
            for (auto iter = curve_in_segment.begin(); iter != curve_in_segment.end(); iter++) {
                drawline(current_point, *iter);
                fillarc(from_position, zoom_level*FIX_UGLY_ROAD, 0, FULL_CIRCLE_DEG);
                fillarc(to_position, zoom_level*FIX_UGLY_ROAD, 0, FULL_CIRCLE_DEG);
                current_point = *iter;
            }
            setcolor(RED);
            drawline(current_point, to_position);
            fillarc(from_position, zoom_level*FIX_UGLY_ROAD, 0, FULL_CIRCLE_DEG);
            fillarc(to_position, zoom_level*FIX_UGLY_ROAD, 0, FULL_CIRCLE_DEG);
        }

    }

    //print the name of the intersection
    fillarc(position, FIX_UGLY_ROAD, 0, FULL_CIRCLE_DEG);
    t_bound_box textBox((position.x) - 0.05, (position.y) - 0.05, (position.x) + 0.05, (position.y) + 0.05);
    setcolor(BLACK);
    settextrotation(0);
    string intersection_name = getIntersectionName(closeIntersection);
    drawtext_in(textBox, intersection_name);


}


void draw_arrow_function(long unsigned int ID, double offset, double multi)
{
    t_point mid_coord;
    unsigned long from = singleton->get_Hash_SegmentID_Point_StreetSegmentInfo(ID).from;
    unsigned long to = singleton->get_Hash_SegmentID_Point_StreetSegmentInfo(ID).to;
    
    t_point start = singleton->get_intersectionPositionIn_xy_coordinate(from);
    t_point end = singleton->get_intersectionPositionIn_xy_coordinate(to);
    
    mid_coord.x = (start.x + end.x) / 2;
    mid_coord.y = (start.y + end.y) / 2;
    
    t_point polygon[3];
    
    polygon[0] = mid_coord;
    
            
    return;
}

void drawdestinations(double offset, double multi, double font, double radius, vector<unsigned>anyPath)
{
    if (anyPath.empty()) 
    {
        return;
    } 
    else 
    {
        for(auto iter = anyPath.begin();iter!=anyPath.end();iter++)
        {
            setcolor(DOLLARGOLD);
            draw_segment(*iter, 0.000050);
            
        }

        t_point drawstart = singleton->get_intersectionPositionIn_xy_coordinate(starting_intersection);
        t_point polygon[5];
        t_point drawend = singleton->get_intersectionPositionIn_xy_coordinate(ending_intersection);
        t_point polygon2[5];

        polygon[0] = drawstart;

        polygon[1].x = drawstart.x + offset*multi;
        polygon[1].y = drawstart.y + offset * 2 * multi;

        polygon[2].x = drawstart.x;
        polygon[2].y = drawstart.y + offset * 4 * multi;

        polygon[3].x = drawstart.x - offset*multi;
        polygon[3].y = drawstart.y + offset * 2 * multi;

        polygon[4] = drawstart;

        t_point center;
        center.y = drawstart.y + offset * 2 * multi;
        center.x = drawstart.x;

        t_point center2;
        center2.y = drawend.y + offset * 2 * multi;
        center2.x = drawend.x;
        setcolor(YELLOW);
        fillarc(drawstart, radius, 0, 360);
        setcolor(BLUE);
        fillpoly(polygon, 5);

        //this is all we need to draw
        polygon2[0] = drawend;

        polygon2[1].x = drawend.x + offset*multi;
        polygon2[1].y = drawend.y + offset * 2 * multi;

        polygon2[2].x = drawend.x;
        polygon2[2].y = drawend.y + offset * 4 * multi;

        polygon2[3].x = drawend.x - offset*multi;
        polygon2[3].y = drawend.y + offset * 2 * multi;

        polygon2[4] = drawend;
        setcolor(YELLOW);
        fillarc(drawend, radius, 0, 360);
        setcolor(BLACK);
        fillpoly(polygon2, 5);

        
        if(zoom_level>6)
        {
            setcolor(WHITE);
            settextrotation(0);
            setfontsize(font);
            long double x = 10000000000000;
            drawtext(center, "Start", x, x);
            
            setcolor(WHITE);
            settextrotation(0);
            setfontsize(font);
            drawtext(center2, "End", x, x);
        }
    }
    return;
}

void act_on_command(void (*drawscreen_ptr) (void)) 
{
    string answer;
    int count = 0;
    do 
    {
        count += 1;
        if (count == 1) 
        {
            cout << "Welcome! Please type Start to initiate:" << endl;
            cout << ">>";
        } 
        else 
        {
            cout << "Exiting and Please Refer Back to the map" << endl;
            return;
        }
        cin >>answer;
    } while (answer != "start");
    tab_auto_completion();
    if (both_intersections == true)
    {
        path = find_path_between_intersections(starting_intersection, ending_intersection);
        cout<<endl;
        cout<<"Directions: "<<endl;
        t_point current;
        t_point next;
        t_point next_next;
        long double angle;
        if (path.empty())
        {
            cout << "No Destination" << endl;
            drawscreen();
            return;
        } 
        else if (path.size() == 1)
        {
            scale_fit = true;
            cout << "Follow " << getStreetName(getStreetSegmentInfo(path[0]).streetID) << " to destination." << endl;
            drawscreen();
            return;
        } 
        else 
        {
            scale_fit = true;
            for (unsigned int i = 0; i < path.size() - 1; i++) 
            {
                current = singleton->get_intersectionPositionIn_xy_coordinate(getStreetSegmentInfo(path[i]).from);
                next = singleton->get_intersectionPositionIn_xy_coordinate(getStreetSegmentInfo(path[i]).to);
                next_next = singleton->get_intersectionPositionIn_xy_coordinate(getStreetSegmentInfo(path[i + 1]).to);
                float a[3];
                float b[3];
                float *c = new float[3];
                a[0]=next.x-current.x;
                a[1]=next.y-current.y;
                a[2]=0;
                b[0]=next_next.x-next.x;
                b[1]=next_next.y-next.y;
                b[2]=0;
                c=CrossProduct(a,b);
                
                if (getStreetName(getStreetSegmentInfo(path[i]).streetID) != getStreetName(getStreetSegmentInfo(path[i + 1]).streetID))
                {
                    if(c[2]<0)
                    {
                        cout<<"Turn Right at "<<getStreetName(getStreetSegmentInfo(path[i + 1]).streetID)<<endl;
                    }
                    else          
                    {
                        cout<<"Turn Left at "<<getStreetName(getStreetSegmentInfo(path[i + 1]).streetID)<<endl;
                    }
                } 
                delete[]c;
            }
            drawscreen();
            return;
        }
    } 
    else 
    {
        path = find_path_to_point_of_interest(starting_intersection, end_poi);
        t_point current;
        t_point next;
        t_point next_next;
        cout<<endl;
        cout<<"Directions: "<<endl;
        if (path.empty()) 
        {
            cout << "No Destination" << endl;
            drawscreen();
            return;
        } 
        else if (path.size() == 1) 
        {
            scale_fit = true;
            cout << "Follow " << getStreetName(getStreetSegmentInfo(path[0]).streetID) << " to destination." << endl;
            drawscreen();
            return;
        } 
        else
        {
            scale_fit = true;
            for (unsigned int i = 0; i < path.size() - 1; i++) 
            {
                current = singleton->get_intersectionPositionIn_xy_coordinate(getStreetSegmentInfo(path[i]).from);
                next = singleton->get_intersectionPositionIn_xy_coordinate(getStreetSegmentInfo(path[i]).to);
                next_next = singleton->get_intersectionPositionIn_xy_coordinate(getStreetSegmentInfo(path[i + 1]).to);
                float a[3];
                float b[3];
                float *c = new float[3];
                a[0]=next.x-current.x;
                a[1]=next.y-current.y;
                a[2]=0;
                b[0]=next_next.x-next.x;
                b[1]=next_next.y-next.y;
                b[2]=0;
                c=CrossProduct(a,b);
                if (getStreetName(getStreetSegmentInfo(path[i]).streetID) != getStreetName(getStreetSegmentInfo(path[i + 1]).streetID))
                {
                    if(c[2]<0)
                    {
                        cout<<"Turn Right at "<<getStreetName(getStreetSegmentInfo(path[i + 1]).streetID)<<endl;
                    }
                    else          
                    {
                        cout<<"Turn Left at "<<getStreetName(getStreetSegmentInfo(path[i + 1]).streetID)<<endl;
                    }
                } 
                delete[]c;
            }
            drawscreen();
            return;
        }
    }
}

void highlight_find_path_m3(vector<unsigned> intersection_id) {
    segfault++;
    cout<<"redraw "<<segfault<<endl;
    //this is where the seg fault occurred
    if (!intersection_id.empty()) {
        for (auto iter = intersection_id.begin(); iter != (intersection_id.end()); iter++) //end - 1
        {   
            setcolor(ORANGE);
            draw_segment(*iter, 0.0005);
            t_point from_position = singleton->get_intersectionPositionIn_xy_coordinate(*iter);
            t_point to_position = singleton->get_intersectionPositionIn_xy_coordinate(*(iter + 1));
            if(from_position.x == 0 || from_position.y == 0 || to_position.x == 0 || to_position.y ==0)
                return;
            t_bound_box arrowBox((from_position.x) - 0.05, (from_position.y) - 0.05, (from_position.x) + 0.05, (from_position.y) + 0.05);
            setcolor(SEGMENTDARKGREY);
            double angle = setRoadNameAngle(from_position, to_position);
            settextrotation(angle);
            //drawtext_in(arrowBox, "->");
        }
        if (intersection_id.size() > 2) {
            StreetSegmentInfo start_info = singleton -> get_Hash_SegmentID_Point_StreetSegmentInfo(intersection_id[0]);
            StreetSegmentInfo second_info = singleton -> get_Hash_SegmentID_Point_StreetSegmentInfo(intersection_id[1]);
            unsigned start = start_info.from;
            if (start == second_info.from || start == second_info.to)
                start = start_info.to;

            unsigned size = intersection_id.size();
            StreetSegmentInfo end_info = singleton ->get_Hash_SegmentID_Point_StreetSegmentInfo(intersection_id[size - 1]);
            StreetSegmentInfo second_last_info = singleton ->get_Hash_SegmentID_Point_StreetSegmentInfo(intersection_id[size - 2]);
            unsigned end = end_info.from;
            if (end == second_last_info.from || end == second_last_info.to)
                end = end_info.to;
            vector<unsigned> start_point(1, start);
            vector<unsigned> end_point(1, end);

            setcolor(RED);
            highlight_intersections(start_point);
            highlight_intersections(end_point);
        }
    }
    else{
        cout<<"empty paths vec"<<endl;
    }
}

vector <long double> findCoordinates(void) {
    vector <long double> map_coordinates;
    double maxLon = 0;
    double minLon = 0;
    double maxLat = 0;
    double minLat = 0;
    LatLon checkingCoordinates;

    // For every intersection, check if its lat/lon points are the current max or min
    // If they are neither, simply iterate to the next intersection
    for (unsigned int i = 0; i < getNumberOfIntersections(); i++) {
        checkingCoordinates = getIntersectionPosition(i);

        // initializes the first set of coordinates to the first point
        if (i == 0) {
            maxLon = checkingCoordinates.lon;
            minLon = checkingCoordinates.lon;
            maxLat = checkingCoordinates.lat;
            minLat = checkingCoordinates.lat;
        }

        if (checkingCoordinates.lon > maxLon) {
            maxLon = checkingCoordinates.lon;
        } else if (checkingCoordinates.lon < minLon) {
            minLon = checkingCoordinates.lon;
        }

        if (checkingCoordinates.lat > maxLat) {
            maxLat = checkingCoordinates.lat;
        } else if (checkingCoordinates.lat < minLat) {
            minLat = checkingCoordinates.lat;
        }
    }
    for (unsigned int i = 0; i < (singleton->get_feature_numbers()); i++) {
        for (unsigned int j = 0; j < getFeaturePointCount(i); j++) {
            checkingCoordinates = getFeaturePoint(i, j);
            if (checkingCoordinates.lon > maxLon) {
                maxLon = checkingCoordinates.lon;
            }
            if (checkingCoordinates.lon < minLon) {
                minLon = checkingCoordinates.lon;
            }
            if (checkingCoordinates.lat > maxLat) {
                maxLat = checkingCoordinates.lat;
            }
            if (checkingCoordinates.lat < minLat) {
                minLat = checkingCoordinates.lat;
            }
        }
    }
    //Place the variables in a vector in the order they would be called to generate the initial box
    map_coordinates.push_back(minLon);
    map_coordinates.push_back(minLat);
    map_coordinates.push_back(maxLon);
    map_coordinates.push_back(maxLat);
    return map_coordinates;
}

void delay(long milliseconds) 
{
    std::chrono::milliseconds duration(milliseconds);
    std::this_thread::sleep_for(duration);
}

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

void act_on_find_path_m3(void (*drawscreen_ptr) (void)) { //set visible world
    //test this fucking seg fault

    for (auto iter = paths.begin(); iter != paths.end(); iter++)
        cout << singleton->get_hash_intersectionIDtoIntersectionName_str(*iter) << endl;
    //DRAW the path
    findPath_click++;
    if (findPath_click % 2 == 0) {
        if (!click_intersectionFindPath.empty())
            click_intersectionFindPath.clear();

        find_Path = false;
        if (!paths.empty())
            paths.clear();
        change_button_text("Path On", "Path Off");
    } else {
        find_Path = true;
        findClick = 0;

        change_button_text("Path Off", "Path On");
    }

    drawscreen();

}

void act_on_clear(void (*drawscreen_ptr) (void)) {
    keep_highlight = false;
    find_Path = false;
    drawscreen();
}

//for serach dstreet function and its button

void act_on_search_street(void (*drawscreen_ptr) (void)) {//must set visible worldc 
    cout << "Enter the name of street you are looking for: ";
    string street_name = test;
    //getline(cin, street_name);
    vector<unsigned> street_ids;
    street_ids = find_street_ids_from_name(street_name);

    if (street_ids.size() > 0)
        searched_street.push_back(street_ids);
    else
        cout << street_name << " not found" << endl;
    drawscreen();
}

//for search intersection and its button

void act_on_find_intersection(void (*drawscreen_ptr) (void)) { //set visible world
    //add bool clicked
    string street_name1, street_name2;
    cout << "Start to find intersection of two streets." << endl;
    cout << "Enter the name of first street: ";
    getline(cin, street_name1);

    bool valid1 = check_street_valid(street_name1);
    while (!valid1) {
        cout << "Street name " << street_name1 << " is not valid." << endl;
        cout << "Enter the name of first street: ";
        getline(cin, street_name1);
    }

    cout << "Enter the name of second street: ";
    getline(cin, street_name2);
    bool valid2 = check_street_valid(street_name1);
    while (!valid2) {
        cout << "Street name " << street_name2 << " is not valid." << endl;
        cout << "Enter the name of second street: ";
        getline(cin, street_name2);
    }

    vector<unsigned> intersection_ids;
    intersection_ids = find_intersection_ids_from_street_names(street_name1, street_name2);
    if (intersection_ids.size() == 0)
        cout << "Street " << street_name1 << " and Street " << street_name2
            << " have no intersection." << endl;
    else {
        searched_intersection.push_back(intersection_ids);
        show_info = true;
    }
    drawscreen();
}

//for turning on/o0ff subway and bus stations

void act_on_station(void (*drawscreen_ptr) (void)) {
    station_click++;
    if (station_click % 2 == 0) {
        transport_on = false;
        change_button_text("Transport On", "Transport Off");
    } else {
        transport_on = true;
        change_button_text("Transport Off", "Transport On");
        cout << "Print all transportations." << endl;
    }
    drawscreen();
}

//for highlight feature and find feature

void act_on_feature(void (*drawscreen_ptr) (void)) { //to do: set visible world
    cout << "Enter the name of feature you are looking for: ";
    string feature_name;
    getline(cin, feature_name);
    //resetVisibleWorld(resetMid,offset_newWorld,resetWorld_Feature); 
    vector<unsigned> feature_ids = singleton ->get_Hash_FeatureName_Point_ID(feature_name);
    if (feature_ids.size() == 0)
        cout << feature_name << " not found." << endl;
    else {
        searched_feature.push_back(feature_ids);
    }

    drawscreen();
}


void act_on_POI(void (*drawscreen_ptr) (void)) {
    cout << "Enter the name of point of interest you are looking for: ";
    string POI_name;
    getline(cin, POI_name);

    vector<unsigned> POI_ids = singleton ->get_Hash_POIname_Point_POIID(POI_name);
    if (POI_ids.size() == 0)
        cout << POI_name << " not found" << endl;
    else
        searched_POI.push_back(POI_ids);

    drawscreen();

}

void printPath() {

    if (find_Path) {

        if (!paths.empty()) {
            cout << "not empty" << endl;
            for (auto iter = paths.begin(); iter != paths.end(); iter++)
                cout << singleton->get_hash_intersectionIDtoIntersectionName_str(*iter);
        }
    }
}

void act_on_switch_map(void (*drawscreen_ptr) (void)) {
    if (click_intersection.size() > 0) {
        click_intersection.clear();
    }
    if (click_POI.size() > 0) {
        click_POI.clear();
    }
    if (searched_street.size() > 0) {
        for (auto iter = searched_street.begin(); iter != searched_street.end(); iter++)
            (*iter).clear();
        searched_street.clear();
    }
    if (searched_intersection.size() > 0) {
        for (auto iter = searched_intersection.begin(); iter != searched_intersection.end(); iter++)
            (*iter).clear();
        searched_intersection.clear();
    }
    if (searched_feature.size() > 0) {
        for (auto iter = searched_feature.begin(); iter != searched_feature.end(); iter++)
            (*iter).clear();
        searched_feature.clear();
    }
    if (searched_POI.size() > 0) {
        for (auto iter = searched_POI.begin(); iter != searched_POI.end(); iter++)
            (*iter).clear();
        searched_POI.clear();
    }
    if (!paths.empty())
        paths.clear();

    keep_highlight = false;
    transport_on = false;
    station_click = 0;
    zoom_level = 0;
    findPath_click = 0;
    findClick = 0;
    find_Path = 0;
    cout << "Switch to another city." << endl;
    singleton->ResetInstance();
    singleton = NULL;
    closeOSMDatabase();
    closeStreetDatabase();
    cout << "Enter city: ";
    getline(cin, city);
    string map_name = "/cad2/ece297s/public/maps/" + city + ".streets.bin";
    load_map(map_name);
    vector<long double> coordinates = findCoordinates();
    set_visible_world(coordinates[0] * singleton->get_cosAvg(), coordinates[1], coordinates[2] * singleton->get_cosAvg(), coordinates[3]);
    update_message("Interactive graphics example.");
    set_keypress_input(true);
    set_mouse_move_input(true);
    drawscreen();
}

float* CrossProduct(float *a, float *b)
{
	float* c = new float[3];

        //Cross product formula 
	c[0] = (a[1] * b[2]) - (a[2] * b[1]);
	c[1] = (a[2] * b[0]) - (a[0] * b[2]);
	c[2] = (a[0] * b[1]) - (a[1] * b[0]);

	return c;
}


void act_on_button_press(float x, float y, t_event_buttonPressed event) {
    if (find_Path && (findClick == 0 || findClick == 1)) {
        findClick++;
        t_point MouseClick;
        MouseClick.x = x;
        MouseClick.y = y;

        LatLon mousePosition;
        mousePosition.lat = y;
        mousePosition.lon = x / (singleton->get_cosAvg());
        unsigned closeIntersection = find_closest_intersection(mousePosition);
        t_point intersection_in_xy = singleton ->get_intersectionPositionIn_xy_coordinate(closeIntersection);
        double distance = sqrt((x - intersection_in_xy.x)*(x - intersection_in_xy.x)
                +(y - intersection_in_xy.y)*(y - intersection_in_xy.y));
        if (distance <= NEARBY_INTERSECTION) {//check if this intersection already be highlight
            auto iter = std::find(click_intersectionFindPath.begin(), click_intersectionFindPath.end(), intersection_in_xy);
            if (iter != click_intersectionFindPath.end()) {//if yes, clear the highlight
                click_intersectionFindPath.erase(std::remove(click_intersectionFindPath.begin(), click_intersectionFindPath.end(), intersection_in_xy),
                     click_intersectionFindPath.end());
                findClick--;
            } else
                click_intersectionFindPath.push_back(intersection_in_xy);
        }
    } else {
        t_point MouseClick;
        MouseClick.x = x;
        MouseClick.y = y;

        LatLon mousePosition;
        mousePosition.lat = y;
        mousePosition.lon = x / (singleton->get_cosAvg());

        unsigned closeIntersection = find_closest_intersection(mousePosition);
        //check if it click at an intersection
        t_point intersection_in_xy = singleton ->get_intersectionPositionIn_xy_coordinate(closeIntersection);
        double distance = sqrt((x - intersection_in_xy.x)*(x - intersection_in_xy.x)
                +(y - intersection_in_xy.y)*(y - intersection_in_xy.y));
        if (distance <= NEARBY_INTERSECTION) {//check if this intersection already be highlight
            auto iter = std::find(click_intersection.begin(), click_intersection.end(), intersection_in_xy);
            if (iter != click_intersection.end())//if yes, clear the highlight
                click_intersection.erase(std::remove(click_intersection.begin(), click_intersection.end(), intersection_in_xy),
                    click_intersection.end());
            else
                click_intersection.push_back(intersection_in_xy);
        } else {//check if click at a point of interest
            unsigned closePOI = find_closest_point_of_interest(mousePosition);
            t_point POI_in_xy = singleton ->get_PointOfInterest_in_xy_coordinate(closePOI);

            double distance = sqrt((x - POI_in_xy.x)*(x - POI_in_xy.x)
                    +(y - POI_in_xy.y)*(y - POI_in_xy.y));
            if (distance <= NEARBY_POI) {//if click on a POI
                auto iter = std::find(click_POI.begin(), click_POI.end(), POI_in_xy);
                if (iter != click_POI.end())//if yes, clear the highlight
                    click_POI.erase(std::remove(click_POI.begin(), click_POI.end(), POI_in_xy),
                        click_POI.end());
                else
                    click_POI.push_back(POI_in_xy);
            } else
                click_intersection.push_back(intersection_in_xy);
        }
    }
    drawscreen();
}

void act_on_help(void (*drawscreen_ptr) (void))
{
    if (to_help == false) 
    {
        to_help = true;
        drawscreen();
    } else {
        to_help = false;
        drawscreen();
    }
}
