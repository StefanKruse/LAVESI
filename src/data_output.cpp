using namespace std;

void Data_output( int t, int jahr, struct Parameter *parameter, int yearposition, vector<list<Tree*> > &world_tree_list, vector<list<seed*> > &world_seed_list, vector<vector<weather*> > &world_weather_list, vector<vector<Karten*> > &world_plot_list, vector<vector<Evaluation*> > &world_evaluation_list)
{
	double xminwindow=0.0;
	double xmaxwindow=0.0;
	double yminwindow=0.0;
	double ymaxwindow=0.0;
	
	FILE *dateizeiger;
	string dateiname;
	if(parameter[0].outputall==0)
	{
	xminwindow=( (double) treecols/2.0)-10.0;
	xmaxwindow=( (double) treecols/2.0)+10.0;
	yminwindow=( (double) treerows/2.0)-10.0;
	ymaxwindow=( (double) treerows/2.0)+10.0;
	
	}
	else if(parameter[0].outputall==1)
	{
	xminwindow=((double) 0.0);
	xmaxwindow=((double) treecols);
	yminwindow=((double) 0.0);
	ymaxwindow=((double) treerows);
	}
	
	
	
	// Declarations for the console output determination
	bool ausgabecurrencies=false;
	bool ausgabeposition=false;
	bool ausgabeindividuen=false;
	bool ausgabedendro=false;
	bool ausgabedensity=false;

	
	// preprocession and output of the data for each plot
	int aktort=0;
	for (vector<list<Tree*> >::iterator posw = world_tree_list.begin(); posw != world_tree_list.end(); ++posw)
	{ // world tree list loop
		list<Tree*>& tree_list = *posw;
		
		vector<list<seed*> >::iterator world_positon_s = (world_seed_list.begin()+aktort);
		list<seed*>& seed_list = *world_positon_s;
		
		vector<vector<Karten*> >::iterator poskarten = (world_plot_list.begin()+aktort);
		vector<Karten*>& plot_list = *poskarten;
		
		vector<vector<weather*> >::iterator posiwelt = (world_weather_list.begin()+aktort);
		vector<weather*>& weather_list = *posiwelt;
		
		vector<vector<Evaluation*> >::iterator posiweltausw = (world_evaluation_list.begin()+aktort);
		vector<Evaluation*>& evaluation_list = *posiweltausw;
		vector<Evaluation*>::iterator posausw = evaluation_list.begin();
		pEvaluation=(*posausw);
		
		aktort++;
		
		// calculation of the current location
		int aktortyworldcoo=(int) floor( (double) (aktort-1)/parameter[0].mapxlength );
		int aktortxworldcoo=(aktort-1) - (aktortyworldcoo * parameter[0].mapxlength);

		
		// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
		// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
		// -- -- -- update evaluation lists -- -- -- --	-- -- -- -- -- //

		//declarations
		double basalarea=0.0;
		int nheight0b40=0, nheight41b200=0, nheight201b10000=0;
		double breastdiameter=0.0;
		int breastdiametercount=0;
		int stemcount=0;
		double meantreeheight=0.0, meantreeage=0.0;

		for (list<Tree*>::iterator pos = tree_list.begin(); pos != tree_list.end(); )
		{ // Begin tree_list loop
			pTree=(*pos);
			
			if ( (pTree->xcoo>=xminwindow) && (pTree->xcoo<=xmaxwindow) && (pTree->ycoo>=yminwindow) && (pTree->ycoo<=ymaxwindow) )
			{ // Begin loop over reduced plot
				// Basal area as population size identifier
				if(pTree->height>=130)
				{
					basalarea += ( M_PI * pow((pTree->dbrust/2),2) );
					stemcount++;
				}
			
				// Population sizes in height classes
				if (pTree->height <= 40)
                                {
                                    nheight0b40++;
                                }
				else if ( (pTree->height > 40) && (pTree->height <= 200) ) 
                                {
                                    nheight41b200++;
                                }
				else if (pTree->height > 200) 
				{
					nheight201b10000++;
					meantreeheight+=pTree->height;
					meantreeage+=(double) pTree->age;
				}
				
				if (pTree->dbrust>0)
				{
					breastdiameter=breastdiameter+pTree->dbrust;
					breastdiametercount++;
				}

			} // Ende loop over reduced plot
			
			++pos;
		} // End tree_list loop
		
		// push back values in evaluation list
		pEvaluation->BAliste.push_back(basalarea);
		
		pEvaluation->nheight0b40liste.push_back(nheight0b40);
		pEvaluation->nheight41b200liste.push_back(nheight41b200);
		pEvaluation->nheight201b10000liste.push_back(nheight201b10000);
		
		if (breastdiametercount>0)
			pEvaluation->meanbreastdiameterliste.push_back( (breastdiameter/ ( (double) breastdiametercount)) );
		else
			pEvaluation->meanbreastdiameterliste.push_back( 0.0 );
			
		pEvaluation->stemcountliste.push_back(stemcount);

		if (nheight201b10000>0)
		{
			pEvaluation->meantreeheightliste.push_back(meantreeheight/ (double) nheight201b10000);
			pEvaluation->meantreeageliste.push_back( meantreeage/ (double) nheight201b10000);
		}
		else 
		{
			pEvaluation->meantreeheightliste.push_back( 0.0 );
			pEvaluation->meantreeageliste.push_back( 0.0 );
		}

		// mean value calculation
		int basalareaanzahl=pEvaluation->BAliste.size();
		
		
		//THERE IS NO LOOP OR IF STATEMENT!?
			// If parameter[0].stabilmovingwindow values are set begin:
			
			//***german:
		{ 	// Falls schon parameter[0].stabilmovingwindow Werte vorhanden Beginn
			double meanbasalarea=0;
			double meannheight0b40=0, meannheight41b200=0, meannheight201b10000=0;
			double meanbreastdiameter=0;
			double meanstemcount=0;
			double meanmeantreeheigth=0;
			double meanmeantreeage=0;
			if(basalareaanzahl==(int) parameter[0].stabilmovingwindow) 
			{ // if exactly parameter[0].stabilmovingwindow values are set BEGIN
				for (int position=basalareaanzahl-(int) parameter[0].stabilmovingwindow;position<basalareaanzahl;++position)
				{
					meanbasalarea+=pEvaluation->BAliste[position]/parameter[0].stabilmovingwindow;
					meannheight0b40+=pEvaluation->nheight0b40liste[position]/parameter[0].stabilmovingwindow;
					meannheight41b200+=pEvaluation->nheight41b200liste[position]/parameter[0].stabilmovingwindow;
					meannheight201b10000+=pEvaluation->nheight201b10000liste[position]/parameter[0].stabilmovingwindow;
					meanbreastdiameter+=pEvaluation->meanbreastdiameterliste[position]/parameter[0].stabilmovingwindow;
					meanstemcount+=pEvaluation->stemcountliste[position]/parameter[0].stabilmovingwindow;
					meanmeantreeheigth+=pEvaluation->meantreeheightliste[position]/parameter[0].stabilmovingwindow;
					meanmeantreeage+=pEvaluation->meantreeageliste[position]/parameter[0].stabilmovingwindow;
				}
				pEvaluation->BArunmeanliste.push_back(meanbasalarea);
				pEvaluation->nheight0b40runmeanliste.push_back(meannheight0b40);
				pEvaluation->nheight41b200runmeanliste.push_back(meannheight41b200);
				pEvaluation->nheight201b10000runmeanliste.push_back(meannheight201b10000);
				pEvaluation->meanbreastdiameterrunmeanliste.push_back(meanbreastdiameter);
				pEvaluation->stemcountrunmeanliste.push_back((int) floor(meanstemcount));
				pEvaluation->meantreeheightrunmeanliste.push_back(meanmeantreeheigth);
				pEvaluation->meantreeagerunmeanliste.push_back(meanmeantreeage);

			} // if exactly parameter[0].stabilmovingwindow values are set END
			else if (basalareaanzahl> (int) parameter[0].stabilmovingwindow) 
			{// if more than parameter[0].stabilmovingwindow values are set BEGIN
				meanbasalarea=pEvaluation->BArunmeanliste[basalareaanzahl-((int) parameter[0].stabilmovingwindow+1)]		// Mittelwert des Vorjahres
							-(pEvaluation->BAliste[basalareaanzahl-((int) parameter[0].stabilmovingwindow+1)]/parameter[0].stabilmovingwindow)		// minus den ersten Wert aus Bereich zur Berechnung des Mittelwertes des Vorjahres
							+(pEvaluation->BAliste[basalareaanzahl-1]/parameter[0].stabilmovingwindow);		// plus den neuen Wert aus diesem Jahr
				pEvaluation->BArunmeanliste.push_back(meanbasalarea);
			
				meannheight0b40=pEvaluation->nheight0b40runmeanliste[basalareaanzahl-((int) parameter[0].stabilmovingwindow+1)]		// Mittelwert des Vorjahres
							-(pEvaluation->nheight0b40liste[basalareaanzahl-((int) parameter[0].stabilmovingwindow+1)]/parameter[0].stabilmovingwindow)		// minus den ersten Wert aus Bereich zur Berechnung des Mittelwertes des Vorjahres
							+(pEvaluation->nheight0b40liste[basalareaanzahl-1]/parameter[0].stabilmovingwindow);		// plus den neuen Wert aus diesem Jahr
				pEvaluation->nheight0b40runmeanliste.push_back(meannheight0b40);
				
				meannheight41b200=pEvaluation->nheight41b200runmeanliste[basalareaanzahl-((int) parameter[0].stabilmovingwindow+1)]		// Mittelwert des Vorjahres
							-(pEvaluation->nheight41b200liste[basalareaanzahl-((int) parameter[0].stabilmovingwindow+1)]/parameter[0].stabilmovingwindow)		// minus den ersten Wert aus Bereich zur Berechnung des Mittelwertes des Vorjahres
							+(pEvaluation->nheight41b200liste[basalareaanzahl-1]/parameter[0].stabilmovingwindow);		// plus den neuen Wert aus diesem Jahr
				pEvaluation->nheight41b200runmeanliste.push_back(meannheight41b200);
				
				meannheight201b10000=pEvaluation->nheight201b10000runmeanliste[basalareaanzahl-((int) parameter[0].stabilmovingwindow+1)]		// Mittelwert des Vorjahres
							-(pEvaluation->nheight201b10000liste[basalareaanzahl-((int) parameter[0].stabilmovingwindow+1)]/parameter[0].stabilmovingwindow)		// minus den ersten Wert aus Bereich zur Berechnung des Mittelwertes des Vorjahres
							+(pEvaluation->nheight201b10000liste[basalareaanzahl-1]/parameter[0].stabilmovingwindow);		// plus den neuen Wert aus diesem Jahr
				pEvaluation->nheight201b10000runmeanliste.push_back(meannheight201b10000);
				
				meanbreastdiameter=pEvaluation->meanbreastdiameterrunmeanliste[basalareaanzahl-((int) parameter[0].stabilmovingwindow+1)]		// Mittelwert des Vorjahres
							-(pEvaluation->meanbreastdiameterliste[basalareaanzahl-((int) parameter[0].stabilmovingwindow+1)]/parameter[0].stabilmovingwindow)		// minus den ersten Wert aus Bereich zur Berechnung des Mittelwertes des Vorjahres
							+(pEvaluation->meanbreastdiameterliste[basalareaanzahl-1]/parameter[0].stabilmovingwindow);		// plus den neuen Wert aus diesem Jahr
				pEvaluation->meanbreastdiameterrunmeanliste.push_back(meanbreastdiameter);

				meanstemcount=(double) pEvaluation->stemcountrunmeanliste[basalareaanzahl-((int) parameter[0].stabilmovingwindow+1)]		// Mittelwert des Vorjahres
							-(pEvaluation->stemcountliste[basalareaanzahl-((int) parameter[0].stabilmovingwindow+1)]/parameter[0].stabilmovingwindow)		// minus den ersten Wert aus Bereich zur Berechnung des Mittelwertes des Vorjahres
							+(pEvaluation->stemcountliste[basalareaanzahl-1]/parameter[0].stabilmovingwindow);		// plus den neuen Wert aus diesem Jahr
				pEvaluation->stemcountrunmeanliste.push_back((int) floor(meanstemcount));

				meanmeantreeheigth=pEvaluation->meantreeheightrunmeanliste[basalareaanzahl-((int) parameter[0].stabilmovingwindow+1)]		// Mittelwert des Vorjahres
							-(pEvaluation->meantreeheightliste[basalareaanzahl-((int) parameter[0].stabilmovingwindow+1)]/parameter[0].stabilmovingwindow)		// minus den ersten Wert aus Bereich zur Berechnung des Mittelwertes des Vorjahres
							+(pEvaluation->meantreeheightliste[basalareaanzahl-1]/parameter[0].stabilmovingwindow);		// plus den neuen Wert aus diesem Jahr
				pEvaluation->meantreeheightrunmeanliste.push_back(meanmeantreeheigth);
				
				meanmeantreeage=pEvaluation->meantreeagerunmeanliste[basalareaanzahl-((int) parameter[0].stabilmovingwindow+1)]		// Mittelwert des Vorjahres
							-(pEvaluation->meantreeageliste[basalareaanzahl-((int) parameter[0].stabilmovingwindow+1)]/parameter[0].stabilmovingwindow)		// minus den ersten Wert aus Bereich zur Berechnung des Mittelwertes des Vorjahres
							+(pEvaluation->meantreeageliste[basalareaanzahl-1]/parameter[0].stabilmovingwindow);		// plus den neuen Wert aus diesem Jahr
				pEvaluation->meantreeagerunmeanliste.push_back(meanmeantreeage);

				
			}// if more than parameter[0].stabilmovingwindow values are set END



			// Verarbeitung der BA-Mittelwerte
			int runmeanbasalareaanzahl=pEvaluation->BArunmeanliste.size();
			if (runmeanbasalareaanzahl>1)
			{ // If more than 2 parameter[0].stabilmovingwindow yearly mean values BEGIN
				
				// Calculation of the slope
				double steigdiff=pEvaluation->BArunmeanliste[runmeanbasalareaanzahl-1]-pEvaluation->BArunmeanliste[runmeanbasalareaanzahl-2];
				if (pEvaluation->nachwendejahr!=true)
				{	
			
					// If a maximal slope is not surpassed in 50 years, the point of inflection of growth is reached
					if (steigdiff>=pEvaluation->maxsteigungbasalarea) 
					{
						pEvaluation->maxsteigungbasalarea=steigdiff;
						pEvaluation->countermaxsteigungbasalarea=50;
					}
					else 
					{
						--pEvaluation->countermaxsteigungbasalarea;
					}
					if (pEvaluation->countermaxsteigungbasalarea==0)
					{
						pEvaluation->wendejahr=jahr;
						pEvaluation->nachwendejahr=true;
					}
				}
				// After the point of inflection is met, the saturation of the system is reached as soon as the slope becomes negative
				else 
				{
					if ( (steigdiff<0.0) && (pEvaluation->saettigungsjahr==-9999) )
					{
						pEvaluation->saettigungsjahr=jahr;
					}
				}
				
			} // more than 2 parameter[0].stabilmovingwindow yearly mean values END			
		} // If 50 basal area values are set END
		
		// -- --			 evaluation_list update				 -- -- //
		// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
		// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

		
		
		// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
		// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
		if (parameter[0].dataoutput==true)
		{//Data output depending on the corresponding parameter BEGIN
			if (parameter[0].outputmode==0) // "normally"
			{
				if (parameter[0].einschwingen==true)
				{
					ausgabecurrencies=true;
					ausgabeposition=true;
				}
				else 
				{
					ausgabecurrencies=true;
					
					if ( (jahr==1010) || (jahr==1510) || (jahr==2010) )
					{
						ausgabeposition=true;
						ausgabeindividuen=true;
						//ausgabedendro=true;
						ausgabedensity=true;
					}
				}
			}
			else if (parameter[0].outputmode==1) // "SensitivityAnalysis"
			{
				if (parameter[0].einschwingen==true)
				{
					ausgabecurrencies=true;
					ausgabeposition=true;
				}
				else 
				{
					ausgabecurrencies=true;
					
					if ( (pEvaluation->saettigungsjahr==jahr) || (pEvaluation->wendejahr==jahr) )
					{
						ausgabeposition=true;
						ausgabeindividuen=true;
						//ausgabedendro=true;
						ausgabedensity=true;
					}
				}
			}
			else if (parameter[0].outputmode==2) // "SA_2"
			{
				ausgabecurrencies=true;

				if ((parameter[0].einschwingen==false) && ((t==0) || (jahr==2010)) )
				{
					ausgabeposition=true;
					ausgabeindividuen=true;
					//ausgabedendro=true;
					ausgabedensity=true;
				}
			}
			else if (parameter[0].outputmode==3) // "ClimGrad"
			{
				ausgabecurrencies=true;

				if ((parameter[0].einschwingen==false) && ((t==0) 	|| (jahr==1100) || (jahr==1110) || (jahr==1120) || (jahr==1130) || (jahr==1140) || (jahr==1150) || (jahr==1160)  			|| (jahr==1500) || (jahr==1510) || (jahr==1520) || (jahr==1530) || (jahr==1540) || (jahr==1550) || (jahr==1560) 						|| (jahr==1770) || (jahr==1780) || (jahr==1790) || (jahr==1800) || (jahr==1810) || (jahr==1820) || (jahr==1830) 			|| (jahr==1950) || (jahr==1960) || (jahr==1970) || (jahr==1980) || (jahr==1990) || (jahr==2000) || (jahr==2010)) )
				{
					ausgabeposition=true;
					ausgabeindividuen=true;
					//ausgabedendro=true;
					ausgabedensity=true;
				}
			}
			else if (parameter[0].outputmode==4) // "TemperatureExperiment"
			{
				ausgabecurrencies=true;
				
				if (parameter[0].einschwingen==true)
				{
					ausgabeposition=true;
				}
				
				if ((parameter[0].einschwingen==false) && ((jahr==1911) || (jahr==1900) || (jahr==1920) || (jahr==1930) || (jahr==1940) || (jahr==1950) || (jahr==1960) || (jahr==1970) || (jahr==1980) || (jahr==1990) || (jahr==2000) || (jahr==2010) || (jahr==2011)))
				{
					ausgabeposition=true;
					// ausgabeindividuen=true;
					//ausgabedendro=true;
					ausgabedensity=true;
				}

			}
			else if (parameter[0].outputmode==5) // "TemperatureExperiment"
			{
				ausgabecurrencies=true;
				
				if (parameter[0].ivort>1045)
				{
					ausgabeindividuen=true;
				}

			}
			else if (parameter[0].outputmode==9) // "OMP"
			{
				ausgabecurrencies=true;
			}
		}//Data output depending on the corresponding parameter END
		
		// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
		// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
		
		ostringstream s1,s2,s3,s4,s5,s6,s7,s8;
	
		if (ausgabecurrencies==true)
		{	//ausgabecurrencies
			// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
			// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
			// -- -- -- -- -- -- -- trees Currencies -- -- -- -- -- -- -- //
	
			if ( parameter[0].einschwingen==true || parameter[0].einschwingen==false )
			{	

				// Deklarationen
				int ageg0=0, ageg1=0, ageg2=0, ageg3=0, ageg4=0, ageg5=0, ageg6b10=0, ageg11b20=0, ageg21b50=0, ageg51b100=0, ageg101b150=0, ageg151b200=0, ageg201b300=0, ageg301b400=0, ageg401b500=0, ageg501b600=0, ageg601b700=0, ageg701plus=0;
				int ages0=0, ages1=0, ages2=0, ages3=0, ages4=0, ages5=0, ages6b10=0, ages11b20=0, ages21b50=0, ages51b100=0, ages101b150=0, ages151b200=0, ages201b300=0, ages301b400=0, ages401b500=0, ages501b600=0, ages601b700=0, ages701plus=0;
				int gesamtseedAKT=0, gesamtseedSUM=0;
				int spectree1=0, spectree2=0;
				double yposmax=0.0;
				double meanbas=0.0;
				int h= 0;
				
	
				// assemble file name
				if (parameter[0].einschwingen==true)
				{
					s1<<parameter[0].repeati;
					s2<<parameter[0].weatherchoice;
					dateiname="output/dataentwicklung_currencies_" + s1.str()+"_"+s2.str() + ".csv";
					s1.str("");s1.clear();
					s2.str("");s2.clear();
				}
				else 
				{
					s1<<parameter[0].weatherchoice;
					dateiname="output/datatrees_currencies_" + s1.str() + ".csv";
					s1.str("");s1.clear();
					s2.str("");s2.clear();
				}
		
				// Trying to open the data file for reading
				dateizeiger = fopen (dateiname.c_str(), "r+");
				// if fopen fails, open a new data file:
				if (dateizeiger == NULL)
				{
					dateizeiger = fopen (dateiname.c_str(), "w+");
					fprintf(dateizeiger, "Repeat;");
					fprintf(dateizeiger, "YPLOTPOS;");
					fprintf(dateizeiger, "XPLOTPOS;");
					fprintf(dateizeiger, "Progress;");
					fprintf(dateizeiger, "Spinup;");
				
					if (parameter[0].einschwingen==true)
					{
						fprintf(dateizeiger, "Randomyear;");
					}
					else
					{
						fprintf(dateizeiger, "Year;");
					}

					fprintf(dateizeiger, "meanbas;");
					fprintf(dateizeiger, "meanbr;");

					// Tree specifics
					fprintf(dateizeiger, "Ausschnittsgroesze;");
					fprintf(dateizeiger, "NderH0bis40;");
					fprintf(dateizeiger, "NderH40bis200;");
					fprintf(dateizeiger, "NderH200bis10000;");
					fprintf(dateizeiger, "Basalarea;");
					fprintf(dateizeiger, "Stemcount;");
					fprintf(dateizeiger, "MeanTreeHeight;");
					fprintf(dateizeiger, "MeanTreeAge;");
					fprintf(dateizeiger, "Wendejahr;");
					fprintf(dateizeiger, "Saettigungsjahr;");
					fprintf(dateizeiger, "ageg0;");
					fprintf(dateizeiger, "ages0;");
					fprintf(dateizeiger, "ageg1;");
					fprintf(dateizeiger, "ages1;");
					fprintf(dateizeiger, "ageg2;");
					fprintf(dateizeiger, "ages2;");
					fprintf(dateizeiger, "ageg3;");
					fprintf(dateizeiger, "ages3;");
					fprintf(dateizeiger, "ageg4;");
					fprintf(dateizeiger, "ages4;");
					fprintf(dateizeiger, "ageg5;");
					fprintf(dateizeiger, "ages5;");
					fprintf(dateizeiger, "ageg6bis10;");
					fprintf(dateizeiger, "ages6bis10;");
					fprintf(dateizeiger, "ageg11bis20;");
					fprintf(dateizeiger, "ages11bis20;");
					fprintf(dateizeiger, "ageg21bis50;");
					fprintf(dateizeiger, "ages21bis50;");
					fprintf(dateizeiger, "ageg51bis100;");
					fprintf(dateizeiger, "ages51bis100;");
					fprintf(dateizeiger, "ageg101bis150;");
					fprintf(dateizeiger, "ages101bis150;");
					fprintf(dateizeiger, "ageg151bis200;");
					fprintf(dateizeiger, "ages151bis200;");
					fprintf(dateizeiger, "ageg201bis300;");
					fprintf(dateizeiger, "ages201bis300;");
					fprintf(dateizeiger, "ageg301bis400;");
					fprintf(dateizeiger, "ages301bis400;");
					fprintf(dateizeiger, "ageg401bis500;");
					fprintf(dateizeiger, "ages401bis500;");
					fprintf(dateizeiger, "ageg501bis600;");
					fprintf(dateizeiger, "ages501bis600;");
					fprintf(dateizeiger, "ageg601bis700;");
					fprintf(dateizeiger, "ages601bis700;");
					fprintf(dateizeiger, "ageg701plus;");
					fprintf(dateizeiger, "ages701plus;");					
					fprintf(dateizeiger, "GesamtseedprodAKT;");
					fprintf(dateizeiger, "GesamtseedprodSUM;");
					fprintf(dateizeiger, "Trees_gmel;");
					fprintf(dateizeiger, "Trees_sib;");					
					fprintf(dateizeiger, "GesamtseedBoden;");
					fprintf(dateizeiger, "Gesamtseedtrees;");
					fprintf(dateizeiger, "Seeds_gmel;");
					fprintf(dateizeiger, "Seeds_sib;");
					fprintf(dateizeiger, "Y_Pos_Max;");
					// weatherspezifika
					fprintf(dateizeiger, "weathertyp;");
					fprintf(dateizeiger, "Starttrees;");
					fprintf(dateizeiger, "Jahrestemp;");
					fprintf(dateizeiger, "Jantemp;");
					fprintf(dateizeiger, "Julitemp;");
					fprintf(dateizeiger, "Veglaenge;");
					fprintf(dateizeiger, "AAT;");
					fprintf(dateizeiger, "DDT;");
					fprintf(dateizeiger, "Prec;");
					fprintf(dateizeiger, "weatherfaktor_gmel;");				
					fprintf(dateizeiger, "weatherfaktor_sib;");							
					fprintf(dateizeiger, "AuftretenFeuer;");
					fprintf(dateizeiger, "thawing_depth_infl;");
					fprintf(dateizeiger, "\n");
				
					if (dateizeiger == NULL)
					{
						fprintf(stderr, "Fehler: Datei konnte nicht geoeffnet werden!\n");
						exit(1);
					}
				}
			
				// else: append data files
				fseek(dateizeiger,0,SEEK_END);
							
				fprintf(dateizeiger, "%d;", parameter[0].repeati);
				fprintf(dateizeiger, "%d;", aktortyworldcoo);
				fprintf(dateizeiger, "%d;", aktortxworldcoo);
				fprintf(dateizeiger, "%d;", parameter[0].ivort);
				fprintf(dateizeiger, "%d;", t);
				fprintf(dateizeiger, "%d;", jahr);

		
		
				for (list<Tree*>::iterator pos = tree_list.begin(); pos != tree_list.end(); )
				{ // tree_list loop BEGIN
					pTree=(*pos);
				
					if ( (pTree->xcoo>=xminwindow) && (pTree->xcoo<=xmaxwindow) && (pTree->ycoo>=yminwindow) && (pTree->ycoo<=ymaxwindow) )
					{ // sector BEGIN
					
						if(pTree->species == 1)
						{
							// grouping ages
							if (pTree->age == 0)	{ageg0++;}
							else if (pTree->age == 1)	{ageg1++;}
							else if (pTree->age == 2)	{ageg2++;}
							else if (pTree->age == 3)	{ageg3++;}
							else if (pTree->age == 4)	{ageg4++;}
							else if (pTree->age == 5)	{ageg5++;}
							else if ( (pTree->age > 5) && (pTree->age <= 10) )	{ageg6b10++;}
							else if ( (pTree->age > 10) && (pTree->age <= 20) )	{ageg11b20++;}
							else if ( (pTree->age > 20) && (pTree->age <= 50) )	{ageg21b50++;}
							else if ( (pTree->age > 50) && (pTree->age <= 100) )	{ageg51b100++;}
							else if ( (pTree->age > 100) && (pTree->age <= 150) )	{ageg101b150++;}
							else if ( (pTree->age > 150) && (pTree->age <= 200) )	{ageg151b200++;}
							else if ( (pTree->age > 200) && (pTree->age <= 300) )	{ageg201b300++;}
							else if ( (pTree->age > 300) && (pTree->age <= 400) )	{ageg301b400++;}
							else if ( (pTree->age > 400) && (pTree->age <= 500) )	{ageg401b500++;}
							else if ( (pTree->age > 500) && (pTree->age <= 600) )	{ageg501b600++;}
							else if ( (pTree->age > 600) && (pTree->age <= 700) )	{ageg601b700++;}
							else if (pTree->age > 700)	{ageg701plus++;}
						}
						else if(pTree->species == 2)
						{
							// grouping ages
							if (pTree->age == 0)	{ages0++;}
							else if (pTree->age == 1)	{ages1++;}
							else if (pTree->age == 2)	{ages2++;}
							else if (pTree->age == 3)	{ages3++;}
							else if (pTree->age == 4)	{ages4++;}
							else if (pTree->age == 5)	{ages5++;}
							else if ( (pTree->age > 5) && (pTree->age <= 10) )	{ages6b10++;}
							else if ( (pTree->age > 10) && (pTree->age <= 20) )	{ages11b20++;}
							else if ( (pTree->age > 20) && (pTree->age <= 50) )	{ages21b50++;}
							else if ( (pTree->age > 50) && (pTree->age <= 100) )	{ages51b100++;}
							else if ( (pTree->age > 100) && (pTree->age <= 150) )	{ages101b150++;}
							else if ( (pTree->age > 150) && (pTree->age <= 200) )	{ages151b200++;}
							else if ( (pTree->age > 200) && (pTree->age <= 300) )	{ages201b300++;}
							else if ( (pTree->age > 300) && (pTree->age <= 400) )	{ages301b400++;}
							else if ( (pTree->age > 400) && (pTree->age <= 500) )	{ages401b500++;}
							else if ( (pTree->age > 500) && (pTree->age <= 600) )	{ages501b600++;}
							else if ( (pTree->age > 600) && (pTree->age <= 700) )	{ages601b700++;}
							else if (pTree->age > 700)	{ages701plus++;}
						}
						
						if (pTree->dbasal>0)
						{
							meanbas=meanbas+pTree->dbasal;
							h++;
						}

						// counting seeds
						gesamtseedAKT+=pTree->seednewly_produced;
						gesamtseedSUM+=pTree->seedproduced;
						
						//counting species
						if (pTree->species==1) 
                                                {
                                                    spectree1++;
                                                }
						else if (pTree->species==2) 
                                                {
                                                    spectree2++;
                                                }
						
						if (pTree->ycoo>yposmax) 
                                                {
                                                    yposmax=pTree->ycoo;
                                                }
					} // sector END
				
					++pos;
				} // tree_list loop END
			


				// for counting seeds:
				int seedconezahl=0, seedbodenzahl=0;
				// for counting species:
				int specseed1 = 0, specseed2 = 0;
				for(list<seed*>::iterator pos = seed_list.begin(); pos != seed_list.end(); pos++) 
				{
					pseed=(*pos);
					if ( (pseed->xcoo>=xminwindow) && (pseed->xcoo<=xmaxwindow) && (pseed->ycoo>=yminwindow) && (pseed->ycoo<=ymaxwindow) )
					{ // Sector BEGIN
						if (pseed->imcone==true) 
                                                {
                                                    seedconezahl++;
                                                }
						else 
                                                {
                                                    seedbodenzahl++;
                                                }
						// counting species:
						if (pseed->species == 1)
						{
							specseed1=specseed1+1;
						}
						else if (pseed->species == 2)
						{
							specseed2=specseed2+1;
						}						
					} // Sector END
				}
			
				if (h>0)
					fprintf(dateizeiger, "%4.4f;", meanbas/((double) h));
				else
					fprintf(dateizeiger, "%4.4f;", 0.0);
				fprintf(dateizeiger, "%4.4f;", pEvaluation->meanbreastdiameterliste[pEvaluation->meanbreastdiameterliste.size()-1]);
				// Treespezifika
				fprintf(dateizeiger, "%4.2f;",(xmaxwindow-xminwindow)*(ymaxwindow-yminwindow));
				fprintf(dateizeiger, "%d;", pEvaluation->nheight0b40liste[pEvaluation->nheight0b40liste.size()-1]);
				fprintf(dateizeiger, "%d;", pEvaluation->nheight41b200liste[pEvaluation->nheight41b200liste.size()-1]);
				fprintf(dateizeiger, "%d;", pEvaluation->nheight201b10000liste[pEvaluation->nheight201b10000liste.size()-1]);
				fprintf(dateizeiger, "%6.4f;", pEvaluation->BAliste[pEvaluation->BAliste.size()-1]);
				fprintf(dateizeiger, "%d;", pEvaluation->stemcountliste[pEvaluation->stemcountliste.size()-1]);
				fprintf(dateizeiger, "%6.4f;", pEvaluation->meantreeheightliste[pEvaluation->meantreeheightliste.size()-1]);
				fprintf(dateizeiger, "%6.4f;", pEvaluation->meantreeageliste[pEvaluation->meantreeageliste.size()-1]);
				fprintf(dateizeiger, "%d;", pEvaluation->wendejahr);
				fprintf(dateizeiger, "%d;", pEvaluation->saettigungsjahr);
				fprintf(dateizeiger, "%d;", ageg0);
				fprintf(dateizeiger, "%d;", ages0);
				fprintf(dateizeiger, "%d;", ageg1);
				fprintf(dateizeiger, "%d;", ages1);
				fprintf(dateizeiger, "%d;", ageg2);
				fprintf(dateizeiger, "%d;", ages2);
				fprintf(dateizeiger, "%d;", ageg3);
				fprintf(dateizeiger, "%d;", ages3);
				fprintf(dateizeiger, "%d;", ageg4);
				fprintf(dateizeiger, "%d;", ages4);
				fprintf(dateizeiger, "%d;", ageg5);
				fprintf(dateizeiger, "%d;", ages5);
				fprintf(dateizeiger, "%d;", ageg6b10);
				fprintf(dateizeiger, "%d;", ages6b10);
				fprintf(dateizeiger, "%d;", ageg11b20);
				fprintf(dateizeiger, "%d;", ages11b20);
				fprintf(dateizeiger, "%d;", ageg21b50);
				fprintf(dateizeiger, "%d;", ages21b50);
				fprintf(dateizeiger, "%d;", ageg51b100);
				fprintf(dateizeiger, "%d;", ages51b100);
				fprintf(dateizeiger, "%d;", ageg101b150);
				fprintf(dateizeiger, "%d;", ages101b150);
				fprintf(dateizeiger, "%d;", ageg151b200);
				fprintf(dateizeiger, "%d;", ages151b200);
				fprintf(dateizeiger, "%d;", ageg201b300);
				fprintf(dateizeiger, "%d;", ages201b300);
				fprintf(dateizeiger, "%d;", ageg301b400);
				fprintf(dateizeiger, "%d;", ages301b400);
				fprintf(dateizeiger, "%d;", ageg401b500);
				fprintf(dateizeiger, "%d;", ages401b500);
				fprintf(dateizeiger, "%d;", ageg501b600);
				fprintf(dateizeiger, "%d;", ages501b600);
				fprintf(dateizeiger, "%d;", ageg601b700);
				fprintf(dateizeiger, "%d;", ages601b700);
				fprintf(dateizeiger, "%d;", ageg701plus);
				fprintf(dateizeiger, "%d;", ages701plus);
				fprintf(dateizeiger, "%d;", gesamtseedAKT);
				fprintf(dateizeiger, "%d;", gesamtseedSUM);
				fprintf(dateizeiger, "%d;", spectree1);
				fprintf(dateizeiger, "%d;", spectree2);					
				fprintf(dateizeiger, "%d;", seedbodenzahl);
				fprintf(dateizeiger, "%d;", seedconezahl);
				fprintf(dateizeiger, "%d;", specseed1);
				fprintf(dateizeiger, "%d;", specseed2);				
				fprintf(dateizeiger, "%4.2f;",yposmax);
				// weatherspezifika
				fprintf(dateizeiger, "%d;", parameter[0].weatherchoice);
				fprintf(dateizeiger, "%d;", parameter[0].starttrees);
				fprintf(dateizeiger, "%4.4f;", weather_list[yearposition]->tempjahrmittel);
				fprintf(dateizeiger, "%4.4f;", weather_list[yearposition]->temp1monatmittel);
				fprintf(dateizeiger, "%4.4f;", weather_list[yearposition]->temp7monatmittel);
				fprintf(dateizeiger, "%d;", weather_list[yearposition]->vegetationslaenge);
				fprintf(dateizeiger, "%4.4f;", weather_list[yearposition]->activeairtemp);
				fprintf(dateizeiger, "%4.4f;", weather_list[yearposition]->degreday);
				fprintf(dateizeiger, "%4.2f;", weather_list[yearposition]->niederschlagssumme);
				fprintf(dateizeiger, "%4.4f;", weather_list[yearposition]->weatherfactorg);
				fprintf(dateizeiger, "%4.4f;", weather_list[yearposition]->weatherfactors);		
				fprintf(dateizeiger, "%d;", parameter[0].feuerausgabe);
				fprintf(dateizeiger, "%d;", parameter[0].thawing_depth);
			
				// End
				fprintf(dateizeiger, "\n");
				
				fclose(dateizeiger);

				//StefanC: Additional console output
				if (parameter[0].yearlyvis ==true)
				{
					cout << endl << "\tBasalarea = " << pEvaluation->BAliste[pEvaluation->BAliste.size()-1] << endl;
					cout << "\tN0-40 = " << nheight0b40 << "\tN40-200 = " << nheight41b200 << "\tN200+ = " << nheight201b10000 << endl;
					cout << "tree_list.size() bei Data_output = " << tree_list.size() << endl;
					cout << "\tNseeds:\tproduced = " << gesamtseedAKT << "\tground = " << seedbodenzahl << "\tcones = " << seedconezahl << endl;
				}
				
			}//spin up phase data output END
			
			// -- -- -- -- -- -- -- trees Currencies -- -- -- -- -- -- -- //
			// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
			// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
		}//currencies output
		
		
		if (ausgabeposition==true)
		{//position output
			// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
			// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
			// -- -- -- -- -- -- - trees Position -- -- -- -- -- -- -- -- //

			if ( parameter[0].ivort==1 || (parameter[0].einschwingen==true && (parameter[0].ivort%10)==0) || (parameter[0].einschwingen==false) )
			{	

				// assemble file name
				if (parameter[0].einschwingen==true)
				{
					s1<<parameter[0].repeati;
					s2<<parameter[0].weatherchoice;
					s3<<parameter[0].ivort;
					dateiname="output/dataentwicklung_positionsanalyse_" + s1.str()+ "_" + s2.str() + "_" + s3.str() + ".csv";
					s1.str("");s1.clear();
					s2.str("");s2.clear();
					s3.str("");s3.clear();
				}
				else 
				{	
					s1<<parameter[0].repeati;
					s2<<parameter[0].weatherchoice;
					s3<<parameter[0].ivort;
					dateiname="output/datatrees_positionsanalyse_" + s1.str() + "_" + s2.str() + "_" + s3.str() + ".csv";
					
					if (parameter[0].outputmode==0 || parameter[0].outputmode==3 || parameter[0].outputmode==4)
					{
						dateiname="output/datatrees_positionsanalyse_" + s1.str() + "_" + s2.str() + "_" + s3.str() + ".csv";
					}
					else if (parameter[0].outputmode==1) // hier weatherchoice nicht berücksichtigt! 	
					{
						if ( (pEvaluation->nachwendejahr==true) && (pEvaluation->saettigungsjahr==-9999) )
						{
							dateiname="output/datatrees_positionsanalyse_WENDEJAHR.csv";
						}
						else if (pEvaluation->saettigungsjahr!=-9999)
						{
							dateiname="output/datatrees_positionsanalyse_SAETTIGUNGSJAHR.csv";
						}
					}
					
					s1.str("");s1.clear();
					s2.str("");s2.clear();
					s3.str("");s3.clear();
				}

				if(parameter[0].periodRB==3)//Transect
				{
					// Trying to open the file for reading
					dateizeiger = fopen (dateiname.c_str(), "r+");
					// if fopen fails, open a new file + header output
					if (dateizeiger == NULL)
					{
					  dateizeiger = fopen (dateiname.c_str(), "w+");
						fprintf(dateizeiger, "Name;");
						fprintf(dateizeiger, "NameM;");
						fprintf(dateizeiger, "DBasal;");
						fprintf(dateizeiger, "DBrust;");
						fprintf(dateizeiger, "height;");
						fprintf(dateizeiger, "age;");
						fprintf(dateizeiger, "X;");
						fprintf(dateizeiger, "Y;");
						fprintf(dateizeiger, "densitywert;");
						fprintf(dateizeiger, "Generation;");
						// fprintf(dateizeiger, "coneheight;");
						fprintf(dateizeiger, "seedprodAKT;");
						fprintf(dateizeiger, "seedprodSUM;");
						// fprintf(dateizeiger, "thawing_depth_infl;");
						fprintf(dateizeiger, "\n");

						if (dateizeiger == NULL)
						{
							fprintf(stderr, "Error: File could not be opened!\n");
							exit(1);
						}
					}

					// else: append new data to the file
					fseek(dateizeiger,0,SEEK_END);

					for (list<Tree*>::iterator pos = tree_list.begin(); pos != tree_list.end(); )
					{ // tree_list loop BEGIN
						pTree=(*pos);
						
						// if ( (pTree->xcoo>=xminwindow) && (pTree->xcoo<=xmaxwindow) && (pTree->ycoo>=yminwindow) && (pTree->ycoo<=ymaxwindow) )
						// { // Sector BEGIN
							// fprintf(dateizeiger, "%d;", parameter[0].repeati);
							// fprintf(dateizeiger, "%d;", pTree->yworldcoo);
							// fprintf(dateizeiger, "%d;", pTree->xworldcoo);
							// fprintf(dateizeiger, "%d;", parameter[0].ivort);
							// fprintf(dateizeiger, "%d;", jahr);
							// fprintf(dateizeiger, "%d;", parameter[0].weatherchoice);
							fprintf(dateizeiger, "%d;", pTree->name);
							fprintf(dateizeiger, "%d;", pTree->namem);
							fprintf(dateizeiger, "%4.4f;", pTree->dbasal);
							fprintf(dateizeiger, "%4.4f;", pTree->dbrust);
							fprintf(dateizeiger, "%4.4f;", pTree->height);
							fprintf(dateizeiger, "%d;", pTree->age);
							fprintf(dateizeiger, "%4.4f;", pTree->xcoo);
							fprintf(dateizeiger, "%4.4f;", pTree->ycoo);
							fprintf(dateizeiger, "%4.5f;", pTree->densitywert);
							fprintf(dateizeiger, "%d;", pTree->generation);
							// fprintf(dateizeiger, "%4.4f;", pTree->coneheight);
							fprintf(dateizeiger, "%d;", pTree->seednewly_produced);
							fprintf(dateizeiger, "%d;", pTree->seedproduced);
							// fprintf(dateizeiger, "%lf;", pTree->thawing_depthinfluence);
							fprintf(dateizeiger, "\n");
						// } // Sector END

						++pos;
					} // tree_list loop END

					fclose(dateizeiger);
				} 
				else
				{
					// Trying to open the file for reading
					dateizeiger = fopen (dateiname.c_str(), "r+");
					// if fopen fails, open a new file + header output
					if (dateizeiger == NULL)
					{
					  dateizeiger = fopen (dateiname.c_str(), "w+");
						fprintf(dateizeiger, "Repeat;");
						fprintf(dateizeiger, "YPLOTPOS;");
						fprintf(dateizeiger, "XPLOTPOS;");
						fprintf(dateizeiger, "Jahr;");
						fprintf(dateizeiger, "zufallsJahr;");
						fprintf(dateizeiger, "weatherchoice;");
						fprintf(dateizeiger, "DBasal;");
						fprintf(dateizeiger, "DBrust;");
						fprintf(dateizeiger, "height;");
						fprintf(dateizeiger, "age;");
						fprintf(dateizeiger, "X;");
						fprintf(dateizeiger, "Y;");
						fprintf(dateizeiger, "densitywert;");
						fprintf(dateizeiger, "Generation;");
						fprintf(dateizeiger, "coneheight;");
						fprintf(dateizeiger, "seedprodAKT;");
						fprintf(dateizeiger, "seedprodSUM;");
						fprintf(dateizeiger, "thawing_depth_infl;");
						fprintf(dateizeiger, "\n");


						if (dateizeiger == NULL)
						{
							fprintf(stderr, "Fehler: Datei konnte nicht geoeffnet werden!\n");
							exit(1);
						}
					}

					// else: append new data to the file
					fseek(dateizeiger,0,SEEK_END);

					for (list<Tree*>::iterator pos = tree_list.begin(); pos != tree_list.end(); )
					{ // tree_list loop BEGIN
						pTree=(*pos);
						
						// if ( (pTree->xcoo>=xminwindow) && (pTree->xcoo<=xmaxwindow) && (pTree->ycoo>=yminwindow) && (pTree->ycoo<=ymaxwindow) )
						// { // Sector BEGIN
							fprintf(dateizeiger, "%d;", parameter[0].repeati);
							fprintf(dateizeiger, "%d;", pTree->yworldcoo);
							fprintf(dateizeiger, "%d;", pTree->xworldcoo);
							fprintf(dateizeiger, "%d;", parameter[0].ivort);
							fprintf(dateizeiger, "%d;", jahr);
							fprintf(dateizeiger, "%d;", parameter[0].weatherchoice);
							fprintf(dateizeiger, "%4.4f;", pTree->dbasal);
							fprintf(dateizeiger, "%4.4f;", pTree->dbrust);
							fprintf(dateizeiger, "%4.4f;", pTree->height);
							fprintf(dateizeiger, "%d;", pTree->age);
							fprintf(dateizeiger, "%4.4f;", pTree->xcoo);
							fprintf(dateizeiger, "%4.4f;", pTree->ycoo);
							fprintf(dateizeiger, "%4.5f;", pTree->densitywert);
							fprintf(dateizeiger, "%d;", pTree->generation);
							fprintf(dateizeiger, "%4.4f;", pTree->coneheight);
							fprintf(dateizeiger, "%d;", pTree->seednewly_produced);
							fprintf(dateizeiger, "%d;", pTree->seedproduced);
							fprintf(dateizeiger, "%lf;", pTree->thawing_depthinfluence);
							fprintf(dateizeiger, "\n");
						// } // Sector END

						++pos;
					} // tree_list loop END

					fclose(dateizeiger);
				}
				
			}//modulo
			
			// -- -- -- -- -- -- - trees Position -- -- -- -- -- -- -- -- //
			// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
			// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
		}//position output

		if (ausgabeindividuen==true)
		{//ausgabeindividuen
			// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
			// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
			// -- -- -- -- -- BEGIN  individual    trees -- -- -- -- -- -- //

			// assemble file name
			if (parameter[0].outputmode!=1)
			{
					s1<<parameter[0].repeati;
					s2<<parameter[0].weatherchoice;
					s3<<parameter[0].ivort;
					dateiname="output/datatrees_" + s1.str()+ "_" + s2.str() + "_" + s3.str() + ".csv";
					s1.str("");s1.clear();
					s2.str("");s2.clear();
					s3.str("");s3.clear();
			}
			else if (parameter[0].outputmode==1)
			{
				if ( (pEvaluation->nachwendejahr==true) && (pEvaluation->saettigungsjahr==-9999) )
				{
					dateiname="output/datatrees_WENDEJAHR.csv";
				}
				else if (pEvaluation->saettigungsjahr!=-9999)
				{
					dateiname="output/datatrees_SAETTIGUNGSJAHR.csv";
				}
			}

			// Trying to open the file for reading
			dateizeiger = fopen (dateiname.c_str(), "r+");
			// if fopen fails, open a new file + header output
			if (dateizeiger == NULL)
			{
				dateizeiger = fopen (dateiname.c_str(), "w+");
		
				// parameters
				fprintf(dateizeiger, "Repeat;");
				fprintf(dateizeiger, "YPLOTPOS;");
				fprintf(dateizeiger, "XPLOTPOS;");
				fprintf(dateizeiger, "weatherchoice;");
				// time variables
				fprintf(dateizeiger, "Zeitschritt;");
				fprintf(dateizeiger, "Jahr;");
				// tree variables
				fprintf(dateizeiger, "X;");
				fprintf(dateizeiger, "Y;");
				fprintf(dateizeiger, "Name;");
				fprintf(dateizeiger, "NameM;");
				fprintf(dateizeiger, "NameP;");
				fprintf(dateizeiger, "line;");
				fprintf(dateizeiger, "Generation;");
				fprintf(dateizeiger, "Art;");
				fprintf(dateizeiger, "height;");
				fprintf(dateizeiger, "Dbasal;");
				fprintf(dateizeiger, "Dbrust;");
				fprintf(dateizeiger, "age;");
				fprintf(dateizeiger, "cone;");
				fprintf(dateizeiger, "coneheight;");
				fprintf(dateizeiger, "seedprodAKT;");
				fprintf(dateizeiger, "seedprodSUM;");
				fprintf(dateizeiger, "Speicher;");
				fprintf(dateizeiger, "densitywert;");
				fprintf(dateizeiger, "Entfernung;");
				fprintf(dateizeiger, "thawing_depth_infl;");	
				
				// End
				fprintf(dateizeiger, "\n");

				if (dateizeiger == NULL)
				{
					fprintf(stderr, "Error: File could not be opened!\n");
					exit(1);
				}
			}

			// else: append new data to the file
			fseek(dateizeiger,0,SEEK_END);

			// Data output for each tree
			for (list<Tree*>::iterator pos = tree_list.begin(); pos != tree_list.end(); )
			{ // tree_list loop BEGIN
				pTree=(*pos);
				// parameters
				fprintf(dateizeiger, "%d;", parameter[0].repeati);
				fprintf(dateizeiger, "%d;", pTree->yworldcoo);
				fprintf(dateizeiger, "%d;", pTree->xworldcoo);
				fprintf(dateizeiger, "%d;", parameter[0].weatherchoice);
				// time variables
				fprintf(dateizeiger, "%d;", t);
				fprintf(dateizeiger, "%d;", jahr);
				// tree variables
				fprintf(dateizeiger, "%4.4f;", pTree->xcoo);
				fprintf(dateizeiger, "%4.4f;", pTree->ycoo);
				fprintf(dateizeiger, "%d;", pTree->name);
				fprintf(dateizeiger, "%d;", pTree->namem);
				fprintf(dateizeiger, "%d;", pTree->namep);
				fprintf(dateizeiger, "%d;", pTree->line);
				fprintf(dateizeiger, "%d;", pTree->generation);
				fprintf(dateizeiger, "%d;", pTree->species);
				fprintf(dateizeiger, "%4.4f;", pTree->height);
				fprintf(dateizeiger, "%4.4f;", pTree->dbasal);
				fprintf(dateizeiger, "%4.4f;", pTree->dbrust);
				fprintf(dateizeiger, "%d;", pTree->age);
				fprintf(dateizeiger, "%d;", pTree->cone);
				fprintf(dateizeiger, "%4.4f;", pTree->coneheight);
				fprintf(dateizeiger, "%d;", pTree->seednewly_produced);
				fprintf(dateizeiger, "%d;", pTree->seedproduced);
				fprintf(dateizeiger, "%d;", pTree->speicher);
				fprintf(dateizeiger, "%4.5f;", pTree->densitywert);
				fprintf(dateizeiger, "%4.5f;", pTree->entfernung);
				fprintf(dateizeiger, "%lf;", pTree->thawing_depthinfluence);
				// End
				fprintf(dateizeiger, "\n");

				++pos;
			} // tree_list loop End

			fclose (dateizeiger);

			// -- -- -- -- --  END  individual  trees    -- -- -- -- -- -- //
			// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
			// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
		}//individual output
		

		if (ausgabedendro==true)
		{//ausgabedendro
			// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
			// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
			// -- -- - BEGIN  dendrochronology growth data     -- -- -- -- //
			
			bool alletreesausgeben=false;

			// read in tree_list and remember the positions of certain trees
			vector<int> Gueltigetrees;
			int aktposTree=0;
			for (list<Tree*>::iterator pos = tree_list.begin(); pos != tree_list.end(); )
			{ // tree_list loop BEGIN
				pTree=(*pos);

				if ( (pTree->height >= 200) && ( (pTree->xcoo>=xminwindow) && (pTree->xcoo<=xmaxwindow) ) && ( (pTree->ycoo>=yminwindow) && (pTree->ycoo<=ymaxwindow) ) ) 
				{
					Gueltigetrees.push_back(aktposTree);
				}
			
				++aktposTree;
				++pos;
			} // tree_list loop END

			// Determine the number of the remaining trees
			int listlength=Gueltigetrees.size();
			for (vector<int>::iterator posit=Gueltigetrees.begin(); posit<Gueltigetrees.end(); )
			{ // Gueltigetrees loop BEGIN

				bool schreiben=false;
				if (alletreesausgeben==true)
				{
					schreiben=true;
				}
				else
				{
					if (listlength<100) 
                    {
                        schreiben=true;
                    }
					else 
					{
						if (*posit<=30) 
                        {
                            schreiben=true;
                        }
						else if (*posit>=(listlength-30) ) 
                        {
                            schreiben=true;
                        }
						else if (*posit>(listlength/2)-15 && *posit<(listlength/2)+15) 
                        {
                            schreiben=true;
                        }
					}	
				}

				// Schreibe die Jahreszuwachsleistungen
				if (schreiben==true)
				{ // Data output BEGIN
					list<Tree*>::iterator pos = tree_list.begin();
					advance(pos, (int) *posit);
					pTree=(*pos);

					// assemble file name
					if (parameter[0].outputmode==0 || parameter[0].outputmode==2 || parameter[0].outputmode==3 || parameter[0].outputmode==4)
					{
						s1<<jahr;s2<<pTree->xworldcoo;s3<<pTree->yworldcoo;s4<<parameter[0].repeati;
						s5<<pTree->species;s6<<parameter[0].weatherchoice;s7<<parameter[0].thawing_depth;
						
						dateiname="output/datatrees_Zuwachs/datatrees_Jahr" +s1.str()+"_X"+s2.str()+"_Y"+s3.str()+"_WDH"+s4.str()+"_SP"+s5.str()+"_weather"+s6.str()+"_thawing_depth"+s7.str()+ ".fh";
						
						s1.str("");s1.clear();
						s2.str("");s2.clear();
						s3.str("");s3.clear();
						s4.str("");s4.clear();
						s5.str("");s5.clear();
						s6.str("");s6.clear();
						s7.str("");s7.clear();
					}
					else if (parameter[0].outputmode==1)
					{
						if ( (pEvaluation->nachwendejahr==true) && (pEvaluation->saettigungsjahr==-9999) )
						{
							s1<<parameter[0].repeati;s2<<pTree->species;s3<<parameter[0].weatherchoice;
							dateiname="output/datatrees_Zuwachs_WENDEJAHR/datatrees_WDH" + s1.str()+"_SP"+s2.str()+"_weather"+s3.str() + ".fh";
							s1.str("");s1.clear();s2.str("");s2.clear();s3.str("");s3.clear();
						}
						else if (pEvaluation->saettigungsjahr!=-9999)
						{
							s1<<parameter[0].repeati;s2<<pTree->species;s3<<parameter[0].weatherchoice;
							dateiname="output/datatrees_Zuwachs_SAETTIGUNGSJAHR/datatrees_WDH" + s1.str()+"_SP"+s2.str()+"_weather"+s3.str() + ".fh";
							s1.str("");s1.clear();s2.str("");s2.clear();s3.str("");s3.clear();
						}
					}
			

					if (parameter[0].ordnerda==false)
					{
						if (parameter[0].outputmode==0 || parameter[0].outputmode==2 || parameter[0].outputmode==3 || parameter[0].outputmode==4)
						{
							system("mkdir datatrees_Zuwachs");
							parameter[0].ordnerda=true;
						}
						else if (parameter[0].outputmode==1)
						{
							system("mkdir datatrees_Zuwachs_WENDEJAHR");
							system("mkdir datatrees_Zuwachs_SAETTIGUNGSJAHR");
							parameter[0].ordnerda=true;
						}
					}

					// Trying to open the file for reading
					dateizeiger = fopen (dateiname.c_str(), "r+");
					// if fopen fails, open a new file + header output
					if (dateizeiger == NULL)
					{
						dateizeiger = fopen (dateiname.c_str(), "w+");

						if (dateizeiger == NULL)
						{
							fprintf(stderr, "Error: File could not be opened!\n");
							exit(1);
						}
					}
					// the new data gets appended to the file
					fseek(dateizeiger,0,SEEK_END);

					fprintf(dateizeiger, "HEADER:\n");
					fprintf(dateizeiger, "DataFormat=Tree\n");
										fprintf(dateizeiger, "DateEnd=%d\n", jahr);		//2011	
					fprintf(dateizeiger, "KeyCode=%d\n", pTree->name);		//TreeA 1_2	
					fprintf(dateizeiger, "Location=X%d-Y%d\n", pTree->xworldcoo, pTree->yworldcoo);	//11-CH-02/II	
					fprintf(dateizeiger, "GlobalMathCommentCount=0\n");
					fprintf(dateizeiger, "ImageCount=0\n");
					fprintf(dateizeiger, "CommentCount=0\n");
					fprintf(dateizeiger, "BibliographyCount=0\n");
					fprintf(dateizeiger, "Unit=1/100 mm\n");
					fprintf(dateizeiger, "Dated=Dated\n");
					fprintf(dateizeiger, "DATA:Single\n");

					if (dateizeiger == NULL)
					{
						fprintf(stderr, "Error: File could not be opened!\n");
						exit(1);
					}

					// the new data gets appended to the file
					fseek(dateizeiger,0,SEEK_END);
					fclose(dateizeiger);
				} // Data output END

				++posit;
			} // // Gueltigetrees loop END

			// -- -- --  END dendrochronology growth rates     -- -- -- -- //
			// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
			// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
		}//ausgabedendro

		if (ausgabedensity==true)
		{//ausgabedensity
			// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
			// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
			// -- -- -- -- -- -- -- --Tree density -- -- -- -- -- -- -- -- //

			// assemble file name:
			if (parameter[0].outputmode==0 || parameter[0].outputmode==2 || parameter[0].outputmode==3 || parameter[0].outputmode==4)
			{
				s1<<jahr;s2<<parameter[0].weatherchoice;
				dateiname="output/datatrees_Treedensity" +s1.str()+"_"+s2.str()+ ".csv";
				s1.str("");s1.clear();s2.str("");s2.clear();
			}
			else if (parameter[0].outputmode==1) //hier weatherchoice nicht berücksichtigt
			{
				if ( (pEvaluation->nachwendejahr==true) && (pEvaluation->saettigungsjahr==-9999) )
				{
					dateiname="output/datatrees_Treedensity_WENDEJAHR.csv";
				}
				else if (pEvaluation->saettigungsjahr!=-9999)
				{
					dateiname="output/datatrees_Treedensity_SAETTIGUNGSJAHR.csv";
				}
			}
				
			// Trying to open the file for reading
			dateizeiger = fopen (dateiname.c_str(), "r+");
			// if fopen fails, open a new file + header output
			if (dateizeiger == NULL)
			{
			  dateizeiger = fopen (dateiname.c_str(), "w+");
				fprintf(dateizeiger, "Repeat;");
				fprintf(dateizeiger, "YPLOTPOS;");
				fprintf(dateizeiger, "XPLOTPOS;");
				fprintf(dateizeiger, "X;");
				fprintf(dateizeiger, "Y;");
				fprintf(dateizeiger, "Treedensitywert;");
				fprintf(dateizeiger, "Treeanzahl;");
				fprintf(dateizeiger, "Auflagenstaerke;");
				fprintf(dateizeiger, "AuflagenstaerkeMittel;");
				fprintf(dateizeiger, "Maxthawing_depth;");
				fprintf(dateizeiger, "weatherchoice;");
				fprintf(dateizeiger, "thawing_depth;");
				fprintf(dateizeiger, "\n");

				if (dateizeiger == NULL)
				{
					fprintf(stderr, "Fehler: Datei konnte nicht geoeffnet werden!\n");
					exit(1);
				}
			}

			// the new data gets appended to the file
			fseek(dateizeiger,0,SEEK_END);

			// Data evaluation and output
			for (int kartenpos=0; kartenpos< (treerows*parameter[0].sizemagnif*treecols*parameter[0].sizemagnif); kartenpos++)
			{ //plot list loop BEGIN
				pKarten=plot_list[kartenpos];
				if ( (pKarten->Treeanzahl>0) && ( (pKarten->xcoo>=xminwindow*parameter[0].sizemagnif) && (pKarten->xcoo<=xmaxwindow*parameter[0].sizemagnif) && (pKarten->ycoo>=yminwindow*parameter[0].sizemagnif) && (pKarten->ycoo<=ymaxwindow*parameter[0].sizemagnif) ) )
				{ // perform output only if tree density values >0 in the evaluation sector BEGIN
					fprintf(dateizeiger, "%d;", parameter[0].repeati);
					fprintf(dateizeiger, "%d;", pKarten->yworldcoo);
					fprintf(dateizeiger, "%d;", pKarten->xworldcoo);
					fprintf(dateizeiger, "%4.4f;", pKarten->xcoo);
					fprintf(dateizeiger, "%4.4f;", pKarten->ycoo);
					fprintf(dateizeiger, "%4.5f;", pKarten->Treedensitywert);
					fprintf(dateizeiger, "%d;", pKarten->Treeanzahl);
					fprintf(dateizeiger, "%u;", pKarten->auflagenstaerke);
					fprintf(dateizeiger, "%u;", pKarten->auflagenstaerkemittel);
					fprintf(dateizeiger, "%u;", pKarten->maxthawing_depth);
					fprintf(dateizeiger, "%d;", parameter[0].weatherchoice);
					fprintf(dateizeiger, "%d;", parameter[0].thawing_depth);
					fprintf(dateizeiger, "\n");
				} // perform output only if tree density values >0 in the evaluation sector END
			} // plot list loop END

			fclose(dateizeiger);

			// -- -- -- -- -- -- -- --Tree density -- -- -- -- -- -- -- -- //
			// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
			// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
		}//ausgabedensity

		
		
		//delete everything if just sensitivity analysis output is set
		if ( (parameter[0].outputmode==1) && (pEvaluation->saettigungsjahr!=-9999) )
		{
			//Calling the function to delete lists
				// tree_listn loeschen
			for (list<Tree*>::iterator pos = tree_list.begin(); pos != tree_list.end(); ++pos)
			{ // Begin tree_list iteration
				pTree=(*pos);
				//pTree->Dbasalliste.clear();
				//pTree->Dbrustliste.clear();
				delete pTree;
			} // End tree_list iteration
			tree_list.clear();
			
	
				// seed_listn loeschen
			for (list<seed*>::iterator pos = seed_list.begin(); pos != seed_list.end(); ++pos)
			{ // Begin seed_list iteration
				pseed=(*pos);
				delete pseed;
			} // End seed_list iteration
			seed_list.clear();
			
		}
		
	} // world tree list loop END

}
