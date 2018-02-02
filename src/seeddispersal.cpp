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
	FILE *filepointer;
	string dateiname;

		// assemble file names
		dateiname="output/dataseed_LDSD.csv";
 
		// Trying to open file for reading
		filepointer = fopen(dateiname.c_str(), "r+");
			// if fopen fails create new data file + header
			if (filepointer == NULL)
			{
			  filepointer = fopen(dateiname.c_str(), "w");
				fprintf(filepointer, "ivort;");
				fprintf(filepointer, "aktort;");
				fprintf(filepointer, "entfernung;");
				fprintf(filepointer, "richtung;");
				fprintf(filepointer, "neueweltcoo;");			
				fprintf(filepointer, "\n");

				if (filepointer == NULL)
				{
					fprintf(stderr, "Error: long distance seed dispersal file could not be opened!\n");
					exit(1);
				}
			}

			// else: append data to file
			fseek(filepointer,0,SEEK_END);

			// data evaluation and output
			fprintf(filepointer, "%d;", parameter[0].ivort);
			fprintf(filepointer, "%d;", aktort);
			fprintf(filepointer, "%4.5f;", entfernung);
			fprintf(filepointer, "%4.5f;", richtung);
			fprintf(filepointer, "%d;", neueweltcoo);		
			fprintf(filepointer, "\n");

		fclose(filepointer);
}



/****************************************************************************************//**
 * \brief calculate seed dispersal
 *
 * 
 *
 *******************************************************************************************/


 void Seeddispersal(int jahr, struct Parameter *parameter, vector<list<Seed*> > &world_seed_list)
{
		
	int aktort=0;
			
	///Loop around all Seed Lists
	for(vector<list<Seed*> >::iterator posw = world_seed_list.begin(); posw != world_seed_list.end(); ++posw)
	{ 
		list<Seed*>& seed_list = *posw;
		
		// determine the current location, so that in long distance dispersal the target can be determined
		aktort++;

		// displaying seeds crossing the borders
		int rausgeflogenN=0, rausgeflogenO=0, rausgeflogenS=0, rausgeflogenW=0;

		if(parameter[0].omp_num_threads==0)
		{
			double cum_time_individual_seed=0;//timer for individual seed 
			double cum_time_seeddisp=0;
				
			///Loop around all Seeds
			for(list<Seed*>::iterator pos = seed_list.begin(); pos != seed_list.end();)
			{
				double time_start_individual_seed=omp_get_wtime();
				
				pSeed=(*pos);

				///If Seed is in a cone
				if (pSeed->imcone==true)
				{
					double flug = 0.0 +( (double) 1.0*rand()/(RAND_MAX + 1.0));
									
					///and random number < rate of emerging seeds
					if(flug <= parameter[0].seedflightrate)
					{ // If the seed disperses a coordinate is calculated
						
						double ratiorn=0.0 +( (double) 1.0*rand()/(RAND_MAX + 1.0));
											
						if(ratiorn>0.0)
						{ // RN >0 BEGIN
							
							pSeed->imcone=false;
							
							double entfernung = 0;
							double richtung=0.0;
							double geschwindigkeit=0;
							double wrichtung=0.0;
							
							double jquer=0;
							double iquer=0;

							double time_start_individual_seed_seeddisp=omp_get_wtime();
							seeddisp(ratiorn, jquer, iquer, geschwindigkeit, wrichtung, pSeed->elternheight, pSeed->species);
							cum_time_seeddisp+=omp_get_wtime()-time_start_individual_seed_seeddisp;

							
							
													
							// seed dispersal output:
								if(parameter[0].ivort>1045 && parameter[0].outputmode!=9)
								{							
										double seedeinschreibzufall=0.0 +( (double) 1.0*rand()/(RAND_MAX + 1.0));
									

										if(seedeinschreibzufall<0.01)
										{
											entfernung=sqrt(pow(iquer,2)+pow(jquer,2));
											richtung=atan2(iquer,jquer);
																	
											FILE *filepointer;
											string dateiname;

											// assembling file name
											char dateinamesuf[12];
											sprintf(dateinamesuf, "%.4d_REP%.3d", parameter[0].weatherchoice,parameter[0].repeati);
											dateiname="output/dataseed_distance" + string(dateinamesuf) + ".csv";
										
											// Trying to open file for reading
											filepointer = fopen (dateiname.c_str(), "r+");
											// If fopen fails, open a new file adding a header
											if (filepointer == NULL)
											{
											  filepointer = fopen (dateiname.c_str(), "w");
												fprintf(filepointer,"IVORT;");
												fprintf(filepointer, "name;");
												fprintf(filepointer, "year;" );
												fprintf(filepointer, "parentheight;");
												//fprintf(filepointer, "xentfernung;");
												//fprintf(filepointer, "yentfernung;");
												fprintf(filepointer, "distance;");
												fprintf(filepointer, "direction;");
												fprintf(filepointer, "xcoo;");
												fprintf(filepointer, "ycoo;");
												fprintf(filepointer, "species;" );
												fprintf(filepointer, "weatherchoice;");
												fprintf(filepointer, "thawing_depth;");	
												fprintf(filepointer, "windspd;");
												fprintf(filepointer, "winddir;");
												fprintf(filepointer, "\n");

												if (filepointer == NULL)
												{
													fprintf(stderr, "Error: Seed distance file could not be opened!\n");
													exit(1);
												}
											}

											fseek(filepointer,0,SEEK_END);

											fprintf(filepointer, "%d;",parameter[0].ivort);
											fprintf(filepointer, "%d;", pSeed->namem);
											fprintf(filepointer, "%d;", jahr);
											fprintf(filepointer, "%4.3f;", pSeed->elternheight);
											//fprintf(filepointer, "%f;", iquer);
											//fprintf(filepointer, "%f;", jquer);
											fprintf(filepointer, "%4.5f;",sqrt(iquer*iquer+jquer*jquer));
											fprintf(filepointer, "%4.5f;", richtung);
											fprintf(filepointer, "%4.5f;", pSeed->xcoo);
											fprintf(filepointer, "%4.5f;", pSeed->ycoo);	
											fprintf(filepointer, "%d;", pSeed->species);
											fprintf(filepointer, "%d;", parameter[0].weatherchoice);
											fprintf(filepointer, "%d;", parameter[0].thawing_depth);
											fprintf(filepointer, "%lf;",geschwindigkeit);
											fprintf(filepointer, "%lf;", wrichtung);
											fprintf(filepointer, "\n");

											fclose(filepointer);
										}
								}//output end if
							
							
							pSeed->xcoo=pSeed->xcoo+jquer;
							pSeed->ycoo=pSeed->ycoo+iquer;
							pSeed->entfernung=entfernung;	



							/****************************************************************************************//**
							* \brief calculate  Dispersal within a plot
							*
							*
							*******************************************************************************************/
							
								bool sameausserhalb=false;
								
								// Check if the seed is on the plot:
								if(pSeed->ycoo > (double) (treerows-1)) 
								{
									if((parameter[0].boundaryconditions==1))
									{
									   pSeed->ycoo=fmod(pSeed->ycoo,(double)(treerows-1));
									   pSeed->namem=0;
									   pSeed->namep=0;
									} 
									else 
									{
										sameausserhalb=true;
										rausgeflogenN++;
									}
								} 
								else if(pSeed->ycoo<0.0)
								{
									if((parameter[0].boundaryconditions==1))
									{
									   pSeed->ycoo=(double)(treerows-1)+fmod(pSeed->ycoo,(double)(treerows-1));
									   pSeed->namem=0;
									   pSeed->namep=0;
									} 
									else 
									{
										sameausserhalb=true;
										rausgeflogenS++;
									}
								} 
								 
								if(pSeed->xcoo<0.0)
								{
									if((parameter[0].boundaryconditions==1))
									{
									   pSeed->xcoo = fmod(pSeed->xcoo,(double)(treecols-1))+(double)(treecols-1);
									   pSeed->namem=0;
									   pSeed->namep=0;
									} 
									else
									{
										sameausserhalb=true;
										rausgeflogenW++;
									}
								} 
								else if(pSeed->xcoo > (double) (treecols-1)) 
								{
									if(parameter[0].boundaryconditions==1)
									{
										pSeed->xcoo = fmod(pSeed->xcoo,(double)(treecols-1));
										pSeed->namem=0;
										pSeed->namep=0;
																	} 
									else if((parameter[0].boundaryconditions==2) && (rand()<0.5*RAND_MAX))
									{ //less reintroduction from the western border than from the eastern
										pSeed->xcoo = fmod(pSeed->xcoo,(double)(treecols-1));
									} 
									else
									{	
										sameausserhalb=true;
										rausgeflogenO++;
									}
								}
		
								if( (sameausserhalb==false) && ( (pSeed->ycoo<0.0) | (pSeed->ycoo> (double) (treerows-1)) | (pSeed->xcoo<0.0) | (pSeed->xcoo> (double) (treecols-1)) ) )
								{
									printf("\n\nLaVeSi was exited ");
									printf("in Seeddispersal.cpp\n");
									printf("... Reason: dispersed seed is, after deleting it, still part of the simulated plot (Pos(Y=%4.2f,X=%4.2f))\n", pSeed->ycoo, pSeed->xcoo);
									exit(1);
								}

								if(sameausserhalb==true)
								{ // deleting seed Begin
									delete pSeed;
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
					list<Seed*>::iterator pos=seed_list.begin();
					advance(pos, pari);
					pSeed=(*pos);

					
					///If Seed is in a cone
					if (pSeed->imcone==true)
					{
						double flug = 0.0 +( (double) 1.0*rand()/(RAND_MAX + 1.0));
										
						///and random number < rate of emerging seeds
						if(flug <= parameter[0].seedflightrate)
						{ // If the seed disperses a coordinate is calculated
							
							double ratiorn=0.0 +( (double) 1.0*rand()/(RAND_MAX + 1.0));
												
							if(ratiorn>0.0)
							{ // RN > 0 BEGIN
								
								pSeed->imcone=false;
								
								double entfernung = 0;
								double richtung=0.0;
								double geschwindigkeit=0;
								double wrichtung=0.0;
								double jquer=0;
								double iquer=0;

								seeddisp(ratiorn, jquer, iquer, geschwindigkeit, wrichtung, pSeed->elternheight, pSeed->species);
								
								
								// seed dispersal output:
								if(parameter[0].ivort>1045 && parameter[0].outputmode!=9 && parameter[0].omp_num_threads==1)
								{							
										double seedeinschreibzufall=0.0 +( (double) 1.0*rand()/(RAND_MAX + 1.0));
									

										if(seedeinschreibzufall<0.01)
										{
											entfernung=sqrt(pow(iquer,2)+pow(jquer,2));
											richtung=atan2(iquer,jquer);
																	
											FILE *filepointer;
											string dateiname;

											// assembling file name
											char dateinamesuf[12];
											sprintf(dateinamesuf, "%.4d_REP%.3d", parameter[0].weatherchoice,parameter[0].repeati);
											dateiname="output/dataseed_distance" + string(dateinamesuf) + ".csv";
										
											// Trying to open files for reading
											filepointer = fopen (dateiname.c_str(), "r+");
											// if fopen fails, create a new file and header
											if (filepointer == NULL)
											{
											  filepointer = fopen (dateiname.c_str(), "w");
												fprintf(filepointer,"IVORT;");
												fprintf(filepointer, "name;");
												fprintf(filepointer, "year;" );
												fprintf(filepointer, "parentheight;");
												//fprintf(filepointer, "xentfernung;");
												//fprintf(filepointer, "yentfernung;");
												fprintf(filepointer, "distance;");
												fprintf(filepointer, "direction;");
												fprintf(filepointer, "xcoo;");
												fprintf(filepointer, "ycoo;");
												fprintf(filepointer, "species;" );
												fprintf(filepointer, "weatherchoice;");
												fprintf(filepointer, "thawing_depth;");	
												fprintf(filepointer, "windspd;");
												fprintf(filepointer, "winddir;");
												fprintf(filepointer, "\n");

												if (filepointer == NULL)
												{
													fprintf(stderr, "Error: seed distance file could not be opened!\n");
													exit(1);
												}
											}

											fseek(filepointer,0,SEEK_END);

											fprintf(filepointer, "%d;",parameter[0].ivort);
											fprintf(filepointer, "%d;", pSeed->namem);
											fprintf(filepointer, "%d;", jahr);
											fprintf(filepointer, "%4.3f;", pSeed->elternheight);
											//fprintf(filepointer, "%f;", iquer);
											//fprintf(filepointer, "%f;", jquer);
											fprintf(filepointer, "%4.5f;",sqrt(iquer*iquer+jquer*jquer));
											fprintf(filepointer, "%4.5f;", richtung);
											fprintf(filepointer, "%4.5f;", pSeed->xcoo);
											fprintf(filepointer, "%4.5f;", pSeed->ycoo);	
											fprintf(filepointer, "%d;", pSeed->species);
											fprintf(filepointer, "%d;", parameter[0].weatherchoice);
											fprintf(filepointer, "%d;", parameter[0].thawing_depth);
											fprintf(filepointer, "%lf;",geschwindigkeit);
											fprintf(filepointer, "%lf;", wrichtung);
											fprintf(filepointer, "\n");

											fclose(filepointer);
										}
								}//output end if
								
								
								
								pSeed->xcoo=pSeed->xcoo+jquer;
								pSeed->ycoo=pSeed->ycoo+iquer;
								pSeed->entfernung=entfernung;	


							
								/****************************************************************************************//**
								* \brief calculate  Dispersal within a plot
								*
								*
								*******************************************************************************************/
									bool sameausserhalb=false;
									
									// Check if the seed is on the plot:
									if(pSeed->ycoo > (double) (treerows-1)) 
									{
										if((parameter[0].boundaryconditions==1))
										{
										   pSeed->ycoo=fmod(pSeed->ycoo,(double)(treerows-1));
										   pSeed->namem=0;
										   pSeed->namep=0;
										
										} 
										else 
										{
											sameausserhalb=true;
											rausgeflogenN++;
										}
									} 
									else if(pSeed->ycoo<0.0)
									{
										if((parameter[0].boundaryconditions==1))
										{
										   pSeed->ycoo=(double)(treerows-1)+fmod(pSeed->ycoo,(double)(treerows-1));
										   pSeed->namem=0;
										   pSeed->namep=0;
										
										} 
										else 
										{
											sameausserhalb=true;
											rausgeflogenS++;
										}
									}
									 
									if(pSeed->xcoo<0.0)
									{
										if((parameter[0].boundaryconditions==1))
										{
										   pSeed->xcoo = fmod(pSeed->xcoo,(double)(treecols-1))+(double)(treecols-1);
										   pSeed->namem=0;
										   pSeed->namep=0;
										} 
										else
										{
											sameausserhalb=true;
											rausgeflogenW++;
										}
									} 
									else if(pSeed->xcoo > (double) (treecols-1)) 
									{
										if(parameter[0].boundaryconditions==1)
										{
											pSeed->xcoo = fmod(pSeed->xcoo,(double)(treecols-1));
											pSeed->namem=0;
											pSeed->namep=0;
										   
										} 
										else if((parameter[0].boundaryconditions==2) && (rand()<0.5*RAND_MAX))
										{ //less seed reintroduction on the western border
											pSeed->xcoo = fmod(pSeed->xcoo,(double)(treecols-1));
										} 
										else
										{	
											sameausserhalb=true;
											rausgeflogenO++;
										}
									} 
									
			
									if( (sameausserhalb==false) && ( (pSeed->ycoo<0.0) | (pSeed->ycoo> (double) (treerows-1)) | (pSeed->xcoo<0.0) | (pSeed->xcoo> (double) (treecols-1)) ) )
									{
										printf("\n\nLaVeSi was exited ");
										printf("in Seeddispersal.cpp\n");
										printf("... Reason: dispersed seed is, after deleting it, still part of the simulated plot (Pos(Y=%4.2f,X=%4.2f))\n", pSeed->ycoo, pSeed->xcoo);
										exit(1);
									}

									if(sameausserhalb==true)
									{ // Seed deletion BEGIN
										delete pSeed;
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
					
				#pragma omp parallel private(pSeed)
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
						pSeed=(*it);
					
					
						///Loop around all Seeds
						///If Seed is in a cone
						if (pSeed->imcone==true)
						{
							double flug = 0.0 +( (double) 1.0*rand()/(RAND_MAX + 1.0));
											
							///and random number < rate of emerging seeds
							if(flug <= parameter[0].seedflightrate)
							{ // If the seed disperses a coordinate is calculated
								
								double ratiorn=0.0 +( (double) 1.0*rand()/(RAND_MAX + 1.0));
													
								if(ratiorn>0.0)
								{ // RN >0 BEGIN
									
									pSeed->imcone=false;
									
									double entfernung=0.0;
									double richtung=0.0;
									double geschwindigkeit=0.0;
									double wrichtung=0.0;
									double jquer=0;
									double iquer=0;

									seeddisp(ratiorn, jquer, iquer, geschwindigkeit, wrichtung, pSeed->elternheight, pSeed->species);
									
									// seed dispersal output:
									if(parameter[0].ivort>1045 && parameter[0].outputmode!=9 && parameter[0].omp_num_threads==1)
									{
											double seedeinschreibzufall=0.0 +( (double) 1.0*rand()/(RAND_MAX + 1.0));
										


											if(seedeinschreibzufall<0.01)
											{
												entfernung=sqrt(pow(iquer,2)+pow(jquer,2));
												richtung=atan2(iquer,jquer);
																		
												//distance output begin
												FILE *filepointer;
												string dateiname;

												// assemble file name
												char dateinamesuf[12];
												sprintf(dateinamesuf, "%.4d_REP%.3d", parameter[0].weatherchoice,parameter[0].repeati);
												dateiname="output/dataseed_distance" + string(dateinamesuf) + ".csv";
											
												// trying to open file for reading
												filepointer = fopen (dateiname.c_str(), "r+");
												// if fopen fails create a new file and header
												if (filepointer == NULL)
												{
												  filepointer = fopen (dateiname.c_str(), "w");
													fprintf(filepointer,"IVORT;");
													fprintf(filepointer, "name;");
													fprintf(filepointer, "year;" );
													fprintf(filepointer, "parentheight;");
													fprintf(filepointer, "distance;");
													fprintf(filepointer, "direction;");
													fprintf(filepointer, "xcoo;");
													fprintf(filepointer, "ycoo;");
													fprintf(filepointer, "species;" );
													fprintf(filepointer, "weatherchoice;");
													fprintf(filepointer, "thawing_depth;");	
													fprintf(filepointer, "windspd;");
													fprintf(filepointer, "winddir;");
													fprintf(filepointer, "\n");

													if (filepointer == NULL)
													{
														fprintf(stderr, "Fehler: seed distance file could not be opened!\n");
														exit(1);
													}
												}

												fseek(filepointer,0,SEEK_END);

												fprintf(filepointer, "%d;",parameter[0].ivort);
												fprintf(filepointer, "%d;", pSeed->namem);
												fprintf(filepointer, "%d;", jahr);
												fprintf(filepointer, "%4.3f;", pSeed->elternheight);
												fprintf(filepointer, "%4.5f;",entfernung);
												fprintf(filepointer, "%4.5f;", richtung);
												fprintf(filepointer, "%4.5f;", pSeed->xcoo);
												fprintf(filepointer, "%4.5f;", pSeed->ycoo);	
												fprintf(filepointer, "%d;", pSeed->species);
												fprintf(filepointer, "%d;", parameter[0].weatherchoice);
												fprintf(filepointer, "%d;", parameter[0].thawing_depth);
												fprintf(filepointer, "%lf;",geschwindigkeit);
												fprintf(filepointer, "%lf;", wrichtung);
												fprintf(filepointer, "\n");

												fclose(filepointer);
											}
									}//output end if
									
									pSeed->xcoo=pSeed->xcoo+jquer;
									pSeed->ycoo=pSeed->ycoo+iquer;
									pSeed->entfernung=entfernung;	

								
									/****************************************************************************************//**
									* \brief calculate  Dispersal within a plot
									*
									*
									*******************************************************************************************/
										bool sameausserhalb=false;
										
										// Check if the seed is on the plot:
										if(pSeed->ycoo > (double) (treerows-1)) 
										{
											if((parameter[0].boundaryconditions==1))
											{
											   pSeed->ycoo=fmod(pSeed->ycoo,(double)(treerows-1));
											   pSeed->namem=0;
											   pSeed->namep=0;
											} 
											else if((parameter[0].boundaryconditions==3))
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
										else if(pSeed->ycoo<0.0)
										{
											if((parameter[0].boundaryconditions==1))
											{
											   pSeed->ycoo=(double)(treerows-1)+fmod(pSeed->ycoo,(double)(treerows-1));
											   pSeed->namem=0;
											   pSeed->namep=0;
											} 
											else if((parameter[0].boundaryconditions==3))
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
										 
										if(pSeed->xcoo<0.0)
										{
											if((parameter[0].boundaryconditions==1 || parameter[0].boundaryconditions==3))
											{
											   pSeed->xcoo = fmod(pSeed->xcoo,(double)(treecols-1))+(double)(treecols-1);
											   pSeed->namem=0;
											   pSeed->namep=0;
											} 
											else
											{
												sameausserhalb=true;
												rausgeflogenW++;
											}
										} 
										else if(pSeed->xcoo > (double) (treecols-1))
										{
											if(parameter[0].boundaryconditions==1 || parameter[0].boundaryconditions==3)
											{
												pSeed->xcoo = fmod(pSeed->xcoo,(double)(treecols-1));
												pSeed->namem=0;
												pSeed->namep=0;
				   
											} 
											else if((parameter[0].boundaryconditions==2) && (rand()<0.5*RAND_MAX))
											{ // Reducing seed introduction on the western border:
												pSeed->xcoo = fmod(pSeed->xcoo,(double)(treecols-1));
											} 
											else
											{	
												sameausserhalb=true;
												rausgeflogenO++;
											}
										} 
				
										if( (sameausserhalb==false) && ( (pSeed->ycoo<0.0) | (pSeed->ycoo> (double) (treerows-1)) | (pSeed->xcoo<0.0) | (pSeed->xcoo> (double) (treecols-1)) ) )
										{
											printf("\n\nLaVeSi was exited ");
											printf("in Seeddispersal.cpp\n");
											printf("... Reason: dispersed seed is, after deleting it, still part of the simulated plot (Pos(Y=%4.2f,X=%4.2f))\n", pSeed->ycoo, pSeed->xcoo);
											exit(1);
										}

										if(sameausserhalb==true)
										{
											pSeed->ycoo=-99999.9;
											// pSeed->xcoo=-99999.9;
										}
								} // RN > 0 END
							}   // If the seed disperses a coordinate is calculated END
						} //Imcone? END
					} // seed_list loop END
				}//END: pragma for
				
				for(list<Seed*>::iterator pos = seed_list.begin(); pos != seed_list.end();)
				{// START: delete seeds with sameausserhalb==true
					pSeed=(*pos);

					if(pSeed->ycoo==-99999.9)
					{ //Seed deletion BEGIN
						delete pSeed;
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































