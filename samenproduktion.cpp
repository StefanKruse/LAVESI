/****************************************************************************************//**
* \brief Calculate seed production
*
* depends on seed production factor, tree size, actual growth
*
*
*******************************************************************************************/
void Samenproduktion(int baeumerows, int baeumecols, struct Parameter *parameter, vector<list<Baum*> > &WeltBaumListe, vector<list<Samen*> > &WeltSamenListe, vector<vector<Wetter*> > &WeltWetterListe,int jahrposition)
{
	int aktort=0;
	
	for (vector<list<Baum*> >::iterator posw = WeltBaumListe.begin(); posw != WeltBaumListe.end(); ++posw)
	{
		list<Baum*>& BaumListe = *posw;

		vector<list<Samen*> >::iterator posiwelt = (WeltSamenListe.begin()+aktort);
		list<Samen*>& SamenListe = *posiwelt;

		aktort++;
 
		int aktortyweltcoo=(int) floor( (double) (aktort-1)/parameter[0].mapxlength );
		int aktortxweltcoo=(aktort-1) - (aktortyweltcoo * parameter[0].mapxlength);

		for (list<Baum*>::iterator pos = BaumListe.begin(); pos != BaumListe.end(); )
		{
			pBaum=(*pos);

			// only maturized trees can produce seeds
			if (pBaum->zapfen==1)
			{
				// estimate the number of produced seeds in the current year
				pBaum->samenneuproduziert=(int) floor( parameter[0].samenprodfaktor
												* pBaum->hoehe/100	
												* pBaum->dbasalrel
												* (1.0-(1.0/(pBaum->hoehe/50))) );

				if (pBaum->samenneuproduziert>0)
				{
					for (int sn=0; sn<pBaum->samenneuproduziert; sn++)
					{
						pSamen= new Samen();
						pSamen->yweltcoo=aktortyweltcoo;
						pSamen->xweltcoo=aktortxweltcoo;
						pSamen->xcoo=pBaum->xcoo;
						pSamen->ycoo=pBaum->ycoo;
						pSamen->namem=pBaum->name;
						pSamen->namep=pBaum->name;
						pSamen->linie=pBaum->linie;
						pSamen->generation=pBaum->generation+1;
						pSamen->imzapfen=true;
						pSamen->gewicht=1;
						pSamen->alter=0;
						SamenListe.push_back(pSamen);
					}
					++pos;
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
	}
}
