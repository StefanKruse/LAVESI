#include "LAVESI.h"				   
using namespace std;

/****************************************************************************************//**
 * \brief calculate dispersal distance dependent on dispersal mode (0-5)
 *
 *
 *
 *
 *******************************************************************************************/

void Seedoutput(int aktort, double dispersaldistance, float direction, int neueweltcoo) 
{
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
			fprintf(filepointer, "dispersaldistance;");
			fprintf(filepointer, "direction;");
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
		fprintf(filepointer, "%4.5f;", dispersaldistance);
		fprintf(filepointer, "%4.5f;", direction);
		fprintf(filepointer, "%d;", neueweltcoo);		
		fprintf(filepointer, "\n");

		fclose(filepointer);
}



/****************************************************************************************//**
 * \brief calculate seed dispersal
 *
 *
 *
 * 
 *
 *******************************************************************************************/

 void Seeddispersal(int jahr, struct Parameter *parameter, vector<list<Seed*> > &world_seed_list)
{
	int aktort=0;
			
	for(vector<list<Seed*> >::iterator posw = world_seed_list.begin(); posw != world_seed_list.end(); ++posw)
	{ 
		list<Seed*>& seed_list = *posw;
		
		// determine the current location, so that in long distance dispersal the target can be determined
		aktort++;

		// variable for displaying seeds crossing the borders
		int rausgeflogenN=0, rausgeflogenO=0, rausgeflogenS=0, rausgeflogenW=0;

		if(parameter[0].omp_num_threads==0)
		{
			double cum_time_individual_seed=0;
			double cum_time_Seedwinddispersal=0;
				
			for(list<Seed*>::iterator pos = seed_list.begin(); pos != seed_list.end();)
			{
				double time_start_individual_seed=omp_get_wtime();
				
				pSeed=(*pos);

				if (pSeed->incone==true)
				{
					double flug = 0.0 +( (double) 1.0*rand()/(RAND_MAX + 1.0));
									
					if(flug <= parameter[0].seedflightrate)
					{
						double ratiorn=0.0 +( (double) 1.0*rand()/(RAND_MAX + 1.0));
											
						if(ratiorn>0.0)
						{
							pSeed->incone=false;
							
							double dispersaldistance = 0;
							double direction=0.0;
							double velocity=0;
							double wdirection=0.0;
							
							double jquer=0;
							double iquer=0;

							double time_start_individual_seed_Seedwinddispersal=omp_get_wtime();
							Seedwinddispersal(ratiorn, jquer, iquer, velocity, wdirection, pSeed->releaseheight, pSeed->species);
							cum_time_Seedwinddispersal+=omp_get_wtime()-time_start_individual_seed_Seedwinddispersal;
							dispersaldistance=sqrt(pow(iquer,2)+pow(jquer,2));
							if(parameter[0].ivort>1045 && parameter[0].outputmode!=9)
							{							
								double seedeinschreibzufall=0.0 +( (double) 1.0*rand()/(RAND_MAX + 1.0));
							
								if(seedeinschreibzufall<0.01)
								{
									dispersaldistance=sqrt(pow(iquer,2)+pow(jquer,2));
									direction=atan2(iquer,jquer);
															
									FILE *filepointer;
									string dateiname;

									// assembling file name
									char dateinamesuf[12];
									sprintf(dateinamesuf, "%.4d_REP%.3d", parameter[0].weatherchoice,parameter[0].repeati);
									dateiname="output/dataseed_distance" + string(dateinamesuf) + ".csv";
								
									filepointer = fopen (dateiname.c_str(), "r+");
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
											fprintf(stderr, "Error: Seed distance file could not be opened!\n");
											exit(1);
										}
									}

									fseek(filepointer,0,SEEK_END);

									fprintf(filepointer, "%d;",parameter[0].ivort);
									fprintf(filepointer, "%d;", pSeed->namem);
									fprintf(filepointer, "%d;", jahr);
									fprintf(filepointer, "%4.3f;", pSeed->releaseheight);
									fprintf(filepointer, "%4.5f;",sqrt(iquer*iquer+jquer*jquer));
									fprintf(filepointer, "%4.5f;", direction);
									fprintf(filepointer, "%4.5f;", pSeed->xcoo);
									fprintf(filepointer, "%4.5f;", pSeed->ycoo);	
									fprintf(filepointer, "%d;", pSeed->species);
									fprintf(filepointer, "%d;", parameter[0].weatherchoice);
									fprintf(filepointer, "%d;", parameter[0].thawing_depth);
									fprintf(filepointer, "%lf;",velocity);
									fprintf(filepointer, "%lf;", wdirection);
									fprintf(filepointer, "\n");

									fclose(filepointer);
								}
							}
						
							pSeed->xcoo=pSeed->xcoo+jquer/pow(pSeed->currentweight,parameter[0].seedweightfactor);
							pSeed->ycoo=pSeed->ycoo+iquer/pow(pSeed->currentweight,parameter[0].seedweightfactor);
							pSeed->dispersaldistance=dispersaldistance/pow(pSeed->currentweight,parameter[0].seedweightfactor);


							/****************************************************************************************//**
							* \brief calculate  Dispersal within a plot
							*
							*
							*******************************************************************************************/
							
							bool sameausserhalb=false;
							
							// check if the seed is on the plot:
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
								{
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
								delete pSeed;
								pos=seed_list.erase(pos);
							} 
							else
							{
								++pos;
							}

						}
					} 
					else
					{
						++pos;
					}
				} 
				else
				{
					++pos;
				}
				
				cum_time_individual_seed+=omp_get_wtime()-time_start_individual_seed;
			}
			
			cout << endl << "All seeds:" << cum_time_individual_seed << " with Seedwinddispersal-function:" << cum_time_Seedwinddispersal << endl;
		}
		else
		{// multi-core-processing
			// manually chose the implementation of multi-core-processing
			int mcorevariant=2;
				// 1 == only advance, which slows down the computation
				// 2 == split list to X lists
			if(mcorevariant==1)
			{// OMP==1
				for(unsigned int pari=0; pari<seed_list.size(); )
				{
					list<Seed*>::iterator pos=seed_list.begin();
					advance(pos, pari);
					pSeed=(*pos);
					
					if (pSeed->incone==true)
					{
						double flug = 0.0 +( (double) 1.0*rand()/(RAND_MAX + 1.0));
										
						if(flug <= parameter[0].seedflightrate)
						{
							
							double ratiorn=0.0 +( (double) 1.0*rand()/(RAND_MAX + 1.0));
												
							if(ratiorn>0.0)
							{
								
								pSeed->incone=false;
								
								double dispersaldistance = 0;
								double direction=0.0;
								double velocity=0;
								double wdirection=0.0;
								double jquer=0;
								double iquer=0;

								Seedwinddispersal(ratiorn, jquer, iquer, velocity, wdirection, pSeed->releaseheight, pSeed->species);
								dispersaldistance=sqrt(pow(iquer,2)+pow(jquer,2));
								if(parameter[0].ivort>1045 && parameter[0].outputmode!=9 && parameter[0].omp_num_threads==1)
								{
									double seedeinschreibzufall=0.0 +( (double) 1.0*rand()/(RAND_MAX + 1.0));

									if(seedeinschreibzufall<0.01)
									{
										dispersaldistance=sqrt(pow(iquer,2)+pow(jquer,2));
										direction=atan2(iquer,jquer);
																
										FILE *filepointer;
										string dateiname;

										// assembling file name
										char dateinamesuf[12];
										sprintf(dateinamesuf, "%.4d_REP%.3d", parameter[0].weatherchoice,parameter[0].repeati);
										dateiname="output/dataseed_distance" + string(dateinamesuf) + ".csv";

										filepointer = fopen (dateiname.c_str(), "r+");
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
												fprintf(stderr, "Error: seed distance file could not be opened!\n");
												exit(1);
											}
										}

										fseek(filepointer,0,SEEK_END);

										fprintf(filepointer, "%d;",parameter[0].ivort);
										fprintf(filepointer, "%d;", pSeed->namem);
										fprintf(filepointer, "%d;", jahr);
										fprintf(filepointer, "%4.3f;", pSeed->releaseheight);
										fprintf(filepointer, "%4.5f;",sqrt(iquer*iquer+jquer*jquer));
										fprintf(filepointer, "%4.5f;", direction);
										fprintf(filepointer, "%4.5f;", pSeed->xcoo);
										fprintf(filepointer, "%4.5f;", pSeed->ycoo);	
										fprintf(filepointer, "%d;", pSeed->species);
										fprintf(filepointer, "%d;", parameter[0].weatherchoice);
										fprintf(filepointer, "%d;", parameter[0].thawing_depth);
										fprintf(filepointer, "%lf;",velocity);
										fprintf(filepointer, "%lf;", wdirection);
										fprintf(filepointer, "\n");

										fclose(filepointer);
									}
								}
								
							pSeed->xcoo=pSeed->xcoo+jquer/pow(pSeed->currentweight,parameter[0].seedweightfactor);
							pSeed->ycoo=pSeed->ycoo+iquer/pow(pSeed->currentweight,parameter[0].seedweightfactor);
							pSeed->dispersaldistance=dispersaldistance/pow(pSeed->currentweight,parameter[0].seedweightfactor);

							
								/****************************************************************************************//**
								* \brief calculate Dispersal within a plot
								*
								*
								*******************************************************************************************/
								bool sameausserhalb=false;
								
								// check if the seed is on the plot:
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
									{
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
									delete pSeed;
									pos=seed_list.erase(pos);
									++pari;
								} 
								else
								{
									++pari;
								}
							}
						} 
						else
						{
							++pari;
						}
					} 
					else
					{
						++pari;
					}
				}
			}// OMP==1
			if(mcorevariant==2)
			{// OMP==2
				omp_set_dynamic(0); // disable dynamic teams
				omp_set_num_threads(parameter[0].omp_num_threads); // set the number of helpers
					
				#pragma omp parallel private(pSeed)
				{
					int thread_count=omp_get_num_threads();
					int thread_num=omp_get_thread_num();
					size_t chunk_size=seed_list.size()/thread_count;
					auto begin=seed_list.begin();
					std::advance(begin,thread_num*chunk_size);
					auto end=begin;
					
					if(thread_num==(thread_count-1))
					{
						end = seed_list.end();
					} 
					else
					{
						std::advance(end, chunk_size);
					}
					
					#pragma omp barrier
					for(auto it = begin; it != end; ++it)
					{
						pSeed=(*it);
					
						if (pSeed->incone==true)
						{
							double flug = 0.0 +( (double) 1.0*rand()/(RAND_MAX + 1.0));
											
							if(flug <= parameter[0].seedflightrate)
							{
								double ratiorn=0.0 +( (double) 1.0*rand()/(RAND_MAX + 1.0));
													
								if(ratiorn>0.0)
								{
									
									pSeed->incone=false;
									
									double dispersaldistance=0.0;
									double direction=0.0;
									double velocity=0.0;
									double wdirection=0.0;
									double jquer=0;
									double iquer=0;

									Seedwinddispersal(ratiorn, jquer, iquer, velocity, wdirection, pSeed->releaseheight, pSeed->species);
									dispersaldistance=sqrt(pow(iquer,2)+pow(jquer,2));
									if(parameter[0].ivort>1045 && parameter[0].outputmode!=9 && parameter[0].omp_num_threads==1)
									{
											double seedeinschreibzufall=0.0 +( (double) 1.0*rand()/(RAND_MAX + 1.0));

											if(seedeinschreibzufall<0.01)
											{
												dispersaldistance=sqrt(pow(iquer,2)+pow(jquer,2));
												direction=atan2(iquer,jquer);
																		
												FILE *filepointer;
												string dateiname;

												// assemble file name
												char dateinamesuf[12];
												sprintf(dateinamesuf, "%.4d_REP%.3d", parameter[0].weatherchoice,parameter[0].repeati);
												dateiname="output/dataseed_distance" + string(dateinamesuf) + ".csv";
											
												filepointer = fopen (dateiname.c_str(), "r+");
												if (filepointer == NULL)
												{
													filepointer = fopen (dateiname.c_str(), "w");
												  
													fprintf(filepointer, "IVORT;");
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
														fprintf(stderr, "Error: seed distance file could not be opened!\n");
														exit(1);
													}
												}

												fseek(filepointer,0,SEEK_END);

												fprintf(filepointer, "%d;",parameter[0].ivort);
												fprintf(filepointer, "%d;", pSeed->namem);
												fprintf(filepointer, "%d;", jahr);
												fprintf(filepointer, "%4.3f;", pSeed->releaseheight);
												fprintf(filepointer, "%4.5f;",dispersaldistance);
												fprintf(filepointer, "%4.5f;", direction);
												fprintf(filepointer, "%4.5f;", pSeed->xcoo);
												fprintf(filepointer, "%4.5f;", pSeed->ycoo);	
												fprintf(filepointer, "%d;", pSeed->species);
												fprintf(filepointer, "%d;", parameter[0].weatherchoice);
												fprintf(filepointer, "%d;", parameter[0].thawing_depth);
												fprintf(filepointer, "%lf;",velocity);
												fprintf(filepointer, "%lf;", wdirection);
												fprintf(filepointer, "\n");

												fclose(filepointer);
											}
									}
									
									pSeed->xcoo=pSeed->xcoo+jquer/pow(pSeed->currentweight,parameter[0].seedweightfactor);
									pSeed->ycoo=pSeed->ycoo+iquer/pow(pSeed->currentweight,parameter[0].seedweightfactor);
									pSeed->dispersaldistance=dispersaldistance/pow(pSeed->currentweight,parameter[0].seedweightfactor);

									
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
									}
								}
							}
						}
					}
				}// pragma
				
				// delete seeds which exceed the plot's borders
				for(list<Seed*>::iterator pos = seed_list.begin(); pos != seed_list.end();)
				{
					pSeed=(*pos);

					if(pSeed->ycoo==-99999.9)
					{
						delete pSeed;
						pos=seed_list.erase(pos);
					} 
					else
					{
						++pos;
					}
				}
			}// OMP==2
		}
		
		// display seed numbers leaving the plot:
		if (parameter[0].seedwinddispersalvis==true) 
			printf("\n   Leaving seeds (N/O/S/W)=(%d/%d/%d/%d) ", rausgeflogenN, rausgeflogenO, rausgeflogenS, rausgeflogenW);
	}
}

