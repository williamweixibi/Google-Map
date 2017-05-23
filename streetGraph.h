
#ifndef STREETGRAPH_H
#define STREETGRAPH_H

#include "StreetsDatabaseAPI.h"
#include "OSMDatabaseAPI.h"
#include "Feature.h"
#include <vector>
#include <unordered_map>
#include <map>
#include <algorithm>
#include <string>
#include <iostream>
#include <cassert>
#include "graphics.h"
#include <utility>

using namespace std;

class streetGraph {
private:
    static streetGraph *theObjectPtr;
    //use xyPosition to store the lat lon converted, correct xy in equirectangular surface

    string OSM_file_name; //need to parse in order to get this

    //order to fraw featrues
    vector<FeatureType> orderDrawFeature;

    //static streetGraph *theObject;

    //street name to IDs, include duplictaes
    unordered_map<string, vector <unsigned>> StreetName_Point_StreetIDs;

    //index by street ID, return segment vector
    vector< vector<unsigned>> Hash_IntersectionID_Point_StreetSegmentID;

    //the street and its corresponding segements
    vector< vector<unsigned> > Hash_StreetID_Point_SegmentID;

    //Segment ID, map to struc info
    unordered_map<unsigned, StreetSegmentInfo>Hash_SegmentID_Point_StreetSegmentInfo;

    // intersection ID, make to adjacent vector
    unordered_map<unsigned, vector<unsigned>> IntersectionID_To_Adjacent;

    //street ID , maps to intersection vector
    unordered_map<unsigned, vector<unsigned>> Hash_StreetID_Point_IntersectionID;

    //point of interest id to find name of point of interest
    vector<string> Hash_POIID_Point_POIname;

    //point of interest id to find type of point of interest
    vector<string> Hash_POIID_Point_POItype;

    //point of interest name as key to find ids of point of interest
    unordered_map<string, vector<unsigned>> Hash_POIname_Point_POIID;

    //vector to extract Lat lon of a position ID fast
    vector<LatLon> IntersectionID_To_Position;

    //vector to extract lat lon of POI fast
    vector<LatLon> PointOfInterest_To_Position;

    //a hash table to store coordinates converted to x,y from lat lon
    vector<t_point> intersectionPositionIn_xy_coordinate;

    //hash table for coordinates of curve points, stored in x, y instead of lat lon
    vector<vector<t_point>> curvePoints_in_xy_coordinate;

    // hash table of POI in 2D x, y coordinates instead of Latlon, easier to draw and work with in M2
    vector<t_point> PointOfInterest_in_xy_coordinate;

    //hash table of street Id to name, used to show road name when zoomed in
    vector<string> hashTable_streetID_to_streetName;

    //??
    vector<string> Hash_FeatureID_Point_Name;

    unordered_map<string, vector<unsigned>> Hash_FeatureName_Point_ID;

    vector<FeatureType> Hash_FeatureID_Point_Type;

    vector<vector<LatLon>> FeatureID_To_Positions;

    vector<vector<t_point>> Feature_Points_In_xycoordinate;

    unordered_map<OSMID, const OSMEntity*> OSMID_Point_OSMEntity;

    unordered_map<OSMID, const OSMNode*> OSMID_Point_OSMNode;

    vector<const OSMNode*> Index_Point_Node;

    vector<const OSMWay*> Index_Point_Way;

    vector<const OSMRelation*> Index_Point_Relation;

    unordered_map<const OSMEntity*, unordered_map<string, string> > Entity_Point_Tags;

    vector<unsigned> Highway;

    vector<unsigned> mainroad;

    vector<unsigned> sideroad;

    vector<unsigned> Hash_Building_Point_ID;

    vector<unsigned> Hash_Park_Point_ID;

    vector<unsigned> Hash_Lake_Point_ID;

    vector<unsigned> Hash_Beach_Point_ID;

    vector<unsigned> Hash_River_Point_ID;

    vector<unsigned> Hash_Island_Point_ID;

    vector<unsigned> Hash_Shoreline_Point_ID;

    vector<unsigned> Hash_Unknown_Point_ID;

    vector<unsigned> Hash_Greenspace_Point_ID;

    vector<unsigned> Hash_Golfcourse_Point_ID;

    vector<t_point> Locations_Of_All_Stations;

    vector<string> Name_of_All_Stations;

    unordered_map<unsigned, unordered_map<unsigned, vector<unsigned>>> Common_segment_between_intersections;

    unordered_map <string, vector<unsigned>> Hash_POItype_Point_ID;


    //used to print all subway route and ferry route
    vector<vector<unsigned long long>> all_transport_lines;

    //std::map<std::long double, unsigned, std::greater<std::long double> > area_Point_Close_Feature;
    //map<long double, vector<unsigned>> area_Point_Close_Feature;

    vector<unsigned> close_feature_in_area; //from largest features to the smallest

    vector<unsigned> open_features;

    //used to create the prmpt line, need a list of all intersections
    vector<const char*> hash_intersectionIDtoIntersectionName;

    vector<string> POI_names;

    unordered_map<string, unsigned> intersections;

    vector<string> intnamestruct;
    map<pair<float, float>, unsigned> hash_Intersection_Position_toID; //for miulestone3
    map<pair<float,float>,unsigned> hash_POI_xyPoint_toID; //milestone 3 sucks
    vector<string> hash_intersectionIDtoIntersectionName_str;
    //declare global variables, will use getters from API
    //we get the following number from API once. gonna save us some time. compared to putting them in the for loops in laod_map
    unsigned int street_numbers;
    unsigned int segment_numbers;
    unsigned int intersection_number;
    unsigned int POI_numbers;
    unsigned int feature_numbers;
    long double latAvg;
    long double cosAvg;
    unsigned long long node_numbers;
    unsigned long long way_numbers;
    unsigned long long relation_numbers;


    //private constructors
    //streetGraph(){ theObjectPtr = NULL;}
    streetGraph(string mapName, string OSM_name);


    //prevent another singleton instance, good practice

    streetGraph(const streetGraph&) {
        assert(false);
    }

    //prevent another singleton instance, good practice

    void operator=(const streetGraph&) {
        assert(false);
    }

public:
    //streetGraph(string mapName);

    static streetGraph* getObjectPtr(string mapName, string OSM_name) {
        //if (theObjectPtr != NULL)
        //delete theObjectPtr;
        bool load_success = loadStreetsDatabaseBIN(mapName);
        bool load_OSM_success = loadOSMDatabaseBIN(OSM_name);
        if (load_success && load_OSM_success) {
            if (theObjectPtr == NULL)
                theObjectPtr = new streetGraph(mapName, OSM_name);
        }
        return theObjectPtr;
    }
    /******getters to access the private global data************/

    //Each function get the content loaded into the data structure

    vector<unsigned> get_StreetName_Point_StreetIDs(string streetName) {
        vector<unsigned> street_ids;
        if (StreetName_Point_StreetIDs.count(streetName) > 0)
            street_ids = StreetName_Point_StreetIDs.find(streetName)->second;
        return street_ids;
    }

    vector<unsigned> get_Hash_IntersectionID_Point_StreetSegmentID(unsigned int intersectionID) {
        return Hash_IntersectionID_Point_StreetSegmentID[intersectionID];
    }

    vector<unsigned> get_Hash_StreetID_Point_SegmentID(unsigned int streetID) {
        return Hash_StreetID_Point_SegmentID[streetID];
    }

    StreetSegmentInfo get_Hash_SegmentID_Point_StreetSegmentInfo(unsigned int segmentID) {
        if (Hash_SegmentID_Point_StreetSegmentInfo.find(segmentID) != Hash_SegmentID_Point_StreetSegmentInfo.end())
            return Hash_SegmentID_Point_StreetSegmentInfo.find(segmentID)->second;
        else
            assert(false);
    }

    vector<unsigned> get_IntersectionID_To_Adjacent(unsigned int intersectionID) {
        return IntersectionID_To_Adjacent.find(intersectionID)->second;
    }

    vector<unsigned> get_Hash_StreetID_Point_IntersectionID(unsigned int streetID) {
        return Hash_StreetID_Point_IntersectionID.find(streetID)->second;
    }

    LatLon get_IntersectionID_To_Position(unsigned int idx) {
        return IntersectionID_To_Position[idx];
    }

    LatLon get_PointOfInterest_To_Position(unsigned int idx) {
        return PointOfInterest_To_Position[idx];
    }

    t_point get_intersectionPositionIn_xy_coordinate(unsigned int intersectionID) {
        if(intersectionID >= intersectionPositionIn_xy_coordinate.size() || intersectionID < 0)
            return t_point(0,0);
        else            
        return intersectionPositionIn_xy_coordinate[intersectionID];
    }

    vector<t_point> get_curvePoints_in_xy_coordinate(unsigned int street) {
        return curvePoints_in_xy_coordinate[street];
    }

    unsigned int get_street_numbers() {
        return street_numbers;
    }

    unsigned int get_segment_numbers() {
        return segment_numbers;
    }

    long double get_latAvg() {
        return latAvg;
    }

    long double get_cosAvg() {
        return cosAvg;
    }

    unsigned int get_intersection_number() {
        return intersection_number;
    }

    unsigned int get_POI_numbers() {
        return POI_numbers;
    }

    unsigned int get_feature_numbers() {
        return feature_numbers;
    }

    string get_hashTable_streetID_to_streetName(unsigned int idx) {
        return hashTable_streetID_to_streetName[idx];
    }

    string get_hash_POIID_Point_POIname(unsigned int POI_id) {
        return Hash_POIID_Point_POIname[POI_id];
    }

    string get_Hash_POIID_Point_POItype(unsigned int POI_id) {
        return Hash_POIID_Point_POItype[POI_id];
    }

    vector<unsigned> get_Hash_POIname_Point_POIID(string POI_name) {
        vector<unsigned> IDs;
        if (Hash_POIname_Point_POIID.count(POI_name) > 0)
            IDs = Hash_POIname_Point_POIID.find(POI_name) -> second;
        return IDs;
    }

    t_point get_PointOfInterest_in_xy_coordinate(unsigned int POI_id) {
        return PointOfInterest_in_xy_coordinate[POI_id];
    }

    string get_Hash_FeatureID_Point_Name(unsigned int Feature_id) {
        return Hash_FeatureID_Point_Name[Feature_id];
    }

    vector<unsigned> get_Hash_FeatureName_Point_ID(string name) {
        vector<unsigned> IDs;
        if (Hash_FeatureName_Point_ID.count(name) > 0)
            IDs = Hash_FeatureName_Point_ID.find(name)->second;
        return IDs;
    }

    FeatureType get_Hash_FeatureID_Point_Type(unsigned int Feature_id) {
        return Hash_FeatureID_Point_Type[Feature_id];
    }

    vector<unsigned> get_Hash_Building_Point_ID() {
        return Hash_Building_Point_ID;
    }

    vector<unsigned> get_Hash_Park_Point_ID() {
        return Hash_Park_Point_ID;
    }

    vector<unsigned> get_Hash_Lake_Point_ID() {
        return Hash_Lake_Point_ID;
    }

    vector<unsigned> get_Hash_Beach_Point_ID() {
        return Hash_Beach_Point_ID;
    }

    vector<unsigned> get_Hash_River_Point_ID() {
        return Hash_River_Point_ID;
    }

    vector<unsigned> get_Hash_Island_Point_ID() {
        return Hash_Island_Point_ID;
    }

    vector<unsigned> get_Hash_Shoreline_Point_ID() {
        return Hash_Shoreline_Point_ID;
    }

    vector<unsigned> get_Hash_Unknown_Point_ID() {
        return Hash_Unknown_Point_ID;
    }

    vector<unsigned> get_Hash_Greenspace_Point_ID() {
        return Hash_Greenspace_Point_ID;
    }

    vector<unsigned> get_Hash_Golfcourse_Point_ID() {
        return Hash_Golfcourse_Point_ID;
    }

    vector<LatLon> get_FeatureID_To_Positions(unsigned int Feature_id) {
        return FeatureID_To_Positions[Feature_id];
    }

    vector<t_point> get_Feature_Points_In_xycoordinate(unsigned int Feature_id) {
        return Feature_Points_In_xycoordinate[Feature_id];
    }

    const OSMNode* get_Index_Point_Node(unsigned idx) {
        return Index_Point_Node[idx];
    }

    const OSMWay* get_Index_Point_Way(unsigned idx) {
        return Index_Point_Way[idx];
    }

    const OSMRelation* get_Index_Point_Relation(unsigned idx) {
        return Index_Point_Relation[idx];
    }

    const OSMEntity* get_OSMID_Point_OSMEntity(OSMID id) {
        return OSMID_Point_OSMEntity.find(id)->second;
    }

    const OSMNode* get_OSMID_Point_OSMNode(OSMID id) {
        return OSMID_Point_OSMNode.find(id)->second;
    }

    unordered_map <string, string> get_Entity_Point_Tags(const OSMEntity* entity) {
        return Entity_Point_Tags.find(entity)->second;
    }

    unordered_map<string, vector<unsigned> > get_Map_POItype_Point_ID() {
        return Hash_POItype_Point_ID;
    }

    vector<unsigned> get_Hash_POItype_Point_ID(string type) {
        vector<unsigned> POI_ids;
        if (Hash_POItype_Point_ID.count(type) > 0)
            POI_ids = Hash_POItype_Point_ID.find(type) ->second;
        return POI_ids;
    }

    vector<t_point> get_Locations_Of_All_Stations() {
        return Locations_Of_All_Stations;
    }

    vector<vector<unsigned long long>> get_all_transport_lines() {
        return all_transport_lines;
    }

    string get_Name_of_All_Stations(unsigned idx) {
        return Name_of_All_Stations[idx];
    }

    multimap<long double, unsigned> area_Point_Close_Feature;

    vector<unsigned> get_Highway() {
        return Highway;
    }

    vector<unsigned> get_mainroad() {
        return mainroad;
    }

    vector<unsigned> get_sideroad() {
        return sideroad;
    }

    vector<unsigned> get_close_feature_in_area() {
        return close_feature_in_area;
    }

    vector<unsigned> get_open_features() {
        return open_features;
    }

    vector<unsigned> get_common_segments(unsigned intersection1, unsigned intersection2) {
        vector<unsigned> common_segments;
        /*auto temp = Common_segment_between_intersections[intersection1].find(intersection2);
        if(temp != Common_segment_between_intersections[intersection1].end())
            common_segments = temp ->second;*/
        common_segments = Common_segment_between_intersections[intersection1][intersection2];
        return common_segments;
    }

    vector<string>get_namesofPOI() {
        return Hash_POIID_Point_POIname;
    }

    unordered_map<string, unsigned>get_intersection_name_point_ID() {
        return intersections;
    }

    vector<string> get_name_int() {
        return intnamestruct;
    }



    //use it to get the readline list of all possible intersections

    vector<const char*> get_hash_intersectionIDtoIntersectionName() {
        return hash_intersectionIDtoIntersectionName;
    }
    unsigned int get_hash_Intersection_Position_toID(t_point intersection) {
        //work around fucking easyGL		

        return hash_Intersection_Position_toID.find(make_pair(intersection.x, intersection.y))->second;
    }
     //use it to get the readline list of all possible intersections		
    string get_hash_intersectionIDtoIntersectionName_str(unsigned int inter){		
        ///cout<<"str getter";		
        return hash_intersectionIDtoIntersectionName_str[inter];		
    }		
    	
    long double get_area(vector<t_point> positions) {
        long double area;
        unsigned num = positions.size();

        long double sum1 = 0;
        long double sum2 = 0;

        for (unsigned i = 0; i < num - 1; i++) {
            sum1 = sum1 + positions[i].x * positions[i + 1].y;
            sum2 = sum2 + positions[i + 1].x * positions[i].y;
        }

        sum1 += positions[num - 1].x * positions[0].y;
        sum2 += positions[0].x * positions[num - 1].y;

        area = std::abs(sum1 - sum2) / 2.0;
        return area;
    }

    bool check_is_closed(vector<t_point> positions) {
        t_point start_point = positions[0];
        t_point last_point = positions[positions.size() - 1];
        if (start_point.x == last_point.x || start_point.x == last_point.x)
            return true;
        else
            return false;
    }

    //this is a mirror of the constructor, used to destroy theObjectPtr in close_map()
    //interesting C++ syntax, can't delete in destructor,infinite loop!

    static void ResetInstance() {
        delete theObjectPtr; // REM : it works even if the pointer is NULL (does nothing then)
        theObjectPtr = NULL; // so theObjPtr getter will still work.
    }

    ~streetGraph() {
        //delete theObjectPtr;
        theObjectPtr = NULL;
    }
};

#endif /* STREETGRAPH_H */
