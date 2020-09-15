// based on initial LAVESI publication code
// originally automatically created with R-script: Parameterization_Programmcode_Parametervariationerzeugen_R-Skript.r

// ... only random parameter choice .. later selfimprovment

/*

if(nruns==0)
{
  
// if (parameter[0].weatherchoice==0)
// {
 // Vergleichswerte_Basalarea.push_back(10.086);
 // Vergleichswerte_Basalarea.push_back(13.526);
 // Vergleichswerte_Basalarea.push_back(99999999);
 // Vergleichswerte_Basalarea.push_back(0.209);
 // Vergleichswerte_Basalarea.push_back(7.127);
 // Vergleichswerte_Basalarea.push_back(99999999);
 // Vergleichswerte_Basalarea.push_back(99999999);
 // Vergleichswerte_Basalarea.push_back(2.071);
 // Vergleichswerte_Basalarea.push_back(11.469);
 
// }
 
// else if (parameter[0].wetterwahl==1001)
// {
 // Vergleichswerte_Basalarea.push_back(10.086);
 // Vergleichswerte_Basalarea.push_back(13.526);
 // Vergleichswerte_Basalarea.push_back(0.209);
 // Vergleichswerte_Basalarea.push_back(7.127);
 // Vergleichswerte_Basalarea.push_back(2.071);
 // Vergleichswerte_Basalarea.push_back(11.469);
 
// }
 
// else if (parameter[0].wetterwahl==1005)
// {
 // Vergleichswerte_Basalarea.push_back(10.086);
 // Vergleichswerte_Basalarea.push_back(13.526);
 // Vergleichswerte_Basalarea.push_back(7.127);
 // Vergleichswerte_Basalarea.push_back(2.071);
 // Vergleichswerte_Basalarea.push_back(11.469);
 
// }
 
// else if (parameter[0].wetterwahl==1002)
// {
 // Vergleichswerte_Basalarea.push_back(10.086);
 
// }
 
// else if (parameter[0].wetterwahl==1003)
// {
 // Vergleichswerte_Basalarea.push_back(13.526);
 
// }
 
// else if (parameter[0].wetterwahl==1004)
// {
 // Vergleichswerte_Basalarea.push_back(0.209);
 
// }
 

 
// Gewichte speichern => gelten fuer alle Parameter!
Parametergewichte.push_back(1.0);// nur bei Lauf==1
Parametergewichte.push_back(1.0);// nur bei Lauf==1 
} 



// declaration of functions
double ndistrn(float mean, float sdev)
{
// Normalverteilte Zahl ziehen und 
	//The polar form of the Box-Muller transformation is both faster and more robust numerically. The algorithmic description of it is:
	float x1, x2, w, y1, y2;
	do
	{
		x1 = 2.0 * (0.0 +(float) (1.0*rand()/(RAND_MAX + 1.0) ) ) - 1.0;
		x2 = 2.0 * (0.0 +(float) (1.0*rand()/(RAND_MAX + 1.0) ) ) - 1.0;
		w = x1 * x1 + x2 * x2;
	} while ( w >= 1.0 );

	w = sqrtf( (-2.0 * logf( w ) ) / w );
	y1 = x1 * w;
	y2 = x2 * w;
	float rn;
	rn= mean + y1 * sdev; // mean=m, sdev=s
	//printf("rn=%4.4f ", rn);
	return rn;
}





// setzte parameterwerte fest
if(nruns==0)
{
	// Grenzen fuer die Parameter schätzen/festlegen!
	double parametermin_mortyouth=0.0;
	double parametermax_mortyouth=1.0;

	// Alle Werte in einem Vektor speichern
	prmz_mortyouth.push_back(parametermin_mortyouth);// nur bei Lauf==1
	prmz_mortyouth.push_back(parametermax_mortyouth);// nur bei Lauf==1
}
double summegewichte=0;
for (unsigned int position=0;position<Parametergewichte.size();++position)
{
	summegewichte+=Parametergewichte[position];
}
cout << "SummeGewichte=" << summegewichte << endl;

// gewichteten Mittelwert fuer jeden Parameter ermitteln
double gewichtetermittelwert_mortyouth=0.0;
for (unsigned int position=0;position<prmz_mortyouth.size();++position)
{
	gewichtetermittelwert_mortyouth+=prmz_mortyouth[position]*Parametergewichte[position];
}
gewichtetermittelwert_mortyouth=gewichtetermittelwert_mortyouth/summegewichte;

// gewichtete Standardabweichung fuer jeden Parameter ermitteln
double gewichtetersd_mortyouth=0.0;
for (unsigned int position=0;position<prmz_mortyouth.size();++position)
{
	gewichtetersd_mortyouth+= pow(prmz_mortyouth[position]-gewichtetermittelwert_mortyouth, 2) *  (Parametergewichte[position]/summegewichte);
}
gewichtetersd_mortyouth=pow(gewichtetersd_mortyouth, 0.5);

// Neuen Parameterwert schätzen
for(int posi=0; posi<1; ++posi) // zum Test 100mal
{
	bool wertinboundary=false;
	int anzahldos=0;
	do
	{
		++anzahldos;

		parameter[0].mortyouth=ndistrn(gewichtetermittelwert_mortyouth,gewichtetersd_mortyouth);
cout << parameter[0].mortyouth << " ";

		if(parameter[0].mortyouth>=0 && parameter[0].mortyouth<=1)
		{
			wertinboundary=true;
		}
		if(anzahldos>1000)
		{
			// cout << "WARNING: Wert nicht schaetzbar in den Grenzen = daher Loop abgebrochen" << endl;

			//Ausgabe der Warnung in Logdatei
			FILE *dateizeiger;
			string dateiname;

			// Dateinamen zusammensetzen
			dateiname="databaeume_LOGDATEI_Parameterization.csv";
			 
			// Datei versuchen zum Lesen und Schreiben zu oeffnen
			dateizeiger = fopen (dateiname.c_str(), "r+");
			// falls nicht vorhanden, eine neue Datei mit Spaltenueberschriften anlegen
			if (dateizeiger == NULL)
			{
			  dateizeiger = fopen (dateiname.c_str(), "w");

			fprintf(dateizeiger, "Repeat;");
			fprintf(dateizeiger, "Prompt;");
			fprintf(dateizeiger, "\n");

			if (dateizeiger == NULL)
			{
			fprintf(stderr, "Fehler: LOGausgabedatei konnte nicht geoeffnet werden!\n");
			exit(1);
			}
			}

			// Die neuen Informationen werden ans Ende der Datei geschrieben
			fseek(dateizeiger,0,SEEK_END);

			// Datenaufbereiten und in die Datei schreiben
			fprintf(dateizeiger, "%d;", parameter[0].wiederholung);
			fprintf(dateizeiger, "WARNING: mortyouth Wert nach 1000 loops nicht schaetzbar in den Grenzen = daher Loop abgebrochen und Wert mit %4.5f verwendet;", parameter[0].mortyouth);
			fprintf(dateizeiger, "\n");

			fclose (dateizeiger);
			// Ende der Ausgabefunktion


			wertinboundary=true;
		}
	}while(wertinboundary==false);
	// Parameterwert in Liste einfuegen
	prmz_mortyouth.push_back(parameter[0].mortyouth);
	//cout << "Anzahl dos=" << anzahldos << endl;

}


*/


/*
 
// setzte parameterwerte fest
if(nruns==0)
{
// Grenzen fuer die Parameter schätzen/festlegen!
double parametermin_aatnddratio=-96;
double parametermax_aatnddratio=100;

// Alle Werte in einem Vektor speichern
Parameterwerte_aatnddratio.push_back(parametermin_aatnddratio);// nur bei Lauf==1
Parameterwerte_aatnddratio.push_back(parametermax_aatnddratio);// nur bei Lauf==1

}

// gewichteten Mittelwert fuer jeden Parameter ermitteln
double gewichtetermittelwert_aatnddratio=0.0;
for (unsigned int position=0;position<Parameterwerte_aatnddratio.size();++position)
{
gewichtetermittelwert_aatnddratio+=Parameterwerte_aatnddratio[position]*Parametergewichte[position];
}
gewichtetermittelwert_aatnddratio=gewichtetermittelwert_aatnddratio/summegewichte;

// gewichtete Standardabweichung fuer jeden Parameter ermitteln
double gewichtetersd_aatnddratio=0.0;
for (unsigned int position=0;position<Parameterwerte_aatnddratio.size();++position)
{
gewichtetersd_aatnddratio+= pow(Parameterwerte_aatnddratio[position]-gewichtetermittelwert_aatnddratio, 2) *  (Parametergewichte[position]/summegewichte);
}
gewichtetersd_aatnddratio=pow(gewichtetersd_aatnddratio, 0.5);

// Neuen Parameterwert schätzen
for(int posi=0; posi<1; ++posi) // zum Test 100mal
{
bool wertinboundary=false;
int anzahldos=0;
do
{
++anzahldos;

parameter[0].aatnddratio=NormverteilungRN(gewichtetermittelwert_aatnddratio,gewichtetersd_aatnddratio);
//cout << parameter[0].aatnddratio << " ";

if(parameter[0].aatnddratio>=1 && parameter[0].aatnddratio<=100)
{
wertinboundary=true;
}
if(anzahldos>1000)
{
// cout << "WARNING: Wert nicht schaetzbar in den Grenzen = daher Loop abgebrochen" << endl;


//Ausgabe der Warnung in Logdatei

FILE *dateizeiger;
string dateiname;

// Dateinamen zusammensetzen
dateiname="databaeume_LOGDATEI_Parameterization.csv";
 
// Datei versuchen zum Lesen und Schreiben zu oeffnen
dateizeiger = fopen (dateiname.c_str(), "r+");
// falls nicht vorhanden, eine neue Datei mit Spaltenueberschriften anlegen
if (dateizeiger == NULL)
{
  dateizeiger = fopen (dateiname.c_str(), "w");

fprintf(dateizeiger, "wiederholung;");
fprintf(dateizeiger, "meldung;");
fprintf(dateizeiger, "\n");

if (dateizeiger == NULL)
{
fprintf(stderr, "Fehler: LOGausgabedatei konnte nicht geoeffnet werden!\n");
exit(1);
}
}

// Die neuen Informationen werden ans Ende der Datei geschrieben
fseek(dateizeiger,0,SEEK_END);

// Datenaufbereiten und in die Datei schreiben
fprintf(dateizeiger, "%d;", parameter[0].wiederholung);
fprintf(dateizeiger, "WARNING: aatnddratio Wert nach 1000 loops nicht schaetzbar in den Grenzen = daher Loop abgebrochen und Wert mit %4.5f verwendet;", parameter[0].aatnddratio);
fprintf(dateizeiger, "\n");

fclose (dateizeiger);
// Ende der Ausgabefunktion


wertinboundary=true;
}
}while(wertinboundary==false);
// Parameterwert in Liste einfuegen
Parameterwerte_aatnddratio.push_back(parameter[0].aatnddratio);
//cout << "Anzahl dos=" << anzahldos << endl;

}
 
// setzte parameterwerte fest
if(nruns==0)
{
// Grenzen fuer die Parameter schätzen/festlegen!
double parametermin_samenflugrate=0;
double parametermax_samenflugrate=1.4;

// Alle Werte in einem Vektor speichern
Parameterwerte_samenflugrate.push_back(parametermin_samenflugrate);// nur bei Lauf==1
Parameterwerte_samenflugrate.push_back(parametermax_samenflugrate);// nur bei Lauf==1

}




// gewichteten Mittelwert fuer jeden Parameter ermitteln
double gewichtetermittelwert_samenflugrate=0.0;
for (unsigned int position=0;position<Parameterwerte_samenflugrate.size();++position)
{
gewichtetermittelwert_samenflugrate+=Parameterwerte_samenflugrate[position]*Parametergewichte[position];
}
gewichtetermittelwert_samenflugrate=gewichtetermittelwert_samenflugrate/summegewichte;

// gewichtete Standardabweichung fuer jeden Parameter ermitteln
double gewichtetersd_samenflugrate=0.0;
for (unsigned int position=0;position<Parameterwerte_samenflugrate.size();++position)
{
gewichtetersd_samenflugrate+= pow(Parameterwerte_samenflugrate[position]-gewichtetermittelwert_samenflugrate, 2) *  (Parametergewichte[position]/summegewichte);
}
gewichtetersd_samenflugrate=pow(gewichtetersd_samenflugrate, 0.5);

// Neuen Parameterwert schätzen
for(int posi=0; posi<1; ++posi) // zum Test 100mal
{
bool wertinboundary=false;
int anzahldos=0;
do
{
++anzahldos;

parameter[0].samenflugrate=NormverteilungRN(gewichtetermittelwert_samenflugrate,gewichtetersd_samenflugrate);
//cout << parameter[0].samenflugrate << " ";

if(parameter[0].samenflugrate>0 && parameter[0].samenflugrate<=1)
{
wertinboundary=true;
}
if(anzahldos>1000)
{
// cout << "WARNING: Wert nicht schaetzbar in den Grenzen = daher Loop abgebrochen" << endl;


//Ausgabe der Warnung in Logdatei

FILE *dateizeiger;
string dateiname;

// Dateinamen zusammensetzen
dateiname="databaeume_LOGDATEI_Parameterization.csv";
 
// Datei versuchen zum Lesen und Schreiben zu oeffnen
dateizeiger = fopen (dateiname.c_str(), "r+");
// falls nicht vorhanden, eine neue Datei mit Spaltenueberschriften anlegen
if (dateizeiger == NULL)
{
  dateizeiger = fopen (dateiname.c_str(), "w");

fprintf(dateizeiger, "wiederholung;");
fprintf(dateizeiger, "meldung;");
fprintf(dateizeiger, "\n");

if (dateizeiger == NULL)
{
fprintf(stderr, "Fehler: LOGausgabedatei konnte nicht geoeffnet werden!\n");
exit(1);
}
}

// Die neuen Informationen werden ans Ende der Datei geschrieben
fseek(dateizeiger,0,SEEK_END);

// Datenaufbereiten und in die Datei schreiben
fprintf(dateizeiger, "%d;", parameter[0].wiederholung);
fprintf(dateizeiger, "WARNING: samenflugrate Wert nach 1000 loops nicht schaetzbar in den Grenzen = daher Loop abgebrochen und Wert mit %4.5f verwendet;", parameter[0].samenflugrate);
fprintf(dateizeiger, "\n");

fclose (dateizeiger);
// Ende der Ausgabefunktion


wertinboundary=true;
}
}while(wertinboundary==false);
// Parameterwert in Liste einfuegen
Parameterwerte_samenflugrate.push_back(parameter[0].samenflugrate);
//cout << "Anzahl dos=" << anzahldos << endl;

}
 
// setzte parameterwerte fest
if(nruns==0)
{
// Grenzen fuer die Parameter schätzen/festlegen!
double parametermin_entfernungsteiler=-1.68;
double parametermax_entfernungsteiler=2;

// Alle Werte in einem Vektor speichern
Parameterwerte_entfernungsteiler.push_back(parametermin_entfernungsteiler);// nur bei Lauf==1
Parameterwerte_entfernungsteiler.push_back(parametermax_entfernungsteiler);// nur bei Lauf==1

}




// gewichteten Mittelwert fuer jeden Parameter ermitteln
double gewichtetermittelwert_entfernungsteiler=0.0;
for (unsigned int position=0;position<Parameterwerte_entfernungsteiler.size();++position)
{
gewichtetermittelwert_entfernungsteiler+=Parameterwerte_entfernungsteiler[position]*Parametergewichte[position];
}
gewichtetermittelwert_entfernungsteiler=gewichtetermittelwert_entfernungsteiler/summegewichte;

// gewichtete Standardabweichung fuer jeden Parameter ermitteln
double gewichtetersd_entfernungsteiler=0.0;
for (unsigned int position=0;position<Parameterwerte_entfernungsteiler.size();++position)
{
gewichtetersd_entfernungsteiler+= pow(Parameterwerte_entfernungsteiler[position]-gewichtetermittelwert_entfernungsteiler, 2) *  (Parametergewichte[position]/summegewichte);
}
gewichtetersd_entfernungsteiler=pow(gewichtetersd_entfernungsteiler, 0.5);

// Neuen Parameterwert schätzen
for(int posi=0; posi<1; ++posi) // zum Test 100mal
{
bool wertinboundary=false;
int anzahldos=0;
do
{
++anzahldos;

parameter[0].entfernungsteiler=NormverteilungRN(gewichtetermittelwert_entfernungsteiler,gewichtetersd_entfernungsteiler);
//cout << parameter[0].entfernungsteiler << " ";

if(parameter[0].entfernungsteiler>0 && parameter[0].entfernungsteiler<=2)
{
wertinboundary=true;
}
if(anzahldos>1000)
{
// cout << "WARNING: Wert nicht schaetzbar in den Grenzen = daher Loop abgebrochen" << endl;


//Ausgabe der Warnung in Logdatei

FILE *dateizeiger;
string dateiname;

// Dateinamen zusammensetzen
dateiname="databaeume_LOGDATEI_Parameterization.csv";
 
// Datei versuchen zum Lesen und Schreiben zu oeffnen
dateizeiger = fopen (dateiname.c_str(), "r+");
// falls nicht vorhanden, eine neue Datei mit Spaltenueberschriften anlegen
if (dateizeiger == NULL)
{
  dateizeiger = fopen (dateiname.c_str(), "w");

fprintf(dateizeiger, "wiederholung;");
fprintf(dateizeiger, "meldung;");
fprintf(dateizeiger, "\n");

if (dateizeiger == NULL)
{
fprintf(stderr, "Fehler: LOGausgabedatei konnte nicht geoeffnet werden!\n");
exit(1);
}
}

// Die neuen Informationen werden ans Ende der Datei geschrieben
fseek(dateizeiger,0,SEEK_END);

// Datenaufbereiten und in die Datei schreiben
fprintf(dateizeiger, "%d;", parameter[0].wiederholung);
fprintf(dateizeiger, "WARNING: entfernungsteiler Wert nach 1000 loops nicht schaetzbar in den Grenzen = daher Loop abgebrochen und Wert mit %4.5f verwendet;", parameter[0].entfernungsteiler);
fprintf(dateizeiger, "\n");

fclose (dateizeiger);
// Ende der Ausgabefunktion


wertinboundary=true;
}
}while(wertinboundary==false);
// Parameterwert in Liste einfuegen
Parameterwerte_entfernungsteiler.push_back(parameter[0].entfernungsteiler);
//cout << "Anzahl dos=" << anzahldos << endl;

}
 
// setzte parameterwerte fest
if(nruns==0)
{
// Grenzen fuer die Parameter schätzen/festlegen!
double parametermin_samenprodfaktor=-8;
double parametermax_samenprodfaktor=20;

// Alle Werte in einem Vektor speichern
Parameterwerte_samenprodfaktor.push_back(parametermin_samenprodfaktor);// nur bei Lauf==1
Parameterwerte_samenprodfaktor.push_back(parametermax_samenprodfaktor);// nur bei Lauf==1

}




// gewichteten Mittelwert fuer jeden Parameter ermitteln
double gewichtetermittelwert_samenprodfaktor=0.0;
for (unsigned int position=0;position<Parameterwerte_samenprodfaktor.size();++position)
{
gewichtetermittelwert_samenprodfaktor+=Parameterwerte_samenprodfaktor[position]*Parametergewichte[position];
}
gewichtetermittelwert_samenprodfaktor=gewichtetermittelwert_samenprodfaktor/summegewichte;

// gewichtete Standardabweichung fuer jeden Parameter ermitteln
double gewichtetersd_samenprodfaktor=0.0;
for (unsigned int position=0;position<Parameterwerte_samenprodfaktor.size();++position)
{
gewichtetersd_samenprodfaktor+= pow(Parameterwerte_samenprodfaktor[position]-gewichtetermittelwert_samenprodfaktor, 2) *  (Parametergewichte[position]/summegewichte);
}
gewichtetersd_samenprodfaktor=pow(gewichtetersd_samenprodfaktor, 0.5);

// Neuen Parameterwert schätzen
for(int posi=0; posi<1; ++posi) // zum Test 100mal
{
bool wertinboundary=false;
int anzahldos=0;
do
{
++anzahldos;

parameter[0].samenprodfaktor=NormverteilungRN(gewichtetermittelwert_samenprodfaktor,gewichtetersd_samenprodfaktor);
//cout << parameter[0].samenprodfaktor << " ";

if(parameter[0].samenprodfaktor>0 && parameter[0].samenprodfaktor<=20)
{
wertinboundary=true;
}
if(anzahldos>1000)
{
// cout << "WARNING: Wert nicht schaetzbar in den Grenzen = daher Loop abgebrochen" << endl;


//Ausgabe der Warnung in Logdatei

FILE *dateizeiger;
string dateiname;

// Dateinamen zusammensetzen
dateiname="databaeume_LOGDATEI_Parameterization.csv";
 
// Datei versuchen zum Lesen und Schreiben zu oeffnen
dateizeiger = fopen (dateiname.c_str(), "r+");
// falls nicht vorhanden, eine neue Datei mit Spaltenueberschriften anlegen
if (dateizeiger == NULL)
{
  dateizeiger = fopen (dateiname.c_str(), "w");

fprintf(dateizeiger, "wiederholung;");
fprintf(dateizeiger, "meldung;");
fprintf(dateizeiger, "\n");

if (dateizeiger == NULL)
{
fprintf(stderr, "Fehler: LOGausgabedatei konnte nicht geoeffnet werden!\n");
exit(1);
}
}

// Die neuen Informationen werden ans Ende der Datei geschrieben
fseek(dateizeiger,0,SEEK_END);

// Datenaufbereiten und in die Datei schreiben
fprintf(dateizeiger, "%d;", parameter[0].wiederholung);
fprintf(dateizeiger, "WARNING: samenprodfaktor Wert nach 1000 loops nicht schaetzbar in den Grenzen = daher Loop abgebrochen und Wert mit %4.5f verwendet;", parameter[0].samenprodfaktor);
fprintf(dateizeiger, "\n");

fclose (dateizeiger);
// Ende der Ausgabefunktion


wertinboundary=true;
}
}while(wertinboundary==false);
// Parameterwert in Liste einfuegen
Parameterwerte_samenprodfaktor.push_back(parameter[0].samenprodfaktor);
//cout << "Anzahl dos=" << anzahldos << endl;

}
 
// setzte parameterwerte fest
if(nruns==0)
{
// Grenzen fuer die Parameter schätzen/festlegen!
double parametermin_keimungwettereinfluss=0;
double parametermax_keimungwettereinfluss=1;

// Alle Werte in einem Vektor speichern
Parameterwerte_keimungwettereinfluss.push_back(parametermin_keimungwettereinfluss);// nur bei Lauf==1
Parameterwerte_keimungwettereinfluss.push_back(parametermax_keimungwettereinfluss);// nur bei Lauf==1

}




// gewichteten Mittelwert fuer jeden Parameter ermitteln
double gewichtetermittelwert_keimungwettereinfluss=0.0;
for (unsigned int position=0;position<Parameterwerte_keimungwettereinfluss.size();++position)
{
gewichtetermittelwert_keimungwettereinfluss+=Parameterwerte_keimungwettereinfluss[position]*Parametergewichte[position];
}
gewichtetermittelwert_keimungwettereinfluss=gewichtetermittelwert_keimungwettereinfluss/summegewichte;

// gewichtete Standardabweichung fuer jeden Parameter ermitteln
double gewichtetersd_keimungwettereinfluss=0.0;
for (unsigned int position=0;position<Parameterwerte_keimungwettereinfluss.size();++position)
{
gewichtetersd_keimungwettereinfluss+= pow(Parameterwerte_keimungwettereinfluss[position]-gewichtetermittelwert_keimungwettereinfluss, 2) *  (Parametergewichte[position]/summegewichte);
}
gewichtetersd_keimungwettereinfluss=pow(gewichtetersd_keimungwettereinfluss, 0.5);

// Neuen Parameterwert schätzen
for(int posi=0; posi<1; ++posi) // zum Test 100mal
{
bool wertinboundary=false;
int anzahldos=0;
do
{
++anzahldos;

parameter[0].keimungwettereinfluss=NormverteilungRN(gewichtetermittelwert_keimungwettereinfluss,gewichtetersd_keimungwettereinfluss);
//cout << parameter[0].keimungwettereinfluss << " ";

if(parameter[0].keimungwettereinfluss>=0 && parameter[0].keimungwettereinfluss<=1)
{
wertinboundary=true;
}
if(anzahldos>1000)
{
// cout << "WARNING: Wert nicht schaetzbar in den Grenzen = daher Loop abgebrochen" << endl;


//Ausgabe der Warnung in Logdatei

FILE *dateizeiger;
string dateiname;

// Dateinamen zusammensetzen
dateiname="databaeume_LOGDATEI_Parameterization.csv";
 
// Datei versuchen zum Lesen und Schreiben zu oeffnen
dateizeiger = fopen (dateiname.c_str(), "r+");
// falls nicht vorhanden, eine neue Datei mit Spaltenueberschriften anlegen
if (dateizeiger == NULL)
{
  dateizeiger = fopen (dateiname.c_str(), "w");

fprintf(dateizeiger, "wiederholung;");
fprintf(dateizeiger, "meldung;");
fprintf(dateizeiger, "\n");

if (dateizeiger == NULL)
{
fprintf(stderr, "Fehler: LOGausgabedatei konnte nicht geoeffnet werden!\n");
exit(1);
}
}

// Die neuen Informationen werden ans Ende der Datei geschrieben
fseek(dateizeiger,0,SEEK_END);

// Datenaufbereiten und in die Datei schreiben
fprintf(dateizeiger, "%d;", parameter[0].wiederholung);
fprintf(dateizeiger, "WARNING: keimungwettereinfluss Wert nach 1000 loops nicht schaetzbar in den Grenzen = daher Loop abgebrochen und Wert mit %4.5f verwendet;", parameter[0].keimungwettereinfluss);
fprintf(dateizeiger, "\n");

fclose (dateizeiger);
// Ende der Ausgabefunktion


wertinboundary=true;
}
}while(wertinboundary==false);
// Parameterwert in Liste einfuegen
Parameterwerte_keimungwettereinfluss.push_back(parameter[0].keimungwettereinfluss);
//cout << "Anzahl dos=" << anzahldos << endl;

}
 
// setzte parameterwerte fest
if(nruns==0)
{
// Grenzen fuer die Parameter schätzen/festlegen!
double parametermin_keimungsrate=-1;
double parametermax_keimungsrate=1;

// Alle Werte in einem Vektor speichern
Parameterwerte_keimungsrate.push_back(parametermin_keimungsrate);// nur bei Lauf==1
Parameterwerte_keimungsrate.push_back(parametermax_keimungsrate);// nur bei Lauf==1

}




// gewichteten Mittelwert fuer jeden Parameter ermitteln
double gewichtetermittelwert_keimungsrate=0.0;
for (unsigned int position=0;position<Parameterwerte_keimungsrate.size();++position)
{
gewichtetermittelwert_keimungsrate+=Parameterwerte_keimungsrate[position]*Parametergewichte[position];
}
gewichtetermittelwert_keimungsrate=gewichtetermittelwert_keimungsrate/summegewichte;

// gewichtete Standardabweichung fuer jeden Parameter ermitteln
double gewichtetersd_keimungsrate=0.0;
for (unsigned int position=0;position<Parameterwerte_keimungsrate.size();++position)
{
gewichtetersd_keimungsrate+= pow(Parameterwerte_keimungsrate[position]-gewichtetermittelwert_keimungsrate, 2) *  (Parametergewichte[position]/summegewichte);
}
gewichtetersd_keimungsrate=pow(gewichtetersd_keimungsrate, 0.5);

// Neuen Parameterwert schätzen
for(int posi=0; posi<1; ++posi) // zum Test 100mal
{
bool wertinboundary=false;
int anzahldos=0;
do
{
++anzahldos;

parameter[0].keimungsrate=NormverteilungRN(gewichtetermittelwert_keimungsrate,gewichtetersd_keimungsrate);
//cout << parameter[0].keimungsrate << " ";

if(parameter[0].keimungsrate>=0 && parameter[0].keimungsrate<=1)
{
wertinboundary=true;
}
if(anzahldos>1000)
{
// cout << "WARNING: Wert nicht schaetzbar in den Grenzen = daher Loop abgebrochen" << endl;


//Ausgabe der Warnung in Logdatei

FILE *dateizeiger;
string dateiname;

// Dateinamen zusammensetzen
dateiname="databaeume_LOGDATEI_Parameterization.csv";
 
// Datei versuchen zum Lesen und Schreiben zu oeffnen
dateizeiger = fopen (dateiname.c_str(), "r+");
// falls nicht vorhanden, eine neue Datei mit Spaltenueberschriften anlegen
if (dateizeiger == NULL)
{
  dateizeiger = fopen (dateiname.c_str(), "w");

fprintf(dateizeiger, "wiederholung;");
fprintf(dateizeiger, "meldung;");
fprintf(dateizeiger, "\n");

if (dateizeiger == NULL)
{
fprintf(stderr, "Fehler: LOGausgabedatei konnte nicht geoeffnet werden!\n");
exit(1);
}
}

// Die neuen Informationen werden ans Ende der Datei geschrieben
fseek(dateizeiger,0,SEEK_END);

// Datenaufbereiten und in die Datei schreiben
fprintf(dateizeiger, "%d;", parameter[0].wiederholung);
fprintf(dateizeiger, "WARNING: keimungsrate Wert nach 1000 loops nicht schaetzbar in den Grenzen = daher Loop abgebrochen und Wert mit %4.5f verwendet;", parameter[0].keimungsrate);
fprintf(dateizeiger, "\n");

fclose (dateizeiger);
// Ende der Ausgabefunktion


wertinboundary=true;
}
}while(wertinboundary==false);
// Parameterwert in Liste einfuegen
Parameterwerte_keimungsrate.push_back(parameter[0].keimungsrate);
//cout << "Anzahl dos=" << anzahldos << endl;

}
 
// setzte parameterwerte fest
if(nruns==0)
{
// Grenzen fuer die Parameter schätzen/festlegen!
double parametermin_precgrenzwert=0;
double parametermax_precgrenzwert=500;

// Alle Werte in einem Vektor speichern
Parameterwerte_precgrenzwert.push_back(parametermin_precgrenzwert);// nur bei Lauf==1
Parameterwerte_precgrenzwert.push_back(parametermax_precgrenzwert);// nur bei Lauf==1

}




// gewichteten Mittelwert fuer jeden Parameter ermitteln
double gewichtetermittelwert_precgrenzwert=0.0;
for (unsigned int position=0;position<Parameterwerte_precgrenzwert.size();++position)
{
gewichtetermittelwert_precgrenzwert+=Parameterwerte_precgrenzwert[position]*Parametergewichte[position];
}
gewichtetermittelwert_precgrenzwert=gewichtetermittelwert_precgrenzwert/summegewichte;

// gewichtete Standardabweichung fuer jeden Parameter ermitteln
double gewichtetersd_precgrenzwert=0.0;
for (unsigned int position=0;position<Parameterwerte_precgrenzwert.size();++position)
{
gewichtetersd_precgrenzwert+= pow(Parameterwerte_precgrenzwert[position]-gewichtetermittelwert_precgrenzwert, 2) *  (Parametergewichte[position]/summegewichte);
}
gewichtetersd_precgrenzwert=pow(gewichtetersd_precgrenzwert, 0.5);

// Neuen Parameterwert schätzen
for(int posi=0; posi<1; ++posi) // zum Test 100mal
{
bool wertinboundary=false;
int anzahldos=0;
do
{
++anzahldos;

parameter[0].precgrenzwert=NormverteilungRN(gewichtetermittelwert_precgrenzwert,gewichtetersd_precgrenzwert);
//cout << parameter[0].precgrenzwert << " ";

if(parameter[0].precgrenzwert>=0 && parameter[0].precgrenzwert<=500)
{
wertinboundary=true;
}
if(anzahldos>1000)
{
// cout << "WARNING: Wert nicht schaetzbar in den Grenzen = daher Loop abgebrochen" << endl;


//Ausgabe der Warnung in Logdatei

FILE *dateizeiger;
string dateiname;

// Dateinamen zusammensetzen
dateiname="databaeume_LOGDATEI_Parameterization.csv";
 
// Datei versuchen zum Lesen und Schreiben zu oeffnen
dateizeiger = fopen (dateiname.c_str(), "r+");
// falls nicht vorhanden, eine neue Datei mit Spaltenueberschriften anlegen
if (dateizeiger == NULL)
{
  dateizeiger = fopen (dateiname.c_str(), "w");

fprintf(dateizeiger, "wiederholung;");
fprintf(dateizeiger, "meldung;");
fprintf(dateizeiger, "\n");

if (dateizeiger == NULL)
{
fprintf(stderr, "Fehler: LOGausgabedatei konnte nicht geoeffnet werden!\n");
exit(1);
}
}

// Die neuen Informationen werden ans Ende der Datei geschrieben
fseek(dateizeiger,0,SEEK_END);

// Datenaufbereiten und in die Datei schreiben
fprintf(dateizeiger, "%d;", parameter[0].wiederholung);
fprintf(dateizeiger, "WARNING: precgrenzwert Wert nach 1000 loops nicht schaetzbar in den Grenzen = daher Loop abgebrochen und Wert mit %4.5f verwendet;", parameter[0].precgrenzwert);
fprintf(dateizeiger, "\n");

fclose (dateizeiger);
// Ende der Ausgabefunktion


wertinboundary=true;
}
}while(wertinboundary==false);
// Parameterwert in Liste einfuegen
Parameterwerte_precgrenzwert.push_back(parameter[0].precgrenzwert);
//cout << "Anzahl dos=" << anzahldos << endl;

}
 
// setzte parameterwerte fest
if(nruns==0)
{
// Grenzen fuer die Parameter schätzen/festlegen!
double parametermin_mortbg=-0.099;
double parametermax_mortbg=0.1;

// Alle Werte in einem Vektor speichern
Parameterwerte_mortbg.push_back(parametermin_mortbg);// nur bei Lauf==1
Parameterwerte_mortbg.push_back(parametermax_mortbg);// nur bei Lauf==1

}




// gewichteten Mittelwert fuer jeden Parameter ermitteln
double gewichtetermittelwert_mortbg=0.0;
for (unsigned int position=0;position<Parameterwerte_mortbg.size();++position)
{
gewichtetermittelwert_mortbg+=Parameterwerte_mortbg[position]*Parametergewichte[position];
}
gewichtetermittelwert_mortbg=gewichtetermittelwert_mortbg/summegewichte;

// gewichtete Standardabweichung fuer jeden Parameter ermitteln
double gewichtetersd_mortbg=0.0;
for (unsigned int position=0;position<Parameterwerte_mortbg.size();++position)
{
gewichtetersd_mortbg+= pow(Parameterwerte_mortbg[position]-gewichtetermittelwert_mortbg, 2) *  (Parametergewichte[position]/summegewichte);
}
gewichtetersd_mortbg=pow(gewichtetersd_mortbg, 0.5);

// Neuen Parameterwert schätzen
for(int posi=0; posi<1; ++posi) // zum Test 100mal
{
bool wertinboundary=false;
int anzahldos=0;
do
{
++anzahldos;

parameter[0].mortbg=NormverteilungRN(gewichtetermittelwert_mortbg,gewichtetersd_mortbg);
//cout << parameter[0].mortbg << " ";

if(parameter[0].mortbg>=0 && parameter[0].mortbg<=0.1)
{
wertinboundary=true;
}
if(anzahldos>1000)
{
// cout << "WARNING: Wert nicht schaetzbar in den Grenzen = daher Loop abgebrochen" << endl;


//Ausgabe der Warnung in Logdatei

FILE *dateizeiger;
string dateiname;

// Dateinamen zusammensetzen
dateiname="databaeume_LOGDATEI_Parameterization.csv";
 
// Datei versuchen zum Lesen und Schreiben zu oeffnen
dateizeiger = fopen (dateiname.c_str(), "r+");
// falls nicht vorhanden, eine neue Datei mit Spaltenueberschriften anlegen
if (dateizeiger == NULL)
{
  dateizeiger = fopen (dateiname.c_str(), "w");

fprintf(dateizeiger, "wiederholung;");
fprintf(dateizeiger, "meldung;");
fprintf(dateizeiger, "\n");

if (dateizeiger == NULL)
{
fprintf(stderr, "Fehler: LOGausgabedatei konnte nicht geoeffnet werden!\n");
exit(1);
}
}

// Die neuen Informationen werden ans Ende der Datei geschrieben
fseek(dateizeiger,0,SEEK_END);

// Datenaufbereiten und in die Datei schreiben
fprintf(dateizeiger, "%d;", parameter[0].wiederholung);
fprintf(dateizeiger, "WARNING: mortbg Wert nach 1000 loops nicht schaetzbar in den Grenzen = daher Loop abgebrochen und Wert mit %4.5f verwendet;", parameter[0].mortbg);
fprintf(dateizeiger, "\n");

fclose (dateizeiger);
// Ende der Ausgabefunktion


wertinboundary=true;
}
}while(wertinboundary==false);
// Parameterwert in Liste einfuegen
Parameterwerte_mortbg.push_back(parameter[0].mortbg);
//cout << "Anzahl dos=" << anzahldos << endl;

}
 
// setzte parameterwerte fest
if(nruns==0)
{
// Grenzen fuer die Parameter schätzen/festlegen!
double parametermin_malt=0;
double parametermax_malt=20;

// Alle Werte in einem Vektor speichern
Parameterwerte_malt.push_back(parametermin_malt);// nur bei Lauf==1
Parameterwerte_malt.push_back(parametermax_malt);// nur bei Lauf==1

}




// gewichteten Mittelwert fuer jeden Parameter ermitteln
double gewichtetermittelwert_malt=0.0;
for (unsigned int position=0;position<Parameterwerte_malt.size();++position)
{
gewichtetermittelwert_malt+=Parameterwerte_malt[position]*Parametergewichte[position];
}
gewichtetermittelwert_malt=gewichtetermittelwert_malt/summegewichte;

// gewichtete Standardabweichung fuer jeden Parameter ermitteln
double gewichtetersd_malt=0.0;
for (unsigned int position=0;position<Parameterwerte_malt.size();++position)
{
gewichtetersd_malt+= pow(Parameterwerte_malt[position]-gewichtetermittelwert_malt, 2) *  (Parametergewichte[position]/summegewichte);
}
gewichtetersd_malt=pow(gewichtetersd_malt, 0.5);

// Neuen Parameterwert schätzen
for(int posi=0; posi<1; ++posi) // zum Test 100mal
{
bool wertinboundary=false;
int anzahldos=0;
do
{
++anzahldos;

parameter[0].malt=NormverteilungRN(gewichtetermittelwert_malt,gewichtetersd_malt);
//cout << parameter[0].malt << " ";

if(parameter[0].malt>=0 && parameter[0].malt<=20)
{
wertinboundary=true;
}
if(anzahldos>1000)
{
// cout << "WARNING: Wert nicht schaetzbar in den Grenzen = daher Loop abgebrochen" << endl;


//Ausgabe der Warnung in Logdatei

FILE *dateizeiger;
string dateiname;

// Dateinamen zusammensetzen
dateiname="databaeume_LOGDATEI_Parameterization.csv";
 
// Datei versuchen zum Lesen und Schreiben zu oeffnen
dateizeiger = fopen (dateiname.c_str(), "r+");
// falls nicht vorhanden, eine neue Datei mit Spaltenueberschriften anlegen
if (dateizeiger == NULL)
{
  dateizeiger = fopen (dateiname.c_str(), "w");

fprintf(dateizeiger, "wiederholung;");
fprintf(dateizeiger, "meldung;");
fprintf(dateizeiger, "\n");

if (dateizeiger == NULL)
{
fprintf(stderr, "Fehler: LOGausgabedatei konnte nicht geoeffnet werden!\n");
exit(1);
}
}

// Die neuen Informationen werden ans Ende der Datei geschrieben
fseek(dateizeiger,0,SEEK_END);

// Datenaufbereiten und in die Datei schreiben
fprintf(dateizeiger, "%d;", parameter[0].wiederholung);
fprintf(dateizeiger, "WARNING: malt Wert nach 1000 loops nicht schaetzbar in den Grenzen = daher Loop abgebrochen und Wert mit %4.5f verwendet;", parameter[0].malt);
fprintf(dateizeiger, "\n");

fclose (dateizeiger);
// Ende der Ausgabefunktion


wertinboundary=true;
}
}while(wertinboundary==false);
// Parameterwert in Liste einfuegen
Parameterwerte_malt.push_back(parameter[0].malt);
//cout << "Anzahl dos=" << anzahldos << endl;

}
 
// setzte parameterwerte fest
if(nruns==0)
{
// Grenzen fuer die Parameter schätzen/festlegen!
double parametermin_mjung=0;
double parametermax_mjung=1.7;

// Alle Werte in einem Vektor speichern
Parameterwerte_mjung.push_back(parametermin_mjung);// nur bei Lauf==1
Parameterwerte_mjung.push_back(parametermax_mjung);// nur bei Lauf==1

}




// gewichteten Mittelwert fuer jeden Parameter ermitteln
double gewichtetermittelwert_mjung=0.0;
for (unsigned int position=0;position<Parameterwerte_mjung.size();++position)
{
gewichtetermittelwert_mjung+=Parameterwerte_mjung[position]*Parametergewichte[position];
}
gewichtetermittelwert_mjung=gewichtetermittelwert_mjung/summegewichte;

// gewichtete Standardabweichung fuer jeden Parameter ermitteln
double gewichtetersd_mjung=0.0;
for (unsigned int position=0;position<Parameterwerte_mjung.size();++position)
{
gewichtetersd_mjung+= pow(Parameterwerte_mjung[position]-gewichtetermittelwert_mjung, 2) *  (Parametergewichte[position]/summegewichte);
}
gewichtetersd_mjung=pow(gewichtetersd_mjung, 0.5);

// Neuen Parameterwert schätzen
for(int posi=0; posi<1; ++posi) // zum Test 100mal
{
bool wertinboundary=false;
int anzahldos=0;
do
{
++anzahldos;

parameter[0].mjung=NormverteilungRN(gewichtetermittelwert_mjung,gewichtetersd_mjung);
//cout << parameter[0].mjung << " ";

if(parameter[0].mjung>=0 && parameter[0].mjung<=1)
{
wertinboundary=true;
}
if(anzahldos>1000)
{
// cout << "WARNING: Wert nicht schaetzbar in den Grenzen = daher Loop abgebrochen" << endl;


//Ausgabe der Warnung in Logdatei

FILE *dateizeiger;
string dateiname;

// Dateinamen zusammensetzen
dateiname="databaeume_LOGDATEI_Parameterization.csv";
 
// Datei versuchen zum Lesen und Schreiben zu oeffnen
dateizeiger = fopen (dateiname.c_str(), "r+");
// falls nicht vorhanden, eine neue Datei mit Spaltenueberschriften anlegen
if (dateizeiger == NULL)
{
  dateizeiger = fopen (dateiname.c_str(), "w");

fprintf(dateizeiger, "wiederholung;");
fprintf(dateizeiger, "meldung;");
fprintf(dateizeiger, "\n");

if (dateizeiger == NULL)
{
fprintf(stderr, "Fehler: LOGausgabedatei konnte nicht geoeffnet werden!\n");
exit(1);
}
}

// Die neuen Informationen werden ans Ende der Datei geschrieben
fseek(dateizeiger,0,SEEK_END);

// Datenaufbereiten und in die Datei schreiben
fprintf(dateizeiger, "%d;", parameter[0].wiederholung);
fprintf(dateizeiger, "WARNING: mjung Wert nach 1000 loops nicht schaetzbar in den Grenzen = daher Loop abgebrochen und Wert mit %4.5f verwendet;", parameter[0].mjung);
fprintf(dateizeiger, "\n");

fclose (dateizeiger);
// Ende der Ausgabefunktion


wertinboundary=true;
}
}while(wertinboundary==false);
// Parameterwert in Liste einfuegen
Parameterwerte_mjung.push_back(parameter[0].mjung);
//cout << "Anzahl dos=" << anzahldos << endl;

}
 
// setzte parameterwerte fest
if(nruns==0)
{
// Grenzen fuer die Parameter schätzen/festlegen!
double parametermin_jugendmorteinflussexp=-1.52;
double parametermax_jugendmorteinflussexp=2;

// Alle Werte in einem Vektor speichern
Parameterwerte_jugendmorteinflussexp.push_back(parametermin_jugendmorteinflussexp);// nur bei Lauf==1
Parameterwerte_jugendmorteinflussexp.push_back(parametermax_jugendmorteinflussexp);// nur bei Lauf==1

}




// gewichteten Mittelwert fuer jeden Parameter ermitteln
double gewichtetermittelwert_jugendmorteinflussexp=0.0;
for (unsigned int position=0;position<Parameterwerte_jugendmorteinflussexp.size();++position)
{
gewichtetermittelwert_jugendmorteinflussexp+=Parameterwerte_jugendmorteinflussexp[position]*Parametergewichte[position];
}
gewichtetermittelwert_jugendmorteinflussexp=gewichtetermittelwert_jugendmorteinflussexp/summegewichte;

// gewichtete Standardabweichung fuer jeden Parameter ermitteln
double gewichtetersd_jugendmorteinflussexp=0.0;
for (unsigned int position=0;position<Parameterwerte_jugendmorteinflussexp.size();++position)
{
gewichtetersd_jugendmorteinflussexp+= pow(Parameterwerte_jugendmorteinflussexp[position]-gewichtetermittelwert_jugendmorteinflussexp, 2) *  (Parametergewichte[position]/summegewichte);
}
gewichtetersd_jugendmorteinflussexp=pow(gewichtetersd_jugendmorteinflussexp, 0.5);

// Neuen Parameterwert schätzen
for(int posi=0; posi<1; ++posi) // zum Test 100mal
{
bool wertinboundary=false;
int anzahldos=0;
do
{
++anzahldos;

parameter[0].jugendmorteinflussexp=NormverteilungRN(gewichtetermittelwert_jugendmorteinflussexp,gewichtetersd_jugendmorteinflussexp);
//cout << parameter[0].jugendmorteinflussexp << " ";

if(parameter[0].jugendmorteinflussexp>=0 && parameter[0].jugendmorteinflussexp<=2)
{
wertinboundary=true;
}
if(anzahldos>1000)
{
// cout << "WARNING: Wert nicht schaetzbar in den Grenzen = daher Loop abgebrochen" << endl;


//Ausgabe der Warnung in Logdatei

FILE *dateizeiger;
string dateiname;

// Dateinamen zusammensetzen
dateiname="databaeume_LOGDATEI_Parameterization.csv";
 
// Datei versuchen zum Lesen und Schreiben zu oeffnen
dateizeiger = fopen (dateiname.c_str(), "r+");
// falls nicht vorhanden, eine neue Datei mit Spaltenueberschriften anlegen
if (dateizeiger == NULL)
{
  dateizeiger = fopen (dateiname.c_str(), "w");

fprintf(dateizeiger, "wiederholung;");
fprintf(dateizeiger, "meldung;");
fprintf(dateizeiger, "\n");

if (dateizeiger == NULL)
{
fprintf(stderr, "Fehler: LOGausgabedatei konnte nicht geoeffnet werden!\n");
exit(1);
}
}

// Die neuen Informationen werden ans Ende der Datei geschrieben
fseek(dateizeiger,0,SEEK_END);

// Datenaufbereiten und in die Datei schreiben
fprintf(dateizeiger, "%d;", parameter[0].wiederholung);
fprintf(dateizeiger, "WARNING: jugendmorteinflussexp Wert nach 1000 loops nicht schaetzbar in den Grenzen = daher Loop abgebrochen und Wert mit %4.5f verwendet;", parameter[0].jugendmorteinflussexp);
fprintf(dateizeiger, "\n");

fclose (dateizeiger);
// Ende der Ausgabefunktion


wertinboundary=true;
}
}while(wertinboundary==false);
// Parameterwert in Liste einfuegen
Parameterwerte_jugendmorteinflussexp.push_back(parameter[0].jugendmorteinflussexp);
//cout << "Anzahl dos=" << anzahldos << endl;

}
 
// setzte parameterwerte fest
if(nruns==0)
{
// Grenzen fuer die Parameter schätzen/festlegen!
double parametermin_mgrowth=-0.8;
double parametermax_mgrowth=1;

// Alle Werte in einem Vektor speichern
Parameterwerte_mgrowth.push_back(parametermin_mgrowth);// nur bei Lauf==1
Parameterwerte_mgrowth.push_back(parametermax_mgrowth);// nur bei Lauf==1

}




// gewichteten Mittelwert fuer jeden Parameter ermitteln
double gewichtetermittelwert_mgrowth=0.0;
for (unsigned int position=0;position<Parameterwerte_mgrowth.size();++position)
{
gewichtetermittelwert_mgrowth+=Parameterwerte_mgrowth[position]*Parametergewichte[position];
}
gewichtetermittelwert_mgrowth=gewichtetermittelwert_mgrowth/summegewichte;

// gewichtete Standardabweichung fuer jeden Parameter ermitteln
double gewichtetersd_mgrowth=0.0;
for (unsigned int position=0;position<Parameterwerte_mgrowth.size();++position)
{
gewichtetersd_mgrowth+= pow(Parameterwerte_mgrowth[position]-gewichtetermittelwert_mgrowth, 2) *  (Parametergewichte[position]/summegewichte);
}
gewichtetersd_mgrowth=pow(gewichtetersd_mgrowth, 0.5);

// Neuen Parameterwert schätzen
for(int posi=0; posi<1; ++posi) // zum Test 100mal
{
bool wertinboundary=false;
int anzahldos=0;
do
{
++anzahldos;

parameter[0].mgrowth=NormverteilungRN(gewichtetermittelwert_mgrowth,gewichtetersd_mgrowth);
//cout << parameter[0].mgrowth << " ";

if(parameter[0].mgrowth>=0 && parameter[0].mgrowth<=1)
{
wertinboundary=true;
}
if(anzahldos>1000)
{
// cout << "WARNING: Wert nicht schaetzbar in den Grenzen = daher Loop abgebrochen" << endl;


//Ausgabe der Warnung in Logdatei

FILE *dateizeiger;
string dateiname;

// Dateinamen zusammensetzen
dateiname="databaeume_LOGDATEI_Parameterization.csv";
 
// Datei versuchen zum Lesen und Schreiben zu oeffnen
dateizeiger = fopen (dateiname.c_str(), "r+");
// falls nicht vorhanden, eine neue Datei mit Spaltenueberschriften anlegen
if (dateizeiger == NULL)
{
  dateizeiger = fopen (dateiname.c_str(), "w");

fprintf(dateizeiger, "wiederholung;");
fprintf(dateizeiger, "meldung;");
fprintf(dateizeiger, "\n");

if (dateizeiger == NULL)
{
fprintf(stderr, "Fehler: LOGausgabedatei konnte nicht geoeffnet werden!\n");
exit(1);
}
}

// Die neuen Informationen werden ans Ende der Datei geschrieben
fseek(dateizeiger,0,SEEK_END);

// Datenaufbereiten und in die Datei schreiben
fprintf(dateizeiger, "%d;", parameter[0].wiederholung);
fprintf(dateizeiger, "WARNING: mgrowth Wert nach 1000 loops nicht schaetzbar in den Grenzen = daher Loop abgebrochen und Wert mit %4.5f verwendet;", parameter[0].mgrowth);
fprintf(dateizeiger, "\n");

fclose (dateizeiger);
// Ende der Ausgabefunktion


wertinboundary=true;
}
}while(wertinboundary==false);
// Parameterwert in Liste einfuegen
Parameterwerte_mgrowth.push_back(parameter[0].mgrowth);
//cout << "Anzahl dos=" << anzahldos << endl;

}
 
// setzte parameterwerte fest
if(nruns==0)
{
// Grenzen fuer die Parameter schätzen/festlegen!
double parametermin_mdichte=-0.8;
double parametermax_mdichte=1;

// Alle Werte in einem Vektor speichern
Parameterwerte_mdichte.push_back(parametermin_mdichte);// nur bei Lauf==1
Parameterwerte_mdichte.push_back(parametermax_mdichte);// nur bei Lauf==1

}




// gewichteten Mittelwert fuer jeden Parameter ermitteln
double gewichtetermittelwert_mdichte=0.0;
for (unsigned int position=0;position<Parameterwerte_mdichte.size();++position)
{
gewichtetermittelwert_mdichte+=Parameterwerte_mdichte[position]*Parametergewichte[position];
}
gewichtetermittelwert_mdichte=gewichtetermittelwert_mdichte/summegewichte;

// gewichtete Standardabweichung fuer jeden Parameter ermitteln
double gewichtetersd_mdichte=0.0;
for (unsigned int position=0;position<Parameterwerte_mdichte.size();++position)
{
gewichtetersd_mdichte+= pow(Parameterwerte_mdichte[position]-gewichtetermittelwert_mdichte, 2) *  (Parametergewichte[position]/summegewichte);
}
gewichtetersd_mdichte=pow(gewichtetersd_mdichte, 0.5);

// Neuen Parameterwert schätzen
for(int posi=0; posi<1; ++posi) // zum Test 100mal
{
bool wertinboundary=false;
int anzahldos=0;
do
{
++anzahldos;

parameter[0].mdichte=NormverteilungRN(gewichtetermittelwert_mdichte,gewichtetersd_mdichte);
//cout << parameter[0].mdichte << " ";

if(parameter[0].mdichte>=0 && parameter[0].mdichte<=1)
{
wertinboundary=true;
}
if(anzahldos>1000)
{
// cout << "WARNING: Wert nicht schaetzbar in den Grenzen = daher Loop abgebrochen" << endl;


//Ausgabe der Warnung in Logdatei

FILE *dateizeiger;
string dateiname;

// Dateinamen zusammensetzen
dateiname="databaeume_LOGDATEI_Parameterization.csv";
 
// Datei versuchen zum Lesen und Schreiben zu oeffnen
dateizeiger = fopen (dateiname.c_str(), "r+");
// falls nicht vorhanden, eine neue Datei mit Spaltenueberschriften anlegen
if (dateizeiger == NULL)
{
  dateizeiger = fopen (dateiname.c_str(), "w");

fprintf(dateizeiger, "wiederholung;");
fprintf(dateizeiger, "meldung;");
fprintf(dateizeiger, "\n");

if (dateizeiger == NULL)
{
fprintf(stderr, "Fehler: LOGausgabedatei konnte nicht geoeffnet werden!\n");
exit(1);
}
}

// Die neuen Informationen werden ans Ende der Datei geschrieben
fseek(dateizeiger,0,SEEK_END);

// Datenaufbereiten und in die Datei schreiben
fprintf(dateizeiger, "%d;", parameter[0].wiederholung);
fprintf(dateizeiger, "WARNING: mdichte Wert nach 1000 loops nicht schaetzbar in den Grenzen = daher Loop abgebrochen und Wert mit %4.5f verwendet;", parameter[0].mdichte);
fprintf(dateizeiger, "\n");

fclose (dateizeiger);
// Ende der Ausgabefunktion


wertinboundary=true;
}
}while(wertinboundary==false);
// Parameterwert in Liste einfuegen
Parameterwerte_mdichte.push_back(parameter[0].mdichte);
//cout << "Anzahl dos=" << anzahldos << endl;

}
 
// setzte parameterwerte fest
if(nruns==0)
{
// Grenzen fuer die Parameter schätzen/festlegen!
double parametermin_mwetter=0;
double parametermax_mwetter=1;

// Alle Werte in einem Vektor speichern
Parameterwerte_mwetter.push_back(parametermin_mwetter);// nur bei Lauf==1
Parameterwerte_mwetter.push_back(parametermax_mwetter);// nur bei Lauf==1

}




// gewichteten Mittelwert fuer jeden Parameter ermitteln
double gewichtetermittelwert_mwetter=0.0;
for (unsigned int position=0;position<Parameterwerte_mwetter.size();++position)
{
gewichtetermittelwert_mwetter+=Parameterwerte_mwetter[position]*Parametergewichte[position];
}
gewichtetermittelwert_mwetter=gewichtetermittelwert_mwetter/summegewichte;

// gewichtete Standardabweichung fuer jeden Parameter ermitteln
double gewichtetersd_mwetter=0.0;
for (unsigned int position=0;position<Parameterwerte_mwetter.size();++position)
{
gewichtetersd_mwetter+= pow(Parameterwerte_mwetter[position]-gewichtetermittelwert_mwetter, 2) *  (Parametergewichte[position]/summegewichte);
}
gewichtetersd_mwetter=pow(gewichtetersd_mwetter, 0.5);

// Neuen Parameterwert schätzen
for(int posi=0; posi<1; ++posi) // zum Test 100mal
{
bool wertinboundary=false;
int anzahldos=0;
do
{
++anzahldos;

parameter[0].mwetter=NormverteilungRN(gewichtetermittelwert_mwetter,gewichtetersd_mwetter);
//cout << parameter[0].mwetter << " ";

if(parameter[0].mwetter>=0 && parameter[0].mwetter<=1)
{
wertinboundary=true;
}
if(anzahldos>1000)
{
// cout << "WARNING: Wert nicht schaetzbar in den Grenzen = daher Loop abgebrochen" << endl;


//Ausgabe der Warnung in Logdatei

FILE *dateizeiger;
string dateiname;

// Dateinamen zusammensetzen
dateiname="databaeume_LOGDATEI_Parameterization.csv";
 
// Datei versuchen zum Lesen und Schreiben zu oeffnen
dateizeiger = fopen (dateiname.c_str(), "r+");
// falls nicht vorhanden, eine neue Datei mit Spaltenueberschriften anlegen
if (dateizeiger == NULL)
{
  dateizeiger = fopen (dateiname.c_str(), "w");

fprintf(dateizeiger, "wiederholung;");
fprintf(dateizeiger, "meldung;");
fprintf(dateizeiger, "\n");

if (dateizeiger == NULL)
{
fprintf(stderr, "Fehler: LOGausgabedatei konnte nicht geoeffnet werden!\n");
exit(1);
}
}

// Die neuen Informationen werden ans Ende der Datei geschrieben
fseek(dateizeiger,0,SEEK_END);

// Datenaufbereiten und in die Datei schreiben
fprintf(dateizeiger, "%d;", parameter[0].wiederholung);
fprintf(dateizeiger, "WARNING: mwetter Wert nach 1000 loops nicht schaetzbar in den Grenzen = daher Loop abgebrochen und Wert mit %4.5f verwendet;", parameter[0].mwetter);
fprintf(dateizeiger, "\n");

fclose (dateizeiger);
// Ende der Ausgabefunktion


wertinboundary=true;
}
}while(wertinboundary==false);
// Parameterwert in Liste einfuegen
Parameterwerte_mwetter.push_back(parameter[0].mwetter);
//cout << "Anzahl dos=" << anzahldos << endl;

}
 
// setzte parameterwerte fest
if(nruns==0)
{
// Grenzen fuer die Parameter schätzen/festlegen!
double parametermin_mtrockenheit=-0.4;
double parametermax_mtrockenheit=1;

// Alle Werte in einem Vektor speichern
Parameterwerte_mtrockenheit.push_back(parametermin_mtrockenheit);// nur bei Lauf==1
Parameterwerte_mtrockenheit.push_back(parametermax_mtrockenheit);// nur bei Lauf==1

}




// gewichteten Mittelwert fuer jeden Parameter ermitteln
double gewichtetermittelwert_mtrockenheit=0.0;
for (unsigned int position=0;position<Parameterwerte_mtrockenheit.size();++position)
{
gewichtetermittelwert_mtrockenheit+=Parameterwerte_mtrockenheit[position]*Parametergewichte[position];
}
gewichtetermittelwert_mtrockenheit=gewichtetermittelwert_mtrockenheit/summegewichte;

// gewichtete Standardabweichung fuer jeden Parameter ermitteln
double gewichtetersd_mtrockenheit=0.0;
for (unsigned int position=0;position<Parameterwerte_mtrockenheit.size();++position)
{
gewichtetersd_mtrockenheit+= pow(Parameterwerte_mtrockenheit[position]-gewichtetermittelwert_mtrockenheit, 2) *  (Parametergewichte[position]/summegewichte);
}
gewichtetersd_mtrockenheit=pow(gewichtetersd_mtrockenheit, 0.5);

// Neuen Parameterwert schätzen
for(int posi=0; posi<1; ++posi) // zum Test 100mal
{
bool wertinboundary=false;
int anzahldos=0;
do
{
++anzahldos;

parameter[0].mtrockenheit=NormverteilungRN(gewichtetermittelwert_mtrockenheit,gewichtetersd_mtrockenheit);
//cout << parameter[0].mtrockenheit << " ";

if(parameter[0].mtrockenheit>=0 && parameter[0].mtrockenheit<=1)
{
wertinboundary=true;
}
if(anzahldos>1000)
{
// cout << "WARNING: Wert nicht schaetzbar in den Grenzen = daher Loop abgebrochen" << endl;


//Ausgabe der Warnung in Logdatei

FILE *dateizeiger;
string dateiname;

// Dateinamen zusammensetzen
dateiname="databaeume_LOGDATEI_Parameterization.csv";
 
// Datei versuchen zum Lesen und Schreiben zu oeffnen
dateizeiger = fopen (dateiname.c_str(), "r+");
// falls nicht vorhanden, eine neue Datei mit Spaltenueberschriften anlegen
if (dateizeiger == NULL)
{
  dateizeiger = fopen (dateiname.c_str(), "w");

fprintf(dateizeiger, "wiederholung;");
fprintf(dateizeiger, "meldung;");
fprintf(dateizeiger, "\n");

if (dateizeiger == NULL)
{
fprintf(stderr, "Fehler: LOGausgabedatei konnte nicht geoeffnet werden!\n");
exit(1);
}
}

// Die neuen Informationen werden ans Ende der Datei geschrieben
fseek(dateizeiger,0,SEEK_END);

// Datenaufbereiten und in die Datei schreiben
fprintf(dateizeiger, "%d;", parameter[0].wiederholung);
fprintf(dateizeiger, "WARNING: mtrockenheit Wert nach 1000 loops nicht schaetzbar in den Grenzen = daher Loop abgebrochen und Wert mit %4.5f verwendet;", parameter[0].mtrockenheit);
fprintf(dateizeiger, "\n");

fclose (dateizeiger);
// Ende der Ausgabefunktion


wertinboundary=true;
}
}while(wertinboundary==false);
// Parameterwert in Liste einfuegen
Parameterwerte_mtrockenheit.push_back(parameter[0].mtrockenheit);
//cout << "Anzahl dos=" << anzahldos << endl;

}
 
// setzte parameterwerte fest
if(nruns==0)
{
// Grenzen fuer die Parameter schätzen/festlegen!
double parametermin_samenbaummort=0;
double parametermax_samenbaummort=1.6;

// Alle Werte in einem Vektor speichern
Parameterwerte_samenbaummort.push_back(parametermin_samenbaummort);// nur bei Lauf==1
Parameterwerte_samenbaummort.push_back(parametermax_samenbaummort);// nur bei Lauf==1

}




// gewichteten Mittelwert fuer jeden Parameter ermitteln
double gewichtetermittelwert_samenbaummort=0.0;
for (unsigned int position=0;position<Parameterwerte_samenbaummort.size();++position)
{
gewichtetermittelwert_samenbaummort+=Parameterwerte_samenbaummort[position]*Parametergewichte[position];
}
gewichtetermittelwert_samenbaummort=gewichtetermittelwert_samenbaummort/summegewichte;

// gewichtete Standardabweichung fuer jeden Parameter ermitteln
double gewichtetersd_samenbaummort=0.0;
for (unsigned int position=0;position<Parameterwerte_samenbaummort.size();++position)
{
gewichtetersd_samenbaummort+= pow(Parameterwerte_samenbaummort[position]-gewichtetermittelwert_samenbaummort, 2) *  (Parametergewichte[position]/summegewichte);
}
gewichtetersd_samenbaummort=pow(gewichtetersd_samenbaummort, 0.5);

// Neuen Parameterwert schätzen
for(int posi=0; posi<1; ++posi) // zum Test 100mal
{
bool wertinboundary=false;
int anzahldos=0;
do
{
++anzahldos;

parameter[0].samenbaummort=NormverteilungRN(gewichtetermittelwert_samenbaummort,gewichtetersd_samenbaummort);
//cout << parameter[0].samenbaummort << " ";

if(parameter[0].samenbaummort>=0 && parameter[0].samenbaummort<=1)
{
wertinboundary=true;
}
if(anzahldos>1000)
{
// cout << "WARNING: Wert nicht schaetzbar in den Grenzen = daher Loop abgebrochen" << endl;


//Ausgabe der Warnung in Logdatei

FILE *dateizeiger;
string dateiname;

// Dateinamen zusammensetzen
dateiname="databaeume_LOGDATEI_Parameterization.csv";
 
// Datei versuchen zum Lesen und Schreiben zu oeffnen
dateizeiger = fopen (dateiname.c_str(), "r+");
// falls nicht vorhanden, eine neue Datei mit Spaltenueberschriften anlegen
if (dateizeiger == NULL)
{
  dateizeiger = fopen (dateiname.c_str(), "w");

fprintf(dateizeiger, "wiederholung;");
fprintf(dateizeiger, "meldung;");
fprintf(dateizeiger, "\n");

if (dateizeiger == NULL)
{
fprintf(stderr, "Fehler: LOGausgabedatei konnte nicht geoeffnet werden!\n");
exit(1);
}
}

// Die neuen Informationen werden ans Ende der Datei geschrieben
fseek(dateizeiger,0,SEEK_END);

// Datenaufbereiten und in die Datei schreiben
fprintf(dateizeiger, "%d;", parameter[0].wiederholung);
fprintf(dateizeiger, "WARNING: samenbaummort Wert nach 1000 loops nicht schaetzbar in den Grenzen = daher Loop abgebrochen und Wert mit %4.5f verwendet;", parameter[0].samenbaummort);
fprintf(dateizeiger, "\n");

fclose (dateizeiger);
// Ende der Ausgabefunktion


wertinboundary=true;
}
}while(wertinboundary==false);
// Parameterwert in Liste einfuegen
Parameterwerte_samenbaummort.push_back(parameter[0].samenbaummort);
//cout << "Anzahl dos=" << anzahldos << endl;

}
 
// setzte parameterwerte fest
if(nruns==0)
{
// Grenzen fuer die Parameter schätzen/festlegen!
double parametermin_samenbodenmort=0;
double parametermax_samenbodenmort=1.6;

// Alle Werte in einem Vektor speichern
Parameterwerte_samenbodenmort.push_back(parametermin_samenbodenmort);// nur bei Lauf==1
Parameterwerte_samenbodenmort.push_back(parametermax_samenbodenmort);// nur bei Lauf==1

}




// gewichteten Mittelwert fuer jeden Parameter ermitteln
double gewichtetermittelwert_samenbodenmort=0.0;
for (unsigned int position=0;position<Parameterwerte_samenbodenmort.size();++position)
{
gewichtetermittelwert_samenbodenmort+=Parameterwerte_samenbodenmort[position]*Parametergewichte[position];
}
gewichtetermittelwert_samenbodenmort=gewichtetermittelwert_samenbodenmort/summegewichte;

// gewichtete Standardabweichung fuer jeden Parameter ermitteln
double gewichtetersd_samenbodenmort=0.0;
for (unsigned int position=0;position<Parameterwerte_samenbodenmort.size();++position)
{
gewichtetersd_samenbodenmort+= pow(Parameterwerte_samenbodenmort[position]-gewichtetermittelwert_samenbodenmort, 2) *  (Parametergewichte[position]/summegewichte);
}
gewichtetersd_samenbodenmort=pow(gewichtetersd_samenbodenmort, 0.5);

// Neuen Parameterwert schätzen
for(int posi=0; posi<1; ++posi) // zum Test 100mal
{
bool wertinboundary=false;
int anzahldos=0;
do
{
++anzahldos;

parameter[0].samenbodenmort=NormverteilungRN(gewichtetermittelwert_samenbodenmort,gewichtetersd_samenbodenmort);
//cout << parameter[0].samenbodenmort << " ";

if(parameter[0].samenbodenmort>=0 && parameter[0].samenbodenmort<=1)
{
wertinboundary=true;
}
if(anzahldos>1000)
{
// cout << "WARNING: Wert nicht schaetzbar in den Grenzen = daher Loop abgebrochen" << endl;


//Ausgabe der Warnung in Logdatei

FILE *dateizeiger;
string dateiname;

// Dateinamen zusammensetzen
dateiname="databaeume_LOGDATEI_Parameterization.csv";
 
// Datei versuchen zum Lesen und Schreiben zu oeffnen
dateizeiger = fopen (dateiname.c_str(), "r+");
// falls nicht vorhanden, eine neue Datei mit Spaltenueberschriften anlegen
if (dateizeiger == NULL)
{
  dateizeiger = fopen (dateiname.c_str(), "w");

fprintf(dateizeiger, "wiederholung;");
fprintf(dateizeiger, "meldung;");
fprintf(dateizeiger, "\n");

if (dateizeiger == NULL)
{
fprintf(stderr, "Fehler: LOGausgabedatei konnte nicht geoeffnet werden!\n");
exit(1);
}
}

// Die neuen Informationen werden ans Ende der Datei geschrieben
fseek(dateizeiger,0,SEEK_END);

// Datenaufbereiten und in die Datei schreiben
fprintf(dateizeiger, "%d;", parameter[0].wiederholung);
fprintf(dateizeiger, "WARNING: samenbodenmort Wert nach 1000 loops nicht schaetzbar in den Grenzen = daher Loop abgebrochen und Wert mit %4.5f verwendet;", parameter[0].samenbodenmort);
fprintf(dateizeiger, "\n");

fclose (dateizeiger);
// Ende der Ausgabefunktion


wertinboundary=true;
}
}while(wertinboundary==false);
// Parameterwert in Liste einfuegen
Parameterwerte_samenbodenmort.push_back(parameter[0].samenbodenmort);
//cout << "Anzahl dos=" << anzahldos << endl;

}
 
 
 
 




// setzte parameterwerte fest
if(nruns==0)
{
// Grenzen fuer die Parameter schätzen/festlegen!
double parametermin_incfac=-80;
double parametermax_incfac=100;

// Alle Werte in einem Vektor speichern
Parameterwerte_incfac.push_back(parametermin_incfac);// nur bei Lauf==1
Parameterwerte_incfac.push_back(parametermax_incfac);// nur bei Lauf==1

}

// gewichteten Mittelwert fuer jeden Parameter ermitteln
double gewichtetermittelwert_incfac=0.0;
for (unsigned int position=0;position<Parameterwerte_incfac.size();++position)
{
gewichtetermittelwert_incfac+=Parameterwerte_incfac[position]*Parametergewichte[position];
}
gewichtetermittelwert_incfac=gewichtetermittelwert_incfac/summegewichte;

// gewichtete Standardabweichung fuer jeden Parameter ermitteln
double gewichtetersd_incfac=0.0;
for (unsigned int position=0;position<Parameterwerte_incfac.size();++position)
{
gewichtetersd_incfac+= pow(Parameterwerte_incfac[position]-gewichtetermittelwert_incfac, 2) *  (Parametergewichte[position]/summegewichte);
}
gewichtetersd_incfac=pow(gewichtetersd_incfac, 0.5);

// Neuen Parameterwert schätzen
for(int posi=0; posi<1; ++posi) // zum Test 100mal
{
bool wertinboundary=false;
int anzahldos=0;
do
{
++anzahldos;

parameter[0].incfac=NormverteilungRN(gewichtetermittelwert_incfac,gewichtetersd_incfac);
//cout << parameter[0].incfac << " ";

if(parameter[0].incfac>0 && parameter[0].incfac<=100)
{
wertinboundary=true;
}
if(anzahldos>1000)
{
// cout << "WARNING: Wert nicht schaetzbar in den Grenzen = daher Loop abgebrochen" << endl;


//Ausgabe der Warnung in Logdatei

FILE *dateizeiger;
string dateiname;

// Dateinamen zusammensetzen
dateiname="databaeume_LOGDATEI_Parameterization.csv";
 
// Datei versuchen zum Lesen und Schreiben zu oeffnen
dateizeiger = fopen (dateiname.c_str(), "r+");
// falls nicht vorhanden, eine neue Datei mit Spaltenueberschriften anlegen
if (dateizeiger == NULL)
{
  dateizeiger = fopen (dateiname.c_str(), "w");

fprintf(dateizeiger, "wiederholung;");
fprintf(dateizeiger, "meldung;");
fprintf(dateizeiger, "\n");

if (dateizeiger == NULL)
{
fprintf(stderr, "Fehler: LOGausgabedatei konnte nicht geoeffnet werden!\n");
exit(1);
}
}

// Die neuen Informationen werden ans Ende der Datei geschrieben
fseek(dateizeiger,0,SEEK_END);

// Datenaufbereiten und in die Datei schreiben
fprintf(dateizeiger, "%d;", parameter[0].wiederholung);
fprintf(dateizeiger, "WARNING: incfac Wert nach 1000 loops nicht schaetzbar in den Grenzen = daher Loop abgebrochen und Wert mit %4.5f verwendet;", parameter[0].incfac);
fprintf(dateizeiger, "\n");

fclose (dateizeiger);
// Ende der Ausgabefunktion


wertinboundary=true;
}
}while(wertinboundary==false);
// Parameterwert in Liste einfuegen
Parameterwerte_incfac.push_back(parameter[0].incfac);
//cout << "Anzahl dos=" << anzahldos << endl;

}
 

*/



/*

//Ausgabe der Parameterschaetzungen
FILE *dateizeiger;
string dateiname;

// Dateinamen zusammensetzen
dateiname="databaeume_parameterschaetzungen_Parameterization.csv";
 
// Datei versuchen zum Lesen und Schreiben zu oeffnen
dateizeiger = fopen (dateiname.c_str(), "r+");
// falls nicht vorhanden, eine neue Datei mit Spaltenueberschriften anlegen
if (dateizeiger == NULL)
{
  dateizeiger = fopen (dateiname.c_str(), "w");
fprintf(dateizeiger, "wiederholung;");
 
fprintf(dateizeiger, "precindexstrength;"); 
fprintf(dateizeiger, "aatnddratio;"); 
fprintf(dateizeiger, "samenflugrate;"); 
fprintf(dateizeiger, "entfernungsteiler;"); 
fprintf(dateizeiger, "samenprodfaktor;"); 
fprintf(dateizeiger, "keimungwettereinfluss;"); 
fprintf(dateizeiger, "keimungsrate;"); 
fprintf(dateizeiger, "precgrenzwert;"); 
fprintf(dateizeiger, "mortbg;"); 
fprintf(dateizeiger, "malt;"); 
fprintf(dateizeiger, "mjung;"); 
fprintf(dateizeiger, "jugendmorteinflussexp;"); 
fprintf(dateizeiger, "mgrowth;"); 
fprintf(dateizeiger, "mdichte;"); 
fprintf(dateizeiger, "mwetter;"); 
fprintf(dateizeiger, "mtrockenheit;"); 
fprintf(dateizeiger, "samenbaummort;"); 
fprintf(dateizeiger, "samenbodenmort;"); 
fprintf(dateizeiger, "incfac;");
fprintf(dateizeiger, "\n");

if (dateizeiger == NULL)
{
fprintf(stderr, "Fehler: Parameterausgabedatei konnte nicht geoeffnet werden!\n");
exit(1);
}
}

// Die neuen Informationen werden ans Ende der Datei geschrieben
fseek(dateizeiger,0,SEEK_END);

// Datenaufbereiten und in die Datei schreiben
fprintf(dateizeiger, "%d;", parameter[0].wiederholung);
 
fprintf(dateizeiger, "%4.5f;", parameter[0].precindexstrength); 
fprintf(dateizeiger, "%4.5f;", parameter[0].aatnddratio); 
fprintf(dateizeiger, "%4.5f;", parameter[0].samenflugrate); 
fprintf(dateizeiger, "%4.5f;", parameter[0].entfernungsteiler); 
fprintf(dateizeiger, "%4.5f;", parameter[0].samenprodfaktor); 
fprintf(dateizeiger, "%4.5f;", parameter[0].keimungwettereinfluss); 
fprintf(dateizeiger, "%4.5f;", parameter[0].keimungsrate); 
fprintf(dateizeiger, "%4.5f;", parameter[0].precgrenzwert); 
fprintf(dateizeiger, "%4.5f;", parameter[0].mortbg); 
fprintf(dateizeiger, "%4.5f;", parameter[0].malt); 
fprintf(dateizeiger, "%4.5f;", parameter[0].mjung); 
fprintf(dateizeiger, "%4.5f;", parameter[0].jugendmorteinflussexp); 
fprintf(dateizeiger, "%4.5f;", parameter[0].mgrowth); 
fprintf(dateizeiger, "%4.5f;", parameter[0].mdichte); 
fprintf(dateizeiger, "%4.5f;", parameter[0].mwetter); 
fprintf(dateizeiger, "%4.5f;", parameter[0].mtrockenheit); 
fprintf(dateizeiger, "%4.5f;", parameter[0].samenbaummort); 
fprintf(dateizeiger, "%4.5f;", parameter[0].samenbodenmort); 
fprintf(dateizeiger, "%4.5f;", parameter[0].incfac);
fprintf(dateizeiger, "\n");

fclose (dateizeiger);
// Ende der Ausgabefunktion

*/
