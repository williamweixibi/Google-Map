/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   m3.h
 * Author: biweixi
 *
 * Created on March 8, 2016, 4:03 PM
 */

#ifndef M3_H
#define M3_H
#include "m1.h"
#include "streetGraph.h"

extern vector<unsigned> paths;

vector<unsigned> find_path_between_intersections(unsigned intersect_id_start, unsigned intersect_id_end);

double compute_path_travel_time(const vector<unsigned>& path);

vector<unsigned> find_path_to_point_of_interest (unsigned intersect_id_start, string point_of_interest_name);
vector<unsigned> reconstruct_path(vector<unsigned> comeFrom, vector<unsigned> previous_segment, unsigned intersect_id_start, unsigned intersect_id_end);
vector<unsigned int> reconstruct(unsigned intersect_id_start, unsigned intersect_id_end);
#endif /* M3_H */
