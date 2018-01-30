using namespace std;




/****************************************************************************************//**
 * \brief calculate dispersal distance dependent on dispersal mode (0-4)
 *
 * 0=zufaellig, 1=exp, 2=fat tailed, 3=gaussian und 4=gaussian+fat tailed combined
 *
 *******************************************************************************************/


void Seedoutput(int aktort, double entfernung, float richtung, int neueweltcoo) 
{
	//StefanC: Ausgabe der LDSD-seed
	// Code-Bsp. hier http://www2.informatik.uni-halle.de/lehre/c/c_fopen.html
	FILE *dateizeiger;
	string dateiname;

		// assemble file names
		dateiname="output/dataseed_LDSD.csv";
 
		// Trying to open file for reading
		dateizeiger = fopen(dateiname.c_str(), "r+");
			// if fopen fails create new data file + header
			if (dateizeiger == NULL)
			{
			  dateizeiger = fopen(dateiname.c_str(), "w");
				fprintf(dateizeiger, "ivort;");
				fprintf(dateizeiger, "aktort;");
				fprintf(dateizeiger, "entfernung;");
				fprintf(dateizeiger, "richtung;");
				fprintf(dateizeiger, "neueweltcoo;");			
				fprintf(dateizeiger, "\n");

				if (dateizeiger == NULL)
				{
					fprintf(stderr, "Error: long distance seed dispersal file could not be opened!\n");
					exit(1);
				}
			}

			// else: append data to file
			fseek(dateizeiger,0,SEEK_END);

			// data evaluation and output
			fprintf(dateizeiger, "%d;", parameter[0].ivort);
			fprintf(dateizeiger, "%d;", aktort);
			fprintf(dateizeiger, "%4.5f;", entfernung);
			fprintf(dateizeiger, "%4.5f;", richtung);
			fprintf(dateizeiger, "%d;", neueweltcoo);		
			fprintf(dateizeiger, "\n");

		fclose(dateizeiger);
}



/****************************************************************************************//**
 * \brief calculate seed dispersal
 *
 * 
 *
 *******************************************************************************************/


 void seedausbreitung(int jahr, struct Parameter *parameter, vector<list<seed*> > &world_seed_list)
{
		
	int aktort=0;
			
	///Loop around all Seed Lists
	for(vector<list<seed*> >::iterator posw = world_seed_list.begin(); posw != world_seed_list.end(); ++posw)
	{ 
		list<seed*>& seed_list = *posw;
		
		// determine the current location, so that in long distance dispersal the target can be determined
		aktort++;

		// displaying seeds crossing the borders
		int rausgeflogenN=0, rausgeflogenO=0, rausgeflogenS=0, rausgeflogenW=0;

		if(parameter[0].omp_num_threads==0)
		{
			double cum_time_individual_seed=0;//timer for individual seed 
			double cum_time_seeddisp=0;
				
			///Loop around all Seeds
			for(list<seed*>::iterator pos = seed_list.begin(); pos != seed_list.end();)
			{
				double time_start_individual_seed=omp_get_wtime();
				
				pseed=(*pos);

				///If Seed is in a cone
				if (pseed->imcone==true)
				{
					double flug = 0.0 +( (double) 1.0*rand()/(RAND_MAX + 1.0));
									
					///and random number < rate of emerging seeds
					if(flug <= parameter[0].seedflugrate)
					{ // If the seed disperses a coordinate is calculated
						
						double ratiorn=0.0 +( (double) 1.0*rand()/(RAND_MAX + 1.0));
											
						if(ratiorn>0.0)
						{ // RN >0 BEGIN
							
							pseed->imcone=false;
							
							double entfernung = 0;
							double richtung=0.0;
							double geschwindigkeit=0;
							double wrichtung=0.0;
							
							double jquer=0;
							double iquer=0;

							double time_start_individual_seed_seeddisp=omp_get_wtime();
							seeddisp(ratiorn, jquer, iquer, geschwindigkeit, wrichtung, pseed->elternheight, pseed->species);
							cum_time_seeddisp+=omp_get_wtime()-time_start_individual_seed_seeddisp;

							
							
													
							// seed dispersal output:
								if(parameter[0].ivort>1045 && parameter[0].outputmode!=9)
								{							
										double seedeinschreibzufall=0.0 +( (double) 1.0*rand()/(RAND_MAX + 1.0));
									

										if(seedeinschreibzufall<0.01)
										{
											entfernung=sqrt(pow(iquer,2)+pow(jquer,2));
											richtung=atan2(iquer,jquer);
																	
											FILE *dateizeiger;
											string dateiname;

											// assembling file name
											char dateinamesuf[12];
											sprintf(dateinamesuf, "%.4d_REP%.3d", parameter[0].weatherchoice,parameter[0].repeati);
											dateiname="output/dataseed_distance" + string(dateinamesuf) + ".csv";
										
											// Trying to open file for reading
											dateizeiger = fopen (dateiname.c_str(), "r+");
											// If fopen fails, open a new file adding a header
											if (dateizeiger == NULL)
											{
											  dateizeiger = fopen (dateiname.c_str(), "w");
												fprintf(dateizeiger,"IVORT;");
												fprintf(dateizeiger, "name;");
												fprintf(dateizeiger, "year;" );
												fprintf(dateizeiger, "parentheight;");
												//fprintf(dateizeiger, "xentfernung;");
												//fprintf(dateizeiger, "yentfernung;");
												fprintf(dateizeiger, "distance;");
												fprintf(dateizeiger, "direction;");
												fprintf(dateizeiger, "xcoo;");
												fprintf(dateizeiger, "ycoo;");
												fprintf(dateizeiger, "species;" );
												fprintf(dateizeiger, "weatherchoice;");
												fprintf(dateizeiger, "thawing_depth;");	
												fprintf(dateizeiger, "windspd;");
												fprintf(dateizeiger, "winddir;");
												fprintf(dateizeiger, "\n");

												if (dateizeiger == NULL)
												{
													fprintf(stderr, "Error: Seed distance file could not be opened!\n");
													exit(1);
												}
											}

											fseek(dateizeiger,0,SEEK_END);

											fprintf(dateizeiger, "%d;",parameter[0].ivort);
											fprintf(dateizeiger, "%d;", pseed->namem);
											fprintf(dateizeiger, "%d;", jahr);
											fprintf(dateizeiger, "%4.3f;", pseed->elternheight);
											//fprintf(dateizeiger, "%f;", iquer);
											//fprintf(dateizeiger, "%f;", jquer);
											fprintf(dateizeiger, "%4.5f;",sqrt(iquer*iquer+jquer*jquer));
											fprintf(dateizeiger, "%4.5f;", richtung);
											fprintf(dateizeiger, "%4.5f;", pseed->xcoo);
											fprintf(dateizeiger, "%4.5f;", pseed->ycoo);	
											fprintf(dateizeiger, "%d;", pseed->species);
											fprintf(dateizeiger, "%d;", parameter[0].weatherchoice);
											fprintf(dateizeiger, "%d;", parameter[0].thawing_depth);
											fprintf(dateizeiger, "%lf;",geschwindigkeit);
											fprintf(dateizeiger, "%lf;", wrichtung);
											fprintf(dateizeiger, "\n");

											fclose(dateizeiger);
										}
								}//output end if
							
							
							pseed->xcoo=pseed->xcoo+jquer;
							pseed->ycoo=pseed->ycoo+iquer;
							pseed->entfernung=entfernung;	



							/****************************************************************************************//**
							* \brief calculate  Dispersal within a plot
							*
							*
							*******************************************************************************************/
							
								bool sameausserhalb=false;
								
								// Check if the seed is on the plot:
								if(pseed->ycoo > (double) (treerows-1)) 
								{
									if((parameter[0].periodRB==1))
									{
									   pseed->ycoo=fmod(pseed->ycoo,(double)(treerows-1));
									   pseed->namem=0;
									   pseed->namep=0;
									} 
									else 
									{
										sameausserhalb=true;
										rausgeflogenN++;
									}
								} 
								else if(pseed->ycoo<0.0)
								{
									if((parameter[0].periodRB==1))
									{
									   pseed->ycoo=(double)(treerows-1)+fmod(pseed->ycoo,(double)(treerows-1));
									   pseed->namem=0;
									   pseed->namep=0;
									} 
									else 
									{
										sameausserhalb=true;
										rausgeflogenS++;
									}
								} 
								 
								if(pseed->xcoo<0.0)
								{
									if((parameter[0].periodRB==1))
									{
									   pseed->xcoo = fmod(pseed->xcoo,(double)(treecols-1))+(double)(treecols-1);
									   pseed->namem=0;
									   pseed->namep=0;
									} 
									else
									{
										sameausserhalb=true;
										rausgeflogenW++;
									}
								} 
								else if(pseed->xcoo > (double) (treecols-1)) 
								{
									if(parameter[0].periodRB==1)
									{
										pseed->xcoo = fmod(pseed->xcoo,(double)(treecols-1));
										pseed->namem=0;
										pseed->namep=0;
																	} 
									else if((parameter[0].periodRB==2) && (rand()<0.5*RAND_MAX))
									{ //less reintroduction from the western border than from the eastern
										pseed->xcoo = fmod(pseed->xcoo,(double)(treecols-1));
									} 
									else
									{	
										sameausserhalb=true;
										rausgeflogenO++;
									}
								}
		
								if( (sameausserhalb==false) && ( (pseed->ycoo<0.0) | (pseed->ycoo> (double) (treerows-1)) | (pseed->xcoo<0.0) | (pseed->xcoo> (double) (treecols-1)) ) )
								{
									printf("\n\nLaVeSi was exited ");
									printf("in seedausbreitung.cpp\n");
									printf("... Reason: dispersed seed is, after deleting it, still part of the simulated plot (Pos(Y=%4.2f,X=%4.2f))\n", pseed->ycoo, pseed->xcoo);
									exit(1);
								}

								if(sameausserhalb==true)
								{ // deleting seed Begin
									delete pseed;
									pos=seed_list.erase(pos);
								} 
								else
								{
									++pos;
								}

						} // RN >0 END

					} 
					else  // If the seed is dispersed, a coordinate is determined END
					{
						++pos;
					}
				} 
				else //Imcone? END
				{
					++pos;
				}
				
				cum_time_individual_seed+=omp_get_wtime()-time_start_individual_seed;
				

			} // seed_list loop END
			
			cout << endl << "All seeds:" << cum_time_individual_seed << " with seeddisp-function:" << cum_time_seeddisp << endl;
				
		} 
		else
		{ // multi-core-processing
			// manually chose the implementation of multi-core-processing
			int mcorevariant=2;
				// 1 == only advance
				// ... ==> result: advance makes the computation really really slow!!
				// 2 == split list to X lists => had to outsource the delete precedure because otherwise it would lead to SEGFAULTs!
			if(mcorevariant==1)
			{// OMP==1
			// cout << endl << " OMP seed dispersal variant (1 was chosen)=" << mcorevariant << endl;
				///Loop around all Seeds
				for(unsigned int pari=0; pari<seed_list.size(); )
				{
					// double t0=omp_get_wtime();
					list<seed*>::iterator pos=seed_list.begin();
					advance(pos, pari);
					pseed=(*pos);

					
					///If Seed is in a cone
					if (pseed->imcone==true)
					{
						double flug = 0.0 +( (double) 1.0*rand()/(RAND_MAX + 1.0));
										
						///and random number < rate of emerging seeds
						if(flug <= parameter[0].seedflugrate)
						{ // If the seed disperses a coordinate is calculated
							
							double ratiorn=0.0 +( (double) 1.0*rand()/(RAND_MAX + 1.0));
												
							if(ratiorn>0.0)
							{ // RN > 0 BEGIN
								
								pseed->imcone=false;
								
								double entfernung = 0;
								double richtung=0.0;
								double geschwindigkeit=0;
								double wrichtung=0.0;
								double jquer=0;
								double iquer=0;

								seeddisp(ratiorn, jquer, iquer, geschwindigkeit, wrichtung, pseed->elternheight, pseed->species);
								
								
								// seed dispersal output:
								if(parameter[0].ivort>1045 && parameter[0].outputmode!=9 && parameter[0].omp_num_threads==1)
								{							
										double seedeinschreibzufall=0.0 +( (double) 1.0*rand()/(RAND_MAX + 1.0));
									

										if(seedeinschreibzufall<0.01)
										{
											entfernung=sqrt(pow(iquer,2)+pow(jquer,2));
											richtung=atan2(iquer,jquer);
																	
											FILE *dateizeiger;
											string dateiname;

											// assembling file name
											char dateinamesuf[12];
											sprintf(dateinamesuf, "%.4d_REP%.3d", parameter[0].weatherchoice,parameter[0].repeati);
											dateiname="output/dataseed_distance" + string(dateinamesuf) + ".csv";
										
											// Trying to open files for reading
											dateizeiger = fopen (dateiname.c_str(), "r+");
											// if fopen fails, create a new file and header
											if (dateizeiger == NULL)
											{
											  dateizeiger = fopen (dateiname.c_str(), "w");
												fprintf(dateizeiger,"IVORT;");
												fprintf(dateizeiger, "name;");
												fprintf(dateizeiger, "year;" );
												fprintf(dateizeiger, "parentheight;");
												//fprintf(dateizeiger, "xentfernung;");
												//fprintf(dateizeiger, "yentfernung;");
												fprintf(dateizeiger, "distance;");
												fprintf(dateizeiger, "direction;");
												fprintf(dateizeiger, "xcoo;");
												fprintf(dateizeiger, "ycoo;");
												fprintf(dateizeiger, "species;" );
												fprintf(dateizeiger, "weatherchoice;");
												fprintf(dateizeiger, "thawing_depth;");	
												fprintf(dateizeiger, "windspd;");
												fprintf(dateizeiger, "winddir;");
												fprintf(dateizeiger, "\n");

												if (dateizeiger == NULL)
												{
													fprintf(stderr, "Error: seed distance file could not be opened!\n");
													exit(1);
												}
											}

											fseek(dateizeiger,0,SEEK_END);

											fprintf(dateizeiger, "%d;",parameter[0].ivort);
											fprintf(dateizeiger, "%d;", pseed->namem);
											fprintf(dateizeiger, "%d;", jahr);
											fprintf(dateizeiger, "%4.3f;", pseed->elternheight);
											//fprintf(dateizeiger, "%f;", iquer);
											//fprintf(dateizeiger, "%f;", jquer);
											fprintf(dateizeiger, "%4.5f;",sqrt(iquer*iquer+jquer*jquer));
											fprintf(dateizeiger, "%4.5f;", richtung);
											fprintf(dateizeiger, "%4.5f;", pseed->xcoo);
											fprintf(dateizeiger, "%4.5f;", pseed->ycoo);	
											fprintf(dateizeiger, "%d;", pseed->species);
											fprintf(dateizeiger, "%d;", parameter[0].weatherchoice);
											fprintf(dateizeiger, "%d;", parameter[0].thawing_depth);
											fprintf(dateizeiger, "%lf;",geschwindigkeit);
											fprintf(dateizeiger, "%lf;", wrichtung);
											fprintf(dateizeiger, "\n");

											fclose(dateizeiger);
										}
								}//output end if
								
								
								
								pseed->xcoo=pseed->xcoo+jquer;
								pseed->ycoo=pseed->ycoo+iquer;
								pseed->entfernung=entfernung;	


							
								/****************************************************************************************//**
								* \brief calculate  Dispersal within a plot
								*
								*
								*******************************************************************************************/
									bool sameausserhalb=false;
									
									// Check if the seed is on the plot:
									if(pseed->ycoo > (double) (treerows-1)) 
									{
										if((parameter[0].periodRB==1))
										{
										   pseed->ycoo=fmod(pseed->ycoo,(double)(treerows-1));
										   pseed->namem=0;
										   pseed->namep=0;
										
										} 
										else 
										{
											sameausserhalb=true;
											rausgeflogenN++;
										}
									} 
									else if(pseed->ycoo<0.0)
									{
										if((parameter[0].periodRB==1))
										{
										   pseed->ycoo=(double)(treerows-1)+fmod(pseed->ycoo,(double)(treerows-1));
										   pseed->namem=0;
										   pseed->namep=0;
										
										} 
										else 
										{
											sameausserhalb=true;
											rausgeflogenS++;
										}
									}
									 
									if(pseed->xcoo<0.0)
									{
										if((parameter[0].periodRB==1))
										{
										   pseed->xcoo = fmod(pseed->xcoo,(double)(treecols-1))+(double)(treecols-1);
										   pseed->namem=0;
										   pseed->namep=0;
										} 
										else
										{
											sameausserhalb=true;
											rausgeflogenW++;
										}
									} 
									else if(pseed->xcoo > (double) (treecols-1)) 
									{
										if(parameter[0].periodRB==1)
										{
											pseed->xcoo = fmod(pseed->xcoo,(double)(treecols-1));
											pseed->namem=0;
											pseed->namep=0;
										   
										} 
										else if((parameter[0].periodRB==2) && (rand()<0.5*RAND_MAX))
										{ //less seed reintroduction on the western border
											pseed->xcoo = fmod(pseed->xcoo,(double)(treecols-1));
										} 
										else
										{	
											sameausserhalb=true;
											rausgeflogenO++;
										}
									} 
									
			
									if( (sameausserhalb==false) && ( (pseed->ycoo<0.0) | (pseed->ycoo> (double) (treerows-1)) | (pseed->xcoo<0.0) | (pseed->xcoo> (double) (treecols-1)) ) )
									{
										printf("\n\nLaVeSi was exited ");
										printf("in seedausbreitung.cpp\n");
										printf("... Reason: dispersed seed is, after deleting it, still part of the simulated plot (Pos(Y=%4.2f,X=%4.2f))\n", pseed->ycoo, pseed->xcoo);
										exit(1);
									}

									if(sameausserhalb==true)
									{ // Seed deletion BEGIN
										delete pseed;
										pos=seed_list.erase(pos);
										++pari;
									} 
									else
									{
										++pari;
									}

							} // RN > 0 END

						} 
						else  // Falls der seed ausfliegt, so wird eine Koordinate ermittelt Ende
						{
							++pari;
						}
					} 
					else //Imcone? Ende
					{
						++pari;
					}
				} // Ende seed_list ablaufen
			}// OMP==1
			if(mcorevariant==2)
			{// OMP==2
				omp_set_dynamic(0); //disable dynamic teams
				omp_set_num_threads(parameter[0].omp_num_threads); //set the number of helpers
					
				#pragma omp parallel private(pseed)
				{
					// initialize the info for each of the thread
					int thread_count = omp_get_num_threads();
					int thread_num   = omp_get_thread_num();
					size_t chunk_size= seed_list.size() / thread_count;
					auto begin = seed_list.begin();
					std::advance(begin, thread_num * chunk_size);
					auto end = begin;
					
					if(thread_num == (thread_count - 1)) // last thread iterates the remaining sequence
					{
						end = seed_list.end();
						// cout << thread_num << " -> thread_num == (thread_count - 1)" << endl;
					} 
					else
					{
						std::advance(end, chunk_size);
						// cout << thread_num << " -> thread_num != (thread_count - 1)" << endl;
					}
					
					
					// wait for all threads to initialize and then proceed
					#pragma omp barrier
					for(auto it = begin; it != end; ++it)
					{
						pseed=(*it);
					
					
						///Loop around all Seeds
						///If Seed is in a cone
						if (pseed->imcone==true)
						{
							double flug = 0.0 +( (double) 1.0*rand()/(RAND_MAX + 1.0));
											
							///and random number < rate of emerging seeds
							if(flug <= parameter[0].seedflugrate)
							{ // If the seed disperses a coordinate is calculated
								
								double ratiorn=0.0 +( (double) 1.0*rand()/(RAND_MAX + 1.0));
													
								if(ratiorn>0.0)
								{ // RN >0 BEGIN
									
									pseed->imcone=false;
									
									double entfernung=0.0;
									double richtung=0.0;
									double geschwindigkeit=0.0;
									double wrichtung=0.0;
									double jquer=0;
									double iquer=0;

									seeddisp(ratiorn, jquer, iquer, geschwindigkeit, wrichtung, pseed->elternheight, pseed->species);
									
									// seed dispersal output:
									if(parameter[0].ivort>1045 && parameter[0].outputmode!=9 && parameter[0].omp_num_threads==1)
									{
											double seedeinschreibzufall=0.0 +( (double) 1.0*rand()/(RAND_MAX + 1.0));
										


											if(seedeinschreibzufall<0.01)
											{
												entfernung=sqrt(pow(iquer,2)+pow(jquer,2));
												richtung=atan2(iquer,jquer);
																		
												//distance output begin
												FILE *dateizeiger;
												string dateiname;

												// assemble file name
												char dateinamesuf[12];
												sprintf(dateinamesuf, "%.4d_REP%.3d", parameter[0].weatherchoice,parameter[0].repeati);
												dateiname="output/dataseed_distance" + string(dateinamesuf) + ".csv";
											
												// trying to open file for reading
												dateizeiger = fopen (dateiname.c_str(), "r+");
												// if fopen fails create a new file and header
												if (dateizeiger == NULL)
												{
												  dateizeiger = fopen (dateiname.c_str(), "w");
													fprintf(dateizeiger,"IVORT;");
													fprintf(dateizeiger, "name;");
													fprintf(dateizeiger, "year;" );
													fprintf(dateizeiger, "parentheight;");
													fprintf(dateizeiger, "distance;");
													fprintf(dateizeiger, "direction;");
													fprintf(dateizeiger, "xcoo;");
													fprintf(dateizeiger, "ycoo;");
													fprintf(dateizeiger, "species;" );
													fprintf(dateizeiger, "weatherchoice;");
													fprintf(dateizeiger, "thawing_depth;");	
													fprintf(dateizeiger, "windspd;");
													fprintf(dateizeiger, "winddir;");
													fprintf(dateizeiger, "\n");

													if (dateizeiger == NULL)
													{
														fprintf(stderr, "Fehler: seed distance file could not be opened!\n");
														exit(1);
													}
												}

												fseek(dateizeiger,0,SEEK_END);

												fprintf(dateizeiger, "%d;",parameter[0].ivort);
												fprintf(dateizeiger, "%d;", pseed->namem);
												fprintf(dateizeiger, "%d;", jahr);
												fprintf(dateizeiger, "%4.3f;", pseed->elternheight);
												fprintf(dateizeiger, "%4.5f;",entfernung);
												fprintf(dateizeiger, "%4.5f;", richtung);
												fprintf(dateizeiger, "%4.5f;", pseed->xcoo);
												fprintf(dateizeiger, "%4.5f;", pseed->ycoo);	
												fprintf(dateizeiger, "%d;", pseed->species);
												fprintf(dateizeiger, "%d;", parameter[0].weatherchoice);
												fprintf(dateizeiger, "%d;", parameter[0].thawing_depth);
												fprintf(dateizeiger, "%lf;",geschwindigkeit);
												fprintf(dateizeiger, "%lf;", wrichtung);
												fprintf(dateizeiger, "\n");

												fclose(dateizeiger);
											}
									}//output end if
									
									pseed->xcoo=pseed->xcoo+jquer;
									pseed->ycoo=pseed->ycoo+iquer;
									pseed->entfernung=entfernung;	

								
									/****************************************************************************************//**
									* \brief calculate  Dispersal within a plot
									*
									*
									*******************************************************************************************/
										bool sameausserhalb=false;
										
										// Check if the seed is on the plot:
										if(pseed->ycoo > (double) (treerows-1)) 
										{
											if((parameter[0].periodRB==1))
											{
											   pseed->ycoo=fmod(pseed->ycoo,(double)(treerows-1));
											   pseed->namem=0;
											   pseed->namep=0;
											} 
											else if((parameter[0].periodRB==3))
											{
												sameausserhalb=true;
												rausgeflogenN++;
											} 
											else 
											{
												sameausserhalb=true;
												rausgeflogenN++;
											}
										} 
										else if(pseed->ycoo<0.0)
										{
											if((parameter[0].periodRB==1))
											{
											   pseed->ycoo=(double)(treerows-1)+fmod(pseed->ycoo,(double)(treerows-1));
											   pseed->namem=0;
											   pseed->namep=0;
											} 
											else if((parameter[0].periodRB==3))
											{
												sameausserhalb=true;
												rausgeflogenS++;
											} 
											else 
											{
												sameausserhalb=true;
												rausgeflogenS++;
											}
										}
										 
										if(pseed->xcoo<0.0)
										{
											if((parameter[0].periodRB==1 || parameter[0].periodRB==3))
											{
											   pseed->xcoo = fmod(pseed->xcoo,(double)(treecols-1))+(double)(treecols-1);
											   pseed->namem=0;
											   pseed->namep=0;
											} 
											else
											{
												sameausserhalb=true;
												rausgeflogenW++;
											}
										} 
										else if(pseed->xcoo > (double) (treecols-1))
										{
											if(parameter[0].periodRB==1 || parameter[0].periodRB==3)
											{
												pseed->xcoo = fmod(pseed->xcoo,(double)(treecols-1));
												pseed->namem=0;
												pseed->namep=0;
				   
											} 
											else if((parameter[0].periodRB==2) && (rand()<0.5*RAND_MAX))
											{ // Reducing seed introduction on the western border:
												pseed->xcoo = fmod(pseed->xcoo,(double)(treecols-1));
											} 
											else
											{	
												sameausserhalb=true;
												rausgeflogenO++;
											}
										} 
				
										if( (sameausserhalb==false) && ( (pseed->ycoo<0.0) | (pseed->ycoo> (double) (treerows-1)) | (pseed->xcoo<0.0) | (pseed->xcoo> (double) (treecols-1)) ) )
										{
											printf("\n\nLaVeSi was exited ");
											printf("in seedausbreitung.cpp\n");
											printf("... Reason: dispersed seed is, after deleting it, still part of the simulated plot (Pos(Y=%4.2f,X=%4.2f))\n", pseed->ycoo, pseed->xcoo);
											exit(1);
										}

										if(sameausserhalb==true)
										{
											pseed->ycoo=-99999.9;
											// pseed->xcoo=-99999.9;
										}
								} // RN > 0 END
							}   // If the seed disperses a coordinate is calculated END
						} //Imcone? END
					} // seed_list loop END
				}//END: pragma for
				
				for(list<seed*>::iterator pos = seed_list.begin(); pos != seed_list.end();)
				{// START: delete seeds with sameausserhalb==true
					pseed=(*pos);

					if(pseed->ycoo==-99999.9)
					{ //Seed deletion BEGIN
						delete pseed;
						pos=seed_list.erase(pos);
					} 
					else
					{
						++pos;
					}
				}// END: delete seeds
			}// OMP==2

		}


		// Display seeds leaving the plot:
		if (parameter[0].seeddispvis==true) 
			printf("\n   Leaving seeds (N/O/S/W)=(%d/%d/%d/%d) ", rausgeflogenN, rausgeflogenO, rausgeflogenS, rausgeflogenW);
	} // World list END
}































