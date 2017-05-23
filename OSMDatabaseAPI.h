#include "OSMEntity.hpp"

#include "OSMNode.hpp"
#include "OSMWay.hpp"
#include "OSMRelation.hpp"

// This is the interface to the lower-level (layer-1) database.
// This database provides access to all OSM data; the data is more
// complete but less structured than the layer-2 API representation.

// load the optional layer-1 OSM database
bool loadOSMDatabaseBIN(const std::string&);
void closeOSMDatabase();

// Query the number of entities in the database
unsigned long long getNumberOfNodes();
unsigned long long getNumberOfWays();
unsigned long long getNumberOfRelations();

// Query all nodes in the database, by node index.
// The range of this index is from 0 to getNumberOfNodes()-1 for OSMNodes
// and so on.
const OSMNode*          getNodeByIndex(unsigned idx);
const OSMWay*           getWayByIndex(unsigned idx);
const OSMRelation*      getRelationByIndex(unsigned idx);

// Count number of tags for a given OSMEntity (OSMWay/OSMNode/OSMRelation)
unsigned getTagCount(const OSMEntity* e);

// Return n'th key-value pair
std::pair<std::string,std::string> getTagPair(const OSMEntity* e,unsigned idx);
