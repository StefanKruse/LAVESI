/****************************************************************************************//**
 * \brief calculate max. possible basal growth
 *
 * either just set to weather_list[yearposition]->weatherfactor * exponential growth 
 * depending on dbh
 * or, if transekts are calculated, calculated out of weather_list[yearposition]->weatherfactormin 
 * (max basal growth at most southern transekt and tree position in north-south transekt
 *
 *******************************************************************************************/
double getMaxbasalwachstum(int yearposition, list<Tree*> &tree_list, vector<weather*> &weather_list)
{ 
	double maxbw_help = 0;
	if (parameter[0].lineartransekt==true)
	{
		if(parameter[0].thawing_depth==true)
		{
			if(pTree->species==1)
			{
				maxbw_help = exp(parameter[0].gdbasalconstgmel+parameter[0].gdbasalfacgmel*pTree->dbasal+parameter[0].gdbasalfacqgmel*pTree->dbasal*pTree->dbasal)*
								(((weather_list[yearposition]->weatherfactorg
									-weather_list[yearposition]->weatherfactorming)
									/((double) treerows))*pTree->ycoo 
								+weather_list[yearposition]->weatherfactorming)
								* (((double) pTree->thawing_depthinfluence)/100 );		
			}
			else if(pTree->species==2)
			{
				maxbw_help=exp(parameter[0].gdbasalconstsib+parameter[0].gdbasalfacsib*pTree->dbasal+parameter[0].gdbasalfacqsib*pTree->dbasal*pTree->dbasal)*
								(((weather_list[yearposition]->weatherfactors
									-weather_list[yearposition]->weatherfactormins)
									/((double) treerows))*pTree->ycoo 
								+weather_list[yearposition]->weatherfactormins)
								* ((((double) pTree->thawing_depthinfluence*0.8)/100 )-0.6);	// Angepasster thawing_depthinfluence für Lsibirica, damit analog zu Lgmelinii 20% des Wachstum bei 20% der minimal benötigten thawing_depth stattfinden
			}		
		}
		else
		{
			if(pTree->species==1)
			{
				maxbw_help = exp(parameter[0].gdbasalconstgmel+parameter[0].gdbasalfacgmel*pTree->dbasal+parameter[0].gdbasalfacqgmel*pTree->dbasal*pTree->dbasal)*
								(((weather_list[yearposition]->weatherfactorg
									-weather_list[yearposition]->weatherfactorming)
									/((double) treerows))*pTree->ycoo 
								+weather_list[yearposition]->weatherfactorming);				
			}
			else if(pTree->species==2)
			{
				maxbw_help=exp(parameter[0].gdbasalconstsib+parameter[0].gdbasalfacsib*pTree->dbasal+parameter[0].gdbasalfacqsib*pTree->dbasal*pTree->dbasal)*
								(((weather_list[yearposition]->weatherfactors
									-weather_list[yearposition]->weatherfactormins)
									/((double) treerows))*pTree->ycoo 
								+weather_list[yearposition]->weatherfactormins);
			}	
		}
	}
	else
	{
		if(parameter[0].thawing_depth==true)
		{
			if(pTree->species==1)
			{
				maxbw_help = exp(parameter[0].gdbasalconstgmel+parameter[0].gdbasalfacgmel*pTree->dbasal+parameter[0].gdbasalfacqgmel*pTree->dbasal*pTree->dbasal)*
							weather_list[yearposition]->weatherfactorg* (((double) pTree->thawing_depthinfluence)/100 );
			}
			else if(pTree->species==2)
			{
				maxbw_help = exp(parameter[0].gdbasalconstsib+parameter[0].gdbasalfacsib*pTree->dbasal+parameter[0].gdbasalfacqsib*pTree->dbasal*pTree->dbasal)*
							weather_list[yearposition]->weatherfactors* ((((double) pTree->thawing_depthinfluence*0.8)/100 )-0.6);	// Angepasster thawing_depthinfluence für Lsibirica, damit analog zu Lgmelinii 20% des Wachstum bei 20% der minimal benötigten thawing_depth stattfinden
			}
		}
		else
		{
			if(pTree->species==1)
			{
				maxbw_help = exp(parameter[0].gdbasalconstgmel+parameter[0].gdbasalfacgmel*pTree->dbasal+parameter[0].gdbasalfacqgmel*pTree->dbasal*pTree->dbasal)*
							weather_list[yearposition]->weatherfactorg;
			}
			else if(pTree->species==2)
			{
				maxbw_help = exp(parameter[0].gdbasalconstsib+parameter[0].gdbasalfacsib*pTree->dbasal+parameter[0].gdbasalfacqsib*pTree->dbasal*pTree->dbasal)*
							weather_list[yearposition]->weatherfactors;
			}
		}	
	}
	return (maxbw_help);
}



/****************************************************************************************//**
 * \brief calculate min. possible basal growth
 *
 * either taken from just set to weather_list[yearposition]->maxbrustwachstumjahr \n
 * or if transekts are calculated calculated out of weather_list[yearposition]->maxbasalwachstumjahrmin 
 * and tree position in north-south transekt
 *
 *******************************************************************************************/
double getMaxbrustwachstum(int yearposition, list<Tree*> &tree_list, vector<weather*> &weather_list)
{	
	double maxbrw_help = 0;
	//If it is switched on, the latest growth performance is calculated in dependence of the coordinate.
	
	//***german:
	// Falls eingestellt, die aktuelle Zuwachsleistung nach dem Ort ermitteln
	
	if (parameter[0].lineartransekt==true)
	{	
		if(parameter[0].thawing_depth==true)
		{
			if(pTree->species==1)
			{
				maxbrw_help = exp(parameter[0].gdbrustconstgmel+parameter[0].gdbrustfacgmel*pTree->dbrust+parameter[0].gdbrustfacqgmel*pTree->dbrust*pTree->dbrust)*
								(((weather_list[yearposition]->weatherfactorg
									-weather_list[yearposition]->weatherfactorming)
									/((double) treerows))*pTree->ycoo 
								+weather_list[yearposition]->weatherfactorming)
								* (((double) pTree->thawing_depthinfluence)/100 );				
			}
			else if(pTree->species==2)
			{
				maxbrw_help=exp(parameter[0].gdbrustconstsib+parameter[0].gdbrustfacsib*pTree->dbrust+parameter[0].gdbrustfacqsib*pTree->dbrust*pTree->dbrust)*
								(((weather_list[yearposition]->weatherfactors
									-weather_list[yearposition]->weatherfactormins)
									/((double) treerows))*pTree->ycoo 
								+weather_list[yearposition]->weatherfactormins)
								* ((((double) pTree->thawing_depthinfluence*0.8)/100 )-0.6);
			}
		}
		else
		{
			if(pTree->species==1)
			{
				maxbrw_help = exp(parameter[0].gdbrustconstgmel+parameter[0].gdbrustfacgmel*pTree->dbrust+parameter[0].gdbrustfacqgmel*pTree->dbrust*pTree->dbrust)*
								(((weather_list[yearposition]->weatherfactorg
									-weather_list[yearposition]->weatherfactorming)
									/((double) treerows))*pTree->ycoo 
								+weather_list[yearposition]->weatherfactorming);				
			}
			else if(pTree->species==2)
			{
				maxbrw_help=exp(parameter[0].gdbrustconstsib+parameter[0].gdbrustfacsib*pTree->dbrust+parameter[0].gdbrustfacqsib*pTree->dbrust*pTree->dbrust)*
								(((weather_list[yearposition]->weatherfactors
									-weather_list[yearposition]->weatherfactormins)
									/((double) treerows))*pTree->ycoo 
								+weather_list[yearposition]->weatherfactormins);
			}
		}
	}
	else
	{
		if(parameter[0].thawing_depth==true)
		{
			if(pTree->species==1)
			{
				maxbrw_help = exp(parameter[0].gdbrustconstgmel+parameter[0].gdbrustfacgmel*pTree->dbrust+parameter[0].gdbrustfacqgmel*pTree->dbrust*pTree->dbrust)*
							weather_list[yearposition]->weatherfactorg* (((double) pTree->thawing_depthinfluence)/100 );
			}
			else if(pTree->species==2)
			{
				maxbrw_help = exp(parameter[0].gdbrustconstsib+parameter[0].gdbrustfacsib*pTree->dbrust+parameter[0].gdbrustfacqsib*pTree->dbrust*pTree->dbrust)*
							weather_list[yearposition]->weatherfactors* ((((double) pTree->thawing_depthinfluence*0.8)/100 )-0.6);
			}
		}
		else
		{
			if(pTree->species==1)
			{
				maxbrw_help = exp(parameter[0].gdbrustconstgmel+parameter[0].gdbrustfacgmel*pTree->dbrust+parameter[0].gdbrustfacqgmel*pTree->dbrust*pTree->dbrust)*
							weather_list[yearposition]->weatherfactorg;
			}
			else if(pTree->species==2)
			{
				maxbrw_help = exp(parameter[0].gdbrustconstsib+parameter[0].gdbrustfacsib*pTree->dbrust+parameter[0].gdbrustfacqsib*pTree->dbrust*pTree->dbrust)*
							weather_list[yearposition]->weatherfactors;
			}	
		}
	}
	return(maxbrw_help);
}


/****************************************************************************************//**
 * \brief calculate basal and breast height growth of each tree in the simulation
 *
 * basalwachstum = maxbasalwachstum * densitywert * thawing_depthinfluence/100;	\n
 * basalwachstum = basalwachstum + basalwachstum * basaleinflussaltneu * dbasal; \n
 * brustwachstum = maxbrustwachstum * densitywert * thawing_depthinfluence)/100;  \n
 *
 *******************************************************************************************/
void Wachstum(int treerows, int treecols, struct Parameter *parameter, int yearposition, vector<list<Tree*> > &world_tree_list, vector<vector<weather*> > &world_weather_list, vector<vector<Karten*> > &world_plot_list)
{
	
	int aktort=0;
	for (vector<list<Tree*> >::iterator posw = world_tree_list.begin(); posw != world_tree_list.end(); ++posw)
	{ 
		list<Tree*>& tree_list = *posw;
	
		vector<vector<weather*> >::iterator posiwelt = (world_weather_list.begin()+aktort);
		vector<weather*>& weather_list = *posiwelt;

		aktort++;

		for (list<Tree*>::iterator pos = tree_list.begin(); pos != tree_list.end(); )
		{ 
			pTree=(*pos);

			double maxbasalwachstum = 0.0;

			maxbasalwachstum = getMaxbasalwachstum(yearposition, tree_list, weather_list);
			
			double basalwachstum = maxbasalwachstum
									* ( (double) pTree->speicher/* GELOESCHT "/3" */ ) //==1
									* (1.0-pTree->densitywert);
									
			basalwachstum  = basalwachstum
							+basalwachstum*parameter[0].basaleinflussaltneu
							*pTree->dbasal;
			if (basalwachstum<0.0)
			{
				basalwachstum=0.0;
			}
			
			if (pTree->growing==true)
			{
				pTree->dbasal+= basalwachstum;
			}		
			
			//The following formulas rely on assumptions or are interpolated from field data.
			//In the following the current basal growth is amplified by the current tree's dimension.
			//Through that healing growth is represented.
			//This also affects relative growth and all processes relying on that.
			
			
			//***german:
			//Formeln beruhen auf reinen Annahmen, bzw. Ableitung von Beobachtungen. 
			//Hier wird das aktuelle Basalwachstum durch die Eigendimension nochmals verstärkt. 
			//Dadurch soll Ausgleichswachstum representiert werden. 
			//Es wirkt sich dann auch das relative Wachstum und darauf aufbauende Prozesse aus.
					
			if (parameter[0].relwachstumeinfluss==0)
			{
				pTree->dbasalrel = 1.0;
			}
			else if (parameter[0].relwachstumeinfluss==1)
			{
					pTree->dbasalrel = basalwachstum
									/(maxbasalwachstum
									  +(maxbasalwachstum
									*parameter[0].basaleinflussaltneu
									*pTree->dbasal));
			}

			double maxbrustwachstum=0;
			double brustwachstum=0;

			if (pTree->height>=130.0)
			{ 
				maxbrustwachstum = getMaxbrustwachstum(yearposition, tree_list, weather_list);

				brustwachstum = maxbrustwachstum
										* ( (double) pTree->speicher/* GELOESCHT "/3" */ ) 
										* (1.0-pTree->densitywert);
										
				if (brustwachstum<0.0)
				{
					brustwachstum=0.0;
				}
				
				if (pTree->growing==true)
				{
					pTree->dbrust+= brustwachstum;
					//pTree->Dbrustliste.push_back(brustwachstum);
				}

				if (parameter[0].relwachstumeinfluss==0)
				{
					pTree->dbrustrel=1.0;
				}		
				else if (parameter[0].relwachstumeinfluss==1)
				{
					pTree->dbrustrel=brustwachstum/maxbrustwachstum;
				}
			} 
			
			// tree height update
			if (pTree->height<130.0)
			{
				if (parameter[0].allometriefunktionstyp==1)
				{
					pTree->height= parameter[0].dbasalheightallosteig * pow(pTree->dbasal, parameter[0].dbasalheightalloexp);
					//height from basal diameter.
				}
				else 
				{
					pTree->height= parameter[0].dbasalheightsteignonlin*pTree->dbasal;
				}
			}
			else if (pTree->height>=130.0)
			{
				if (parameter[0].allometriefunktionstyp==1)
				{
					pTree->height= parameter[0].dbrustheightallosteig * pow(pTree->dbrust, parameter[0].dbrustheightalloexp) + 130.0;
				}
				else 
				{
					pTree->height= pow( (parameter[0].dbrustheightsteignonlin*pow(pTree->dbrust,0.5)), 2)+130.0; 
				}
			}
			else
			{ 	
				
				//Safety routine: is the substructure height now defined?
				
				//***german:
				// Sicherheitsabfrage height-Variable gesetzt?  
				signed int abbrechenwachstumfehler; 
				printf("\n In growth.cpp a tree was not assigned a new height value.\n");
				printf("\n continue typing 1, press any other key to exit\n");
				
				//printf("\n In der Wachstumsfunktion hat ein Tree keinen Wert in der Variable pTree->height\n"); 
				//printf("\n Weiter mit 1, beenden mit irgendeiner Eingabe\n"); 
				scanf("%d", &abbrechenwachstumfehler); 
				if (abbrechenwachstumfehler!=1) 
				{
					printf("LAVESI was exited after an error occuring in growth.cccp\n");
					//printf("LaVeSi wurde nach einem Fehler in der Wachstumsfunktion beendet\n\n");
					exit(1);
				}
				delete pTree;
				pos=tree_list.erase(pos);						
			} 

			++pos;
		
		}
		

	}

}


