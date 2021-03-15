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
				
		// ##### Fire weather #####
		// Based on fire probability rating (FPR) assigned to each month from linear model between T/P and observed MODIS fire pixels
		// FPR is basically the logarithmic predicted amount of MODIS pixels burning under the respective T/P conditions during a given month
		
		unsigned short int n_mildweather = 0;	// holding the number of occurences of mild/severe/extreme monthly fire weather
		unsigned short int n_severeweather = 0;
		unsigned short int n_extremeweather = 0;
		
		unsigned short int firethresh1 = 3.48;	// determine thresholds that decide fire weather severity (previous values: 3.48, 5, 7)
		unsigned short int firethresh2 = 6.9;
		unsigned short int firethresh3 = 8.3;
		
		
		// cout << "January fire index =" << weather_list[yearposition].fireindex1  << endl;
		if (weather_list[yearposition].fireindex1 < firethresh1){	// print fire weather severity depending on calculated FPR, and if fire weather occurs increase counter of fire weather occurences 
			// cout << "No fire weather" << endl;
		} else if ((weather_list[yearposition].fireindex1 >= firethresh1) && (weather_list[yearposition].fireindex1 <= firethresh2)){
			// cout << "Mild fire weather" << endl;
			n_mildweather++;
		} else if ((weather_list[yearposition].fireindex1 > firethresh2) && (weather_list[yearposition].fireindex1 <= firethresh3)){
			// cout << "Severe fire weather" << endl;
			n_severeweather++;
		} else if (weather_list[yearposition].fireindex1 > firethresh3){
			// cout << "Extreme fire weather" << endl;
			n_extremeweather++;
		}
			
		// cout << "February fire index =" << weather_list[yearposition].fireindex2  << endl;
		if (weather_list[yearposition].fireindex2 < firethresh1){
			// cout << "No fire weather" << endl;
		} else if ((weather_list[yearposition].fireindex2 >= firethresh1) && (weather_list[yearposition].fireindex2 <= firethresh2)){
			// cout << "Mild fire weather" << endl;
			n_mildweather++;
		} else if ((weather_list[yearposition].fireindex2 > firethresh2) && (weather_list[yearposition].fireindex2 <= firethresh3)){
			// cout << "Severe fire weather" << endl;
			n_severeweather++;
		} else if (weather_list[yearposition].fireindex2 > firethresh3){
			// cout << "Extreme fire weather" << endl;
			n_extremeweather++;
		}
			
		// cout << "March fire index =" << weather_list[yearposition].fireindex3  << endl;
		if (weather_list[yearposition].fireindex3 < firethresh1){
			// cout << "No fire weather" << endl;
		} else if ((weather_list[yearposition].fireindex3 >= firethresh1) && (weather_list[yearposition].fireindex3 <= firethresh2)){
			// cout << "Mild fire weather" << endl;
			n_mildweather++;
		} else if ((weather_list[yearposition].fireindex3 > firethresh2) && (weather_list[yearposition].fireindex3 <= firethresh3)){
			// cout << "Severe fire weather" << endl;
			n_severeweather++;
		} else if (weather_list[yearposition].fireindex3 > firethresh3){
			// cout << "Extreme fire weather" << endl;
			n_extremeweather++;
		}
			
		// cout << "April fire index =" << weather_list[yearposition].fireindex4  << endl;
		if (weather_list[yearposition].fireindex4 < firethresh1){
			// cout << "No fire weather" << endl;
		} else if ((weather_list[yearposition].fireindex4 >= firethresh1) && (weather_list[yearposition].fireindex4 <= firethresh2)){
			// cout << "Mild fire weather" << endl;
			n_mildweather++;
		} else if ((weather_list[yearposition].fireindex4 > firethresh2) && (weather_list[yearposition].fireindex4 <= firethresh3)){
			// cout << "Severe fire weather" << endl;
			n_severeweather++;
		} else if (weather_list[yearposition].fireindex4 > firethresh3){
			// cout << "Extreme fire weather" << endl;
			n_extremeweather++;
		}
			
		// cout << "May fire index =" << weather_list[yearposition].fireindex5  << endl;
		if (weather_list[yearposition].fireindex5 < firethresh1){
			// cout << "No fire weather" << endl;
		} else if ((weather_list[yearposition].fireindex5 >= firethresh1) && (weather_list[yearposition].fireindex5 <= firethresh2)){
			// cout << "Mild fire weather" << endl;
			n_mildweather++;
		} else if ((weather_list[yearposition].fireindex5 > firethresh2) && (weather_list[yearposition].fireindex5 <= firethresh3)){
			// cout << "Severe fire weather" << endl;
			n_severeweather++;
		} else if (weather_list[yearposition].fireindex5 > firethresh3){
			// cout << "Extreme fire weather" << endl;
			n_extremeweather++;
		}

		// cout << "June fire index =" << weather_list[yearposition].fireindex6  << endl;
		if (weather_list[yearposition].fireindex6 < firethresh1){
			// cout << "No fire weather" << endl;
		} else if ((weather_list[yearposition].fireindex6 >= firethresh1) && (weather_list[yearposition].fireindex6 <= firethresh2)){
			// cout << "Mild fire weather" << endl;
			n_mildweather++;
		} else if ((weather_list[yearposition].fireindex6 > firethresh2) && (weather_list[yearposition].fireindex6 <= firethresh3)){
			// cout << "Severe fire weather" << endl;
			n_severeweather++;
		} else if (weather_list[yearposition].fireindex6 > firethresh3){
			// cout << "Extreme fire weather" << endl;
			n_extremeweather++;
		}

		// cout << "July fire index =" << weather_list[yearposition].fireindex7  << endl;
		if (weather_list[yearposition].fireindex7 < firethresh1){
			// cout << "No fire weather" << endl;
		} else if ((weather_list[yearposition].fireindex7 >= firethresh1) && (weather_list[yearposition].fireindex7 <= firethresh2)){
			// cout << "Mild fire weather" << endl;
			n_mildweather++;
		} else if ((weather_list[yearposition].fireindex7 > firethresh2) && (weather_list[yearposition].fireindex7 <= firethresh3)){
			// cout << "Severe fire weather" << endl;
			n_severeweather++;
		} else if (weather_list[yearposition].fireindex7 > firethresh3){
			// cout << "Extreme fire weather" << endl;
			n_extremeweather++;
		}

		// cout << "August fire index =" << weather_list[yearposition].fireindex8  << endl;
		if (weather_list[yearposition].fireindex8 < firethresh1){
			// cout << "No fire weather" << endl;
		} else if ((weather_list[yearposition].fireindex8 >= firethresh1) && (weather_list[yearposition].fireindex8 <= firethresh2)){
			// cout << "Mild fire weather" << endl;
			n_mildweather++;
		} else if ((weather_list[yearposition].fireindex8 > firethresh2) && (weather_list[yearposition].fireindex8 <= firethresh3)){
			// cout << "Severe fire weather" << endl;
			n_severeweather++;
		} else if (weather_list[yearposition].fireindex8 > firethresh3){
			// cout << "Extreme fire weather" << endl;
			n_extremeweather++;
		}

		// cout << "September fire index =" << weather_list[yearposition].fireindex9  << endl;
		if (weather_list[yearposition].fireindex9 < firethresh1){
			// cout << "No fire weather" << endl;
		} else if ((weather_list[yearposition].fireindex9 >= firethresh1) && (weather_list[yearposition].fireindex9 <= firethresh2)){
			// cout << "Mild fire weather" << endl;
			n_mildweather++;
		} else if ((weather_list[yearposition].fireindex9 > firethresh2) && (weather_list[yearposition].fireindex9 <= firethresh3)){
			// cout << "Severe fire weather" << endl;
			n_severeweather++;
		} else if (weather_list[yearposition].fireindex9 > firethresh3){
			// cout << "Extreme fire weather" << endl;
			n_extremeweather++;
		}

		// cout << "October fire index =" << weather_list[yearposition].fireindex10  << endl;
		if (weather_list[yearposition].fireindex10 < firethresh1){
			// cout << "No fire weather" << endl;
		} else if ((weather_list[yearposition].fireindex10 >= firethresh1) && (weather_list[yearposition].fireindex10 <= firethresh2)){
			// cout << "Mild fire weather" << endl;
			n_mildweather++;
		} else if ((weather_list[yearposition].fireindex10 > firethresh2) && (weather_list[yearposition].fireindex10 <= firethresh3)){
			// cout << "Severe fire weather" << endl;
			n_severeweather++;
		} else if (weather_list[yearposition].fireindex10 > firethresh3){
			// cout << "Extreme fire weather" << endl;
			n_extremeweather++;
		}

		// cout << "November fire index =" << weather_list[yearposition].fireindex11  << endl;
		if (weather_list[yearposition].fireindex11 < firethresh1){
			// cout << "No fire weather" << endl;
		} else if ((weather_list[yearposition].fireindex11 >= firethresh1) && (weather_list[yearposition].fireindex11 <= firethresh2)){
			// cout << "Mild fire weather" << endl;
			n_mildweather++;
		} else if ((weather_list[yearposition].fireindex11 > firethresh2) && (weather_list[yearposition].fireindex11 <= firethresh3)){
			// cout << "Severe fire weather" << endl;
			n_severeweather++;
		} else if (weather_list[yearposition].fireindex11 > firethresh3){
			// cout << "Extreme fire weather" << endl;
			n_extremeweather++;
		}

		// cout << "December fire index =" << weather_list[yearposition].fireindex12  << endl;
		if (weather_list[yearposition].fireindex12 < firethresh1){
			// cout << "No fire weather" << endl;
		} else if ((weather_list[yearposition].fireindex12 >= firethresh1) && (weather_list[yearposition].fireindex12 <= firethresh2)){
			// cout << "Mild fire weather" << endl;
			n_mildweather++;
		} else if ((weather_list[yearposition].fireindex12 > firethresh2) && (weather_list[yearposition].fireindex12 <= firethresh3)){
			// cout << "Severe fire weather" << endl;
			n_severeweather++;
		} else if (weather_list[yearposition].fireindex12 > firethresh3){
			// cout << "Extreme fire weather" << endl;
			n_extremeweather++;
		}
		
		// Calculation of annual FPR based on weather conditions
		double fireprobabilityrating = (n_mildweather*0.01 + n_severeweather*0.1 + n_extremeweather*1); // tried to include: / std::max(n_mildweather*0.005 + n_severeweather*0.01 + n_extremeweather*0.03);
		cout << "n_mildweather:" << (n_mildweather) << endl;	// can be removed
		cout << "n_severeweather:" << (n_severeweather) << endl;	// can be removed
		cout << "n_extremeweather:" << (n_extremeweather) << endl;	// can be removed
		cout << "Annual FPR:" << fireprobabilityrating << endl;	// can be removed
		
		
		
		
		
		// ##### Fire occurrence #####
		
		if (uniform.draw() < fireprobabilityrating) {	// if random number drawn between 0 and 1 is larger than the annual FPR, impact area is calculated
			cout << "+++ A wildfire occurs! +++" << endl;
			double xfirecenter = uniform.draw()*treecols;
			double yfirecenter = uniform.draw()*treerows;
			
			int i = yfirecenter* parameter[0].sizemagnif; // gridcell coordinate in envirgird
			int j = xfirecenter* parameter[0].sizemagnif; 
			double fireimpactareasize = fireprobabilityrating*30000; // determines affected area, rough estimate that average fire burns c. 2.5% of the total area (when using 2x2 km this is c. 10 cells of 90x90m)
			// cout << "Fire X coordinate:" << j << endl;	// can be removed
			// cout << "Fire Y coordinate:" << i << endl;	// can be removed
			cout << "Impacted area:" << fireimpactareasize << endl; // can be removed

			
			// check which gridcells fall into fire scar
			if (fireimpactareasize > 0) {  // if there is an impacted area
				// determine dimensions of the grid
				int xyquerrastpos = fireimpactareasize * parameter[0].sizemagnif;
				// determine shifted coordinates and adding up the density value
				for (int rastposi = (i + xyquerrastpos); rastposi > (i - (xyquerrastpos + 1)); rastposi--) {
					for (int rastposj = (j - xyquerrastpos); rastposj < (j + xyquerrastpos + 1); rastposj++) {
						if ((rastposi <= (int)((treerows - 1) * parameter[0].sizemagnif) && rastposi >= 0)
							&& (rastposj <= (int)((treecols - 1) * parameter[0].sizemagnif) && rastposj >= 0)) {  // TODO directly use in for loop boundaries
							// distance calculation to determine the influence of the density value in spatial units ...
							// ... and inserting the value at every position
							double entfrastpos = sqrt(pow(double(i - rastposi), 2) + pow(double(j - rastposj), 2));
							// only if the current grid cell is part of the influence area, a value is assigned
							if (entfrastpos <= (double)xyquerrastpos) {
								std::size_t curposii =
									static_cast<std::size_t>(rastposi) * static_cast<std::size_t>(treecols) * static_cast<std::size_t>(parameter[0].sizemagnif)
									+ static_cast<std::size_t>(rastposj);
								auto& cur_plot = plot_list[curposii];
								//cur_plot.fire == true;	// bool variant
								
								// ##### Fire impact #####
								// fire impact on grid cell
								// fireprobabilityrating // 0 to 1 .. 1 fire occurs definitely
								// cur_plot.Treedensityvalue // 0 to 10000 ... 10000 is very dense
								// cur_plot.envirfireimpact // 0 to 10000 ... 10000 is very dry
								
								// if ((fireprobabilityrating < 1/3) & 
									// (cur_plot.Treedensityvalue < 3333) & (cur_plot.envirfireimpact < 3333)) {
									// cur_plot.fire = 1;	// int variant => 0 no fire, 1 ground fire, 2 medium fire, 3 crown fire
									// cur_plot.litterheight = cur_plot.litterheight / 2;
								// } else if ((fireprobabilityrating < 2/3) & 
									// (cur_plot.Treedensityvalue >= 3333) & (cur_plot.envirfireimpact >= 3333)) {
									// cur_plot.fire = 2;
									// cur_plot.litterheight = cur_plot.litterheight / 4;									
								// } else if ((fireprobabilityrating >= 2/3) &
									// (cur_plot.Treedensityvalue >= 3333) & (cur_plot.envirfireimpact >= 3333)){
									// cur_plot.fire = 3;
									// cur_plot.litterheight = 0;									
								// } else {
									// cur_plot.fire = 1;
									// cur_plot.litterheight = cur_plot.litterheight / 2;
								// }
								
								// cout << "cur_plot.fire:" << cur_plot.fire << " --- cur_plot.litterheight:" << cur_plot.litterheight << endl;

								// Simple alternative for testing
								if (fireprobabilityrating < 0.1) {
									cur_plot.fire=1;	// int variant => 0 no fire, 1 ground fire, 2 medium fire, 3 crown fire
									cur_plot.litterheight = cur_plot.litterheight / 2;
								} else if ((fireprobabilityrating >= 0.1)&(fireprobabilityrating < 0.3)) {
									cur_plot.fire=2;
									cur_plot.litterheight = cur_plot.litterheight / 4;
								} else {
									cur_plot.fire=3;
									cur_plot.litterheight = 0;
								}
							}
						}
					}
				}

				// Tree's sensing of fire, code from plotupdate
				#pragma omp parallel for default(shared) private(uniform) schedule(guided)
				for (unsigned int tree_i = 0; tree_i < tree_list.size(); ++tree_i) {
					auto& tree = tree_list[tree_i];

					if(tree.growing == true) {
						int i = tree.ycoo * parameter[0].sizemagnif / 1000;
						int j = tree.xcoo * parameter[0].sizemagnif / 1000;

						if (parameter[0].densitymode == 1) {
							tree.densitywert = 0.0;
						} else {
							double impactareasize = 0.0;
							if (parameter[0].calcinfarea == 1)  // linearly increasing
								impactareasize = tree.dbasal * parameter[0].incfac / 100.0;
							else if (parameter[0].calcinfarea == 2)  // linearly increasing
								impactareasize = tree.dbasal * (2 / 3) * parameter[0].incfac / 100.0;
							else if (parameter[0].calcinfarea == 3)  // linearly increasing
								impactareasize = tree.dbasal * (4 / 3) * parameter[0].incfac / 100.0;
							else if (parameter[0].calcinfarea == 4)  // logistic growth function with maximum at 8 m
								impactareasize = (9 / (1 + (((1 / 0.1) - 1) * exp(-0.1 * tree.dbasal)))) - 1;
							else if (parameter[0].calcinfarea == 5)  // logistic growth function with maximum at 8 m
								impactareasize = (9 / (1 + (((1 / 0.1) - 1) * exp(-0.2 * tree.dbasal)))) - 1;
							else if (parameter[0].calcinfarea == 6)  // logistic growth function with maximum at 8 m
								impactareasize = (9 / (1 + (((1 / 0.1) - 1) * exp(-0.5 * tree.dbasal)))) - 1;

							// if the tree only influences one grid cell
							if (impactareasize < (1.0 / parameter[0].sizemagnif)) {
								const std::size_t curposi = static_cast<std::size_t>(i) * static_cast<std::size_t>(treecols) * static_cast<std::size_t>(parameter[0].sizemagnif)
															+ static_cast<std::size_t>(j);
								auto& cur_plot = plot_list[curposi];

								// fire sensing
								tree.firedamage = cur_plot.fire;	//Since tree is within one gridcell, it can take that cell's value directly (needs to be divided by 3)
							} else {  // ... if the tree influences more than one section
								// determine dimensions of the considered grid around a tree
								int xyquerrastpos = impactareasize * parameter[0].sizemagnif;

								// fire sensing
								unsigned int firedamage = 0;

								for (int rastposi = (i + xyquerrastpos); rastposi > (i - (xyquerrastpos + 1)); rastposi--) {
									for (int rastposj = (j - xyquerrastpos); rastposj < (j + xyquerrastpos + 1); rastposj++) {
										if ((rastposi <= ((int)(treerows - 1) * parameter[0].sizemagnif) && rastposi >= 0)
											&& (rastposj <= ((int)(treecols - 1) * parameter[0].sizemagnif) && rastposj >= 0)) {
											double entfrastpos = sqrt(pow(double(i - rastposi), 2) + pow(double(j - rastposj), 2));
											if (entfrastpos <= (double)xyquerrastpos) {
												int icurr = rastposi;
												int jcurr = rastposj;

												std::size_t curposii =
													static_cast<std::size_t>(icurr) * static_cast<std::size_t>(treecols) * static_cast<std::size_t>(parameter[0].sizemagnif)
													+ static_cast<std::size_t>(jcurr);
												auto& cur_plot = plot_list[curposii];

												// fire sensing
												if(cur_plot.fire > firedamage)
													firedamage = cur_plot.fire;	// If tree covers multiple gridcells, it will take the highest cur_plot.fire value from these for now
											}
										}
									}
								}
								
								// fire sensing
								// tree.firedamage = firedamage;
								// if (tree.firedamage > 0) {
									// cout <<  " | firedamage baum von cur_plot.fire = " << tree.firedamage << "  ";
								// }
							}
						}
					}
				}
				
				
				
			// +++++ Seed mortality test 1 (all seeds) +++++
				
			// #pragma omp for schedule(guided)
			// for (unsigned int i = 0; i < seed_list.size(); ++i) {
				// auto& seed = seed_list[i];
				// if (!seed.dead) {
					// seed.dead = true;
					// seed_list.remove(i);
					// cout << "Coordinates of dead seed: seed.xcoo: " << seed.xcoo << ", seed.ycoo: " << seed.ycoo << endl;
					// }
				// }
						
						
						
			// +++++ Seed mortality test 2 (depending on cur_plot.fire value) +++++				
			
				#pragma omp for schedule(guided)
				for (unsigned int i = 0; i < seed_list.size(); ++i) {
					auto& seed = seed_list[i];
					if (!seed.dead && !seed.incone) {
						seed.dead = true;
						seed_list.remove(i);
						// cout << "seed1";
					} else if (!seed.dead && seed.incone == true) {
						int i = seed.ycoo * parameter[0].sizemagnif / 1000;
						int j = seed.xcoo * parameter[0].sizemagnif / 1000;
						const std::size_t curposi = static_cast<std::size_t>(i) * static_cast<std::size_t>(treecols) * static_cast<std::size_t>(parameter[0].sizemagnif)
													+ static_cast<std::size_t>(j);
						auto& cur_plot = plot_list[curposi];
						if (cur_plot.fire == 1) {
							if (uniform.draw() < 1/3){
								seed.dead = true;
								seed_list.remove(i);
								// cout << "seed2";
							}
						} else if (cur_plot.fire == 2) {
							if (uniform.draw() < 2/3){
								seed.dead = true;
								seed_list.remove(i);
								// cout << "seed3";
							}
						} else {
							seed.dead = true;
							seed_list.remove(i);
						}
												
					}
				}
	
			
			}
		}
	}
}