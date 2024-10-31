#include <fstream>
#include <sstream>
#include <vector>
#include "geodb.h"
#include "geotools.h"
#include <iomanip>  //to avoid rounding

using namespace std;

GeoDatabase::GeoDatabase() : GeoDatabaseBase() {}

GeoDatabase::~GeoDatabase() {}

bool GeoDatabase::load(const string& map_data_file) {

    ifstream inf(map_data_file);

    if (!inf)return false;


    string sLine;

    while (getline(inf, sLine)) {

        //if line is empty move onto next line
        if (sLine.empty())continue;

        //first line is street
        string streetName = sLine;

        //second line is geoPoints
        string geoLine;
        getline(inf, geoLine);

        //parses  lat and lon from line 
        istringstream iss(geoLine);
        string lat1, lon1, lat2, lon2;
        iss >> lat1 >> lon1 >> lat2 >> lon2;

        //next line is amount of pois on street segment 
        int n_poi;
        inf >> n_poi;
        inf.ignore(10000, '\n');


        GeoPoint startPoint(lat1, lon1);
        GeoPoint endPoint(lat2, lon2);
        GeoPoint midPoint = midpoint(startPoint, endPoint);
        m_connectedMap[startPoint.to_string()].push_back(endPoint);
        m_connectedMap[endPoint.to_string()].push_back(startPoint);



        // add startPoint and Endpoint to streetMap 
        m_streetMap[startPoint.to_string()].push_back(streetName);
        m_streetMap[endPoint.to_string()].push_back(streetName);
        m_streetNames.push_back(streetName);


        // Update poiMap
        if (n_poi > 0) {

            //only add midpoint if there is more than 1 poi 
            m_connectedMap[startPoint.to_string()].push_back(midPoint);
            m_connectedMap[endPoint.to_string()].push_back(midPoint);
            m_connectedMap[midPoint.to_string()].push_back(startPoint);
            m_connectedMap[midPoint.to_string()].push_back(endPoint);

            // Extract points of interest
            for (int i = 0; i < n_poi; i++) {
                char ch;
                string p_Name;
                while (inf.get(ch)) {
                    if (ch == '|')
                        break;
                    p_Name += ch;
                }

                //extract geopoints of pois
                string poiGeo;
                getline(inf, poiGeo);
                istringstream iss(poiGeo);
                string pLat, pLon;
                iss >> pLat >> pLon;

                GeoPoint poiPoint(pLat, pLon);
                m_poiMap.insert(p_Name, poiPoint);

                // Connect poiPoint with midpoint
                m_connectedMap[midPoint.to_string()].push_back(poiPoint);
                m_connectedMap[poiPoint.to_string()].push_back(midPoint);
                m_poiNames.push_back(p_Name);
                m_streetMap[midPoint.to_string()].push_back(streetName);
            }
        }
    }

    return true;
}

bool GeoDatabase::get_poi_location(const string& poi, GeoPoint& point) const {
    const GeoPoint* poiPoint = m_poiMap.find(poi);
    if (poiPoint!=nullptr) {
        point = *poiPoint;
        return true;
    }
    return false;
}

vector<GeoPoint> GeoDatabase::get_connected_points(const GeoPoint& pt) const {
    const vector<GeoPoint>* connectedPoints = m_connectedMap.find(pt.to_string());
    if (connectedPoints!=nullptr)
        return *connectedPoints;

    return vector<GeoPoint>();
}

string GeoDatabase::get_street_name(const GeoPoint& pt1, const GeoPoint& pt2) const {
    
    for (int i = 0; i < m_poiNames.size(); i++) {
        const GeoPoint* poiPoint = m_poiMap.find(m_poiNames[i]);
        if (m_poiMap.find(m_poiNames[i]) != nullptr && m_connectedMap.find(poiPoint->to_string()) != nullptr) {
            GeoPoint midPoint = m_connectedMap.find(poiPoint->to_string())->at(0);
            if ((poiPoint->to_string() == pt1.to_string() && midPoint.to_string() == pt2.to_string()) ||
                (poiPoint->to_string() == pt2.to_string() && midPoint.to_string() == pt1.to_string()))
                return "a path";
        }
    }

    const vector<string>* str1 = m_streetMap.find(pt1.to_string());
    const vector<string>* str2 = m_streetMap.find(pt2.to_string());
    if (str1 != nullptr && str2 != nullptr)
        for (int i = 0; i < str1->size(); i++)
            for (int j = 0; j < str2->size(); j++)
                if (str1->at(i) == str2->at(j))
                    return str1->at(i);

    return "";
}
