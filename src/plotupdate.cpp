using namespace std;

/****************************************************************************************//**
 * \brief 
 *
 * calculate simple density value for each tree
 *
 *
 *
 *******************************************************************************************/
void AddTreeDensity(list<Tree*> &tree_list, vector<Envirgrid*> &plot_list)
{
	for (list<Tree*>::iterator pos = tree_list.begin(); pos != tree_list.end(); )
	{
		pTree=(*pos);
		
		int i=(int) floor(pTree->ycoo*parameter[0].sizemagnif);
		int j=(int) floor(pTree->xcoo*parameter[0].sizemagnif);

		double flaechengroesze=0.0;
		if (parameter[0].calcinfarea == 1) //linearly increasing
			flaechengroesze = pTree->dbasal * parameter[0].incfac/100.0; 
		else if (parameter[0].calcinfarea == 2) //linearly increasing
			flaechengroesze = pTree->dbasal * (2/3) * parameter[0].incfac/100.0; 
		else if (parameter[0].calcinfarea == 3) //linearly increasing
			flaechengroesze = pTree->dbasal * (4/3) * parameter[0].incfac/100.0; 
		else if (parameter[0].calcinfarea == 4) //logistic growth function with maximum at 8 m
			flaechengroesze= ( 9/( 1+( ( (1/0.1)-1 )*exp(-0.1*pTree->dbasal) ) ) )-1;
		else if (parameter[0].calcinfarea == 5) //logistic growth function with maximum at 8 m
			flaechengroesze= ( 9/( 1+( ( (1/0.1)-1 )*exp(-0.2*pTree->dbasal) ) ) )-1;
		else if (parameter[0].calcinfarea == 6) //logistic growth function with maximum at 8 m
			flaechengroesze= ( 9/( 1+( ( (1/0.1)-1 )*exp(-0.5*pTree->dbasal) ) ) )-1;
		
		// if the trees influences only one density grid cell
		if ( flaechengroesze<(1.0/parameter[0].sizemagnif) )
		{
			plot_list[i*treecols*parameter[0].sizemagnif+j]->Treedensityvalue += 
									pow(
												pow(flaechengroesze/(1.0/parameter[0].sizemagnif),parameter[0].densitysmallweighing)
												//weighing with diameter
												*
												pow(pTree->dbasal, parameter[0].densitytreetile),
										parameter[0].densityvaluemanipulatorexp);
			plot_list[i*treecols*parameter[0].sizemagnif+j]->Treenumber++;
			// pTree->densitywert=pow(pTree->dbasal, parameter[0].densitytreetile);
			pTree->densitywert=pow(
							pow(pTree->dbasal, parameter[0].densitytreetile)*pow(flaechengroesze/(1.0/parameter[0].sizemagnif),parameter[0].densitysmallweighing),
							parameter[0].densityvaluemanipulatorexp);
		}
		// if the trees influences more than one density grid cell
		else
		{
			// determine dimensions of the grid around the tree
			int xyquerrastpos= (int) floor( flaechengroesze*parameter[0].sizemagnif );
			// determine shifted coordinates and adding up the density value
			double sumdensitywert=0;
			
			for (int rastposi=(i+xyquerrastpos); rastposi>(i-(xyquerrastpos+1)); rastposi--)
			{
				for (int rastposj=(j-xyquerrastpos); rastposj<(j+xyquerrastpos+1); rastposj++)
				{
					if ( ( rastposi<=( (treerows-1)*parameter[0].sizemagnif) && rastposi>=0 ) && ( rastposj<=( (treecols-1)*parameter[0].sizemagnif) && rastposj>=0 ) )
					{
						
						// Distance calculation to determine the influence of the density value in spatial units ...
						// ... and inserting the value at every position
						double entfrastpos=sqrt(pow(double(i-rastposi),2)+pow(double(j-rastposj),2));
						// only if the current grid cell is part of the influence area, a value is assigned
						if (entfrastpos<= (double) xyquerrastpos)
						{
							plot_list[rastposi*treecols*parameter[0].sizemagnif+rastposj]->Treedensityvalue 
								+= 
								pow(
									pow(pTree->dbasal, parameter[0].densitytreetile)/(entfrastpos+1.0),
									parameter[0].densityvaluemanipulatorexp);
								
							plot_list[rastposi*treecols*parameter[0].sizemagnif+rastposj]->Treenumber++;

							sumdensitywert+=pow(
												pow(pTree->dbasal, parameter[0].densitytreetile)/(entfrastpos+1.0),
												parameter[0].densityvaluemanipulatorexp);
						}
					}
				}
			}
							
			pTree->densitywert=sumdensitywert;
		}

		++pos;
	}
}





/****************************************************************************************//**
 * \brief 
 *
 * loop around every tree to determine it's individual density value \n
 * that's the value with which it influences other trees
 *
 * 3 kinds of density calculation are possible \n
 * 1: no density calculated and densitywert=0.0 \n
 *
 * 2: ZOI-kind of type and pTree->densitywert=1.0-(density_help/plot_list[i*treecols*parameter[0].sizemagnif+j]->Treedensityvalue) or \n
 *							sumdensitywert+=plot_list[rastposi*treecols*parameter[0].sizemagnif+rastposj]->Treedensityvalue; \n
 *							pTree->densitywert=1.0-(density_help/sumdensitywert);	
 *							pTree->densitywert= pTree->densitywert *pow((1.0-(0.01/pTree->dbasal)),parameter[0].densityvaluedbasalinfluence);	
 *
 * 3: random field of ZOI-Type and pTree->densitywert=1.0-(density_help/plot_list[izuf*treecols*parameter[0].sizemagnif+jzuf]->Treedensityvalue) or \n
 *								   sumdensitywert+=plot_list[izuf*treecols*parameter[0].sizemagnif+jzuf]->Treedensityvalue \n
 *								   pTree->densitywert=1.0-(density_help/sumdensitywert);
 *								   pTree->densitywert= pTree->densitywert *pow((1.0-(0.01/pTree->dbasal)),parameter[0].densityvaluedbasalinfluence);
 *******************************************************************************************/

 void IndividualTreeDensity(list<Tree*> &tree_list, vector<Envirgrid*> &plot_list)
{
	if(parameter[0].omp_num_threads==1)
	{// only one core
		for (list<Tree*>::iterator pos = tree_list.begin(); pos != tree_list.end(); ++pos)
		{
			pTree=(*pos);

			int i = (int) floor(pTree->ycoo * parameter[0].sizemagnif);
			int j = (int) floor(pTree->xcoo * parameter[0].sizemagnif);

			//DENSITY 1
			if (parameter[0].densitymode == 1)
			{
				pTree->densitywert = 0.0;
			}
			else 
			{	
				double flaechengroesze=0.0;
				if (parameter[0].calcinfarea == 1) //linearly increasing
					flaechengroesze = pTree->dbasal * parameter[0].incfac/100.0; 
				else if (parameter[0].calcinfarea == 2) //linearly increasing
					flaechengroesze = pTree->dbasal * (2/3) * parameter[0].incfac/100.0; 
				else if (parameter[0].calcinfarea == 3) //linearly increasing
					flaechengroesze = pTree->dbasal * (4/3) * parameter[0].incfac/100.0; 
				else if (parameter[0].calcinfarea == 4) //logistic growth function with maximum at 8 m
					flaechengroesze= ( 9/( 1+( ( (1/0.1)-1 )*exp(-0.1*pTree->dbasal) ) ) )-1;
				else if (parameter[0].calcinfarea == 5) //logistic growth function with maximum at 8 m
					flaechengroesze= ( 9/( 1+( ( (1/0.1)-1 )*exp(-0.2*pTree->dbasal) ) ) )-1;
				else if (parameter[0].calcinfarea == 6) //logistic growth function with maximum at 8 m
					flaechengroesze= ( 9/( 1+( ( (1/0.1)-1 )*exp(-0.5*pTree->dbasal) ) ) )-1;

				// if the tree only influences one grid cell
				if ( flaechengroesze < (1.0/parameter[0].sizemagnif) )
				{
					// DENSITY 2
					if (parameter[0].densitymode == 2)
					{
						if (plot_list[i*treecols*parameter[0].sizemagnif+j]->Treedensityvalue > 0.0) 
						{
							if (parameter[0].densitytiletree==1)	// sum of values
							{
								pTree->densitywert =	(1.0 - (pTree->densitywert / plot_list[i*treecols*parameter[0].sizemagnif+j]->Treedensityvalue));
									//                           density_desired(at position) / density_currently(at position)
							}
							else if (parameter[0].densitytiletree==2)	// multiplication of values
							{
								pTree->densitywert =	(1.0 - (pTree->densitywert
															/ 
															(plot_list[i*treecols*parameter[0].sizemagnif+j]->Treedensityvalue*pTree->densitywert) 
															)
													);
									//                           density_desired(at position) / density_currently(at position)
							}
						}
						else
						{
							pTree->densitywert=0.0; // no competition
						}
					}
					// DENSITY 3
					else if (parameter[0].densitymode == 3)
					{
						// determine random grid cell position and assign a value
						int izuf= (int) floor( 0.0 + ( (double)  ( ((double) (treerows-1)) * parameter[0].sizemagnif * rand() / (RAND_MAX + 1.0))) );
						int jzuf= (int) floor( 0.0 + ( (double)  ( ((double) (treecols-1)) * parameter[0].sizemagnif * rand() / (RAND_MAX + 1.0))) );
						
						if (plot_list[izuf*treecols*parameter[0].sizemagnif+jzuf]->Treedensityvalue > 0.0) 
						{
							if (parameter[0].densitytiletree==1)	// sum of values
							{
								pTree->densitywert = (1.0 - (pTree->densitywert/ plot_list[izuf*treecols*parameter[0].sizemagnif+jzuf]->Treedensityvalue));
									//                           density_desired(at position) / density_currently(at position)
							}
							else if (parameter[0].densitytiletree==2)	// multiplication of values
							{
								pTree->densitywert =	(1.0 - (pTree->densitywert
															/ 
															(plot_list[i*treecols*parameter[0].sizemagnif+j]->Treedensityvalue*pTree->densitywert) 
															)
													);
							}
						}
						else
						{
							pTree->densitywert = 0.0;
						}
					}
					
					// calculate the thawing depth influence on the tree growth
					if ((plot_list[i*treecols*parameter[0].sizemagnif+j]->maxthawing_depth<2000) && (parameter[0].thawing_depth==true && parameter[0].spinupphase==false)) 
					{
						pTree->thawing_depthinfluence= (unsigned short) ((200.0/2000.0)* (double) plot_list[i*treecols*parameter[0].sizemagnif+j]->maxthawing_depth);
					}
					else
					{
						pTree->thawing_depthinfluence=100;
					}
				}
				else
				{
					// determine dimensions of the grid around the tree
					int xyquerrastpos= (int) floor( flaechengroesze*parameter[0].sizemagnif );
					double maxdist= sqrt(pow(double(xyquerrastpos),2)+pow(double(xyquerrastpos),2));

					// determine shifted coordinates and adding up the density value
					double sumdensitywert=0;
					double sumthawing_depth=0;
					
					unsigned int anzahlflaechen=0;
					
					for (int rastposi=(i+xyquerrastpos); rastposi>(i-(xyquerrastpos+1)); rastposi--)
					{
						for (int rastposj=(j-xyquerrastpos); rastposj<(j+xyquerrastpos+1); rastposj++)
						{
							if ((rastposi<=( (treerows-1)*parameter[0].sizemagnif) && rastposi>=0) && ( rastposj<=( (treecols-1)*parameter[0].sizemagnif) && rastposj>=0))
							{
								// distance calculation to determine the influence of the density value in spatial unit
								// ... similar to value insertion above
								double entfrastpos=sqrt(pow(double(i-rastposi),2)+pow(double(j-rastposj),2));
								
								// only if the current grid cell is a part of the influenced area, a value is assigned
								if (entfrastpos<= (double) xyquerrastpos)
								{
									int icurr=rastposi;
									int jcurr=rastposj;

									// change of i/j vals only if DENSITY 3
									if (parameter[0].densitymode==3)
									{
										// calculate the position from where the density values are taken randomly
										icurr= (int) floor( 0.0 + ( (double)  ( ((double) (treerows-1)) *parameter[0].sizemagnif*rand()/(RAND_MAX + 1.0))) );
										jcurr= (int) floor( 0.0 + ( (double)  ( ((double) (treecols-1)) *parameter[0].sizemagnif*rand()/(RAND_MAX + 1.0))) );
									}
									
									if (parameter[0].densitytiletree==1)	// sum of values
									{
										sumdensitywert+=plot_list[icurr*treecols*parameter[0].sizemagnif+jcurr]->Treedensityvalue
														* (1-entfrastpos/maxdist);
									}
									else if (parameter[0].densitytiletree==2)	// multiplication of values
									{
										// after weighting the additional values by the individual influence values the offset is added
										sumdensitywert+=(plot_list[icurr*treecols*parameter[0].sizemagnif+jcurr]->Treedensityvalue
														-pow(pTree->dbasal, parameter[0].densitytreetile)/(entfrastpos+1.0))
														* pow(pTree->dbasal, parameter[0].densitytreetile)/(entfrastpos+1.0)
														+ pow(pTree->dbasal, parameter[0].densitytreetile)/(entfrastpos+1.0);
									}

									sumthawing_depth+=(double) plot_list[rastposi*treecols*parameter[0].sizemagnif+rastposj]->maxthawing_depth;
									anzahlflaechen++;
								}
							}
						}
					}

					if (sumdensitywert > 0.0) 
						pTree->densitywert = 1.0 - (pTree->densitywert/sumdensitywert);
					//                           density_desired(at position) / density_currently(at position)
					else 
						pTree->densitywert=0.0;
						
					sumthawing_depth=( sumthawing_depth / (double) anzahlflaechen );
					
					if (sumthawing_depth  <2000)
						pTree->thawing_depthinfluence = (unsigned short) ((200.0/2000.0) * sumthawing_depth);
					else
						pTree->thawing_depthinfluence = 100;
				}

				pTree->densitywert= pTree->densitywert*pow((1.0-(0.01/pTree->dbasal)),parameter[0].densityvaluedbasalinfluence);
				
				if (parameter[0].dichtheightrel==1) 
				{
					pTree->densitywert= pTree->densitywert*( -1.0/130.0*pTree->height + 1.0 );
				} 
				else if (parameter[0].dichtheightrel==2) 
				{
					pTree->densitywert= pTree->densitywert*( -1.0/200.0*pTree->height + 1.0 );
				} 
				else if (parameter[0].dichtheightrel==3) 
				{
					double hrelbuf=( -1.0/200.0*pTree->height + 1.0 );
					if(hrelbuf<0.1) 
						hrelbuf=0.1;
					pTree->densitywert= pTree->densitywert*hrelbuf;
				} 
				else if (parameter[0].dichtheightrel==4)
				{
					if (pTree->height<40)
						pTree->densitywert=1.0;
					else if ((pTree->height>=40) & (pTree->height<200))
						pTree->densitywert=0.5;
					else if (pTree->height>=200)
						pTree->densitywert=0.0;
				} 
				else if (parameter[0].dichtheightrel==5) 
				{
					if (pTree->height<40)
						pTree->densitywert=1.0;
					else if ((pTree->height>=40) & (pTree->height<200))
						pTree->densitywert=0.55;
					else if (pTree->height>=200)
						pTree->densitywert=0.1;
				} 
				else if (parameter[0].dichtheightrel==6) 
				{
					if (pTree->height<40)
						pTree->densitywert=1.0;
					else if ((pTree->height>=40) & (pTree->height<200))
						pTree->densitywert=0.9;
					else if (pTree->height>=200)
						pTree->densitywert=0.8;
				}
				else if (parameter[0].dichtheightrel==10) // added to fit the height classes distribution properly
				{
					// density value decreases by increasing tree height
					// ... linearly
					double densitywertminimum=0.1;
					double ageseinfluss=( -1*(1-densitywertminimum)/100.0* (double) pTree->age + 1.0 );
					if(ageseinfluss<densitywertminimum)
					{
						ageseinfluss=densitywertminimum;
					}
					pTree->densitywert= pTree->densitywert*ageseinfluss;
				}
				else if (parameter[0].dichtheightrel==11) // added to fit the height classes distribution properly
				{
					// density value decreases by increasing tree height
					// ... linearly
					double densitywertminimum=0.5;
					double ageseinfluss=( -1*(1-densitywertminimum)/100.0* (double) pTree->age + 1.0 );
					if(ageseinfluss<densitywertminimum)
					{
						ageseinfluss=densitywertminimum;
					}
					pTree->densitywert= pTree->densitywert*ageseinfluss;
				}
				else if (parameter[0].dichtheightrel==12) // added to fit the height classes distribution properly
				{
					// density value decreases by increasing tree height
					// ... linearly
					double densitywertminimum=0.1;
					double ageseinfluss=( -1*(1-densitywertminimum)/50.0* (double) pTree->age + 1.0 );
					if(ageseinfluss<densitywertminimum)
					{
						ageseinfluss=densitywertminimum;
					}
					pTree->densitywert= pTree->densitywert*ageseinfluss;
				}
				else if (parameter[0].dichtheightrel==13) // added to fit the height classes distribution properly
				{
					// density value decreases by increasing tree height
					// ... linearly
					double densitywertminimum=0.5;
					double ageseinfluss=( -1*(1-densitywertminimum)/50.0* (double) pTree->age + 1.0 );
					if(ageseinfluss<densitywertminimum)
					{
						ageseinfluss=densitywertminimum;
					}
					pTree->densitywert= pTree->densitywert*ageseinfluss;
				}
				else if (parameter[0].dichtheightrel==20) // added to fit the height classes distribution properly
				{
					// density value decreases by increasing tree height
					double densitywertminimum=0.1;
					double ageseinfluss=(1.0/pow(( (double) pTree->age +1.0), 0.5));
					if(ageseinfluss<densitywertminimum)
					{
						ageseinfluss=densitywertminimum;
					}
					pTree->densitywert= pTree->densitywert*ageseinfluss;

				}
				else if (parameter[0].dichtheightrel==21) // added to fit the height classes distribution properly
				{
					// density value decreases by increasing tree height
					double densitywertminimum=0.5;
					double ageseinfluss=(1.0/pow(( (double) pTree->age +1.0), 0.15));
					if(ageseinfluss<densitywertminimum)
					{
						ageseinfluss=densitywertminimum;
					}
					pTree->densitywert= pTree->densitywert*ageseinfluss;

				}
				else if (parameter[0].dichtheightrel==22) // added to fit the height classes distribution properly
				{
					// density value decreases by increasing tree height
					double densitywertminimum=0.1;
					double ageseinfluss=(1.0/pow(( (double) pTree->age +1.0), 0.6));
					if(ageseinfluss<densitywertminimum)
					{
						ageseinfluss=densitywertminimum;
					}
					pTree->densitywert= pTree->densitywert*ageseinfluss;

				}
				else if (parameter[0].dichtheightrel==23) // added to fit the height classes distribution properly
				{
					// density value decreases by increasing tree height
					double densitywertminimum=0.5;
					double ageseinfluss=(1.0/pow(( (double) pTree->age +1.0), 0.175));
					if(ageseinfluss<densitywertminimum)
					{
						ageseinfluss=densitywertminimum;
					}
					pTree->densitywert= pTree->densitywert*ageseinfluss;

				}				
				
				if (pTree->densitywert<0) 
					pTree->densitywert=0.0;

				if (pTree->densitywert>parameter[0].desitymaxreduction) 
					pTree->densitywert=parameter[0].desitymaxreduction;
			} 
		}
	}// only one core
	
	if(parameter[0].omp_num_threads>1)
	{// more than one core
		omp_set_dynamic(0); //disable dynamic teams
		omp_set_num_threads(parameter[0].omp_num_threads); //set the number of helpers
			
		#pragma omp parallel private(pTree)
		{
			int thread_count=omp_get_num_threads();
			int thread_num=omp_get_thread_num();
			size_t chunk_size=tree_list.size()/thread_count;
			auto begin=tree_list.begin();
			std::advance(begin,thread_num*chunk_size);
			auto end=begin;
			
			if(thread_num==(thread_count-1)) // last thread takes the remaining elements
			{
				end = tree_list.end();
			} 
			else
			{
				std::advance(end, chunk_size);
			}
			
			// wait for all threads to initialize and then proceed
			#pragma omp barrier
			
			for(auto it = begin; it != end; ++it)
			{
				pTree=(*it);

				int i = (int) floor(pTree->ycoo * parameter[0].sizemagnif);
				int j = (int) floor(pTree->xcoo * parameter[0].sizemagnif);

				//DENSITY 1
				if (parameter[0].densitymode==1)
				{
					pTree->densitywert = 0.0;
				}
				else 
				{
					double flaechengroesze=0.0;
					if (parameter[0].calcinfarea == 1) //linearly increasing
						flaechengroesze = pTree->dbasal * parameter[0].incfac/100.0; 
					else if (parameter[0].calcinfarea == 2) //linearly increasing
						flaechengroesze = pTree->dbasal * (2/3) * parameter[0].incfac/100.0; 
					else if (parameter[0].calcinfarea == 3) //linearly increasing
						flaechengroesze = pTree->dbasal * (4/3) * parameter[0].incfac/100.0; 
					else if (parameter[0].calcinfarea == 4) //logistic growth function with maximum at 8 m
						flaechengroesze= ( 9/( 1+( ( (1/0.1)-1 )*exp(-0.1*pTree->dbasal) ) ) )-1;
					else if (parameter[0].calcinfarea == 5) //logistic growth function with maximum at 8 m
						flaechengroesze= ( 9/( 1+( ( (1/0.1)-1 )*exp(-0.2*pTree->dbasal) ) ) )-1;
					else if (parameter[0].calcinfarea == 6) //logistic growth function with maximum at 8 m
						flaechengroesze= ( 9/( 1+( ( (1/0.1)-1 )*exp(-0.5*pTree->dbasal) ) ) )-1;

					// if the tree only influences one grid cell
					if ( flaechengroesze < (1.0/parameter[0].sizemagnif) )
					{
						// DENSITY 2
						if (parameter[0].densitymode == 2)
						{
							if (plot_list[i*treecols*parameter[0].sizemagnif+j]->Treedensityvalue > 0.0) 
							{
								if (parameter[0].densitytiletree==1)	// sum of values
								{
									pTree->densitywert =	(1.0 - (pTree->densitywert / plot_list[i*treecols*parameter[0].sizemagnif+j]->Treedensityvalue));
									//                           density_desired(at position) / density_currently(at position)
								}
								else if (parameter[0].densitytiletree==2)	// multiplication of values
								{
									pTree->densitywert =	(1.0 - (pTree->densitywert
																/ 
																(plot_list[i*treecols*parameter[0].sizemagnif+j]->Treedensityvalue*pTree->densitywert) 
																)
														);
								//                         density_desired(at position) / density_currently(at position)
								}
							}
							else
							{
								pTree->densitywert=0.0; // no competition effect
							}
						}
						// DENSITY 3
						else if (parameter[0].densitymode == 3)
						{
							// determine the grid position randomly
							int izuf= (int) floor( 0.0 + ( (double)  ( ((double) (treerows-1)) * parameter[0].sizemagnif * rand() / (RAND_MAX + 1.0))) );
							int jzuf= (int) floor( 0.0 + ( (double)  ( ((double) (treecols-1)) * parameter[0].sizemagnif * rand() / (RAND_MAX + 1.0))) );
							
							if (plot_list[izuf*treecols*parameter[0].sizemagnif+jzuf]->Treedensityvalue > 0.0) 
							{
								if (parameter[0].densitytiletree==1)	// sum of values
								{
									pTree->densitywert = (1.0 - (pTree->densitywert/ plot_list[izuf*treecols*parameter[0].sizemagnif+jzuf]->Treedensityvalue));
									//                           density_desired(at position) / density_currently(at position)
								}
								else if (parameter[0].densitytiletree==2)	// multiplication of values
								{
									pTree->densitywert =	(1.0 - (pTree->densitywert
																/ 
																(plot_list[i*treecols*parameter[0].sizemagnif+j]->Treedensityvalue*pTree->densitywert) 
																)
														);
								}
							}
							else 
							{
								pTree->densitywert = 0.0;
							}
						}
						
						// calculate the influence of the thawing depth on the tree growth
						if ((plot_list[i*treecols*parameter[0].sizemagnif+j]->maxthawing_depth<2000) && (parameter[0].thawing_depth==true && parameter[0].spinupphase==false))
						{
							pTree->thawing_depthinfluence= (unsigned short) ((200.0/2000.0)* (double) plot_list[i*treecols*parameter[0].sizemagnif+j]->maxthawing_depth);
						}
						else
						{
							pTree->thawing_depthinfluence=100;
						}
					}
					// ... if the tree influences more than one section
					else
					{
						// determine dimensions of the considered grid around a tree
						int xyquerrastpos= (int) floor( flaechengroesze*parameter[0].sizemagnif );
						double maxdist= sqrt(pow(double(xyquerrastpos),2)+pow(double(xyquerrastpos),2));

						// determine rescaled coordinates and summation of the density value
						double sumdensitywert=0;
						double sumthawing_depth=0;
						
						unsigned int anzahlflaechen=0;
						
						for (int rastposi=(i+xyquerrastpos); rastposi>(i-(xyquerrastpos+1)); rastposi--)
						{
							for (int rastposj=(j-xyquerrastpos); rastposj<(j+xyquerrastpos+1); rastposj++)
							{
								if ((rastposi<=( (treerows-1)*parameter[0].sizemagnif) && rastposi>=0) && ( rastposj<=( (treecols-1)*parameter[0].sizemagnif) && rastposj>=0))
								{
									double entfrastpos=sqrt(pow(double(i-rastposi),2)+pow(double(j-rastposj),2));
									
									if (entfrastpos<= (double) xyquerrastpos)
									{
										int icurr=rastposi;
										int jcurr=rastposj;

										// change of i/j vals only if DENSITY 3
										if (parameter[0].densitymode==3)
										{
											// determine the position of density values randomly
											icurr= (int) floor( 0.0 + ( (double)  ( ((double) (treerows-1)) *parameter[0].sizemagnif*rand()/(RAND_MAX + 1.0))) );
											jcurr= (int) floor( 0.0 + ( (double)  ( ((double) (treecols-1)) *parameter[0].sizemagnif*rand()/(RAND_MAX + 1.0))) );
										}
										
										if (parameter[0].densitytiletree==1)
										{
											sumdensitywert+=plot_list[icurr*treecols*parameter[0].sizemagnif+jcurr]->Treedensityvalue
															* (1-entfrastpos/maxdist);	
															// added the values influence as becoming weaker in the periphery, otherwise the density value influence would be overestimated
										}
										else if (parameter[0].densitytiletree==2)
										{
											// after weighting the additional values by the individual influence values the offset is added
											sumdensitywert+=(plot_list[icurr*treecols*parameter[0].sizemagnif+jcurr]->Treedensityvalue
															-pow(pTree->dbasal, parameter[0].densitytreetile)/(entfrastpos+1.0) )
															* pow(pTree->dbasal, parameter[0].densitytreetile)/(entfrastpos+1.0)															
															+ pow(pTree->dbasal, parameter[0].densitytreetile)/(entfrastpos+1.0);
										}

										sumthawing_depth+=(double) plot_list[rastposi*treecols*parameter[0].sizemagnif+rastposj]->maxthawing_depth;
										anzahlflaechen++;										
									}
								}
							}
						}

						if (sumdensitywert > 0.0) 
							pTree->densitywert = 1.0 - (pTree->densitywert/sumdensitywert);
						else 
							pTree->densitywert=0.0;

						sumthawing_depth=( sumthawing_depth / (double) anzahlflaechen );
						
						if (sumthawing_depth  <2000)
							pTree->thawing_depthinfluence = (unsigned short) ((200.0/2000.0) * sumthawing_depth);
						else
							pTree->thawing_depthinfluence = 100;
					}

					pTree->densitywert= pTree->densitywert
					*pow((1.0-(0.01/pTree->dbasal)),parameter[0].densityvaluedbasalinfluence);	// Optional: increasing influence by increasing tree height
					
					if (parameter[0].dichtheightrel==1) 
					{
						pTree->densitywert= pTree->densitywert*( -1.0/130.0*pTree->height + 1.0 );
					} 
					else if (parameter[0].dichtheightrel==2) 
					{
						pTree->densitywert= pTree->densitywert*( -1.0/200.0*pTree->height + 1.0 );
					} 
					else if (parameter[0].dichtheightrel==3) 
					{
						double hrelbuf=( -1.0/200.0*pTree->height + 1.0 );
						if(hrelbuf<0.1) 
							hrelbuf=0.1;
						pTree->densitywert= pTree->densitywert*hrelbuf;
					} 
					else if (parameter[0].dichtheightrel==4)
					{
						if (pTree->height<40)
							pTree->densitywert=1.0;
						else if ((pTree->height>=40) & (pTree->height<200))
							pTree->densitywert=0.5;
						else if (pTree->height>=200)
							pTree->densitywert=0.0;
					} 
					else if (parameter[0].dichtheightrel==5) 
					{
						if (pTree->height<40)
							pTree->densitywert=1.0;
						else if ((pTree->height>=40) & (pTree->height<200))
							pTree->densitywert=0.55;
						else if (pTree->height>=200)
							pTree->densitywert=0.1;
					} 
					else if (parameter[0].dichtheightrel==6) 
					{
						if (pTree->height<40)
							pTree->densitywert=1.0;
						else if ((pTree->height>=40) & (pTree->height<200))
							pTree->densitywert=0.9;
						else if (pTree->height>=200)
							pTree->densitywert=0.8;
					}
					else if (parameter[0].dichtheightrel==10) // added to fit the height classes distribution properly
					{
						// density value decreases by increasing tree height
						// ... linearly
						double densitywertminimum=0.1;
						double ageseinfluss=( -1*(1-densitywertminimum)/100.0* (double) pTree->age + 1.0 );
						if(ageseinfluss<densitywertminimum)
						{
							ageseinfluss=densitywertminimum;
						}
						pTree->densitywert= pTree->densitywert*ageseinfluss;
					}
					else if (parameter[0].dichtheightrel==11) // added to fit the height classes distribution properly
					{
						// density value decreases by increasing tree height
						// ... linearly
						double densitywertminimum=0.5;
						double ageseinfluss=( -1*(1-densitywertminimum)/100.0* (double) pTree->age + 1.0 );
						if(ageseinfluss<densitywertminimum)
						{
							ageseinfluss=densitywertminimum;
						}
						pTree->densitywert= pTree->densitywert*ageseinfluss;
					}
					else if (parameter[0].dichtheightrel==12) // added to fit the height classes distribution properly
					{
						// density value decreases by increasing tree height
						// ... linearly
						double densitywertminimum=0.1;
						double ageseinfluss=( -1*(1-densitywertminimum)/50.0* (double) pTree->age + 1.0 );
						if(ageseinfluss<densitywertminimum)
						{
							ageseinfluss=densitywertminimum;
						}
						pTree->densitywert= pTree->densitywert*ageseinfluss;
					}
					else if (parameter[0].dichtheightrel==13) // added to fit the height classes distribution properly
					{
						// density value decreases by increasing tree height
						// ... linearly
						double densitywertminimum=0.5;
						double ageseinfluss=( -1*(1-densitywertminimum)/50.0* (double) pTree->age + 1.0 );
						if(ageseinfluss<densitywertminimum)
						{
							ageseinfluss=densitywertminimum;
						}
						pTree->densitywert= pTree->densitywert*ageseinfluss;
					}
					else if (parameter[0].dichtheightrel==20) // added to fit the height classes distribution properly
					{
						// density value decreases by increasing tree height
						double densitywertminimum=0.1;
						double ageseinfluss=(1.0/pow(( (double) pTree->age +1.0), 0.5));
						if(ageseinfluss<densitywertminimum)
						{
							ageseinfluss=densitywertminimum;
						}
						pTree->densitywert= pTree->densitywert*ageseinfluss;

					}
					else if (parameter[0].dichtheightrel==21) // added to fit the height classes distribution properly
					{
						// density value decreases by increasing tree height
						double densitywertminimum=0.5;
						double ageseinfluss=(1.0/pow(( (double) pTree->age +1.0), 0.15));
						if(ageseinfluss<densitywertminimum)
						{
							ageseinfluss=densitywertminimum;
						}
						pTree->densitywert= pTree->densitywert*ageseinfluss;

					}
					else if (parameter[0].dichtheightrel==22) // added to fit the height classes distribution properly
					{
						// density value decreases by increasing tree height
						double densitywertminimum=0.1;
						double ageseinfluss=(1.0/pow(( (double) pTree->age +1.0), 0.6));
						if(ageseinfluss<densitywertminimum)
						{
							ageseinfluss=densitywertminimum;
						}
						pTree->densitywert= pTree->densitywert*ageseinfluss;

					}
					else if (parameter[0].dichtheightrel==23) 
						// added to fit the height classes distribution properly
					{
						// density value decreases by increasing tree height
						double densitywertminimum=0.5;
						double ageseinfluss=(1.0/pow(( (double) pTree->age +1.0), 0.175));
						if(ageseinfluss<densitywertminimum)
						{
							ageseinfluss=densitywertminimum;
						}
						pTree->densitywert= pTree->densitywert*ageseinfluss;

					}
					
					if (pTree->densitywert<0) 
						pTree->densitywert=0.0;

					// set to maximal value if density value is greater than it (rescaling)
					if (pTree->densitywert>parameter[0].desitymaxreduction) 
						pTree->densitywert=parameter[0].desitymaxreduction;
				}
			}
		}// parallel region
	}// more than one core
}




/****************************************************************************************//**
 * \brief reset Maps "Envirgrid"
 *
 * reset Treedensity und Treenumber \n
 * calculate vegetation density (auflagenstärke) if vegetation==true \n
 * calculate max active layer depth
 *
 *
 *******************************************************************************************/

void ResetMaps(int yearposition, vector<Envirgrid*> &plot_list, vector<Weather*> &weather_list)
{
	if(parameter[0].omp_num_threads==1)
	{// only one core
		for (int kartenpos=0; kartenpos< (treerows*parameter[0].sizemagnif*treecols*parameter[0].sizemagnif); kartenpos++)
		{
			pEnvirgrid=plot_list[kartenpos];
			pEnvirgrid->Treedensityvalue=0;
			pEnvirgrid->Treenumber=0;
			
			if (parameter[0].vegetation==true && parameter[0].spinupphase==false)
			{
				double auflagenwachstumsrate =0.05
											 +( 1.0/( ((1.0/0.01)-(1.0/0.95))
													  *exp(-(1.0/200.0)*(double) pEnvirgrid->maxthawing_depth) 
													  +(1/0.95)) ); 
				
				pEnvirgrid->litterheight+= (unsigned short) (auflagenwachstumsrate*60.0);

				pEnvirgrid->litterheightmean = (unsigned short) ( (double) 
												 (pEnvirgrid->litterheight8
												 +pEnvirgrid->litterheight7
												 +pEnvirgrid->litterheight6
												 +pEnvirgrid->litterheight5
												 +pEnvirgrid->litterheight4
												 +pEnvirgrid->litterheight3
												 +pEnvirgrid->litterheight2
												 +pEnvirgrid->litterheight1
												 +pEnvirgrid->litterheight0
												 +pEnvirgrid->litterheight)
												 /10.0);

				pEnvirgrid->litterheight8 = pEnvirgrid->litterheight7;
				pEnvirgrid->litterheight7 = pEnvirgrid->litterheight6;
				pEnvirgrid->litterheight6 = pEnvirgrid->litterheight5;
				pEnvirgrid->litterheight5 = pEnvirgrid->litterheight4;
				pEnvirgrid->litterheight4 = pEnvirgrid->litterheight3;
				pEnvirgrid->litterheight3 = pEnvirgrid->litterheight2;
				pEnvirgrid->litterheight2 = pEnvirgrid->litterheight1;
				pEnvirgrid->litterheight1 = pEnvirgrid->litterheight0;
				pEnvirgrid->litterheight0 = pEnvirgrid->litterheight;
			}
			
			if (parameter[0].thawing_depth==true && parameter[0].spinupphase==false)
			{
				// calculation of an insulation by organic material (damping reduces thawing_depth, formula taken from literature)
				double daempfung = (1.0/4000.0) * (double) pEnvirgrid->litterheightmean; // 1/4000 =slope to reach the maximum value at appr. 4000
				
				if (daempfung>=0.9) 
					daempfung=0.9;

				pEnvirgrid->maxthawing_depth= (unsigned short) ( 1000.0*(1.0-daempfung)*0.050*sqrt(weather_list[yearposition]->degreday));	// 1000 (scaling from m to mm)*edaphicfactor=0.050 (SD=0.019)
			}
		}
	}// only one core

	if(parameter[0].omp_num_threads>1)
	{// more than one core
		omp_set_dynamic(1);
		omp_set_num_threads(parameter[0].omp_num_threads); //set the number of helpers
		
		#pragma omp parallel for private(pEnvirgrid)
		for(int kartenpos=0; kartenpos< (treerows*parameter[0].sizemagnif*treecols*parameter[0].sizemagnif); kartenpos++)
		{
			pEnvirgrid=plot_list[kartenpos];
			pEnvirgrid->Treedensityvalue=0;
			pEnvirgrid->Treenumber=0;
			
			if (parameter[0].vegetation==true && parameter[0].spinupphase==false)
			{
				double auflagenwachstumsrate =0.05
											 +( 1.0/( ((1.0/0.01)-(1.0/0.95))
													  *exp(-(1.0/200.0)*(double) pEnvirgrid->maxthawing_depth) 
													  +(1/0.95)) ); 
													  // logistic growth: capacity=0.95; N0=0.01; r=1/200; offset= 0.05
				
				pEnvirgrid->litterheight+= (unsigned short) (auflagenwachstumsrate*60.0);	
				// in 0.1 mm steps; 6mm growth annualy from 30 cm growth in 50 years (literature value)

				pEnvirgrid->litterheightmean = (unsigned short) ( (double) 
												 (pEnvirgrid->litterheight8
												 +pEnvirgrid->litterheight7
												 +pEnvirgrid->litterheight6
												 +pEnvirgrid->litterheight5
												 +pEnvirgrid->litterheight4
												 +pEnvirgrid->litterheight3
												 +pEnvirgrid->litterheight2
												 +pEnvirgrid->litterheight1
												 +pEnvirgrid->litterheight0
												 +pEnvirgrid->litterheight)
												 /10.0); 

				pEnvirgrid->litterheight8 = pEnvirgrid->litterheight7;
				pEnvirgrid->litterheight7 = pEnvirgrid->litterheight6;
				pEnvirgrid->litterheight6 = pEnvirgrid->litterheight5;
				pEnvirgrid->litterheight5 = pEnvirgrid->litterheight4;
				pEnvirgrid->litterheight4 = pEnvirgrid->litterheight3;
				pEnvirgrid->litterheight3 = pEnvirgrid->litterheight2;
				pEnvirgrid->litterheight2 = pEnvirgrid->litterheight1;
				pEnvirgrid->litterheight1 = pEnvirgrid->litterheight0;
				pEnvirgrid->litterheight0 = pEnvirgrid->litterheight;
			}			

			if (parameter[0].thawing_depth==true && parameter[0].spinupphase==false)
			{
				double daempfung = (1.0/4000.0) * (double) pEnvirgrid->litterheightmean; // 1/4000 =slope to reach the maximum value at appr. 4000
				
				if (daempfung>=0.9) 
					daempfung=0.9;
				
				pEnvirgrid->maxthawing_depth= (unsigned short) ( 1000.0*(1.0-daempfung)*0.050*sqrt(weather_list[yearposition]->degreday));// 1000 (scaling from m to mm)*edaphicfactor=0.050 (SD=0.019)
			}
		}
	}// more than one core
}






/****************************************************************************************//**
 * \brief update density maps and active layer depth
 *
 * 
 *
 *
 *******************************************************************************************/
 
void Environmentupdate(struct Parameter *parameter, int yearposition, vector<vector<Envirgrid*> > &world_plot_list, vector<list<Tree*> > &world_tree_list, vector<vector<Weather*> > &world_weather_list)
{
	int aktort=0;
	
	for (vector<vector<Envirgrid*> >::iterator posw = world_plot_list.begin(); posw != world_plot_list.end(); ++posw)
	{
		vector<Envirgrid*>& plot_list = *posw;

		vector<list<Tree*> >::iterator world_positon_b = (world_tree_list.begin()+aktort);
		list<Tree*>& tree_list = *world_positon_b;

		vector<vector<Weather*> >::iterator posiwelt = (world_weather_list.begin()+aktort);
		vector<Weather*>& weather_list = *posiwelt;

		aktort++;

		double time_start_0=omp_get_wtime();
		ResetMaps(yearposition, plot_list, weather_list);
		double time_ResetMaps=omp_get_wtime();
		
		AddTreeDensity(tree_list, plot_list);
		double time_AddTreeDensity=omp_get_wtime();

		IndividualTreeDensity(tree_list, plot_list);
		
		if(parameter[0].computationtimevis==1)
		{
			openupdate:
			FILE *fp5;
			fp5=fopen("t_N_plotupdate.txt","a+");
				if(fp5==0)
				{
				goto openupdate;
				}
				fprintf(fp5,"%d;%lu;%10.20f;%10.20f;%10.20f\n",
					parameter[0].ivort, 
					tree_list.size(),
					time_ResetMaps-time_start_0,
					time_AddTreeDensity-time_ResetMaps, 
					omp_get_wtime()-time_AddTreeDensity
				);
				
			fclose(fp5);
		}
	}
}

