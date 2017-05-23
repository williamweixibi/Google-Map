#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <unordered_map>
#include <algorithm>
#include <map>
#include <unordered_set>

#include "m3.h"
#include "m1.h"
#include "StreetsDatabaseAPI.h"
#include "streetGraph.h"



//the shortest path, store segment ids
vector<unsigned> paths;

unsigned find_closest_intersection_by_xy(t_point my_position) 
{
    unsigned int closest_in = 0;
    t_point tempIntersection = singleton->get_intersectionPositionIn_xy_coordinate(0);

    //evaluate the first distance of the 0th intersection
    double min_len = (tempIntersection.x - my_position.x)*(tempIntersection.x - my_position.x)
            +(tempIntersection.y - my_position.y)*(tempIntersection.y - my_position.y);

    //check the intersections in the whole hash table
    for (unsigned int i = 1; i < singleton->get_intersection_number(); i++) {

        //get the lat lon of the interection i
        tempIntersection = singleton->get_intersectionPositionIn_xy_coordinate(i);

        double distance = (tempIntersection.x - my_position.x)*(tempIntersection.x - my_position.x)
                +(tempIntersection.y - my_position.y)*(tempIntersection.y - my_position.y);

        if (distance < min_len) {
            min_len = distance;
            closest_in = i;
        }
    }
    return closest_in;
}

vector<unsigned> reconstruct_path(vector<unsigned> comeFrom, vector<unsigned> previous_segment, unsigned intersect_id_start, unsigned intersect_id_end) {
    //can find a valid path
    unsigned current = intersect_id_end;
    while (current != intersect_id_start) {
        paths.push_back(previous_segment[current]);
        current = comeFrom[current];
    }

    reverse(paths.begin(), paths.end());
    return paths;
}

// Returns a path (route) between the start intersection and the end 
// intersection, if one exists. If no path exists, this routine returns 
// an empty (size == 0) vector. If more than one path exists, the path 
// with the shortest travel time is returned. The path is returned as a vector 
// of street segment ids; traversing these street segments, in the given order,
// would take one from the start to the end intersection.

vector<unsigned> find_path_between_intersections(unsigned intersect_id_start, unsigned intersect_id_end) {
    if (!paths.empty())
        paths.clear();

    unsigned intersection_number = singleton ->get_intersection_number();

    //the previous intersections and most efficient currrent intersection
    vector<unsigned> comeFrom(intersection_number, intersection_number);

    //the previous intersection and the segment it connects
    vector<unsigned> previous_segment(intersection_number, intersection_number);

    //the visited intersection and the total cost time from start point
    vector<double> visited_gScore(intersection_number, intersection_number);

    //the set of intersection that has been checked and already put into paths, close set
    vector<unsigned> record_intersection(intersection_number, intersection_number);

    //the fscore and its corresponding intersection
    map<double, unsigned> checked_fScore;

    visited_gScore[intersect_id_start] = 0;

    t_point final_position = singleton ->get_intersectionPositionIn_xy_coordinate(intersect_id_end);
    t_point initial_position = singleton ->get_intersectionPositionIn_xy_coordinate(intersect_id_start);
    double final_x = final_position.x;
    double final_y = final_position.y;
    double initial_x = initial_position.x;
    double initial_y = initial_position.y;
    double initial_distance = (initial_x - final_x)*(initial_x - final_x)
            +(initial_y - final_y)*(initial_y - final_y);

    checked_fScore[initial_distance] = intersect_id_start;

    while (!checked_fScore.empty()) {
        auto current_front = checked_fScore.begin();
        unsigned current = current_front->second; //find the intersection has lowest total score
        //unsigned current = checked_fScore.begin()->second;
        
        if (current == intersect_id_end)//already reach the end point, find path done
            return reconstruct_path(comeFrom, previous_segment, intersect_id_start, current);

        record_intersection[current] = current;
        //checked_fScore.erase(checked_fScore.begin());
        checked_fScore.erase(current_front);

        //get all adjacent street
        vector<unsigned> connected_segment = singleton ->get_Hash_IntersectionID_Point_StreetSegmentID(current);

        for (auto iter = connected_segment.begin(); iter != connected_segment.end(); iter++) {
            StreetSegmentInfo current_streetID_info = getStreetSegmentInfo(*iter);
            bool valid = true;
            unsigned id = current_streetID_info.from;

            if (id == current)
                id = current_streetID_info.to;
            else if (current_streetID_info.oneWay)//current is not the from point, check if one way
                valid = false;

            if (valid && record_intersection[id] != id) {//check if intersection has been recored into path
                t_point this_position = singleton ->get_intersectionPositionIn_xy_coordinate(id);
                //find distance between this point to end point, this is the hscore
                double this_position_x = this_position.x;
                double this_position_y = this_position.y;
                double distance = (this_position_x - final_x)*(this_position_x - final_x)
                        +(this_position_y - final_y)*(this_position_y - final_y);

                double time = find_street_segment_travel_time(*iter);

                if (current != intersect_id_start) {//check if need to add 15 sec for take turn
                    unsigned previous = previous_segment[current];
                    unsigned previous_id = getStreetSegmentInfo(previous).streetID;
                    unsigned current_id = current_streetID_info.streetID;

                    if (previous_id != current_id)//different street, add 15sec
                        time = time + 0.25;
                }

                double total_cost = visited_gScore[current] + time;
                double total_score = total_cost + distance;

                //if not visited before, label as visited now
                if (visited_gScore[id] == intersection_number) {
                    comeFrom[id] = current;
                    visited_gScore[id] = total_cost;
                    checked_fScore[total_score] = id;
                    previous_segment[id] = *iter;
                } else if (total_cost < visited_gScore[id]) {
                    comeFrom[id] = current;
                    visited_gScore[id] = total_cost;
                    checked_fScore[total_score] = id;
                    previous_segment[id] = *iter;
                }
            }
        }
    }
    return paths;
}

// Returns the time required to travel along the path specified. The path
// is passed in as a vector of street segment ids, and this function can 
// assume the vector either forms a legal path or has size == 0.
// The travel time is the sum of the length/speed-limit of each street 
// segment, plus 15 seconds per turn implied by the path. A turn occurs
// when two consecutive street segments have different street names.

double compute_path_travel_time(const vector<unsigned>& path) {
    long double time = 0;
    //if there is nothing in path
    if (path.empty()) {
        return time;
    } else if (path.size() == 1) {
        //if only one element in vector
        time = find_street_segment_travel_time(path[0]);
        return time;
    }//if there is content in path
    else {
        unsigned int size;
        //every path is confirmed to be one way path which leads to destination
        for (size = 0; size < path.size() - 1; size++) {
            //for every streetsegment ID, gets its streetsegmentInfo
            StreetSegmentInfo current_streetID_info = singleton ->get_Hash_SegmentID_Point_StreetSegmentInfo(path[size]);
            //next one in the list
            StreetSegmentInfo next_streetID_info = singleton ->get_Hash_SegmentID_Point_StreetSegmentInfo(path[size + 1]);
            //find the distance of the current segment
            double current_travel_time = find_street_segment_travel_time(path[size]);
            unsigned seg_one_id = current_streetID_info.streetID;
            unsigned seg_two_id = next_streetID_info.streetID;
            if (seg_one_id == seg_two_id) {
                time = time + current_travel_time;
            } else {
                //time in minute
                time = time + current_travel_time + 15.0 / 60.0;
            }
        }

        time = time + find_street_segment_travel_time(path[size]);

        //calculate the final path and time consumed
        return time;
    }
}

//use to find shortest path to a closet poi

vector<unsigned> dijkstra_search(unsigned intersect_id_start, unordered_set<unsigned> list) {
    unsigned intersection_number = singleton ->get_intersection_number();
    
    //the previous intersections and most efficient currrent intersection
    vector<unsigned> comeFrom(intersection_number, intersection_number);

    //the previous intersection and the segment it connects
    vector<unsigned> previous_segment(intersection_number, intersection_number);

    //the visited intersection and the total cost time from start point
    vector<double> visited_gScore(intersection_number, intersection_number);

    //the set of intersection that has been checked and already put into paths, close set
    vector<unsigned> record_intersection(intersection_number, intersection_number);

    //the fscore and its corresponding intersection
    map<double, unsigned> checked_gScore;

    visited_gScore[intersect_id_start] = 0;
    checked_gScore[0] = intersect_id_start;

    while (!checked_gScore.empty()) {
        unsigned current = checked_gScore.begin()->second; //find the intersection has lowest total score

        if (list.find(current) != list.end())//already reach the end point, find path done
            return reconstruct_path(comeFrom, previous_segment, intersect_id_start, current);

        record_intersection[current] = current;
        checked_gScore.erase(visited_gScore[current]);

        //get all adjacent street
        vector<unsigned> connected_segment = singleton ->get_Hash_IntersectionID_Point_StreetSegmentID(current);

        for (auto iter = connected_segment.begin(); iter != connected_segment.end(); iter++) {
            StreetSegmentInfo current_streetID_info = singleton ->get_Hash_SegmentID_Point_StreetSegmentInfo(*iter);
            bool valid = true;
            unsigned id = current_streetID_info.from;
            if (id != current) {//current is not the from point, check if one way
                if (current_streetID_info.oneWay)
                    valid = false;
            } else
                id = current_streetID_info.to;

            if (valid && record_intersection[id] != id) {//check if intersection has been recored into path
                double time = find_street_segment_travel_time(*iter);

                if (current != intersect_id_start) {//check if need to add 15 sec for take turn
                    unsigned previous = previous_segment[current];
                    unsigned previous_id = singleton ->get_Hash_SegmentID_Point_StreetSegmentInfo(previous).streetID;
                    unsigned current_id = current_streetID_info.streetID;

                    if (previous_id != current_id)//different street, add 15sec
                        time = time + 0.25;
                }

                double total_cost = visited_gScore[current] + time;

                //if not visited before, label as visited now
                if (visited_gScore[id] == intersection_number) {
                    comeFrom[id] = current;
                    visited_gScore[id] = total_cost;
                    checked_gScore[total_cost] = id;
                    previous_segment[id] = *iter;
                } else if (total_cost < visited_gScore[id]) {
                    comeFrom[id] = current;
                    checked_gScore.erase(visited_gScore[id]);
                    checked_gScore[total_cost] = id;
                    visited_gScore[id] = total_cost;
                    previous_segment[id] = *iter;
                }
            }
        }
    }
    return paths;
}


// Returns the shortest travel time path (vector of street segments) from 
// the start intersection to a point of interest with the specified name.
// If no such path exists, returns an empty (size == 0) vector.

vector<unsigned> find_path_to_point_of_interest(unsigned intersect_id_start, string point_of_interest_name) {
    if (!paths.empty())
        paths.clear();

    vector<unsigned> POI_ids = singleton ->get_Hash_POIname_Point_POIID(point_of_interest_name);

    if (POI_ids.size() > 0) {
        unordered_set<unsigned> closest_intersections;
        //get the closest intersection of each poi
        for (auto iter = POI_ids.begin(); iter != POI_ids.end(); iter++) {
            t_point position = singleton ->get_PointOfInterest_in_xy_coordinate(*iter);
            unsigned closest_int = find_closest_intersection_by_xy(position);
            closest_intersections.insert(closest_int);
        }
        return dijkstra_search(intersect_id_start, closest_intersections);

    } else
        return paths;
}
