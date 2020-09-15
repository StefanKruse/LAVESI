

/*
	
	cout << endl;
	cout << " Endausgabe nach Simulation:" << endl;
	cout << "Kumulierte Abweichung ... Wert (BA, N_(seed-sap-tree)) ... Wert pro ha ... Vergleichswert pro ha" << endl;
	
	// gewicht startet bei 0 und wird dann mit dem Gewicht aufsummiert von allen Sites und am Ende der Mittelwert berechnet
	double abweichungaktuellerlauf=0.0;
	// VERGLEICHSDATEN ALS VECTOREN MIT POSITION WIE YPLOTPOS FÜR DIE 4 WERTE
	// ... Pos 0 in Vector ist Plotpos 0!
			 // vector<double> Vergleichswerte_Basalarea;//!<
			 // vector<double> Vergleichswerte_Seed_No;//!<
			 // vector<double> Vergleichswerte_Saps_No;//!<
			 // vector<double> Vergleichswerte_Tree_No;//!<
	// Faktor für Umrechnung von pro 20*20 m^2 Fläche zu 1 ha Flächen
	double perhascalingfactor=1.0/((20.0*20.0)/(100.0*100.0));
	
	int aktort_parameterization=0;
	for (vector<list<Baum*> >::iterator posw = WeltBaumListe.begin(); posw != WeltBaumListe.end(); ++posw)
	{ // Weltschleife
		// Liste auf den aktuellen Ort setzen
		list<Baum*>& BaumListe = *posw;

		vector<vector<Auswertung*> >::iterator posiweltausw = (WeltAuswertungsListe.begin()+aktort_parameterization);
		vector<Auswertung*>& AuswertungsListe = *posiweltausw;
		vector<Auswertung*>::iterator posausw = AuswertungsListe.begin();
		pAuswertung=(*posausw);
		
		aktort_parameterization++;
		
		// Berechnung des aktuellen Ortes
		int aktortyweltcoo=(int) floor( (double) (aktort_parameterization-1)/parameter[0].mapxlength );
		int aktortxweltcoo=(aktort_parameterization-1) - (aktortyweltcoo * parameter[0].mapxlength);

				
		if(
		((aktortyweltcoo==0 || aktortyweltcoo==1 || aktortyweltcoo==3 || aktortyweltcoo==4 || aktortyweltcoo==7 || aktortyweltcoo==8) && parameter[0].wetterwahl==1000 ) || parameter[0].wetterwahl==1001 || parameter[0].wetterwahl==1005 || parameter[0].wetterwahl==1002 || parameter[0].wetterwahl==1003 || parameter[0].wetterwahl==1004)
		{
			// //Abweichung der Simulation von Real ermitteln
			// abweichung=(schaetzwert-zufinden)/zufinden
			// // Gewichtung festlegen (gute Schätzung, hohe Werte, also mehr Einfluss)
			// gewicht=1/abs(abweichung)
			// // Erst am Ende berechnen, ansonsten kann es bei exaktem Wert zu unendlich großen Zahlen kommen
			
			abweichungaktuellerlauf+=fabs( ( (pAuswertung->BAliste.back()*(perhascalingfactor/10000)) - Vergleichswerte_Basalarea[aktortyweltcoo] ) / Vergleichswerte_Basalarea[aktortyweltcoo]);
			cout << aktortyweltcoo << "   " << abweichungaktuellerlauf << "   " << pAuswertung->BAliste.back() << "   " << ( pAuswertung->BAliste.back()*perhascalingfactor) << "  " << Vergleichswerte_Basalarea[aktortyweltcoo] << "  " << endl;
			
			abweichungaktuellerlauf+=fabs( ( (pAuswertung->nhoehe0b40liste.back()*perhascalingfactor) - Vergleichswerte_Seed_No[aktortyweltcoo] ) / Vergleichswerte_Seed_No[aktortyweltcoo]);
			cout << "   " << abweichungaktuellerlauf << "   " << pAuswertung->nhoehe0b40liste.back() << "   " << ( pAuswertung->nhoehe0b40liste.back()*perhascalingfactor) << "  " << Vergleichswerte_Seed_No[aktortyweltcoo] << "  " << endl;
			
			abweichungaktuellerlauf+=fabs( ( (pAuswertung->nhoehe41b200liste.back()*perhascalingfactor) - Vergleichswerte_Saps_No[aktortyweltcoo] ) / Vergleichswerte_Saps_No[aktortyweltcoo]);
			cout << "   " << abweichungaktuellerlauf << "   " << pAuswertung->nhoehe41b200liste.back() << "   " << ( pAuswertung->nhoehe41b200liste.back()*perhascalingfactor) << "  " << Vergleichswerte_Saps_No[aktortyweltcoo] << "  " << endl;
			
			abweichungaktuellerlauf+=fabs( ( (pAuswertung->nhoehe201b10000liste.back()*perhascalingfactor) - Vergleichswerte_Tree_No[aktortyweltcoo] ) / Vergleichswerte_Tree_No[aktortyweltcoo]);
			cout << "   " << abweichungaktuellerlauf << "   " << pAuswertung->nhoehe201b10000liste.back() << "   " << ( pAuswertung->nhoehe201b10000liste.back()*perhascalingfactor) << "  " << Vergleichswerte_Tree_No[aktortyweltcoo] << "  " << endl;
		}
	}
	
	// Gewichte-Wert Mittelwert berechnen und in List ans Ende anfuegen
		// ... wenn keine Individuen gewachsen sind ist der Wert bei 24.0, dann wird der Wert auf 0.001 gesetzt
		double abweichungsmapylength=4;
		if (parameter[0].wetterwahl==1001 || parameter[0].wetterwahl==1005 || parameter[0].wetterwahl==1002 || parameter[0].wetterwahl==1003 || parameter[0].wetterwahl==1004)
		{
			abweichungsmapylength=parameter[0].mapylength*abweichungsmapylength;
		} else if (parameter[0].wetterwahl==1000)
		{
			abweichungsmapylength=6.0*abweichungsmapylength;
		}
		// Wenn kein Wachstum auf den Flächen möglich ist, wird die Abweichung künstlich verstärkt
		if(abweichungaktuellerlauf==abweichungsmapylength)
		{
			abweichungaktuellerlauf=abweichungaktuellerlauf*1000.0;
		}
		// limitierung des Maximalwertes auf 100 (Gewicht = 1/ abweichungaktuellerlauf => wert=0.01 => Gewicht=100)
		if(abweichungaktuellerlauf<0.01)
		{
			abweichungaktuellerlauf=0.01;
		}
		
		if(parameter[0].parametergewichteverbessern==1)
		{
			Parametergewichte.push_back( (1.0/fabs( abweichungaktuellerlauf/abweichungsmapylength )) );
		} else
		{
			Parametergewichte.push_back( 1.0 );
		}

	//Ausgabe der Gewichte in eine Datei
		// Dateinamen zusammensetzen
		dateiname="databaeume_Gewichte_Parameterization.csv";
		 
		// Datei versuchen zum Lesen und Schreiben zu oeffnen
		dateizeiger = fopen (dateiname.c_str(), "r+");
		// falls nicht vorhanden, eine neue Datei mit Spaltenueberschriften anlegen
		if (dateizeiger == NULL)
		{
		  dateizeiger = fopen (dateiname.c_str(), "w");

			fprintf(dateizeiger, "wiederholung;");
			fprintf(dateizeiger, "Gewicht;");
			fprintf(dateizeiger, "\n");

			if (dateizeiger == NULL)
			{
				fprintf(stderr, "Fehler: databaeume_Gewichte_Parameterization.csv-Datei konnte nicht geoeffnet werden!\n");
				exit(1);
			}
		}

		// Die neuen Informationen werden ans Ende der Datei geschrieben
		fseek(dateizeiger,0,SEEK_END);

		// Datenaufbereiten und in die Datei schreiben
		fprintf(dateizeiger, "%d;", parameter[0].wiederholung);
		fprintf(dateizeiger, "%4.5f;", Parametergewichte.back());
		fprintf(dateizeiger, "\n");

		fclose (dateizeiger);
	// Ende der Ausgabefunktion

	
	cout << "Parametergewichte.back() => letzter Wert=" << Parametergewichte.back() << endl << endl;
	
	if(parameter[0].parametergewichteverbessern==1 && nruns==11) // auf 11 damit noch 10 Werte zum Schätzen verbleiben => BEGINNT bei 0
	{
		// lösche die ersten beiden startwerte diese führen zu einer großen Streuung
		// ... dazu kann auch einfach das Gewicht auf 0.0 gesetzt werden
		cout << "Parametergewichte[0] =" << Parametergewichte[0] << "Parametergewichte[1] =" << Parametergewichte[1] << endl;
		Parametergewichte[0]=0.0;
		Parametergewichte[1]=0.0;
		cout << "Parametergewichte[0] =" << Parametergewichte[0] << "Parametergewichte[1] =" << Parametergewichte[1] << endl << endl;

	} 
	if(parameter[0].parametergewichteverbessern==1 && nruns>=21) //  auf 21 damit 20 Werte zum Schätzen genutzt werden können
	{
		cout << "Parametergewichte veraendern:    von= ";
		// kleinsten Gewichtswert auf 0.0 setzen, da immer je Lauf ein neuer hinzukommt sollte es stabil laufen
		signed int minimumgewichtposi=0;
		double minimumgewicht=0.0;
		int minimumgewichtiter=0;
		for(signed int parameteri=0; parameteri<Parametergewichte.size(); ++parameteri)
		{
			cout << Parametergewichte[parameteri] << ", ";
			
			if(Parametergewichte[parameteri]>0.0)
			{
				// vergleiche Wert
				// ... wenn es der erste Wert ist, dann speichere diesen in die Puffervariable
				// ... ansonsten vergleiche den aktuellen mit dem gespeicherten Wert
				// ... falls der neue Wert kleiner als der gespeicherte ist, kopiere diesen
				if(minimumgewichtiter==0)
				{
					minimumgewicht=Parametergewichte[parameteri];
					minimumgewichtposi=parameteri;
				} else
				{
					if(Parametergewichte[parameteri]<minimumgewicht)
					{
						minimumgewicht=Parametergewichte[parameteri];
						minimumgewichtposi=parameteri;
					}
				}
				
				++minimumgewichtiter;
			}
		}
		cout << endl << "   => kleinster Wert=" << minimumgewicht << " auf Position=" << minimumgewichtposi << endl;
		Parametergewichte[minimumgewichtposi]=0.0;
		
		cout << "Parametergewichte veraendern:    nach= ";
		// kleinsten Gewichtswert auf 0.0 setzen, da immer je Lauf ein neuer hinzukommt sollte es stabil laufen
		for(signed int parameteri=0; parameteri<Parametergewichte.size(); ++parameteri)
		{
			cout << Parametergewichte[parameteri] << ", ";
		}
	}
	
	// Programm beenden
	//exit(0);
	
	// // Abfrage ob das Programm beendet oder fortgesetzt werden soll
	// signed int abbruch;
	// cout << " Weiter mit 1, beenden mit irgendeiner Eingabe";
	// scanf("%d", &abbruch);

	// if (abbruch!=1) 
	// {
		// cout << "LaVeSi wurde beendet" << endl << endl; 
		// exit(0);
	// }
	


	// //Abweichung der Simulation von Real ermitteln
	// abweichung=(schaetzwert-zufinden)/zufinden
	// // Gewichtung festlegen (gute Schätzung, hohe Werte, also mehr Einfluss)
	// gewicht=1/abs(abweichung)
	
	// if(i==31)
	// {// lösche die ersten beiden startwerte diese führen zu einer großen Streuung
		// wertea=wertea[-(1:2)]
		// werteb=werteb[-(1:2)]
		// gewichte=gewichte[-(1:2)]
		
		// wertea=c(wertea,parameterestimatea)
		// werteb=c(werteb,parameterestimateb)
		// gewichte=c(gewichte,gewicht)
	// } else if(i>32)
	// {
		// # gewichte[1:(length(gewichte)-100)]=0
		// # plot(sort(gewichte))
		// mingew=which(gewichte==min(gewichte))
		// #wenn der kleinste Wert in der Liste kleiner als der aktuelle Wert ist, dann lösche den alten und fügen den neuen hinzu
		// if(gewichte[mingew]<gewicht)
		// {
			// wertea=wertea[-mingew]
			// werteb=werteb[-mingew]
			// gewichte=gewichte[-mingew]
		
			// wertea=c(wertea,parameterestimatea)
			// werteb=c(werteb,parameterestimateb)
			// gewichte=c(gewichte,gewicht)
		// }
	// } else
	// {
		// wertea=c(wertea,parameterestimatea)
		// werteb=c(werteb,parameterestimateb)
		// gewichte=c(gewichte,gewicht)
	// }


*/
