#include "StreetsDatabaseAPI.h"
#include "m1.h"
#include <vector>
#include <unordered_map>
#include <math.h>
#include <algorithm>
#include "streetGraph.h"
#include <iostream>
#include <string>
using namespace std;
streetGraph *singleton;
string city;



bool load_map(std::string map_name) {
    
    string OSM_name;
    string temp;
    if(map_name[0]!='/')
    OSM_name = "/cad2/ece297s/public/maps/"+city+".osm.bin";
   else{
        temp = map_name;
        auto iter = temp.find("streets.bin");
        temp.erase(iter);
        OSM_name = temp + "osm.bin";
    }
    singleton = streetGraph::getObjectPtr(map_name,OSM_name);
    bool load_success;
    if (singleton != NULL)
        load_success = true;
    else
        load_success = false;
    return load_success;
}


//close the map

void close_map() {
    closeOSMDatabase();
    closeStreetDatabase();
    singleton->ResetInstance();
   
    singleton = NULL;
    // destroy/clear any data structures you created in load_map
    // nothing dynamics we created
}

//function to return street id(s) for a street name
//return a 0-length vector if no street with this name exists.

std::vector<unsigned> find_street_ids_from_name(std::string street_name) {
    return singleton-> get_StreetName_Point_StreetIDs(street_name);
}

//function to return the street segments for a given intersection 

std::vector<unsigned> find_intersection_street_segments(unsigned intersection_id) {
    return singleton -> get_Hash_IntersectionID_Point_StreetSegmentID(intersection_id);

}


//Third-given a intersectionID, find all street names

std::vector<std::string> find_intersection_street_names(unsigned intersection_id) {
    vector <string> streetNames;
    if (intersection_id < singleton->get_intersection_number()) {
        //check for each segmentID, iterate
        auto segments = singleton->get_Hash_IntersectionID_Point_StreetSegmentID(intersection_id);
        for (auto segID = segments.begin(); segID != segments.end(); segID++) {
            StreetSegmentInfo info = getStreetSegmentInfo(*segID);
            streetNames.push_back(getStreetName(info.streetID));
        }
    }
    return streetNames;
}
//find all intersections reachable by traveling down one street segment 
//from given intersection (hint: you can't travel the wrong way on a 1-way street)
//the returned vector should NOT contain duplicate intersections

std::vector<unsigned> find_adjacent_intersections(unsigned intersection_id) {
    //access by intersection ID and return the vector of adjacent intersections
    return singleton->get_IntersectionID_To_Adjacent(intersection_id);
}

//can you get from intersection1 to intersection2 using a single street segment (hint: check for 1-way streets too)
//corner case: an intersection is considered to be connected to itself

bool are_directly_connected(unsigned intersection_id1, unsigned intersection_id2) {

    //not the same ID, check their intersecion vector
    //if id1' intersection vector has id2, connected
    if (intersection_id1 != intersection_id2) {
        vector<unsigned> adjacent_to_this = find_adjacent_intersections(intersection_id1);
        for (auto iter = adjacent_to_this.begin(); iter != adjacent_to_this.end(); iter++) {
            if (*iter == intersection_id2)
                return true;
        }
        return false;
    }//check for this corner case
        //check if the id 2 is in the adjacent list of id1
        //the same id, connected to itself
    else
        return true;


}

//for a given street, return all the street segments

std::vector<unsigned> find_street_street_segments(unsigned street_id) {

    //access the segments vector by street ID and just return
    //O(1)
    return singleton->get_Hash_StreetID_Point_SegmentID(street_id);
}

//for a given street, find all the intersections

std::vector<unsigned> find_all_street_intersections(unsigned street_id) {
    //insert intersections, index by ID
    vector<unsigned> intersections;
    if (street_id < singleton-> get_street_numbers())
        intersections = singleton->get_Hash_StreetID_Point_IntersectionID(street_id);
    return intersections;
}

//function to return all intersection ids for two intersecting streets
//this function will typically return one intersection id between two street names
//but duplicate street names are allowed, so more than 1 intersection id may exist for 2 street names

std::vector<unsigned> find_intersection_ids_from_street_names(std::string street_name1, std::string street_name2) {
    vector<unsigned>filtered_intersection;
    vector<unsigned>street_ID1, street_ID2;
    vector<unsigned>inserted_vector_intersections1, inserted_vector_intersections2;
    //find the streetIDs from the name and store in vector
    street_ID1 = find_street_ids_from_name(street_name1);
    street_ID2 = find_street_ids_from_name(street_name2);

    //store all the intersections for every street ID that is found
    for (auto i = street_ID1.begin(); i != street_ID1.end(); i++) {
        vector<unsigned>intersections1;
        intersections1 = find_all_street_intersections(*i);
        inserted_vector_intersections1.insert(inserted_vector_intersections1.end(), intersections1.begin(), intersections1.end());
    }
    for (auto j = street_ID2.begin(); j != street_ID2.end(); j++) {
        vector<unsigned>intersections2;
        intersections2 = find_all_street_intersections(*j);
        inserted_vector_intersections2.insert(inserted_vector_intersections2.end(), intersections2.begin(), intersections2.end());
    }

    //sort the vectors
    sort(inserted_vector_intersections1.begin(), inserted_vector_intersections1.end());
    sort(inserted_vector_intersections2.begin(), inserted_vector_intersections2.end());

    //use set_intersection to find and keep the duplicates
    set_intersection(inserted_vector_intersections1.begin(), inserted_vector_intersections1.end(),
            inserted_vector_intersections2.begin(), inserted_vector_intersections2.end(),
            back_inserter(filtered_intersection));
    //which is the result intersection vector
    return filtered_intersection;
}

//find distance between two coordinates

double find_distance_between_two_points(LatLon point1, LatLon point2) {

    //convert to radian
    long double lat1 = point1.lat *DEG_TO_RAD;
    long double lat2 = point2.lat *DEG_TO_RAD;
    long double lon1 = point1.lon *DEG_TO_RAD;
    long double lon2 = point2.lon *DEG_TO_RAD;

    long double latAvg = (lat1 + lat2) / 2;
    long double cosAvg; //use my cos to find cos(latAvg);
    //long double distance = 0;

    //use taylor series
    //use the magic numbers here, cuz it's 2 am and I'm starving
    // they are 2!, 4!, 6!, etc etc
   cosAvg = 1 - latAvg * latAvg / 2 + latAvg * latAvg * latAvg * latAvg / 24 - latAvg * latAvg * latAvg * latAvg * latAvg * latAvg / 720 
               + (latAvg * latAvg * latAvg * latAvg * latAvg * latAvg * latAvg * latAvg) / (40320);
    
    long double x1 = lon1*cosAvg;
    long double x2 = lon2*cosAvg;

    //lat2 == y2, lat 1 == y1 in the formula
    long double squareDis = (x2 - x1)*(x2 - x1) + (lat2 - lat1)*(lat2 - lat1);
    long double distance = EARTH_RADIUS_IN_METERS * sqrt(squareDis);


    return (distance);
}

//find the length of a given street segment

double find_street_segment_length(unsigned street_segment_id) 
{
    double length = 0;
    StreetSegmentInfo given_segment = getStreetSegmentInfo(street_segment_id);

    unsigned int in_from, in_to, num_curve;

    in_from = given_segment.from;
    in_to = given_segment.to;
    num_curve = given_segment.curvePointCount;

    LatLon intersection1, intersection2;
    intersection1 = getIntersectionPosition(in_from);
    intersection2 = getIntersectionPosition(in_to);

    //return the distance if it's just a straight
    if (num_curve == 0)
        length = find_distance_between_two_points(intersection1, intersection2);
    else {
        LatLon temp_pt1 = intersection1;
        //for curves, loop through all the loop segments and add up all segments
        for (unsigned int i = 0; i < num_curve; i++) {
            LatLon temp_pt2;
            temp_pt2 = getStreetSegmentCurvePoint(street_segment_id, i);
            double curve_length;
            curve_length = find_distance_between_two_points(temp_pt1, temp_pt2);
            length = length + curve_length;
            temp_pt1 = temp_pt2;
        }
        // add length between the last point and intersection
        length = length + find_distance_between_two_points(temp_pt1, intersection2);
    }

    return length;
}

//find the length of a whole street

double find_street_length(unsigned street_id) {
    double street_length = 0;
    vector<unsigned> segment_id;
    //sum all the length of segments on one street
    segment_id = find_street_street_segments(street_id);
    for (auto iter = segment_id.begin(); iter != segment_id.end(); iter++) {
        street_length = street_length + find_street_segment_length(*iter);
    }
    return street_length;
}

//find the travel time to drive a street segment (time(minutes) = distance(km)/speed_limit(km/hr) * 60

double find_street_segment_travel_time(unsigned street_segment_id) {
    double time;
    double speed = getStreetSegmentInfo(street_segment_id).speedLimit;
    double length = find_street_segment_length(street_segment_id);
    //use the formula given
    //this 100 converts meter to kilo meter
    time = (length / 1000.0) / speed * 60;
    return time;
}

//find the nearest point of interest(POI) to a given position

unsigned find_closest_point_of_interest(LatLon my_position) {
    unsigned int closest_in = 0;
    double min_len;

    LatLon interestTemp;

    //evaluate the first distance of the 0th POI
    interestTemp = singleton->get_PointOfInterest_To_Position(0);
    double distance = find_distance_between_two_points(interestTemp, my_position);
    min_len = distance;
    //loop through Hash table POI, start with element 1
    for (unsigned int i = 1; i < singleton->get_POI_numbers(); i++) {
        //double distance;
        //bool notFound = true;
        //get the position, or lat and lon, of POI
        interestTemp = singleton->get_PointOfInterest_To_Position(i);
        //calculate the distance using the distance function

        distance = find_distance_between_two_points(interestTemp, my_position);
        //  if (i == 0)
        // min_len = distance;
        // else {
        if (distance < min_len) {
            min_len = distance;
            closest_in = i;
        }
        // }
    }
    return closest_in;
}

//find the nearest intersection (by ID) to a given position

unsigned find_closest_intersection(LatLon my_position) {
    unsigned int closest_in = 0;
    double min_len;
    LatLon tempIntersection = singleton->get_IntersectionID_To_Position(0);

    //evaluate the first distance of the 0th intersection
    double distance = find_distance_between_two_points(tempIntersection, my_position);
    min_len = distance;

    //check the intersections in the whole hash table
    for (unsigned int i = 1; i < singleton->get_intersection_number(); i++) {

        //get the lat lon of the interection i
        tempIntersection = singleton->get_IntersectionID_To_Position(i);

        distance = find_distance_between_two_points(tempIntersection, my_position);

        //if (i == 0)
        //  min_len = distance;
        // else {
        //update the minimum distance
        if (distance < min_len) {
            min_len = distance;
            closest_in = i;
        }
        // }
    }
    return closest_in;
}


