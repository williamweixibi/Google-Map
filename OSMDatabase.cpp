/*
 * OSMDatabase.cpp
 *
 *  Created on: Jun 15, 2015
 *      Author: jcassidy
 */

#include "OSMDatabase.hpp"
#include <boost/range/adaptor/transformed.hpp>

using namespace std;

void OSMDatabase::print() const
{
	cout << "OSMDatabaseBuilder summary: " << endl;
	cout << "  Bounds: " << bounds_.first << "-" << bounds_.second << endl;
	cout << "  " << relations_.size() << " relations" << endl;
	cout << "  " << nodes_.size() << " nodes" << endl;
	cout << "  " << ways_.size() << " ways" << endl;

	cout << "  Relation member roles: ";
	for(const auto r : relationMemberRoles_.values())
		cout << r << "  ";
	cout << endl;
}

void OSMDatabase::showNode(const unsigned i) const
{
	if (i >= nodes_.size())
	{
		cout  << "No such node" << endl;
		return;
	}
	cout << "Full details for node " << i << " (OSM ID " << nodes_[i].id() << ")" << endl;
	cout << "  Coords (" << nodes_[i].coords().lat << "," << nodes_[i].coords().lon << ")" << endl;

	for(auto p : nodes_[i].tags() | boost::adaptors::transformed(nodeTags_.pairLookup()))
		cout << "  " << p.first << " = " << p.second << endl;
}

void OSMDatabase::showRelation(const unsigned i) const
{
	if (i >= relations_.size())
	{
		cout  << "No such relation" << endl;
		return;
	}

	cout << "Full details for relation " << i << "(OSM ID " << relations_[i].id() << ")" << endl;
	cout << " Tags" << endl;
	for(auto p : relations_[i].tags() | boost::adaptors::transformed(relationTags_.pairLookup()))
		cout << "  " << p.first << " = " << p.second << endl;

	cout << " Members" << endl;

	boost::container::flat_map<OSMRelation::MemberType,string> typeMap;

	typeMap.insert(make_pair(OSMRelation::Node,string("node")));
	typeMap.insert(make_pair(OSMRelation::Way,string("way")));
	typeMap.insert(make_pair(OSMRelation::Relation,string("relation")));

	for(auto p : relations_[i].members())
	{
		cout << "  type='" << typeMap.at(p.type) << " ID " << p.id << "  role='" << relationMemberRoles_.getValue(p.role) << "' " << endl;
	}
}

void OSMDatabase::showWay(const unsigned i) const
{
	if (i >= ways_.size())
	{
		cout  << "No such way" << endl;
		return;
	}
	cout << "Full details for way " << i << " (OSM ID " << ways_[i].id() << ")" << endl;
	cout << " Tags" << endl;
	for(auto p : ways_[i].tags() | boost::adaptors::transformed(wayTags_.pairLookup()))
		cout << "  " << p.first << " = " << p.second << endl;
	cout << " Node refs" << endl;

	for(auto p: ways_[i].ndrefs())
		cout << "  " << p << endl;
}

vector<vector<LatLon>> close_polys(const vector<vector<LatLon>>& segs,pair<LatLon,LatLon> bounds);

vector<LatLon> OSMDatabase::extractPoly(const OSMWay& way) const
{
	vector<LatLon> ll;

	// extract all points from way
	for(const auto nd : way.ndrefs())
	{
		const auto n_it = idToNodeMap_.find(nd);
		if (n_it == idToNodeMap_.end())
			cout << "Warning: dangling reference to node " << nd << " in extractPoly(OSMWay)" << endl;
		else
		{
			const OSMNode& n = *(n_it->second);
			ll.push_back(n.coords());
		}
	}
	return ll;
}
//
//vector<vector<LatLon>> OSMDatabase::extractPoly(const OSMRelation& rel) const
//{
//	vector<vector<LatLon>> ll;
//
//	// loop over all ways in the relation
//	for(const OSMRelation::Member& m : rel.members() | boost::adaptors::filtered([](OSMRelation::Member m)
//			{ return m.type==OSMRelation::Way; }))
//	{
//
//		const OSMWay* wp = wayPtrFromID(m.id);
//
//		if (!wp)
//			cout << "Warning: dangling way reference " << m.id << " in extractPoly" << endl;
//		else
//		{
//			ll.emplace_back(extractPoly(*wp));
//			cout << "  Way with " << ll.back().size() << " node refs" << endl;
//		}
//	}
//
//	vector<vector<LatLon>> o = close_polys(ll,bounds_);
//
//	cout << "After closing there are " << o.size() << " polys" << endl;
//	return o;
//}
