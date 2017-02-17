/****************************************************************************************//**
 * \brief Establish new seedlings from seeds
 *
 *
 * 
 * 
 * 
 *******************************************************************************************/
void Etablierung(int baeumerows, int baeumecols, struct Parameter *parameter, int jahrposition, vector<list<Baum*> > &WeltBaumListe, vector<list<Samen*> > & WeltSamenListe, vector<vector<Wetter*> > &WeltWetterListe, vector<vector<Karten*> > &WeltKartenListe)
{
	int aktort=0;
	
	for (vector<vector<Wetter*> >::iterator posw = WeltWetterListe.begin(); posw != WeltWetterListe.end(); ++posw)
	{

		vector<Wetter*>& WetterListe = *posw;

		vector<list<Baum*> >::iterator posiweltb = (WeltBaumListe.begin()+aktort);
		list<Baum*>& BaumListe = *posiweltb;
	
		vector<list<Samen*> >::iterator posiwelts = (WeltSamenListe.begin()+aktort);
		list<Samen*>& SamenListe = *posiwelts;

		vector<vector<Karten*> >::iterator posweltk = (WeltKartenListe.begin()+aktort);
		vector<Karten*>& KartenListe = *posweltk;

		aktort++;
		
			// go through seed list
			for (list<Samen*>::iterator pos = SamenListe.begin(); pos != SamenListe.end(); )
			{ 

				pSamen=(*pos);

				// if seed is at the forest floor
				if (pSamen->imzapfen==false)
				{ 
				
					// determine if a seed can calculate depending on its location ...
					int i=(int) floor(pSamen->ycoo*parameter[0].sizemagnif);
					int j=(int) floor(pSamen->xcoo*parameter[0].sizemagnif);
					
					// ... and climate
					double maxbasalwachstum;
					
					if (parameter[0].lineartransekt==true)
					{	
						maxbasalwachstum=((WetterListe[jahrposition]->maxbasalwachstumjahr-WetterListe[jahrposition]->maxbasalwachstumjahrmin)/baeumerows)*pSamen->ycoo + WetterListe[jahrposition]->maxbasalwachstumjahrmin;
					}
					else
					{
						maxbasalwachstum=WetterListe[jahrposition]->maxbasalwachstumjahr;
					}	

					// add new tree if seed germinates
					double zufallsz= 0.0 +( (double) 1.0*rand()/(RAND_MAX + 1.0));
					if (zufallsz< (parameter[0].keimungsrate+(parameter[0].keimungwettereinfluss*maxbasalwachstum/parameter[0].gdbasal)) )
					{  

						if (maxbasalwachstum>0.0) 
						{
							
							pBaum= new Baum();
							pBaum->yweltcoo=pSamen->yweltcoo;
							pBaum->xweltcoo=pSamen->xweltcoo;
							pBaum->xcoo=pSamen->xcoo;
							pBaum->ycoo=pSamen->ycoo;
							pBaum->name=++parameter[0].nameakt;
							pBaum->namem=pSamen->namem;
							pBaum->namep=pSamen->namep;
							pBaum->etablierungsjahr=jahrposition;
							pBaum->linie=pSamen->linie;
							pBaum->generation=pSamen->generation;
							pBaum->dbasal=maxbasalwachstum;
							pBaum->dbasalrel=1.0;
							pBaum->dbrust=0.0;
							pBaum->dbrustrel=1.0;
							
							if (parameter[0].allometriefunktionstyp==1) 
							{
								pBaum->hoehe= parameter[0].dbasalhoeheallosteig * pow(maxbasalwachstum, parameter[0].dbasalhoehealloexp);
							}
							else 
							{
								pBaum->hoehe= parameter[0].dbasalhoehesteignonlin*maxbasalwachstum;	
							}
							
							pBaum->alter=0; 
							pBaum->zapfen=0; 
							pBaum->zapfenhoehe=99999.0;
							pBaum->samenneuproduziert=0; 
							pBaum->samenproduziert=0; 
							pBaum->dichtewert=0;
							pBaum->growing=true;
							BaumListe.push_back(pBaum);

							delete pSamen;
							pos=SamenListe.erase(pos);
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

				}
				// ... or if the seed is still in the cone do not let it germinate
				else if (pSamen->imzapfen==true)
				{
					++pos;
				}
				// ... or security check
				else
				{
					printf("LAVESI exited\n\n");
					printf("... in etablierung.cpp\n");
					printf("... cause pSamen->imzapfen is not set of a seed\n");
					exit(1);
				} 

			} 

	} 

}

