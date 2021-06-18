#include "LAVESI.h"
#include "RandomNumber.h"
#include "VectorList.h"

using namespace std;

void AddTreeDensity(VectorList<Tree>& tree_list, vector<Envirgrid>& plot_list) {
#pragma omp parallel for default(shared) schedule(guided)
    for (unsigned int tree_i = 0; tree_i < tree_list.size(); ++tree_i) {
        auto& tree = tree_list[tree_i];

		if (tree.growing == true) {
			int i = tree.ycoo * parameter[0].sizemagnif / 1000;
			int j = tree.xcoo * parameter[0].sizemagnif / 1000;

			// assess the size of the area the current tree influences
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

			if (impactareasize < (1.0 / parameter[0].sizemagnif)) {  // if the trees influences only one density grid cell
				const std::size_t curposi = static_cast<std::size_t>(i) * static_cast<std::size_t>(treecols) * static_cast<std::size_t>(parameter[0].sizemagnif)
											+ static_cast<std::size_t>(j);
				auto& cur_plot = plot_list[curposi];

				cur_plot.Treedensityvalue += 10000
											 * pow(pow(impactareasize / (1.0 / parameter[0].sizemagnif), parameter[0].densitysmallweighing)
													   // weighing with diameter
													   * pow(tree.dbasal, parameter[0].densitytreetile),
												   parameter[0].densityvaluemanipulatorexp);
				++cur_plot.Treenumber;
				tree.densitywert =
					pow(pow(tree.dbasal, parameter[0].densitytreetile) * pow(impactareasize / (1.0 / parameter[0].sizemagnif), parameter[0].densitysmallweighing),
						parameter[0].densityvaluemanipulatorexp);
			} else {  // if the trees influences more than one density grid cell
				// determine dimensions of the grid around the tree
				int xyquerrastpos = impactareasize * parameter[0].sizemagnif;
				// determine shifted coordinates and adding up the density value
				double sumdensitywert = 0;
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
								const auto density =
									std::pow(std::pow(tree.dbasal, parameter[0].densitytreetile) / (entfrastpos + 1.0), parameter[0].densityvaluemanipulatorexp);
								cur_plot.Treedensityvalue += 10000 * density;
								++cur_plot.Treenumber;
								sumdensitywert += density;
							}
						}
					}
				}
				tree.densitywert = sumdensitywert;
			}
		}
    }  // each tree
}

void IndividualTreeDensity(VectorList<Tree>& tree_list, vector<Envirgrid>& plot_list) {
    RandomNumber<double> uniform(0, 1);
#pragma omp parallel for default(shared) private(uniform) schedule(guided)
    for (unsigned int tree_i = 0; tree_i < tree_list.size(); ++tree_i) {
        auto& tree = tree_list[tree_i];

		if (tree.growing == true) {
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

					if (parameter[0].densitymode == 2) {
						if (cur_plot.Treedensityvalue > 0) {
							if (parameter[0].densitytiletree == 1)  // sum of values
							{
								tree.densitywert = (1.0 - (tree.densitywert / ((double)cur_plot.Treedensityvalue / 10000)));
								//                           density_desired(at position) / density_currently(at position)
							} else if (parameter[0].densitytiletree == 2)  // multiplication of values
							{
								tree.densitywert = (1.0 - (tree.densitywert / (((double)cur_plot.Treedensityvalue / 10000) * tree.densitywert)));
								//                         density_desired(at position) / density_currently(at position)
							}
						} else {
							tree.densitywert = 0.0;  // no competition effect
						}
					} else if (parameter[0].densitymode == 3) {
						// determine the grid position randomly
						int izuf = (treerows - 1) * parameter[0].sizemagnif * uniform.draw();
						int jzuf = (treecols - 1) * parameter[0].sizemagnif * uniform.draw();

						std::size_t curposii =
							static_cast<std::size_t>(izuf) * static_cast<std::size_t>(treecols) * static_cast<std::size_t>(parameter[0].sizemagnif)
							+ static_cast<std::size_t>(jzuf);
						auto& cur_plot = plot_list[curposii];

						if (cur_plot.Treedensityvalue > 0) {
							if (parameter[0].densitytiletree == 1)  // sum of values
							{
								tree.densitywert = (1.0 - (tree.densitywert / ((double)cur_plot.Treedensityvalue / 10000)));
								//                           density_desired(at position) / density_currently(at position)
							} else if (parameter[0].densitytiletree == 2)  // multiplication of values
							{
								tree.densitywert = (1.0 - (tree.densitywert / (((double)cur_plot.Treedensityvalue / 10000) * tree.densitywert)));
							}
						} else {
							tree.densitywert = 0.0;
						}
					}

					// calculate the influence of the thawing depth on the tree growth
					if ( (parameter[0].thawing_depth == true) && (cur_plot.maxthawing_depth < speciestrait[tree.species].minactivelayer*10) ) {
						tree.thawing_depthinfluence = (unsigned short)((200.0 / (speciestrait[tree.species].minactivelayer*10)) * (double)cur_plot.maxthawing_depth);
					} else {
						tree.thawing_depthinfluence = 100;
					}
					
					// dem sensing
					if (parameter[0].demlandscape && (cur_plot.elevation < 32767)) {  // dem sensing
						tree.elevation = cur_plot.elevation;
						tree.envirimpact = cur_plot.envirgrowthimpact;
					}
					// assess humidity which influences growth
					tree.soilhumidity = cur_plot.soilhumidity;
					tree.twi = cur_plot.twi;
					
				} else {  // ... if the tree influences more than one section
					// determine dimensions of the considered grid around a tree
					int xyquerrastpos = impactareasize * parameter[0].sizemagnif;
					double maxdist = sqrt(pow(double(xyquerrastpos), 2) + pow(double(xyquerrastpos), 2));

					// determine rescaled coordinates and summation of the density value
					double sumdensitywert = 0;
					double sumthawing_depth = 0;
					unsigned int anzahlflaechen = 0;

					double sumelevation = 0;
					double sumenvirgrowthimpact = 0;
					double sumsoilhumidity = 0;
					double sumtwi = 0;
					unsigned int countelevation = 0;

					for (int rastposi = (i + xyquerrastpos); rastposi > (i - (xyquerrastpos + 1)); rastposi--) {
						for (int rastposj = (j - xyquerrastpos); rastposj < (j + xyquerrastpos + 1); rastposj++) {
							if ((rastposi <= ((int)(treerows - 1) * parameter[0].sizemagnif) && rastposi >= 0)
								&& (rastposj <= ((int)(treecols - 1) * parameter[0].sizemagnif) && rastposj >= 0)) {
								double entfrastpos = sqrt(pow(double(i - rastposi), 2) + pow(double(j - rastposj), 2));

								if (entfrastpos <= (double)xyquerrastpos) {
									int icurr = rastposi;
									int jcurr = rastposj;

									if (parameter[0].densitymode == 3) {  // determine the position of density values randomly
										icurr = (treerows - 1) * parameter[0].sizemagnif * uniform.draw();
										jcurr = (treecols - 1) * parameter[0].sizemagnif * uniform.draw();
									}

									std::size_t curposii =
										static_cast<std::size_t>(icurr) * static_cast<std::size_t>(treecols) * static_cast<std::size_t>(parameter[0].sizemagnif)
										+ static_cast<std::size_t>(jcurr);
									auto& cur_plot = plot_list[curposii];

									if (parameter[0].densitytiletree == 1) {
										sumdensitywert += ((double)cur_plot.Treedensityvalue / 10000) * (1 - entfrastpos / maxdist);
										// added the values influence as becoming weaker in the periphery, otherwise the density value influence would be
										// overestimated
									} else if (parameter[0].densitytiletree == 2) {
										// after weighting the additional values by the individual influence values the offset is added
										sumdensitywert +=
											(((double)cur_plot.Treedensityvalue / 10000) - pow(tree.dbasal, parameter[0].densitytreetile) / (entfrastpos + 1.0))
												* pow(tree.dbasal, parameter[0].densitytreetile) / (entfrastpos + 1.0)
											+ pow(tree.dbasal, parameter[0].densitytreetile) / (entfrastpos + 1.0);
									}

									sumthawing_depth += (double)cur_plot.maxthawing_depth;
									anzahlflaechen++;

									if (parameter[0].demlandscape && (cur_plot.elevation < 32767)) {  // dem sensing
										sumelevation += (double)cur_plot.elevation / 10;
										sumenvirgrowthimpact += (double)cur_plot.envirgrowthimpact / 10000;
										countelevation++;
									}
									
									// assess humidity which influences growth
									sumsoilhumidity += (double)cur_plot.soilhumidity;
									sumtwi += (double)cur_plot.twi;
								}
							}
						}
					}

					if (sumdensitywert > 0.0)
						tree.densitywert = 1.0 - (tree.densitywert / sumdensitywert);
					else
						tree.densitywert = 0.0;

					sumthawing_depth /= anzahlflaechen;
					if (sumthawing_depth < (speciestrait[tree.species].minactivelayer*10))
						tree.thawing_depthinfluence = (unsigned short)((200.0 / (speciestrait[tree.species].minactivelayer*10)) * sumthawing_depth);
					else
						tree.thawing_depthinfluence = 100;

					if (parameter[0].demlandscape) {  // dem sensing by mean value of gridcells in range
						tree.elevation = 10 * sumelevation / (double)countelevation;
						tree.envirimpact = 10000 * sumenvirgrowthimpact / (double)countelevation;
					}

					// assess humidity which influences growth
					sumsoilhumidity /= anzahlflaechen;
					tree.soilhumidity = sumsoilhumidity;
					sumtwi /= anzahlflaechen;
					tree.twi = sumtwi;
				}

				tree.densitywert = tree.densitywert
								   * pow((1.0 - (0.01 / tree.dbasal)),
										 parameter[0].densityvaluedbasalinfluence);  // increasing influence by increasing tree height

				if (parameter[0].dichtheightrel == 1) {
					tree.densitywert = tree.densitywert * (-1.0 / 130.0 * (double)tree.height / 10 + 1.0);
				} else if (parameter[0].dichtheightrel == 2) {
					tree.densitywert = tree.densitywert * (-1.0 / 200.0 * (double)tree.height / 10 + 1.0);
				} else if (parameter[0].dichtheightrel == 3) {
					double hrelbuf = (-1.0 / 200.0 * (double)tree.height / 10 + 1.0);
					if (hrelbuf < 0.1)
						hrelbuf = 0.1;
					tree.densitywert = tree.densitywert * hrelbuf;
				} else if (parameter[0].dichtheightrel == 4) {
					if ((double)tree.height / 10 < 40)
						tree.densitywert = 1.0;
					else if (((double)tree.height / 10 >= 40) && ((double)tree.height / 10 < 200))
						tree.densitywert = 0.5;
					else if ((double)tree.height / 10 >= 200)
						tree.densitywert = 0.0;
				} else if (parameter[0].dichtheightrel == 5) {
					if ((double)tree.height / 10 < 40)
						tree.densitywert = 1.0;
					else if (((double)tree.height / 10 >= 40) && ((double)tree.height / 10 < 200))
						tree.densitywert = 0.55;
					else if ((double)tree.height / 10 >= 200)
						tree.densitywert = 0.1;
				} else if (parameter[0].dichtheightrel == 6) {
					if ((double)tree.height / 10 < 40)
						tree.densitywert = 1.0;
					else if (((double)tree.height / 10 >= 40) && ((double)tree.height / 10 < 200))
						tree.densitywert = 0.9;
					else if ((double)tree.height / 10 >= 200)
						tree.densitywert = 0.8;
				} else if (parameter[0].dichtheightrel == 10) {  // added to fit the height classes distribution properly
					// density value decreases by increasing tree height linearly
					double densitywertminimum = 0.1;
					double ageseinfluss = (-1 * (1 - densitywertminimum) / 100.0 * (double)tree.age + 1.0);
					if (ageseinfluss < densitywertminimum) {
						ageseinfluss = densitywertminimum;
					}
					tree.densitywert = tree.densitywert * ageseinfluss;
				} else if (parameter[0].dichtheightrel == 11) {
					double densitywertminimum = 0.5;
					double ageseinfluss = (-1 * (1 - densitywertminimum) / 100.0 * (double)tree.age + 1.0);
					if (ageseinfluss < densitywertminimum) {
						ageseinfluss = densitywertminimum;
					}
					tree.densitywert = tree.densitywert * ageseinfluss;
				} else if (parameter[0].dichtheightrel == 12) {
					double densitywertminimum = 0.1;
					double ageseinfluss = (-1 * (1 - densitywertminimum) / 50.0 * (double)tree.age + 1.0);
					if (ageseinfluss < densitywertminimum) {
						ageseinfluss = densitywertminimum;
					}
					tree.densitywert = tree.densitywert * ageseinfluss;
				} else if (parameter[0].dichtheightrel == 13) {
					double densitywertminimum = 0.5;
					double ageseinfluss = (-1 * (1 - densitywertminimum) / 50.0 * (double)tree.age + 1.0);
					if (ageseinfluss < densitywertminimum) {
						ageseinfluss = densitywertminimum;
					}
					tree.densitywert = tree.densitywert * ageseinfluss;
				} else if (parameter[0].dichtheightrel == 20) {
					double densitywertminimum = 0.1;
					double ageseinfluss = (1.0 / pow(((double)tree.age + 1.0), 0.5));
					if (ageseinfluss < densitywertminimum) {
						ageseinfluss = densitywertminimum;
					}
					tree.densitywert = tree.densitywert * ageseinfluss;

				} else if (parameter[0].dichtheightrel == 21) {
					double densitywertminimum = 0.5;
					double ageseinfluss = (1.0 / pow(((double)tree.age + 1.0), 0.15));
					if (ageseinfluss < densitywertminimum) {
						ageseinfluss = densitywertminimum;
					}
					tree.densitywert = tree.densitywert * ageseinfluss;

				} else if (parameter[0].dichtheightrel == 22) {
					double densitywertminimum = 0.1;
					double ageseinfluss = (1.0 / pow(((double)tree.age + 1.0), 0.6));
					if (ageseinfluss < densitywertminimum) {
						ageseinfluss = densitywertminimum;
					}
					tree.densitywert = tree.densitywert * ageseinfluss;

				} else if (parameter[0].dichtheightrel == 23) {
					double densitywertminimum = 0.5;
					double ageseinfluss = (1.0 / pow(((double)tree.age + 1.0), 0.175));
					if (ageseinfluss < densitywertminimum) {
						ageseinfluss = densitywertminimum;
					}
					tree.densitywert = tree.densitywert * ageseinfluss;
				}

				if (tree.densitywert < 0)
					tree.densitywert = 0.0;

				// set to maximal value if density value is greater than it (rescaling)
				if (tree.densitywert > parameter[0].desitymaxreduction)
					tree.densitywert = parameter[0].desitymaxreduction;
					
			}
				// water stress update
				// ... dependency on local site conditions
// if(tree.height/10 > 130) cout << tree.twi << "\t" << tree.soilhumidity << "\t" << tree.envirimpact << endl;
				tree.soilhumidity = pow(tree.twi/(6.25*100), 0.5*0.25) * tree.soilhumidity;
// if(tree.height/10 > 130) cout << " ... between: " << tree.twi << "\t" << tree.soilhumidity << "\t" << tree.envirimpact << endl;
				if( (tree.soilhumidity < speciestrait[tree.species].minsoilwater*100) | (tree.soilhumidity > speciestrait[tree.species].maxsoilwater*100) )
					tree.soilhumidity=0;
				else
					tree.soilhumidity=1;
// if(tree.height/10 > 130) cout << " ... after: " << tree.twi << "\t" << tree.soilhumidity << "\t" << tree.envirimpact << endl;
		}
	}
}

void PrepareCryogrid(VectorList<Tree>& tree_list, vector<Cryogrid>& cryo_list, vector<Envirgrid>& plot_list) {
	// setup and wipe grid
	double sizemagnifcryo =  ((double) parameter[0].sizemagnif) / 50;
	// cout << sizemagnifcryo << " sizemagnifcryo " << endl;

	// TODO: maybe move to intializing
	// cout << (int) (ceil(treerows*sizemagnifcryo) * ceil(treecols*sizemagnifcryo)) << " = cells in cryogrid (plotupdate)" << endl;	
// #pragma omp parallel for default(shared) schedule(guided)
	for(int kartenpos=0; kartenpos < (int) (ceil(treerows*sizemagnifcryo) * ceil(treecols*sizemagnifcryo)); kartenpos++) {
		auto& pCryogrid = cryo_list[kartenpos];
		
		// aggregiere nach Scenarien!! -> x wert abhängig 0-A, A-B, C-max
		pCryogrid.leafarea = 0;
		pCryogrid.stemarea = 0;
		pCryogrid.maxtreeheight = 0;
		pCryogrid.treecount = 0;
		pCryogrid.meantreeheight = 0;
		pCryogrid.maxthawing_depth = 0;	// x/ 10 => in cm
		pCryogrid.litterheight0 = 0;	 // x/ 100 => in cm
		pCryogrid.soilhumidity = 0;
		pCryogrid.envirgridcount = 0;
	}

// #pragma omp parallel for default(shared) schedule(guided)
	for (unsigned int tree_i = 0; tree_i < tree_list.size(); ++tree_i) {
        auto& tree = tree_list[tree_i];

		if (tree.growing == true) {
			int i=(int) floor((tree.ycoo / 1000) *sizemagnifcryo);
			int j=(int) floor((tree.xcoo / 1000) *sizemagnifcryo);
			
			// cout << i << " i & " << j << " j " << endl;
			
			
			// TODO: leaf- and stemarea distribution
			// TODO: impact area for leaf- stem area check
			// double crownradius=exp(0.9193333*log(tree.dbasal) + 2.4618496) / 100; //in m // LACA Chukotka
			double crownradius=exp(1.067957*log(tree.dbasal) + 2.063213) / 100; //in m // LAGM Central Yakutia
			    
			// cout << crownradius << endl;
			// cout << cryo_list.size() << " = length cryo_list" << endl;
			// cout << world_cryo_list.size() << " = length world_cryo_list" << endl;
			
			double cryogridcellarea = pow(1.0/sizemagnifcryo, 2);
			// cout << cryogridcellarea << " cellarea | "  << endl ;

			// living branches at the tree 
			double livingtreefraction = (tree.height - tree.crownstart) / tree.height;
			if(livingtreefraction < 0.0)
				livingtreefraction = 0.0;
			else if(livingtreefraction > 1.0)
				livingtreefraction = 1.0;

			// if the trees influences only one density grid cell
			if ( crownradius < (1.0/sizemagnifcryo) ) {
				// cryo_list[i*ceil(treecols*sizemagnifcryo)+j].leafarea += livingtreefraction * 23.99583 * tree.dbasal * 2 / cryogridcellarea; // LACA Chukotka
				cryo_list[i*ceil(treecols*sizemagnifcryo)+j].leafarea += livingtreefraction * 13.7443 * tree.dbasal * 2 / cryogridcellarea; // LAGM Central Yakutia
				cryo_list[i*ceil(treecols*sizemagnifcryo)+j].stemarea += M_PI * tree.dbasal/100 * sqrt( pow(tree.dbasal/100,2) + pow(tree.height / 10 / 100,2) )  / cryogridcellarea;
				if(tree.height / 10 > cryo_list[i*ceil(treecols*sizemagnifcryo)+j].maxtreeheight)
					cryo_list[i*ceil(treecols*sizemagnifcryo)+j].maxtreeheight = tree.height / 10;
				cryo_list[i*ceil(treecols*sizemagnifcryo)+j].meantreeheight += tree.height / 10;
				// cryo_list[i*ceil(treecols*sizemagnifcryo)+j].soilhumidity += pow(tree.twi/(6.25*100), 0.5*0.25) * tree.soilhumidity;
				cryo_list[i*ceil(treecols*sizemagnifcryo)+j].treecount++;
			} else {// if the trees influences more than one density grid cell
				// determine dimensions of the grid around the tree
				int xyquerrastpos= (int) floor(crownradius*sizemagnifcryo);
				
				// TODO: check
				double fractionimpactpertile = cryogridcellarea / (M_PI * pow(crownradius,2));
				
				if(fractionimpactpertile > 0) {
					// determine shifted coordinates and adding up the density value
					// double leafareaoftreepertile = fractionimpactpertile * livingtreefraction * 23.99583 * tree.dbasal * 2;
					double leafareaoftreepertile = fractionimpactpertile * livingtreefraction * 13.7443 * tree.dbasal * 2;
					double stemareaoftreepertile = fractionimpactpertile * M_PI * tree.dbasal/100 * sqrt( pow(tree.dbasal/100,2) + pow(tree.height / 10 / 100,2) );
					
					for (int rastposi=(i+xyquerrastpos); rastposi>(i-(xyquerrastpos+1)); rastposi--) {
						for (int rastposj=(j-xyquerrastpos); rastposj<(j+xyquerrastpos+1); rastposj++) {
							if ( ( rastposi<=( ceil((treerows-1)*sizemagnifcryo)) && rastposi>=0 ) && ( rastposj<=( ceil((treecols-1)*sizemagnifcryo)) && rastposj>=0 ) ) {
								// Distance calculation to determine the influence of the density value in spatial units ...
								// ... and inserting the value at every position
								double entfrastpos=sqrt(pow(double(i-rastposi),2)+pow(double(j-rastposj),2));
								// only if the current grid cell is part of the influence area, a value is assigned
								if (entfrastpos <= (double) xyquerrastpos) {
									cryo_list[rastposi*ceil(treecols*sizemagnifcryo)+rastposj].leafarea += leafareaoftreepertile;
									cryo_list[rastposi*ceil(treecols*sizemagnifcryo)+rastposj].stemarea += stemareaoftreepertile;
									if(tree.height / 10 > cryo_list[rastposi*ceil(treecols*sizemagnifcryo)+rastposj].maxtreeheight)
										cryo_list[rastposi*ceil(treecols*sizemagnifcryo)+rastposj].maxtreeheight = tree.height / 10;
									cryo_list[rastposi*ceil(treecols*sizemagnifcryo)+rastposj].meantreeheight += tree.height / 10;
									// cryo_list[rastposi*ceil(treecols*sizemagnifcryo)+rastposj].soilhumidity += pow(tree.twi/(6.25*100), 0.5*0.25) * tree.soilhumidity;
									cryo_list[rastposi*ceil(treecols*sizemagnifcryo)+rastposj].treecount++;
								}
							}
						}
					}
				} else {
					cout << " ... warning fractionimpactpertile unrealistic with = " << fractionimpactpertile << endl;
				}
			}
		}
	}

	// add envir-grid values
   const auto loop_size = static_cast<std::size_t>(treerows) * static_cast<std::size_t>(parameter[0].sizemagnif) * static_cast<std::size_t>(treecols)* static_cast<std::size_t>(parameter[0].sizemagnif);
// #pragma omp parallel for default(shared) schedule(guided)
	for (std::size_t kartenpos = 0; kartenpos < loop_size; ++kartenpos) {
		auto& pEnvirgrid = plot_list[kartenpos];		
		// calculate weighted mean of surrounding 3x3 grid Cryogrid-maxthawing_depth values for each Envirgrid-Tile

		// find position in Cryogrid
		double sizemagnifcryo =  ((double) parameter[0].sizemagnif) / 50;

		// pEnvirgrid->ycoo=floor( (double) kartenpos/(treecols*parameter[0].sizemagnif) );
		// pEnvirgrid->xcoo=(double) kartenpos - (pEnvirgrid->ycoo * (treecols*parameter[0].sizemagnif));
		double pEnvirgridycoo=floor( (double) kartenpos/(treecols*parameter[0].sizemagnif) );
		double pEnvirgridxcoo=(double) kartenpos - (pEnvirgridycoo * (treecols*parameter[0].sizemagnif));
		double yii = (sizemagnifcryo * pEnvirgridycoo / parameter[0].sizemagnif);
		double xii = (sizemagnifcryo * pEnvirgridxcoo / parameter[0].sizemagnif);
		int yi = (int) floor(yii);
		int xi = (int) floor(xii);
		
		// add values
		cryo_list[yi*sizemagnifcryo*treecols+xi].maxthawing_depth += pEnvirgrid.maxthawing_depth / 10; // in cm
		cryo_list[yi*sizemagnifcryo*treecols+xi].litterheight0 += pEnvirgrid.litterheight0 / 100; // in cm
		cryo_list[yi*sizemagnifcryo*treecols+xi].soilhumidity += pow(pEnvirgrid.twi/(6.25*100), 0.5*0.25) * pEnvirgrid.soilhumidity / 100; // in percent
		cryo_list[yi*sizemagnifcryo*treecols+xi].envirgridcount++;
	}
	
	// process mean value of tree height and of soilhumidity and litterheight
	// cryo_list[i*ceil(treecols*sizemagnifcryo)+j].meantreeheight/cryo_list[i*ceil(treecols*sizemagnifcryo)+j].treecount;
// #pragma omp parallel for default(shared) schedule(guided)
	for (int kartenpos=0; kartenpos < (int) (ceil(treerows*sizemagnifcryo) * ceil(treecols*sizemagnifcryo)); kartenpos++) {
		auto& pCryogrid = cryo_list[kartenpos];

		if (pCryogrid.treecount > 0) {
			pCryogrid.meantreeheight = pCryogrid.meantreeheight / (double) pCryogrid.treecount;
			// pCryogrid.soilhumidity = pCryogrid.soilhumidity / (double) pCryogrid.treecount;
		} else {
			pCryogrid.meantreeheight = 0.0;
			// pCryogrid.soilhumidity = 0.0;
		}
			
		if (pCryogrid.envirgridcount > 0) {
			pCryogrid.maxthawing_depth = pCryogrid.maxthawing_depth / (double) pCryogrid.envirgridcount;
			pCryogrid.litterheight0 = pCryogrid.litterheight0 / (double) pCryogrid.envirgridcount;
			pCryogrid.soilhumidity = pCryogrid.soilhumidity / (double) pCryogrid.envirgridcount;
		} else {
			pCryogrid.maxthawing_depth = 0.0;
			pCryogrid.litterheight0 = 0.0;
			pCryogrid.soilhumidity = 0.0;
		}
	}
	
	// output before read in and update from Cryogrid
	if( (parameter[0].outputmode == 14) && ((parameter[0].ivort % 50 == 0) || (parameter[0].ivort >= 2005)) ) { // compare with dataoutput
            // assemble file name
			ostringstream s1, s2, s3, s4, s5, s6, s7, s8;
			s1 << parameter[0].repeati;
            s2 << parameter[0].weatherchoice;
            s3 << parameter[0].ivort;
            s4 << 1;
            s5 << parameter[0].cryogrid_scenario;
			
			FILE* filepointer;
			string dateiname;
			dateiname = "./output_" + s5.str() + "/datacryogrid_" + s1.str() + "_" + s2.str() + "_" + s3.str() + "_" + s4.str() + "_beforeupdate.csv";
 
            // trying to open the file for writing
            filepointer = fopen(dateiname.c_str(), "r+");
            // if fopen fails, open a new file + header output
            if (filepointer == NULL) {
                filepointer = fopen(dateiname.c_str(), "w+");

                fprintf(filepointer, "x;");
                fprintf(filepointer, "y;");
                fprintf(filepointer, "leafarea;");
                fprintf(filepointer, "stemarea;");
                fprintf(filepointer, "maxtreeheight;");
                fprintf(filepointer, "meantreeheight;");
                fprintf(filepointer, "treecount;");
                fprintf(filepointer, "maxthawing_depth;");
                fprintf(filepointer, "litterheight0;");
                fprintf(filepointer, "soilhumidity;");
                fprintf(filepointer, "envirgridcount");
                fprintf(filepointer, "\n");

                if (filepointer == NULL) {
                    fprintf(stderr, "Error: File could not be opened!\n");
                    exit(1);
                }
            }

            fseek(filepointer, 0, SEEK_END);

            // data output for each grid cell
			double sizemagnifcryo =  ((double) parameter[0].sizemagnif) / 50;
			for (int kartenpos=0; kartenpos < (int) (ceil(treerows*sizemagnifcryo) * ceil(treecols*sizemagnifcryo)); kartenpos++) {
				auto& pCryogrid = cryo_list[kartenpos];
				
				fprintf(filepointer, "%d;", (int)pCryogrid.xcoo);
				fprintf(filepointer, "%d;", (int)pCryogrid.ycoo);
				fprintf(filepointer, "%4.4f;", pCryogrid.leafarea);
				fprintf(filepointer, "%4.4f;", pCryogrid.stemarea);
				fprintf(filepointer, "%4.2f;", pCryogrid.maxtreeheight);
				fprintf(filepointer, "%4.2f;", pCryogrid.meantreeheight);
				fprintf(filepointer, "%d;", pCryogrid.treecount);
				fprintf(filepointer, "%4.2f;", pCryogrid.maxthawing_depth);
				fprintf(filepointer, "%4.2f;", pCryogrid.litterheight0);
				fprintf(filepointer, "%4.2f;", pCryogrid.soilhumidity);
				fprintf(filepointer, "%d", pCryogrid.envirgridcount);				
				fprintf(filepointer, "\n");
			}
			
            fclose(filepointer);
 
	}	
}
	
void UpdateCryogrid(vector<Cryogrid>& cryo_list) {
	// rationale:
	// 1. compile data and write to file 
	// 2. call CryoGrid and estimate permafrost thaw depth
	// 3. read thaw depth and assign thaw depth to Environment-grid by interpolation from 10 x 10 m grid to 0.2 x 0.2 m grid
	
	// declaration of used vectors 
	std::vector<double> leafareaiout;
	std::vector<double> maxthawing_depthiout;
	std::vector<double> soilhumidityiout;

	if(true) {// 1. compile data and write to file
		FILE *filepointer;
		string filename;

		// assemble file name:
		ostringstream s1,s2, s3;
		s1 << std::setfill('0') << std::setw(5) << parameter[0].ivort; // TODO: replace or add current year here
		s2 << std::setfill('0') << std::setw(10) << parameter[0].weatherchoice;
		s3 << parameter[0].cryogrid_scenario;
		// filename="/legacy/Model/Modelling/CryogridLAVESI/CouplingMaster/output/cryogridoutput_" +s1.str()+"_"+s2.str()+"_aggregated.txt"; // TODO: add relative path
		filename="../output_"+s3.str()+"/cryogridoutput_" +s1.str()+"_"+s2.str()+"_aggregated.txt"; // TODO: add relative path
		// s1.str("");s1.clear();s2.str("");s2.clear();s2.str("");s2.clear();

		// unsigned short int cryogrid_scenario = 2; // read from parameterfile

		if(parameter[0].cryogrid_scenario == 0) { // ... create copy of cryo_list and sort by leaf area
			
			// trying to open the file for reading
			filepointer = fopen (filename.c_str(), "r+");
			// if fopen fails, open a new file + header output
			if (filepointer == NULL) {
				filepointer = fopen (filename.c_str(), "w+"); // TODO: overwrite
				
				// first line is header
				int yeartocryogrid=1979;
				if(parameter[0].currentyear>1979 && parameter[0].currentyear<=2100)// Range of EraInterim data for CryoGrid Forcing
					yeartocryogrid=parameter[0].currentyear;
				fprintf(filepointer, "%d;current year;;;;", yeartocryogrid);//TODO: check if year is correctly assigned
				fprintf(filepointer, "\n");
				
				fprintf(filepointer, "LAVESI output file for CryoGrid input;");
				fprintf(filepointer, "min;");
				fprintf(filepointer, "25quantile;");
				fprintf(filepointer, "median;");
				fprintf(filepointer, "75quantile;");
				fprintf(filepointer, "max");

				fprintf(filepointer, "\n");

				if (filepointer == NULL) {
					fprintf(stderr, "Error: output file is missing!\n");
					exit(1);
				}
			}

			// data evaluation and output
			fseek(filepointer,0,SEEK_END);
			
			// aggregate data
			std::vector<double> leafareai;
			std::vector<double> stemareai;
			std::vector<double> maxtreeheighti;
			std::vector<double> meantreeheighti;
			std::vector<double> maxthawing_depthi;
			std::vector<double> litterheight0i;
			std::vector<double> soilhumidityi;
			// create a copy of cryo_list and sort by leaf area values
			vector<Cryogrid> cryo_list_sortcopy(cryo_list); 
			std::sort(cryo_list_sortcopy.begin(), cryo_list_sortcopy.end(), 
				[](const Cryogrid &firstelement, const Cryogrid &secondelement) { return firstelement.leafarea < secondelement.leafarea; });
			
			double sizemagnifcryo =  ((double) parameter[0].sizemagnif) /50;
			for (int kartenpos=0; kartenpos< (ceil(treerows*sizemagnifcryo) *ceil(treecols*sizemagnifcryo)); kartenpos++) {
				auto& pCryogrid = cryo_list_sortcopy[kartenpos];

				leafareai.push_back(pCryogrid.leafarea);
				stemareai.push_back(pCryogrid.stemarea);
				maxtreeheighti.push_back(pCryogrid.maxtreeheight / 100); // in m
				meantreeheighti.push_back(pCryogrid.meantreeheight / 100); // in m
				maxthawing_depthi.push_back(pCryogrid.maxthawing_depth); // in cm
				litterheight0i.push_back(pCryogrid.litterheight0); // in cm
				soilhumidityi.push_back(pCryogrid.soilhumidity); // in percent
			}
			// setup leafareaiout and maxthawing_depthi aggregated
			leafareaiout.push_back(leafareai[0]);
			leafareaiout.push_back(leafareai[leafareai.size()/2/2]);
			leafareaiout.push_back(leafareai[leafareai.size()/2]);
			leafareaiout.push_back(leafareai[leafareai.size()/2+leafareai.size()/2/2]);
			leafareaiout.push_back(leafareai[leafareai.size()-1]);
			maxthawing_depthiout.push_back(maxthawing_depthi[0]);
			maxthawing_depthiout.push_back(maxthawing_depthi[maxthawing_depthi.size()/2/2]);
			maxthawing_depthiout.push_back(maxthawing_depthi[maxthawing_depthi.size()/2]);
			maxthawing_depthiout.push_back(maxthawing_depthi[maxthawing_depthi.size()/2+maxthawing_depthi.size()/2/2]);
			maxthawing_depthiout.push_back(maxthawing_depthi[maxthawing_depthi.size()-1]);
			soilhumidityiout.push_back(soilhumidityi[0]);
			soilhumidityiout.push_back(soilhumidityi[soilhumidityi.size()/2/2]);
			soilhumidityiout.push_back(soilhumidityi[soilhumidityi.size()/2]);
			soilhumidityiout.push_back(soilhumidityi[soilhumidityi.size()/2+soilhumidityi.size()/2/2]);
			soilhumidityiout.push_back(soilhumidityi[soilhumidityi.size()-1]);
			
			// print to output file
			fprintf(filepointer, "leafareaindex;");
			fprintf(filepointer, "%10.1f;", leafareai[0]);
			fprintf(filepointer, "%10.1f;", leafareai[leafareai.size()/2/2]);
			fprintf(filepointer, "%10.1f;", leafareai[leafareai.size()/2]);
			fprintf(filepointer, "%10.1f;", leafareai[leafareai.size()/2+leafareai.size()/2/2]);
			fprintf(filepointer, "%10.1f", leafareai[leafareai.size()-1]);
			fprintf(filepointer, "\n");

			fprintf(filepointer, "stemareaindex;");
			fprintf(filepointer, "%10.2f;", stemareai[0]);
			fprintf(filepointer, "%10.2f;", stemareai[stemareai.size()/2/2]);
			fprintf(filepointer, "%10.2f;", stemareai[stemareai.size()/2]);
			fprintf(filepointer, "%10.2f;", stemareai[stemareai.size()/2+stemareai.size()/2/2]);
			fprintf(filepointer, "%10.2f", stemareai[stemareai.size()-1]);
			fprintf(filepointer, "\n");
			
			fprintf(filepointer, "mean50percent2maxtreeheight_m;");
			fprintf(filepointer, "%3.3f;", (meantreeheighti[0] + maxtreeheighti[0])/2);
			fprintf(filepointer, "%3.3f;", (meantreeheighti[meantreeheighti.size()/2/2] + maxtreeheighti[maxtreeheighti.size()/2/2])/2);
			fprintf(filepointer, "%3.3f;", (meantreeheighti[meantreeheighti.size()/2] + maxtreeheighti[maxtreeheighti.size()/2])/2);
			fprintf(filepointer, "%3.3f;", (meantreeheighti[meantreeheighti.size()/2+meantreeheighti.size()/2/2] + maxtreeheighti[maxtreeheighti.size()/2+maxtreeheighti.size()/2/2])/2);
			fprintf(filepointer, "%3.3f", (meantreeheighti[meantreeheighti.size()-1] + maxtreeheighti[maxtreeheighti.size()-1])/2);
			fprintf(filepointer, "\n");
			
			fprintf(filepointer, "maxtreeheight_m;");
			fprintf(filepointer, "%3.3f;", maxtreeheighti[0]);
			fprintf(filepointer, "%3.3f;", maxtreeheighti[maxtreeheighti.size()/2/2]);
			fprintf(filepointer, "%3.3f;", maxtreeheighti[maxtreeheighti.size()/2]);
			fprintf(filepointer, "%3.3f;", maxtreeheighti[maxtreeheighti.size()/2+maxtreeheighti.size()/2/2]);
			fprintf(filepointer, "%3.3f", maxtreeheighti[maxtreeheighti.size()-1]);
			fprintf(filepointer, "\n");

			fprintf(filepointer, "meantreeheight_m;");
			fprintf(filepointer, "%3.3f;", meantreeheighti[0]);
			fprintf(filepointer, "%3.3f;", meantreeheighti[meantreeheighti.size()/2/2]);
			fprintf(filepointer, "%3.3f;", meantreeheighti[meantreeheighti.size()/2]);
			fprintf(filepointer, "%3.3f;", meantreeheighti[meantreeheighti.size()/2+meantreeheighti.size()/2/2]);
			fprintf(filepointer, "%3.3f", meantreeheighti[meantreeheighti.size()-1]);
			fprintf(filepointer, "\n");
			
			fprintf(filepointer, "litterheight_cm;");
			fprintf(filepointer, "%3.3f;", litterheight0i[0]);
			fprintf(filepointer, "%3.3f;", litterheight0i[litterheight0i.size()/2/2]);
			fprintf(filepointer, "%3.3f;", litterheight0i[litterheight0i.size()/2]);
			fprintf(filepointer, "%3.3f;", litterheight0i[litterheight0i.size()/2+litterheight0i.size()/2/2]);
			fprintf(filepointer, "%3.3f", litterheight0i[litterheight0i.size()-1]);
			fprintf(filepointer, "\n");
			
			fprintf(filepointer, "soilhumidity_percent;");
			fprintf(filepointer, "%3.3f;", soilhumidityi[0]);
			fprintf(filepointer, "%3.3f;", soilhumidityi[soilhumidityi.size()/2/2]);
			fprintf(filepointer, "%3.3f;", soilhumidityi[soilhumidityi.size()/2]);
			fprintf(filepointer, "%3.3f;", soilhumidityi[soilhumidityi.size()/2+soilhumidityi.size()/2/2]);
			fprintf(filepointer, "%3.3f", soilhumidityi[soilhumidityi.size()-1]);
			fprintf(filepointer, "\n");
			
			fprintf(filepointer, "albedo;");
			fprintf(filepointer, "%3.3f;", 0.4);
			fprintf(filepointer, "%3.3f;", 0.4);
			fprintf(filepointer, "%3.3f;", 0.4);
			fprintf(filepointer, "%3.3f;", 0.4);
			fprintf(filepointer, "%3.3f", 0.4);
			fprintf(filepointer, "\n");
			
			fprintf(filepointer, "organiclayer;");
			fprintf(filepointer, "%d;", 1);
			fprintf(filepointer, "%d;", 1);
			fprintf(filepointer, "%d;", 1);
			fprintf(filepointer, "%d;", 1);
			fprintf(filepointer, "%d", 1);
			fprintf(filepointer, "\n");
			
			fprintf(filepointer, "maxthawing_depth_cm;");
			fprintf(filepointer, "%3.3f;", maxthawing_depthi[0]);
			fprintf(filepointer, "%3.3f;", maxthawing_depthi[maxthawing_depthi.size()/2/2]);
			fprintf(filepointer, "%3.3f;", maxthawing_depthi[maxthawing_depthi.size()/2]);
			fprintf(filepointer, "%3.3f;", maxthawing_depthi[maxthawing_depthi.size()/2+maxthawing_depthi.size()/2/2]);
			fprintf(filepointer, "%3.3f", maxthawing_depthi[maxthawing_depthi.size()-1]);
			fprintf(filepointer, "\n");

			fclose(filepointer);
		} else { // ... for each scenario mean over 3 condition
				// natural						albedo		organic layer thick		adtreemort	defoliation
					// 11 {x<1666}				0.15		1						0			0
					// 12 {1666<x<3333}			0.15		1						0			0
					// 13 {x>3333}				0.15		1						0			0
				// logging
					// 21 {x<1666}				0.15		1						0.25		0
					// 22 {1666<x<3333}			0.15		1						0.50		0
					// 23 {x>3333}				0.15		1						1.00		0
				// surface fire
					// 31 {x<1666}				0.4			1						0.12		0
					// 32 {1666<x<3333}			0.4			0.1						0.20		0
					// 33 {x>3333}				0.4			0						0.50		0
				// crown fire
					// 41 {x<1666}				0.4			0.1						0.60		1.00
					// 42 {1666<x<3333}			0.4			0						0.75		1.00
					// 43 {x>3333}				0.4			0						1.00		1.00
				// insects/pests
					// 51 {x<1666}				0.15		1						0			0.10
					// 52 {1666<x<3333}			0.15		1						0.06		0.50
					// 53 {x>3333}				0.15		1						0.10		1.00
	
			double yearsincedisturbance = 0;
			if(parameter[0].currentyear >= parameter[0].cryogrid_disturbanceyear)
				yearsincedisturbance = parameter[0].currentyear - parameter[0].cryogrid_disturbanceyear;

			double albedo = 0.15;
			if( ((parameter[0].cryogrid_scenario == 3) | (parameter[0].cryogrid_scenario == 4)) && (parameter[0].ivort >= parameter[0].cryogrid_disturbanceyear) )
				albedo = 0.40 + ((0.15-0.40)/7) * yearsincedisturbance; // based on Jin, 2012 ground albedo after 7 years back to pre-fire conditions
			if( albedo < 0.15)
				albedo = 0.15;

			double organiclayer[3]={0};
			for (auto i = 0; i < 3; i++) { // check
				organiclayer[i] = 1.0;
			}
// for (auto i = 0; i < 3; i++) { // check
	// cout << "olayer = " << organiclayer[i] << endl;
// }
			if( (parameter[0].cryogrid_scenario == 3) && (parameter[0].ivort >= parameter[0].cryogrid_disturbanceyear) ) {
				organiclayer[1] = 0.1 + ((1.0-0.1)/10) * yearsincedisturbance; // based on Bonan, 1989, organic layer revoverd after 10 years back to pre-fire conditions
				organiclayer[2] = 0.0 + ((1.0-0.0)/10) * yearsincedisturbance;
			} else if( (parameter[0].cryogrid_scenario == 4) && (parameter[0].ivort >= parameter[0].cryogrid_disturbanceyear) ) {
				organiclayer[0] = 0.1 + ((1.0-0.1)/10) * yearsincedisturbance;
				organiclayer[1] = 0.0 + ((1.0-0.0)/10) * yearsincedisturbance;
				organiclayer[2] = 0.0 + ((1.0-0.0)/10) * yearsincedisturbance;
			}
			for (auto i = 0; i < 3; i++) { // check
				if( organiclayer[i] > 1.0)
					organiclayer[i] = 1.0;
				else if( organiclayer[i] < 0.0)
					organiclayer[i] = 0.0;
			}


			
			// aggregate data
			double leafareai[3]={0};
			double stemareai[3]={0};
			double maxtreeheighti[3]={0};
			double meantreeheighti[3]={0};
			double maxthawing_depthi[3]={0};
			double litterheight0i[3]={0};
			double soilhumidityi[3]={0};
			double counti[3]={0};
			
			double sizemagnifcryo =  ((double) parameter[0].sizemagnif) /50;
			const auto loop_size_cryogrid = static_cast<std::size_t>(ceil(treerows*sizemagnifcryo)) * static_cast<std::size_t>(ceil(treecols*sizemagnifcryo));
// #pragma omp parallel for default(shared) schedule(guided)
			for (std::size_t kartenpos=0; kartenpos < loop_size_cryogrid; kartenpos++) {
				auto& pCryogrid = cryo_list[kartenpos];

				if( ((pCryogrid.xcoo * (1/sizemagnifcryo)) < ((double)treecols - 50))
					&& ((pCryogrid.ycoo * (1/sizemagnifcryo)) < ((double)treerows - 50))
					&& ((pCryogrid.xcoo * (1/sizemagnifcryo)) > 50)
					&& ((pCryogrid.ycoo * (1/sizemagnifcryo)) > 50)
					) { // set a 50 m buffer to exclude boundary conditions at the plot's margin
					// depending on position assign to the condition
					unsigned int conditioni = 2;
					if( (pCryogrid.xcoo * (1/sizemagnifcryo)) < ((double)treecols/3) )
						conditioni = 1;
					else if( (pCryogrid.xcoo * (1/sizemagnifcryo)) > (2 * (double)treecols/3) )
						conditioni = 3;
						
					leafareai[conditioni-1] += pCryogrid.leafarea;
					stemareai[conditioni-1] += pCryogrid.stemarea;
					maxtreeheighti[conditioni-1] += pCryogrid.maxtreeheight / 100;
					meantreeheighti[conditioni-1] += pCryogrid.meantreeheight / 100;
					maxthawing_depthi[conditioni-1] += pCryogrid.maxthawing_depth;
					litterheight0i[conditioni-1] += pCryogrid.litterheight0;
					soilhumidityi[conditioni-1] += pCryogrid.soilhumidity;
					counti[conditioni-1] ++;
				}
			}

// #pragma omp single
// {
			for (auto i = 0; i < 3; i++) { // calculate mean
				leafareai[i] = leafareai[i]/counti[i];
				stemareai[i] = stemareai[i]/counti[i];
				maxtreeheighti[i] = maxtreeheighti[i]/counti[i];
				meantreeheighti[i] = meantreeheighti[i]/counti[i];
				maxthawing_depthi[i] = maxthawing_depthi[i]/counti[i];
				litterheight0i[i] = litterheight0i[i]/counti[i];
				soilhumidityi[i] = soilhumidityi[i]/counti[i];
			}
			// setup leafareaiout aggregated
			leafareaiout.push_back(leafareai[0]);
			leafareaiout.push_back(leafareai[1]);
			leafareaiout.push_back(leafareai[2]);
			maxthawing_depthiout.push_back(maxthawing_depthi[0]);
			maxthawing_depthiout.push_back(maxthawing_depthi[1]);
			maxthawing_depthiout.push_back(maxthawing_depthi[2]);
			soilhumidityiout.push_back(soilhumidityi[0]);
			soilhumidityiout.push_back(soilhumidityi[1]);
			soilhumidityiout.push_back(soilhumidityi[2]);

			// trying to open the file for reading
			filepointer = fopen (filename.c_str(), "r+");
			// if fopen fails, open a new file + header output
			if (filepointer == NULL) {
				filepointer = fopen (filename.c_str(), "w+");
				
				// first line is header
				int yeartocryogrid=1979;
				if(parameter[0].currentyear>1979 && parameter[0].currentyear<=2100)// Range of EraInterim data for CryoGrid Forcing
					yeartocryogrid=parameter[0].currentyear;
				fprintf(filepointer, "%d;current year;;", yeartocryogrid);//TODO: check if year is correctly assigned
				fprintf(filepointer, "\n");
				
				fprintf(filepointer, "LAVESI output file for CryoGrid input;");
				fprintf(filepointer, "condition1;");
				fprintf(filepointer, "condition2;");
				fprintf(filepointer, "condition3");

				fprintf(filepointer, "\n");

				if (filepointer == NULL) {
					fprintf(stderr, "Error: output file is missing!\n");
					exit(1);
				}
			}

			// data evaluation and output
			fseek(filepointer,0,SEEK_END);

			// print to output file
			fprintf(filepointer, "leafareaindex;");
			fprintf(filepointer, "%10.1f;", leafareai[0]);
			fprintf(filepointer, "%10.1f;", leafareai[1]);
			fprintf(filepointer, "%10.1f", leafareai[2]);
			fprintf(filepointer, "\n");
			
			fprintf(filepointer, "stemareaindex;");
			fprintf(filepointer, "%10.2f;", stemareai[0]);
			fprintf(filepointer, "%10.2f;", stemareai[1]);
			fprintf(filepointer, "%10.2f", stemareai[2]);
			fprintf(filepointer, "\n");
			
			fprintf(filepointer, "mean50percent2maxtreeheight_m;");
			fprintf(filepointer, "%3.3f;", (meantreeheighti[0]+maxtreeheighti[0])/2);
			fprintf(filepointer, "%3.3f;", (meantreeheighti[1]+maxtreeheighti[1])/2);
			fprintf(filepointer, "%3.3f", (meantreeheighti[2]+maxtreeheighti[2])/2);
			fprintf(filepointer, "\n");
			
			fprintf(filepointer, "maxtreeheight_m;");
			fprintf(filepointer, "%3.3f;", maxtreeheighti[0]);
			fprintf(filepointer, "%3.3f;", maxtreeheighti[1]);
			fprintf(filepointer, "%3.3f", maxtreeheighti[2]);
			fprintf(filepointer, "\n");

			fprintf(filepointer, "meantreeheight_m;");
			fprintf(filepointer, "%3.3f;", meantreeheighti[0]);
			fprintf(filepointer, "%3.3f;", meantreeheighti[1]);
			fprintf(filepointer, "%3.3f", meantreeheighti[2]);
			fprintf(filepointer, "\n");
			
			fprintf(filepointer, "litterheight_cm;");
			fprintf(filepointer, "%3.3f;", litterheight0i[0]);
			fprintf(filepointer, "%3.3f;", litterheight0i[1]);
			fprintf(filepointer, "%3.3f", litterheight0i[2]);
			fprintf(filepointer, "\n");
			
			fprintf(filepointer, "soilhumidity_percent;");
			fprintf(filepointer, "%3.3f;", soilhumidityi[0]);
			fprintf(filepointer, "%3.3f;", soilhumidityi[1]);
			fprintf(filepointer, "%3.3f", soilhumidityi[2]);
			fprintf(filepointer, "\n");
			
			fprintf(filepointer, "albedo;");
			fprintf(filepointer, "%3.3f;", albedo);
			fprintf(filepointer, "%3.3f;", albedo);
			fprintf(filepointer, "%3.3f", albedo);
			fprintf(filepointer, "\n");
			
			fprintf(filepointer, "organiclayer;");
			fprintf(filepointer, "%3.3f;", organiclayer[0]);
			fprintf(filepointer, "%3.3f;", organiclayer[1]);
			fprintf(filepointer, "%3.3f", organiclayer[2]);
			fprintf(filepointer, "\n");

			fprintf(filepointer, "maxthawing_depth_cm;");
			fprintf(filepointer, "%3.3f;", maxthawing_depthi[0]);
			fprintf(filepointer, "%3.3f;", maxthawing_depthi[1]);
			fprintf(filepointer, "%3.3f", maxthawing_depthi[2]);
			fprintf(filepointer, "\n");

			fclose(filepointer);
// }
		}
	}

	if(true) {// 2. call CryoGrid and estimate permafrost thaw depth
		cout << "try to called matlab at " << clock() << endl;
		int cryogridcallreturn = 0;
		if (parameter[0].cryogrid_slurm == true) {
			cryogridcallreturn = system("sbatch ../CryoGrid/sample_CryoVeg_switch_scenarios/submit.sh");
		} else {
			cryogridcallreturn = system("matlab -nodisplay -nosplash -nodesktop -r \"run('../CryoGrid/sample_CryoVeg_switch_scenarios/main_CryoVeg.m');exit;\"");
		}
		if(cryogridcallreturn == -1)
		  cout << "system call of CryoGrid failed!" << endl;
		cout << "after called matlab at " << clock() << endl;
	}

	if(true) {// 3. read thaw depth and assign thaw depth to Environment-grid by interpolation from 10 x 10 m grid to 0.2 x 0.2 m grid
		// TODO: clean couts
		// from "/legacy/Model/Modelling/CryogridLAVESI/CouplingMaster/output"
		// cryogridoutput_00025_0000007001_aggregated.txt_CG.csv
		
		FILE *filepointer;
		string filename;
		ostringstream s1,s2,s3;

		// assemble file name
		// ... use same as above //TODO: call it only one for matching
		s1 << std::setfill('0') << std::setw(5) << parameter[0].ivort; // TODO: replace or add current year here
		s2 << std::setfill('0') << std::setw(10) << parameter[0].weatherchoice;
		s3 << parameter[0].cryogrid_scenario;

		// filename="/legacy/Model/Modelling/CryogridLAVESI/CouplingMaster/output/cryogridoutput_" +s1.str()+"_"+s2.str()+"_aggregated.txt_CG.csv"; // TODO: add relative path
		// filename="/legacy/Model/Modelling/CryogridLAVESI/CouplingMaster/output/cryogridoutput_" +s1.str()+"_"+s2.str()+"_aggregated.txt_test_vegetation_snow_40_CG.csv"; // TODO: add relative path
		// filename="/legacy/Model/Modelling/CryogridLAVESI/CouplingMaster/output/cryogridoutput_" +s1.str()+"_"+s2.str()+"_aggregated.txt_CG.csv"; // TODO: add relative path
		filename="../output_"+s3.str()+"/cryogridoutput_" +s1.str()+"_"+s2.str()+"_aggregated.txt_CG.csv"; // TODO: add relative path

		while((filepointer == NULL)) { // wait until output file is avalable
			cout << " try to open file  ... 1 min later ... ";
			filepointer = fopen (filename.c_str(), "r+"); // trying to open the file for reading
			std::this_thread::sleep_for(std::chrono::seconds(10));
		}

		// wait until file is written
		std::this_thread::sleep_for(std::chrono::seconds(10));

		if (filepointer == NULL) { // if fopen fails just do no update of the Envirgrid
			fprintf(stderr, "Error: cryogidresponse file is missing!\n");
		} else {

			// read file per line
			char linebuffer[255];
			std::vector<double> activelayerdepthin;
			std::vector<double> soilhumidityin;
cout << "leafareaiout.size = " << leafareaiout.size() << endl;
cout << "maxthawing_depthiout.size = " << maxthawing_depthiout.size() << endl;

			int counter = 1; // to skip first line of file 
			while (fgets(linebuffer, stringlengthmax, filepointer) != NULL) {
cout << " ... " << counter << " - read in ->  " << linebuffer << endl; // TODO: only for testing, delete
// file in 
// 1. header
// 2. ALthickness max
// 3. Pflanzenverfügbares Wasser in AL
// 4. LAI
// 5. SAI
// 6. height
				if (counter==2) { // maximum active layer thickness
					activelayerdepthin.push_back(strtod(strtok(linebuffer, "\t"), NULL)); // ACHTUNG 
					activelayerdepthin.push_back(strtod(strtok(NULL, "\t"), NULL)); // ACHTUNG 
					activelayerdepthin.push_back(strtod(strtok(NULL, "\t"), NULL)); // ACHTUNG 
cout << activelayerdepthin[0] << " activelayerdepth 1 "  << endl; // print current value // TODO: only for testing, delete
cout << activelayerdepthin[1] << " activelayerdepth 2 "  << endl; // print current value // TODO: only for testing, delete
cout << activelayerdepthin[2] << " activelayerdepth 3 "  << endl; // print current value // TODO: only for testing, delete
cout << leafareaiout[0] << " leafarea 1 " << endl; // print current value // TODO: only for testing, delete
cout << leafareaiout[1] << " leafarea 2 " << endl; // print current value // TODO: only for testing, delete
cout << leafareaiout[2] << " leafarea 3 " << endl; // print current value // TODO: only for testing, delete
cout << maxthawing_depthiout[0] << " maxthawing_depth 1 " << endl; // print current value // TODO: only for testing, delete
cout << maxthawing_depthiout[1] << " maxthawing_depth 2 " << endl; // print current value // TODO: only for testing, delete
cout << maxthawing_depthiout[2] << " maxthawing_depth 3 " << endl; // print current value // TODO: only for testing, delete
				} else if (counter==3) { // plant available soil humidity
					soilhumidityin.push_back(strtod(strtok(linebuffer, "\t"), NULL)); // ACHTUNG 
					soilhumidityin.push_back(strtod(strtok(NULL, "\t"), NULL)); // ACHTUNG 
					soilhumidityin.push_back(strtod(strtok(NULL, "\t"), NULL)); // ACHTUNG 
cout << soilhumidityin[0] << endl; // print current value // TODO: only for testing, delete
cout << soilhumidityin[1] << endl; // print current value // TODO: only for testing, delete
cout << soilhumidityin[2] << endl; // print current value // TODO: only for testing, delete
				}
				counter++;
			}
cout << "length of activelayerdepthin = " << activelayerdepthin.size() << endl;// TODO: only for testing, delete
			
			fclose(filepointer);

			if(parameter[0].cryogrid_scenario == 0) { // setup linear model for interpolation
				// y{activelayerdepthin} ~ x{leafareaiout}
				
				// calculation
					// n vals
					int xn=leafareaiout.size();
					int yn=activelayerdepthin.size();
					// only if equal proceed
					
cout << xn << " xn and yn " << yn << endl;
					
					// mean x
					double sum = 0;
					for(int i = 0; i < xn; i++)
					{
						sum += leafareaiout[i];
cout << leafareaiout[i] << " / " << endl;
					}
					double xmean = sum/xn;
					// diff square sums
					double xvariance;
					for(int i = 0; i < xn; i++)
						xvariance += pow(xmean-leafareaiout[i],2);
					// mean y
					sum = 0;
					for(int i = 0; i < xn; i++) {
						sum += activelayerdepthin[i];
cout << activelayerdepthin[i] << " / " << endl;
					}
					double ymean = sum/xn;
					// diff square sums
					double yxcovariance;
					for(int i = 0; i < xn; i++)
						yxcovariance += (xmean-leafareaiout[i]) * (ymean-activelayerdepthin[i]);
					// slope
					parameter[0].altslope = yxcovariance / xvariance;
					parameter[0].altintercept = ymean - parameter[0].altslope * xmean;
					
cout << " .. slope = " << parameter[0].altslope << endl;
cout << " .. intercept = " << parameter[0].altintercept << endl;	
			} else { // calculate active layer depth and soil humidity anomalies in each of the three conditions
				for(long unsigned int i = 0; i < activelayerdepthin.size(); i++) {
cout << " ALT in / out : " << activelayerdepthin[i] << " / " << maxthawing_depthiout[i] << " / " << endl;
					if(i == 0) {
						parameter[0].activelayeranomaly1 = maxthawing_depthiout[i] - activelayerdepthin[i]*100; // is positive when new/read in mean is higher and this anomaly needs to be added to each pCryogrid.maxthawing_depth value
						parameter[0].soilhumidityanomaly1 = soilhumidityiout[i] - soilhumidityin[i];
					} else if(i == 1) {
						parameter[0].activelayeranomaly2 = maxthawing_depthiout[i] - activelayerdepthin[i]*100;
						parameter[0].soilhumidityanomaly2 = soilhumidityiout[i] - soilhumidityin[i];
					} else if(i == 2) {
						parameter[0].activelayeranomaly3 = maxthawing_depthiout[i] - activelayerdepthin[i]*100;
						parameter[0].soilhumidityanomaly3 = soilhumidityiout[i] - soilhumidityin[i];
					}
				}
cout << parameter[0].activelayeranomaly1 << " | " << parameter[0].activelayeranomaly2 << " | " << parameter[0].activelayeranomaly3 << endl;
cout << parameter[0].soilhumidityanomaly1 << " | " << parameter[0].soilhumidityanomaly2 << " | " << parameter[0].soilhumidityanomaly3 << endl;
			}
		}
	}
}

double estimateALT(double pai_i, double degredayi_i) {
	double alt_i = 0.0;
	double spike = -2.85000 + 0.00275*degredayi_i;
	
	if(pai_i<200)
		alt_i = 1.8666667 + 0.0006667*pai_i;
	else if((pai_i>200) & (pai_i<350))
		alt_i = 2.0 + ((spike-2.0)/(350-200))*(pai_i-200);
	else if((pai_i>200) & (pai_i<350))
		alt_i = spike + ((2.2-spike)/(500-350))*(pai_i-350);
	else if(pai_i>500)
		alt_i = 1.8666667 + 0.0006667*pai_i;
		
	return(alt_i);
}

void UpdateEnvirgridALD(vector<Cryogrid>& cryo_list, vector<Envirgrid>& plot_list, vector<Weather>& weather_list, int yearposition) { // estimate data to Cryogrid and from this to Envirgrid
	// estimate data to Cryogrid
	double sizemagnifcryo =  ((double) parameter[0].sizemagnif) / 50;
	const auto loop_size_cryogrid = static_cast<std::size_t>(ceil(treerows*sizemagnifcryo)) * static_cast<std::size_t>(ceil(treecols*sizemagnifcryo));
#pragma omp parallel for default(shared) schedule(guided)
	for (std::size_t kartenpos=0; kartenpos < loop_size_cryogrid; ++kartenpos) {
		auto& pCryogrid = cryo_list[kartenpos];

		double activelayeri = 0.0;
		double soilhumidityi = 0.0;
		if(parameter[0].cryogridcalled == true) {// when ALT was calculated by CryoGrid
			if(parameter[0].cryogrid_scenario == 0) { // setup linear model for interpolation
				activelayeri = (parameter[0].altslope * pCryogrid.leafarea + parameter[0].altintercept) * 100;
				// activelayeri = (parameter[0].altslope * pCryogrid.leafarea + parameter[0].altintercept) * 100; //TODO
				// check for valid values
				if(activelayeri < 0)
					activelayeri = 0;
				if(soilhumidityi < 0)
					soilhumidityi = 0;
				if(soilhumidityi > 100)
					soilhumidityi = 100;
			} else { // calculate active layer depth anomalie in each of the three conditions
				// depending on position assign to the condition
				if(pCryogrid.xcoo * (1/sizemagnifcryo) < ((double)treecols/3)) { // conditioni = 1;
					// activelayeri = pCryogrid.maxthawing_depth - parameter[0].activelayeranomaly1;
					activelayeri = parameter[0].activelayeranomaly1;
					soilhumidityi = parameter[0].soilhumidityanomaly1;
				} else if(pCryogrid.xcoo * (1/sizemagnifcryo) > (2 * (double)treecols/3)) { // conditioni = 3;
					activelayeri = parameter[0].activelayeranomaly3;
					soilhumidityi = parameter[0].soilhumidityanomaly3;
				} else { // conditioni = 2;
					activelayeri = parameter[0].activelayeranomaly2;
					soilhumidityi = parameter[0].soilhumidityanomaly2;
				}
			}
		} else {// prior CryoGrid-parameterized ALT estimation
		// assess climate data (Active Air Temperature (Sum(T>0)), AAT_>0, here named degreday) of current year
		// ... and estimate ALT for each LAI+SAI=PAI value on the grid
			activelayeri = estimateALT(pCryogrid.leafarea+pCryogrid.stemarea, weather_list[yearposition].degreday) * 100;
			// TODO: add code for soilhumidity
			// check for valid values
			if(activelayeri < 0)
				activelayeri = 0;
			if(soilhumidityi < 0)
				soilhumidityi = 0;
			if(soilhumidityi > 100)
				soilhumidityi = 100;
		}


// cout << " ALT cryogrid = " << activelayeri << " <- " << pCryogrid.maxthawing_depth << endl;
		// assing value to cryogrid
		pCryogrid.maxthawing_depth = activelayeri; // conversion from meter input to cm and *10 is internal storage conversion
		pCryogrid.soilhumidity = soilhumidityi;
	}

	// downscale Cryogrid to Envirgrid
    const auto loop_size = static_cast<std::size_t>(treerows) * static_cast<std::size_t>(parameter[0].sizemagnif) * static_cast<std::size_t>(treecols)* static_cast<std::size_t>(parameter[0].sizemagnif);
// bool stophere = false;
#pragma omp parallel for default(shared) schedule(guided)
	for (std::size_t kartenpos = 0; kartenpos < loop_size; ++kartenpos) {
		auto& pEnvirgrid = plot_list[kartenpos];		
		// calculate weighted mean of surrounding 3x3 grid Cryogrid-maxthawing_depth values for each Envirgrid-Tile

		// find position in Cryogrid
		double sizemagnifcryo =  ((double) parameter[0].sizemagnif) / 50;

		// pEnvirgrid->ycoo=floor( (double) kartenpos/(treecols*parameter[0].sizemagnif) );
		// pEnvirgrid->xcoo=(double) kartenpos - (pEnvirgrid->ycoo * (treecols*parameter[0].sizemagnif));
		double pEnvirgridycoo=floor( (double) kartenpos/(treecols*parameter[0].sizemagnif) );
		double pEnvirgridxcoo=(double) kartenpos - (pEnvirgridycoo * (treecols*parameter[0].sizemagnif));
		double yii = (sizemagnifcryo * pEnvirgridycoo / parameter[0].sizemagnif);
		double xii = (sizemagnifcryo * pEnvirgridxcoo / parameter[0].sizemagnif);
		int yi = (int) floor(yii);
		int xi = (int) floor(xii);

// cout << endl << endl << "  plot coo x,y = " << pEnvirgrid.xcoo << "," << pEnvirgrid.ycoo << endl;
// cout << " coo in Cryogrid x,y = " << xi << "," << yi << endl;

		// access surrounding tiles values
		// ... and calculate distance to Envirgrid-tile
		// for testing only if all exist
		if((yi>1) & (xi>1) & (yi<((sizemagnifcryo*treecols)-1)) & (xi<((sizemagnifcryo*treecols)-1))) {
// if(stophere) cout << endl << endl << "  plot coo x,y = " << pEnvirgridxcoo << "," << pEnvirgridycoo << endl;
// if(stophere) cout << " coo in Cryogrid x,y = " << xi << "," << yi << endl;
			
			std::vector<double> cgvals;
			std::vector<double> cgvals_soilhumidity;
			std::vector<double> dists;

			// access data
// if(stophere) cout << endl << cryo_list[(yi+1)*sizemagnifcryo*treecols+xi-1].maxthawing_depth; // NW
// if(stophere) cout << " " << cryo_list[(yi+1)*sizemagnifcryo*treecols+xi].maxthawing_depth; // N
// if(stophere) cout << " " << cryo_list[(yi+1)*sizemagnifcryo*treecols+xi+1].maxthawing_depth; // NE
			cgvals.push_back(cryo_list[(yi+1)*sizemagnifcryo*treecols+xi-1].maxthawing_depth); // NW
			cgvals.push_back(cryo_list[(yi+1)*sizemagnifcryo*treecols+xi].maxthawing_depth); // N
			cgvals.push_back(cryo_list[(yi+1)*sizemagnifcryo*treecols+xi+1].maxthawing_depth); // NE
			cgvals_soilhumidity.push_back(cryo_list[(yi+1)*sizemagnifcryo*treecols+xi-1].soilhumidity); // NW
			cgvals_soilhumidity.push_back(cryo_list[(yi+1)*sizemagnifcryo*treecols+xi].soilhumidity); // N
			cgvals_soilhumidity.push_back(cryo_list[(yi+1)*sizemagnifcryo*treecols+xi+1].soilhumidity); // NE
				// distance to curent tile
// if(stophere) cout << " - " << sqrt(pow(1-(yii - (double) yi),2) + pow(-1-(xii - (double) xi),2));
// if(stophere) cout << " " << sqrt(pow(1-(yii - (double) yi),2) + pow(0-(xii - (double) xi),2));
// if(stophere) cout << " " << sqrt(pow(1-(yii - (double) yi),2) + pow(1-(xii - (double) xi),2));
				dists.push_back(sqrt(pow(1-(yii - (double) yi),2) + pow(-1-(xii - (double) xi),2)));
				dists.push_back(sqrt(pow(1-(yii - (double) yi),2) + pow(0-(xii - (double) xi),2)));
				dists.push_back(sqrt(pow(1-(yii - (double) yi),2) + pow(1-(xii - (double) xi),2)));
			
// if(stophere) cout << endl << cryo_list[yi*sizemagnifcryo*treecols+xi-1].maxthawing_depth; // W
// if(stophere) cout << " " << cryo_list[yi*sizemagnifcryo*treecols+xi].maxthawing_depth; // actual
// if(stophere) cout << " " << cryo_list[yi*sizemagnifcryo*treecols+xi+1].maxthawing_depth; // E
			cgvals.push_back(cryo_list[yi*sizemagnifcryo*treecols+xi-1].maxthawing_depth); // W
			cgvals.push_back(cryo_list[yi*sizemagnifcryo*treecols+xi].maxthawing_depth); // actual
			cgvals.push_back(cryo_list[yi*sizemagnifcryo*treecols+xi+1].maxthawing_depth); // E
			cgvals_soilhumidity.push_back(cryo_list[yi*sizemagnifcryo*treecols+xi-1].soilhumidity); // W
			cgvals_soilhumidity.push_back(cryo_list[yi*sizemagnifcryo*treecols+xi].soilhumidity); // actual
			cgvals_soilhumidity.push_back(cryo_list[yi*sizemagnifcryo*treecols+xi+1].soilhumidity); // E
				// distance to curent tile
// if(stophere) cout << " - " << sqrt(pow(0-(yii - (double) yi),2) + pow(-1-(xii - (double) xi),2));
// if(stophere) cout << " " << sqrt(pow(0-(yii - (double) yi),2) + pow(0-(xii - (double) xi),2));
// if(stophere) cout << " " << sqrt(pow(0-(yii - (double) yi),2) + pow(1-(xii - (double) xi),2));
				dists.push_back(sqrt(pow(0-(yii - (double) yi),2) + pow(-1-(xii - (double) xi),2)));
				dists.push_back(sqrt(pow(0-(yii - (double) yi),2) + pow(0-(xii - (double) xi),2)));
				dists.push_back(sqrt(pow(0-(yii - (double) yi),2) + pow(1-(xii - (double) xi),2)));

// if(stophere) cout << endl << cryo_list[(yi-1)*sizemagnifcryo*treecols+xi-1].maxthawing_depth; // SW
// if(stophere) cout << " " << cryo_list[(yi-1)*sizemagnifcryo*treecols+xi].maxthawing_depth; // S
// if(stophere) cout << " " << cryo_list[(yi-1)*sizemagnifcryo*treecols+xi+1].maxthawing_depth; // SE
			cgvals.push_back(cryo_list[(yi-1)*sizemagnifcryo*treecols+xi-1].maxthawing_depth); // SW
			cgvals.push_back(cryo_list[(yi-1)*sizemagnifcryo*treecols+xi].maxthawing_depth); // S
			cgvals.push_back(cryo_list[(yi-1)*sizemagnifcryo*treecols+xi+1].maxthawing_depth); // SE
			cgvals_soilhumidity.push_back(cryo_list[(yi-1)*sizemagnifcryo*treecols+xi-1].soilhumidity); // SW
			cgvals_soilhumidity.push_back(cryo_list[(yi-1)*sizemagnifcryo*treecols+xi].soilhumidity); // S
			cgvals_soilhumidity.push_back(cryo_list[(yi-1)*sizemagnifcryo*treecols+xi+1].soilhumidity); // SE
				// distance to curent tile
// if(stophere) cout << " - " << sqrt(pow(-1-(yii - (double) yi),2) + pow(-1-(xii - (double) xi),2));
// if(stophere) cout << " " << sqrt(pow(-1-(yii - (double) yi),2) + pow(0-(xii - (double) xi),2));
// if(stophere) cout << " " << sqrt(pow(-1-(yii - (double) yi),2) + pow(1-(xii - (double) xi),2));
				dists.push_back(sqrt(pow(-1-(yii - (double) yi),2) + pow(-1-(xii - (double) xi),2)));
				dists.push_back(sqrt(pow(-1-(yii - (double) yi),2) + pow(0-(xii - (double) xi),2)));
				dists.push_back(sqrt(pow(-1-(yii - (double) yi),2) + pow(1-(xii - (double) xi),2)));
				
// if(stophere) cout << endl << dists.size() << endl;
// if(stophere) cout << cgvals.size() << endl;
			
			// 1 - dist/maxdist
// if(stophere) double maxdist = *max_element(dists.begin(), dists.end());
			double sumdist = accumulate(dists.begin(), dists.end(), 0.0);
// if(stophere) cout << endl << "maxdist=" << maxdist << endl;
// if(stophere) cout << endl << "sumdist=" << sumdist << endl;
			double sumdistsscaled = 0;
			for(long unsigned int veci = 0; veci < cgvals.size() ; veci++) {
				if(dists[veci] == 0)
					dists[veci] = 0.0001;
				sumdistsscaled += 1/(dists[veci]/sumdist);
				dists[veci] = 1/(dists[veci]/sumdist);
			}
// if(stophere) cout << endl << "sumdistsscaled=" << sumdistsscaled << endl;
			double weightedmean = 0;
			double weightedmean_soilhumidity = 0;
			for(long unsigned int veci = 0; veci < cgvals.size() ; veci++) {
				cgvals[veci] = cgvals[veci] * (dists[veci]/sumdistsscaled);
				weightedmean += cgvals[veci];
				cgvals_soilhumidity[veci] = cgvals_soilhumidity[veci] * (dists[veci]/sumdistsscaled);
				weightedmean_soilhumidity += cgvals_soilhumidity[veci];
			}
			
// if(stophere) cout << " ... ... dists and cgvals ... ... " << endl;
// if(stophere) for( int veci = 0; veci < cgvals.size() ; veci++) {
	// cout << "veci=" << veci << ": d=" << dists[veci] << "  & ald=" << cgvals[veci] << endl;
	// }
			// weightedmean = weightedmean/cgvals.size();
// if(stophere) cout << "weightedmean=" << weightedmean << endl;
			
// if(stophere) cout << endl;
			double maxthawing_depth_local = 0;
			double soilhumidity_local = 0;
			if(parameter[0].cryogrid_scenario == 0) { // setup linear model for interpolation
				maxthawing_depth_local = weightedmean; // internal stored /10
				soilhumidity_local = weightedmean_soilhumidity; // internal stored /100			
			} else { // calculate active layer depth anomalie in each of the three conditions
// cout << "wmean:" << weightedmean << "+ALT:" << pEnvirgrid.maxthawing_depth ;
				maxthawing_depth_local = ((double)pEnvirgrid.maxthawing_depth/10 - weightedmean); // internal stored /10
				soilhumidity_local = ((double)pEnvirgrid.soilhumidity/100 - weightedmean_soilhumidity); // internall stored /100				
// cout << " ==> ALT:" << pEnvirgrid.maxthawing_depth << endl;
// if(pEnvirgrid.maxthawing_depth <=0)
 // cout << pEnvirgrid.maxthawing_depth << "|wmean="<< weightedmean << "sumdist=" << sumdist << "cgvals.size()=" << cgvals.size() << "sumdist=" << sumdist;
 // stophere=true;
 // else
 // stophere=false;
 
 
/* for R code testing



after=read.csv2("//dmawi/potsdam/data/legacy/Model/Modelling/CryogridLAVESI/CouplingMaster/LAVESI/output_4/datacryogrid_1_1210001_100_1.csv",dec=".")
before=read.csv2("//dmawi/potsdam/data/legacy/Model/Modelling/CryogridLAVESI/CouplingMaster/LAVESI/output_4/datacryogrid_1_1210001_100_1_beforeupdate.csv",dec=".")
 str(before)
 with(before,lattice::levelplot(maxthawing_depth~x+y))
 with(after,lattice::levelplot(maxthawing_depth~x+y))
 with(before,summary(maxthawing_depth))
 with(after,summary(maxthawing_depth))
 
 with(before,summary(soilhumidity))
 with(after,summary(soilhumidity))
 
 
 
*/
			}
			
			if (maxthawing_depth_local < 0) // negative values
				maxthawing_depth_local = 0.0;
			if (maxthawing_depth_local > 1000*10) // unreal large
				maxthawing_depth_local = 0.0; 
			if (soilhumidity_local < 0) // negative values
				soilhumidity_local = 0;
			if (soilhumidity_local > 100*100) // unreal large
				soilhumidity_local = 100*100; 
				
			pEnvirgrid.maxthawing_depth = maxthawing_depth_local * 10; // internal stored /10
			pEnvirgrid.soilhumidity = soilhumidity_local * 100; // internal stored /100		
		}
	}
}

void ResetMaps(int yearposition, vector<Envirgrid>& plot_list, vector<Weather>& weather_list) {
    const auto loop_size = static_cast<std::size_t>(treerows) * static_cast<std::size_t>(parameter[0].sizemagnif) * static_cast<std::size_t>(treecols) * static_cast<std::size_t>(parameter[0].sizemagnif);
	if (parameter[0].thawing_depth == true) {
        RandomNumber<double> uniform(0, 1);

#pragma omp parallel for default(shared) private(uniform) schedule(guided)
		for (std::size_t kartenpos = 0; kartenpos < loop_size; ++kartenpos) {
			auto& pEnvirgrid = plot_list[kartenpos];

			if (parameter[0].litterlayer==true) {
				// stochastic disturbance effect
				auto rn = uniform.draw();
				if ( (rn < 0.2) & (rn >= 0.1) )
					pEnvirgrid.litterheight0 = (double)pEnvirgrid.litterheight0 * 0.9;
				else if ( (rn < 0.1) & (rn >= 0.01) )
					pEnvirgrid.litterheight0 = (double)pEnvirgrid.litterheight0 * 0.75;
				else if ( (rn < 0.01) & (rn >= 0.001) )
					pEnvirgrid.litterheight0 = (double)pEnvirgrid.litterheight0 * 0.5;
				else if ( (rn < 0.001) & (rn >= 0.0001) )
					pEnvirgrid.litterheight0 = (double)pEnvirgrid.litterheight0 * 0.1;
				else if (rn < 0.0001)
					pEnvirgrid.litterheight0 = (double)pEnvirgrid.litterheight0 * 0.01;

				unsigned short litterlayergrowthrate = 0.5 * 100;	//0.5 cm * 100 wegen Skalierung; evt mit TWI verknüpfen oder evt Lärchenwachstum
											 // +( 1.0/( ((1.0/0.01)-(1.0/0.95))
													  // *exp(-(1.0/2000.0)*(double) pEnvirgrid.maxthawing_depth) 
													  // +(1/0.95)) ); 
				
				pEnvirgrid.litterheight0+= litterlayergrowthrate;
				
				if (pEnvirgrid.litterheight0 > 3000) //1000 (= 10 cm)
					pEnvirgrid.litterheight0 = 3000;

				pEnvirgrid.litterheightmean = (unsigned short) ( (double) 
												 (pEnvirgrid.litterheight9
												 +pEnvirgrid.litterheight8
												 +pEnvirgrid.litterheight7
												 +pEnvirgrid.litterheight6
												 +pEnvirgrid.litterheight5
												 +pEnvirgrid.litterheight4
												 +pEnvirgrid.litterheight3
												 +pEnvirgrid.litterheight2
												 +pEnvirgrid.litterheight1
												 +pEnvirgrid.litterheight0)
												 /10.0);

				pEnvirgrid.litterheight9 = pEnvirgrid.litterheight8;
				pEnvirgrid.litterheight8 = pEnvirgrid.litterheight7;
				pEnvirgrid.litterheight7 = pEnvirgrid.litterheight6;
				pEnvirgrid.litterheight6 = pEnvirgrid.litterheight5;
				pEnvirgrid.litterheight5 = pEnvirgrid.litterheight4;
				pEnvirgrid.litterheight4 = pEnvirgrid.litterheight3;
				pEnvirgrid.litterheight3 = pEnvirgrid.litterheight2;
				pEnvirgrid.litterheight2 = pEnvirgrid.litterheight1;
				pEnvirgrid.litterheight1 = pEnvirgrid.litterheight0;
			}

			// double daempfung = (1.0 / 4000.0) * 200;  // 1/4000 =slope to reach the maximum value at appr. 4000
			// double daempfung = (1.0 / 4000.0) * (double)pEnvirgrid.litterheightmean;  // 1/4000 =slope to reach the maximum value at appr. 4000
			double daempfung = (1.0 / 3000.0) * (double)pEnvirgrid.litterheightmean;  // 1/4000 =slope to reach the maximum value at appr. 4000 (<- value before)

			if (daempfung >= 0.9) {
				daempfung = 0.9;
			}

			double elefactor = 0.0;
			if (parameter[0].demlandscape) // calculate relative elevation position
				elefactor = ((double)pEnvirgrid.elevation / 10) / (1000 + parameter[0].elevationoffset);

			const unsigned short maxthawing_depth =
				// 1000.0 * (1.0 - daempfung) * 0.050 * weather_list[yearposition].degreday_sqrt;  // 1000 (scaling from m to mm)*edaphicfactor=0.050 (SD=0.019)
				100 * (1.0 - daempfung) * 0.050 * std::sqrt(
					weather_list[yearposition].degreday + elefactor*(weather_list[yearposition].degredaymin - weather_list[yearposition].degreday) // reduction based on per 1000 m
				); // 100 (scaling from m to cm)*edaphicfactor=0.050 (SD=0.019)
// cout << maxthawing_depth << " | " << daempfung << " | " << weather_list[yearposition].degreday<< " & " << weather_list[yearposition].degredaymin << " & " << elefactor << " & " << ((double)pEnvirgrid.elevation / 10) << endl;
			pEnvirgrid.maxthawing_depth = maxthawing_depth * 10; // *10 for internal storage conversion
			pEnvirgrid.Treedensityvalue = 0;
			pEnvirgrid.Treenumber = 0;
			// pEnvirgrid.soilhumidity = 30 * 100; // *100 for internal storage conversion; reset to start value
		}
	} else {
#pragma omp parallel for default(shared) schedule(guided)
        for (std::size_t kartenpos = 0; kartenpos < loop_size; ++kartenpos) {
            auto& pEnvirgrid = plot_list[kartenpos];
            pEnvirgrid.Treedensityvalue = 0;
            pEnvirgrid.Treenumber = 0;
			// pEnvirgrid.soilhumidity = 30 * 100; // *100 for internal storage conversion; reset to start value
        }
    }
}

void Disturbance(VectorList<Tree>& tree_list, vector<Envirgrid>& plot_list) {
	cout << "\n... called disturbance with scenario: " << parameter[0].cryogrid_scenario << endl;
 // ... for each scenario mean over 3 condition
				// natural						albedo		organic layer thick		adtreemort	defoliation
					// 11 {x<1666}				0.15		1						0			0
					// 12 {1666<x<3333}			0.15		1						0			0
					// 13 {x>3333}				0.15		1						0			0
				// logging
					// 21 {x<1666}				0.15		1						0.25		0
					// 22 {1666<x<3333}			0.15		1						0.50		0
					// 23 {x>3333}				0.15		1						1.00		0
				// surface fire
					// 31 {x<1666}				0.4			1						0.12		0
					// 32 {1666<x<3333}			0.4			0.1						0.20		0
					// 33 {x>3333}				0.4			0						0.50		0
				// crown fire
					// 41 {x<1666}				0.4			0.1						0.60		1.00
					// 42 {1666<x<3333}			0.4			0						0.75		1.00
					// 43 {x>3333}				0.4			0						1.00		1.00
				// insects/pests
					// 51 {x<1666}				0.15		1						0			0.10
					// 52 {1666<x<3333}			0.15		1						0.06		0.50
					// 53 {x>3333}				0.15		1						0.10		1.00

	RandomNumber<double> uniform(0, 1);

	// litter layer removal
	const auto loop_size = static_cast<std::size_t>(treerows) * static_cast<std::size_t>(parameter[0].sizemagnif) * static_cast<std::size_t>(treecols)* static_cast<std::size_t>(parameter[0].sizemagnif);
#pragma omp parallel for default(shared) private(uniform) schedule(guided)
	for (std::size_t kartenpos = 0; kartenpos < loop_size; ++kartenpos) {
		auto& pEnvirgrid = plot_list[kartenpos];
		
		double ycoo=floor( (double) kartenpos/(treecols*parameter[0].sizemagnif) );
		double xcoo=(double) kartenpos - (ycoo * (treecols*parameter[0].sizemagnif));
		xcoo = xcoo / parameter[0].sizemagnif; // in m
		
		if (xcoo < ((double)treecols/3)) {
			if (parameter[0].cryogrid_scenario == 4)
				pEnvirgrid.litterheight0 = 0.1 * pEnvirgrid.litterheight0;			
		} else if ( (xcoo >= ((double)treecols/3)) && (xcoo < (2 * (double)treecols/3)) ) {
			if (parameter[0].cryogrid_scenario == 3)
				pEnvirgrid.litterheight0 = 0.1 * pEnvirgrid.litterheight0;	
			else if (parameter[0].cryogrid_scenario == 4)
				pEnvirgrid.litterheight0 = 0.0;	
		} else if (xcoo >= (2 * (double)treecols/3)) {
			if (parameter[0].cryogrid_scenario == 3)
				pEnvirgrid.litterheight0 = 0;						
			else if (parameter[0].cryogrid_scenario == 4)
				pEnvirgrid.litterheight0 = 0.0;	
		}
	}
	
	// tree mortality and defoliation (latter impact on tree AGB/needle calculation)
#pragma omp parallel for default(shared) private(uniform) schedule(guided)
    for (unsigned int tree_i = 0; tree_i < tree_list.size(); ++tree_i) {
        auto& tree = tree_list[tree_i];

		if (tree.growing == true) {
			double xcoo = (double)tree.xcoo / 1000;
// cout << floor(xcoo) << " ";
			
			if (xcoo < ((double)treecols/3)) {
				if ( (parameter[0].cryogrid_scenario == 2) && (uniform.draw() < 0.25) ) {
					tree.growing = false;
					tree_list.remove(tree_i);
				} else if ( (parameter[0].cryogrid_scenario == 3) && (uniform.draw() < 0.12) ) {
					tree.growing = false;
					tree_list.remove(tree_i);
				} else if (parameter[0].cryogrid_scenario == 4) {
					if(uniform.draw() < 0.60) {
						tree.growing = false;
						tree_list.remove(tree_i);
					} else {
						tree.crownstart = tree.height;
					}
				} else if (parameter[0].cryogrid_scenario == 5) {
					tree.crownstart = 0.10 * tree.height;
				}
			} else if ( (xcoo >= ((double)treecols/3)) & (xcoo < (2 * (double)treecols/3)) ) {
				if ( (parameter[0].cryogrid_scenario == 2) && (uniform.draw() < 0.50) ) {
					tree.growing = false;
					tree_list.remove(tree_i);
				} else if ( (parameter[0].cryogrid_scenario == 3) && (uniform.draw() < 0.20) ) {
					// tree.growing = false;
					tree_list.remove(tree_i);
				} else if (parameter[0].cryogrid_scenario == 4) {
					if (uniform.draw() < 0.75) {
						tree.growing = false;
						tree_list.remove(tree_i);
					} else {
						tree.crownstart = tree.height;
					}
				} else if (parameter[0].cryogrid_scenario == 5) {
					if (uniform.draw() < 0.06) {
						tree.growing = false;
						tree_list.remove(tree_i);
					} else {
						tree.crownstart = 0.50 * tree.height;
					}
				}
			} else if (xcoo >= (2 * (double)treecols/3)) {
				if (parameter[0].cryogrid_scenario == 2) {
					tree.growing = false;
					tree_list.remove(tree_i);
				} else if ( (parameter[0].cryogrid_scenario == 3) && (uniform.draw() < 0.50) ) {
					tree.growing = false;
					tree_list.remove(tree_i);
				} else if (parameter[0].cryogrid_scenario == 4) {
					tree.growing = false;
					tree_list.remove(tree_i);
				} else if (parameter[0].cryogrid_scenario == 5) {
					if (uniform.draw() < 0.10) {
						tree.growing = false;
						tree_list.remove(tree_i);
					} else {
						tree.crownstart = tree.height;
					}
				}
			}
			
			// update relative crown damage for mortality
			tree.relcrowndamage = ((tree.crownstart / 10) / (tree.height / 10))*1000;
		}
	}
	tree_list.consolidate();

}

void Environmentupdate(//Parameter* parameter,
                       int yearposition,
                       vector<vector<Envirgrid>>& world_plot_list,
                       vector<VectorList<Tree>>& world_tree_list,
                       vector<vector<Weather>>& world_weather_list,
					   vector<vector<Cryogrid>>& world_cryo_list) {
    int aktort = 0;

    for (vector<vector<Envirgrid>>::iterator posw = world_plot_list.begin(); posw != world_plot_list.end(); ++posw) {
        vector<Envirgrid>& plot_list = *posw;

        vector<VectorList<Tree>>::iterator world_positon_b = (world_tree_list.begin() + aktort);
        VectorList<Tree>& tree_list = *world_positon_b;

        vector<vector<Weather>>::iterator posiwelt = (world_weather_list.begin() + aktort);
        vector<Weather>& weather_list = *posiwelt;

		vector<vector<Cryogrid> >::iterator worldposcryogrid = (world_cryo_list.begin()+aktort);
		vector<Cryogrid>& cryo_list = *worldposcryogrid;

        aktort++;


		ResetMaps(yearposition, plot_list, weather_list);
// unsigned int miniter = treerows*parameter[0].sizemagnif* 100*parameter[0].sizemagnif +100*parameter[0].sizemagnif;//100 m vom rand
// unsigned int maxiter = miniter+5;
// const auto loop_size = static_cast<std::size_t>(treerows) * static_cast<std::size_t>(parameter[0].sizemagnif) * static_cast<std::size_t>(treecols) * static_cast<std::size_t>(parameter[0].sizemagnif);
// unsigned int iter=0;
// cout << "-> Thawing depths after ResetMaps: ";
// for (std::size_t kartenpos = 0; kartenpos < loop_size; ++kartenpos) {
	// iter++;
	// if(iter > miniter & iter < maxiter)
		// cout << plot_list[kartenpos].maxthawing_depth << " ";
// }
// iter=0;
// cout << endl;
		
		// impact function
		if( parameter[0].ivort == parameter[0].cryogrid_disturbanceyear )
			Disturbance(tree_list,plot_list);

		if( (parameter[0].cryogrid_thawing_depth==true) ) { // external update of active layer thickness values
		
			// if( (parameter[0].cryogridcalled == true) | (parameter[0].ivort % 20 == 0) ) {
			if( ((parameter[0].outputmode == 14) && (parameter[0].ivort % 50 == 0)) || (parameter[0].ivort == parameter[0].cryogrid_firstyear) || (parameter[0].cryogridcalled == true) ) {
				PrepareCryogrid(tree_list, cryo_list, plot_list);		// collect information of trees
				cout << " -> called ... PrepareCryogrid " << endl;
			}

			// if( (parameter[0].ivort % 20 == 0) ) {
			// if( (parameter[0].ivort == parameter[0].cryogrid_firstyear) || (parameter[0].ivort >= parameter[0].cryogrid_disturbanceyear) ) {
			if( (parameter[0].ivort == parameter[0].cryogrid_firstyear) || (parameter[0].ivort >= parameter[0].cryogrid_firstyear) ) {
				UpdateCryogrid(cryo_list);		// export data and call Cryogrid instance and collect back output
				parameter[0].cryogridcalled = true;
				cout << " -> called ... UpdateCryogrid " << endl;
			}

			if( parameter[0].cryogridcalled == true ) {
				UpdateEnvirgridALD(cryo_list, plot_list, weather_list, yearposition);	// interpolate for Envirgrid-tiles from Cryogrid active layer depth, use former estimation values after once called CryoGrid
				cout << " -> called ... UpdateEnvirgridALD " << endl;
			}
// cout << "-> Thawing depths after UpdateEnvirgridALD: ";
// for (std::size_t kartenpos = 0; kartenpos < loop_size; ++kartenpos) {
	// iter++;
	// if(iter > miniter & iter < maxiter)
		// cout << plot_list[kartenpos].maxthawing_depth << " ";
// }
// iter=0;
// cout << endl;
		}

        AddTreeDensity(tree_list, plot_list);

        IndividualTreeDensity(tree_list, plot_list);
    }
}
