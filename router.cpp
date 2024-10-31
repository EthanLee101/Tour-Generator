#include"router.h"



vector<GeoPoint> Router::route(const GeoPoint& pt1,
	const GeoPoint& pt2) const {

	
	//declares the containers needed 
	queue<GeoPoint>q; 
	HashMap<GeoPoint>locationPrevious;  
	vector<GeoPoint>m_route; 
	
	//if the points are the same then there is no route 
	if (pt1.latitude == pt2.latitude && pt1.longitude == pt2.longitude) {
		m_route.push_back(pt1); 
		return m_route; 
	}

	q.push(pt1); 
	locationPrevious.insert(pt1.to_string(), pt1);  
	GeoPoint p; 

	while (!q.empty()) {
		p = q.front(); 
		q.pop(); 
		
		if (p.latitude == pt2.latitude && p.longitude == pt2.longitude) {
			// Destination reached, reconstruct the route
			m_route.push_back(p); 
			GeoPoint* temp = &p;
			
			while (temp->latitude != pt1.latitude && temp -> longitude != pt1.longitude) {
				temp = locationPrevious.find(temp->to_string()); 
				m_route.push_back(*temp); 
			}

			
			m_route.push_back(pt1); // Add the starting point
			reverse(m_route.begin(), m_route.end()); // Reverse the route to get correct order
			return m_route; 
		}

		
		vector<GeoPoint> connectedPoints = m_db.get_connected_points(p);
		map<double, GeoPoint> m_distance;

		for (const auto& next : connectedPoints) {
			if (locationPrevious.find(next.to_string()) == nullptr) { // If next has not been visited 
				double distance = distance_earth_miles(p, next);
				m_distance[distance] = next;
				locationPrevious[next.to_string()] = p; // Map next to current
			}

		}

		for (auto& c : m_distance) {
			q.push(c.second);
		}
		
	}
	
	return m_route; 
 }