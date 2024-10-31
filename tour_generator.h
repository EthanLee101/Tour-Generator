#ifndef TOUR_GENERATOR_H
#define TOUR_GENERATOR_H 

#include"stops.h"
#include"tourcmd.h"
#include"base_classes.h"
#include"geotools.h"
using namespace std;

class TourGenerator : public TourGeneratorBase
{
public:
	TourGenerator(const GeoDatabaseBase& geodb, const RouterBase& router):m_gdb(geodb),m_router(router) {}
	virtual ~TourGenerator() {}
	virtual std::vector<TourCommand> generate_tour(const Stops& stops) const;

private:
	const GeoDatabaseBase& m_gdb; 
	const RouterBase& m_router; 
};



#endif 