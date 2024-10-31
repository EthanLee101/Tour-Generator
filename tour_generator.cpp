#include"tour_generator.h"

vector<TourCommand> TourGenerator::generate_tour(const Stops& stops) const {

	vector<TourCommand>tour; 

	for (size_t i = 0; i < stops.size(); i++) {

		vector<GeoPoint>curr_route; 
		string poi, poi_com; 
		stops.get_poi_data(i, poi, poi_com); 

		TourCommand commentary; 
		commentary.init_commentary(poi, poi_com); 
		tour.push_back(commentary); 

		if (i + 1 < stops.size()) {
			
			
			//current and next geopoint 
			GeoPoint curr;
			GeoPoint next; 
	
			//next name of poi 
			string n_poi, n_poi_com; 
			stops.get_poi_data(i + 1, n_poi, n_poi_com);

		

			m_gdb.get_poi_location(poi, curr);
			m_gdb.get_poi_location(n_poi, next);

			
			curr_route = m_router.route(curr, next);  
			 
				for (size_t j = 0; j < curr_route.size() - 1; j++) {
					
					string streetName = m_gdb.get_street_name(curr_route[j], curr_route[j+1]);
					double distance = distance_earth_miles(curr_route[j], curr_route[j + 1]); 
					double direction = angle_of_line(curr_route[j], curr_route[j + 1]); 
					string m_dir; 

					if (direction >= 0 && direction < 22.5) {
						m_dir = "east"; 
					}
					else if (direction >= 22.5 && direction < 67.5) {
						m_dir = "northeast"; 
					}
					else if (direction >= 22.5 && direction < 7.5) {
						m_dir = "northeast";
					}
					else if (direction >= 67.5 && direction < 112.5) {
						m_dir = "north";
					}
					else if (direction >= 112.5 && direction < 157.5) {
						m_dir = "northwest";
					}
					else if (direction >= 157.5 && direction < 202.5) {
						m_dir = "west";
					}
					else if (direction >= 202.5 && direction < 247.5) {
						m_dir = "southwest";
					}
					else if (direction >= 247.5 && direction < 292.5) {
						m_dir = "south";
					}
					else if (direction >= 292.5 && direction < 337.5) {
						m_dir = "southeast";
					}
					else if (direction >= 337.5) {
						m_dir = "east"; 
					}
					
					if (distance != 0) {
						TourCommand proceed; 
						proceed.init_proceed(m_dir, streetName, distance, curr, next); 
						tour.push_back(proceed); 
					}
			

					if (j + 2 < curr_route.size()) {

						string streetName2 = m_gdb.get_street_name(curr_route[j + 1], curr_route[j + 2]);
						double angle_dir = angle_of_turn(curr_route[j], curr_route[j + 1], curr_route[j + 2]); 
						if (streetName != streetName2 && angle_dir >= 1 && angle_dir <= 359) {
							string m_turn; 
							if (angle_dir >= 1 && angle_dir < 180)m_turn = "left";
							else if (angle_dir >= 180 && angle_dir <= 359)m_turn = "right"; 
							TourCommand turn;
							turn.init_turn(m_turn,streetName2);
							tour.push_back(turn);  

						}
						
						     
					}
					
				}
			

		}


	}
	
	return tour; 
}