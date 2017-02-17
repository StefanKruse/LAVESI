/****************************************************************************************//**
 * \brief Process stand data and write data to files
 *
 * 
 * 
 * 
 * 
 *******************************************************************************************/
void Datenausgabe(int baeumerows, int baeumecols, int t, int jahr, struct Parameter *parameter, int jahrposition, vector<list<Baum*> > &WeltBaumListe, vector<list<Samen*> > &WeltSamenListe, vector<vector<Wetter*> > &WeltWetterListe, vector<vector<Karten*> > &WeltKartenListe, vector<vector<Auswertung*> > &WeltAuswertungsListe)
{

	FILE *dateizeiger;
	string dateiname;
	
	// declaration of the plot area that is used for output which is actually the plot centre of 20 m x 20 m
	double xminfenster=( (double) baeumecols/2.0)-10.0;
	double xmaxfenster=( (double) baeumecols/2.0)+10.0;
	double yminfenster=( (double) baeumerows/2.0)-10.0;
	double ymaxfenster=( (double) baeumerows/2.0)+10.0;

	// declaration of output functions
	bool ausgabecurrencies=false;
	bool ausgabeposition=false;
	bool ausgabeindividuen=false;
	bool ausgabedichte=false;

	// process data at every simulated location (aktort) and sample data for export to files
	int aktort=0;
	for (vector<list<Baum*> >::iterator posw = WeltBaumListe.begin(); posw != WeltBaumListe.end(); ++posw)
	{
		list<Baum*>& BaumListe = *posw;
		
		vector<list<Samen*> >::iterator posiwelts = (WeltSamenListe.begin()+aktort);
		list<Samen*>& SamenListe = *posiwelts;
		
		vector<vector<Karten*> >::iterator poskarten = (WeltKartenListe.begin()+aktort);
		vector<Karten*>& KartenListe = *poskarten;
		
		vector<vector<Wetter*> >::iterator posiwelt = (WeltWetterListe.begin()+aktort);
		vector<Wetter*>& WetterListe = *posiwelt;
		
		vector<vector<Auswertung*> >::iterator posiweltausw = (WeltAuswertungsListe.begin()+aktort);
		vector<Auswertung*>& AuswertungsListe = *posiweltausw;
		vector<Auswertung*>::iterator posausw = AuswertungsListe.begin();
		pAuswertung=(*posausw);
		
		aktort++;
		
		// calculate current location if more than one YPLOT-plots are simulated
		int aktortyweltcoo=(int) floor( (double) (aktort-1)/parameter[0].mapxlength );
		int aktortxweltcoo=(aktort-1) - (aktortyweltcoo * parameter[0].mapxlength);

		
		// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
		// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
		// - START update lists for runtime output AuswertungsListen - //

		// declarations
		double basalarea=0.0;
		int nhoehe0b40=0, nhoehe41b200=0, nhoehe201b10000=0;
		double breastdiameter=0.0;
		int breastdiametercount=0;
		int stemcount=0;
		double meantreeheight=0.0, meantreeage=0.0;

		// go through list with all trees BaumListe
		for (list<Baum*>::iterator pos = BaumListe.begin(); pos != BaumListe.end(); )
		{
			pBaum=(*pos);
			
			if ( (pBaum->xcoo>=xminfenster) && (pBaum->xcoo<=xmaxfenster) && (pBaum->ycoo>=yminfenster) && (pBaum->ycoo<=ymaxfenster) )
			{
				// calculate the Basal Area
				if(pBaum->hoehe>=130)
				{
					basalarea += ( M_PI * pow((pBaum->dbrust/2),2) );
					stemcount++;
				}
			
				// calculate the number of trees in the specified three height classes
				if (pBaum->hoehe <= 40) {nhoehe0b40++;}
				else if ( (pBaum->hoehe > 40) && (pBaum->hoehe <= 200) ) {nhoehe41b200++;}
				else if (pBaum->hoehe > 200) 
				{
					nhoehe201b10000++;
					meantreeheight+=pBaum->hoehe;
					meantreeage+=(double) pBaum->alter;
				}
				
				if (pBaum->dbrust>0)
				{
					breastdiameter=breastdiameter+pBaum->dbrust;
					breastdiametercount++;
				}

			}
			
			++pos;
		}	
		
		// add values to the output list
		pAuswertung->BAliste.push_back(basalarea);
		pAuswertung->nhoehe0b40liste.push_back(nhoehe0b40);
		pAuswertung->nhoehe41b200liste.push_back(nhoehe41b200);
		pAuswertung->nhoehe201b10000liste.push_back(nhoehe201b10000);
		if (breastdiametercount>0)
		{
			pAuswertung->meanbreastdiameterliste.push_back( (breastdiameter/ ( (double) breastdiametercount)) );
		}
		else
		{
			pAuswertung->meanbreastdiameterliste.push_back(0.0);
		}
		pAuswertung->stemcountliste.push_back(stemcount);
		if (nhoehe201b10000>0)
		{
			pAuswertung->meantreeheightliste.push_back(meantreeheight/ (double) nhoehe201b10000);
			pAuswertung->meantreeageliste.push_back( meantreeage/ (double) nhoehe201b10000);
		}
		else 
		{
			pAuswertung->meantreeheightliste.push_back( 0.0 );
			pAuswertung->meantreeageliste.push_back( 0.0 );
		}

		// calculate mean values of the specified time period (stabilmovingwindow)
		int basalareaanzahl=pAuswertung->BAliste.size();
		{ 
			// local declarations
			double meanbasalarea=0;
			double meannhoehe0b40=0, meannhoehe41b200=0, meannhoehe201b10000=0;
			double meanbreastdiameter=0;
			double meanstemcount=0;
			double meanmeantreeheigth=0;
			double meanmeantreeage=0;

			if(basalareaanzahl==(int) parameter[0].stabilmovingwindow) 
			{ 
				for (int position=basalareaanzahl-(int) parameter[0].stabilmovingwindow;position<basalareaanzahl;++position)
				{
					meanbasalarea+=pAuswertung->BAliste[position]/parameter[0].stabilmovingwindow;
					meannhoehe0b40+=pAuswertung->nhoehe0b40liste[position]/parameter[0].stabilmovingwindow;
					meannhoehe41b200+=pAuswertung->nhoehe41b200liste[position]/parameter[0].stabilmovingwindow;
					meannhoehe201b10000+=pAuswertung->nhoehe201b10000liste[position]/parameter[0].stabilmovingwindow;
					meanbreastdiameter+=pAuswertung->meanbreastdiameterliste[position]/parameter[0].stabilmovingwindow;
					meanstemcount+=pAuswertung->stemcountliste[position]/parameter[0].stabilmovingwindow;
					meanmeantreeheigth+=pAuswertung->meantreeheightliste[position]/parameter[0].stabilmovingwindow;
					meanmeantreeage+=pAuswertung->meantreeageliste[position]/parameter[0].stabilmovingwindow;
				}
				pAuswertung->BArunmeanliste.push_back(meanbasalarea);
				pAuswertung->nhoehe0b40runmeanliste.push_back(meannhoehe0b40);
				pAuswertung->nhoehe41b200runmeanliste.push_back(meannhoehe41b200);
				pAuswertung->nhoehe201b10000runmeanliste.push_back(meannhoehe201b10000);
				pAuswertung->meanbreastdiameterrunmeanliste.push_back(meanbreastdiameter);
				pAuswertung->stemcountrunmeanliste.push_back((int) floor(meanstemcount));
				pAuswertung->meantreeheightrunmeanliste.push_back(meanmeantreeheigth);
				pAuswertung->meantreeagerunmeanliste.push_back(meanmeantreeage);

			}
			else if (basalareaanzahl> (int) parameter[0].stabilmovingwindow) 
			{
				meanbasalarea=pAuswertung->BArunmeanliste[basalareaanzahl-((int) parameter[0].stabilmovingwindow+1)]		// mean of preceeding year
							-(pAuswertung->BAliste[basalareaanzahl-((int) parameter[0].stabilmovingwindow+1)]/parameter[0].stabilmovingwindow)		// minus first value of values that were used for calculation of the value in the preceeding year
							+(pAuswertung->BAliste[basalareaanzahl-1]/parameter[0].stabilmovingwindow);		// plus the current value
				pAuswertung->BArunmeanliste.push_back(meanbasalarea);
			
				meannhoehe0b40=pAuswertung->nhoehe0b40runmeanliste[basalareaanzahl-((int) parameter[0].stabilmovingwindow+1)]
							-(pAuswertung->nhoehe0b40liste[basalareaanzahl-((int) parameter[0].stabilmovingwindow+1)]/parameter[0].stabilmovingwindow)
							+(pAuswertung->nhoehe0b40liste[basalareaanzahl-1]/parameter[0].stabilmovingwindow);	
				pAuswertung->nhoehe0b40runmeanliste.push_back(meannhoehe0b40);
				
				meannhoehe41b200=pAuswertung->nhoehe41b200runmeanliste[basalareaanzahl-((int) parameter[0].stabilmovingwindow+1)]
							-(pAuswertung->nhoehe41b200liste[basalareaanzahl-((int) parameter[0].stabilmovingwindow+1)]/parameter[0].stabilmovingwindow)
							+(pAuswertung->nhoehe41b200liste[basalareaanzahl-1]/parameter[0].stabilmovingwindow);
				pAuswertung->nhoehe41b200runmeanliste.push_back(meannhoehe41b200);
				
				meannhoehe201b10000=pAuswertung->nhoehe201b10000runmeanliste[basalareaanzahl-((int) parameter[0].stabilmovingwindow+1)]	
							-(pAuswertung->nhoehe201b10000liste[basalareaanzahl-((int) parameter[0].stabilmovingwindow+1)]/parameter[0].stabilmovingwindow)
							+(pAuswertung->nhoehe201b10000liste[basalareaanzahl-1]/parameter[0].stabilmovingwindow);
				pAuswertung->nhoehe201b10000runmeanliste.push_back(meannhoehe201b10000);
				
				meanbreastdiameter=pAuswertung->meanbreastdiameterrunmeanliste[basalareaanzahl-((int) parameter[0].stabilmovingwindow+1)]
							-(pAuswertung->meanbreastdiameterliste[basalareaanzahl-((int) parameter[0].stabilmovingwindow+1)]/parameter[0].stabilmovingwindow)
							+(pAuswertung->meanbreastdiameterliste[basalareaanzahl-1]/parameter[0].stabilmovingwindow);	
				pAuswertung->meanbreastdiameterrunmeanliste.push_back(meanbreastdiameter);

				meanstemcount=(double) pAuswertung->stemcountrunmeanliste[basalareaanzahl-((int) parameter[0].stabilmovingwindow+1)]
							-(pAuswertung->stemcountliste[basalareaanzahl-((int) parameter[0].stabilmovingwindow+1)]/parameter[0].stabilmovingwindow)
							+(pAuswertung->stemcountliste[basalareaanzahl-1]/parameter[0].stabilmovingwindow);	
				pAuswertung->stemcountrunmeanliste.push_back((int) floor(meanstemcount));

				meanmeantreeheigth=pAuswertung->meantreeheightrunmeanliste[basalareaanzahl-((int) parameter[0].stabilmovingwindow+1)]
							-(pAuswertung->meantreeheightliste[basalareaanzahl-((int) parameter[0].stabilmovingwindow+1)]/parameter[0].stabilmovingwindow)
							+(pAuswertung->meantreeheightliste[basalareaanzahl-1]/parameter[0].stabilmovingwindow);	
				pAuswertung->meantreeheightrunmeanliste.push_back(meanmeantreeheigth);
				
				meanmeantreeage=pAuswertung->meantreeagerunmeanliste[basalareaanzahl-((int) parameter[0].stabilmovingwindow+1)]	
							-(pAuswertung->meantreeageliste[basalareaanzahl-((int) parameter[0].stabilmovingwindow+1)]/parameter[0].stabilmovingwindow)
							+(pAuswertung->meantreeageliste[basalareaanzahl-1]/parameter[0].stabilmovingwindow);
				pAuswertung->meantreeagerunmeanliste.push_back(meanmeantreeage);

				
			}

			// determination of the current state of the population
			int runmeanbasalareaanzahl=pAuswertung->BArunmeanliste.size();
			if (runmeanbasalareaanzahl>1)
			{ 
				
				// calculate slope
				double steigdiff=pAuswertung->BArunmeanliste[runmeanbasalareaanzahl-1]-pAuswertung->BArunmeanliste[runmeanbasalareaanzahl-2];
				if (pAuswertung->nachwendejahr!=true)
				{
					// determine the maximum slope (wendejahr) based on 50-year-running mean 
					if (steigdiff>=pAuswertung->maxsteigungbasalarea) 
					{
						pAuswertung->maxsteigungbasalarea=steigdiff;
						pAuswertung->countermaxsteigungbasalarea=50;
					}
					else 
					{
						--pAuswertung->countermaxsteigungbasalarea;
					}
					
					if (pAuswertung->countermaxsteigungbasalarea==0)
					{
						pAuswertung->wendejahr=jahr;
						pAuswertung->nachwendejahr=true;
					}
				}
				// if the slope turns negative after reaching the maximum slope the plateau (saettigungsjahr) is reached
				else 
				{
					if ( (steigdiff<0.0) && (pAuswertung->saettigungsjahr==-9999) )
					{
						pAuswertung->saettigungsjahr=jahr;
					}
				}
				
			}			
		} 
		// -- END update lists for runtime output AuswertungsListen -- //
		// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
		// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

		
		
		
		
		
		
		
		
		// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
		// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
		// -- -- -- -- -- START Output file creation -- -- -- -- -- -- //
		if (parameter[0].datenausgabe==true)
		{
			if (parameter[0].ausgabemodus==0) // "normal"
			{
				if (parameter[0].einschwingen==true)
				{
					ausgabecurrencies=true;
				}
				else 
				{
					ausgabecurrencies=true;
					
					if ( (jahr==1960) || (jahr==2010) )
					{
						ausgabeindividuen=true;
						ausgabedichte=true;
					}
				}
			}
			
			
		}

		// modes for writing output files
		if (ausgabecurrencies==true)
		{

		if ( (parameter[0].einschwingen==true & (parameter[0].ivort%5)==0 ) || (parameter[0].einschwingen==false) )
		{	

		// declarations
		int alter0=0, alter1=0, alter2=0, alter3=0, alter4=0, alter5=0, alter6b10=0, alter11b20=0, alter21b50=0, alter51b100=0, alter101b150=0, alter151b200=0, alter201b300=0, alter301b400=0, alter401b500=0, alter501b600=0, alter601b700=0, alter701plus=0;
		int gesamtsamenAKT=0, gesamtsamenSUM=0;
		double yposmax=0.0;
		double meanbas=0;
		int h= 0;

		// prepare file name
		if (parameter[0].einschwingen==true)
		{
			char dateinamesuf[5];
			sprintf(dateinamesuf, "_%.3d", parameter[0].wiederholung);
			dateiname="dataentwicklung_currencies" + string(dateinamesuf) + ".csv";
		}
		else 
		{
			dateiname="databaeume_currencies.csv";
		}
		
		// try open file to append data
		dateizeiger = fopen (dateiname.c_str(), "r+");
		// if not available begin a new file and write the header into
		if (dateizeiger == NULL)
		{
			dateizeiger = fopen (dateiname.c_str(), "w");
			fprintf(dateizeiger, "SimulationNo;");
			fprintf(dateizeiger, "YPLOTPOS;");
			fprintf(dateizeiger, "XPLOTPOS;");
			fprintf(dateizeiger, "Timestep;");
			fprintf(dateizeiger, "StabilPeriod;");
			
			if (parameter[0].einschwingen==true)
			{
				fprintf(dateizeiger, "RandomYear;");
			}
			else
			{
				fprintf(dateizeiger, "Year;");
			}

			fprintf(dateizeiger, "meanbasal;");
			fprintf(dateizeiger, "meanbreast;");

			// Baumspezifika
			fprintf(dateizeiger, "SimuAreaSize;");
			fprintf(dateizeiger, "NH0to40;");
			fprintf(dateizeiger, "NH40to200;");
			fprintf(dateizeiger, "NH200to10000;");
			fprintf(dateizeiger, "BasalArea;");
			fprintf(dateizeiger, "Stemcount;");
			fprintf(dateizeiger, "MeanTreeHeight;");
			fprintf(dateizeiger, "MeanTreeAge;");
			fprintf(dateizeiger, "YearMaximumGrowth;");
			fprintf(dateizeiger, "YearPlateaou;");
			fprintf(dateizeiger, "NTreeAge0;");
			fprintf(dateizeiger, "NTreeAge1;");
			fprintf(dateizeiger, "NTreeAge2;");
			fprintf(dateizeiger, "NTreeAge3;");
			fprintf(dateizeiger, "NTreeAge4;");
			fprintf(dateizeiger, "NTreeAge5;");
			fprintf(dateizeiger, "NTreeAge6to10;");
			fprintf(dateizeiger, "NTreeAge11to20;");
			fprintf(dateizeiger, "NTreeAge21to50;");
			fprintf(dateizeiger, "NTreeAge51to100;");
			fprintf(dateizeiger, "NTreeAge101to150;");
			fprintf(dateizeiger, "NTreeAge151to200;");
			fprintf(dateizeiger, "NTreeAge201to300;");
			fprintf(dateizeiger, "NTreeAge301to400;");
			fprintf(dateizeiger, "NTreeAge401to500;");
			fprintf(dateizeiger, "NTreeAge501to600;");
			fprintf(dateizeiger, "NTreeAge601to700;");
			fprintf(dateizeiger, "NTreeAge701plus;");
			fprintf(dateizeiger, "SumSeedProduceActual;");
			fprintf(dateizeiger, "SumSeedProduceCumulativ;");
			fprintf(dateizeiger, "SumSeedOnFloor;");
			fprintf(dateizeiger, "SumSeedInTrees;");
			// info on weather and derivates
			fprintf(dateizeiger, "WeatherType;");
			fprintf(dateizeiger, "TemperatureYear;");
			fprintf(dateizeiger, "TemperatureJanuary;");
			fprintf(dateizeiger, "TemperatureJuly;");
			fprintf(dateizeiger, "NDD;");
			fprintf(dateizeiger, "AAT;");
			fprintf(dateizeiger, "DDT;");
			fprintf(dateizeiger, "PrecipitationSum;");
			fprintf(dateizeiger, "MaxBasalGrowthSeedling;");
			fprintf(dateizeiger, "MaxBasalGrowth;");
			fprintf(dateizeiger, "MaxBreastGrowthm;");
			fprintf(dateizeiger, "\n");
			
			if (dateizeiger == NULL)
			{
				fprintf(stderr, "Error: File could not be openend!\n");
				exit(1);
			}
		}
		
		// go to end of file and append new data
		fseek(dateizeiger,0,SEEK_END);
						
		fprintf(dateizeiger, "%d;", parameter[0].wiederholung);
		fprintf(dateizeiger, "%d;", aktortyweltcoo);
		fprintf(dateizeiger, "%d;", aktortxweltcoo);
		fprintf(dateizeiger, "%d;", parameter[0].ivort);
		fprintf(dateizeiger, "%d;", t);
		fprintf(dateizeiger, "%d;", jahr);

		
		
		for (list<Baum*>::iterator pos = BaumListe.begin(); pos != BaumListe.end(); )
		{ 
			pBaum=(*pos);
			
			if ( (pBaum->xcoo>=xminfenster) && (pBaum->xcoo<=xmaxfenster) && (pBaum->ycoo>=yminfenster) && (pBaum->ycoo<=ymaxfenster) )
			{ 
				if (pBaum->alter == 0)	{alter0++;}
				else if (pBaum->alter == 1)	{alter1++;}
				else if (pBaum->alter == 2)	{alter2++;}
				else if (pBaum->alter == 3)	{alter3++;}
				else if (pBaum->alter == 4)	{alter4++;}
				else if (pBaum->alter == 5)	{alter5++;}
				else if ( (pBaum->alter > 5) && (pBaum->alter <= 10) )	{alter6b10++;}
				else if ( (pBaum->alter > 10) && (pBaum->alter <= 20) )	{alter11b20++;}
				else if ( (pBaum->alter > 20) && (pBaum->alter <= 50) )	{alter21b50++;}
				else if ( (pBaum->alter > 50) && (pBaum->alter <= 100) )	{alter51b100++;}
				else if ( (pBaum->alter > 100) && (pBaum->alter <= 150) )	{alter101b150++;}
				else if ( (pBaum->alter > 150) && (pBaum->alter <= 200) )	{alter151b200++;}
				else if ( (pBaum->alter > 200) && (pBaum->alter <= 300) )	{alter201b300++;}
				else if ( (pBaum->alter > 300) && (pBaum->alter <= 400) )	{alter301b400++;}
				else if ( (pBaum->alter > 400) && (pBaum->alter <= 500) )	{alter401b500++;}
				else if ( (pBaum->alter > 500) && (pBaum->alter <= 600) )	{alter501b600++;}
				else if ( (pBaum->alter > 600) && (pBaum->alter <= 700) )	{alter601b700++;}
				else if (pBaum->alter > 700)	{alter701plus++;}
				
				if (pBaum->dbasal>0)
				{
					meanbas=meanbas+pBaum->dbasal;
					h++;
				}



				// Samenzaehlung
				gesamtsamenAKT+=pBaum->samenneuproduziert;
				gesamtsamenSUM+=pBaum->samenproduziert;
				
				// Maximale Y-Position ermitteln
				if (pBaum->ycoo>yposmax) {yposmax=pBaum->ycoo;}
			} // Ende Ausschnitt
			
			++pos;
		}
		


		// count seeds
		int samenzapfenzahl=0, samenbodenzahl=0;
		for(list<Samen*>::iterator pos = SamenListe.begin(); pos != SamenListe.end(); pos++) 
		{
			pSamen=(*pos);
			if ( (pSamen->xcoo>=xminfenster) && (pSamen->xcoo<=xmaxfenster) && (pSamen->ycoo>=yminfenster) && (pSamen->ycoo<=ymaxfenster) )
			{ // Beginn Ausschnitt
				if (pSamen->imzapfen==true) {samenzapfenzahl++;}
				else {samenbodenzahl++;}
			} // Ende Ausschnitt
		}
		
		if (h>0)
			fprintf(dateizeiger, "%4.4f;", meanbas/((double) h));
		else
			fprintf(dateizeiger, "%4.4f;", 0.0);
		fprintf(dateizeiger, "%4.4f;", pAuswertung->meanbreastdiameterliste[pAuswertung->meanbreastdiameterliste.size()-1]);
		fprintf(dateizeiger, "%4.2f;",(xmaxfenster-xminfenster)*(ymaxfenster-yminfenster));
		fprintf(dateizeiger, "%d;", pAuswertung->nhoehe0b40liste[pAuswertung->nhoehe0b40liste.size()-1]);
		fprintf(dateizeiger, "%d;", pAuswertung->nhoehe41b200liste[pAuswertung->nhoehe41b200liste.size()-1]);
		fprintf(dateizeiger, "%d;", pAuswertung->nhoehe201b10000liste[pAuswertung->nhoehe201b10000liste.size()-1]);
		fprintf(dateizeiger, "%6.4f;", pAuswertung->BAliste[pAuswertung->BAliste.size()-1]);
		fprintf(dateizeiger, "%d;", pAuswertung->stemcountliste[pAuswertung->stemcountliste.size()-1]);
		fprintf(dateizeiger, "%6.4f;", pAuswertung->meantreeheightliste[pAuswertung->meantreeheightliste.size()-1]);
		fprintf(dateizeiger, "%6.4f;", pAuswertung->meantreeageliste[pAuswertung->meantreeageliste.size()-1]);
		fprintf(dateizeiger, "%d;", pAuswertung->wendejahr);
		fprintf(dateizeiger, "%d;", pAuswertung->saettigungsjahr);
		fprintf(dateizeiger, "%d;", alter0);
		fprintf(dateizeiger, "%d;", alter1);
		fprintf(dateizeiger, "%d;", alter2);
		fprintf(dateizeiger, "%d;", alter3);
		fprintf(dateizeiger, "%d;", alter4);
		fprintf(dateizeiger, "%d;", alter5);
		fprintf(dateizeiger, "%d;", alter6b10);
		fprintf(dateizeiger, "%d;", alter11b20);
		fprintf(dateizeiger, "%d;", alter21b50);
		fprintf(dateizeiger, "%d;", alter51b100);
		fprintf(dateizeiger, "%d;", alter101b150);
		fprintf(dateizeiger, "%d;", alter151b200);
		fprintf(dateizeiger, "%d;", alter201b300);
		fprintf(dateizeiger, "%d;", alter301b400);
		fprintf(dateizeiger, "%d;", alter401b500);
		fprintf(dateizeiger, "%d;", alter501b600);
		fprintf(dateizeiger, "%d;", alter601b700);
		fprintf(dateizeiger, "%d;", alter701plus);
		fprintf(dateizeiger, "%d;", gesamtsamenAKT);
		fprintf(dateizeiger, "%d;", gesamtsamenSUM);
		fprintf(dateizeiger, "%d;", samenbodenzahl);
		fprintf(dateizeiger, "%d;", samenzapfenzahl);
		// info on weather and derivates
		fprintf(dateizeiger, "%d;", parameter[0].wetterwahl);
		fprintf(dateizeiger, "%4.4f;", WetterListe[jahrposition]->tempjahrmittel);
		fprintf(dateizeiger, "%4.4f;", WetterListe[jahrposition]->temp1monatmittel);
		fprintf(dateizeiger, "%4.4f;", WetterListe[jahrposition]->temp7monatmittel);
		fprintf(dateizeiger, "%d;", WetterListe[jahrposition]->vegetationslaenge);
		fprintf(dateizeiger, "%4.4f;", WetterListe[jahrposition]->activeairtemp);
		fprintf(dateizeiger, "%4.4f;", WetterListe[jahrposition]->degreday);
		fprintf(dateizeiger, "%4.2f;", WetterListe[jahrposition]->niederschlagssumme);
		fprintf(dateizeiger, "%4.4f;", WetterListe[jahrposition]->maxbasalwachstumjahr);
		fprintf(dateizeiger, "%4.4f;", WetterListe[jahrposition]->maxbasalsaplwachstumjahr);
		fprintf(dateizeiger, "%4.4f;", WetterListe[jahrposition]->maxbrustwachstumjahr);
		
		fprintf(dateizeiger, "\n");
		
		fclose (dateizeiger);

		if (parameter[0].jahranzeige ==true) 
		{
			cout << "\t" << aktort << ": N0-40 = " << nhoehe0b40 << "\tN40-200 = " << nhoehe41b200 << "\tN200+ = " << nhoehe201b10000 << "\tNseeds:\tproduced = " << gesamtsamenAKT << "\tground = " << samenbodenzahl << "\tcones = " << samenzapfenzahl << endl;
		}
		
		}

		}//ausgabecurrencies
		
		if (ausgabeindividuen==true)
		{

		// prepare file name
		if (parameter[0].ausgabemodus==0)
		{
			char dateinamesuf[10];
			sprintf(dateinamesuf, "%.5d", jahr);
			dateiname="databaeume_" + string(dateinamesuf) + ".csv";
		}

		// try open file to append data
		dateizeiger = fopen (dateiname.c_str(), "r+");
		// if not available begin a new file and write the header into
		if (dateizeiger == NULL)
		{
			dateizeiger = fopen (dateiname.c_str(), "w");
	
			// general information
			fprintf(dateizeiger, "SimulationNo;");
			fprintf(dateizeiger, "YPLOTPOS;");
			fprintf(dateizeiger, "XPLOTPOS;");
			fprintf(dateizeiger, "Timestep;");
			fprintf(dateizeiger, "Year;");
			// tree specific
			fprintf(dateizeiger, "X;");
			fprintf(dateizeiger, "Y;");
			fprintf(dateizeiger, "Name;");
			fprintf(dateizeiger, "NameM;");
			fprintf(dateizeiger, "NameP;");
			fprintf(dateizeiger, "Lineage;");
			fprintf(dateizeiger, "Generation;");
			fprintf(dateizeiger, "Height;");
			fprintf(dateizeiger, "DiaBasal;");
			fprintf(dateizeiger, "DiaBreast;");
			fprintf(dateizeiger, "Age;");
			fprintf(dateizeiger, "Cones;");
			fprintf(dateizeiger, "ConeHeightStart;");
			fprintf(dateizeiger, "SeedProdActual;");
			fprintf(dateizeiger, "SeedProdCumulat;");
			fprintf(dateizeiger, "DensityValue;");
			// Ende
			fprintf(dateizeiger, "\n");

			if (dateizeiger == NULL)
			{
				fprintf(stderr, "Error: File could not be openend!\n");
				exit(1);
			}
		}

		// go to end of file and append new data
		fseek(dateizeiger,0,SEEK_END);

		for (list<Baum*>::iterator pos = BaumListe.begin(); pos != BaumListe.end(); )
		{ 
			pBaum=(*pos);
			// general information
			fprintf(dateizeiger, "%d;", parameter[0].wiederholung);
			fprintf(dateizeiger, "%d;", pBaum->yweltcoo);
			fprintf(dateizeiger, "%d;", pBaum->xweltcoo);
			fprintf(dateizeiger, "%d;", t);
			fprintf(dateizeiger, "%d;", jahr);
			// tree specific
			fprintf(dateizeiger, "%4.4f;", pBaum->xcoo);
			fprintf(dateizeiger, "%4.4f;", pBaum->ycoo);
			fprintf(dateizeiger, "%d;", pBaum->name);
			fprintf(dateizeiger, "%d;", pBaum->namem);
			fprintf(dateizeiger, "%d;", pBaum->namep);
			fprintf(dateizeiger, "%d;", pBaum->linie);
			fprintf(dateizeiger, "%d;", pBaum->generation);
			fprintf(dateizeiger, "%4.4f;", pBaum->hoehe);
			fprintf(dateizeiger, "%4.4f;", pBaum->dbasal);
			fprintf(dateizeiger, "%4.4f;", pBaum->dbrust);
			fprintf(dateizeiger, "%d;", pBaum->alter);
			fprintf(dateizeiger, "%d;", pBaum->zapfen);
			fprintf(dateizeiger, "%4.4f;", pBaum->zapfenhoehe);
			fprintf(dateizeiger, "%d;", pBaum->samenneuproduziert);
			fprintf(dateizeiger, "%d;", pBaum->samenproduziert);
			fprintf(dateizeiger, "%4.5f;", pBaum->dichtewert);
			fprintf(dateizeiger, "\n");

			++pos;
		}

		fclose (dateizeiger);

		}
		
		if (ausgabedichte==true)
		{

		// prepare file name
		if (parameter[0].ausgabemodus==0)
		{
			char dateinamesuf[10];
			sprintf(dateinamesuf, "%.5d", jahr);
			dateiname="databaeume_baumdichte" + string(dateinamesuf) + ".csv";
		}
	
		// try open file to append data
		dateizeiger = fopen (dateiname.c_str(), "r+");
		// if not available begin a new file and write the header into
		if (dateizeiger == NULL)
		{
		  dateizeiger = fopen (dateiname.c_str(), "w");
		  
			fprintf(dateizeiger, "SimulationNo;");
			fprintf(dateizeiger, "YPLOTPOS;");
			fprintf(dateizeiger, "XPLOTPOS;");
			fprintf(dateizeiger, "X;");
			fprintf(dateizeiger, "Y;");
			fprintf(dateizeiger, "TreeDensityValue;");
			fprintf(dateizeiger, "TreeNumber;");
			fprintf(dateizeiger, "\n");

			if (dateizeiger == NULL)
			{
				fprintf(stderr, "Error: File could not be openend!\n");
				exit(1);
			}
		}

		// go to end of file and append new data
		fseek(dateizeiger,0,SEEK_END);

		// process data and append to file 
		for (int kartenpos=0; kartenpos< (baeumerows*parameter[0].sizemagnif*baeumecols*parameter[0].sizemagnif); kartenpos++)
		{
			pKarten=KartenListe[kartenpos];
			if ( (pKarten->baumanzahl>0) && ( (pKarten->xcoo>=xminfenster*parameter[0].sizemagnif) && (pKarten->xcoo<=xmaxfenster*parameter[0].sizemagnif) && (pKarten->ycoo>=yminfenster*parameter[0].sizemagnif) && (pKarten->ycoo<=ymaxfenster*parameter[0].sizemagnif) ) )
			{ // write only data with tree density values to file
				fprintf(dateizeiger, "%d;", parameter[0].wiederholung);
				fprintf(dateizeiger, "%d;", pKarten->yweltcoo);
				fprintf(dateizeiger, "%d;", pKarten->xweltcoo);
				fprintf(dateizeiger, "%4.4f;", pKarten->xcoo);
				fprintf(dateizeiger, "%4.4f;", pKarten->ycoo);
				fprintf(dateizeiger, "%4.5f;", pKarten->baumdichtewert);
				fprintf(dateizeiger, "%d;", pKarten->baumanzahl);
				fprintf(dateizeiger, "\n");
			}
		}

		fclose (dateizeiger);

		}
		// -- -- -- -- -- - END Output file creation -- -- -- -- -- -- //
		// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
		// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
		
	}

}



