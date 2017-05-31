
/****************************************************************************************//**
 * \brief 
 *
 * calculate simple density value for each tree
 *
 *
 *
 *******************************************************************************************/
void AddTreeDensity(list<Tree*> &tree_list, vector<Karten*> &plot_list)
{

		for (list<Tree*>::iterator pos = tree_list.begin(); pos != tree_list.end(); )
		{ // tree_list Beginn
			pTree=(*pos);
			

			int i=(int) floor(pTree->ycoo*parameter[0].sizemagnif);
			int j=(int) floor(pTree->xcoo*parameter[0].sizemagnif);

			// Raster um Tree in Abh. von Dbasal ablaufen ...
			// ... und in Abh. von Entfernung der Flaecheneinheit einen densitywert eintragen
			// Umrechnung des Radius in Meter
			// = dbasal*parameter[0].incfac/100.0;
			
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
			
			//StefanC: Wenn der Tree nur eine densitykachel beeinflusst
			if ( flaechengroesze<(1.0/parameter[0].sizemagnif) )
			{
				plot_list[i*treecols*parameter[0].sizemagnif+j]->Treedensitywert += 
										pow(
													pow(flaechengroesze/(1.0/parameter[0].sizemagnif),parameter[0].densitysmallweighing)
													//weighing with diameter
													*
													pow(pTree->dbasal, parameter[0].densitytreetile),
											parameter[0].densitywertmanipulatorexp);
				plot_list[i*treecols*parameter[0].sizemagnif+j]->Treeanzahl++;
				// pTree->densitywert=pow(pTree->dbasal, parameter[0].densitytreetile);
				pTree->densitywert=pow(
								pow(pTree->dbasal, parameter[0].densitytreetile)*pow(flaechengroesze/(1.0/parameter[0].sizemagnif),parameter[0].densitysmallweighing),
								parameter[0].densitywertmanipulatorexp);
			}
			
			//StefanC: Wenn der Tree mehr als eine densitykachel beeinflusst
			else
			//Treedichtwert nimmt Kegelförmig ab
			{
				// Ausmasze des abzulaufenden Rasters um den Tree bestimmen
				int xyquerrastpos= (int) floor( flaechengroesze*parameter[0].sizemagnif );
				// Versetzte Koordinaten ermitteln und Mitsummieren des densitywertes
				double sumdensitywert=0;
				
				for (int rastposi=(i+xyquerrastpos); rastposi>(i-(xyquerrastpos+1)); rastposi--)
				{
					for (int rastposj=(j-xyquerrastpos); rastposj<(j+xyquerrastpos+1); rastposj++)
					{
						if ( ( rastposi<=( (treerows-1)*parameter[0].sizemagnif) && rastposi>=0 ) && ( rastposj<=( (treecols-1)*parameter[0].sizemagnif) && rastposj>=0 ) )
						{
							// Entfernungsberechnung um Groesze des densitywertes in Raumeinheit zu bestimmen ...
							// ... und eintragen des Wertes an jenen Ort
							double entfrastpos=sqrt(pow(double(i-rastposi),2)+pow(double(j-rastposj),2));
									// Überprüfungsausgabe:
									// // //cout << "Gesamtentf=" << xyquerrastpos << "  & akt. Entf=" << entfrastpos;
									//cout.precision(2);
									//cout.setf( ios::fixed, ios::floatfield );
							// Nur wenn die aktuelle Kachel in den Einflussbereich faellt, wird ein Wert eingeschrieben
							if (entfrastpos<= (double) xyquerrastpos)
							{
								plot_list[rastposi*treecols*parameter[0].sizemagnif+rastposj]->Treedensitywert 
									+= 
									pow(
										pow(pTree->dbasal, parameter[0].densitytreetile)/(entfrastpos+1.0),
										parameter[0].densitywertmanipulatorexp);
									
								plot_list[rastposi*treecols*parameter[0].sizemagnif+rastposj]->Treeanzahl++;

								sumdensitywert+=pow(
													pow(pTree->dbasal, parameter[0].densitytreetile)/(entfrastpos+1.0),
													parameter[0].densitywertmanipulatorexp);
									// Überprüfungsausgabe:
									//cout << pow(pTree->dbasal, parameter[0].densitytreetile)/(entfrastpos+1.0) << "\t";
							}
							else
							{
								// Überprüfungsausgabe:
								//cout << 0.0 << "\t";
							}
						}
					}
								//cout << endl;
				}
								//cout << endl;
								//cout << endl;

				pTree->densitywert=sumdensitywert;
			}

			++pos;
		} // tree_list Ende
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
 * 2: ZOI-kind of type and pTree->densitywert=1.0-(density_help/plot_list[i*treecols*parameter[0].sizemagnif+j]->Treedensitywert) or \n
 *							sumdensitywert+=plot_list[rastposi*treecols*parameter[0].sizemagnif+rastposj]->Treedensitywert; \n
 *							pTree->densitywert=1.0-(density_help/sumdensitywert);	
 *							pTree->densitywert= pTree->densitywert *pow((1.0-(0.01/pTree->dbasal)),parameter[0].densitywertdbasaleinfluss);	
 *
 * 3: random field of ZOI-Type and pTree->densitywert=1.0-(density_help/plot_list[izuf*treecols*parameter[0].sizemagnif+jzuf]->Treedensitywert) or \n
 *								   sumdensitywert+=plot_list[izuf*treecols*parameter[0].sizemagnif+jzuf]->Treedensitywert \n
 *								   pTree->densitywert=1.0-(density_help/sumdensitywert);
 *								   pTree->densitywert= pTree->densitywert *pow((1.0-(0.01/pTree->dbasal)),parameter[0].densitywertdbasaleinfluss);
 *******************************************************************************************/

 
 
  
 
 
// densitywert: beinhaltet Stärke der umgebenden Konkurrenz und den Raum den er theoretisch zum Wachsen benötigt 
// Treedensitywert: beinhaltet wieviel Raum ihm in Ahängigkeit der umgebenden Vegetation und unter Berücksichtigung der  Entfernung der Konkurrenz zur Verfügung steht




void IndividualTreeDensity(list<Tree*> &tree_list, vector<Karten*> &plot_list)
{

	if(parameter[0].omp_num_threads==1)
	{// only one core
		for (list<Tree*>::iterator pos = tree_list.begin(); pos != tree_list.end(); ++pos)
		{// START: loop over each tree
			pTree=(*pos);

			///Get tree position
			// 				ycoo = exact position
			//				ycoo* sizemagnif = cell number
			int i = (int) floor(pTree->ycoo * parameter[0].sizemagnif);
			int j = (int) floor(pTree->xcoo * parameter[0].sizemagnif);



			//DENSITY 1
			if (parameter[0].densityart == 1)
			{
				pTree->densitywert = 0.0;
			}
			else 
			{	
				//double density_help = pTree->densitywert;
				
				// Raster um Tree in Abh. von Dbasal ablaufen
				// und in Abh. von Entfernung der Flaecheneinheit einen densitywert eintragen
				// Umrechnung des Radius in Meter

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

				//wenn der Tree nur eine Zelle beeinflusst
				if ( flaechengroesze < (1.0/parameter[0].sizemagnif) )
				{
					// DENSITY 2
					if (parameter[0].densityart == 2)
					{
						if (plot_list[i*treecols*parameter[0].sizemagnif+j]->Treedensitywert > 0.0) 
						{
							if (parameter[0].densitytiletree==1)	// Summe der Werte
							{
								pTree->densitywert =	(1.0 - (pTree->densitywert / plot_list[i*treecols*parameter[0].sizemagnif+j]->Treedensitywert));
								//                           density_soll(an position) / density_ist(an position)
							}
							else if (parameter[0].densitytiletree==2)	// Multiplikation der Werte
							{
								pTree->densitywert =	(1.0 - (pTree->densitywert
															/ 
															(plot_list[i*treecols*parameter[0].sizemagnif+j]->Treedensitywert*pTree->densitywert) 
															)
													);
							//                         density_soll(an position) / density_ist(an position)
							}
						}
						
						else 
							pTree->densitywert=0.0; // keine Konkurrenz
					}
					


					// DENSITY 3
					else if (parameter[0].densityart == 3)
					{
						
						// Ziehe zufaellige Position im Gitter und weise den Wert zu
						int izuf= (int) floor( 0.0 + ( (double)  ( ((double) (treerows-1)) * parameter[0].sizemagnif * rand() / (RAND_MAX + 1.0))) );
						int jzuf= (int) floor( 0.0 + ( (double)  ( ((double) (treecols-1)) * parameter[0].sizemagnif * rand() / (RAND_MAX + 1.0))) );
						
						if (plot_list[izuf*treecols*parameter[0].sizemagnif+jzuf]->Treedensitywert > 0.0) 
						{
							if (parameter[0].densitytiletree==1)	//StefanC: Summe der Werte
							{
								pTree->densitywert = (1.0 - (pTree->densitywert/ plot_list[izuf*treecols*parameter[0].sizemagnif+jzuf]->Treedensitywert));
								//                           density_soll(an position) / density_ist(an position)
							}
							else if (parameter[0].densitytiletree==2)	// Multiplikation der Werte
							{
								pTree->densitywert =	(1.0 - (pTree->densitywert
															/ 
															(plot_list[i*treecols*parameter[0].sizemagnif+j]->Treedensitywert*pTree->densitywert) 
															)
													);
							}
						}
						else 
							pTree->densitywert = 0.0;
					}
					
					
					// Berechne den Einfluss der thawing_depth auf das Wachstum des Treees
					if ((plot_list[i*treecols*parameter[0].sizemagnif+j]->maxthawing_depth<2000) && (parameter[0].thawing_depth==true && parameter[0].einschwingen==false)) 
						pTree->thawing_depthinfluence= (unsigned short) ((200.0/2000.0)* (double) plot_list[i*treecols*parameter[0].sizemagnif+j]->maxthawing_depth);
					
					else
						pTree->thawing_depthinfluence=100;
				}
				

				
				
				// ... wenn der Tree mehrere Zellen beeinflusst
				else
				{
					// Ausmasze des abzulaufenden Rasters um den Tree bestimmen
					int xyquerrastpos= (int) floor( flaechengroesze*parameter[0].sizemagnif );
					double maxdist= sqrt(pow(double(xyquerrastpos),2)+pow(double(xyquerrastpos),2));

					// Versetzte Koordinaten ermitteln und Mitsummieren des densitywertes
					double sumdensitywert=0;
					double sumthawing_depth=0;
					
					unsigned int anzahlflaechen=0;
					
					for (int rastposi=(i+xyquerrastpos); rastposi>(i-(xyquerrastpos+1)); rastposi--)
					{
						for (int rastposj=(j-xyquerrastpos); rastposj<(j+xyquerrastpos+1); rastposj++)
						{
							if ((rastposi<=( (treerows-1)*parameter[0].sizemagnif) && rastposi>=0) && ( rastposj<=( (treecols-1)*parameter[0].sizemagnif) && rastposj>=0))
							{
								// Entfernungsberechnung um Einfluss des densitywertes in Raumeinheit zu bestimmen ...
								// ... wie beim Eintragen
								double entfrastpos=sqrt(pow(double(i-rastposi),2)+pow(double(j-rastposj),2));
								
								// Nur wenn das aktuelle Kachel in den Einflussbereich faellt, wird ein Wert eingeschrieben
								if (entfrastpos<= (double) xyquerrastpos)
								{
									int icurr=rastposi;
									int jcurr=rastposj;

									// Change of i/j vals only if DENSITY 3
									if (parameter[0].densityart==3)
									{
										// Zufaellig ermitteln der Position von der die densitywerte genommen werden
										icurr= (int) floor( 0.0 + ( (double)  ( ((double) (treerows-1)) *parameter[0].sizemagnif*rand()/(RAND_MAX + 1.0))) );
										jcurr= (int) floor( 0.0 + ( (double)  ( ((double) (treecols-1)) *parameter[0].sizemagnif*rand()/(RAND_MAX + 1.0))) );
									}
									
									if (parameter[0].densitytiletree==1)	// Summe der Werte
									{
										sumdensitywert+=plot_list[icurr*treecols*parameter[0].sizemagnif+jcurr]->Treedensitywert
														* (1-entfrastpos/maxdist);	// Hinzugefuegt damit die Werte an der Peripherie schwaecher wirken, sonst wuerde es zu sehr groszen unrealistischen Werten fuehren
									}
									else if (parameter[0].densitytiletree==2)	// Multiplikation der Werte
									{
										// Nach der Wichtung der zusaetzlichen Werte anhand der individuellen Einflusswerte wird das Grundniveau dazuaddiert
										sumdensitywert+=(plot_list[icurr*treecols*parameter[0].sizemagnif+jcurr]->Treedensitywert
														-pow(pTree->dbasal, parameter[0].densitytreetile)/(entfrastpos+1.0)/*Treeeinflusswert an der akt Position*/ )
														* pow(pTree->dbasal, parameter[0].densitytreetile)/(entfrastpos+1.0)/*Treeeinflusswert an der akt Position*/
														
														+ pow(pTree->dbasal, parameter[0].densitytreetile)/(entfrastpos+1.0)/*Treeeinflusswert an der akt Position*/;
									}

									sumthawing_depth+=(double) plot_list[rastposi*treecols*parameter[0].sizemagnif+rastposj]->maxthawing_depth;
									anzahlflaechen++;
									
								}

							}

						}

					}


					if (sumdensitywert > 0.0) 
						pTree->densitywert = 1.0 - (pTree->densitywert/sumdensitywert);
													//density_soll / density_ist
					else 
						pTree->densitywert=0.0;

						
					sumthawing_depth=( sumthawing_depth / (double) anzahlflaechen );
					
					if (sumthawing_depth  <2000)
						pTree->thawing_depthinfluence = (unsigned short) ((200.0/2000.0) * sumthawing_depth);
					
					else
						pTree->thawing_depthinfluence = 100;

				}


				pTree->densitywert= pTree->densitywert
										  *pow((1.0-(0.01/pTree->dbasal)),parameter[0].densitywertdbasaleinfluss);	// Optional: Je groeszer der Tree, desto heightr der Einfluss
				
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
				else if (parameter[0].dichtheightrel==10) // Hinzugefügt am 13.11. um die Größenklassenverteilung besser zu fitten
				{
					// densitywert wird kleiner je großer der Tree ist
					// ... linear
					double densitywertminimum=0.1;
					double ageseinfluss=( -1*(1-densitywertminimum)/100.0* (double) pTree->age + 1.0 );
					if(ageseinfluss<densitywertminimum)
					{
						ageseinfluss=densitywertminimum;
					}
					pTree->densitywert= pTree->densitywert*ageseinfluss;
				}
				else if (parameter[0].dichtheightrel==11) // Hinzugefügt am 13.11. um die Größenklassenverteilung besser zu fitten
				{
					// densitywert wird kleiner je großer der Tree ist
					// ... linear
					double densitywertminimum=0.5;
					double ageseinfluss=( -1*(1-densitywertminimum)/100.0* (double) pTree->age + 1.0 );
					if(ageseinfluss<densitywertminimum)
					{
						ageseinfluss=densitywertminimum;
					}
					pTree->densitywert= pTree->densitywert*ageseinfluss;
				}
				else if (parameter[0].dichtheightrel==12) // Hinzugefügt am 13.11. um die Größenklassenverteilung besser zu fitten
				{
					// densitywert wird kleiner je großer der Tree ist
					// ... linear
					double densitywertminimum=0.1;
					double ageseinfluss=( -1*(1-densitywertminimum)/50.0* (double) pTree->age + 1.0 );
					if(ageseinfluss<densitywertminimum)
					{
						ageseinfluss=densitywertminimum;
					}
					pTree->densitywert= pTree->densitywert*ageseinfluss;
				}
				else if (parameter[0].dichtheightrel==13) // Hinzugefügt am 13.11. um die Größenklassenverteilung besser zu fitten
				{
					// densitywert wird kleiner je großer der Tree ist
					// ... linear
					double densitywertminimum=0.5;
					double ageseinfluss=( -1*(1-densitywertminimum)/50.0* (double) pTree->age + 1.0 );
					if(ageseinfluss<densitywertminimum)
					{
						ageseinfluss=densitywertminimum;
					}
					pTree->densitywert= pTree->densitywert*ageseinfluss;
				}
				else if (parameter[0].dichtheightrel==20) // Hinzugefügt am 13.11. um die Größenklassenverteilung besser zu fitten
				{
					// densitywert wird kleiner je großer der Tree ist
					double densitywertminimum=0.1;
					double ageseinfluss=(1.0/pow(( (double) pTree->age +1.0), 0.5));
					if(ageseinfluss<densitywertminimum)
					{
						ageseinfluss=densitywertminimum;
					}
					pTree->densitywert= pTree->densitywert*ageseinfluss;

				}
				else if (parameter[0].dichtheightrel==21) // Hinzugefügt am 13.11. um die Größenklassenverteilung besser zu fitten
				{
					// densitywert wird kleiner je großer der Tree ist
					double densitywertminimum=0.5;
					double ageseinfluss=(1.0/pow(( (double) pTree->age +1.0), 0.15));
					if(ageseinfluss<densitywertminimum)
					{
						ageseinfluss=densitywertminimum;
					}
					pTree->densitywert= pTree->densitywert*ageseinfluss;

				}
				else if (parameter[0].dichtheightrel==22) // Hinzugefügt am 13.11. um die Größenklassenverteilung besser zu fitten
				{
					// densitywert wird kleiner je großer der Tree ist
					double densitywertminimum=0.1;
					double ageseinfluss=(1.0/pow(( (double) pTree->age +1.0), 0.6));
					if(ageseinfluss<densitywertminimum)
					{
						ageseinfluss=densitywertminimum;
					}
					pTree->densitywert= pTree->densitywert*ageseinfluss;

				}
				else if (parameter[0].dichtheightrel==23) // Hinzugefügt am 13.11. um die Größenklassenverteilung besser zu fitten
				{
					// densitywert wird kleiner je großer der Tree ist
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

				// auf einstellbaren Maximalwert setzen falls sehr grosz
				if (pTree->densitywert>parameter[0].desitymaxreduction) 
					pTree->densitywert=parameter[0].desitymaxreduction;
					
			} 
		}//END: loop over each tree
	}// only one core
	
	if(parameter[0].omp_num_threads>1)
	{// more than one core
	// test simply parallelize
		omp_set_dynamic(0); //disable dynamic teams
		// omp_set_num_threads(parameter[0].omp_num_threads); //set the number of helpers
		omp_set_num_threads(parameter[0].omp_num_threads); //set the number of helpers
			
		#pragma omp parallel private(pTree)
		{
			// initialize the info for each of the thread
			int thread_count = omp_get_num_threads();
			int thread_num   = omp_get_thread_num();
			size_t chunk_size= tree_list.size() / thread_count;
			auto begin = tree_list.begin();
			std::advance(begin, thread_num * chunk_size);
			auto end = begin;
			
			if(thread_num == (thread_count - 1)) // last thread iterates the remaining sequence
			{
				end = tree_list.end();
				// cout << thread_num << " -> thread_num == (thread_count - 1)" << endl;
			} else
			{
				std::advance(end, chunk_size);
				// cout << thread_num << " -> thread_num != (thread_count - 1)" << endl;
			}
			
			// wait for all threads to initialize and then proceed
			#pragma omp barrier
			
			// for (list<Tree*>::iterator pos = tree_list.begin(); pos != tree_list.end(); ++pos)
			for(auto it = begin; it != end; ++it)
			{// START: loop over each tree
				pTree=(*it);

				///Get tree position
				// 				ycoo = exact position
				//				ycoo* sizemagnif = cell number
				int i = (int) floor(pTree->ycoo * parameter[0].sizemagnif);
				int j = (int) floor(pTree->xcoo * parameter[0].sizemagnif);



				//DENSITY 1
				if (parameter[0].densityart == 1)
				{
					pTree->densitywert = 0.0;
				}
				else 
				{
					//double density_help = pTree->densitywert;
					
					// Raster um Tree in Abh. von Dbasal ablaufen
					// und in Abh. von Entfernung der Flaecheneinheit einen densitywert eintragen
					// Umrechnung des Radius in Meter

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

					//wenn der Tree nur eine Zelle beeinflusst
					if ( flaechengroesze < (1.0/parameter[0].sizemagnif) )
					{
						// DENSITY 2
						if (parameter[0].densityart == 2)
						{
							if (plot_list[i*treecols*parameter[0].sizemagnif+j]->Treedensitywert > 0.0) 
							{
								if (parameter[0].densitytiletree==1)	// Summe der Werte
								{
									pTree->densitywert =	(1.0 - (pTree->densitywert / plot_list[i*treecols*parameter[0].sizemagnif+j]->Treedensitywert));
									//                           density_soll(an position) / density_ist(an position)
								}
								else if (parameter[0].densitytiletree==2)	// Multiplikation der Werte
								{
									pTree->densitywert =	(1.0 - (pTree->densitywert
																/ 
																(plot_list[i*treecols*parameter[0].sizemagnif+j]->Treedensitywert*pTree->densitywert) 
																)
														);
								//                         density_soll(an position) / density_ist(an position)
								}
							}
							
							else 
								pTree->densitywert=0.0; // keine Konkurrenz
						}
						


						// DENSITY 3
						else if (parameter[0].densityart == 3)
						{
							
							// Ziehe zufaellige Position im Gitter und weise den Wert zu
							int izuf= (int) floor( 0.0 + ( (double)  ( ((double) (treerows-1)) * parameter[0].sizemagnif * rand() / (RAND_MAX + 1.0))) );
							int jzuf= (int) floor( 0.0 + ( (double)  ( ((double) (treecols-1)) * parameter[0].sizemagnif * rand() / (RAND_MAX + 1.0))) );
							
							if (plot_list[izuf*treecols*parameter[0].sizemagnif+jzuf]->Treedensitywert > 0.0) 
							{
								if (parameter[0].densitytiletree==1)	//StefanC: Summe der Werte
								{
									pTree->densitywert = (1.0 - (pTree->densitywert/ plot_list[izuf*treecols*parameter[0].sizemagnif+jzuf]->Treedensitywert));
									//                           density_soll(an position) / density_ist(an position)
								}
								else if (parameter[0].densitytiletree==2)	// Multiplikation der Werte
								{
									pTree->densitywert =	(1.0 - (pTree->densitywert
																/ 
																(plot_list[i*treecols*parameter[0].sizemagnif+j]->Treedensitywert*pTree->densitywert) 
																)
														);
								}
							}
							else 
								pTree->densitywert = 0.0;
						}
						
						
						// Berechne den Einfluss der thawing_depth auf das Wachstum des Treees
						if ((plot_list[i*treecols*parameter[0].sizemagnif+j]->maxthawing_depth<2000) && (parameter[0].thawing_depth==true && parameter[0].einschwingen==false)) 
							pTree->thawing_depthinfluence= (unsigned short) ((200.0/2000.0)* (double) plot_list[i*treecols*parameter[0].sizemagnif+j]->maxthawing_depth);
						
						else
							pTree->thawing_depthinfluence=100;
					}
					// ... wenn der Tree mehrere Zellen beeinflusst
					else
					{
						// Ausmasze des abzulaufenden Rasters um den Tree bestimmen
						int xyquerrastpos= (int) floor( flaechengroesze*parameter[0].sizemagnif );
						double maxdist= sqrt(pow(double(xyquerrastpos),2)+pow(double(xyquerrastpos),2));

						// Versetzte Koordinaten ermitteln und Mitsummieren des densitywertes
						double sumdensitywert=0;
						double sumthawing_depth=0;
						
						unsigned int anzahlflaechen=0;
						
						for (int rastposi=(i+xyquerrastpos); rastposi>(i-(xyquerrastpos+1)); rastposi--)
						{
							for (int rastposj=(j-xyquerrastpos); rastposj<(j+xyquerrastpos+1); rastposj++)
							{
								if ((rastposi<=( (treerows-1)*parameter[0].sizemagnif) && rastposi>=0) && ( rastposj<=( (treecols-1)*parameter[0].sizemagnif) && rastposj>=0))
								{
									// Entfernungsberechnung um Einfluss des densitywertes in Raumeinheit zu bestimmen ...
									// ... wie beim Eintragen
									double entfrastpos=sqrt(pow(double(i-rastposi),2)+pow(double(j-rastposj),2));
									
									// Nur wenn das aktuelle Kachel in den Einflussbereich faellt, wird ein Wert eingeschrieben
									if (entfrastpos<= (double) xyquerrastpos)
									{
										int icurr=rastposi;
										int jcurr=rastposj;

										// Change of i/j vals only if DENSITY 3
										if (parameter[0].densityart==3)
										{
											// Zufaellig ermitteln der Position von der die densitywerte genommen werden
											icurr= (int) floor( 0.0 + ( (double)  ( ((double) (treerows-1)) *parameter[0].sizemagnif*rand()/(RAND_MAX + 1.0))) );
											jcurr= (int) floor( 0.0 + ( (double)  ( ((double) (treecols-1)) *parameter[0].sizemagnif*rand()/(RAND_MAX + 1.0))) );
										}
										
										if (parameter[0].densitytiletree==1)	// Summe der Werte
										{
											sumdensitywert+=plot_list[icurr*treecols*parameter[0].sizemagnif+jcurr]->Treedensitywert
															* (1-entfrastpos/maxdist);	// Hinzugefuegt damit die Werte an der Peripherie schwaecher wirken, sonst wuerde es zu sehr groszen unrealistischen Werten fuehren
										}
										else if (parameter[0].densitytiletree==2)	// Multiplikation der Werte
										{
											// Nach der Wichtung der zusaetzlichen Werte anhand der individuellen Einflusswerte wird das Grundniveau dazuaddiert
											sumdensitywert+=(plot_list[icurr*treecols*parameter[0].sizemagnif+jcurr]->Treedensitywert
															-pow(pTree->dbasal, parameter[0].densitytreetile)/(entfrastpos+1.0)/*Treeeinflusswert an der akt Position*/ )
															* pow(pTree->dbasal, parameter[0].densitytreetile)/(entfrastpos+1.0)/*Treeeinflusswert an der akt Position*/
															
															+ pow(pTree->dbasal, parameter[0].densitytreetile)/(entfrastpos+1.0)/*Treeeinflusswert an der akt Position*/;
										}

										sumthawing_depth+=(double) plot_list[rastposi*treecols*parameter[0].sizemagnif+rastposj]->maxthawing_depth;
										anzahlflaechen++;
										
									}

								}

							}

						}


						if (sumdensitywert > 0.0) 
							pTree->densitywert = 1.0 - (pTree->densitywert/sumdensitywert);
														//density_soll / density_ist
						else 
							pTree->densitywert=0.0;

							
						sumthawing_depth=( sumthawing_depth / (double) anzahlflaechen );
						
						if (sumthawing_depth  <2000)
							pTree->thawing_depthinfluence = (unsigned short) ((200.0/2000.0) * sumthawing_depth);
						
						else
							pTree->thawing_depthinfluence = 100;

					}


					pTree->densitywert= pTree->densitywert
											  *pow((1.0-(0.01/pTree->dbasal)),parameter[0].densitywertdbasaleinfluss);	// Optional: Je groeszer der Tree, desto heightr der Einfluss
					
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
					else if (parameter[0].dichtheightrel==10) // Hinzugefügt am 13.11. um die Größenklassenverteilung besser zu fitten
					{
						// densitywert wird kleiner je großer der Tree ist
						// ... linear
						double densitywertminimum=0.1;
						double ageseinfluss=( -1*(1-densitywertminimum)/100.0* (double) pTree->age + 1.0 );
						if(ageseinfluss<densitywertminimum)
						{
							ageseinfluss=densitywertminimum;
						}
						pTree->densitywert= pTree->densitywert*ageseinfluss;
					}
					else if (parameter[0].dichtheightrel==11) // Hinzugefügt am 13.11. um die Größenklassenverteilung besser zu fitten
					{
						// densitywert wird kleiner je großer der Tree ist
						// ... linear
						double densitywertminimum=0.5;
						double ageseinfluss=( -1*(1-densitywertminimum)/100.0* (double) pTree->age + 1.0 );
						if(ageseinfluss<densitywertminimum)
						{
							ageseinfluss=densitywertminimum;
						}
						pTree->densitywert= pTree->densitywert*ageseinfluss;
					}
					else if (parameter[0].dichtheightrel==12) // Hinzugefügt am 13.11. um die Größenklassenverteilung besser zu fitten
					{
						// densitywert wird kleiner je großer der Tree ist
						// ... linear
						double densitywertminimum=0.1;
						double ageseinfluss=( -1*(1-densitywertminimum)/50.0* (double) pTree->age + 1.0 );
						if(ageseinfluss<densitywertminimum)
						{
							ageseinfluss=densitywertminimum;
						}
						pTree->densitywert= pTree->densitywert*ageseinfluss;
					}
					else if (parameter[0].dichtheightrel==13) // Hinzugefügt am 13.11. um die Größenklassenverteilung besser zu fitten
					{
						// densitywert wird kleiner je großer der Tree ist
						// ... linear
						double densitywertminimum=0.5;
						double ageseinfluss=( -1*(1-densitywertminimum)/50.0* (double) pTree->age + 1.0 );
						if(ageseinfluss<densitywertminimum)
						{
							ageseinfluss=densitywertminimum;
						}
						pTree->densitywert= pTree->densitywert*ageseinfluss;
					}
					else if (parameter[0].dichtheightrel==20) // Hinzugefügt am 13.11. um die Größenklassenverteilung besser zu fitten
					{
						// densitywert wird kleiner je großer der Tree ist
						double densitywertminimum=0.1;
						double ageseinfluss=(1.0/pow(( (double) pTree->age +1.0), 0.5));
						if(ageseinfluss<densitywertminimum)
						{
							ageseinfluss=densitywertminimum;
						}
						pTree->densitywert= pTree->densitywert*ageseinfluss;

					}
					else if (parameter[0].dichtheightrel==21) // Hinzugefügt am 13.11. um die Größenklassenverteilung besser zu fitten
					{
						// densitywert wird kleiner je großer der Tree ist
						double densitywertminimum=0.5;
						double ageseinfluss=(1.0/pow(( (double) pTree->age +1.0), 0.15));
						if(ageseinfluss<densitywertminimum)
						{
							ageseinfluss=densitywertminimum;
						}
						pTree->densitywert= pTree->densitywert*ageseinfluss;

					}
					else if (parameter[0].dichtheightrel==22) // Hinzugefügt am 13.11. um die Größenklassenverteilung besser zu fitten
					{
						// densitywert wird kleiner je großer der Tree ist
						double densitywertminimum=0.1;
						double ageseinfluss=(1.0/pow(( (double) pTree->age +1.0), 0.6));
						if(ageseinfluss<densitywertminimum)
						{
							ageseinfluss=densitywertminimum;
						}
						pTree->densitywert= pTree->densitywert*ageseinfluss;

					}
					else if (parameter[0].dichtheightrel==23) // Hinzugefügt am 13.11. um die Größenklassenverteilung besser zu fitten
					{
						// densitywert wird kleiner je großer der Tree ist
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

					// auf einstellbaren Maximalwert setzen falls sehr grosz
					if (pTree->densitywert>parameter[0].desitymaxreduction) 
						pTree->densitywert=parameter[0].desitymaxreduction;
						
				}
			}//END: loop over each tree
		}//END: pragma
	}// more than one core
//cout << endl;

}




/****************************************************************************************//**
 * \brief reset Maps "Karten"
 *
 * reset Treedensity und Treeanzahl \n
 * calculate vegetation density (auflagenstärke) if vegetation==true \n
 * calculate max active layer depth
 *
 *
 *******************************************************************************************/

void ResetMaps(int yearposition, vector<Karten*> &plot_list, vector<weather*> &weather_list)
{
	if(parameter[0].omp_num_threads==1)
	{// only one core
		for (int kartenpos=0; kartenpos< (treerows*parameter[0].sizemagnif*treecols*parameter[0].sizemagnif); kartenpos++)
		{
			pKarten=plot_list[kartenpos];
			pKarten->Treedensitywert=0;
			pKarten->Treeanzahl=0;
			// pKarten->Dbasalliste.clear();
			
			if (parameter[0].vegetation==true && parameter[0].einschwingen==false)
			{
				double auflagenwachstumsrate =0.05
											 +( 1.0/( ((1.0/0.01)-(1.0/0.95))
													  *exp(-(1.0/200.0)*(double) pKarten->maxthawing_depth) 
													  +(1/0.95)) ); // Logistisches Wachstum Kapazitaet=0.95; N0=0.01; r=(1/200); Verschiebung auf mind. 0.05 bis 1.0
				
				pKarten->auflagenstaerke+= (unsigned short) (auflagenwachstumsrate*60.0);	// in 0.1mm Schritten; 6mm Zuwachs jaehrlich aus 30 cm Aufwachs in 50 jahren (Aus Literatur)

				pKarten->auflagenstaerkemittel = (unsigned short) ( (double) 
												 (pKarten->auflagenstaerke8
												 +pKarten->auflagenstaerke7
												 +pKarten->auflagenstaerke6
												 +pKarten->auflagenstaerke5
												 +pKarten->auflagenstaerke4
												 +pKarten->auflagenstaerke3
												 +pKarten->auflagenstaerke2
												 +pKarten->auflagenstaerke1
												 +pKarten->auflagenstaerke0
												 +pKarten->auflagenstaerke)
												 /10.0); //thawing_depth reagiert mit 10-jahres lag auf änderungen der auflagenstärke, daher 10-jahres mittel der zuwächse

				pKarten->auflagenstaerke8 = pKarten->auflagenstaerke7;
				pKarten->auflagenstaerke7 = pKarten->auflagenstaerke6;
				pKarten->auflagenstaerke6 = pKarten->auflagenstaerke5;
				pKarten->auflagenstaerke5 = pKarten->auflagenstaerke4;
				pKarten->auflagenstaerke4 = pKarten->auflagenstaerke3;
				pKarten->auflagenstaerke3 = pKarten->auflagenstaerke2;
				pKarten->auflagenstaerke2 = pKarten->auflagenstaerke1;
				pKarten->auflagenstaerke1 = pKarten->auflagenstaerke0;
				pKarten->auflagenstaerke0 = pKarten->auflagenstaerke;
			}
			

			if (parameter[0].thawing_depth==true && parameter[0].einschwingen==false)
			{
				// Berechnung der Daempfung die durch die organische Auflage entsteht (Dämpfung vermindert thawing_depth, Formel aus Lit)
				double daempfung = (1.0/4000.0) * (double) pKarten->auflagenstaerkemittel; // 1/4000 ist Steigung um bei etwa 4000 den Maximalwert zu erreichen
				
				if (daempfung>=0.9) 
					daempfung=0.9;
				
				// Berechnung der SAL
				pKarten->maxthawing_depth= (unsigned short) ( 1000.0*(1.0-daempfung)*0.050*sqrt(weather_list[yearposition]->degreday));	// 1000 (scaling from m to mm)*edaphicfactor=0.050 (SD=0.019)
			}
		} // Kartenschleife Ende
	}// only one core

	if(parameter[0].omp_num_threads>1)
	{// more than one core
		omp_set_dynamic(1); //disable dynamic teams
		// omp_set_num_threads(parameter[0].omp_num_threads); //set the number of helpers
		omp_set_num_threads(parameter[0].omp_num_threads); //set the number of helpers
		
		#pragma omp parallel for private(pKarten)
		for(int kartenpos=0; kartenpos< (treerows*parameter[0].sizemagnif*treecols*parameter[0].sizemagnif); kartenpos++)
		{
			pKarten=plot_list[kartenpos];
			pKarten->Treedensitywert=0;
			pKarten->Treeanzahl=0;
			// pKarten->Dbasalliste.clear();
			
			if (parameter[0].vegetation==true && parameter[0].einschwingen==false)
			{
				double auflagenwachstumsrate =0.05
											 +( 1.0/( ((1.0/0.01)-(1.0/0.95))
													  *exp(-(1.0/200.0)*(double) pKarten->maxthawing_depth) 
													  +(1/0.95)) ); // Logistisches Wachstum Kapazitaet=0.95; N0=0.01; r=(1/200); Verschiebung auf mind. 0.05 bis 1.0
				
				pKarten->auflagenstaerke+= (unsigned short) (auflagenwachstumsrate*60.0);	// in 0.1mm Schritten; 6mm Zuwachs jaehrlich aus 30 cm Aufwachs in 50 jahren (Aus Literatur)

				pKarten->auflagenstaerkemittel = (unsigned short) ( (double) 
												 (pKarten->auflagenstaerke8
												 +pKarten->auflagenstaerke7
												 +pKarten->auflagenstaerke6
												 +pKarten->auflagenstaerke5
												 +pKarten->auflagenstaerke4
												 +pKarten->auflagenstaerke3
												 +pKarten->auflagenstaerke2
												 +pKarten->auflagenstaerke1
												 +pKarten->auflagenstaerke0
												 +pKarten->auflagenstaerke)
												 /10.0); //thawing_depth reagiert mit 10-jahres lag auf änderungen der auflagenstärke, daher 10-jahres mittel der zuwächse

				pKarten->auflagenstaerke8 = pKarten->auflagenstaerke7;
				pKarten->auflagenstaerke7 = pKarten->auflagenstaerke6;
				pKarten->auflagenstaerke6 = pKarten->auflagenstaerke5;
				pKarten->auflagenstaerke5 = pKarten->auflagenstaerke4;
				pKarten->auflagenstaerke4 = pKarten->auflagenstaerke3;
				pKarten->auflagenstaerke3 = pKarten->auflagenstaerke2;
				pKarten->auflagenstaerke2 = pKarten->auflagenstaerke1;
				pKarten->auflagenstaerke1 = pKarten->auflagenstaerke0;
				pKarten->auflagenstaerke0 = pKarten->auflagenstaerke;
			}
			

			if (parameter[0].thawing_depth==true && parameter[0].einschwingen==false)
			{
				// Berechnung der Daempfung die durch die organische Auflage entsteht (Dämpfung vermindert thawing_depth, Formel aus Lit)
				double daempfung = (1.0/4000.0) * (double) pKarten->auflagenstaerkemittel; // 1/4000 ist Steigung um bei etwa 4000 den Maximalwert zu erreichen
				
				if (daempfung>=0.9) 
					daempfung=0.9;
				
				// Berechnung der SAL
				pKarten->maxthawing_depth= (unsigned short) ( 1000.0*(1.0-daempfung)*0.050*sqrt(weather_list[yearposition]->degreday));	// 1000 (scaling from m to mm)*edaphicfactor=0.050 (SD=0.019)
			}
		} // Kartenschleife Ende

	}// more than one core
	
	
	
}






/****************************************************************************************//**
 * \brief update density maps and active layer depth
 *
 * Karten-Element ändert exakte Position in Gridzellen um
 * 
 *
 *
 *******************************************************************************************/
void Kartenupdate(int treerows, int treecols, struct Parameter *parameter, int yearposition, vector<vector<Karten*> > &world_plot_list, vector<list<Tree*> > &world_tree_list, vector<vector<weather*> > &world_weather_list)
{

	int aktort=0;
	
	for (vector<vector<Karten*> >::iterator posw = world_plot_list.begin(); posw != world_plot_list.end(); ++posw)
	{ // Weltschleife Beginn
		vector<Karten*>& plot_list = *posw;

		vector<list<Tree*> >::iterator world_positon_b = (world_tree_list.begin()+aktort);
		list<Tree*>& tree_list = *world_positon_b;

		vector<vector<weather*> >::iterator posiwelt = (world_weather_list.begin()+aktort);
		vector<weather*>& weather_list = *posiwelt;

		aktort++;

		double time_start_0=omp_get_wtime();
				
		// Kartenelemente Reset und Update
		/*!ResetMaps(int yearposition, vector<Karten*>& plot_list, vector<weather*>& weather_list)*/
		ResetMaps(yearposition, plot_list, weather_list);

		double time_ResetMaps=omp_get_wtime();

		
		// Treedensity eintragen
		/*!AddTreeDensity(list<Tree*>& tree_list, vector<Karten*>& plot_list)*/
		AddTreeDensity(tree_list, plot_list);
		
		double time_AddTreeDensity=omp_get_wtime();


		// Weitere Verarbeitung der densitywerte
		/*!IndividualTreeDensity(list<Tree*>& tree_list, vector<Karten*>& plot_list)*/
		IndividualTreeDensity(tree_list, plot_list);
		
		cout << endl << " TIME: ResetMaps=" << time_ResetMaps-time_start_0 << endl;
		cout << endl << " TIME: AddTreeDensity=" << time_AddTreeDensity-time_ResetMaps << endl;
		cout << endl << " TIME: IndividualTreeDensity=" << omp_get_wtime()-time_AddTreeDensity << endl;

	} // Weltschleife Ende

}

