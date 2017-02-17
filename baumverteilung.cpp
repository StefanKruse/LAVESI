/****************************************************************************************//**
 * \brief Seed input in the beginnig of a simulation
 *
 * if seedinput==true, start simulation with seed input \n
 * recall if sites are empty of trees 
 *
 *******************************************************************************************/
void Seedin()
{
	int aktort=0;
	for (vector<list<Samen*> >::iterator posw = WeltSamenListe.begin(); posw != WeltSamenListe.end(); ++posw)
	{
		list<Samen*>& SamenListe = *posw;

		aktort++;
		
		int aktortyweltcoo=(int) floor( (double) (aktort-1)/parameter[0].mapxlength );
		int aktortxweltcoo=(aktort-1) - (aktortyweltcoo * parameter[0].mapxlength);

		// declaration of local variables
		bool seedinput;

		// seedinput on all sites
		if (parameter[0].realseedconnect==false)
		{
			seedinput=true;
		}
		// ... or seedinput on southern site only
		else if (parameter[0].realseedconnect==true && aktortyweltcoo==(parameter[0].mapylength-1))
		{
			seedinput=true;
		}
		// ... or no seedinput
		else
		{
			seedinput=false;
		}

		// start seedinput
		if (seedinput==true)
		{
			// set the number of seeds that should be introduced into the plot
			int seednobuffer; 
			if (parameter[0].jahremitsameneintrag<=0)
			{
				seednobuffer=parameter[0].etabbgpermanent;
			}
			else
			{
				seednobuffer=parameter[0].etabbg;
			}
			
			// introduce seeds
			for (int n=0;n<seednobuffer;n++)
			{ 
				// calculate the position for each seed
				double jsamen, isamen;
				bool sameneintragen=false;

				// estimate the location depending on specified modes 
				// ... either seeddispmode==1: negative exponentially from the south border of each plot
				if (parameter[0].seeddispmode==1)
				{ 
					
					// draw random position for x-value
					jsamen= 0.0 + ( (double)  ( ((double) (baeumecols-1)) *rand()/(RAND_MAX + 1.0)));
					
					// estimate the distance from the south border y-value
					double entfernung;
			
					do
					{
						double anteil;
				
						do
						{
							anteil= 0.0 +( (double) 1.0*rand()/(RAND_MAX + 1.0));
						} while (anteil<=0.0);

						entfernung= (log(anteil)/(-0.2))/parameter[0].entfernungsteiler;

					} while ( (entfernung>= (double) (baeumerows-1)) | (entfernung<0.0) );

					sameneintragen=true;
					isamen=entfernung;
				} 
				// ... or seeddispmode==2: randomly into the plot
				else if (parameter[0].seeddispmode==2)
				{ 
					// x-value
					jsamen= 0.0 + ( (double)  ( ((double) (baeumecols-1)) *rand()/(RAND_MAX + 1.0)));
					
					// y-value
					isamen= 0.0 + ( (double)  ( ((double) (baeumerows-1)) *rand()/(RAND_MAX + 1.0)));
				
					sameneintragen=true;
				} 
				// ... security check
				else 
				{
					printf("\n\nLAVESI exited\n");
					printf("...baumverteilung.cpp\n");
					printf("...cause: no valid seed dispersal mode was specified!");
					exit(1);
				}
				
				// add new seeds into the list of seeds
				if (sameneintragen==true)
				{ 
					pSamen= new Samen();
					pSamen->yweltcoo=aktortyweltcoo;
					pSamen->xweltcoo=aktortxweltcoo;
					pSamen->xcoo=jsamen;
					pSamen->ycoo=isamen;
					pSamen->namem=0;
					pSamen->namep=0;
					pSamen->linie=++parameter[0].linieakt;
					pSamen->generation=0;
					pSamen->imzapfen=false;
					pSamen->gewicht=1;
					pSamen->alter=0;
					pSamen->longdispersed=false;
					SamenListe.push_back(pSamen);
						
					if ( (pSamen->yweltcoo<0.0) | (pSamen->yweltcoo> (double) (baeumerows-1)) | (pSamen->xcoo<0.0) | (pSamen->xcoo> (double) (baeumecols-1)) )
					{
						printf("\n\nLAVESI exited\n");
						printf("...baumverteilung.cpp\n");
						printf("...cause: introduced seed was placed out of valid plot coordinates (that are Pos(Y=%4.2f,X=%4.2f))\n", isamen, jsamen);
						exit(1);
					}
				
				} 

			} 	
		
		}
	
	} 

}









/****************************************************************************************//**
 * \brief Start with empty site and introduce seeds
 *
 * 
 *
 *
 *******************************************************************************************/
void Baumverteilung(int baeumerows, int baeumecols, struct Parameter *parameter, int wortlaengemax, int jahrposition, vector<list<Baum*> > &WeltBaumListe, vector<list<Samen*> > &WeltSamenListe)
{
	// if seeds should be introduced cast the seed introduction function
	if (parameter[0].starter==true)
	{
		Seedin();
	}

}


