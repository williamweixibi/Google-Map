
#pragma once //protects against multiple inclusions of this header file

#include <string>
#include "LatLon.h"

#include "Feature.h"
#include "OSMEntityType.h"

// The ID in the global OSM database of a particular data item
// You can use this ID to look up more detailed info about a data item
// from the "layer-1" OSMDatabaseAPI.h
typedef unsigned long long OSMID;

// Structure used to return information about a StreetSegment, 
// which is the portion of a street between two intersections.
struct StreetSegmentInfo {
    OSMID wayOSMID; // OSM ID of the source way
    unsigned from, to; // intersection ID this segment runs from/to
    bool oneWay; // if true, then can only travel in from->to direction
    unsigned curvePointCount; // number of curve points between the ends
    float speedLimit;   // speed  limit in km/hr
    unsigned streetID; // index of street to which this segment belongs
};

// load the layer-2 streets database. This is the easier to use API 
// that provides higher-level information than the raw OSM data.
// To access the raw OSM data, use the layer-1 API in OSMDatabaseAPI.h
bool loadStreetsDatabaseBIN(std::string);
void closeStreetDatabase();

// aggregate queries
unsigned getNumberOfStreets();
unsigned getNumberOfStreetSegments();
unsigned getNumberOfIntersections();
unsigned getNumberOfPointsOfInterest();
unsigned getNumberOfFeatures(); 

//------------------------------------------------
// Intersection information
std::string getIntersectionName(unsigned intersectionID);
LatLon getIntersectionPosition(unsigned intersectionID);
OSMID getIntersectionOSMNodeID(unsigned intersectionID);

//number of street segments at an intersection
unsigned getIntersectionStreetSegmentCount(unsigned intersectionID);

// find the street segments at an intersection. idx is from 
// 0..streetSegmentCount-1 (at this intersection)
unsigned getIntersectionStreetSegment(unsigned intersectionID, unsigned idx);


//------------------------------------------------
// Street segment information

//find the street to which this segment belongs
StreetSegmentInfo getStreetSegmentInfo(unsigned streetSegmentID);

//fetch the latlon of the idx'th curve point
LatLon getStreetSegmentCurvePoint(unsigned streetSegmentID, unsigned idx);


//------------------------------------------------
// Street information
std::string getStreetName(unsigned streetID);

//------------------------------------------------
// Points of interest
std::string getPointOfInterestType(unsigned pointOfInterestID);
std::string getPointOfInterestName(unsigned pointOfInterestID);
LatLon getPointOfInterestPosition(unsigned pointOfInterestID);
OSMID getPointOfInterestOSMNodeID(unsigned pointOfInterestID);

//------------------------------------------------
// Natural features
const std::string& getFeatureName(unsigned featureID);
FeatureType getFeatureType(unsigned featureID);
OSMID getFeatureOSMID(unsigned featureID);
OSMEntityType getFeatureOSMEntityType(unsigned featureID);
unsigned getFeaturePointCount(unsigned featureID);
LatLon getFeaturePoint(unsigned featureID, unsigned idx);
