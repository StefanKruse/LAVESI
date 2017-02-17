/****************************************************************************************//**
 * \brief Read in monthly temperature data and store in WetterListe
 * 
 * 
 * 
 *
 *******************************************************************************************/
void getTemp1(int aktort, char dateinametemp[50],vector<Wetter*>& WetterListe)
{				
	
	// determine the current location
	int aktortyweltcoo=(int) floor( (double) (aktort-1)/parameter[0].mapxlength );
	int aktortxweltcoo=(aktort-1) - (aktortyweltcoo * parameter[0].mapxlength);

	// if a weather gradient is assumed calculate the difference in temperature and precipitation based on linear changing values
	if (parameter[0].mapylength>1 && parameter[0].weathercalcgradient==true)
	{
		double Nposcenter=(parameter[0].Nposmax+parameter[0].Nposmin)/2;
		double mapylengthdummy=parameter[0].mapylength;
		double Nposakt=parameter[0].Nposmax-( (parameter[0].Nposmax-parameter[0].Nposmin)*aktortyweltcoo/(mapylengthdummy-1.0) );
		parameter[0].tempdiffort=-0.3508 * (Nposakt-Nposcenter);
		parameter[0].precdiffort=-5.3699 * (Nposakt-Nposcenter);
	}

	// if a climate gradient is assumen on the simulated area (south-to-north) calculate the difference in metres
	if (parameter[0].lineartransekt==true)
	{
		// reformat degrees latitude to metres => 1 degree = 60 x 1852 m = 111120 m
		parameter[0].tempdiffortmin=-0.3508 * baeumerows/(111120);
		parameter[0].precdiffortmin=-5.3699 * baeumerows/(111120);
	}

	// open the file
	FILE *f;
	f = fopen(dateinametemp,"r"); 
	if (f == NULL)
	{
		printf("Requested temperature data not available!\n");
		exit(1);
	}
	
	char puffer[255];
	int counter=1;
	double tempjahrmittelbuf, temp1monatmittelbuf, temp2monatmittelbuf, temp3monatmittelbuf, temp4monatmittelbuf, temp5monatmittelbuf, temp6monatmittelbuf, temp7monatmittelbuf, temp8monatmittelbuf, temp9monatmittelbuf, temp10monatmittelbuf, temp11monatmittelbuf, temp12monatmittelbuf;

	// read in row by row
	while( fgets(puffer,255,f) !=NULL)
	{

		if (counter>=2)	// skip first line with headings
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

			pWetter= new Wetter();	
			pWetter->yweltcoo=aktortyweltcoo;
			pWetter->xweltcoo=aktortxweltcoo;
			pWetter->jahr=counter+parameter[0].startjahr-2;
			pWetter->tempjahrmittel=tempjahrmittelbuf+parameter[0].tempdiffort;
			pWetter->temp1monatmittel=temp1monatmittelbuf+parameter[0].tempdiffort;
			pWetter->temp1monatmittelmin=temp1monatmittelbuf+parameter[0].tempdiffort+parameter[0].tempdiffortmin;
			pWetter->temp2monatmittel=temp2monatmittelbuf+parameter[0].tempdiffort;
			pWetter->temp3monatmittel=temp3monatmittelbuf+parameter[0].tempdiffort;
			pWetter->temp4monatmittel=temp4monatmittelbuf+parameter[0].tempdiffort;
			pWetter->temp5monatmittel=temp5monatmittelbuf+parameter[0].tempdiffort;
			pWetter->temp6monatmittel=temp6monatmittelbuf+parameter[0].tempdiffort;
			pWetter->temp7monatmittel=temp7monatmittelbuf+parameter[0].tempdiffort;
			pWetter->temp7monatmittelmin=temp7monatmittelbuf+parameter[0].tempdiffort+parameter[0].tempdiffortmin;
			pWetter->temp8monatmittel=temp8monatmittelbuf+parameter[0].tempdiffort;
			pWetter->temp9monatmittel=temp9monatmittelbuf+parameter[0].tempdiffort;
			pWetter->temp10monatmittel=temp10monatmittelbuf+parameter[0].tempdiffort;
			pWetter->temp11monatmittel=temp11monatmittelbuf+parameter[0].tempdiffort;
			pWetter->temp12monatmittel=temp12monatmittelbuf+parameter[0].tempdiffort;

			pWetter->maxbasalwachstumjahr=0;
			pWetter->maxbrustwachstumjahr=0;
			pWetter->maxbasalwachstumjahrmin=0;
			pWetter->maxbrustwachstumjahrmin=0;
			
			// process data
			double sumacttemp=0, sumacttempmin=0;
			int ndegreday=0, ndegredaymin=0;

			// estimate daily temperatures based on the temperatures of July
			double julitemp=pWetter->temp7monatmittel;
			double julitempmin=pWetter->temp7monatmittelmin;
			
			for (int i=1;i<=365;i++)
			{
				
				double tagestemp=0;

				if (parameter[0].wetterwahl<10)
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
			pWetter->activeairtemp=sumacttemp;
			pWetter->vegetationslaenge=ndegreday;
			pWetter->activeairtempmin=sumacttempmin;
			pWetter->vegetationslaengemin=ndegredaymin;
			WetterListe.push_back(pWetter);

		}
		counter++;
						
	}
	fclose(f);
}

/****************************************************************************************//**
 * \brief Read in monthly precipitation data and store in WetterListe
 * 
 *
 *
 *
 *******************************************************************************************/
void getPrec1(char dateinameprec[50],vector<Wetter*>& WetterListe)
{
			FILE *fp;
			fp = fopen(dateinameprec,"r");

			if (fp == NULL)
			{
				printf("Precipitation data not available!\n");
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

					pWetter=WetterListe[counter-2];	
					pWetter->prec1monatmittel=prec1monatmittelbuf+parameter[0].precdiffort/12;
					pWetter->prec2monatmittel=prec2monatmittelbuf+parameter[0].precdiffort/12;
					pWetter->prec3monatmittel=prec3monatmittelbuf+parameter[0].precdiffort/12;
					pWetter->prec4monatmittel=prec4monatmittelbuf+parameter[0].precdiffort/12;
					pWetter->prec5monatmittel=prec5monatmittelbuf+parameter[0].precdiffort/12;
					pWetter->prec6monatmittel=prec6monatmittelbuf+parameter[0].precdiffort/12;
					pWetter->prec7monatmittel=prec7monatmittelbuf+parameter[0].precdiffort/12;
					pWetter->prec8monatmittel=prec8monatmittelbuf+parameter[0].precdiffort/12;
					pWetter->prec9monatmittel=prec9monatmittelbuf+parameter[0].precdiffort/12;
					pWetter->prec10monatmittel=prec10monatmittelbuf+parameter[0].precdiffort/12;
					pWetter->prec11monatmittel=prec11monatmittelbuf+parameter[0].precdiffort/12;
					pWetter->prec12monatmittel=prec12monatmittelbuf+parameter[0].precdiffort/12;
					pWetter->niederschlagssumme=niederschlagssummebuf+parameter[0].precdiffort;
					pWetter->niederschlagssummemin=niederschlagssummebuf+parameter[0].precdiffort+parameter[0].precdiffortmin;

				}
				counter++;
			}
			fclose(fp);
}

/****************************************************************************************//**
 * \brief Process waeather data and calculate auxiliary variables influencing the larch trees
 * 
 *
 *
 *
 *******************************************************************************************/
void passWeather()
{

	for (vector<vector<Wetter*> >::iterator posw = WeltWetterListe.begin(); posw != WeltWetterListe.end(); posw++)
	{
		vector<Wetter*>& WetterListe = *posw;

		for (unsigned int iWetter=0; iWetter<WetterListe.size(); ++iWetter)	
		{

			// influence of climate factors by three indices

				// active air temperature
					double aatindex=(1.0/600.0)*WetterListe[iWetter]->activeairtemp;
					double aatindexmin=(1.0/600.0)*WetterListe[iWetter]->activeairtempmin;
				// growing season length
					double gsindex=(WetterListe[iWetter]->vegetationslaenge/60.0)-1.0;
					double gsindexmin=(WetterListe[iWetter]->vegetationslaengemin/60.0)-1.0;
				// precipitation sum
					double precindex=0.0;
					double precindexmin=0.0;

				if (parameter[0].precwetter==0) 
				{
					precindex=1.0;
					precindexmin=1.0;
				}
				else if (parameter[0].precwetter==1) 
				{
					precindex=pow(1.0-parameter[0].precindexstrength*( ( ((double) WetterListe[iWetter]->niederschlagssumme)-parameter[0].precgrenzwert)/parameter[0].precgrenzwert), parameter[0].precindexexponent);
					precindexmin=pow(1.0-parameter[0].precindexstrength*( ( ((double) WetterListe[iWetter]->niederschlagssummemin)-parameter[0].precgrenzwert)/parameter[0].precgrenzwert), parameter[0].precindexexponent);
				}
				else if (parameter[0].precwetter==2) 
				{
					precindex=-0.0000015*pow( ((double) (WetterListe[iWetter]->niederschlagssumme-250)),2)+1.0;
					precindexmin=-0.0000015*pow( ((double) (WetterListe[iWetter]->niederschlagssummemin-250)),2)+1.0;
				}

				// estimate the current tree diamter growth rates for the current year based on the climate indices 
				double aateinfluss=(1.0/parameter[0].aatnddratio);
				double gseinfluss=1.0-aateinfluss;
				double maxdbasalwachstum=parameter[0].gdbasal; 
				double maxdbasalsaplwachstum=parameter[0].gdbasalsapl;
				double maxdbrustwachstum=parameter[0].gdbrust;
				 
				WetterListe[iWetter]->maxbasalwachstumjahr=precindex*(aateinfluss*aatindex*maxdbasalwachstum+gseinfluss*gsindex*maxdbasalwachstum); 
				WetterListe[iWetter]->maxbasalwachstumjahrmin=precindexmin*(aateinfluss*aatindexmin*maxdbasalwachstum+gseinfluss*gsindexmin*maxdbasalwachstum);  
				WetterListe[iWetter]->maxbasalsaplwachstumjahr=precindex*(aateinfluss*aatindex*maxdbasalsaplwachstum+gseinfluss*gsindex*maxdbasalsaplwachstum); 
				WetterListe[iWetter]->maxbasalsaplwachstumjahrmin=precindexmin*(aateinfluss*aatindexmin*maxdbasalsaplwachstum+gseinfluss*gsindexmin*maxdbasalsaplwachstum);
				WetterListe[iWetter]->maxbrustwachstumjahr=precindex*(aateinfluss*aatindex*maxdbrustwachstum+gseinfluss*gsindex*maxdbrustwachstum);
				WetterListe[iWetter]->maxbrustwachstumjahrmin=precindexmin*(aateinfluss*aatindexmin*maxdbrustwachstum+gseinfluss*gsindexmin*maxdbrustwachstum);
				
				// calculate long time mean values
				double jantempsum=0;
				double jantempsummin=0;
				double jultempsum=0;
				double jultempsummin=0;
				double nddsum=0;
				double nddsummin=0;
				if (iWetter>10)
				{
					for (int i=1; i<(10+1); ++i)
					{
						jantempsum+=WetterListe[iWetter+i-10]->temp1monatmittel;
						jantempsummin+=WetterListe[iWetter+i-10]->temp1monatmittelmin;
						jultempsum+=WetterListe[iWetter+i-10]->temp7monatmittel;
						jultempsummin+=WetterListe[iWetter+i-10]->temp7monatmittelmin;
						nddsum+=WetterListe[iWetter+i-10]->vegetationslaenge;
						nddsummin+=WetterListe[iWetter+i-10]->vegetationslaengemin;

					}
					jantempsum=jantempsum/10;
					jantempsummin=jantempsummin/10;
					jultempsum=jultempsum/10;
					jultempsummin=jultempsummin/10;
					nddsum=nddsum/10;
					nddsummin=nddsummin/10;
				}
				else if (iWetter>1 && iWetter<=10)
				{
					for (unsigned int i=1; i<(iWetter+1); ++i)
					{
						jantempsum+=WetterListe[iWetter+i-iWetter]->temp1monatmittel;
						jantempsummin+=WetterListe[iWetter+i-iWetter]->temp1monatmittelmin;
						jultempsum+=WetterListe[iWetter+i-iWetter]->temp7monatmittel;
						jultempsummin+=WetterListe[iWetter+i-iWetter]->temp7monatmittelmin;
						nddsum+=WetterListe[iWetter+i-iWetter]->vegetationslaenge;
						nddsummin+=WetterListe[iWetter+i-iWetter]->vegetationslaengemin;

					}
					jantempsum=jantempsum/iWetter;
					jantempsummin=jantempsummin/iWetter;
					jultempsum=jultempsum/iWetter;
					jultempsummin=jultempsummin/iWetter;
					nddsum=nddsum/iWetter;
					nddsummin=nddsummin/iWetter;
				}
				else
				{
					jantempsum=WetterListe[iWetter]->temp1monatmittel;
					jantempsummin=WetterListe[iWetter]->temp1monatmittelmin;
					jultempsum=WetterListe[iWetter]->temp7monatmittel;
					jultempsummin=WetterListe[iWetter]->temp7monatmittelmin;
					nddsum=WetterListe[iWetter]->vegetationslaenge;
					nddsummin=WetterListe[iWetter]->vegetationslaengemin;
				}
				WetterListe[iWetter]->temp1monatmitteliso=jantempsum;
				WetterListe[iWetter]->temp1monatmittelisomin=jantempsummin;
				WetterListe[iWetter]->temp7monatmitteliso=jultempsum;
				WetterListe[iWetter]->temp7monatmittelisomin=jultempsummin;
				WetterListe[iWetter]->vegetationslaengeiso=(int) floor(nddsum);
				WetterListe[iWetter]->vegetationslaengeisomin=(int) floor(nddsummin);

				// calculation of a drought index
				double trockenheitsmortbuf=0.0;
				double precgs=	WetterListe[iWetter]->prec5monatmittel
									+ WetterListe[iWetter]->prec6monatmittel
									+ WetterListe[iWetter]->prec7monatmittel
									+ WetterListe[iWetter]->prec8monatmittel;
				double duerreindex=parameter[0].evapod*WetterListe[iWetter]->vegetationslaenge/precgs;
				if (duerreindex>1) 
				{
					double aattrockenheit=(WetterListe[iWetter]->activeairtemp/300.0)-1.0;
					if (aattrockenheit>1)
					{
						trockenheitsmortbuf=trockenheitsmortbuf+aattrockenheit*0.1;
					}
				}
				WetterListe[iWetter]->trockenheitsmort=trockenheitsmortbuf;


			if (parameter[0].qualijahranzeige==true)
			{
				printf("	Weather(%d; iWeather=%d): aati=%4.2f, gsind=%4.5f, preci=%4.2f  => maxbasalwachstumjahr=%4.4f & maxbrustwachstumjahr=%4.4f - \nDrought moratlity=%4.5f\n", WetterListe[iWetter]->jahr, iWetter, aatindex, gsindex, precindex, WetterListe[iWetter]->maxbasalwachstumjahr, WetterListe[iWetter]->maxbrustwachstumjahr, WetterListe[iWetter]->trockenheitsmort);
				printf("\tJanT10=%4.2f, JulT10=%4.2f, NDD10=%d\n",WetterListe[iWetter]->temp1monatmitteliso, WetterListe[iWetter]->temp7monatmitteliso, WetterListe[iWetter]->vegetationslaengeiso);
			}

		}
	}
}

/****************************************************************************************//**
 * \brief Read in weather data
 * 
 *
 *
 *
 *******************************************************************************************/
extern void Wettereinlesen(int baeumerows, struct Parameter *parameter,  int wortlaengemax, vector<vector<Wetter*> > &WeltWetterListe)
{
	char dateinametemp[50];
	char dateinameprec[50];


	int aktort=0;
	for (vector<vector<Wetter*> >::iterator posw = WeltWetterListe.begin(); posw != WeltWetterListe.end(); posw++)
	{
		vector<Wetter*>& WetterListe = *posw;
		aktort++;

		// as specified in the parameter.txt, the requested files are read in
			if (parameter[0].wetterwahl==1001)
			{

				if (aktort==1)
				{
					char tempbuf[]="ch02tmp_1919-2011.txt";
					char precbuf[]="ch02prc_1919-2011.txt";
					strcpy(dateinametemp, tempbuf);
					strcpy(dateinameprec, precbuf);
				}

				if (aktort==2)
				{
					char tempbuf[]="ch06tmp_1919-2011.txt";
					char precbuf[]="ch06prc_1919-2011.txt";
					strcpy(dateinametemp, tempbuf);
					strcpy(dateinameprec, precbuf);
				}

				if (aktort==3)
				{
					char tempbuf[]="ch12tmp_1919-2011.txt";
					char precbuf[]="ch12prc_1919-2011.txt";
					strcpy(dateinametemp, tempbuf);
					strcpy(dateinameprec, precbuf);
				}

				if (aktort==4)
				{
					char tempbuf[]="ch17tmp_1919-2011.txt";
					char precbuf[]="ch17prc_1919-2011.txt";
					strcpy(dateinametemp, tempbuf);
					strcpy(dateinameprec, precbuf);
				}

				if (aktort==5)
				{
					char tempbuf[]="ty04tmp_1919-2011.txt";
					char precbuf[]="ty04prc_1919-2011.txt";
					strcpy(dateinametemp, tempbuf);
					strcpy(dateinameprec, precbuf);
				}

				if (aktort==6)
				{
					char tempbuf[]="ty07tmp_1919-2011.txt";
					char precbuf[]="ty07prc_1919-2011.txt";
					strcpy(dateinametemp, tempbuf);
					strcpy(dateinameprec, precbuf);
				}

			}

			getTemp1(aktort, dateinametemp,  WetterListe);
			getPrec1(dateinameprec, WetterListe);

		if (parameter[0].parameterlesenanzeige ==true) 
		{
			for (unsigned int iWetter=0; iWetter<WetterListe.size(); ++iWetter)	
			{
				printf("%d,Tmean=%4.2f,Precsum=%4.2f, AAT=%4.2f, DDT=%4.2f, NDD=%d\n", WetterListe[iWetter]->jahr, WetterListe[iWetter]->tempjahrmittel, WetterListe[iWetter]->niederschlagssumme, WetterListe[iWetter]->activeairtemp, WetterListe[iWetter]->degreday, WetterListe[iWetter]->vegetationslaenge);
			}
		}
	}

	passWeather();

}
































