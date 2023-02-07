#include "LAVESI.h"
#include "RandomNumber.h"
#include "VectorList.h"

using namespace std;

void TreeMort(int yearposition_help, vector<Weather>& weather_list, VectorList<Tree>& tree_list) {
    // abiotic influence: calculation of the factors in a function that adds a mortality rate impact
	// multiple species integration
	for (int species_counter = 1; species_counter < 99; species_counter++) {
		if(speciestrait[species_counter].number == 0)
			break;

		speciestrait[species_counter].yearlycalcofanstiegweathermort=(60*weather_list[yearposition_help].janisothermrestriktion[species_counter]+60*weather_list[yearposition_help].julisothermrestriktion+60*weather_list[yearposition_help].nddrestriktion); // access via: speciestrait[tree.species].yearlycalcofanstiegweathermort
		speciestrait[species_counter].yearlycalcofanstiegweathermortmin=(60*weather_list[yearposition_help].janisothermrestriktionmin[species_counter]+60*weather_list[yearposition_help].julisothermrestriktion+60*weather_list[yearposition_help].nddrestriktion); // access via: speciestrait[tree.species].yearlycalcofanstiegweathermort
	}

    RandomNumber<double> uniform(0, 1);
	
#pragma omp parallel for default(shared) private(uniform) schedule(guided)
    for (unsigned int tree_i = 0; tree_i < tree_list.size(); ++tree_i) {
        auto& tree = tree_list[tree_i];

        if (tree.growing == true) {
            double agesmort = 0.0;
			agesmort = tree.age / speciestrait[tree.species].maximumage;
            // if (tree.age > speciestrait[tree.species].maximumage) {  // if maximal age is exceeded an additional factor occurs
                // agesmort = 1.0;
            // }
			
            double windthrowmort = 0.0;
			windthrowmort = speciestrait[tree.species].mwindthrow * ((double)tree.height / 10) / (65535 / 10);// scaled to maximum of height in model of 65 m

			double firecrowndamagemort = (double)tree.relcrowndamage / 1000;
			if(firecrowndamagemort < 0.0)
				firecrowndamagemort = 0.0;
			else if(firecrowndamagemort > 1.0)
				firecrowndamagemort = 1.0 - speciestrait[tree.species].resprouting*speciestrait[tree.species].relbarkthickness;
			tree.relcrowndamage = 0*1000;
// cout << firecrowndamagemort << " ... " << tree.relcrowndamage / 1000 << " | ";
			
            // height dependent influences
            double wachstumrel = 1.0;
            if ((double)tree.height / 10 < 130) {
                wachstumrel = (double)tree.dbasalrel / 1000;
            } else {
                wachstumrel = (double)tree.dbreastrel / 1000;
            }

            // extra competition effect
            double heightnkugeleinfluss = 1;
            if ((double)tree.height / 10 < (speciestrait[tree.species].densityvaluemaximumatheight * 2)) {
                heightnkugeleinfluss =
                    heightnkugeleinfluss
                    + (sqrt(pow(speciestrait[tree.species].densityvaluemaximumatheight, 2) - pow((double)tree.height / 10 - speciestrait[tree.species].densityvaluemaximumatheight, 2))
                       / speciestrait[tree.species].densityvaluemaximumatheight);
            }

            double sapl_mort = speciestrait[tree.species].mortyouth * pow(exp((-1.0 * tree.dbasal) + (double)tree.dbasalmax / 1000), speciestrait[tree.species].mortyouthinfluenceexp);
            double age_mort = speciestrait[tree.species].mortage * agesmort * (10.0 * speciestrait[tree.species].mortbg);
            double growth_mort = speciestrait[tree.species].mgrowth * (1.0 - pow(wachstumrel, parameter[0].relgrowthmortinfluenceexp));
            double dens_mort = speciestrait[tree.species].mdensity * heightnkugeleinfluss * tree.densitywert;

            double weathermortadd;
            if (parameter[0].lineartransect) {
                weathermortadd = 1.0
                                  - (1.0
                                     / (1.0
                                        + (((1.0 - 0.5) / 0.5)
                                           * exp((speciestrait[tree.species].yearlycalcofanstiegweathermort
                                                  - ((speciestrait[tree.species].yearlycalcofanstiegweathermort - speciestrait[tree.species].yearlycalcofanstiegweathermort) * 1.0 / (((double)treerows) / (double)tree.ycoo / 1000)))
                                                 * (weather_list[yearposition_help].weatherfactor[tree.species]
                                                    + ((weather_list[yearposition_help].weatherfactormin[tree.species] - weather_list[yearposition_help].weatherfactor[tree.species])
                                                       * ((double)tree.ycoo / 1000) / ((double)treerows)))
                                                 * exp(speciestrait[tree.species].gdbasalconst + speciestrait[tree.species].gdbasalfac * tree.dbasal
                                                       + speciestrait[tree.species].gdbasalfacq * tree.dbasal * tree.dbasal)))));
            } else if (parameter[0].demlandscape) {
                weathermortadd =
                    1.0
                    - (1.0
                       / (1.0
                          + (((1.0 - 0.5) / 0.5)
                             * exp((speciestrait[tree.species].yearlycalcofanstiegweathermort
                                    - ((speciestrait[tree.species].yearlycalcofanstiegweathermort - speciestrait[tree.species].yearlycalcofanstiegweathermort) * 1.0 / (((double)treerows) / (double)tree.ycoo / 1000)))
                                   * ((weather_list[yearposition_help].weatherfactor[tree.species] * (((double)tree.elevation / 10) - (parameter[0].elevationoffset + 1000))
                                       / (parameter[0].elevationoffset - (parameter[0].elevationoffset + 1000)))
                                      + (weather_list[yearposition_help].weatherfactormin[tree.species]
                                         * (1
                                            - (((double)tree.elevation / 10) - (parameter[0].elevationoffset + 1000))
                                                  / (parameter[0].elevationoffset - (parameter[0].elevationoffset + 1000)))))
                                   * exp(speciestrait[tree.species].gdbasalconst + speciestrait[tree.species].gdbasalfac * tree.dbasal
                                         + speciestrait[tree.species].gdbasalfacq * tree.dbasal * tree.dbasal)))));
            } else {
                weathermortadd = 1.0
                                  - (1.0
                                     / (1.0
                                        + (((1.0 - 0.5) / 0.5)
                                           * exp(speciestrait[tree.species].yearlycalcofanstiegweathermort * weather_list[yearposition_help].weatherfactor[tree.species]
                                                 * exp(speciestrait[tree.species].gdbasalconst + speciestrait[tree.species].gdbasalfac * tree.dbasal
                                                       + speciestrait[tree.species].gdbasalfacq * tree.dbasal * tree.dbasal)))));
            }

            double heightreduce = pow((1.0 / (double)tree.height / 10), speciestrait[tree.species].heightweathermorteinflussexp);  // includes a minimun limit
            if (heightreduce < 0.001)
                heightreduce = 0.001;
            double weather_mort = speciestrait[tree.species].mweather * weathermortadd * heightreduce;

            double dry_mort = 0.0;
            if (parameter[0].lineartransect) {
                dry_mort = speciestrait[tree.species].mdrought
                           * (weather_list[yearposition_help].droughtmort
                              + ((weather_list[yearposition_help].droughtmortmin - weather_list[yearposition_help].droughtmort) * ((double)tree.ycoo / 1000)
                                 / ((double)treerows)))
                           * pow((1.0 / (double)tree.height / 10), 0.5);
            } else if (parameter[0].demlandscape) {
                dry_mort = speciestrait[tree.species].mdrought
                           * ((weather_list[yearposition_help].droughtmort * (((double)tree.elevation / 10) - (parameter[0].elevationoffset + 1000))
                               / (parameter[0].elevationoffset - (parameter[0].elevationoffset + 1000)))
                              + (weather_list[yearposition_help].droughtmortmin
                                 * (1
                                    - (((double)tree.elevation / 10) - (parameter[0].elevationoffset + 1000))
                                          / (parameter[0].elevationoffset - (parameter[0].elevationoffset + 1000)))))
                           * pow((1.0 / (double)tree.height / 10), 0.5);
            } else {
                dry_mort = speciestrait[tree.species].mdrought * weather_list[yearposition_help].droughtmort * (pow((1.0 / (double)tree.height / 10), 0.5)+0.5)*(tree.droughtresist/100);
            }

            // calculating the mortality rate of the tree considering the factors of each mortality rate
            // double treemortality = 0.0 + speciestrait[tree.species].mortbg + sapl_mort + age_mort + growth_mort + dens_mort + weather_mort + dry_mort + windthrowmort + firecrowndamagemort + tree.soilhumidity;
            double treemortality = 0.0 + speciestrait[tree.species].mortbg + sapl_mort + age_mort + growth_mort + dens_mort + weather_mort + dry_mort + windthrowmort + firecrowndamagemort;

			if(tree.soilhumidity == 0.0)
				treemortality += 0.1;
				
            if (treemortality > 1.0) {
                treemortality = 1.0;
            } else if (treemortality < 0.0) {
                treemortality = 0.0;
            }

// if((double)tree.height / 10 > 10)
		// cout << (double)tree.height / 10 << " ..... " << treemortality << " <<< " << wachstumrel << " + " << tree.dbasal << " - " << tree.dbasalrel<< "..." << tree.dbreast << " - " << tree.dbreastrel << " | " << sapl_mort  << " - " <<   age_mort  << " - " <<   growth_mort  << " - " <<   dens_mort  << " - " <<   weather_mort  << " - " <<   dry_mort <<  " - " << tree.soilhumidity << endl;

            // determine if a tree dies
			if ( ((double) uniform.draw() < treemortality) || (tree.envirimpact <= 0) ) {
                tree.growing = false;
                tree_list.remove(tree_i);
                // TODO: alternatively set variables to dead and not growing: negative ages could be used for rotting deadwood
            }
        }
    }
    tree_list.consolidate();
cout << endl;
}

void Mortality(Parameter* parameter,
               // int Jahr,
               int yearposition,
               vector<VectorList<Tree>>& world_tree_list,
               vector<VectorList<Seed>>& world_seed_list,
               vector<vector<Weather>>& world_weather_list,
			   vector<vector<Pollengrid>>& world_pollen_list,
			   vector<vector<Envirgrid>>& world_plot_list) {
    int aktort = 0;
	

    for (vector<vector<Weather>>::iterator posw = world_weather_list.begin(); posw != world_weather_list.end(); ++posw) {
		
        vector<Weather>& weather_list = *posw;
        vector<VectorList<Tree>>::iterator world_positon_b = (world_tree_list.begin() + aktort);
        VectorList<Tree>& tree_list = *world_positon_b;
        vector<VectorList<Seed>>::iterator world_positon_s = (world_seed_list.begin() + aktort);
        VectorList<Seed>& seed_list = *world_positon_s;
		vector<vector<Pollengrid> >::iterator world_positon_p = (world_pollen_list.begin()+aktort);
		vector<Pollengrid>& pollen_list = *world_positon_p;
		vector<vector<Envirgrid>>::iterator world_positon_k = (world_plot_list.begin() + aktort);
        vector<Envirgrid>& plot_list = *world_positon_k;
        aktort++;

#pragma omp parallel default(shared)
			{
            RandomNumber<double> uniform(0, 1);
			RandomNumber<double> uniformrange(-2000, 2000);
			RandomNumber<double> uniformneutral(0, 999999);

            // mortality of seeds
#pragma omp for schedule(guided)
            for (unsigned int i = 0; i < seed_list.size(); ++i) {
                auto& seed = seed_list[i];
                if (!seed.dead) {
                    if (uniform.draw() < (seed.incone ? speciestrait[seed.species].seedconemort : speciestrait[seed.species].seedfloormort)) {
                        seed.dead = true;
                        seed_list.remove(i);
                    } else {
// if (!seed.incone)
	// cout << " seed survived on ground !!! " << endl;
					}
					// if(!seed.incone)
						// cout << "seed on ground!!! " << endl;
                }
            }
			//double end_time_seedsuviving=omp_get_wtime(); //marked as unused
			int aktortyworldcoo=(int) floor( (double) (aktort-1)/parameter[0].mapxlength );
			int aktortxworldcoo=(aktort-1) - (aktortyworldcoo * parameter[0].mapxlength);
			double timer_eachtree_advance_all=0;
			double timer_eachtree_vectini_all=0;
			double timer_eachtree_seedsurv_all=0;
			double timer_eachtree_seedadd_all=0;
			double timer_eachtree_total_all=0;
			double timer_tresedliv_all=0;
			double timer_createseeds_all=0;
//#pragma omp for schedule(guided)
			
			// 1 == use advance to iterate through lists, but this makes the computation really slow
			// 2 == split list to X lists of the same length
			// 3 == trees are ordered by age which is highly correlated with seedprodAKT so that only elements are considered untiil the last tree producing seeds
		
		
		
            // OMP==3
			omp_set_dynamic(0); //disable dynamic teams
			omp_set_num_threads(parameter[0].omp_num_threads); //set the number of helpers

			double direction=0.0;
			double velocity=0.0;
			unsigned int ripm=0,cntr=0;
			double p=0.0,kappa=pow(180/(parameter[0].pollendirectionvariance*M_PI),2),phi=0.0,dr=0.0,dx=0.0,dy=0.0;
			double I0kappa=0.0;
			double pe=0.01;
			double C=parameter[0].pollengregoryc;
			double m=parameter[0].pollengregorym;
			vector<int> Vname;
			vector<double> Vthdpth;
			vector<double> Vdrought;
			vector<double> Vnumber;
			vector<double> Vclone;
			vector<double> Vgrowth;
			vector<double> Vactive;
			vector<double> Vselve;
			vector<double> Vmatur;
			vector<double> Vwinwat;
			vector<double> Vnutri;
			vector<vector<vector<unsigned int>>> Vneutral;
			vector<int> pollname;
			vector<unsigned int> copyneutralmarkers(24, 0);
			vector<unsigned int> fathvector(24, 0);
			vector<vector<unsigned int>> storevector;
			// set end of the iterations split up to last tree with produced seeds
			vector<VectorList<Tree>>::iterator lasttreewithseeds_iter=world_tree_list.begin();
			int lasttreewithseeds_pos=0;
			int treeiter=0;
			for(int kartenpos=0;kartenpos<(parameter[0].pollengridxpoints*parameter[0].pollengridypoints);kartenpos++)
							{
							
									pollen_list[kartenpos].Treenames.clear();
									pollen_list[kartenpos].seedweight=0;
									pollen_list[kartenpos].droughtresist=0;
									pollen_list[kartenpos].seednumber=0;
									pollen_list[kartenpos].clonality=0;
									pollen_list[kartenpos].growthform=0;
									pollen_list[kartenpos].activedepth=0;
									pollen_list[kartenpos].selving=0;
									pollen_list[kartenpos].maturation=0;
									pollen_list[kartenpos].winterwater=0;
									pollen_list[kartenpos].nutrition=0;
									pollen_list[kartenpos].neutralmarkers.clear();
							}	
			for (unsigned int tree_i = 0; tree_i < tree_list.size(); ++tree_i) {
				auto& tree = tree_list[tree_i];
				
				
				treeiter=treeiter+1;
				
				if(tree.seednewly_produced>0)
				{
					lasttreewithseeds_pos=treeiter;
				}
				
///NOTE: POLLEN GRID ONLY WORKS IN mcorevariant==3!!		 
					if(tree.cone!=0){
						
						//...because the grid is updated here in every time step. This way dying trees are filtered.
							//double lent=sqrt(parameter[0].pollengridpoints); //marked as unused variable
							double lentx=(parameter[0].pollengridxpoints);
							double lenty=(parameter[0].pollengridypoints);
							for(int kartenpos=0;kartenpos<(parameter[0].pollengridxpoints*parameter[0].pollengridypoints);kartenpos++)
							{
							if(			 (pollen_list[kartenpos].xcoo + 0.5*treerows/lentx >= tree.xcoo)
								&&       (pollen_list[kartenpos].ycoo + 0.5*treecols/lenty	>= tree.ycoo)
								&&		 (pollen_list[kartenpos].ycoo - 0.5*treecols/lentx	<  tree.ycoo)	
								&&		 (pollen_list[kartenpos].xcoo - 0.5*treerows/lenty	<  tree.xcoo)
								)
								{
									pollen_list[kartenpos].Treenames.push_back(tree.name);
									//tree.subgridVECpos=//pollen_list2[kartenpos].Treenames.begin()+
									//(pollen_list[kartenpos].Treenames.size()-1);
									pollen_list[kartenpos].seedweight+=tree.seedweight;
									pollen_list[kartenpos].droughtresist+=tree.droughtresist;
									pollen_list[kartenpos].seednumber+=tree.seednumber;
									pollen_list[kartenpos].clonality+=tree.clonality;
									pollen_list[kartenpos].growthform+=tree.growthform;
									pollen_list[kartenpos].activedepth+=tree.activedepth;
									pollen_list[kartenpos].selving+=tree.selving;
									pollen_list[kartenpos].maturation+=tree.maturation;
									pollen_list[kartenpos].winterwater+=tree.winterwater;
									pollen_list[kartenpos].nutrition+=tree.nutrition;
									pollen_list[kartenpos].neutralmarkers.push_back(tree.neutralmarkers);
									//NEUE IDEE: RAND ZIEHEN. IF RAND()>xyz, return irgendwas. DAS BENÖTIGT KEINEN VEKTOR

									tree.subgridpos=kartenpos+1;
								}
							}	
								
								
				}
			}
			for(int kartenpos=0;kartenpos<(parameter[0].pollengridxpoints*parameter[0].pollengridypoints);kartenpos++)
			{
				if(pollen_list[kartenpos].Treenames.size()>1){
					pollen_list[kartenpos].seedweight/=pollen_list[kartenpos].Treenames.size();
					pollen_list[kartenpos].droughtresist/=pollen_list[kartenpos].Treenames.size();
					pollen_list[kartenpos].seednumber/=pollen_list[kartenpos].Treenames.size();
					pollen_list[kartenpos].clonality/=pollen_list[kartenpos].Treenames.size();
					pollen_list[kartenpos].growthform/=pollen_list[kartenpos].Treenames.size();
					pollen_list[kartenpos].activedepth/=pollen_list[kartenpos].Treenames.size();
					pollen_list[kartenpos].selving/=pollen_list[kartenpos].Treenames.size();
					pollen_list[kartenpos].maturation/=pollen_list[kartenpos].Treenames.size();
					pollen_list[kartenpos].winterwater/=pollen_list[kartenpos].Treenames.size();
					pollen_list[kartenpos].nutrition/=pollen_list[kartenpos].Treenames.size();
					// pollen_list[kartenpos].seedweightvar+=(tree.seedweight-pollen_list[kartenpos].seedweight)*(tree.seedweight-pollen_list[kartenpos].seedweight);
				}
			}
			advance(lasttreewithseeds_iter, lasttreewithseeds_pos);
					
			#pragma omp parallel default(shared) private(direction,velocity,ripm,cntr,p,kappa,phi,dr,dx,dy,I0kappa,pe,C,m       ,Vname,Vthdpth,Vdrought,Vnumber,Vclone,Vgrowth,Vactive,Vselve,Vmatur,Vwinwat,Vnutri,Vneutral,pollname,copyneutralmarkers,fathvector,storevector)
			{
				direction=0.0;velocity=0.0;ripm=0,cntr=0;p=0.0;kappa=pow(180/(parameter[0].pollendirectionvariance*M_PI),2);phi=0.0;dr=0.0;dx=0.0;dy=0.0;
				I0kappa=0.0;pe=0.01;C=parameter[0].pollengregoryc;m=parameter[0].pollengregorym;

				int thread_count=omp_get_num_threads();
				int thread_num=omp_get_thread_num();
				size_t chunk_size=lasttreewithseeds_pos/thread_count;
				auto begin=world_tree_list.begin();
				std::advance(begin, thread_num*chunk_size);
				auto end=begin;
				
				if(thread_num==(thread_count-1))
				{
					if((parameter[0].ivort==1))
					{
						cout << " -- OMP -- set current number of helpers to =" << parameter[0].omp_num_threads << " --> realized =" << omp_get_num_threads() << " of maximum N=" << omp_get_num_procs() << " on this machine" << endl << endl;
					}
				
					end=lasttreewithseeds_iter;
				} 
				else
				{
					std::advance(end,chunk_size);
				}
		
				// declare a local seed list to be filled by each thread
				list<Seed> newseed_list;
				
				int n_trees=0;
				double timer_eachtree_advance=0;
				double timer_eachtree_vectini=0;
				double timer_eachtree_seedsurv=0;
				double timer_eachtree_seedadd=0;
				double timer_eachtree_total=0;
				double timer_tresedliv=0;
				double timer_createseeds=0;

				// wait for all threads to initialize and then proceed
				#pragma omp barrier
				for (unsigned int it = 0; it < tree_list.size(); ++it) {
					double start_timer_eachtree=omp_get_wtime();
					++n_trees;//for calculating mean of computation times
				
					double end_timer_eachtree_advance=omp_get_wtime();
					timer_eachtree_advance+=end_timer_eachtree_advance-start_timer_eachtree;
					 
					auto& tree = tree_list[it];
							
					
					double end_timer_eachtree_vecini=omp_get_wtime();
					timer_eachtree_vectini+=end_timer_eachtree_vecini-end_timer_eachtree_advance;
					
					if(tree.seednewly_produced>0)
					{
						int seedlebend=0;
						for(int sna=0; sna < tree.seednewly_produced; sna++)
						{
							double zufallsz = 0.0 +( (double) 1.0*uniform.draw() );
							if(zufallsz>=parameter[0].seedconemort) 
							{
								++seedlebend;
							}
						}
						
						double end_timer_seedsurv_vecini=omp_get_wtime();
						timer_eachtree_seedsurv+=end_timer_seedsurv_vecini-end_timer_eachtree_vecini;
						
						if(seedlebend>0)
						{
							double start_timer_tresedliv=omp_get_wtime();	
							
							if( (parameter[0].pollination==1 && parameter[0].ivort>1045) || (parameter[0].pollination==9))
							{
								Pollinationprobability(tree.xcoo,tree.ycoo,&parameter[0],world_positon_p,direction,velocity,ripm,cntr,p,kappa,phi,dr,dx,dy,I0kappa,pe,C,m, Vname,Vthdpth,Vdrought,Vnumber,Vclone,Vgrowth,Vactive,Vselve,Vmatur,Vwinwat,Vnutri,Vneutral,pollname,n_trees);
							}
							
							double end_timer_tresedliv=omp_get_wtime();	
							timer_tresedliv+=end_timer_tresedliv-start_timer_tresedliv;

							for(int sl=0; sl<seedlebend; sl++)
							{
								auto& seed = seed_list[sl];
								seed.yworldcoo=aktortyworldcoo;
								seed.xworldcoo=aktortxworldcoo;
								seed.xcoo=tree.xcoo;
								seed.ycoo=tree.ycoo;
								seed.namem=tree.name;
								
								double rans = uniform.draw();
								if (((Vname.size()==0) && (parameter[0].pollination==1 || parameter[0].pollination==9)) || rans*100 < tree.selving)
								{
									//If no fathering pollen grid cell is found....
									seed.namep=0;
									//seed.seedweight=1;
									seed.seedweight=normrand(tree.seedweight,0.05,0.33,1.66);								
									seed.droughtresist=normrand(tree.droughtresist,8,0,100);
									seed.seednumber=normrand(tree.seednumber,0.05,0.33,1.66);
									//seed.clonality=normrand(tree.clonality,8,0,100);
									seed.clonality=0;
									seed.growthform=normrand(tree.growthform,8,0,100);
									seed.activedepth=normrand(tree.activedepth,8,0,100);
									seed.selving=0;
									//seed.selving=normrand(tree.selving,8,0,100);
									seed.maturation=normrand(tree.maturation,8,0,100);
									seed.winterwater=normrand(tree.winterwater,8,0,100);
									seed.nutrition=normrand(tree.nutrition,8,0,100);
									//seed.droughtresist=100;
									copyneutralmarkers=mixvector(tree.neutralmarkers,tree.neutralmarkers);
									seed.neutralmarkers=copyneutralmarkers;
									//seed.neutralmarkers=mixvector(tree.neutralmarkers,tree.neutralmarkers);
									//vector<unsigned int> copyneutralmarkers(16, 0);
									// generate(copyneutralmarkers.begin(),copyneutralmarkers.end(), uniformneutral);
									//seed.neutralmarkers=copyneutralmarkers;
									}
								// if chosen, determine the father by pollination out of available (matured) trees
								else if((Vname.size()>0) && (parameter[0].pollination==1 || parameter[0].pollination==9))
								{
									int iran=(int) uniform.draw() *Vname.size(); //at the end -1 was deleteed as it is suspected to be causing the bug
									//Vname.at(iran) is the chosen pollen grid cell number returned from the pollination function
									//Vthdpth.at(iran) is the chosen trait (seed weight) value returned from the pollination function
									seed.namep=Vname.at(iran); 
									seed.thawing_depthinfluence=100;
									//The standard deviations of the two gaussian peaks from which the new seed weight value
									//is drawn is here set to 0.5 (above:0.0) for this benchmarking version. 
									//This should be changed to a pollengrid size dependent law derived from genetic studies
									//(square root?... according to neutral theory) for std1 and a tree dependent std2.
									//
									//seed.seedweight=1;
									seed.seedweight=mixrand(Vthdpth.at(iran),0.05,tree.seedweight,0.05,0.33,1.66); // changed the std to be a lot smaller since they can be any value anway. realistic value to be determined
									//seed.seedweight=averand(Vthdpth.at(iran),tree.seedweight,0.5,0.05,0.33,1.66);	
									// seed.seedweight=Vthdpth.at(iran); //used for testing
									seed.droughtresist=mixrand(Vdrought.at(iran),8,tree.droughtresist,8,0,100);
									seed.seednumber=mixrand(Vnumber.at(iran),0.05,tree.seednumber,0.05,0.33,1.66);
									//seed.clonality=mixrand(Vdrought.at(iran),8,tree.clonality,8,0,100);
									seed.clonality=0;
									seed.growthform=mixrand(Vdrought.at(iran),8,tree.growthform,8,0,100);
									seed.activedepth=mixrand(Vdrought.at(iran),8,tree.activedepth,8,0,100);
									//seed.selving=0;
									seed.selving=mixrand(Vdrought.at(iran),8,tree.selving,8,0,100);
									seed.maturation=mixrand(Vdrought.at(iran),8,tree.maturation,8,0,100);
									seed.winterwater=mixrand(Vdrought.at(iran),8,tree.winterwater,8,0,100);
									seed.nutrition=mixrand(Vdrought.at(iran),8,tree.nutrition,8,0,100);
									//seed.droughtresist=100;
									// seed.droughtresist=Vdrought.at(iran); //used for testing
									
									double randecide=uniform.draw();
									storevector=Vneutral.at(iran);
									int decider = floor((storevector.size()* randecide )-0.000001); //reducing by small number to avoid florring to length as that position would be sigfault
									fathvector = storevector[decider];
									
									copyneutralmarkers=mixvector(tree.neutralmarkers,fathvector);
									seed.neutralmarkers=copyneutralmarkers;
									//seed.neutralmarkers=mixvector(tree.neutralmarkers,fathvector);
								} 
								else if (parameter[0].pollination ==0)   ///should be further upstream to avoid unnecessary computation. no need to keep pollenlist when there is no pollination 
								{
									if (parameter[0].variabletraits == 1)
									{
									seed.namep=0;
									//seed.seedweight=normrand(1,0.5,0.33,1.66);
									seed.seedweight=1;
									seed.droughtresist=100;
									//seed.droughtresist=normrand(28.4532485252458,20,0,100);
									seed.seednumber=normrand(1,0.5,0.33,1.66);
									//seed.clonality=normrand(50,20,0,100);
									seed.clonality=0;
									seed.growthform=normrand(50,20,0,100);
									seed.activedepth=normrand(50,20,0,100);
									seed.selving=0;
									//seed.selving=normrand(50,20,0,100);
									seed.maturation=normrand(50,20,0,100);
									seed.winterwater=normrand(50,20,0,100);
									seed.nutrition=normrand(50,20,0,100);
								
									 generate(copyneutralmarkers.begin(),copyneutralmarkers.end(), uniformneutral);
									seed.neutralmarkers=copyneutralmarkers;
									}
									else
									{
									seed.namep=0;
									seed.seedweight=1;
									seed.droughtresist=100;
									seed.seednumber=1;
									seed.clonality=0;
									seed.growthform=0;
									seed.activedepth=50;
									seed.selving=0;
									seed.maturation=50;
									seed.winterwater=50;
									seed.nutrition=50;
									}
								} 
								
								
								seed.currentweight=tree.seedweight;
								seed.line=tree.line;
								seed.generation=tree.generation+1;
								seed.incone=true;
								//seed.weight=1;
								seed.age=0;
								seed.species=tree.species;
								seed.releaseheight=tree.height;

								newseed_list.push_back(seed);
							}
							
							double end_timer_createseeds=omp_get_wtime();	
							timer_createseeds+=end_timer_createseeds-end_timer_tresedliv;
						}
						
						double end_timer_seedsurv_seedadd=omp_get_wtime();
						timer_eachtree_seedadd+=end_timer_seedsurv_seedadd-end_timer_seedsurv_vecini;
					}
					
					if (tree.cloningactive==true){
						
							
						if (tree.cloned!=true){
							//printf("Did this work?");  Its seems to be the creation of the new tree that is causing the issue
							tree.clonetimer=6;
							//printf("started cloning %d", tree.clonetimer);
							//printf("\n");
							tree.cloned=true;
							Tree treenew;
							               int i = tree.ycoo * parameter[0].sizemagnif / 1000;
							int j = tree.xcoo * parameter[0].sizemagnif / 1000;

							const auto curposi = static_cast<std::size_t>(i) * static_cast<std::size_t>(treecols) * static_cast<std::size_t>(parameter[0].sizemagnif) + static_cast<std::size_t>(j);

							double thawing_depthinfluence_help = 100;
							if ( (parameter[0].thawing_depth == true) && (plot_list[curposi].maxthawing_depth < (speciestrait[tree.species].minactivelayer*10)) ) {  // TODO: check calculation only during spinup
							thawing_depthinfluence_help = (unsigned short)((200.0 / (speciestrait[tree.species].minactivelayer*10)) * (double)plot_list[curposi].maxthawing_depth);
							}

                // ... and weather.
                // calculate the latest growth performance
                // TODO: merge with general growth functions
                double maxbw_help = 0;

                if (parameter[0].lineartransect == true) {
					maxbw_help = exp(speciestrait[tree.species].gdbasalconst + speciestrait[tree.species].gdbasalfac * 0 + speciestrait[tree.species].gdbasalfacq * 0 * 0)
								 * (weather_list[yearposition].weatherfactor[tree.species]
									+ ((weather_list[yearposition].weatherfactormin[tree.species] - weather_list[yearposition].weatherfactor[tree.species])
									   * ((double)tree.ycoo / 1000) / ((double)treerows)))
								 * (((double)thawing_depthinfluence_help) / 100);
                } else if (parameter[0].demlandscape) {  // copied from growth.cpp
                    if (parameter[0].thawing_depth) {
						maxbw_help = exp(speciestrait[tree.species].gdbasalconst + speciestrait[tree.species].gdbasalfac * 0 + speciestrait[tree.species].gdbasalfacq * 0 * 0)
									 * ((weather_list[yearposition].weatherfactor[tree.species]
										 * (((double)plot_list[curposi].elevation / 10) - (parameter[0].elevationoffset + 1000))
										 / (parameter[0].elevationoffset - (parameter[0].elevationoffset + 1000)))
										+ (weather_list[yearposition].weatherfactormin[tree.species]
										   * (1
											  - (((double)plot_list[curposi].elevation / 10) - (parameter[0].elevationoffset + 1000))
													/ (parameter[0].elevationoffset - (parameter[0].elevationoffset + 1000)))))
									 * (((double)tree.thawing_depthinfluence) / 100);
                    } else {
						maxbw_help = exp(speciestrait[tree.species].gdbasalconst + speciestrait[tree.species].gdbasalfac * 0 + speciestrait[tree.species].gdbasalfacq * 0 * 0)
									 * ((weather_list[yearposition].weatherfactor[tree.species]
										 * (((double)plot_list[curposi].elevation / 10) - (parameter[0].elevationoffset + 1000))
										 / (parameter[0].elevationoffset - (parameter[0].elevationoffset + 1000)))
										+ (weather_list[yearposition].weatherfactormin[tree.species]
										   * (1
											  - (((double)plot_list[curposi].elevation / 10) - (parameter[0].elevationoffset + 1000))
													/ (parameter[0].elevationoffset - (parameter[0].elevationoffset + 1000)))));
                    }
                } else {
                        maxbw_help = exp(speciestrait[tree.species].gdbasalconst + speciestrait[tree.species].gdbasalfac * 0 + speciestrait[tree.species].gdbasalfacq * 0 * 0)
                                     * weather_list[yearposition].weatherfactor[tree.species] * (((double)thawing_depthinfluence_help) / 100);
				}

                // individual seedling growth depends on density
                // define seedlings density value
                double flaechengroesze = 0.0;
                if (parameter[0].calcinfarea == 1)  // linearly increasing
                    flaechengroesze = maxbw_help * parameter[0].incfac / 100.0;
                else if (parameter[0].calcinfarea == 2)  // linearly increasing
                    flaechengroesze = maxbw_help * (2 / 3) * parameter[0].incfac / 100.0;
                else if (parameter[0].calcinfarea == 3)  // linearly increasing
                    flaechengroesze = maxbw_help * (4 / 3) * parameter[0].incfac / 100.0;
                else if (parameter[0].calcinfarea == 4)  // logistic growth function with maximum at 8 m
                    flaechengroesze = (9 / (1 + (((1 / 0.1) - 1) * exp(-0.1 * maxbw_help)))) - 1;
                else if (parameter[0].calcinfarea == 5)  // logistic growth function with maximum at 8 m
                    flaechengroesze = (9 / (1 + (((1 / 0.1) - 1) * exp(-0.2 * maxbw_help)))) - 1;
                else if (parameter[0].calcinfarea == 6)  // logistic growth function with maximum at 8 m
                    flaechengroesze = (9 / (1 + (((1 / 0.1) - 1) * exp(-0.5 * maxbw_help)))) - 1;

                double density_help = pow(
                    pow(maxbw_help, parameter[0].densitytreetile) * pow(flaechengroesze / (1.0 / parameter[0].sizemagnif), parameter[0].densitysmallweighing),
                    parameter[0].densityvaluemanipulatorexp);
                // get gridcell
                if (parameter[0].densitymode == 2) {
                    if (plot_list[curposi].Treedensityvalue > 0) {
                        if (parameter[0].densitytiletree == 1)  // sum of values
                        {
                            density_help = (1.0 - (density_help / ((double)plot_list[curposi].Treedensityvalue / 10000)));
                            //                           density_desired(at position) / density_currently(at position)
                        } else if (parameter[0].densitytiletree == 2)  // multiplication of values
                        {
                            density_help = (1.0 - (density_help / (((double)plot_list[curposi].Treedensityvalue / 10000) * density_help)));
                            //                           density_desired(at position) / density_currently(at position)
                        }
                    } else {
                        density_help = 0.0;  // no competition
                    }
                }
				if (density_help < 0.0)
					density_help = 0.0;
				if (density_help > 1.0)
					density_help = 1.0;
                // update growth
                double basalgrowth_help = maxbw_help * (1.0 - density_help);
							
							
						
						treenew.yworldcoo = tree.yworldcoo;
						treenew.xworldcoo = tree.xworldcoo;
					
						
						double moveranx=uniformrange.draw();
						double moverany=uniformrange.draw();
						
							//printf("creatinf tree at %4.4f",  tree.ycoo+moverany );
							//printf("and  %4.4f", tree.xcoo+moveranx);
						if (tree.ycoo+moveranx > 1000 * ((int)treerows - 1)) {
							treenew.ycoo = 1000 * ((int)treerows - 1);
							//printf("moved");
						}
						else if (tree.ycoo+moveranx <0) {
							treenew.ycoo =0.0;
							//printf("moved");
						}
						else{
							treenew.ycoo = tree.ycoo+moverany; 
						}
						
						if (tree.xcoo+moveranx > 1000 * ((int)treecols - 1)) {
							treenew.xcoo = 1000 * ((int)treecols - 1);
							//printf("moved");
						}
						else if (tree.xcoo+moveranx < 0) {
							treenew.xcoo = 0.0;
							//printf("moved");
						}
						else{
						treenew.xcoo = tree.xcoo+moveranx;
						}
						
						
						// treenew.name = ++parameter[0].nameakt;
						// treenew.namem = tree.namem;
						// treenew.namep = tree.namep;
						// treenew.yr_of_establishment = yearposition;
						// treenew.line = tree.line;
						// treenew.generation = tree.generation;
						treenew.dbasal = basalgrowth_help;
						treenew.dbasalmax = 1000 * maxbw_help;
						treenew.dbasalrel = 1000;
						treenew.dbreast = 0.0;
						treenew.dbreastrel = 1000;

						// tree height update
						if (parameter[0].allometryfunctiontype == 3) {// logistic growth
							treenew.height = 10 * exp(speciestrait[tree.species].heightloga/(1+exp((speciestrait[tree.species].heightlogb-log(treenew.dbasal*10))/speciestrait[tree.species].heightlogc)));
		// cout << "H = " << treenew.height << endl;
						} else if (parameter[0].allometryfunctiontype == 1) {
							treenew.height = 10 * speciestrait[tree.species].dbasalheightalloslope * pow(maxbw_help, speciestrait[tree.species].dbasalheightalloexp);
						} else {
							treenew.height = 10 * speciestrait[tree.species].dbasalheightslopenonlin * maxbw_help;
						}
						treenew.age = 0;
						treenew.cone = false;
						treenew.coneheight = 65535;
						treenew.seednewly_produced = 0;
						// treenew.seedproduced = 0;
						// treenew.buffer = 1;
						treenew.densitywert = density_help;
						// treenew.dispersaldistance = tree.dispersaldistance;
						treenew.growing = true;
						treenew.species = tree.species;
						treenew.thawing_depthinfluence = thawing_depthinfluence_help;
						treenew.seedweight = tree.seedweight;
						treenew.droughtresist = tree.droughtresist;
						treenew.seednumber = tree.seednumber;
						treenew.clonality = tree.clonality;
						//double ranc = uniform.draw();
						//if (ranc*100<=treenew.clonality && treenew.clonality!=0){
						//	treenew.cloning = true;
						//} else {
							treenew.cloning = false;
						//}
						treenew.cloningactive=true;
						treenew.clonetimer=10;
						//printf("new tree cloned %d", treenew.clonetimer);
						//printf("\n");
						treenew.cloned=true;
						treenew.growthform = tree.growthform;
						double rang = uniform.draw();
						if (rang*100<=treenew.growthform && treenew.growthform!=0){
							treenew.growthstunt= true;
						} else {
							treenew.growthstunt = false;
						}
						treenew.activedepth = tree.activedepth;
						treenew.selving = tree.selving;
						treenew.maturation = tree.maturation;
						treenew.winterwater = tree.winterwater;
						treenew.nutrition = tree.nutrition;
						treenew.neutralmarkers=tree.neutralmarkers;
						treenew.inbreedingdepression=tree.inbreedingdepression;
						treenew.envirimpact = 10000;
						treenew.twi = 6.25*100;
						treenew.soilhumidity = 1;
						treenew.crownstart = 0*10;
						treenew.relcrowndamage = 0*1000;
						//adding factor for growth here. should make this dependend on height maybe. although treeas are maturation height when the clone, so straight might be fine.
							treenew.cloneboost=2;
							tree.cloneboost=0.95;
						// deleting tree if outside of plot should be here ???	
						tree_list.add(std::move(treenew));
						
						}
						if (tree.clonetimer<=0){
							tree.cloned=false;
							tree.cloneboost=1;
							tree.cloningactive=false;
						}
					}
					
					timer_eachtree_total+=omp_get_wtime()-start_timer_eachtree;
				}// main tree loop on each core
				
				// append all newly created seed from each thread at once to the seed_list
				#pragma omp critical
				{
					//world_seed_list.splice(world_seed_list.end(), newseed_list);    //this one needs looking into
					
					timer_eachtree_advance_all+=timer_eachtree_advance/n_trees;
					timer_eachtree_vectini_all+=timer_eachtree_vectini/n_trees;
					timer_eachtree_seedsurv_all+=timer_eachtree_seedsurv/n_trees;
					timer_eachtree_seedadd_all+=timer_eachtree_seedadd/n_trees;
					timer_eachtree_total_all+=timer_eachtree_total/n_trees;
					
					timer_tresedliv_all+=timer_tresedliv/n_trees;
					timer_createseeds_all+=timer_createseeds/n_trees;
				}
			} // parallel region
			
			Vname.clear();Vname.shrink_to_fit();
			Vthdpth.clear();Vthdpth.shrink_to_fit();
		
				
			
        }      // parallel region
        seed_list.consolidate();
		tree_list.consolidate();
		//double end_time_poll=omp_get_wtime();
        TreeMort(yearposition, weather_list, tree_list);
		//double end_time_mortpoll=omp_get_wtime();
		}
    }


