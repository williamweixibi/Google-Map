#include "OSMDatabaseAPI.h"
#include "streetGraph.h"
#include "StreetsDatabaseAPI.h"
#include "OSMEntity.hpp"
#include "OSMNode.hpp"
#include "OSMWay.hpp"
#include "OSMRelation.hpp"
#include <iostream>
#include <vector>
#include <unordered_map>
#include <math.h>
#include <utility>
#include "m1.h"
using namespace std;

//declare a class pointer 
streetGraph* streetGraph::theObjectPtr = NULL;

bool check_is_closed(vector<t_point> positions) 
{
    t_point start_point = positions[0];
    t_point last_point = positions[positions.size() - 1];
    if (start_point.x == last_point.x || start_point.x == last_point.x)
        return true;
    else
        return false;
}

streetGraph::streetGraph(string mapName, string OSM_name) 
{

    street_numbers = getNumberOfStreets();
    segment_numbers = getNumberOfStreetSegments();
    intersection_number = getNumberOfIntersections();
    POI_numbers = getNumberOfPointsOfInterest();
    feature_numbers = getNumberOfFeatures();
    node_numbers = getNumberOfNodes();
    way_numbers = getNumberOfWays();
    relation_numbers = getNumberOfRelations();

    // create any data structures here to speed up your API functions

    //set up Hash_FeatureID_Point_Type
    for (unsigned i = 0; i < feature_numbers; i++) 
    {
        unsigned int point_count = getFeaturePointCount(i);
        vector<LatLon> this_feature_points;
        for (unsigned j = 0; j < point_count; j++) 
        {
            LatLon position = getFeaturePoint(i, j);
            this_feature_points.push_back(position);
        }
        FeatureID_To_Positions.push_back(this_feature_points);

        //vector<string> Hash_FeatureID_Point_Name;
        Hash_FeatureID_Point_Name.push_back(getFeatureName(i));

        //unordered_map<string, vector<unsigned>>Hash_FeatureName_Point_ID;
        //take care of the case that it's not there
        auto exists = Hash_FeatureName_Point_ID.find(Hash_FeatureID_Point_Name[i]);
        if (exists == Hash_FeatureName_Point_ID.end())
        {
            //insert a new vector at the end/start
            vector <unsigned> temp;
            temp.push_back(i);
            Hash_FeatureName_Point_ID.insert(make_pair(Hash_FeatureID_Point_Name[i], temp));
        } 
        else 
        {
            //insert in the middle of hash table
            exists->second.push_back(i);
        }

        Hash_FeatureID_Point_Type.push_back(getFeatureType(i));
        if (getFeatureType(i) == Building)
            Hash_Building_Point_ID.push_back(i);
        else if (getFeatureType(i) == Park)
            Hash_Park_Point_ID.push_back(i);
        else if (getFeatureType(i) == Lake)
            Hash_Lake_Point_ID.push_back(i);
        else if (getFeatureType(i) == Beach)
            Hash_Beach_Point_ID.push_back(i);
        else if (getFeatureType(i) == River)
            Hash_River_Point_ID.push_back(i);
        else if (getFeatureType(i) == Island)
            Hash_Island_Point_ID.push_back(i);
        else if (getFeatureType(i) == Shoreline)
            Hash_Shoreline_Point_ID.push_back(i);
        else if (getFeatureType(i) == Unknown)
            Hash_Unknown_Point_ID.push_back(i);
        else if (getFeatureType(i) == Greenspace)
            Hash_Greenspace_Point_ID.push_back(i);
        else if (getFeatureType(i) == Golfcourse)
            Hash_Golfcourse_Point_ID.push_back(i);
    }

    //make a hash table of segment ID
    //maps to its info struct
    //useful to extract stuff like oneWay, to, from
    for (unsigned j = 0; j < segment_numbers; j++) 
    {
        StreetSegmentInfo info = getStreetSegmentInfo(j);
        /*info.visited = false; //initialize to false, once this segment has been drawn, set to true
        info.segmentLength = find_street_segment_length(j); //get the segment length in order to draw the map*/
        Hash_SegmentID_Point_StreetSegmentInfo.insert(make_pair(j, info));
    }

    //a hash table to index street ID, map to street Name
    for (unsigned i = 0; i < street_numbers; i++) 
    {
        //take care of the case that it's not there
        auto exists = StreetName_Point_StreetIDs.find(getStreetName(i));
        if (exists == StreetName_Point_StreetIDs.end()) {
            //insert a new vector at the end/start
            vector <unsigned> temp;
            temp.push_back(i);
            StreetName_Point_StreetIDs.insert(make_pair(getStreetName(i), temp));
        } else {
            //insert in the middle of hash table
            exists->second.push_back(i);
        }

        hashTable_streetID_to_streetName.push_back(getStreetName(i));
    }

    //hash_intersectionID_To_StreetSegmentID
    //has table for intersection ID -> a vector of the street segments
    //indexed by intersectionID, StreetSegmentIDs are vectors
    for (unsigned IntersectionID = 0; IntersectionID < intersection_number; IntersectionID++) {
        vector<unsigned> segment;
        unsigned int count = getIntersectionStreetSegmentCount(IntersectionID);
        for (unsigned int i = 0; i < count; i++) {
            unsigned int segmentID;
            segmentID = getIntersectionStreetSegment(IntersectionID, i);
            segment.push_back(segmentID);
        }
        Hash_IntersectionID_Point_StreetSegmentID.push_back(segment);
    }

    //hash table, street to get vector of segment IDs
    Hash_StreetID_Point_SegmentID.resize(street_numbers);
    for (unsigned int segment_id = 0; segment_id < segment_numbers; segment_id++) {
        unsigned stID = getStreetSegmentInfo(segment_id).streetID;
        if (Hash_StreetID_Point_SegmentID[stID].empty()) {
            //insert the vector in the start/end
            vector <unsigned> temp;
            temp.push_back(segment_id);
            Hash_StreetID_Point_SegmentID[stID] = temp;
        } else {
            //insert in the middle
            Hash_StreetID_Point_SegmentID[stID].push_back(segment_id);
        }
    }

    //unordered_map<unsigned, unsigned> Hash_StreetID_Point_IntersectionID
    for (unsigned StreetID = 0; StreetID < street_numbers; StreetID++) {
        vector<unsigned> intersectionIDs;
        vector<unsigned> segments;
        segments = Hash_StreetID_Point_SegmentID[StreetID]; //get all segment IDs on this street

        for (auto iter = segments.begin(); iter != segments.end(); iter++) {
            StreetSegmentInfo this_segment;
            this_segment = getStreetSegmentInfo(*iter);
            //get the intersection ids (from and to) and insert into intersection vectors
            intersectionIDs.push_back(this_segment.from);
            intersectionIDs.push_back(this_segment.to);
        }
        //check for duplicate intersection IDs
        sort(intersectionIDs.begin(), intersectionIDs.end());
        intersectionIDs.erase(unique(intersectionIDs.begin(), intersectionIDs.end()), intersectionIDs.end());
        Hash_StreetID_Point_IntersectionID.insert(make_pair(StreetID, intersectionIDs));
    }

    //unordered_map<unsigned, unordered_map<unsigned, vector<unsigned>>>Common_segment_between_intersections;

    //set up Hash_SegmentID_Point_StreetSegmentInfo
    //will use it to extract street Segement form segment ID
    for (unsigned int i = 0; i < intersection_number; i++) {
        //intersection -> segments -> check if one way
        vector<unsigned> adjacent_intersection;
        vector<unsigned> this_intersection_seg;
        this_intersection_seg = Hash_IntersectionID_Point_StreetSegmentID[i];

        for (auto iter = this_intersection_seg.begin(); iter != this_intersection_seg.end(); iter++) {
            StreetSegmentInfo connected_segment;
            connected_segment = Hash_SegmentID_Point_StreetSegmentInfo[*iter];


            if (!connected_segment.oneWay) {//if two-way, check the connected intersection
                unsigned int adjacent_id = connected_segment.from;
                if (adjacent_id == i)
                    adjacent_intersection.push_back(connected_segment.to);
                else
                    adjacent_intersection.push_back(adjacent_id);
            } else {//one-way, check whether from or to
                unsigned int adjacent_from = connected_segment.from;
                unsigned int adjacent_to = connected_segment.to;
                if (adjacent_from == adjacent_to)//check if it adjacent to itself
                    adjacent_intersection.push_back(adjacent_from);
                else {
                    if (adjacent_from == i)
                        adjacent_intersection.push_back(adjacent_to);
                }
            }
        }
        //check if duplicate, need to sort first by C++ syntax
        sort(adjacent_intersection.begin(), adjacent_intersection.end());
        adjacent_intersection.erase(unique(adjacent_intersection.begin(), adjacent_intersection.end()), adjacent_intersection.end());
        IntersectionID_To_Adjacent.insert(make_pair(i, adjacent_intersection));
    }

    //set up hash table to, get interscection's postion from ID fast
    for (unsigned i = 0; i < intersection_number; i++) {
        IntersectionID_To_Position.push_back(getIntersectionPosition(i));
    }

    for (unsigned i = 0; i < POI_numbers; i++) {
        string POI_name = getPointOfInterestName(i);
        Hash_POIID_Point_POIname.push_back(POI_name);

        string POI_type = getPointOfInterestType(i);
        Hash_POIID_Point_POItype.push_back(POI_type);

        //set up hash table to, get POI's postion from ID fast
        PointOfInterest_To_Position.push_back(getPointOfInterestPosition(i));
        
         t_point POI_xy_coordinate;
        LatLon POI_position = PointOfInterest_To_Position[i];

        POI_xy_coordinate.x = POI_position.lon * cosAvg;
        POI_xy_coordinate.y = POI_position.lat;
        hash_POI_xyPoint_toID[make_pair(POI_xy_coordinate.x, POI_xy_coordinate.y)] = i;
        
        
        
        
       // PointOfInterest_in_xy_coordinate.push_back(POI_xy_coordinate);
    }

    for (unsigned i = 0; i < POI_numbers; i++) {
        //take care of the case that it's not there
        string name = Hash_POIID_Point_POIname[i];
        auto exists_name = Hash_POIname_Point_POIID.find(name);
        if (exists_name == Hash_POIname_Point_POIID.end()) {
            //insert a new vector at the end/start
            vector <unsigned> temp_name;
            temp_name.push_back(i);
            Hash_POIname_Point_POIID.insert(make_pair(name, temp_name));
        } else {
            //insert in the middle of hash table
            exists_name->second.push_back(i);
        }

        //unordered_map <string, unsigned> Hash_POItype_Point_ID;
        //take care of the case that it's not there
        string type = getPointOfInterestType(i);
        auto exists_type = Hash_POItype_Point_ID.find(type);
        if (exists_type == Hash_POItype_Point_ID.end()) {
            //insert a new vector at the end/start
            vector <unsigned> temp_type;
            temp_type.push_back(i);
            Hash_POItype_Point_ID.insert(make_pair(type, temp_type));
        } else {
            //insert in the middle of hash table
            exists_type->second.push_back(i);
        }
    }


    //find the avgerage min max lat of a city
    float max_lat = IntersectionID_To_Position[0].lat;
    float min_lat = max_lat;
    for (unsigned i = 0; i < intersection_number; i++) {
        float this_lat;
        this_lat = IntersectionID_To_Position[i].lat; //check position of intersection
        if (this_lat > max_lat)
            max_lat = this_lat;
        if (this_lat < min_lat)
            min_lat = this_lat;


        //then check for position of curve points
        vector<unsigned> this_segments;
        this_segments = Hash_IntersectionID_Point_StreetSegmentID[i];
        for (auto iter = this_segments.begin(); iter != this_segments.end(); iter++) {
            StreetSegmentInfo connected_segment;
            connected_segment = Hash_SegmentID_Point_StreetSegmentInfo[*iter];

            unsigned curveCount = connected_segment.curvePointCount;
            if (curveCount > 0) {
                for (unsigned k = 0; k < curveCount; k++) {
                    float curve_lat = getStreetSegmentCurvePoint(*iter, k).lat;
                    if (curve_lat > max_lat)
                        max_lat = curve_lat;
                    if (curve_lat < min_lat)
                        min_lat = curve_lat;
                }
            }
        }
    }

    latAvg = (max_lat + min_lat) / 2 * DEG_TO_RAD;
    cosAvg = 1 - latAvg * latAvg / 2 + latAvg * latAvg * latAvg * latAvg / 24
            - latAvg * latAvg * latAvg * latAvg * latAvg * latAvg / 720;

    //store intersection position in xy_coordinate in order to draw the maps
    for (unsigned i = 0; i < intersection_number; i++) {
        t_point intersection_xyCoor;
        LatLon this_position = IntersectionID_To_Position[i];

        intersection_xyCoor.x = this_position.lon * cosAvg;
        intersection_xyCoor.y = this_position.lat;
        intersectionPositionIn_xy_coordinate.push_back(intersection_xyCoor);
        
        hash_Intersection_Position_toID[make_pair(intersection_xyCoor.x, intersection_xyCoor.y)] = i;
        string intersectionNameStr = getIntersectionName(i);
        hash_intersectionIDtoIntersectionName_str.push_back(intersectionNameStr);
    }

    //store position of curve points of each segment in xy-coordinate
    for (unsigned i = 0; i < segment_numbers; i++) {
        StreetSegmentInfo this_segment;
        vector<t_point> listOfCurvePts_xyCoor;
        unsigned curveCount;

        this_segment = Hash_SegmentID_Point_StreetSegmentInfo[i];
        curveCount = this_segment.curvePointCount;
        if (curveCount > 0) {
            for (unsigned j = 0; j < curveCount; j++) {
                LatLon curve_position = getStreetSegmentCurvePoint(i, j);
                t_point curvePt_xyCoor;

                curvePt_xyCoor.x = curve_position.lon * cosAvg;
                curvePt_xyCoor.y = curve_position.lat;
                listOfCurvePts_xyCoor.push_back(curvePt_xyCoor);
            }
        }
        curvePoints_in_xy_coordinate.push_back(listOfCurvePts_xyCoor);
    }

    for (unsigned i = 0; i < POI_numbers; i++) {
        t_point POI_xy_coordinate;
        LatLon POI_position = PointOfInterest_To_Position[i];

        POI_xy_coordinate.x = POI_position.lon * cosAvg;
        POI_xy_coordinate.y = POI_position.lat;
        PointOfInterest_in_xy_coordinate.push_back(POI_xy_coordinate);
    }

    //vector<vector<t_point>> Feature_Points_In_xycoordinate;
    for (unsigned i = 0; i < feature_numbers; i++) {
        vector<LatLon> this_position;
        this_position = FeatureID_To_Positions[i];

        vector<t_point> position_in_xy_coor;
        for (auto iter = this_position.begin(); iter != this_position.end(); iter++) {
            t_point point_in_xy;
            point_in_xy.x = (*iter).lon * cosAvg;
            point_in_xy.y = (*iter).lat;
            position_in_xy_coor.push_back(point_in_xy);
        }
        Feature_Points_In_xycoordinate.push_back(position_in_xy_coor);
    }

    for (unsigned i = 0; i < feature_numbers; i++) {
        vector<t_point> positions = Feature_Points_In_xycoordinate[i];
        bool is_close = check_is_closed(positions);
        if (is_close) {
            long double area = get_area(positions);
            area_Point_Close_Feature.insert(make_pair(area, i));
        } else
            open_features.push_back(i);
    }

    //vector<OSMNode*> Index_Point_Node
    //and unordered_map<OSMNode*, vector<pair<string, string>> > Node_Point_Tags
    for (unsigned i = 0; i < node_numbers; i++) {
        const OSMNode* current = getNodeByIndex(i);
        Index_Point_Node.push_back(current);
        unsigned tag_count = getTagCount(current);

        unordered_map <string, string> tag_list;
        for (unsigned j = 0; j < tag_count; j++) {
            tag_list.insert(getTagPair(current, j));
        }
        Entity_Point_Tags.insert(make_pair(current, tag_list));

        OSMID id = current ->id();
        OSMID_Point_OSMNode.insert(make_pair(id, current));
        OSMID_Point_OSMEntity.insert(make_pair(id, current));
    }

    //vector<OSMWay*> Index_Point_Way
    //unordered_map<OSMWay*, vector<pair<string, string>> > Way_Point_Tags
    for (unsigned i = 0; i < way_numbers; i++) {
        const OSMWay* current = getWayByIndex(i);
        Index_Point_Way.push_back(current);
        unsigned tag_count = getTagCount(current);

        unordered_map <string, string> tag_list;
        for (unsigned j = 0; j < tag_count; j++) {
            tag_list.insert(getTagPair(current, j));
        }
        Entity_Point_Tags.insert(make_pair(current, tag_list));

        OSMID id = current ->id();
        OSMID_Point_OSMEntity.insert(make_pair(id, current));
    }

    //vector<OSMRelation*> Index_Point_Relation
    //unordered_map<OSMRelation*, vector<pair<string, string>> > Relation_Point_Tags
    for (unsigned i = 0; i < relation_numbers; i++) {
        const OSMRelation* current = getRelationByIndex(i);
        Index_Point_Relation.push_back(current);
        unsigned tag_count = getTagCount(current);

        unordered_map <string, string> tag_list;
        for (unsigned j = 0; j < tag_count; j++) {
            tag_list.insert(getTagPair(current, j));
        }
        Entity_Point_Tags.insert(make_pair(current, tag_list));

        OSMID id = current ->id();
        OSMID_Point_OSMEntity.insert(make_pair(id, current));
    }

    for (unsigned i = 0; i < segment_numbers; i++) {
        StreetSegmentInfo this_segment;
        this_segment = Hash_SegmentID_Point_StreetSegmentInfo.find(i) ->second;
        OSMID id = this_segment.wayOSMID;
        const OSMEntity* this_way = OSMID_Point_OSMEntity.find(id)->second;

        unordered_map<string, string> get_tags;
        get_tags = Entity_Point_Tags.find(this_way)->second;
        auto iter = get_tags.find("highway");
        if (iter != get_tags.end()) {
            if (iter->second == "motorway")
                Highway.push_back(i);
            else if (iter->second == "trunk" || iter->second == "primary" || iter->second == "secondary" ||
                    iter->second == "tertiary" || iter->second == "trunk_link" || iter->second == "primary_link" ||
                    iter->second == "secondary_link" || iter->second == "tertiary_link" || iter->second == "motorway_link")
                mainroad.push_back(i);
            else
                sideroad.push_back(i);
        }
    }

    for (unsigned i = 0; i < node_numbers; i++) {
        bool is_Station = false;
        const OSMNode* node = Index_Point_Node[i];
        unordered_map<string, string> get_tags = Entity_Point_Tags.find(node) ->second;

        auto iter = get_tags.find("public_transport");
        if (iter != get_tags.end()) {
            if (iter ->second == "stop_position" || iter->second == "platform" || iter->second == "station")
                is_Station = true;
        } else {
            iter = get_tags.find("highway");

            if (iter != get_tags.end()) {
                if (iter->second == "bus_stop")
                    is_Station = true;
            } else {
                iter = get_tags.find("railway");
                if (iter != get_tags.end()) {
                    if (iter->second == "station" || iter->second == "subway_entrance")
                        is_Station = true;
                } else {
                    iter = get_tags.find("aeroway");
                    if (iter != get_tags.end()) {
                        if (iter->second == "aerodrome")
                            is_Station = true;
                        else {
                            iter = get_tags.find("amenity");
                            if (iter != get_tags.end()) {
                                if (iter->second == "ferry_terminal")
                                    is_Station = true;
                            }
                        }
                    }
                }
            }
        }

        if (is_Station) {
            LatLon position = node->coords();
            t_point position_in_xy;
            position_in_xy.x = position.lon * cosAvg;
            position_in_xy.y = position.lat;
            //store the position
            Locations_Of_All_Stations.push_back(position_in_xy);
            //store the name
            auto name = get_tags.find("name");
            if (name != get_tags.end() && name ->second != "<unknown>")
                Name_of_All_Stations.push_back(name ->second);
            else
                Name_of_All_Stations.push_back("\0");

        }
    }

    //vector<const vector<unsigned long long>> all_transport_lines;
    for (unsigned i = 0; i < way_numbers; i++) {
        const OSMWay* this_way = Index_Point_Way[i];
        unordered_map<string, string> get_tags;
        get_tags = Entity_Point_Tags.find(this_way) ->second;
        auto iter = get_tags.find("railway");
        if (iter != get_tags.end()) {
            if (iter ->second == "subway") {
                vector<unsigned long long> noderef = this_way ->ndrefs();
                all_transport_lines.push_back(noderef);
            }
        } else {
            auto iter = get_tags.find("route");
            if (iter != get_tags.end()) {
                if (iter ->second == "ferry") {
                    vector<unsigned long long> noderef = this_way ->ndrefs();
                    all_transport_lines.push_back(noderef);
                }

            }
        }
    }
    //the hash table used to return all the possible intersection names, used for parser
    //also convert the string type to char*
    
    
    for(unsigned long long intersectionID=0;intersectionID<intersection_number;intersectionID++) {
        string name_of_intersection=getIntersectionName(intersectionID);
        intersections[name_of_intersection]=intersectionID; 
        intnamestruct.push_back(name_of_intersection);
    }
    //get the layer information from 
    //layer information is Values between -5 to 5
    // for (unsigned i = 0; i < feature_numbers; i++) {
    //    auto iter = get_tags.find("highway");
    //}

}
