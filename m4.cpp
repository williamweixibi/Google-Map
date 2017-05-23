/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#pragma once
#include <vector>
#include <string>
#include "m3.h"
#include "m4.h"
#include "m1.h"
#include "StreetsDatabaseAPI.h"
#include "LatLon.h"
#include "streetGraph.h"
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <map>
#include <unordered_set>

vector<unsigned> deliveryOrder;
vector<unsigned> deliveryPath;
unsigned current_start;
double least_cost = 0;

/*extern struct DeliveryInfo {
    //Specifies a delivery order.
    //
    //To satisfy the order the item-to-be-delivered must have been picked-up 
    //from the pickUp intersection before visiting the dropOff intersection.

    DeliveryInfo(unsigned pick_up, unsigned drop_off)
    : pickUp(pick_up), dropOff(drop_off) {
    }


    //The intersection id where the item-to-be-delivered is picked-up.
    unsigned pickUp;

    //The intersection id where the item-to-be-delivered is dropped-off.
    unsigned dropOff;
};*/


// This routine takes in a vector of N deliveries (pickUp, dropOff
// intersection pairs) and another vector of M intersections that
// are legal start and end points for the path. 
//
// The first vector gives the delivery information: a set of 
// pickUp/dropOff pairs of intersection ids which specify the 
// deliveries to be made. A delivery can only be dropped-off after 
// the associated item has been picked-up. 
// 
// The second vector gives the intersection
// ids of courier company depots containing trucks; you start at any
// one of these depots and end at any one of the depots.
//
// This routine returns a vector of street segment ids that form a
// path, where the first street segment id is connected to a depot
// intersection, and the last street segment id also connects to a
// depot intersection.  The path must traverse all the delivery
// intersections in an order that allows all deliveries to be made --
// i.e. a package won't be dropped off if you haven't picked it up
// yet.
//
// You can assume that N is always at least 1, and M is always at
// least one (i.e. both input vectors are non-empty).
//
// It is legal for the same intersection to appear multiple times in
// the pickUp or dropOff list (e.g. you might have two deliveries with
// a pickUp intersection id of #50). The same intersection can also
// appear as both a pickUp location and a dropOff location.
//        
// If you have two pickUps to make at an intersection, 
// traversing the intersection once is sufficient
// to pick up both packages, and similarly one traversal of an 
// intersection is sufficient to drop off all the packages that have
// already been picked up and need to be dropped off at that intersection.
//
// Depots will never appear as pickUp or dropOff locations for deliveries.
//  
// If no path connecting all the delivery locations
// and a start and end depot exists, this routine should return an
// empty (size == 0) vector.

vector<unsigned> start_point_dijkstra_search(unsigned intersect_id_start, unordered_set<unsigned> list, unsigned intersection_number) {
    if (!paths.empty())
        paths.clear();

    //the previous intersections and most efficient current intersection
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
        auto temp = checked_gScore.begin();
        unsigned current = temp ->second; //find the intersection has lowest total score

        if (list.find(current) != list.end()) {//already reach the end point, find path done
            current_start = current;
            //deliveryOrder.push_back(current);
            //cout << current << " ";
            least_cost = least_cost + temp ->first;
            return reconstruct_path(comeFrom, previous_segment, intersect_id_start, current);
        }

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

vector<unsigned> new_dijkstra_search(unsigned intersect_id_start, unordered_set<unsigned> &list,
        vector<unordered_set<unsigned>> &drop_offs, unsigned intersection_number) {
    if (!paths.empty())
        paths.clear();

    //the previous intersections and most efficient current intersection
    vector<unsigned> comeFrom(intersection_number, intersection_number);

    //the previous intersection and the segment it connects
    vector<unsigned> previous_segment(intersection_number, intersection_number);

    //the visited intersection and the total cost time from start point
    vector<double> visited_gScore(intersection_number, intersection_number);

    //the set of intersection that has been checked and already put into paths, close set
    vector<unsigned> record_intersection(intersection_number, intersection_number);

    //the gscore and its corresponding intersection
    map<double, unsigned> checked_gScore;

    visited_gScore[intersect_id_start] = 0;
    checked_gScore[0] = intersect_id_start;

    while (!checked_gScore.empty()) {
        auto temp = checked_gScore.begin();
        unsigned current = temp ->second; //find the intersection has lowest total score

        if (list.find(current) != list.end()) {
            bool valid_point = false;
            //check if it's a new pickup point or the drop off point that the relative pickup has been done
            if (drop_offs[current].empty()) {//it's a pickup location
                valid_point = true;
                //pick_ups[current] = intersection_number;
                list.erase(current);
            }
            else {//it's a drop_off location, check if it's legal
                unordered_set<unsigned> this_pickups = drop_offs[current];
                vector<unsigned> need_delete;
                for (auto iter = this_pickups.begin(); iter != this_pickups.end(); iter++) {
                    if (list.find(*iter) == list.end()) {//it's legal
                        valid_point = true;
                        //this_pickups.erase(*iter);
                        need_delete.push_back(*iter);
                    }
                }

                for (unsigned k = 0; k < need_delete.size(); k++) {
                    drop_offs[current].erase(need_delete[k]);
                }

                if (drop_offs[current].empty())//all corresponding pickup locations have been passed
                    list.erase(current);
            }

            if (valid_point) {
                current_start = current;
                deliveryOrder.push_back(current);
                //cout << current << " ";
                least_cost = least_cost + temp ->first;
                return reconstruct_path(comeFrom, previous_segment, intersect_id_start, current);
            }
        }

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

double acceptance_probability(double old_cost, double new_cost, double Time_ref) {
    double rate = exp(-(new_cost - old_cost) / Time_ref);
    return rate;
}

/*std::vector<unsigned> traveling_courier(const std::vector<DeliveryInfo>& deliveries, const std::vector<unsigned>& depots){
    if(!deliveryOrder.empty())
        deliveryOrder.clear();
    if(!deliveryPath.empty())
        deliveryPath.clear();
    least_cost = 0;
    
    unordered_set<unsigned> all_depots;
    
    deliveryOrder.push_back(depots[0]);
    for(unsigned i = 0; i < deliveries.size(); i++){
        deliveryOrder.push_back(deliveries[i].pickUp);
        deliveryOrder.push_back(deliveries[i].dropOff);
    }
    
    for (unsigned i = 0; i < depots.size(); i++) {
        all_depots.insert(depots[i]);
        //unsigned depot_id = depots[i];
        //depot_ref[depot_id] = depot_id;
    }
    
    
    //get initial solution
    vector<unsigned> deliveryPath = find_path_between_intersections(depot[0], deliveries[])
}*/


std::vector<unsigned> traveling_courier(const std::vector<DeliveryInfo>& deliveries, const std::vector<unsigned>& depots) {
    if (!deliveryOrder.empty())
        deliveryOrder.clear();
    if (!deliveryPath.empty())
        deliveryPath.clear();
    least_cost = 0;

    unsigned intersection_number = singleton ->get_intersection_number();
    //vector<unsigned> depot_ref(intersection_number, intersection_number);
    //vector<unsigned> pick_ups(intersection_number, intersection_number);

    unordered_set<unsigned> pickup_refs;
    vector<unordered_set<unsigned>> drop_offs(intersection_number, pickup_refs);
    unordered_set<unsigned> all_pickups;
    unordered_set<unsigned> all_pass;
    unordered_set<unsigned> all_depots;

    for (unsigned i = 0; i < depots.size(); i++) {
        all_depots.insert(depots[i]);
    }

    for (unsigned i = 0; i < deliveries.size(); i++) {
        unsigned pickup_id = deliveries[i].pickUp;
        unsigned dropoff_id = deliveries[i].dropOff;
        //pick_ups[pickup_id] = pickup_id;
        drop_offs[dropoff_id].insert(pickup_id);
        all_pass.insert(pickup_id);
        all_pass.insert(dropoff_id);
        all_pickups.insert(pickup_id);
    }

    //get an initial solution, start with depot[0]
    deliveryPath = start_point_dijkstra_search(depots[0], all_pickups, intersection_number);
    unsigned best_start = depots[0];
    unsigned best_pickup = current_start;
    if (depots.size() > 1) {
        double initial_cost = least_cost;
        for (unsigned i = 1; i < depots.size(); i++) {
            least_cost = 0;
            vector<unsigned> temp_path = start_point_dijkstra_search(depots[i], all_pickups, intersection_number);
            if(deliveryPath.empty() || least_cost < initial_cost){
                    best_start = depots[i];
                    best_pickup = current_start;
                    initial_cost = least_cost;
                    deliveryPath = temp_path;  
            }
        }
        current_start = best_pickup;
    }
    
    deliveryOrder.push_back(best_start);
    deliveryOrder.push_back(best_pickup);
    all_pass.erase(best_pickup);

    while (!all_pass.empty()) {
        vector<unsigned> next_path = new_dijkstra_search(current_start, all_pass, drop_offs, intersection_number);
        deliveryPath.insert(deliveryPath.end(), next_path.begin(), next_path.end());
        //least_cost = least_cost + compute_path_travel_time(next_path);
    }

    vector<unsigned> last_path = start_point_dijkstra_search(current_start, all_depots, intersection_number);
    deliveryPath.insert(deliveryPath.end(), last_path.begin(), last_path.end());
    deliveryOrder.push_back(current_start);
    /*for (unsigned i = 0; i < deliveryOrder.size(); i++) {
        cout << deliveryOrder[i] << " ";
    }*/

    return deliveryPath;



    /*double Time_cost = 50;
    double reduce_rate = 0.95;

    vector<unsigned> initial_solution = start_point_dijkstra_search(depots[0], all_pickups);
    double temp_cost = compute_path_travel_time(initial_solution);
    unsigned start_id = depots[0];
    unsigned temp_end = current_start;

    for (unsigned i = 1; i < depots.size(); i++) {
        unsigned new_start = depots[i];
        vector<unsigned> new_solution = start_point_dijkstra_search(depots[i], all_pickups);
        double new_cost = compute_path_travel_time(new_solution);
        double probability = acceptance_probability(temp_cost, new_cost, Time_cost);
        if (new_cost < temp_cost || random(0, 1) < probability) {
            initial_solution = new_solution;
            temp_cost = new_cost;
            start_id = new_start;
            temp_end = current_start;
        }
    }
    pick_ups[temp_end] = intersection_number;
    Time_cost = Time_cost * reduce_rate;

    while (temp_cost > Time_cost) {
        all_pass.erase(temp_end);
        if (!all_pass.empty()) {
            vector<unsigned> next_route = new_dijkstra_search(temp_end, all_pass, drop_offs, pick_ups);
            double new_cost = compute_path_travel_time(next_route);
            double probability = acceptance_probability(temp_cost, new_cost, Time_cost);
            if (new_cost < temp_cost || random(0, 1) < probability) {
                initial_solution = new_solution;
                temp_cost = new_cost;
                start_id = new_start;
                temp_end = current_start;
            }
        }

    }*/

}