
#ifndef ROUTER_H
#define ROUTER_H

#include"geodb.h"
#include"geotools.h"
#include<queue>
#include<iostream>
#include<map>

class Router : public RouterBase
{
public:
	Router(const GeoDatabaseBase& geo_db) :m_db(geo_db) {}
	virtual ~Router() {};
	virtual vector<GeoPoint> route(const GeoPoint& pt1,
		const GeoPoint& pt2) const;

private:
	const GeoDatabaseBase& m_db;  

};




#endif // !ROUTER_H



