#include "LAVESI.h"
#include "RandomNumber.h"
#include "VectorList.h"

using namespace std;

void Fire(Parameter* parameter,
                       int yearposition,
                       vector<VectorList<Tree>>& world_tree_list,
                       vector<VectorList<Seed>>& world_seed_list,
                       vector<vector<Weather>>& world_weather_list,
                       vector<vector<Envirgrid>>& world_plot_list) {
    int aktort = 0;
    for (vector<vector<Weather>>::iterator posw = world_weather_list.begin(); posw != world_weather_list.end(); ++posw) {
        vector<Weather>& weather_list = *posw;
        vector<VectorList<Tree>>::iterator world_positon_b = (world_tree_list.begin() + aktort);
        VectorList<Tree>& tree_list = *world_positon_b;
        vector<VectorList<Seed>>::iterator world_positon_s = (world_seed_list.begin() + aktort);
        VectorList<Seed>& seed_list = *world_positon_s;
        vector<vector<Envirgrid>>::iterator world_positon_k = (world_plot_list.begin() + aktort);
        vector<Envirgrid>& plot_list = *world_positon_k;
        aktort++;
		
		RandomNumber<double> uniform(0, 1);
				
		// fire weather translation
		// Obtain fire index for each month (likely not how it should be done!)
		cout << "January fire index =" << weather_list[yearposition].fireindex1  << endl;
		if (weather_list[yearposition].fireindex1 < 3.48){
			cout << "No fire weather" << endl;
		} else if ((weather_list[yearposition].fireindex1 >= 3.48) && (weather_list[yearposition].fireindex1 <= 5)){
			cout << "Mild fire weather" << endl;
		} else if ((weather_list[yearposition].fireindex1 > 5) && (weather_list[yearposition].fireindex1 <= 7)){
			cout << "Severe fire weather" << endl;
		} else if (weather_list[yearposition].fireindex1 > 7){
			cout << "Extreme fire weather" << endl;
		}
			
			cout << "February fire index =" << weather_list[yearposition].fireindex2  << endl;
		if (weather_list[yearposition].fireindex2 < 3.48){
			cout << "No fire weather" << endl;
		} else if ((weather_list[yearposition].fireindex2 >= 3.48) && (weather_list[yearposition].fireindex2 <= 5)){
			cout << "Mild fire weather" << endl;
		} else if ((weather_list[yearposition].fireindex2 > 5) && (weather_list[yearposition].fireindex2 <= 7)){
			cout << "Severe fire weather" << endl;
		} else if (weather_list[yearposition].fireindex2 > 7){
			cout << "Extreme fire weather" << endl;
		}
			
			cout << "March fire index =" << weather_list[yearposition].fireindex3  << endl;
		if (weather_list[yearposition].fireindex3 < 3.48){
			cout << "No fire weather" << endl;
		} else if ((weather_list[yearposition].fireindex3 >= 3.48) && (weather_list[yearposition].fireindex3 <= 5)){
			cout << "Mild fire weather" << endl;
		} else if ((weather_list[yearposition].fireindex3 > 5) && (weather_list[yearposition].fireindex3 <= 7)){
			cout << "Severe fire weather" << endl;
		} else if (weather_list[yearposition].fireindex3 > 7){
			cout << "Extreme fire weather" << endl;
		}
			
			cout << "April fire index =" << weather_list[yearposition].fireindex4  << endl;
		if (weather_list[yearposition].fireindex4 < 3.48){
			cout << "No fire weather" << endl;
		} else if ((weather_list[yearposition].fireindex4 >= 3.48) && (weather_list[yearposition].fireindex4 <= 5)){
			cout << "Mild fire weather" << endl;
		} else if ((weather_list[yearposition].fireindex4 > 5) && (weather_list[yearposition].fireindex4 <= 7)){
			cout << "Severe fire weather" << endl;
		} else if (weather_list[yearposition].fireindex4 > 7){
			cout << "Extreme fire weather" << endl;
		}
			
			cout << "May fire index =" << weather_list[yearposition].fireindex5  << endl;
		if (weather_list[yearposition].fireindex5 < 3.48){
			cout << "No fire weather" << endl;
		} else if ((weather_list[yearposition].fireindex5 >= 3.48) && (weather_list[yearposition].fireindex5 <= 5)){
			cout << "Mild fire weather" << endl;
		} else if ((weather_list[yearposition].fireindex5 > 5) && (weather_list[yearposition].fireindex5 <= 7)){
			cout << "Severe fire weather" << endl;
		} else if (weather_list[yearposition].fireindex5 > 7){
			cout << "Extreme fire weather" << endl;
		}

			cout << "June fire index =" << weather_list[yearposition].fireindex6  << endl;
		if (weather_list[yearposition].fireindex6 < 3.48){
			cout << "No fire weather" << endl;
		} else if ((weather_list[yearposition].fireindex6 >= 3.48) && (weather_list[yearposition].fireindex6 <= 5)){
			cout << "Mild fire weather" << endl;
		} else if ((weather_list[yearposition].fireindex6 > 5) && (weather_list[yearposition].fireindex6 <= 7)){
			cout << "Severe fire weather" << endl;
		} else if (weather_list[yearposition].fireindex6 > 7){
			cout << "Extreme fire weather" << endl;
		}

			cout << "July fire index =" << weather_list[yearposition].fireindex7  << endl;
		if (weather_list[yearposition].fireindex7 < 3.48){
			cout << "No fire weather" << endl;
		} else if ((weather_list[yearposition].fireindex7 >= 3.48) && (weather_list[yearposition].fireindex7 <= 5)){
			cout << "Mild fire weather" << endl;
		} else if ((weather_list[yearposition].fireindex7 > 5) && (weather_list[yearposition].fireindex7 <= 7)){
			cout << "Severe fire weather" << endl;
		} else if (weather_list[yearposition].fireindex7 > 7){
			cout << "Extreme fire weather" << endl;
		}

			cout << "August fire index =" << weather_list[yearposition].fireindex8  << endl;
		if (weather_list[yearposition].fireindex8 < 3.48){
			cout << "No fire weather" << endl;
		} else if ((weather_list[yearposition].fireindex8 >= 3.48) && (weather_list[yearposition].fireindex8 <= 5)){
			cout << "Mild fire weather" << endl;
		} else if ((weather_list[yearposition].fireindex8 > 5) && (weather_list[yearposition].fireindex8 <= 7)){
			cout << "Severe fire weather" << endl;
		} else if (weather_list[yearposition].fireindex8 > 7){
			cout << "Extreme fire weather" << endl;
		}

			cout << "September fire index =" << weather_list[yearposition].fireindex9  << endl;
		if (weather_list[yearposition].fireindex9 < 3.48){
			cout << "No fire weather" << endl;
		} else if ((weather_list[yearposition].fireindex9 >= 3.48) && (weather_list[yearposition].fireindex9 <= 5)){
			cout << "Mild fire weather" << endl;
		} else if ((weather_list[yearposition].fireindex9 > 5) && (weather_list[yearposition].fireindex9 <= 7)){
			cout << "Severe fire weather" << endl;
		} else if (weather_list[yearposition].fireindex9 > 7){
			cout << "Extreme fire weather" << endl;
		}

			cout << "October fire index =" << weather_list[yearposition].fireindex10  << endl;
		if (weather_list[yearposition].fireindex10 < 3.48){
			cout << "No fire weather" << endl;
		} else if ((weather_list[yearposition].fireindex10 >= 3.48) && (weather_list[yearposition].fireindex10 <= 5)){
			cout << "Mild fire weather" << endl;
		} else if ((weather_list[yearposition].fireindex10 > 5) && (weather_list[yearposition].fireindex10 <= 7)){
			cout << "Severe fire weather" << endl;
		} else if (weather_list[yearposition].fireindex10 > 7){
			cout << "Extreme fire weather" << endl;
		}

			cout << "November fire index =" << weather_list[yearposition].fireindex11  << endl;
		if (weather_list[yearposition].fireindex11 < 3.48){
			cout << "No fire weather" << endl;
		} else if ((weather_list[yearposition].fireindex11 >= 3.48) && (weather_list[yearposition].fireindex11 <= 5)){
			cout << "Mild fire weather" << endl;
		} else if ((weather_list[yearposition].fireindex11 > 5) && (weather_list[yearposition].fireindex11 <= 7)){
			cout << "Severe fire weather" << endl;
		} else if (weather_list[yearposition].fireindex11 > 7){
			cout << "Extreme fire weather" << endl;
		}

		cout << "December fire index =" << weather_list[yearposition].fireindex12  << endl;
		if (weather_list[yearposition].fireindex12 < 3.48){
			cout << "No fire weather" << endl;
		} else if ((weather_list[yearposition].fireindex12 >= 3.48) && (weather_list[yearposition].fireindex12 <= 5)){
			cout << "Mild fire weather" << endl;
		} else if ((weather_list[yearposition].fireindex12 > 5) && (weather_list[yearposition].fireindex12 <= 7)){
			cout << "Severe fire weather" << endl;
		} else if (weather_list[yearposition].fireindex12 > 7){
			cout << "Extreme fire weather" << endl;
		}
		
	}
		
		// Update code above so that it properly assigns a fire probability rating (FPR) to each month
			// e.g.: No = 0, Mild = 0.05, Severe = 0.12, Extreme = 0.25
			// For "tuning": Check observed ratio of fire pixels / total pixels
			
		// FPR decides area burned, randomly distributed in plot area (already burning areas count towards new month's fire distribution)
			// Altertantive: FPR is scaled by "ignition probability rating" (IPR) that is a similar index for ground cells, depending on fuel and moisture. Random number decides whether cell ignites.
		
		// Within burning areas, fire damage is assessed
			// Tree mortality based on FPR and tree age (young trees have hgher risk of dying)
			// Tree damage: Decreased seed availability
		
		// On extinct fire areas, post-fire effects are assessed
			// Growth rate of existing trees increases (higher nutrient availability, lower competition) -> Function over time?
			// Number of successful saplings increased (higher recruitment of saplings because of reduced origanic surface layer) -> Function over time? (c. 30 yrs?)
		
}