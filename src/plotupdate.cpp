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
					tree.densitywert = tree.densitywert * (-1.0 / 130.0 * (double)tree.height / 100 + 1.0);
				} else if (parameter[0].dichtheightrel == 2) {
					tree.densitywert = tree.densitywert * (-1.0 / 200.0 * (double)tree.height / 100 + 1.0);
				} else if (parameter[0].dichtheightrel == 3) {
					double hrelbuf = (-1.0 / 200.0 * (double)tree.height / 100 + 1.0);
					if (hrelbuf < 0.1)
						hrelbuf = 0.1;
					tree.densitywert = tree.densitywert * hrelbuf;
				} else if (parameter[0].dichtheightrel == 4) {
					if ((double)tree.height / 100 < 40)
						tree.densitywert = 1.0;
					else if (((double)tree.height / 100 >= 40) && ((double)tree.height / 100 < 200))
						tree.densitywert = 0.5;
					else if ((double)tree.height / 100 >= 200)
						tree.densitywert = 0.0;
				} else if (parameter[0].dichtheightrel == 5) {
					if ((double)tree.height / 100 < 40)
						tree.densitywert = 1.0;
					else if (((double)tree.height / 100 >= 40) && ((double)tree.height / 100 < 200))
						tree.densitywert = 0.55;
					else if ((double)tree.height / 100 >= 200)
						tree.densitywert = 0.1;
				} else if (parameter[0].dichtheightrel == 6) {
					if ((double)tree.height / 100 < 40)
						tree.densitywert = 1.0;
					else if (((double)tree.height / 100 >= 40) && ((double)tree.height / 100 < 200))
						tree.densitywert = 0.9;
					else if ((double)tree.height / 100 >= 200)
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
// if(tree.height/100 > 130) cout << tree.twi << "\t" << tree.soilhumidity << "\t" << tree.envirimpact << endl;
				tree.soilhumidity = pow(tree.twi/(6.25*100), 0.5*0.25) * tree.soilhumidity;
// if(tree.height/100 > 130) cout << " ... between: " << tree.twi << "\t" << tree.soilhumidity << "\t" << tree.envirimpact << endl;
				if( (tree.soilhumidity < speciestrait[tree.species].minsoilwater*100) | (tree.soilhumidity > speciestrait[tree.species].maxsoilwater*100) )
					tree.soilhumidity=0;
				else
					tree.soilhumidity=1;
// if(tree.height/100 > 130) cout << " ... after: " << tree.twi << "\t" << tree.soilhumidity << "\t" << tree.envirimpact << endl;
		}
	}
}

/****************************************************************************************/
/**
 * \brief reset Maps "Envirgrid"
 *
 * reset Treedensity und Treenumber \n
 * calculate litterlayer height if litterlayer==true
 *\n calculate max active layer depth
 *
 *
 *******************************************************************************************/

void ResetMaps(int yearposition, vector<Envirgrid>& plot_list, vector<Weather>& weather_list) {
    const auto loop_size = static_cast<std::size_t>(treerows) * static_cast<std::size_t>(parameter[0].sizemagnif) * static_cast<std::size_t>(treecols)
                           * static_cast<std::size_t>(parameter[0].sizemagnif);

	if (parameter[0].thawing_depth == true) {
        RandomNumber<double> uniform(0, 1);

#pragma omp parallel for default(shared) private(uniform) schedule(guided)
		for (std::size_t kartenpos = 0; kartenpos < loop_size; ++kartenpos) {
			auto& pEnvirgrid = plot_list[kartenpos];

			if (parameter[0].litterlayer==true) {
				// stochastic disturbance effect
				auto rn = uniform.draw();
				if ( (rn < 0.2) & (rn >= 0.1) )
					pEnvirgrid.litterheight0 = pEnvirgrid.litterheight0 * 0.9;
				else if ( (rn < 0.1) & (rn >= 0.01) )
					pEnvirgrid.litterheight0 = pEnvirgrid.litterheight0 * 0.75;
				else if ( (rn < 0.01) & (rn >= 0.001) )
					pEnvirgrid.litterheight0 = pEnvirgrid.litterheight0 * 0.5;
				else if ( (rn < 0.001) & (rn >= 0.0001) )
					pEnvirgrid.litterheight0 = pEnvirgrid.litterheight0 * 0.1;
				else if (rn < 0.0001)
					pEnvirgrid.litterheight0 = pEnvirgrid.litterheight0 * 0.01;

				unsigned short litterlayergrowthrate =0.5 * 100;	//0.5 cm * 100 wegen Skalierung; evt mit TWI verknüpfen oder evt Lärchenwachstum
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
			if (parameter[0].demlandscape)
				// calculate relative elevatio position
				elefactor = ((double)pEnvirgrid.elevation / 10) / (1000 + parameter[0].elevationoffset);

			const unsigned short maxthawing_depth =
				// 1000.0 * (1.0 - daempfung) * 0.050 * weather_list[yearposition].degreday_sqrt;  // 1000 (scaling from m to mm)*edaphicfactor=0.050 (SD=0.019)
				1000.0 * (1.0 - daempfung) * 0.050*4 * std::sqrt(
				
					weather_list[yearposition].degreday + elefactor*(weather_list[yearposition].degredaymin - weather_list[yearposition].degreday) // reduction based on per 1000 m
				
				); // 1000 (scaling from m to mm)*edaphicfactor=0.050 (SD=0.019) // factor 4 assumed use for tuning ALT
// cout << maxthawing_depth << " | " << daempfung << " | " << weather_list[yearposition].degreday<< " & " << weather_list[yearposition].degredaymin << " & " << elefactor << " & " << ((double)pEnvirgrid.elevation / 10) << endl;
			pEnvirgrid.maxthawing_depth = maxthawing_depth;
			pEnvirgrid.Treedensityvalue = 0;
			pEnvirgrid.Treenumber = 0;
		}
	} else {
#pragma omp parallel for default(shared) schedule(guided)
        for (std::size_t kartenpos = 0; kartenpos < loop_size; ++kartenpos) {
            auto& pEnvirgrid = plot_list[kartenpos];
            pEnvirgrid.Treedensityvalue = 0;
            pEnvirgrid.Treenumber = 0;
			pEnvirgrid.fire = 0; 		// ###FIRE### int variant
			pEnvirgrid.envirfireimpact = 0; 		// ###FIRE### int variant
        }
    }
}

/****************************************************************************************/
/**
 * \brief update density maps and active layer depth
 *
 *
 *
 *
 *******************************************************************************************/

void Environmentupdate(//Parameter* parameter,
                       int yearposition,
                       vector<vector<Envirgrid>>& world_plot_list,
                       vector<VectorList<Tree>>& world_tree_list,
                       vector<vector<Weather>>& world_weather_list) {
    int aktort = 0;

    for (vector<vector<Envirgrid>>::iterator posw = world_plot_list.begin(); posw != world_plot_list.end(); ++posw) {
        vector<Envirgrid>& plot_list = *posw;

        vector<VectorList<Tree>>::iterator world_positon_b = (world_tree_list.begin() + aktort);
        VectorList<Tree>& tree_list = *world_positon_b;

        vector<vector<Weather>>::iterator posiwelt = (world_weather_list.begin() + aktort);
        vector<Weather>& weather_list = *posiwelt;

        aktort++;

        ResetMaps(yearposition, plot_list, weather_list);

        AddTreeDensity(tree_list, plot_list);

        IndividualTreeDensity(tree_list, plot_list);
    }
}
