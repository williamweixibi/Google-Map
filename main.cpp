#include "StreetsDatabaseAPI.h"
#include "OSMDatabaseAPI.h"
#include "m1.h"
#include "m2.h"
#include "m3.h"
#include "graphics.h"
#include "LatLon.h"
#include <iostream>
#include <string>
#include "streetGraph.h"
#include <cstdlib>
#include <readline/readline.h>
#include <readline/history.h>


using namespace std;

extern string city;

void enter_city(void)
{
    cout << "Enter city: ";
    string map_name;
    getline(cin, city);
    //mini parser
    if
    (       city != "toronto" 
            && city != "sainthelena" 
            && city != "newyork" 
            && city != "hamilton" 
            && city != "cairo" 
            && city != "goldenhorseshoe" 
            && city!="london_england") 
    {
        cout << "Wrong city!" << endl;
        enter_city();
    }          
    else if (city[0] != '/') 
    {
        map_name = "/cad2/ece297s/public/maps/" + city + ".streets.bin";
        load_map(map_name);
        
        cout<<endl;
        cout<<endl;
        cout<<"*******************************************************************"<<endl;
        cout<<"Instructions: "<<endl;
        cout<<"Enter Point of Interest or Intersection to insert location."<<endl;
        cout<<"Enter 'confirm' to exit insertion and obtain schedule."<<endl;
        cout<<"*******************************************************************"<<endl;
        cout<<"Step One: "<<endl;
        cout<<"Enter Intersection or POI: CN Tower"<<endl;
        cout<<"Step Two: "<<endl;
        cout<<"Enter Intersection or POI: Grenadier Pond"<<endl;
        cout<<"Step Three: "<<endl;
        cout<<"Enter Intersection or POI: Hilton Hotel"<<endl;
        cout<<"Step Four: "<<endl;
        cout<<"Enter Intersection or POI: confirm"<<endl;
        cout<<endl;
        cout<<"Enter leaving time: 7:00am"<<endl;
        cout<<endl;
        cout<<"1st Journey is 20 minutes."<<endl;
        cout<<"2nd Journey is 27 minutes."<<endl;
        cout<<endl;
        cout<<"Arrival time is approximately at 7:47am."<<endl;
        cout<<endl;
        cout<<"Plan again? (Enter y or n): "<<endl;
        
        string title = "Map Of city";
        draw_map();
        close_map();
    }
    else
    {
        map_name = city;
        load_map(map_name);
        
        cout<<endl;
        cout<<endl;
        cout<<"Step One: "<<endl;
        cout<<"Enter Starting Intersection: Adelaide Street West & Spadina Avenue"<<endl;
        cout<<"Step Two: "<<endl;
        cout<<"Enter Ending Intersection: Bay Street & Elm Street"<<endl;
        cout<<"Step Three: "<<endl;
        cout<<"Enter leaving time: 7:00am"<<endl;
        cout<<endl;
        cout<<"Arrival time is approximately at 7:10am."<<endl;
        cout<<endl;
        cout<<"Plan again? (Enter y or n): "<<endl;
        
        draw_map();
        close_map();
    }
}

int main() 
{
    enter_city();
    return 0;
}


