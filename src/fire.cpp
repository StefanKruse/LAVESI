/****************************************************************************************//**
 * \brief calculate fire
 *
 * three different kinds of fire, from low to high impact 
 * frequency approx. adapted from literature
 *******************************************************************************************/
void Fire(int treerows, int treecols, struct Parameter *parameter, int yearposition, vector<vector<Karten*> > &world_plot_list, vector<vector<weather*> > &world_weather_list)
{

	if (parameter[0].feuer==1 && parameter[0].einschwingen==false)
	{ 
		int aktort=0;
		
		for (vector<vector<Karten*> >::iterator posw = world_plot_list.begin(); posw != world_plot_list.end(); ++posw)
		{ 
			vector<Karten*>& plot_list = *posw;

			vector<vector<weather*> >::iterator posiwelt = (world_weather_list.begin()+aktort);
			vector<weather*>& weather_list = *posiwelt;

			aktort++;

			// Abhaengig vom weather wird eine Feuerausbruchswahrscheinlichkeit berechnet und 
			// da etwa 1.5 % der Flachen jaehrlich brennen wird die Chance von 0.015
			// mit der Auflagenstaerke verrechnet
			double zufallsz = 0.0 +( (double) 1.0*rand()/(RAND_MAX + 1.0));


			///starkes Feuer zerstört fast komplette´vegetationsauflage und erhöht die "Trockenheitsmortalitaet" um 3
			if (zufallsz< (weather_list[yearposition]->trockenheitsmort/10.0))
			{ 
				cout << "  +++FEUER!!! ";

				// Wenn ein Feuer ausbricht wird die Vegetationsdecke vermindert (Nicht: in Abhaengigkeit von der Auflagenstaerke und der Feuermort)
				for (int kartenpos=0; kartenpos< (treerows*parameter[0].sizemagnif*treecols*parameter[0].sizemagnif); kartenpos++)
				{ 
					pKarten=plot_list[kartenpos];
					pKarten->auflagenstaerke=200;
				}  

				// Falls ein Feuer ausbrach, dann wir die Trockenheitsmortalitaet dreifach so groß
				weather_list[yearposition]->trockenheitsmort=weather_list[yearposition]->trockenheitsmort*3;
				
				/*
				// ziehe Zufallsposition im Feld an der das Feuer ausbricht
				int j= (int) floor( 0.0 + ( (double)  ( ((double) (treecols-1)) *parameter[0].sizemagnif*rand()/(RAND_MAX + 1.0))) );
				int i= (int) floor( 0.0 + ( (double)  ( ((double) (treerows-1)) *parameter[0].sizemagnif*rand()/(RAND_MAX + 1.0))) );

				cout << "Flaeche auf der Feuer ausbricht y=" << j << " & x=" << i << endl;

				plot_list[i*treecols*parameter[0].sizemagnif+j]->auflagenstaerke		// bei 200 ist Feuer sehr unwahrscheinlich

				// 1 Quadratmeter wird durch den Blitz getroffen und somit dort die Vegetation in Brand gesetzt. 
				// Ausbreitung in Abhaengigkeit von Intensitaet die wiederum von der Biomasse auf dem Feld abhaengt
				*/

				parameter[0].feuerausgabe=3;
			} 
			

			///mittleres Feuer zerstört weniger auflage und erhöht die "Trockenheitsmortalitaet" um 2
			else if (zufallsz>=(weather_list[yearposition]->trockenheitsmort/10.0) && zufallsz<(weather_list[yearposition]->trockenheitsmort/5.0))
			{ 
				cout << "  ++FEUER!!! ";

				// Wenn ein Feuer ausbricht wird die Vegetationsdecke vermindert (Nicht: in Abhaengigkeit von der Auflagenstaerke und der Feuermort)
				for (int kartenpos=0; kartenpos< (treerows*parameter[0].sizemagnif*treecols*parameter[0].sizemagnif); kartenpos++)
				{ 
					pKarten=plot_list[kartenpos];
					pKarten->auflagenstaerke=(unsigned short) ((( (double) pKarten->auflagenstaerke)+200.0)/2.0);
				}  

				// Falls ein Feuer ausbrach, dann wir die Trockenheitsmortalitaet doppelt so groß
				weather_list[yearposition]->trockenheitsmort=weather_list[yearposition]->trockenheitsmort*2;
				parameter[0].feuerausgabe=2;
			} 
			

			///schwaches Feuer zerstört kaum auflage und erhöht die "Trockenheitsmortalitaet" nicht
			else if (zufallsz>=(weather_list[yearposition]->trockenheitsmort/5.0) && zufallsz<(weather_list[yearposition]->trockenheitsmort))
			{
				cout << "  +FEUER!!! ";

				// Wenn ein Feuer ausbricht wird die Vegetationsdecke vermindert (Nicht: in Abhaengigkeit von der Auflagenstaerke und der Feuermort)
				for (int kartenpos=0; kartenpos< (treerows*parameter[0].sizemagnif*treecols*parameter[0].sizemagnif); kartenpos++)
				{ 
					pKarten=plot_list[kartenpos];
					pKarten->auflagenstaerke=(unsigned short) ((( (double) pKarten->auflagenstaerke)+200.0)/1.5);
				} 

				
				// Falls ein Feuer ausbrach, dann bleibt die Trockenheitsmortalitaet so groß wie sie ist
				weather_list[yearposition]->trockenheitsmort=weather_list[yearposition]->trockenheitsmort;
				parameter[0].feuerausgabe=1;
			}

		}

	} 

}
