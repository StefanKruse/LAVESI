/****************************************************************************************//**
 * \brief  
 *
 *
 *******************************************************************************************/
void getTemp1(int aktort, char dateinametemp[50],vector<weather*>& weather_list)
{				
	
	// Berechnung des aktuellen Ortes
	int aktortyworldcoo=(int) floor( (double) (aktort-1)/parameter[0].mapxlength );
	int aktortxworldcoo=(aktort-1) - (aktortyworldcoo * parameter[0].mapxlength);

	// 2. N_inGrad aus den N_max und N_min und Ygesamt-werten berechnen
	if (parameter[0].mapylength>1 && parameter[0].weathercalcgradient==true)
	{
		double Nposcenter=(parameter[0].Nposmax+parameter[0].Nposmin)/2;
		double mapylengthdummy=parameter[0].mapylength;	// irgendwie funktioniert nicht die direkte Verwendung in der nachfolgenden Formel??
		double Nposakt=parameter[0].Nposmax-( (parameter[0].Nposmax-parameter[0].Nposmin)*aktortyworldcoo/(mapylengthdummy-1.0) );
		// 3. Differenzwerte berechnen mit Prec=-5.3699 pro Grad und Temp=-0.3508 pro Grad
		parameter[0].tempdiffort=-0.3508 * (Nposakt-Nposcenter);
		parameter[0].precdiffort=-5.3699 * (Nposakt-Nposcenter);
	}


	if (parameter[0].lineartransekt==true)
	{
		// 3. Differenzwerte berechnen mit Prec=-5.3699 pro Grad und Temp=-0.3508 pro Grad
		// Umrechnung von Grad in Meter  "1 Grad = 60 x 1852 m = 111120 m"
		parameter[0].tempdiffortmin=-0.3508 * treerows/(111120);
		parameter[0].precdiffortmin=-5.3699 * treerows/(111120);
	}



	// Datei oeffnen
	FILE *f;
	f = fopen(dateinametemp,"r"); 
	if (f == NULL)
	{
		printf("Temperaturdatei nicht vorhanden!\n");
		exit(1);
	}
	
	char puffer[255];
	int counter=1;
	double tempjahrmittelbuf, temp1monatmittelbuf, temp2monatmittelbuf, temp3monatmittelbuf, temp4monatmittelbuf, temp5monatmittelbuf, temp6monatmittelbuf, temp7monatmittelbuf, temp8monatmittelbuf, temp9monatmittelbuf, temp10monatmittelbuf, temp11monatmittelbuf, temp12monatmittelbuf;

	// Zeilenweise einlesen und entsprechende Spalteninhalte auslesen und teilweise verarbeiten
	while( fgets(puffer,255,f) !=NULL)
	{

	
		if (counter>=2)	// Erste Spalte wird ausgelassen, da dort Beschriftungen liegen
		{
			temp1monatmittelbuf= strtod(strtok(puffer, " "),NULL);
			temp2monatmittelbuf= strtod(strtok(NULL, " "),NULL);
			temp3monatmittelbuf= strtod(strtok(NULL, " "),NULL) ;
			temp4monatmittelbuf= strtod(strtok(NULL, " "),NULL);
			temp5monatmittelbuf= strtod(strtok(NULL, " "),NULL) ;
			temp6monatmittelbuf= strtod(strtok(NULL, " "),NULL) ;
			temp7monatmittelbuf= strtod(strtok(NULL, " "),NULL);
			temp8monatmittelbuf= strtod(strtok(NULL, " "),NULL) ;
			temp9monatmittelbuf= strtod(strtok(NULL, " "),NULL) ;
			temp10monatmittelbuf= strtod(strtok(NULL, " "),NULL) ;
			temp11monatmittelbuf= strtod(strtok(NULL, " "),NULL) ;
			temp12monatmittelbuf= strtod(strtok(NULL, " "),NULL) ;

			tempjahrmittelbuf=(temp1monatmittelbuf+temp2monatmittelbuf+temp3monatmittelbuf+temp4monatmittelbuf+temp5monatmittelbuf+temp6monatmittelbuf+temp7monatmittelbuf+temp8monatmittelbuf+temp9monatmittelbuf+temp10monatmittelbuf+temp11monatmittelbuf+temp12monatmittelbuf)/12;

			// neues Jahr hinzufuegen
			pweather= new weather();			// 1. Neues Jahr erzeugen
			pweather->yworldcoo=aktortyworldcoo;
			pweather->xworldcoo=aktortxworldcoo;
			pweather->jahr=counter+parameter[0].startjahr-2;					// 2. Werte dem Jahr zuweisen
			pweather->tempjahrmittel=tempjahrmittelbuf+parameter[0].tempdiffort;
			pweather->temp1monatmittel=temp1monatmittelbuf+parameter[0].tempdiffort;
			pweather->temp1monatmittelmin=temp1monatmittelbuf+parameter[0].tempdiffort+parameter[0].tempdiffortmin;
			pweather->temp2monatmittel=temp2monatmittelbuf+parameter[0].tempdiffort;
			pweather->temp3monatmittel=temp3monatmittelbuf+parameter[0].tempdiffort;
			pweather->temp4monatmittel=temp4monatmittelbuf+parameter[0].tempdiffort;
			pweather->temp5monatmittel=temp5monatmittelbuf+parameter[0].tempdiffort;
			pweather->temp6monatmittel=temp6monatmittelbuf+parameter[0].tempdiffort;
			pweather->temp7monatmittel=temp7monatmittelbuf+parameter[0].tempdiffort;
			pweather->temp7monatmittelmin=temp7monatmittelbuf+parameter[0].tempdiffort+parameter[0].tempdiffortmin;
			pweather->temp8monatmittel=temp8monatmittelbuf+parameter[0].tempdiffort;
			pweather->temp9monatmittel=temp9monatmittelbuf+parameter[0].tempdiffort;
			pweather->temp10monatmittel=temp10monatmittelbuf+parameter[0].tempdiffort;
			pweather->temp11monatmittel=temp11monatmittelbuf+parameter[0].tempdiffort;
			pweather->temp12monatmittel=temp12monatmittelbuf+parameter[0].tempdiffort;
			
			// Active Air Temperature AAT = Summe der Temperatur von Tagen die > 10°C liegen
			// Number of days with temperatures above zero degrees = net degree days = NDD = Anzahl Tage > 0°C ("Vegetationslaenge")
			double sumacttemp=0, sumacttempmin=0, sumdegreday=0;
			int ndegreday=0, ndegredaymin=0;

			// Schätzung der Tagesmitteltemperatur aus der Julimitteltemperatur
			double julitemp=pweather->temp7monatmittel;
			double julitempmin=pweather->temp7monatmittelmin;
			
			for (int i=1;i<=365;i++)
			{
				
				double tagestemp=0;

				if (parameter[0].weatherchoice<10)
				{
					tagestemp=((20.5*sin(   ( 0.61*( (double) i/(365.0/12.0)) )+3.8   ))-21.0+julitemp);
				}	
				else
				{
					tagestemp=(julitemp+24*sin((2*M_PI)/365*(i-100)))-25;
				}
				
				if (tagestemp>0)
				{
					ndegreday++;
					sumdegreday+=tagestemp;
					if (tagestemp>10)
					{
						sumacttemp=sumacttemp+tagestemp;
					}
				}
				double tagestempmin=tagestemp-julitemp+julitempmin;
				if (tagestempmin>0)
				{
					ndegredaymin++;
					if (tagestempmin>10)
					{
						sumacttempmin=sumacttempmin+tagestempmin;
					}
				}
			}
			pweather->activeairtemp=sumacttemp;
			pweather->vegetationslaenge=ndegreday;
			pweather->activeairtempmin=sumacttempmin;
			pweather->vegetationslaengemin=ndegredaymin;
			pweather->degreday=sumdegreday;
			weather_list.push_back(pweather);// 3. Jahr in Liste einfuegen

		}
		counter++;
						
	}
	
	fclose(f);
}



/****************************************************************************************//**
 * \brief 
 *
 *
 *******************************************************************************************/
void getPrec1(char dateinameprec[50],vector<weather*>& weather_list)
{
			FILE *fp;
			fp = fopen(dateinameprec,"r");

			if (fp == NULL)
			{
				printf("Niederschlagsdatei nicht vorhanden!\n");
				exit(1);
			}

			int counter=1;
			char puffer[255];
			double niederschlagssummebuf;
			double prec1monatmittelbuf, prec2monatmittelbuf, prec3monatmittelbuf, prec4monatmittelbuf, prec5monatmittelbuf, prec6monatmittelbuf, prec7monatmittelbuf, prec8monatmittelbuf, prec9monatmittelbuf, prec10monatmittelbuf, prec11monatmittelbuf, prec12monatmittelbuf;

			while( fgets(puffer,wortlaengemax,fp) !=NULL)
			{
				if (counter>=2)
				{
					prec1monatmittelbuf= strtod(strtok(puffer, " "),NULL);
					prec2monatmittelbuf= strtod(strtok(NULL, " "),NULL);
					prec3monatmittelbuf= strtod(strtok(NULL, " "),NULL);
					prec4monatmittelbuf= strtod(strtok(NULL, " "),NULL);
					prec5monatmittelbuf= strtod(strtok(NULL, " "),NULL);
					prec6monatmittelbuf= strtod(strtok(NULL, " "),NULL);
					prec7monatmittelbuf= strtod(strtok(NULL, " "),NULL);
					prec8monatmittelbuf= strtod(strtok(NULL, " "),NULL);
					prec9monatmittelbuf= strtod(strtok(NULL, " "),NULL);
					prec10monatmittelbuf= strtod(strtok(NULL, " "),NULL);
					prec11monatmittelbuf= strtod(strtok(NULL, " "),NULL);
					prec12monatmittelbuf= strtod(strtok(NULL, " "),NULL);

					niederschlagssummebuf=(prec1monatmittelbuf+prec2monatmittelbuf+prec3monatmittelbuf+prec4monatmittelbuf+prec5monatmittelbuf+prec6monatmittelbuf+prec7monatmittelbuf+prec8monatmittelbuf+prec9monatmittelbuf+prec10monatmittelbuf+prec11monatmittelbuf+prec12monatmittelbuf);

					// Werte zu aktuellem Jahr hinzufuegen
					pweather=weather_list[counter-2];			// 1. Zum aktuellen Jahr springen
					pweather->prec1monatmittel=prec1monatmittelbuf+parameter[0].precdiffort/12;					// 2. Werte dem Jahr hinzufuegen
					pweather->prec2monatmittel=prec2monatmittelbuf+parameter[0].precdiffort/12;
					pweather->prec3monatmittel=prec3monatmittelbuf+parameter[0].precdiffort/12;
					pweather->prec4monatmittel=prec4monatmittelbuf+parameter[0].precdiffort/12;
					pweather->prec5monatmittel=prec5monatmittelbuf+parameter[0].precdiffort/12;
					pweather->prec6monatmittel=prec6monatmittelbuf+parameter[0].precdiffort/12;
					pweather->prec7monatmittel=prec7monatmittelbuf+parameter[0].precdiffort/12;
					pweather->prec8monatmittel=prec8monatmittelbuf+parameter[0].precdiffort/12;
					pweather->prec9monatmittel=prec9monatmittelbuf+parameter[0].precdiffort/12;
					pweather->prec10monatmittel=prec10monatmittelbuf+parameter[0].precdiffort/12;
					pweather->prec11monatmittel=prec11monatmittelbuf+parameter[0].precdiffort/12;
					pweather->prec12monatmittel=prec12monatmittelbuf+parameter[0].precdiffort/12;
					pweather->niederschlagssumme=niederschlagssummebuf+parameter[0].precdiffort;
					pweather->niederschlagssummemin=niederschlagssummebuf+parameter[0].precdiffort+parameter[0].precdiffortmin;	// Berechnung oben

				}
				counter++;
			}
			
			fclose(fp);
}



/****************************************************************************************//**
 * \brief 
 *
 *
 *******************************************************************************************/
void getTemp2(int aktort, char dateinametemp[50],vector<weather*>& weather_list)
{
			// Berechnung des aktuellen Ortes
			int aktortyworldcoo=(int) floor( (double) (aktort-1)/parameter[0].mapxlength );
			int aktortxworldcoo=(aktort-1) - (aktortyworldcoo * parameter[0].mapxlength);

			// 2. N_inGrad aus den N_max und N_min und Ygesamt-werten berechnen
			if (parameter[0].mapylength>1 && parameter[0].weathercalcgradient==true)
			{
				double Nposcenter=(parameter[0].Nposmax+parameter[0].Nposmin)/2;
			
				double mapylengthdummy=parameter[0].mapylength;	// irgendwie funktioniert nicht die direkte Verwendung in der nachfolgenden Formel??
				double Nposakt=parameter[0].Nposmax-( (parameter[0].Nposmax-parameter[0].Nposmin)*aktortyworldcoo/(mapylengthdummy-1.0) );
				// 3. Differenzwerte berechnen mit Prec=-5.3699 pro Grad und Temp=-0.3508 pro Grad
				parameter[0].tempdiffort=-0.3508 * (Nposakt-Nposcenter);
				parameter[0].precdiffort=-5.3699 * (Nposakt-Nposcenter);
			}
			else
			{
				parameter[0].tempdiffort=0.0;
				parameter[0].precdiffort=0.0;
			}


			if (parameter[0].lineartransekt==true)
			{
				// 3. Differenzwerte berechnen mit Prec=-5.3699 pro Grad und Temp=-0.3508 pro Grad
				// Umrechnung von Grad in Meter  "1 Grad = 60 x 1852 m = 111120 m"
				parameter[0].tempdiffortmin=-0.3508 * treerows/(111120);
				parameter[0].precdiffortmin=-5.3699 * treerows/(111120);
			}


			// Datei oeffnen
			FILE *f;
			f = fopen(dateinametemp,"r"); 
			if (f == NULL)
			{
				printf("Temperaturdatei nicht vorhanden!\n");
				exit(1);
			}

			char puffer[255];
			int counter=1;
			double jahrbuf, tempjahrmittelbuf;

			// Zeilenweise einlesen und entsprechende Spalteninhalte auslesen und teilweise verarbeiten
			while( fgets(puffer,wortlaengemax,f) !=NULL)
			{
				if (counter>=2)
				{
					jahrbuf= strtod(strtok(puffer, " "),NULL);
					tempjahrmittelbuf= strtod(strtok(NULL, " "),NULL);

					// neues Jahr hinzufuegen
					pweather= new weather();			// 1. Neues Jahr erzeugen
					pweather->yworldcoo=aktortyworldcoo;
					pweather->xworldcoo=aktortxworldcoo;
					pweather->jahr=(int) floor(jahrbuf);					// 2. Werte dem Jahr zuweisen
					pweather->tempjahrmittel=tempjahrmittelbuf+parameter[0].tempdiffort;
					pweather->tempjahrmittelmin=tempjahrmittelbuf+parameter[0].tempdiffort+parameter[0].tempdiffortmin;
					/*
					pweather->temp1monatmittel=temp1monatmittelbuf+parameter[0].tempdiffort;
					pweather->temp1monatmittelmin=temp1monatmittelbuf+parameter[0].tempdiffort+parameter[0].tempdiffortmin;
					pweather->temp2monatmittel=temp2monatmittelbuf+parameter[0].tempdiffort;
					pweather->temp3monatmittel=temp3monatmittelbuf+parameter[0].tempdiffort;
					pweather->temp4monatmittel=temp4monatmittelbuf+parameter[0].tempdiffort;
					pweather->temp5monatmittel=temp5monatmittelbuf+parameter[0].tempdiffort;
					pweather->temp6monatmittel=temp6monatmittelbuf+parameter[0].tempdiffort;
					pweather->temp7monatmittel=temp7monatmittelbuf+parameter[0].tempdiffort;
					pweather->temp7monatmittelmin=temp7monatmittelbuf+parameter[0].tempdiffort+parameter[0].tempdiffortmin;
					pweather->temp8monatmittel=temp8monatmittelbuf+parameter[0].tempdiffort;
					pweather->temp9monatmittel=temp9monatmittelbuf+parameter[0].tempdiffort;
					pweather->temp10monatmittel=temp10monatmittelbuf+parameter[0].tempdiffort;
					pweather->temp11monatmittel=temp11monatmittelbuf+parameter[0].tempdiffort;
					pweather->temp12monatmittel=temp12monatmittelbuf+parameter[0].tempdiffort;
					*/

					// Active Air Temperature AAT = Summe der Temperatur von Tagen die > 10°C liegen
					// Number of Degree Days NDD = Anzahl Tage > 0°C (Vegetationslaenge)
					double sumacttemp=0;
					double sumacttempmin=0;
					double sumdegreday=0;
					int ndegreday=0;
					int ndegredaymin=0;

					// Schätzung der Tagesmitteltemperatur aus der Julimitteltemperatur

					double tempjahrmittelbuf=pweather->tempjahrmittel;
					double tempjahrmittelminbuf=pweather->tempjahrmittelmin;
					double temp1monatmittelbuf=0, temp1monatmittelminbuf=0;
					double temp7monatmittelbuf=0, temp7monatmittelminbuf=0;

					// Sinusfunktionsparameterwerte
					double ausdehnung=0.567980348064239;
					double verschiebung=4.03428152771763;
					double amplianstieg=-0.585543287044634;
					double ampliyschn=15.5976677143297;
					double yabsanstieg=-0.881262021169085;
					double yabsschn=-0.00908805024019213;
					double ampli=1.05*(ampliyschn+amplianstieg*tempjahrmittelbuf);
					double amplimin=1.05*(ampliyschn+amplianstieg*tempjahrmittelminbuf);
					double yabs=yabsschn+yabsanstieg*tempjahrmittelbuf;
					double yabsmin=yabsschn+yabsanstieg*tempjahrmittelminbuf;

					for (int i=1;i<=365;i++)
					{
						double tagestemp=((ampli*sin(( ausdehnung*((double) i/(365.0/12.0)))+verschiebung))-yabs);
						if (tagestemp>0)
						{
							ndegreday++;
							sumdegreday+=tagestemp;
							if (tagestemp>10)
							{
								sumacttemp=sumacttemp+tagestemp;
							}
						}
						double tagestempmin=((tagestemp+yabs)/ampli)*amplimin-yabsmin;
						if (tagestempmin>0)
						{
							ndegredaymin++;
							if (tagestempmin>10)
							{
								sumacttempmin=sumacttempmin+tagestempmin;
							}
						}

						if ( (i>=1) & (i<=31) )
						{ //Januarmittelwert bilden
							temp1monatmittelbuf=temp1monatmittelbuf+tagestemp;
							temp1monatmittelminbuf=temp1monatmittelminbuf+tagestempmin;
						} //Januarmittelwert bilden
					
						if ( (i>=182) & (i<=212) )
						{ //Julimittelwert bilden
							temp7monatmittelbuf=temp7monatmittelbuf+tagestemp;
							temp7monatmittelminbuf=temp7monatmittelminbuf+tagestempmin;
						} //Julimittelwert bilden
					}

					pweather->activeairtemp=sumacttemp;
					pweather->degreday=sumdegreday;
					pweather->vegetationslaenge=ndegreday;
					pweather->activeairtempmin=sumacttempmin;
					pweather->vegetationslaengemin=ndegredaymin;
					pweather->temp1monatmittel=temp1monatmittelbuf/31;
					pweather->temp1monatmittelmin=temp1monatmittelminbuf/31;
					pweather->temp7monatmittel=temp7monatmittelbuf/31;
					pweather->temp7monatmittelmin=temp7monatmittelminbuf/31;
					weather_list.push_back(pweather);// 3. Jahr in Liste einfuegen

				}
				counter++;
			}
			
			fclose(f);

}



/****************************************************************************************//**
 * \brief 
 *
 *
 *******************************************************************************************/
void getPrec2(char dateinameprec[50],vector<weather*>& weather_list)
{

			FILE *fp;
			fp = fopen(dateinameprec,"r");

			if (fp == NULL)
			{
				printf("Niederschlagsdatei nicht vorhanden!\n");
				exit(1);
			}

			int counter=1;
			char puffer[255];
			double niederschlagssummebuf;
			double prec1monatmittelbuf;
			double prec2monatmittelbuf;
			double prec3monatmittelbuf;
			double prec4monatmittelbuf;
			double prec5monatmittelbuf;
			double prec6monatmittelbuf;
			double prec7monatmittelbuf;
			double prec8monatmittelbuf;
			double prec9monatmittelbuf;
			double prec10monatmittelbuf;
			double prec11monatmittelbuf;
			double prec12monatmittelbuf;

			while( fgets(puffer,wortlaengemax,fp) !=NULL)
			{
				if (counter>=2)
				{
					strtod(strtok(puffer, " "),NULL);
					prec1monatmittelbuf= strtod(strtok(NULL, " "),NULL);
					prec2monatmittelbuf= strtod(strtok(NULL, " "),NULL);
					prec3monatmittelbuf= strtod(strtok(NULL, " "),NULL);
					prec4monatmittelbuf= strtod(strtok(NULL, " "),NULL);
					prec5monatmittelbuf= strtod(strtok(NULL, " "),NULL);
					prec6monatmittelbuf= strtod(strtok(NULL, " "),NULL);
					prec7monatmittelbuf= strtod(strtok(NULL, " "),NULL);
					prec8monatmittelbuf= strtod(strtok(NULL, " "),NULL);
					prec9monatmittelbuf= strtod(strtok(NULL, " "),NULL);
					prec10monatmittelbuf= strtod(strtok(NULL, " "),NULL);
					prec11monatmittelbuf= strtod(strtok(NULL, " "),NULL);
					prec12monatmittelbuf= strtod(strtok(NULL, " "),NULL);

					niederschlagssummebuf=(prec1monatmittelbuf+prec2monatmittelbuf+prec3monatmittelbuf+prec4monatmittelbuf+prec5monatmittelbuf+prec6monatmittelbuf+prec7monatmittelbuf+prec8monatmittelbuf+prec9monatmittelbuf+prec10monatmittelbuf+prec11monatmittelbuf+prec12monatmittelbuf);

					// Werte zu aktuellem Jahr hinzufuegen
					pweather=weather_list[counter-2];			// 1. Zum aktuellen Jahr springen
					pweather->prec1monatmittel=prec1monatmittelbuf+parameter[0].precdiffort/12;					// 2. Werte dem Jahr hinzufuegen
					pweather->prec2monatmittel=prec2monatmittelbuf+parameter[0].precdiffort/12;
					pweather->prec3monatmittel=prec3monatmittelbuf+parameter[0].precdiffort/12;
					pweather->prec4monatmittel=prec4monatmittelbuf+parameter[0].precdiffort/12;
					pweather->prec5monatmittel=prec5monatmittelbuf+parameter[0].precdiffort/12;
					pweather->prec6monatmittel=prec6monatmittelbuf+parameter[0].precdiffort/12;
					pweather->prec7monatmittel=prec7monatmittelbuf+parameter[0].precdiffort/12;
					pweather->prec8monatmittel=prec8monatmittelbuf+parameter[0].precdiffort/12;
					pweather->prec9monatmittel=prec9monatmittelbuf+parameter[0].precdiffort/12;
					pweather->prec10monatmittel=prec10monatmittelbuf+parameter[0].precdiffort/12;
					pweather->prec11monatmittel=prec11monatmittelbuf+parameter[0].precdiffort/12;
					pweather->prec12monatmittel=prec12monatmittelbuf+parameter[0].precdiffort/12;
					pweather->niederschlagssumme=niederschlagssummebuf+parameter[0].precdiffort;
					pweather->niederschlagssummemin=niederschlagssummebuf+parameter[0].precdiffort+parameter[0].precdiffortmin;	// Berechnung oben

				}
				counter++;
			}
			
			fclose(fp);

}



/****************************************************************************************//**
 * \brief  
 *
 *
 *******************************************************************************************/
void getTemp3(int aktort, char dateinametemp[50],vector<weather*>& weather_list)
{				
	
	// Berechnung des aktuellen Ortes
	int aktortyworldcoo=(int) floor( (double) (aktort-1)/parameter[0].mapxlength );
	int aktortxworldcoo=(aktort-1) - (aktortyworldcoo * parameter[0].mapxlength);

	// 2. N_inGrad aus den N_max und N_min und Ygesamt-werten berechnen
	if (parameter[0].mapylength>1 && parameter[0].weathercalcgradient==true)
	{
		double Nposcenter=(parameter[0].Nposmax+parameter[0].Nposmin)/2;
		double mapylengthdummy=parameter[0].mapylength;	// irgendwie funktioniert nicht die direkte Verwendung in der nachfolgenden Formel??
		double Nposakt=parameter[0].Nposmax-( (parameter[0].Nposmax-parameter[0].Nposmin)*aktortyworldcoo/(mapylengthdummy-1.0) );
		// 3. Differenzwerte berechnen mit Prec=-5.3699 pro Grad und Temp=-0.3508 pro Grad
		parameter[0].tempdiffort=-0.3508 * (Nposakt-Nposcenter);
		parameter[0].precdiffort=-5.3699 * (Nposakt-Nposcenter);
	}


	if (parameter[0].lineartransekt==true)
	{
		// 3. Differenzwerte berechnen mit Prec=-5.3699 pro Grad und Temp=-0.3508 pro Grad
		// Umrechnung von Grad in Meter  "1 Grad = 60 x 1852 m = 111120 m"
		parameter[0].tempdiffortmin=-0.3508 * treerows/(111120);
		parameter[0].precdiffortmin=-5.3699 * treerows/(111120);
	}



	// Datei oeffnen
	FILE *f;
	f = fopen(dateinametemp,"r"); 
	if (f == NULL)
	{
		printf("Temperaturdatei nicht vorhanden!\n");
		exit(1);
	}
	
	char puffer[255];
	int counter=1;
	double tempjahrmittelbuf, temp1monatmittelbuf, temp2monatmittelbuf, temp3monatmittelbuf, temp4monatmittelbuf, temp5monatmittelbuf, temp6monatmittelbuf, temp7monatmittelbuf, temp8monatmittelbuf, temp9monatmittelbuf, temp10monatmittelbuf, temp11monatmittelbuf, temp12monatmittelbuf;

	// Zeilenweise einlesen und entsprechende Spalteninhalte auslesen und teilweise verarbeiten
	while( fgets(puffer,255,f) !=NULL)
	{

	
		if (counter>=2)	// Erste Spalte wird ausgelassen, da dort Beschriftungen liegen
		{
			temp1monatmittelbuf= strtod(strtok(puffer, " "),NULL);
			temp2monatmittelbuf= strtod(strtok(NULL, " "),NULL);
			temp3monatmittelbuf= strtod(strtok(NULL, " "),NULL) ;
			temp4monatmittelbuf= strtod(strtok(NULL, " "),NULL);
			temp5monatmittelbuf= strtod(strtok(NULL, " "),NULL) ;
			temp6monatmittelbuf= strtod(strtok(NULL, " "),NULL) ;
			temp7monatmittelbuf= strtod(strtok(NULL, " "),NULL);
			temp8monatmittelbuf= strtod(strtok(NULL, " "),NULL) ;
			temp9monatmittelbuf= strtod(strtok(NULL, " "),NULL) ;
			temp10monatmittelbuf= strtod(strtok(NULL, " "),NULL) ;
			temp11monatmittelbuf= strtod(strtok(NULL, " "),NULL) ;
			temp12monatmittelbuf= strtod(strtok(NULL, " "),NULL) ;

			tempjahrmittelbuf=(temp1monatmittelbuf+temp2monatmittelbuf+temp3monatmittelbuf+temp4monatmittelbuf+temp5monatmittelbuf+temp6monatmittelbuf+temp7monatmittelbuf+temp8monatmittelbuf+temp9monatmittelbuf+temp10monatmittelbuf+temp11monatmittelbuf+temp12monatmittelbuf)/12;

			// neues Jahr hinzufuegen
			pweather= new weather();			// 1. Neues Jahr erzeugen
			pweather->yworldcoo=aktortyworldcoo;
			pweather->xworldcoo=aktortxworldcoo;
			pweather->jahr=counter+parameter[0].startjahr-2;					// 2. Werte dem Jahr zuweisen
			pweather->tempjahrmittel=tempjahrmittelbuf+parameter[0].tempdiffort;
			pweather->temp1monatmittel=temp1monatmittelbuf+parameter[0].tempdiffort;
			pweather->temp1monatmittelmin=temp1monatmittelbuf+parameter[0].tempdiffort+parameter[0].tempdiffortmin;
			pweather->temp2monatmittel=temp2monatmittelbuf+parameter[0].tempdiffort;
			pweather->temp3monatmittel=temp3monatmittelbuf+parameter[0].tempdiffort;
			pweather->temp4monatmittel=temp4monatmittelbuf+parameter[0].tempdiffort;
			pweather->temp5monatmittel=temp5monatmittelbuf+parameter[0].tempdiffort;
			pweather->temp6monatmittel=temp6monatmittelbuf+parameter[0].tempdiffort;
			pweather->temp7monatmittel=temp7monatmittelbuf+parameter[0].tempdiffort;
			pweather->temp7monatmittelmin=temp7monatmittelbuf+parameter[0].tempdiffort+parameter[0].tempdiffortmin;
			pweather->temp8monatmittel=temp8monatmittelbuf+parameter[0].tempdiffort;
			pweather->temp9monatmittel=temp9monatmittelbuf+parameter[0].tempdiffort;
			pweather->temp10monatmittel=temp10monatmittelbuf+parameter[0].tempdiffort;
			pweather->temp11monatmittel=temp11monatmittelbuf+parameter[0].tempdiffort;
			pweather->temp12monatmittel=temp12monatmittelbuf+parameter[0].tempdiffort;
			
			// Active Air Temperature AAT = Summe der Temperatur von Tagen die > 10°C liegen
			// Number of days with temperatures above zero degrees = net degree days = NDD = Anzahl Tage > 0°C ("Vegetationslaenge")
			double sumacttemp=0, sumacttempmin=0, sumdegreday=0;
			int ndegreday=0, ndegredaymin=0;

			// Schätzung der Tagesmitteltemperatur aus der Julimitteltemperatur
			double julitemp=pweather->temp7monatmittel;
			double julitempmin=pweather->temp7monatmittelmin;
			double jantemp=pweather->temp1monatmittel;
			//double jantempmin=pweather->temp1monatmittelmin;
			
			for (int i=1;i<=365;i++)
			{
				// für jede der zwei an Jan- bzw Jultemp. aufgehängten Fkt einmal den Tag berechnen (weatherfunktionsabschätzung.docx)
				double tagestempjan=0.0;
				double tagestempjul=0.0;
				double tagestemp=0.0;

				tagestempjan=((22.55131 *sin(0.49102*( (double) i/(365.0/12.0)) +4.62730))+22.04378+jantemp);	
				tagestempjul=((22.55261 *sin(0.49063*( (double) i/(365.0/12.0)) +4.62760))-22.27861+julitemp);	

				//tagestempjan und tagestempjul verrechnen (weatherfunktionsabschätzung.docx)
				double gewicht=0.0;
				if((i>=1) &( i<=15))
				{
					gewicht=((double) 1.0*((15.0-i)/184.0));
				} 
				if((i >=16) & (i<=196))
				{
					gewicht =((double) 1.0*((i-15.0)/181.0));
				}
				if((i>=197) & (i<=365))
				{
					gewicht =((double) 1.0*((365.0+15.0-i)/184.0));
				}

				tagestemp=tagestempjan*(1-gewicht)+tagestempjul*gewicht;
				
				if (tagestemp>0)
				{
					ndegreday++;
					sumdegreday+=tagestemp;
					if (tagestemp>10)
					{
						sumacttemp=sumacttemp+tagestemp;
					}
				}
				double tagestempmin=tagestemp-julitemp+julitempmin;
				if (tagestempmin>0)
				{
					ndegredaymin++;
					if (tagestempmin>10)
					{
						sumacttempmin=sumacttempmin+tagestempmin;
					}
				}
			}
			//hier noch Faktoren für Ort mit drin. Zahlen bisher für weatherchoice==41:44
			
			
			pweather->activeairtemp=(int) round(sumacttemp*0.84630);
			pweather->vegetationslaenge=(int) round(ndegreday*0.88432);
			pweather->activeairtempmin=(int) round(sumacttempmin*0.84630);
			pweather->vegetationslaengemin=(int) round(ndegredaymin*0.88432);
			pweather->degreday=(int) round(sumdegreday*0.87583);
			weather_list.push_back(pweather);// 3. Jahr in Liste einfuegen

		}
		counter++;
						
	}
	
	fclose(f);
}




/****************************************************************************************//**
 * \brief 
 *
 *
 *******************************************************************************************/
void passWeather()
{
	
	for (vector<vector<weather*> >::iterator posw = world_weather_list.begin(); posw != world_weather_list.end(); posw++)
	{// Weltschleife Beginn
		vector<weather*>& weather_list = *posw;

		for (unsigned int iweather=0; iweather<weather_list.size(); ++iweather)	
		{// weather_list Beginn

			// Werte fuer das jeweilige Jahr
			// Einfluss der verschiedenen weatherfaktoren mittels Indices bestimmen

			// Active Air Temperature: >600°C = Wälder (Osawa 2010, S. 53) & 300°C -> Nördliche Grenze (Osawa 2010, S. 47)
			//double aatindex=(1.0/600.0)*weather_list[iweather]->activeairtemp;
			//double aatindexmin=(1.0/600.0)*weather_list[iweather]->activeairtempmin;
			//double julindexs = 0.0227*weather_list[iweather]->temp7monatmittel;
			//double julindexg = 0.00934*weather_list[iweather]->temp7monatmittel;
			// Julitemperaturindex julindex wegen R-Skript Yamal/trw_Bilder Plot "Julitemp_trw_Vergleich_Ch_Y_linear_log_einplot" im Ordner CRUTS gewichtet, zu beachten ist, dass sich auch einige Anpassungswerte verändern, da sich geringfügig andere Korrelationen ergeben mit neuen weatherdaten. weatherchoice != 21-32 --> sib (0.107/(1+exp(10.684-weather_list[iweather]->temp7monatmittelmin)))+0.198 und gmel (0.076/(1+exp(14.568-weather_list[iweather]->temp7monatmittel)))+0.101;
			double julindexs = 0;
			double julindexmins = 0;
			double julindexg = 0;
			double julindexming	= 0;
			julindexs = (0.163/(1+exp(12.319-weather_list[iweather]->temp7monatmittel)))+0.168;
			julindexmins = (0.163/(1+exp(12.319-weather_list[iweather]->temp7monatmittelmin)))+0.168;
			julindexg = (0.078/(1+exp(14.825-weather_list[iweather]->temp7monatmittel)))+0.108;
			julindexming = (0.078/(1+exp(14.825-weather_list[iweather]->temp7monatmittelmin)))+0.108;
			// Growing Season length: 60 Tage -> Nördliche Grenze (Osawa 2010, S. 47)  Beachte: CRU_TS: ca. 120 Tage über 0 im Norden!!! 
			// double gsindex=(weather_list[iweather]->vegetationslaenge/60.0)-1.0;
			// double gsindexmin=(weather_list[iweather]->vegetationslaengemin/60.0)-1.0;
	
			/* da Julindex aufgrund von Fkt Julitemp-realisiertes Max-Wachstum gefittet wurde, ist das reale Wachstum schon eingeflossen.
			Da nur dimensionsloser Faktor gewünscht für spätere Berechnung des Wachstums hier prozentualer Wert ermittelt zwischen minimalem und maximalem Wachstum, indem von dem berechneten Wert (julindexg/s) 
			min. Wachstum abgezogen wird und dann durch die Spanne max. - min. Wachstum geteilt wird. Daten sind aus Yamal-R-Datei mit gewichtetem CRUTS-Datensatz*/
			weather_list[iweather]->weatherfactorg=(double) (julindexg-0.108)/(0.1771-0.108); 
			weather_list[iweather]->weatherfactorming=(double) (julindexming-0.108)/(0.1771-0.108);  
			weather_list[iweather]->weatherfactors=(double) (julindexs-0.168)/(0.305-0.168); 
			weather_list[iweather]->weatherfactormins=(double) (julindexmins-0.168)/(0.305-0.168);  
			
			// Test wegen BasalDwachstum, bei Wachstum, Mort und Etab gucken, dass direkt das maxwaxchstumpro jahr einflieszt daher auch den rest ausgeblendet
			// Langfristige Mittelwerte
			// weatherkennwerte
			// // -30°C Januar-Isotherm -> Grenze für Vorkommen (Osawa 2010, S. 48)
			double jantempsum=0;
			double jantempsummin=0;
			// // 10°C July-Isotherm -> Nördliche Grenze (Osawa 2010, S. 47)
			double jultempsum=0;
			double jultempsummin=0;
			// // Growing Season length = 60 Tage -> Nördliche Grenze (Osawa 2010, S. 47)  CRU_TS: ca. 120 Tage über 0 im Norden!!!  = = jetzt egal
			double nddsum=0;
			double nddsummin=0;
			if (iweather>9)
			{
				for (int i=1; i<(10+1); ++i)
				{
					if((parameter[0].weatherchoice==25) | (parameter[0].weatherchoice==26) | (parameter[0].weatherchoice==27) | (parameter[0].weatherchoice==28) |		
					(parameter[0].weatherchoice==29) | (parameter[0].weatherchoice==30) | (parameter[0].weatherchoice==31) | (parameter[0].weatherchoice==32))
					{
						jantempsum+=weather_list[iweather+i-10]->janmorttemp;
						//jantempsummin+=weather_list[iweather+i-10]->janmorttempmin;
					}
					else
					{
						jantempsum+=weather_list[iweather+i-10]->temp1monatmittel;
						jantempsummin+=weather_list[iweather+i-10]->temp1monatmittelmin;
					}
					jultempsum+=weather_list[iweather+i-10]->temp7monatmittel;
					jultempsummin+=weather_list[iweather+i-10]->temp7monatmittelmin;
					nddsum+=weather_list[iweather+i-10]->vegetationslaenge;
					nddsummin+=weather_list[iweather+i-10]->vegetationslaengemin;
				}
				jantempsum=jantempsum/10;
				jantempsummin=jantempsummin/10;
				jultempsum=jultempsum/10;
				jultempsummin=jultempsummin/10;
				nddsum=nddsum/10;
				nddsummin=nddsummin/10;
			}
			else if (iweather>0 && iweather<=9)
			{
				for (unsigned int i=0; i<(iweather+1); ++i)
				{
					if ((parameter[0].weatherchoice==25) | (parameter[0].weatherchoice==26) | (parameter[0].weatherchoice==27) | (parameter[0].weatherchoice==28) |		
					(parameter[0].weatherchoice==29) | (parameter[0].weatherchoice==30) | (parameter[0].weatherchoice==31) | (parameter[0].weatherchoice==32))
					{
						jantempsum+=weather_list[i]->janmorttemp;
						//jantempsummin+=weather_list[i]->janmorttempmin;
					}
					else
					{
						jantempsum+=weather_list[i]->temp1monatmittel;
						jantempsummin+=weather_list[i]->temp1monatmittelmin;
					}				
					jultempsum+=weather_list[i]->temp7monatmittel;
					jultempsummin+=weather_list[i]->temp7monatmittelmin;
					nddsum+=weather_list[i]->vegetationslaenge;
					nddsummin+=weather_list[i]->vegetationslaengemin;
				}
				jantempsum=jantempsum/(iweather+1);
				jantempsummin=jantempsummin/(iweather+1);
				jultempsum=jultempsum/(iweather+1);
				jultempsummin=jultempsummin/(iweather+1);
				nddsum=nddsum/(iweather+1);
				nddsummin=nddsummin/(iweather+1);
			}
			else
			{
				if ((parameter[0].weatherchoice==25) | (parameter[0].weatherchoice==26) | (parameter[0].weatherchoice==27) | (parameter[0].weatherchoice==28) |		
					(parameter[0].weatherchoice==29) | (parameter[0].weatherchoice==30) | (parameter[0].weatherchoice==31) | (parameter[0].weatherchoice==32))
				{
					jantempsum=weather_list[iweather]->janmorttemp;
					//jantempsummin=weather_list[iweather]->janmorttempmin;
				}
				else
				{
					jantempsum=weather_list[iweather]->temp1monatmittel;
					jantempsummin=weather_list[iweather]->temp1monatmittelmin;
				}					
				jultempsum=weather_list[iweather]->temp7monatmittel;
				jultempsummin=weather_list[iweather]->temp7monatmittelmin;
				nddsum=weather_list[iweather]->vegetationslaenge;
				nddsummin=weather_list[iweather]->vegetationslaengemin;
			}
			weather_list[iweather]->temp1monatmitteliso=jantempsum;
			weather_list[iweather]->temp1monatmittelisomin=jantempsummin;
			weather_list[iweather]->temp7monatmitteliso=jultempsum;
			weather_list[iweather]->temp7monatmittelisomin=jultempsummin;
			weather_list[iweather]->vegetationslaengeiso=(int) floor(nddsum);
			weather_list[iweather]->vegetationslaengeisomin=(int) floor(nddsummin);

			// Berechnung eines Trockenheitsindex
			// // "Evaporation rates of 1.5 mm/day mean growing season" (Dolmann, 2004)
			double trockenheitsmortbuf=0.0;
			double precgs=	weather_list[iweather]->prec5monatmittel
								+ weather_list[iweather]->prec6monatmittel
								+ weather_list[iweather]->prec7monatmittel
								+ weather_list[iweather]->prec8monatmittel;
			double duerreindex=parameter[0].evapod*weather_list[iweather]->vegetationslaenge/precgs;
			if (duerreindex>1) 
			{
				double aattrockenheit=(weather_list[iweather]->activeairtemp/300.0)-1.0;
				if (aattrockenheit>1)
				{
					trockenheitsmortbuf=trockenheitsmortbuf+aattrockenheit*0.1;
				}
			}
			weather_list[iweather]->trockenheitsmort=trockenheitsmortbuf;

			if (parameter[0].qualijahranzeige==true)
			{// Anzeige Beginn
				printf("	weather(%d; iweather=%d): weatherfactorg=%4.4f, weatherfactors=%4.4f ===> \nFeuermortalitaet=%4.5f\n", weather_list[iweather]->jahr, iweather, weather_list[iweather]->weatherfactorg, weather_list[iweather]->weatherfactors, weather_list[iweather]->trockenheitsmort);
				printf("\tJanT10=%4.2f, JuliT10=%4.2f, NDD10=%d\n",weather_list[iweather]->temp1monatmitteliso, weather_list[iweather]->temp7monatmitteliso, weather_list[iweather]->vegetationslaengeiso);
			}// Anzeige Ende

			// Abiotische Einfluesse
			// weatherabhaengige Mortalitaet aus 10 Jahresmittelwerten ermitteln
			// -30°C Januar-Isotherm -> Grenze für Vorkommen (Osawa 2010, S. 48)
			weather_list[iweather]->janisothermrestriktiong=0.0;
			weather_list[iweather]->janisothermrestriktions=0.0;
			
			//  10°C July-Isotherm -> Nördliche Grenze (Osawa 2010, S. 47)
			weather_list[iweather]->julisothermrestriktion=0.0;
			
			// Growing Season length = 60 Tage -> Nördliche Grenze (Osawa 2010, S. 47)
			weather_list[iweather]->nddrestriktion=0.0;

			/// mean January temp < -40°C?
			// temp1monatmitteliso = aus temp01monatmittel berechnet falls weather ungleich 25-32, bei diesen weatherchoiceen weather mit eingelesenem Janmorttemp berechnet
			if (weather_list[iweather]->temp1monatmitteliso<(parameter[0].jantodestempgmel))
			{
				weather_list[iweather]->janisothermrestriktiong=1.0;
			}
			else
			{
				weather_list[iweather]->janisothermrestriktiong=1.0-fabs(9.0*(weather_list[iweather]->temp1monatmitteliso-parameter[0].jantodestempgmel)/(-parameter[0].jantodestempgmel));	// hier Betrag ueberfluessig da Zahl niemals negativ da nur Zahlen <= -45.0 °C
			}


			/// mean January temp < -40°C?
			// temp1monatmitteliso = aus temp01monatmittel berechnet falls weather ungleich 25-32, bei diesen weatherchoiceen weather mit eingelesenem Janmorttemp berechnet
			if (weather_list[iweather]->temp1monatmitteliso<(-33.0))
			{
				weather_list[iweather]->janisothermrestriktions=1.0;
			}
			else
			{
				weather_list[iweather]->janisothermrestriktions=1.0-fabs(6.6*(weather_list[iweather]->temp1monatmitteliso+33.0)/33.0);	// hier Betrag ueberfluessig da Zahl niemals negativ da nur Zahlen <= -33.0 °C
			}		
			
			//printf("%d,TJanMort=%4.2f,TJanMortIso=%4.2f,Janresg=%4.2f\n", weather_list[iweather]->jahr, weather_list[iweather]->janmorttemp, weather_list[iweather]->temp1monatmitteliso,weather_list[iweather]->janisothermrestriktiong);
			
			/// mean July temp < 10°C?
			if (weather_list[iweather]->temp7monatmitteliso<10.0) 
			{
				weather_list[iweather]->julisothermrestriktion=1.0;
			}
			else 
			{
				weather_list[iweather]->julisothermrestriktion=1.0-fabs(2.0*(weather_list[iweather]->temp7monatmitteliso-10.0)/10.0);	// hier Betrag ueberfluessig da Zahl niemals negativ da nur Zahlen >= 10.0 °C
			}


			/// Vegetationslaenge <60 days
			if (weather_list[iweather]->vegetationslaengeiso<60.0)
			{
				weather_list[iweather]->nddrestriktion=1.0; 
			}
			else 
			{
				weather_list[iweather]->nddrestriktion=1.0-fabs(( ((double) weather_list[iweather]->vegetationslaengeiso)-60.0)/60.0);	//hier Betrag ueberfluessig da Zahl niemals negativ da nur Zahlen >= 60 d
			}
				
		
			/*		
			signed int wettabbruch;
			// Abfrage ob das Programm beendet oder fortgesetzt werden soll
			printf("\n Ende des Passweather. Weiter mit 1, beenden mit irgendeiner Eingabe\n");
			scanf("%d", &wettabbruch);

			if (wettabbruch!=1) 
			{
				printf("LaVeSi wurde beendet\n\n"); 
				exit(0);
			}					
			*/	
			
		}// weather_list Ende
		
		// Ausgabe einer Uebersichtsanzeige nach Einlesen der weatherdaten
		if (parameter[0].parameterlesenanzeige ==true) 
		{
			for (unsigned int iweather=0; iweather<weather_list.size(); ++iweather)	
			{
				// printf("%d,Tmean=%4.2f,Precsum=%4.2f,AAT=%4.1f,GDD=%4.1f,NDD=%d,TJan=%4.2f\n", weather_list[iweather]->jahr, weather_list[iweather]->tempjahrmittel, weather_list[iweather]->niederschlagssumme, weather_list[iweather]->activeairtemp,weather_list[iweather]->degreday,weather_list[iweather]->vegetationslaenge,weather_list[iweather]->janmorttemp);
				// printf("%d,TJanMort=%4.2f,TJanMortIso=%4.2f\n", weather_list[iweather]->jahr, weather_list[iweather]->janmorttemp, weather_list[iweather]->temp1monatmitteliso);
			}
		}		
		
	}//Weltschleife Ende
}










/****************************************************************************************//**
 * \brief 
 *
 *
 *******************************************************************************************/
extern void weathereinlesen(int treerows, struct Parameter *parameter,  int wortlaengemax, vector<vector<weather*> > &world_weather_list)
{

	// -- -- -- -- weathereinlesen BEGINN -- -- -- -- //
	char dateinametemp[50];
	char dateinameprec[50];
	
	
	
	int findyr1=0,findyr2=-100,jahr=0,cntr=0;
	vector<double> wdir,wspd;
	string filename;
	std::ostringstream ss;
	string item;
	
	for (int t=0;t<parameter[0].simdauer;t++)
		{ // Jahresschrittschleife Beginn
			
			cntr=0;
			
			jahr=parameter[0].startjahr+t;
			
			if(parameter[0].windsource==1){
			findyr1=1947;findyr2=2012;
			}else if(parameter[0].windsource==2){
			findyr1=1979;findyr2=2012;
			}else if(parameter[0].windsource==3){
			findyr1=1959;findyr2=2002;
			}//else if(parameter[0].windsource==4){//north
			//
			//}else if(parameter[0].windsource==5){//south
			//	
			//}
			
			
		ss.str("");
		ss.clear();  	
		
		if((jahr<findyr2+1) && (jahr>findyr1-1)){
							
		ss<<jahr;
							
		if(parameter[0].windsource==1){
		filename="input/winddaten/winddaten"+ss.str()+"_GSOD.dat";
		}else if(parameter[0].windsource==2){
		filename="input/winddaten/winddaten"+ss.str()+"_EraInterim.dat";
		}else if(parameter[0].windsource==3){
		filename="input/winddaten/winddaten"+ss.str()+"_Era40.dat";
		}
							
						//cout<<jahr<<endl;
// 						fstream fileinp;
  						//cout<<"opening "<<filename<<endl;
 						op:
						ifstream fileinp(filename.c_str());
  						if(!fileinp){goto op;}

						while(fileinp>>item){
// 						  itemvector.push_back(item);
						  cntr++;
						  if(cntr%2){
						    // if(stof(item)<270){
						      // wdir.push_back(stof(item)+90);
						    // }else if(stof(item)>270){
						      // wdir.push_back(stof(item)-270);
						    // }else{wdir.push_back(0);}
						    if(stof(item)>=0 && stof(item)<=360)
							{
								wdir.push_back(stof(item));
						    }else
							{
								wdir.push_back(0);
							}
						  }else{wspd.push_back(stof(item));}
						}
						
						if(cntr>0){
						windspd.push_back(wspd);
						winddir.push_back(wdir);
						globalyears.push_back(jahr);
						
						//cout<<"pushing_back "<<jahr<<endl;
						//cout<<"Größe wspd="<<wspd.size()<<endl;
				}
				wspd.clear();wdir.clear();
			}
	
		}
		jahr=0;
	
	

	int aktort=0;
	for (vector<vector<weather*> >::iterator posw = world_weather_list.begin(); posw != world_weather_list.end(); posw++)
	{	// Weltschleife Beginn
		// Um auf die Inhalte in den weather_listn zuzugreifen muss eine weather_listn als Refernenz
		// erstellt werden um die Struktur zu kennen und dann kann wie schon im Code
		// realisiert ist weiterverfahren werden
		// Loesung brachte http://www.easy-coding.de/auf-listen-von-listen-zugreifen-t2529.html
		vector<weather*>& weather_list = *posw;
		aktort++;

		// Je nach choice werden unterschiedliche Dateien geoeffnet und dann Zeilenweise eingelesen
		if ((parameter[0].weatherchoice==10) | (parameter[0].weatherchoice==11) | (parameter[0].weatherchoice==12) | (parameter[0].weatherchoice==13) |
		(parameter[0].weatherchoice==111) | (parameter[0].weatherchoice==120) | (parameter[0].weatherchoice==121) | (parameter[0].weatherchoice==122) |
		(parameter[0].weatherchoice==123) | (parameter[0].weatherchoice==222) | (parameter[0].weatherchoice==309) | (parameter[0].weatherchoice==999) |
		(parameter[0].weatherchoice==1111) | (parameter[0].weatherchoice==2221))
		{
			if (parameter[0].weatherchoice==111)
			{
				if (aktort==1)
				{
					char tempbuf[]="input/CH06CRUtemp_long.csv";
					char precbuf[]="input/CH06CRUprec_long.csv";
					strcpy(dateinametemp, tempbuf);
					strcpy(dateinameprec, precbuf);
				}
				else if (aktort==2)
				{
					char tempbuf[]="input/CH02CRUtemp_long.csv";
					char precbuf[]="input/CH02CRUprec_long.csv";
					strcpy(dateinametemp, tempbuf);
					strcpy(dateinameprec, precbuf);
				}
				else if (aktort==3)
				{
					char tempbuf[]="input/CH1712CRUtemp_long.csv";
					char precbuf[]="input/CH1712CRUprec_long.csv";
					strcpy(dateinametemp, tempbuf);
					strcpy(dateinameprec, precbuf);
				}
				else if (aktort==4)
				{
					char tempbuf[]="input/CH12NCRUtemp_long.csv";
					char precbuf[]="input/CH12NCRUprec_long.csv";
					strcpy(dateinametemp, tempbuf);
					strcpy(dateinameprec, precbuf);
				}
			}
			else if (parameter[0].weatherchoice==120)
			{
				char tempbuf[]="input/CH06CRUtemp_long.csv";
				char precbuf[]="input/CH06CRUprec_long.csv";
				strcpy(dateinametemp, tempbuf);
				strcpy(dateinameprec, precbuf);
			}
			else if (parameter[0].weatherchoice==121)
			{
				char tempbuf[]="input/CH02CRUtemp_long.csv";
				char precbuf[]="input/CH02CRUprec_long.csv";
				strcpy(dateinametemp, tempbuf);
				strcpy(dateinameprec, precbuf);
			}
			else if (parameter[0].weatherchoice==122)
			{
				char tempbuf[]="input/CH1712CRUtemp_long.csv";
				char precbuf[]="input/CH1712CRUprec_long.csv";
				strcpy(dateinametemp, tempbuf);
				strcpy(dateinameprec, precbuf);
			}
			else if (parameter[0].weatherchoice==123)
			{
				char tempbuf[]="input/CH12NCRUtemp_long.csv";
				char precbuf[]="input/CH12NCRUprec_long.csv";
				strcpy(dateinametemp, tempbuf);
				strcpy(dateinameprec, precbuf);
			}
			else if (parameter[0].weatherchoice==222 || parameter[0].weatherchoice==999)
			{
				if (aktort==1)
				{
					char tempbuf[]="input/CH06CRUtemp.csv";
					char precbuf[]="input/CH06CRUprec.csv";
					strcpy(dateinametemp, tempbuf);
					strcpy(dateinameprec, precbuf);
				}
				else if (aktort==2)
				{
					char tempbuf[]="input/CH02CRUtemp.csv";
					char precbuf[]="input/CH02CRUprec.csv";
					strcpy(dateinametemp, tempbuf);
					strcpy(dateinameprec, precbuf);
				}
				else if (aktort==3)
				{
					char tempbuf[]="input/CH1712CRUtemp.csv";
					char precbuf[]="input/CH1712CRUprec.csv";
					strcpy(dateinametemp, tempbuf);
					strcpy(dateinameprec, precbuf);
				}
				else if (aktort==4)
				{
					char tempbuf[]="input/CH12NCRUtemp.csv";
					char precbuf[]="input/CH12NCRUprec.csv";
					strcpy(dateinametemp, tempbuf);
					strcpy(dateinameprec, precbuf);
				}
			}
			else if (parameter[0].weatherchoice==1111)
			{
				// Fast R supported climate matrix design
				#include "weathereingabe_matrix.cpp"
				
			}
			else if (parameter[0].weatherchoice==2221)
			{
				// Fast R supported climate matrix design
				#include "weathereingabe_tempexp.cpp"
				
			}
			else if (parameter[0].weatherchoice==10)
			{
				char tempbuf[]="input/CH06CRUtemp.csv";
				char precbuf[]="input/CH06CRUprec.csv";
				strcpy(dateinametemp, tempbuf);
				strcpy(dateinameprec, precbuf);
			}
			else if (parameter[0].weatherchoice==11)
			{
				char tempbuf[]="input/CH02CRUtemp.csv";
				char precbuf[]="input/CH02CRUprec.csv";
				strcpy(dateinametemp, tempbuf);
				strcpy(dateinameprec, precbuf);
			}
			else if (parameter[0].weatherchoice==12)
			{
				char tempbuf[]="input/CH1712CRUtemp.csv";
				char precbuf[]="input/CH1712CRUprec.csv";
				strcpy(dateinametemp, tempbuf);
				strcpy(dateinameprec, precbuf);
			}
			else if (parameter[0].weatherchoice==13)
			{
				char tempbuf[]="input/CH12NCRUtemp.csv";
				char precbuf[]="input/CH12NCRUprec.csv";
				strcpy(dateinametemp, tempbuf);
				strcpy(dateinameprec, precbuf);
			}
			else if (parameter[0].weatherchoice==309)
			{
				char tempbuf[]="input/CHATANGASTATtemp.csv";
				char precbuf[]="input/CHATANGASTATprec.csv";
				strcpy(dateinametemp, tempbuf);
				strcpy(dateinameprec, precbuf);
			}
		
			getTemp1(aktort, dateinametemp,  weather_list);
		
			getPrec1(dateinameprec, weather_list);
		}
		else if((parameter[0].weatherchoice==1) | (parameter[0].weatherchoice==9))
		{
			if (parameter[0].weatherchoice==1)
			{
				char tempbuf[]="input/RS000020891_MONTHLY_Temp_const.csv";
				char precbuf[]="input/RS000020891_MONTHLY_Prec_const.csv";
				strcpy(dateinametemp, tempbuf);
				strcpy(dateinameprec, precbuf);
			}
			else if (parameter[0].weatherchoice==9)
			{
				char tempbuf[]="input/weather_synopse_temp.csv";
				char precbuf[]="input/weather_synopse_prec.csv";
				strcpy(dateinametemp, tempbuf);
				strcpy(dateinameprec, precbuf);
			}
			
			
			getTemp2(aktort, dateinametemp,  weather_list);

			getPrec2(dateinameprec, weather_list);
		}
		else
		{
			if (parameter[0].weatherchoice==21)
			{
				char tempbuf[]="input/TY02_tmpweighted_model.csv";
				char precbuf[]="input/TY02_prcweighted_model.csv";
				strcpy(dateinametemp, tempbuf);
				strcpy(dateinameprec, precbuf);
			}
			else if (parameter[0].weatherchoice==22)
			{
				char tempbuf[]="input/CH17_tmpweighted_model.csv";
				char precbuf[]="input/CH17_prcweighted_model.csv";
				strcpy(dateinametemp, tempbuf);
				strcpy(dateinameprec, precbuf);
			}
			else if (parameter[0].weatherchoice==23)
			{
				char tempbuf[]="input/CH02_tmpweighted_model.csv";
				char precbuf[]="input/CH02_prcweighted_model.csv";
				strcpy(dateinametemp, tempbuf);
				strcpy(dateinameprec, precbuf);
			}
			else if (parameter[0].weatherchoice==24)
			{
				char tempbuf[]="input/CH06_tmpweighted_model.csv";
				char precbuf[]="input/CH06_prcweighted_model.csv";
				strcpy(dateinametemp, tempbuf);
				strcpy(dateinameprec, precbuf);
			}
			else if ((parameter[0].weatherchoice==25) | (parameter[0].weatherchoice== 26) | (parameter[0].weatherchoice== 27) | (parameter[0].weatherchoice== 28) | (parameter[0].weatherchoice== 29) | (parameter[0].weatherchoice== 30) | (parameter[0].weatherchoice== 31) | (parameter[0].weatherchoice== 32))
			{
				char tempbuf[]="input/CH17_tmpweighted_model.csv";
				char precbuf[]="input/CH17_prcweighted_model.csv";
				strcpy(dateinametemp, tempbuf);
				strcpy(dateinameprec, precbuf);
			}
			else if (parameter[0].weatherchoice==41)
			{
				char tempbuf[]="input/Khatanga_exp_tmp.csv";
				char precbuf[]="input/Khatanga_exp_prc.csv";
				strcpy(dateinametemp, tempbuf);
				strcpy(dateinameprec, precbuf);
			}	
			else if (parameter[0].weatherchoice==42)
			{
				char tempbuf[]="input/Norilsk_exp_tmp.csv";
				char precbuf[]="input/Norilsk_exp_prc.csv";
				strcpy(dateinametemp, tempbuf);
				strcpy(dateinameprec, precbuf);
			}	
			else if (parameter[0].weatherchoice==43)
			{
				char tempbuf[]="input/Vorkuta_exp_tmp.csv";
				char precbuf[]="input/Vorkuta_exp_prc.csv";
				strcpy(dateinametemp, tempbuf);
				strcpy(dateinameprec, precbuf);
			}	
			else if (parameter[0].weatherchoice==44)
			{
				char tempbuf[]="input/Zhigansk_exp_tmp.csv";
				char precbuf[]="input/Zhigansk_exp_prc.csv";
				strcpy(dateinametemp, tempbuf);
				strcpy(dateinameprec, precbuf);
			}
			else if (parameter[0].weatherchoice==51)
			{
				char tempbuf[]="input/Coredata_exp_tmp.csv";
				char precbuf[]="input/Coredata_exp_prc.csv";
				strcpy(dateinametemp, tempbuf);
				strcpy(dateinameprec, precbuf);
			}
			else if (parameter[0].weatherchoice==52)
			{
				char tempbuf[]="input/Coredata_complete_LauraS_tmp.csv";
				char precbuf[]="input/Coredata_complete_LauraS_prc.csv";
				strcpy(dateinametemp, tempbuf);
				strcpy(dateinameprec, precbuf);
			}
			else if (parameter[0].weatherchoice==53)
			{
				char tempbuf[]="input/Coredata_complete_LauraN_tmp.csv";
				char precbuf[]="input/Coredata_complete_LauraN_prc.csv";
				strcpy(dateinametemp, tempbuf);
				strcpy(dateinameprec, precbuf);
			}
			else if (parameter[0].weatherchoice==54)
			{
				char tempbuf[]="input/Coredata_complete_Khan_tmp.csv";
				char precbuf[]="input/Coredata_complete_Khan_prc.csv";
				strcpy(dateinametemp, tempbuf);
				strcpy(dateinameprec, precbuf);
			}			
			getTemp3(aktort, dateinametemp,  weather_list);
			getPrec1(dateinameprec, weather_list);
		}
		
		parameter[0].parameterlesenanzeige=true;
		// Ausgabe einer Uebersichtsanzeige nach Einlesen der weatherdaten
		if (parameter[0].parameterlesenanzeige ==true) 
		{
			for (unsigned int iweather=0; iweather<weather_list.size(); ++iweather)	
			{
				// printf("%d,Tmean=%4.2f,Precsum=%4.2f,AAT=%4.1f,GDD=%4.1f,NDD=%d,TJan=%4.2f\n", weather_list[iweather]->jahr, weather_list[iweather]->tempjahrmittel, weather_list[iweather]->niederschlagssumme, weather_list[iweather]->activeairtemp,weather_list[iweather]->degreday,weather_list[iweather]->vegetationslaenge,weather_list[iweather]->janmorttemp);
				//printf("%d,thdd=%d\n", weather_list[iweather]->jahr, weather_list[iweather]->thawdegreday);
			}
		}
		// exit(0);
	}//Weltschleife Ende


	// -- -- -- -- weathereinlesen ENDE -- -- -- -- //

	passWeather();

}






























