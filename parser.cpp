#include "StreetsDatabaseAPI.h"
#include "OSMDatabaseAPI.h"
#include "m1.h"
#include "m2.h"
#include "graphics.h"
#include "LatLon.h"
#include <iostream>
#include <string>
#include "parser.h"
#include "cstring"
#include "sstream"
#include <readline/readline.h>
#include <readline/history.h>

using namespace std;

bool isPOI;
bool Reverse;
bool both_intersections;

extern unsigned starting_intersection;
extern unsigned ending_intersection;

string Result_Reverse;
string Result_Space_Removed;
string end_poi;

bool check_category(string name)
{
    vector<unsigned> POI_y;
    POI_y = singleton->get_Hash_POIname_Point_POIID(name);
    int n = 0;
    int str_len = name.length();
    char* char_ptr = (char*)malloc(str_len*sizeof(char));
   
    for (int i=0; i < str_len; i++) 
    {
        if (name[i] != '\"')
        {
            char_ptr[n] = name[i];
            n+=1;
        }
    } 
    char_ptr[n] = '\0';
    string new_string(char_ptr);
    free (char_ptr);
    
    if (POI_y.begin() == POI_y.end()) 
    {
        return false;//is intersection
    }
    else
    {
        return true;//is POI
    }
}
        
bool check_validity (char* current) 
{
    //convert into string
    string str(current);
    //declare ampersand
    string amp;
    
    string first_input, second_intersection_name, intersection_input_1, second_name_POI;
    string first_input_sub1, second_input_sub1, first_input_sub2, second_input_sub2, first_input_sub3, second_input_sub3;  
    int count_words = 1;
    int w = 0;
    //the intersection info stored in database
    string intersection_input_sub1, intersection_input2_sub1, intersection_input_sub2, intersection_input2_sub2, intersection_input_sub3, intersection_input2_sub3;  
    
    //word count
    for(unsigned i = 0; i < str.length(); i++) 
    {
        if(str[i] == ' ')
        {
            count_words+=1;
        }
    }
    //find the length of the string
    int str_len = str.length();
    //use character pointer to navigate through the string
    char* char_ptr = (char*) malloc(str_len*sizeof(char));
    //if it is a quotation mark, then ignore and create new string
    for (int j=0; j < str_len; j++)
    {
        if (str[j] != '\"')
        {
            char_ptr[w] = str[j];
            w++;
        }
    } 
    char_ptr[w] = '\0';
    string new_string(char_ptr);
    free (char_ptr);

    stringstream check_input (new_string); 
    //change here
    if (count_words == 1)
    {
        //could be first name of POI
        check_input >> first_input;  
    }
    else if (count_words == 2) 
    {
        //could be second name of POI
        //entered 2 words and combine them into one string 
        check_input >> first_input_sub1 >> first_input_sub2;
        first_input = first_input_sub1 + first_input_sub2;
    }
    else if (count_words == 3) 
    {
        //intersection name
        check_input >> first_input_sub1 >> first_input_sub2 >> amp; 
        //combine
        first_input = first_input_sub1 + first_input_sub2 + amp;
    }
    else if (count_words == 4) 
    {
        check_input >> first_input_sub1 >> first_input_sub2 >> amp >> second_input_sub1;  
        //combine
        first_input = first_input_sub1 + first_input_sub2 + amp + second_input_sub1;
    }
    
    else if (count_words == 5)
    {
        check_input >> first_input_sub1 >> first_input_sub2 >> amp >> second_input_sub1 >> second_input_sub2;  
        if (isPOI) 
        {
            first_input = first_input_sub1 + first_input_sub2 + amp + second_input_sub1 + second_input_sub2;
        }
        else 
        {
            //split into 2 intersections
            first_input = first_input_sub1 + first_input_sub2;
            second_intersection_name = second_input_sub1 + second_input_sub2;
        }
    }
    else if (count_words == 6) 
    {
        check_input >> first_input_sub1 >> first_input_sub2 >> amp >> second_input_sub1 >> second_input_sub2 >> second_input_sub3;
        if (isPOI)
        {
            first_input = first_input_sub1 + first_input_sub2 + amp + second_input_sub1 + second_input_sub2 + second_input_sub3;      
        }
        else 
        {
            if (amp == "&") 
            {
                //split into intersections
                first_input = first_input_sub1 + first_input_sub2;
                second_intersection_name = second_input_sub1 + second_input_sub2 + second_input_sub3;
            }
            else 
            {
                //no amp, then just parse 
                first_input = first_input_sub1 + first_input_sub2 + amp;
                second_intersection_name = second_input_sub2 + second_input_sub3;
            }
        }
    }
    
    else if (count_words == 7) 
    {
        check_input >> first_input_sub1 >> first_input_sub2 >> first_input_sub3 >> amp >> second_input_sub1 >> second_input_sub2 >> second_input_sub3;
        if (isPOI)
        {
            //combine all
            first_input = first_input_sub1 + first_input_sub2 + first_input_sub3 + amp + second_input_sub1 + second_input_sub2 + second_input_sub3;
        }
        else 
        {
            //split accordingly
            first_input = first_input_sub1 + first_input_sub2 + first_input_sub3;
            second_intersection_name = second_input_sub1 + second_input_sub2 + second_input_sub3;
        }
    }
    //if intersection
    if (isPOI == false) 
    {
        vector<string>temp=singleton->get_name_int();
        for(unsigned int i=0;i<temp.size();i++)
        {
            stringstream input_intersection(temp[i]);

            if (count_words == 1)
            {
                input_intersection >> intersection_input_1;  
            }
            else if (count_words == 2) 
            {
                input_intersection >> intersection_input_sub1 >> intersection_input_sub2;
                //combine
                intersection_input_1 = intersection_input_sub1 + intersection_input_sub2;
            }
            else if (count_words == 3) 
            {
                input_intersection >> intersection_input_sub1 >> intersection_input_sub2 >> amp; 
                //combine
                intersection_input_1 = intersection_input_sub1 + intersection_input_sub2 + amp;
            }
            else if (count_words == 4) 
            {
                input_intersection >> intersection_input_sub1 >> intersection_input_sub2 >> amp >> intersection_input2_sub1;  
                intersection_input_1 = intersection_input_sub1 + intersection_input_sub2 + amp + intersection_input2_sub1;
            }
            else if (count_words == 5) 
            {
                input_intersection >> intersection_input_sub1 >> intersection_input_sub2 >> amp >> intersection_input2_sub1 >> intersection_input2_sub2;  
                intersection_input_1 = intersection_input_sub1 + intersection_input_sub2;
                second_name_POI = intersection_input2_sub1 + intersection_input2_sub2;
            }
            else if (count_words == 6) 
            {
                input_intersection >> intersection_input_sub1 >> intersection_input_sub2 >> amp >> intersection_input2_sub1 >> intersection_input2_sub2 >> intersection_input2_sub3;
                if (amp == "&")
                {
                    intersection_input_1 = intersection_input_sub1 + intersection_input_sub2;
                    second_name_POI = intersection_input2_sub1 + intersection_input2_sub2 + intersection_input2_sub3;
                }
                else
                {
                    intersection_input_1 = intersection_input_sub1 + intersection_input_sub2 + amp;
                    second_name_POI = intersection_input2_sub2 + intersection_input2_sub3;
                }
            }
            else if (count_words == 7)
            {
                input_intersection >> intersection_input_sub1 >> intersection_input_sub2 >> intersection_input_sub3 >> amp >> intersection_input2_sub1 >> intersection_input2_sub2 >> intersection_input2_sub3;
                intersection_input_1 = intersection_input_sub1 + intersection_input_sub2 + intersection_input_sub3;
                second_name_POI = intersection_input2_sub1 + intersection_input2_sub2 + intersection_input2_sub3;
            }
            //normal flow
            if ((first_input == intersection_input_1) && (second_intersection_name == second_name_POI)) 
            {                 
                Reverse = false;
                Result_Space_Removed = temp[i];     
                return true;                                                                    
            }
            //reverse flow
            else if ((first_input == second_name_POI) && (second_intersection_name == intersection_input_1))
            {
                Reverse = true;
                Result_Reverse = temp[i];
                return true; 
            }
        }
    }
    else
    {
        vector<string>tempstring=singleton->get_namesofPOI();
        for(unsigned int j=0;j<tempstring.size();j++)
        {
            stringstream input_intersection (tempstring[j]);

            if (count_words == 1)
            {
                input_intersection >> intersection_input_1;  
            }
            else if (count_words == 2) 
            {
                input_intersection >> intersection_input_sub1 >> intersection_input_sub2;
                intersection_input_1 = intersection_input_sub1 + intersection_input_sub2;
            }
            else if (count_words == 3) 
            {
                input_intersection >> intersection_input_sub1 >> intersection_input_sub2 >> amp; 
                intersection_input_1 = intersection_input_sub1 + intersection_input_sub2 + amp;
            }

            else if (count_words == 4) 
            {
                input_intersection >> intersection_input_sub1 >> intersection_input_sub2 >> amp >> intersection_input2_sub1;  
                intersection_input_1 = intersection_input_sub1 + intersection_input_sub2 + amp + intersection_input2_sub1;
            }

            else if (count_words == 5) 
            {
                input_intersection >> intersection_input_sub1 >> intersection_input_sub2 >> amp >> intersection_input2_sub1 >> intersection_input2_sub2;  
                intersection_input_1 = intersection_input_sub1 + intersection_input_sub2 + amp + intersection_input2_sub1 + intersection_input2_sub2;
            }

            else if (count_words == 6) 
            {
                input_intersection >> intersection_input_sub1 >> intersection_input_sub2 >> amp >> intersection_input2_sub1 >> intersection_input2_sub2 >> intersection_input2_sub3;
                intersection_input_1 = intersection_input_sub1 + intersection_input_sub2 + amp + intersection_input2_sub1 + intersection_input2_sub2 + intersection_input2_sub3;      
            }

            else if (count_words == 7) 
            {
                input_intersection >> intersection_input_sub1 >> intersection_input_sub2 >> intersection_input_sub3 >> amp >> intersection_input2_sub1 >> intersection_input2_sub2 >> intersection_input2_sub3;
                intersection_input_1 = intersection_input_sub1 + intersection_input_sub2 + intersection_input_sub3 + amp + intersection_input2_sub1 + intersection_input2_sub2 + intersection_input2_sub3; 
            }
            if ((first_input == intersection_input_1)) 
            {                  
                Result_Space_Removed = tempstring[j];
                end_poi = Result_Space_Removed;
                return true;                                                                    
            }
        }     
    }
    return false;
}


void tab_auto_completion() 
{
    //Use tab for auto completion
    rl_bind_key('\t', rl_complete);
    //Use our function for auto-complete
    rl_attempted_completion_function = command_completion;
    //Tell readline to handle double and single quotes for us
    rl_completer_quote_characters = strdup("\"\'"); 
    int count = 1;
    cout << "***********************************Instructions***************************************" << endl;
    cout << "Enter Quotations then press Tab to Enable auto-completion" << endl;
    cout << "Enter exit/quit/end to end command function" << endl;
    cout << "**************************************************************************************" << endl;
    while (count < 3)
    {
        char* char_input;
        if (count == 1)
        {
            cout << "Starting Intersection: " << endl;
        }
        else if (count == 2)
        {
            cout << "Ending Intersection:" << endl;
        }
        while((char_input = readline("")) != NULL) 
        {
            //add to history when something is detected
            if(strcmp(char_input, "") != 0)
            {
                add_history(char_input); 
            }
            if(strcmp(char_input, "exit") == 0 || strcmp(char_input, "quit") == 0 || strcmp(char_input, "end") == 0)
            {
                cout << "Terminating Command Function" << endl;
                free(char_input);
                return;
            }
            string input_text(char_input);
            //the function returns true is POI, false if intersection
            isPOI = check_category(input_text);
            if (count == 1) 
            {
                //if it is a valid POI or intersection and is a intersection
                if (check_validity(char_input) == true && isPOI == false)
                {
                    
                    break;
                }
                else 
                { 
                    cout << "Intersection not found" << endl;
                }
            }
            else if (count == 2)
            {
                if (check_validity(char_input) == true)
                {
                    break;
                }
                else
                { 
                    cout << "Invalid Input" << endl;
                }
            }
            free(char_input);
            char_input = NULL;
        }
        //for intersections only
        unordered_map<string,unsigned>temp=singleton->get_intersection_name_point_ID();
        if (count == 1)
        {
            if (Reverse == false && isPOI == false)
            {
                drawscreen();
                starting_intersection = temp[Result_Space_Removed]; 
            }
                
            else if (Reverse == true && isPOI == false)
            {
                drawscreen();
                starting_intersection = temp[Result_Reverse]; 
            }
        }
        if (count == 2) 
        {
            if (Reverse == false && isPOI == false)
            {
                drawscreen();
                ending_intersection = temp[Result_Space_Removed]; 
                both_intersections = true;
            }
            else if (Reverse == true && isPOI == false)   
            {
                drawscreen();
                ending_intersection = temp[Result_Reverse]; 
                both_intersections = true;
            }
            else if (isPOI == true) 
            {
                drawscreen();
                end_poi = Result_Space_Removed;
                both_intersections = false;
            }
            drawscreen();
        }
        free(char_input);
        count ++;
        drawscreen();
    }
    drawscreen();
}

char* intersection_name_generator(const char* stem_text, int state) 
{
    static int count;
    char* temp=(char*)"(unknown)";
    
    if (isPOI == true) 
    {   
        vector<string>atemp = singleton->get_namesofPOI();
        
        if(state == 0)
        {
            count = -1;
        }
        int temp_len=strlen(temp);
        int text_len = strlen(stem_text);
        
        while(count < (int)atemp.size()-1) 
        {
            count++;
            
            if(strncmp(atemp[count].c_str(), temp, temp_len)!=0)
            {
                if(strncmp(atemp[count].c_str(), stem_text, text_len) == 0) 
                {
                    return strdup(atemp[count].c_str());
                }
            }
        }
    }
    else 
    {
        vector<string> atemp = singleton->get_name_int();
        if(state == 0) 
        {
            count = -1;
        }
        int temp_len=strlen(temp);
        int text_len = strlen(stem_text);
        //Search through intersections_names until we find a match
        while(count < (int) ((atemp.size())-1)) 
        {
            count++;
            if(strncmp(atemp[count].c_str(),temp,temp_len)!=0)
            {
                if(strncmp(atemp[count].c_str(), stem_text, text_len) == 0) 
                {
                    return strdup(atemp[count].c_str());
                }
            }
        }
    }
    return NULL;
}

char** command_completion(const char* stem_text, int start, int end) 
{
    char ** matches = NULL;
    if(start != 0) 
    {
        matches = rl_completion_matches(stem_text, intersection_name_generator);
    }
        return matches;
}