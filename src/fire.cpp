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
		
				
		// ##################################	
		// ########## Fire weather ##########################################################################################################
		// ##################################
		
		// Based on fire probability rating (FPR) assigned to each month from linear model between T/P and observed MODIS fire pixels
		// FPR is basically the logarithmic predicted amount of MODIS pixels burning under the respective T/P conditions during a given month
		
		unsigned short int n_mildweather = 0;	// holding the number of occurences of mild/severe/extreme monthly fire weather
		unsigned short int n_severeweather = 0;
		unsigned short int n_extremeweather = 0;
		
		// Used for Lake Satagay localization 
		unsigned short int firethresh1 = 6.6;	// determine thresholds that decide monthly fire weather severity
		unsigned short int firethresh2 = 6.85;
		unsigned short int firethresh3 = 7.1;
		
		// Used for Lake Khamra localization
		// unsigned short int firethresh1 = 4.0;	// determine thresholds that decide monthly fire weather severity (previous values: 3.48, 6.9, 8.3)
		// unsigned short int firethresh2 = 6.5;
		// unsigned short int firethresh3 = 7.5;
		
		// cout << "January fire index =" << weather_list[yearposition].fireindex1  << endl;
		if (weather_list[yearposition].fireindex1 < firethresh1){	// print fire weather severity depending on calculated FPR, and if fire weather occurs increase counter of fire weather occurences 
		} else if ((weather_list[yearposition].fireindex1 >= firethresh1) && (weather_list[yearposition].fireindex1 <= firethresh2)){
			n_mildweather++;
		} else if ((weather_list[yearposition].fireindex1 > firethresh2) && (weather_list[yearposition].fireindex1 <= firethresh3)){
			n_severeweather++;
		} else if (weather_list[yearposition].fireindex1 > firethresh3){
			n_extremeweather++;
		}
			
		// cout << "February fire index =" << weather_list[yearposition].fireindex2  << endl;
		if (weather_list[yearposition].fireindex2 < firethresh1){
		} else if ((weather_list[yearposition].fireindex2 >= firethresh1) && (weather_list[yearposition].fireindex2 <= firethresh2)){
			n_mildweather++;
		} else if ((weather_list[yearposition].fireindex2 > firethresh2) && (weather_list[yearposition].fireindex2 <= firethresh3)){
			n_severeweather++;
		} else if (weather_list[yearposition].fireindex2 > firethresh3){
			n_extremeweather++;
		}
			
		// cout << "March fire index =" << weather_list[yearposition].fireindex3  << endl;
		if (weather_list[yearposition].fireindex3 < firethresh1){
		} else if ((weather_list[yearposition].fireindex3 >= firethresh1) && (weather_list[yearposition].fireindex3 <= firethresh2)){
			n_mildweather++;
		} else if ((weather_list[yearposition].fireindex3 > firethresh2) && (weather_list[yearposition].fireindex3 <= firethresh3)){
			n_severeweather++;
		} else if (weather_list[yearposition].fireindex3 > firethresh3){
			n_extremeweather++;
		}
			
		// cout << "April fire index =" << weather_list[yearposition].fireindex4  << endl;
		if (weather_list[yearposition].fireindex4 < firethresh1){
		} else if ((weather_list[yearposition].fireindex4 >= firethresh1) && (weather_list[yearposition].fireindex4 <= firethresh2)){
			n_mildweather++;
		} else if ((weather_list[yearposition].fireindex4 > firethresh2) && (weather_list[yearposition].fireindex4 <= firethresh3)){
			n_severeweather++;
		} else if (weather_list[yearposition].fireindex4 > firethresh3){
			n_extremeweather++;
		}
			
		// cout << "May fire index =" << weather_list[yearposition].fireindex5  << endl;
		if (weather_list[yearposition].fireindex5 < firethresh1){
		} else if ((weather_list[yearposition].fireindex5 >= firethresh1) && (weather_list[yearposition].fireindex5 <= firethresh2)){
			n_mildweather++;
		} else if ((weather_list[yearposition].fireindex5 > firethresh2) && (weather_list[yearposition].fireindex5 <= firethresh3)){
			n_severeweather++;
		} else if (weather_list[yearposition].fireindex5 > firethresh3){
			n_extremeweather++;
		}

		// cout << "June fire index =" << weather_list[yearposition].fireindex6  << endl;
		if (weather_list[yearposition].fireindex6 < firethresh1){
		} else if ((weather_list[yearposition].fireindex6 >= firethresh1) && (weather_list[yearposition].fireindex6 <= firethresh2)){
			n_mildweather++;
		} else if ((weather_list[yearposition].fireindex6 > firethresh2) && (weather_list[yearposition].fireindex6 <= firethresh3)){
			n_severeweather++;
		} else if (weather_list[yearposition].fireindex6 > firethresh3){
			n_extremeweather++;
		}

		// cout << "July fire index =" << weather_list[yearposition].fireindex7  << endl;
		if (weather_list[yearposition].fireindex7 < firethresh1){
		} else if ((weather_list[yearposition].fireindex7 >= firethresh1) && (weather_list[yearposition].fireindex7 <= firethresh2)){
			n_mildweather++;
		} else if ((weather_list[yearposition].fireindex7 > firethresh2) && (weather_list[yearposition].fireindex7 <= firethresh3)){
			n_severeweather++;
		} else if (weather_list[yearposition].fireindex7 > firethresh3){
			n_extremeweather++;
		}

		// cout << "August fire index =" << weather_list[yearposition].fireindex8  << endl;
		if (weather_list[yearposition].fireindex8 < firethresh1){
		} else if ((weather_list[yearposition].fireindex8 >= firethresh1) && (weather_list[yearposition].fireindex8 <= firethresh2)){
			n_mildweather++;
		} else if ((weather_list[yearposition].fireindex8 > firethresh2) && (weather_list[yearposition].fireindex8 <= firethresh3)){
			n_severeweather++;
		} else if (weather_list[yearposition].fireindex8 > firethresh3){
			n_extremeweather++;
		}

		// cout << "September fire index =" << weather_list[yearposition].fireindex9  << endl;
		if (weather_list[yearposition].fireindex9 < firethresh1){
		} else if ((weather_list[yearposition].fireindex9 >= firethresh1) && (weather_list[yearposition].fireindex9 <= firethresh2)){
			n_mildweather++;
		} else if ((weather_list[yearposition].fireindex9 > firethresh2) && (weather_list[yearposition].fireindex9 <= firethresh3)){
			n_severeweather++;
		} else if (weather_list[yearposition].fireindex9 > firethresh3){
			n_extremeweather++;
		}

		// cout << "October fire index =" << weather_list[yearposition].fireindex10  << endl;
		if (weather_list[yearposition].fireindex10 < firethresh1){
		} else if ((weather_list[yearposition].fireindex10 >= firethresh1) && (weather_list[yearposition].fireindex10 <= firethresh2)){
			n_mildweather++;
		} else if ((weather_list[yearposition].fireindex10 > firethresh2) && (weather_list[yearposition].fireindex10 <= firethresh3)){
			n_severeweather++;
		} else if (weather_list[yearposition].fireindex10 > firethresh3){
			n_extremeweather++;
		}

		// cout << "November fire index =" << weather_list[yearposition].fireindex11  << endl;
		if (weather_list[yearposition].fireindex11 < firethresh1){
		} else if ((weather_list[yearposition].fireindex11 >= firethresh1) && (weather_list[yearposition].fireindex11 <= firethresh2)){
			n_mildweather++;
		} else if ((weather_list[yearposition].fireindex11 > firethresh2) && (weather_list[yearposition].fireindex11 <= firethresh3)){
			n_severeweather++;
		} else if (weather_list[yearposition].fireindex11 > firethresh3){
			n_extremeweather++;
		}

		// cout << "December fire index =" << weather_list[yearposition].fireindex12  << endl;
		if (weather_list[yearposition].fireindex12 < firethresh1){
		} else if ((weather_list[yearposition].fireindex12 >= firethresh1) && (weather_list[yearposition].fireindex12 <= firethresh2)){
			n_mildweather++;
		} else if ((weather_list[yearposition].fireindex12 > firethresh2) && (weather_list[yearposition].fireindex12 <= firethresh3)){
			n_severeweather++;
		} else if (weather_list[yearposition].fireindex12 > firethresh3){
			n_extremeweather++;
		}
		
		// Initializing fireprobabilityrating
		double fireprobabilityrating = 0.0;
		
		fireprobabilityrating = (n_mildweather*0.04 + ((n_severeweather*0.5)/6) + n_extremeweather*0.5); // Calculation of annual FPR based on monthly weather conditions
		
		if (fireprobabilityrating > 1) {
			fireprobabilityrating = 1;
		}
		
		weather_list[yearposition].FPR = fireprobabilityrating;
		cout << endl << "\tn_mildweather:" << (n_mildweather) << endl;	// optional output during simulation
		cout << "\tn_severeweather:" << (n_severeweather) << endl;	// optional output during simulation
		cout << "\tn_extremeweather:" << (n_extremeweather) << endl;	// optional output during simulation
		cout << "\tAnnual FPR:" << fireprobabilityrating << endl;	// optional output during simulation
		

		// ####################################	
		// ########## Fire occurence ########################################################################################################
		// ####################################
		
		bool ignition = false;
		
		// Decide if fire is ignited (depending on firemode and fireprobabilityrating)
		if (parameter[0].firemode == 112) {
			if (uniform.draw() < fireprobabilityrating) {
				ignition = true;
			} else {
				ignition = false;
			}
		} else if (parameter[0].firemode == 0) {
			ignition = false;
		} else if ((parameter[0].firemode > 0) & (parameter[0].firemode != 112)) {
			if (parameter[0].ivort % parameter[0].firemode == 0) {
				ignition = true;
			} else {
				ignition = false;
				cout << "\tFire ignitions can't occur in this year!" << endl;
			}
		}
		
		// Set fire size and assign impacted area 
		if ((ignition == true)) {
			cout << "+++ A wildfire occurs! +++" << endl;
			double xfirecenter = uniform.draw()*treecols;
			double yfirecenter = uniform.draw()*treerows;
			
			double fireimpactareasize = 0.0;
			
			if (parameter[0].firemode == 112) {
				fireimpactareasize = fireprobabilityrating * treecols; // Relative area of plot depnding on fire intensity
			} else if (parameter[0].firemode == 0) {
				fireimpactareasize = 0.0;
				} else if ((parameter[0].firemode > 0) & (parameter[0].firemode != 112)) {
				fireimpactareasize = 5 * treecols;
				}
			
			int i = yfirecenter* parameter[0].sizemagnif; // gridcell coordinate in envirgird
			int j = xfirecenter* parameter[0].sizemagnif; 
			// double fireimpactareasize = fireprobabilityrating * (treecols*treerows); // determines affected area; currently depending on FPR range (0-1) - so if FPR = 0.1, then 10% of the area burn.
			// cout << "Fire X coordinate:" << j << endl;	// optional output during simulation
			// cout << "Fire Y coordinate:" << i << endl;	// optional output during simulation
			// cout << "\tImpacted area: " << printf("%.2f", (pi * pow(2.0, fireimpactareasize))) << " m2 or " << printf("%.0f",((pi * pow(2.0, fireimpactareasize))/(treecols*treerows)))*100 << "% of total plot area" << endl; // optional output during simulation
			// cout << "Total area (m2) from treecols*treerows: " << treecols*treerows << endl; // optional output during simulation

			// check which gridcells fall into fire scar
			if (fireimpactareasize > 0) {  // if there is an impacted area
				// determine dimensions of the grid
				int xyquerrastpos = fireimpactareasize * parameter[0].sizemagnif;
				// determine shifted coordinates and adding up the density value
				for (int rastposi = (i + xyquerrastpos); rastposi > (i - (xyquerrastpos + 1)); rastposi--) {
					for (int rastposj = (j - xyquerrastpos); rastposj < (j + xyquerrastpos + 1); rastposj++) {
						if ((rastposi <= (int)((treerows - 1) * parameter[0].sizemagnif) && rastposi >= 0)
							&& (rastposj <= (int)((treecols - 1) * parameter[0].sizemagnif) && rastposj >= 0)) {
							// distance calculation to determine the influence of the density value in spatial units ...
							// ... and inserting the value at every position
							double entfrastpos = sqrt(pow(double(i - rastposi), 2) + pow(double(j - rastposj), 2));
							// only if the current grid cell is part of the influence area, a value is assigned
							if (entfrastpos <= (double)xyquerrastpos) {
								std::size_t curposii =
									static_cast<std::size_t>(rastposi) * static_cast<std::size_t>(treecols) * static_cast<std::size_t>(parameter[0].sizemagnif)
									+ static_cast<std::size_t>(rastposj);
								auto& cur_plot = plot_list[curposii];
								
								// ##################################################
								// ##### Fire intensity and litter layer impact #####
								// ##################################################
								
								// Fire impact on grid cell, including these variables:
								// fireprobabilityrating // 0 to 1 -> 0 = no fire; 1 = fire occurs definitely
								// cur_plot.fire // 0 to 1 -> 0 = no fire impact, 1 = highest fire intensity
								// cur_plot.Treedensityvalue // 0 to 10000 -> 0 = no trees; 10000 = very dense
								// cur_plot.envirfireimpact // 0 to 10000 -> as calculated in main.cpp, 0 = fully wet; 10000 = very dry // ##########CHECK##########
								
								// Set FPR as cell fire intensity and scale by TWI (only of firemode == 112)
								if (parameter[0].firemode == 112) {
									cur_plot.fire = pow(fireprobabilityrating, ((cur_plot.twi / 100) / 12.5));
								} else if ((parameter[0].firemode > 0) & (parameter[0].firemode  != 112)) {
									cur_plot.fire = 1;
								}
								// If a custom, fixed fire intensity was selected, apply that instead
								if (parameter[0].fireintensitymode != 1.0) {
									cur_plot.fire = parameter[0].fireintensitymode;
									//cout << " - Fire intensity: " << cur_plot.fire << endl;	// optional output during simulation
								}
								
								// Fire intensity stays between 0-1
								if (cur_plot.fire > 1) {
									cur_plot.fire = 1;
								}
								
								// Fire damage on litter layer
								cur_plot.litterheight0 = cur_plot.litterheight0 - (cur_plot.litterheight0 * cur_plot.fire);
								
							}
						}
					}
				}		
				
				// ################################
				// ##### Fire impact on trees #####
				// ################################
				
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
								tree.firedamage = cur_plot.fire;	// Since tree is within one gridcell, it can take that cell's value directly
								
								// if (tree.firedamage > 0) {
									// cout <<  " +++ bark = " << speciestrait[tree.species].relbarkthickness <<  "& firedamage = " << tree.firedamage;
								// }
								
							} else {  // ... if the tree influences more than one section
								// determine dimensions of the considered grid around a tree
								int xyquerrastpos = impactareasize * parameter[0].sizemagnif;

								// fire sensing
								unsigned firedamage = 0;
								unsigned int firedamagecells = 0;

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
												firedamage = firedamage + cur_plot.fire;	// If tree covers multiple gridcells, it will sum up all cur_plot.fire values
												++firedamagecells;
												
											}
										}
									}
								}
								
								// fire sensing
								tree.firedamage = firedamage / (unsigned short) firedamagecells;	// Tree gets assigned the mean cur_plot.fire value of all included gridcells
							}
							
							// Fire intensity mediated by bark thickness (only if firemode == 112, otherwise if firemode > 0 it's always 1 - OR if custom fireintensity is given)
							if (parameter[0].firemode == 112 || parameter[0].fireintensitymode != 1.0) {
								tree.firedamage = tree.firedamage - (0.1 * speciestrait[tree.species].relbarkthickness / 3);	
							} else if ((parameter[0].firemode > 0) & (parameter[0].firemode != 112)) {
								tree.firedamage = 1.0;
							}
							
							// Fire intensity stays between 0-1							
							if (tree.firedamage <= 0) {
								tree.firedamage = 0;
							}
							
							// Flame height depending on tree fire damage (only if firemode == 112 OR if custom fireintensity is given)
							if (parameter[0].firemode == 112 || parameter[0].fireintensitymode != 1.0) {
								tree.crownstart = tree.firedamage * 5000 * 10; // flames reach 5000 cm high
								tree.relcrowndamage = ((tree.crownstart / 10) / (tree.height / 10)) * 1000;
							} else {
								tree.relcrowndamage = 0.0;
							}	
						}
					}
				}
				
				
			// ################################
			// ##### Fire impact on seeds #####
			// ################################
										
			// Seeds die depending on location and fire intensity			
				
				#pragma omp for schedule(guided)
				for (unsigned int i = 0; i < seed_list.size(); ++i) {
                auto& seed = seed_list[i];
					if (!seed.dead) {
						int i = seed.ycoo * parameter[0].sizemagnif / 1000;
						int j = seed.xcoo * parameter[0].sizemagnif / 1000;
						const std::size_t curposi = static_cast<std::size_t>(i) * static_cast<std::size_t>(treecols) * static_cast<std::size_t>(parameter[0].sizemagnif) 
													+ static_cast<std::size_t>(j);
						auto& cur_plot = plot_list[curposi];
						if (!seed.incone) {
							if (cur_plot.fire > 0) {
								seed.dead = true;
								seed_list.remove(i);
								//cout << "seed died. ";
							} else {
								seed.dead = false;
								//cout << "seed survived. ";
							}
						} else {
							if (cur_plot.fire > 0) {
								if (uniform.draw() <= cur_plot.fire){
									seed.dead = true;
									seed_list.remove(i);
									//cout << "some cones died. ";
								} else {
									seed.dead = false;
									//cout << "cones survived. ";
								}
							}
						}
					}
				}
			}
		}
	}
}	