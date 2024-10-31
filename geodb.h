#ifndef GEODB_H
#define GEODB_H

#include "base_classes.h"
#include "hashmap.h" // Assuming this file contains your HashMap implementation
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

class GeoDatabase : public GeoDatabaseBase
{
public:
    GeoDatabase();
    virtual ~GeoDatabase();
    virtual bool load(const string& map_data_file);
    virtual bool get_poi_location(const string& poi, GeoPoint& point) const;
    virtual vector<GeoPoint> get_connected_points(const GeoPoint& pt) const;
    virtual string get_street_name(const GeoPoint& pt1, const GeoPoint& pt2) const;

private:
    //HashMap<vector<vector<GeoPoint>>> streetMap;
    HashMap<vector<string>> m_streetMap;
    vector<string> m_streetNames;

    HashMap<GeoPoint> m_poiMap;
    vector<string> m_poiNames;

    HashMap<vector<GeoPoint>> m_connectedMap;

};

#endif 