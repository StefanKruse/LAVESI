#include "LAVESI.h"
#include "RandomNumber.h"
#include "VectorList.h"
#include <optional>

using namespace std;

void Pestoutbreak(Parameter* parameter,
                       int yearposition,
                       vector<VectorList<Tree>>& world_tree_list,
                       vector<vector<Weather>>& world_weather_list) {
						   
	int aktort = 0;
    for (vector<vector<Weather>>::iterator posw = world_weather_list.begin(); posw != world_weather_list.end(); ++posw) {
        vector<Weather>& weather_list = *posw;
        vector<VectorList<Tree>>::iterator world_positon_b = (world_tree_list.begin() + aktort);
        VectorList<Tree>& tree_list = *world_positon_b;
        aktort++;
		
		RandomNumber<double> uniform(0, 1);
		
		// structures for output
		vector<unsigned short int> out_pestoutbreak; // true == 1 
		// assemble file name:
		string filename;
		filename = "output/pestoutbreaks.csv";
		
		// trying to open the file for reading
		FILE* filepointer;
		filepointer = fopen(filename.c_str(), "r+");
		// if fopen fails, open a new file + header output
		if (filepointer == NULL) {
			filepointer = fopen(filename.c_str(), "w+");

			fprintf(filepointer, "Progress;");
			fprintf(filepointer, "Yearposition;");
			fprintf(filepointer, "Pestspeciesname;");
			fprintf(filepointer, "Pestnumber;");
			fprintf(filepointer, "Probability;");
			fprintf(filepointer, "OutbreakIf1;");
			fprintf(filepointer, "\n");

			if (filepointer == NULL) {
				fprintf(stderr, "Error: output file for pest outbreaks is missing!\n");
				exit(1);
			}
		}

		// use weather list and pest outbreak probability to compute wether a pest outbreak happens or not. 
		for(unsigned short int pestspeciesi=1;pestspeciesi<=parameter[0].pest_species_max;pestspeciesi++) {
			cout << " ... processing pestspecies (#=" <<  pestspeciesi << ") : " << pesttrait[pestspeciesi].pestspeciesname << endl;
			
			cout << " weather_list[yearposition].pestoutbreakprobability[pestspeciesi-1] = " << weather_list[yearposition].pestoutbreakprobability[pestspeciesi-1] << endl; 
			double pestproba_i = weather_list[yearposition].pestoutbreakprobability[pestspeciesi-1];//first elememt is 0
			if(uniform.draw() < pestproba_i) {//outbreak happens
				out_pestoutbreak.push_back(1); // record for output
				// assess which species are affected and how
				// depending on the pest species number check which trees are affected and by which +mortality=> pestinfectancedamage(fraction 0 to 1, 1000 precision) and +defoliation => defoliation(fraction 0 to 1, 1000 precision)
				vector<int> treespeciesaffected;		// number of the species following the specieslist definition
				vector<double> treespeciesmortality;	// 0 or fration to 1
				vector<double> treespeciesdefoliation; 	// 0 or fraction to 1
				vector<double> treespeciesdefoliationmort; 	// 0 or fraction to 1
				
				if(pestspeciesi == 1) {//pest == aspen_leafminer
					treespeciesaffected.push_back(22);//POBA
					treespeciesmortality.push_back(0.0);
					treespeciesdefoliation.push_back(30.75);
					treespeciesdefoliationmort.push_back(14.44); // percentage defoliated, will be directly increase moratlity
					
					treespeciesaffected.push_back(21);//POTR
					treespeciesmortality.push_back(0.0);
					treespeciesdefoliation.push_back(31.0);
					treespeciesdefoliationmort.push_back(14.44);
				}
				if(pestspeciesi == 2) {//pest == spruce_beetle
					treespeciesaffected.push_back(18);//PIMA 
					treespeciesmortality.push_back(25.30769231);
					treespeciesdefoliation.push_back(0.0);
					treespeciesdefoliationmort.push_back(0.0);
					
					treespeciesaffected.push_back(19);// PISI 
					treespeciesmortality.push_back(21.68571429);
					treespeciesdefoliation.push_back(0.0);
					treespeciesdefoliationmort.push_back(0.0);
					
					treespeciesaffected.push_back(20);//PIGL
					treespeciesmortality.push_back(28.8);
					treespeciesdefoliation.push_back(0.0);
					treespeciesdefoliationmort.push_back(0.0);
				}
				// if(pestspeciesi == 3) {//pest == willow_leaf_blotchminer -> host salix currently not simulated
				// }
				if(pestspeciesi == 4) {//pest == western_blackheaded_budworm
					treespeciesaffected.push_back(19);//PISI 
					treespeciesmortality.push_back(0.0);
					treespeciesdefoliation.push_back(50.0);// second defoliation not considered
					treespeciesdefoliationmort.push_back(10.0);
					
					treespeciesaffected.push_back(17);// TSME
					treespeciesmortality.push_back(0.0);
					treespeciesdefoliation.push_back(38.09090909);
					treespeciesdefoliationmort.push_back(7.0);
				}
				if(pestspeciesi == 5) {//pest == larch_sawfly
					treespeciesaffected.push_back(23);//LALA
					treespeciesmortality.push_back(0.0);
					treespeciesdefoliation.push_back(30.0);
					treespeciesdefoliationmort.push_back(80.0);
				}
				if(pestspeciesi == 6) {//pest == hemlock_sawfly
					treespeciesaffected.push_back(17);//TSME 
					treespeciesmortality.push_back(28.8);
					treespeciesdefoliation.push_back(28.8);
					treespeciesdefoliationmort.push_back(7.0);
					
					treespeciesaffected.push_back(19);// PISI
					treespeciesmortality.push_back(28.8);
					treespeciesdefoliation.push_back(28.8);
					treespeciesdefoliationmort.push_back(7.0);
				}
				if(pestspeciesi == 7) {//pest == birch_leafminer
					treespeciesaffected.push_back(16);//BENE
					treespeciesmortality.push_back(0.0);
					treespeciesdefoliation.push_back(37.38297872);
					treespeciesdefoliationmort.push_back(10.0);
				}
				
				// impact
				if(treespeciesaffected.size() > 0) {// if any treespecies are impacted
					for(unsigned short int i=0;i<=treespeciesaffected.size();i++) {//for each element in treespeciesaffected
						unsigned short int treespeciesaffected_i = treespeciesaffected[i];
						// for each tree add impact
#pragma omp parallel for default(shared) private(uniform) schedule(guided)
						for (unsigned int tree_i = 0; tree_i < tree_list.size(); ++tree_i) {
							auto& tree = tree_list[tree_i];
							if(tree.species == treespeciesaffected_i && (uniform.draw() < (treespeciesmortality[i]/100))) {//impact mortality
								tree.pestinfectancedamage = parameter[0].pest_disturbances_impactfactor * 1.00 * 1000; // 100 percent mortality
							}
							if(tree.species == treespeciesaffected_i && (uniform.draw() < (treespeciesdefoliation[i]/100))) {//impact mortality
								tree.relcrowndamage += parameter[0].pest_disturbances_impactfactor * (treespeciesdefoliationmort[i]/100) * 1000;//add to the damage by fire
							}
						}// end loop each tree
					}// end loop impacted species
				}// if any treespecies are impacted
				
				
			} // end outbreak happens
			else {
				out_pestoutbreak.push_back(0); // record for output
			}
		}// end loop pest species		   
		
		// data evaluation and output		
		fseek(filepointer, 0, SEEK_END);
		for(unsigned short int pestspeciesi=1;pestspeciesi<=parameter[0].pest_species_max;pestspeciesi++) {
			fprintf(filepointer, "%d;", parameter[0].ivort);
			fprintf(filepointer, "%d;", yearposition);
			fprintf(filepointer, "%s;", pesttrait[pestspeciesi].pestspeciesname.c_str());
			fprintf(filepointer, "%d;", pesttrait[pestspeciesi].pestspecies);
			fprintf(filepointer, "%4.4f;", weather_list[yearposition].pestoutbreakprobability[pestspeciesi-1]);
			fprintf(filepointer, "%d;", out_pestoutbreak[pestspeciesi-1]);
			fprintf(filepointer, "\n");
		}
		fclose(filepointer);
	}// end world loop


}// end function

/* 
///Appendix
// TSME = mountain hemplock
// TSHE = western hemlock => not considered!
// ABAM = pacific silver fir => not in Alaska present
Disturbance		Host			abbreviation	effect	mean_percentage_affected	mortality_after_defoliation	
aspen_leafminer	balsam_poplar	POBA	defoliation		30.75						14.44	
aspen_leafminer	quakin_aspen	POTR	defoliation		31							14.44	
birch_leafminer	birch_spp		BENE	defoliation		37.38297872					10	
// hemlock_sawfly	western_hemlock	TSHE	defoliation	28.8	4.6	
// hemlock_sawfly	western_hemlock	TSHE	mortality	28.8		
hemlock_sawfly	mountain_hemlock._Sitka_spruce._pacific_silver_fir	TSME,PISI,ABAM	mortality	28.8		
hemlock_sawfly	mountain_hemlock._Sitka_spruce._pacific_silver_fir	TSME,PISI,ABAM	defoliation	28.8	7	estimated_value_
spruce_beetle	black_spruce	PIMA	mortality	25.30769231		
spruce_beetle	Sitka_spruce	PISI	mortality	21.68571429		
// DELETED.duplicate spruce_beetle	spruce_spp.		PIMA+	mortality	27.30769231		
spruce_beetle	white_spruce	PIGL	mortality	28.8		
western_blackheaded_budworm	Sitka_spruce	PISI	defoliation	50	10	estimated_value
western_blackheaded_budworm	western_hemlock	TSHE,	defoliation	35.08333333	4.6	
western_blackheaded_budworm	western_hemlock._mountain_hemlock._Sitka_spruce	TSME,TSHE,PISI	defoliation	38.09090909	7	estimated_value
// willow_leaf_blotchminer	willow_spp.		defoliation	44.47666667	1	estimated_value_according_literature_(https://emrlibrary.gov.yk.ca/forestry/forest_health/forest-insect-and-disease-pamphlets/25-willow-blotch-miner.pdf)
// willow_leaf_blotchminer	?		defoliation	29.16923077		I_only_found_that_the_willow_leaf_blotchminer_affects_willow_trees
larch_sawfly	western_larch	LALA	defoliation	30	80	mean_percentage_estimated._mortality_according_literature_(https://www.srs.fs.usda.gov/pubs/gtr/gtr_srs176/gtr_srs176_143.pdf)


16 BENE 3 10 1 139 0.2 0.16 16 0.01 0.447975 -0.02893947 0.136437824 -3.479336714 -0.02893947 0.136437824 -3.479336714 42.88 1 42.88 1 44.43163 7.02 7.18017 3.16288 0.82948 0.0001 100 8.18785 0.4 0.79295 0.01 0.2 0 0.1 0.2 0.01 0.15 1 0.8 13 -40 6.6 1 15.87896 0 1 40 9 85 100 1 0.01 0 2.4513 2.22271 0 2.4513 2.22271 1 0.48
17 TSME 3 20 1 25 1 0.16 16 0.01 0.447975 -0.001234403 0.040404475 -1.895297753 -0.001234403 0.040404475 -1.895297753 42.88 1 42.88 1 44.43163 7.02 12.878784 5.889165 7.490937 0.0001 500 8.18785 0.4 0.79295 0.01 0.2 0 0.1 0.2 0.01 0.7 0.44724 0.999 3 -7 6.6 1 13.32363 0 1 100 12 55 100 1 0 0 2.5697 -2.29208 0 2.5697 -2.29208 1 0.42
18 PIMA 3 15 0.9 15 1.2 0.16 16 0.01 0.447975 0.06188611 -0.54592642 -2.55988545 0.06188611 -0.54592642 -2.55988545 42.88 1 42.88 1 44.43163 7.02 8.140161 4.122785 1.240902 0.0001 250 8.18785 0.4 0.79295 0.01 0.2 0 0.1 0.2 0.01 0.5 0.44724 0.999 1 -33 6.6 1 14.59999 0 1 200 25 40 200 1.5 0 0 2.3233 -2.31774 0 2.3233 -2.31774 3 0.38
19 PISI 3 20 0.73 30 0.8 0.16 16 0.01 0.447975 -0.001236552 0.046709691 -2.610931447 -0.001236552 0.046709691 -2.610931447 42.88 1 42.88 1 44.43163 7.02 7.4374 3.1562 1.636 0.0001 700 8.18785 0.4 0.79295 0.01 0.2 0 0.1 0.2 0.01 0.7 0.44724 0.999 1 -3 6.6 1 13.42066 0 1 10 25 75 200 1 0 0 2.5567 -2.3123 0 2.5567 -2.3123 1 0.33
20 PIGL 3 15 0.9 15 1.2 0.16 16 0.01 0.447975 0.001918668 -0.062894722 -2.35583974 0.001918668 -0.062894722 -2.35583974 42.88 1 42.88 1 44.43163 7.02 7.267284 3.676405 1.04931 0.0001 250 8.18785 0.4 0.79295 0.01 0.2 0 0.1 0.2 0.01 0.5 0.44724 0.999 1 -33 6.6 1 14.21738 0 1 200 25 40 200 1.5 0 0 2.3233 -2.31774 0 2.3233 -2.31774 3 0.37
21 POTR 3 15 1 500 0.1 0.16 16 0.01 0.447975 0.02056935 -0.13155375 -1.94265259 0.02056935 -0.13155375 -1.94265259 42.88 1 42.88 1 44.43163 7.02 7.679233 3.663742 0.891484 0.0001 200 8.18785 0.4 0.79295 0.01 0.2 0 0.1 0.2 0.01 0.2 0.44724 0.999 0.08 -30 6.6 1 15.16684 0 1 50 10 85 75 1 0 0 2.4561 -2.4441 0 2.4561 -2.4441 1 0.35
22 POBA 3 8 1 200 0.3 0.16 16 0.01 0.447975 -0.1140205 0.6212088 -3.1148959 -0.1140205 0.6212088 -3.1148959 42.88 1 42.88 1 44.43163 7.02 8.009746 3.599828 1.238247 0.0001 140 8.18785 0.4 0.79295 0.01 0.2 0 0.1 0.2 0.01 0.7 0.44724 0.999 0.1 -30 6.6 1 13.82667 0 1 50 4 47 800 1.5 0 0 2.4561 -2.26863 0 2.4561 -2.26863 1 0.31
23 LALA 2 15 0.64 60.1 0.86 0.16 16 0.01 0.45 0 0.15 -1.61 0 0.15 -1.61 42.88 1 42.88 1 44.43 7.02 18.51 3.2 4.35 0.0001 609 8.19 0.4 0.79 0.01 0.2 0 0.1 0.2 0.01 0.24 0.45 0.56 4 -45 9 1 15.1 0 1 20 21.1 40 50 2 0.01 NA 1.53 NA NA 2.41 NA 1 0
*/
