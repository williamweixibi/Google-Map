/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   parser.h
 * Author: biweixi
 *
 * Created on March 11, 2016, 11:10 AM
 */

#ifndef PARSER_H
#define PARSER_H

#ifdef __cplusplus
extern "C" {
#endif
    
#include <iostream>
#include <vector>
#include <cstdlib>
#include <readline/readline.h>
#include <readline/history.h>

using namespace std;

extern bool isPOI;
extern bool Reverse;
extern bool both_intersections;

extern unsigned starting_intersection;
extern unsigned ending_intersection;

extern string Result_Reverse;
extern string Result_Space_Removed;
extern string end_poi;


void enter_city(void);

char** command_completion(const char* stem_text, int start, int end);

bool check_validity (char* my_location);

char * intersection_name_generator(const char* stem_text, int state);

bool check_category(string name);

void tab_auto_completion();
#ifdef __cplusplus
}
#endif

#endif /* PARSER_H */

