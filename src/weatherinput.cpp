using namespace std;


void getTemp1(int aktort, char dateinametemp[50],vector<Weather*>& weather_list)
{				
	int aktortyworldcoo=(int) floor( (double) (aktort-1)/parameter[0].mapxlength );
	int aktortxworldcoo=(aktort-1) - (aktortyworldcoo * parameter[0].mapxlength);

	//2.) Calculation of latitudinal position from parameter[0].nposmax and parameter[0].nposmin
	if (parameter[0].mapylength>1 && parameter[0].weathercalcgradient==true)
	{
		double Nposcenter=(parameter[0].nposmax+parameter[0].nposmin)/2;
		double mapylengthdummy=parameter[0].mapylength;	// irgendwie funktioniert nicht die direkte Verwendung in der nachfolgenden Formel??
		double Nposakt=parameter[0].nposmax-( (parameter[0].nposmax-parameter[0].nposmin)*aktortyworldcoo/(mapylengthdummy-1.0) );
		//3.) Calculate precipitation and temperature difference for each simulated plot, assuming a linear correlation: T,Prec~°latitude
		parameter[0].tempdiffort=-0.3508 * (Nposakt-Nposcenter);
		parameter[0].precdiffort=-5.3699 * (Nposakt-Nposcenter);
	}


	if (parameter[0].lineartransect==true)
	{
		
		//3.) Calculate precipitation and temperature difference for each simulated plot, assuming a linear correlation: T~°latitude
		parameter[0].tempdiffortmin=-0.3508 * treerows/(111120);
		parameter[0].precdiffortmin=-5.3699 * treerows/(111120);
	}


	// opening temperature input file
	FILE *f;
	f = fopen(dateinametemp,"r"); 
	if (f == NULL)
	{
		printf("Temperature file not available!\n");
		exit(1);
	}
	
	char puffer[255];
	int counter=1;
	double tempyearmeanbuf, temp1monthmeanbuf, temp2monthmeanbuf, temp3monthmeanbuf, temp4monthmeanbuf, temp5monthmeanbuf, temp6monthmeanbuf, temp7monthmeanbuf, temp8monthmeanbuf, temp9monthmeanbuf, temp10monthmeanbuf, temp11monthmeanbuf, temp12monthmeanbuf;

	//Read in line by line, partially preprocess data
	
	//***german:
	// Zeilenweise einlesen und entsprechende Spalteninhalte auslesen und teilweise verarbeiten
	while( fgets(puffer,255,f) !=NULL)
	{

	
		if (counter>=2)	// ignore header
		{
			temp1monthmeanbuf= strtod(strtok(puffer, " "),NULL);
			temp2monthmeanbuf= strtod(strtok(NULL, " "),NULL);
			temp3monthmeanbuf= strtod(strtok(NULL, " "),NULL) ;
			temp4monthmeanbuf= strtod(strtok(NULL, " "),NULL);
			temp5monthmeanbuf= strtod(strtok(NULL, " "),NULL) ;
			temp6monthmeanbuf= strtod(strtok(NULL, " "),NULL) ;
			temp7monthmeanbuf= strtod(strtok(NULL, " "),NULL);
			temp8monthmeanbuf= strtod(strtok(NULL, " "),NULL) ;
			temp9monthmeanbuf= strtod(strtok(NULL, " "),NULL) ;
			temp10monthmeanbuf= strtod(strtok(NULL, " "),NULL) ;
			temp11monthmeanbuf= strtod(strtok(NULL, " "),NULL) ;
			temp12monthmeanbuf= strtod(strtok(NULL, " "),NULL) ;

			tempyearmeanbuf=(temp1monthmeanbuf+temp2monthmeanbuf+temp3monthmeanbuf+temp4monthmeanbuf+temp5monthmeanbuf+temp6monthmeanbuf+temp7monthmeanbuf+temp8monthmeanbuf+temp9monthmeanbuf+temp10monthmeanbuf+temp11monthmeanbuf+temp12monthmeanbuf)/12;

			
			
			// add a new year
			pWeather= new Weather();			// 1. generate new year
			pWeather->yworldcoo=aktortyworldcoo;
			pWeather->xworldcoo=aktortxworldcoo;
			pWeather->jahr=counter+parameter[0].startjahr-2;					// 2. apply the data to the new year
			pWeather->tempyearmean=tempyearmeanbuf+parameter[0].tempdiffort;
			pWeather->temp1monthmean=temp1monthmeanbuf+parameter[0].tempdiffort;
			pWeather->temp1monthmeanmin=temp1monthmeanbuf+parameter[0].tempdiffort+parameter[0].tempdiffortmin;
			pWeather->temp2monthmean=temp2monthmeanbuf+parameter[0].tempdiffort;
			pWeather->temp3monthmean=temp3monthmeanbuf+parameter[0].tempdiffort;
			pWeather->temp4monthmean=temp4monthmeanbuf+parameter[0].tempdiffort;
			pWeather->temp5monthmean=temp5monthmeanbuf+parameter[0].tempdiffort;
			pWeather->temp6monthmean=temp6monthmeanbuf+parameter[0].tempdiffort;
			pWeather->temp7monthmean=temp7monthmeanbuf+parameter[0].tempdiffort;
			pWeather->temp7monthmeanmin=temp7monthmeanbuf+parameter[0].tempdiffort+parameter[0].tempdiffortmin;
			pWeather->temp8monthmean=temp8monthmeanbuf+parameter[0].tempdiffort;
			pWeather->temp9monthmean=temp9monthmeanbuf+parameter[0].tempdiffort;
			pWeather->temp10monthmean=temp10monthmeanbuf+parameter[0].tempdiffort;
			pWeather->temp11monthmean=temp11monthmeanbuf+parameter[0].tempdiffort;
			pWeather->temp12monthmean=temp12monthmeanbuf+parameter[0].tempdiffort;
			
			// Active Air Temperature AAT = Sum of days warmer than 10°C
			// Number of days with temperatures above zero degrees = net degree days = NDD ("Vegetationslaenge")
			double sumacttemp=0, sumacttempmin=0, sumdegreday=0;
			int ndegreday=0, ndegredaymin=0;

			// daily mean temperature estimation from mean july temperature:
			double julitemp=pWeather->temp7monthmean;
			double julitempmin=pWeather->temp7monthmeanmin;
			
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
			pWeather->activeairtemp=sumacttemp;
			pWeather->vegetationslaenge=ndegreday;
			pWeather->activeairtempmin=sumacttempmin;
			pWeather->vegetationslaengemin=ndegredaymin;
			pWeather->degreday=sumdegreday;
			weather_list.push_back(pWeather);// 3. push back the current year in weather_list

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
void getPrec1(char dateinameprec[50],vector<Weather*>& weather_list, int maximal_word_length)
{
			FILE *fp;
			fp = fopen(dateinameprec,"r");

			if (fp == NULL)
			{
				printf("Precipitation file not available!\n");
				exit(1);
			}

			int counter=1;
			char puffer[255];
			double niederschlagssummebuf;
			double prec1monthmeanbuf, prec2monthmeanbuf, prec3monthmeanbuf, prec4monthmeanbuf, prec5monthmeanbuf, prec6monthmeanbuf, prec7monthmeanbuf, prec8monthmeanbuf, prec9monthmeanbuf, prec10monthmeanbuf, prec11monthmeanbuf, prec12monthmeanbuf;

			while( fgets(puffer,maximal_word_length,fp) !=NULL)
			{
				if (counter>=2)
				{
					prec1monthmeanbuf= strtod(strtok(puffer, " "),NULL);
					prec2monthmeanbuf= strtod(strtok(NULL, " "),NULL);
					prec3monthmeanbuf= strtod(strtok(NULL, " "),NULL);
					prec4monthmeanbuf= strtod(strtok(NULL, " "),NULL);
					prec5monthmeanbuf= strtod(strtok(NULL, " "),NULL);
					prec6monthmeanbuf= strtod(strtok(NULL, " "),NULL);
					prec7monthmeanbuf= strtod(strtok(NULL, " "),NULL);
					prec8monthmeanbuf= strtod(strtok(NULL, " "),NULL);
					prec9monthmeanbuf= strtod(strtok(NULL, " "),NULL);
					prec10monthmeanbuf= strtod(strtok(NULL, " "),NULL);
					prec11monthmeanbuf= strtod(strtok(NULL, " "),NULL);
					prec12monthmeanbuf= strtod(strtok(NULL, " "),NULL);

					niederschlagssummebuf=(prec1monthmeanbuf+prec2monthmeanbuf+prec3monthmeanbuf+prec4monthmeanbuf+prec5monthmeanbuf+prec6monthmeanbuf+prec7monthmeanbuf+prec8monthmeanbuf+prec9monthmeanbuf+prec10monthmeanbuf+prec11monthmeanbuf+prec12monthmeanbuf);

					// append values to current year
					pWeather=weather_list[counter-2];			// 1. go to current year
					pWeather->prec1monthmean=prec1monthmeanbuf+parameter[0].precdiffort/12;					// 2. append values to current year
					pWeather->prec2monthmean=prec2monthmeanbuf+parameter[0].precdiffort/12;
					pWeather->prec3monthmean=prec3monthmeanbuf+parameter[0].precdiffort/12;
					pWeather->prec4monthmean=prec4monthmeanbuf+parameter[0].precdiffort/12;
					pWeather->prec5monthmean=prec5monthmeanbuf+parameter[0].precdiffort/12;
					pWeather->prec6monthmean=prec6monthmeanbuf+parameter[0].precdiffort/12;
					pWeather->prec7monthmean=prec7monthmeanbuf+parameter[0].precdiffort/12;
					pWeather->prec8monthmean=prec8monthmeanbuf+parameter[0].precdiffort/12;
					pWeather->prec9monthmean=prec9monthmeanbuf+parameter[0].precdiffort/12;
					pWeather->prec10monthmean=prec10monthmeanbuf+parameter[0].precdiffort/12;
					pWeather->prec11monthmean=prec11monthmeanbuf+parameter[0].precdiffort/12;
					pWeather->prec12monthmean=prec12monthmeanbuf+parameter[0].precdiffort/12;
					pWeather->niederschlagssumme=niederschlagssummebuf+parameter[0].precdiffort;
					pWeather->niederschlagssummemin=niederschlagssummebuf+parameter[0].precdiffort+parameter[0].precdiffortmin;	

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
void getTemp2(int aktort, char dateinametemp[50],vector<Weather*>& weather_list,int maximal_word_length)
{
			//1.) Calculation of the current transect coordinate
	
			//***german:
			// Berechnung des aktuellen Ortes
			int aktortyworldcoo=(int) floor( (double) (aktort-1)/parameter[0].mapxlength );
			int aktortxworldcoo=(aktort-1) - (aktortyworldcoo * parameter[0].mapxlength);

			//2.) Calculation of latitudinal position from parameter[0].nposmax and parameter[0].nposmin
	
			//***german:
			// 2. N_inGrad aus den N_max und N_min und Ygesamt-werten berechnen
			if (parameter[0].mapylength>1 && parameter[0].weathercalcgradient==true)
			{
				double Nposcenter=(parameter[0].nposmax+parameter[0].nposmin)/2;
			
				double mapylengthdummy=parameter[0].mapylength;	
				double Nposakt=parameter[0].nposmax-( (parameter[0].nposmax-parameter[0].nposmin)*aktortyworldcoo/(mapylengthdummy-1.0) );
				//3.) Calculate precipitation and temperature difference for each simulated plot, assuming a linear correlation: T,Prec~°latitude
		
				//***german:
				// 3. Differenzwerte berechnen mit Prec=-5.3699 pro Grad und Temp=-0.3508 pro Grad
				parameter[0].tempdiffort=-0.3508 * (Nposakt-Nposcenter);
				parameter[0].precdiffort=-5.3699 * (Nposakt-Nposcenter);
			}
			else
			{
				parameter[0].tempdiffort=0.0;
				parameter[0].precdiffort=0.0;
			}


			if (parameter[0].lineartransect==true)
			{
				//3.) Calculate precipitation and temperature difference for each simulated plot, assuming a linear correlation: T,Prec~°latitude
		
				//***german:
				// 3. Differenzwerte berechnen mit Prec=-5.3699 pro Grad und Temp=-0.3508 pro Grad
				// Umrechnung von Grad in Meter  "1 Grad = 60 x 1852 m = 111120 m"
				parameter[0].tempdiffortmin=-0.3508 * treerows/(111120);
				parameter[0].precdiffortmin=-5.3699 * treerows/(111120);
			}


			// opening file
			FILE *f;
			f = fopen(dateinametemp,"r"); 
			if (f == NULL)
			{
				printf("Temperaturdatei nicht vorhanden!\n");
				exit(1);
			}

			char puffer[255];
			int counter=1;
			double jahrbuf, tempyearmeanbuf;

			//Read in line by line, partially preprocess data
	
			//***german:
			// Zeilenweise einlesen und entsprechende Spalteninhalte auslesen und teilweise verarbeiten
			while( fgets(puffer,maximal_word_length,f) !=NULL)
			{
				if (counter>=2)
				{
					jahrbuf= strtod(strtok(puffer, " "),NULL);
					tempyearmeanbuf= strtod(strtok(NULL, " "),NULL);

					// add a new year
					pWeather= new Weather();			// 1. generate new year
					pWeather->yworldcoo=aktortyworldcoo;
					pWeather->xworldcoo=aktortxworldcoo;
					pWeather->jahr=(int) floor(jahrbuf);					// 2. apply the data to the new year
					pWeather->tempyearmean=tempyearmeanbuf+parameter[0].tempdiffort;
					pWeather->tempyearmeanmin=tempyearmeanbuf+parameter[0].tempdiffort+parameter[0].tempdiffortmin;
					
					// Active Air Temperature AAT = Sum of days warmer than 10°C
					// Number of days with temperatures above zero degrees = net degree days = NDD ("Vegetationslaenge")
					double sumacttemp=0;
					double sumacttempmin=0;
					double sumdegreday=0;
					int ndegreday=0;
					int ndegredaymin=0;

					// daily mean temperature estimation from mean july temperature:

					double tempyearmeanbuf=pWeather->tempyearmean;
					double tempyearmeanminbuf=pWeather->tempyearmeanmin;
					double temp1monthmeanbuf=0, temp1monthmeanminbuf=0;
					double temp7monthmeanbuf=0, temp7monthmeanminbuf=0;

					// Parameters of the sine function
					double ausdehnung=0.567980348064239;
					double verschiebung=4.03428152771763;
					double amplianstieg=-0.585543287044634;
					double ampliyschn=15.5976677143297;
					double yabsanstieg=-0.881262021169085;
					double yabsschn=-0.00908805024019213;
					double ampli=1.05*(ampliyschn+amplianstieg*tempyearmeanbuf);
					double amplimin=1.05*(ampliyschn+amplianstieg*tempyearmeanminbuf);
					double yabs=yabsschn+yabsanstieg*tempyearmeanbuf;
					double yabsmin=yabsschn+yabsanstieg*tempyearmeanminbuf;

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
						{ //calculate january mean value
							temp1monthmeanbuf=temp1monthmeanbuf+tagestemp;
							temp1monthmeanminbuf=temp1monthmeanminbuf+tagestempmin;
						} //calculate january mean value
					
						if ( (i>=182) & (i<=212) )
						{ //calculate july mean value
							temp7monthmeanbuf=temp7monthmeanbuf+tagestemp;
							temp7monthmeanminbuf=temp7monthmeanminbuf+tagestempmin;
						} //calculate july mean value
					}

					pWeather->activeairtemp=sumacttemp;
					pWeather->degreday=sumdegreday;
					pWeather->vegetationslaenge=ndegreday;
					pWeather->activeairtempmin=sumacttempmin;
					pWeather->vegetationslaengemin=ndegredaymin;
					pWeather->temp1monthmean=temp1monthmeanbuf/31;
					pWeather->temp1monthmeanmin=temp1monthmeanminbuf/31;
					pWeather->temp7monthmean=temp7monthmeanbuf/31;
					pWeather->temp7monthmeanmin=temp7monthmeanminbuf/31;
					weather_list.push_back(pWeather);// 3. push back the current year in weather_list

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
void getPrec2(char dateinameprec[50],vector<Weather*>& weather_list,int maximal_word_length)
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
			double prec1monthmeanbuf;
			double prec2monthmeanbuf;
			double prec3monthmeanbuf;
			double prec4monthmeanbuf;
			double prec5monthmeanbuf;
			double prec6monthmeanbuf;
			double prec7monthmeanbuf;
			double prec8monthmeanbuf;
			double prec9monthmeanbuf;
			double prec10monthmeanbuf;
			double prec11monthmeanbuf;
			double prec12monthmeanbuf;

			while( fgets(puffer,maximal_word_length,fp) !=NULL)
			{
				if (counter>=2)
				{
					strtod(strtok(puffer, " "),NULL);
					prec1monthmeanbuf= strtod(strtok(NULL, " "),NULL);
					prec2monthmeanbuf= strtod(strtok(NULL, " "),NULL);
					prec3monthmeanbuf= strtod(strtok(NULL, " "),NULL);
					prec4monthmeanbuf= strtod(strtok(NULL, " "),NULL);
					prec5monthmeanbuf= strtod(strtok(NULL, " "),NULL);
					prec6monthmeanbuf= strtod(strtok(NULL, " "),NULL);
					prec7monthmeanbuf= strtod(strtok(NULL, " "),NULL);
					prec8monthmeanbuf= strtod(strtok(NULL, " "),NULL);
					prec9monthmeanbuf= strtod(strtok(NULL, " "),NULL);
					prec10monthmeanbuf= strtod(strtok(NULL, " "),NULL);
					prec11monthmeanbuf= strtod(strtok(NULL, " "),NULL);
					prec12monthmeanbuf= strtod(strtok(NULL, " "),NULL);

					niederschlagssummebuf=(prec1monthmeanbuf+prec2monthmeanbuf+prec3monthmeanbuf+prec4monthmeanbuf+prec5monthmeanbuf+prec6monthmeanbuf+prec7monthmeanbuf+prec8monthmeanbuf+prec9monthmeanbuf+prec10monthmeanbuf+prec11monthmeanbuf+prec12monthmeanbuf);

					// append values to current year
					pWeather=weather_list[counter-2];			// 1. go to current year
					pWeather->prec1monthmean=prec1monthmeanbuf+parameter[0].precdiffort/12;					// 2. append values to current year
					pWeather->prec2monthmean=prec2monthmeanbuf+parameter[0].precdiffort/12;
					pWeather->prec3monthmean=prec3monthmeanbuf+parameter[0].precdiffort/12;
					pWeather->prec4monthmean=prec4monthmeanbuf+parameter[0].precdiffort/12;
					pWeather->prec5monthmean=prec5monthmeanbuf+parameter[0].precdiffort/12;
					pWeather->prec6monthmean=prec6monthmeanbuf+parameter[0].precdiffort/12;
					pWeather->prec7monthmean=prec7monthmeanbuf+parameter[0].precdiffort/12;
					pWeather->prec8monthmean=prec8monthmeanbuf+parameter[0].precdiffort/12;
					pWeather->prec9monthmean=prec9monthmeanbuf+parameter[0].precdiffort/12;
					pWeather->prec10monthmean=prec10monthmeanbuf+parameter[0].precdiffort/12;
					pWeather->prec11monthmean=prec11monthmeanbuf+parameter[0].precdiffort/12;
					pWeather->prec12monthmean=prec12monthmeanbuf+parameter[0].precdiffort/12;
					pWeather->niederschlagssumme=niederschlagssummebuf+parameter[0].precdiffort;
					pWeather->niederschlagssummemin=niederschlagssummebuf+parameter[0].precdiffort+parameter[0].precdiffortmin;	// Berechnung oben

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
void getTemp3(int aktort, char dateinametemp[50],vector<Weather*>& weather_list)
{				
	
	//1.) Calculation of the current transect coordinate
	
	//***german:
	// Berechnung des aktuellen Ortes
	int aktortyworldcoo=(int) floor( (double) (aktort-1)/parameter[0].mapxlength );
	int aktortxworldcoo=(aktort-1) - (aktortyworldcoo * parameter[0].mapxlength);

	//2.) Calculation of latitudinal position from parameter[0].nposmax and parameter[0].nposmin
	
	//***german:
	// 2. N_inGrad aus den N_max und N_min und Ygesamt-werten berechnen
	if (parameter[0].mapylength>1 && parameter[0].weathercalcgradient==true)
	{
		double Nposcenter=(parameter[0].nposmax+parameter[0].nposmin)/2;
		double mapylengthdummy=parameter[0].mapylength;	// irgendwie funktioniert nicht die direkte Verwendung in der nachfolgenden Formel??
		double Nposakt=parameter[0].nposmax-( (parameter[0].nposmax-parameter[0].nposmin)*aktortyworldcoo/(mapylengthdummy-1.0) );
		
		//3.) Calculate precipitation and temperature difference for each simulated plot, assuming a linear correlation: T,Prec~°latitude
		
		//***german:
		// 3. Differenzwerte berechnen mit Prec=-5.3699 pro Grad und Temp=-0.3508 pro Grad
		
		
		// Conversion degrees to metres  "1 degree = 60 x 1852 m = 111120 m"
		parameter[0].tempdiffort=-0.3508 * (Nposakt-Nposcenter);
		parameter[0].precdiffort=-5.3699 * (Nposakt-Nposcenter);
	}


	if (parameter[0].lineartransect==true)
	{
		//3.) Calculate precipitation and temperature difference for each simulated plot, assuming a linear correlation: T,Prec~°latitude
		
		//***german:
		// 3. Differenzwerte berechnen mit Prec=-5.3699 pro Grad und Temp=-0.3508 pro Grad
		
		
		// Conversion degrees to metres  "1 degree = 60 x 1852 m = 111120 m"
		parameter[0].tempdiffortmin=-0.3508 * treerows/(111120);
		parameter[0].precdiffortmin=-5.3699 * treerows/(111120);
	}



	// opening file
	FILE *f;
	f = fopen(dateinametemp,"r"); 
	if (f == NULL)
	{
		printf("Temperaturdatei nicht vorhanden!\n");
		exit(1);
	}
	
	char puffer[255];
	int counter=1;
	double tempyearmeanbuf, temp1monthmeanbuf, temp2monthmeanbuf, temp3monthmeanbuf, temp4monthmeanbuf, temp5monthmeanbuf, temp6monthmeanbuf, temp7monthmeanbuf, temp8monthmeanbuf, temp9monthmeanbuf, temp10monthmeanbuf, temp11monthmeanbuf, temp12monthmeanbuf;

	//Read in line by line, partially preprocess data
	
	//***german:
	// Zeilenweise einlesen und entsprechende Spalteninhalte auslesen und teilweise verarbeiten
	while( fgets(puffer,255,f) !=NULL)
	{

	
		if (counter>=2)	// ignore header
		{
			temp1monthmeanbuf= strtod(strtok(puffer, " "),NULL);
			temp2monthmeanbuf= strtod(strtok(NULL, " "),NULL);
			temp3monthmeanbuf= strtod(strtok(NULL, " "),NULL) ;
			temp4monthmeanbuf= strtod(strtok(NULL, " "),NULL);
			temp5monthmeanbuf= strtod(strtok(NULL, " "),NULL) ;
			temp6monthmeanbuf= strtod(strtok(NULL, " "),NULL) ;
			temp7monthmeanbuf= strtod(strtok(NULL, " "),NULL);
			temp8monthmeanbuf= strtod(strtok(NULL, " "),NULL) ;
			temp9monthmeanbuf= strtod(strtok(NULL, " "),NULL) ;
			temp10monthmeanbuf= strtod(strtok(NULL, " "),NULL) ;
			temp11monthmeanbuf= strtod(strtok(NULL, " "),NULL) ;
			temp12monthmeanbuf= strtod(strtok(NULL, " "),NULL) ;

			tempyearmeanbuf=(temp1monthmeanbuf+temp2monthmeanbuf+temp3monthmeanbuf+temp4monthmeanbuf+temp5monthmeanbuf+temp6monthmeanbuf+temp7monthmeanbuf+temp8monthmeanbuf+temp9monthmeanbuf+temp10monthmeanbuf+temp11monthmeanbuf+temp12monthmeanbuf)/12;

			// add a new year
			pWeather= new Weather();			// 1. generate new year
			pWeather->yworldcoo=aktortyworldcoo;
			pWeather->xworldcoo=aktortxworldcoo;
			pWeather->jahr=counter+parameter[0].startjahr-2;					// 2. apply the data to the new year
			pWeather->tempyearmean=tempyearmeanbuf+parameter[0].tempdiffort;
			pWeather->temp1monthmean=temp1monthmeanbuf+parameter[0].tempdiffort;
			pWeather->temp1monthmeanmin=temp1monthmeanbuf+parameter[0].tempdiffort+parameter[0].tempdiffortmin;
			pWeather->temp2monthmean=temp2monthmeanbuf+parameter[0].tempdiffort;
			pWeather->temp3monthmean=temp3monthmeanbuf+parameter[0].tempdiffort;
			pWeather->temp4monthmean=temp4monthmeanbuf+parameter[0].tempdiffort;
			pWeather->temp5monthmean=temp5monthmeanbuf+parameter[0].tempdiffort;
			pWeather->temp6monthmean=temp6monthmeanbuf+parameter[0].tempdiffort;
			pWeather->temp7monthmean=temp7monthmeanbuf+parameter[0].tempdiffort;
			pWeather->temp7monthmeanmin=temp7monthmeanbuf+parameter[0].tempdiffort+parameter[0].tempdiffortmin;
			pWeather->temp8monthmean=temp8monthmeanbuf+parameter[0].tempdiffort;
			pWeather->temp9monthmean=temp9monthmeanbuf+parameter[0].tempdiffort;
			pWeather->temp10monthmean=temp10monthmeanbuf+parameter[0].tempdiffort;
			pWeather->temp11monthmean=temp11monthmeanbuf+parameter[0].tempdiffort;
			pWeather->temp12monthmean=temp12monthmeanbuf+parameter[0].tempdiffort;
			
			// Active Air Temperature AAT = Sum of days warmer than 10°C
			// Number of days with temperatures above zero degrees = net degree days = NDD ("Vegetationslaenge")
			double sumacttemp=0, sumacttempmin=0, sumdegreday=0;
			int ndegreday=0, ndegredaymin=0;

			// daily mean temperature estimation from mean july temperature:
			double julitemp=pWeather->temp7monthmean;
			double julitempmin=pWeather->temp7monthmeanmin;
			double jantemp=pWeather->temp1monthmean;
			
			for (int i=1;i<=365;i++)
			{
				
				// calculate each day once for each january or july temperature oriented function 
				
				//***german:
				// für jede der zwei an Jan- bzw Jultemp. aufgehängten Fkt einmal den Tag berechnen (weatherfunktionsabschätzung.docx)
				double tagestempjan=0.0;
				double tagestempjul=0.0;
				double tagestemp=0.0;

				tagestempjan=((22.55131 *sin(0.49102*( (double) i/(365.0/12.0)) +4.62730))+22.04378+jantemp);	
				tagestempjul=((22.55261 *sin(0.49063*( (double) i/(365.0/12.0)) +4.62760))-22.27861+julitemp);	
				
				// incorporate daily temperatures of january and july 
				
				//***german:
				//tagestempjan und tagestempjul verrechnen (weatherfunktionsabschätzung.docx)
				double weight=0.0;
				if((i>=1) &( i<=15))
				{
					weight=((double) 1.0*((15.0-i)/184.0));
				} 
				if((i >=16) & (i<=196))
				{
					weight =((double) 1.0*((i-15.0)/181.0));
				}
				if((i>=197) & (i<=365))
				{
					weight =((double) 1.0*((365.0+15.0-i)/184.0));
				}

				tagestemp=tagestempjan*(1-weight)+tagestempjul*weight;
				
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
			
					
			pWeather->activeairtemp=(int) round(sumacttemp*0.84630);
			pWeather->vegetationslaenge=(int) round(ndegreday*0.88432);
			pWeather->activeairtempmin=(int) round(sumacttempmin*0.84630);
			pWeather->vegetationslaengemin=(int) round(ndegredaymin*0.88432);
			pWeather->degreday=(int) round(sumdegreday*0.87583);
			weather_list.push_back(pWeather);// 3. push back the current year in weather_list
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
	
	for (vector<vector<Weather*> >::iterator posw = world_weather_list.begin(); posw != world_weather_list.end(); posw++)
	{// world weather list loop begin
		vector<Weather*>& weather_list = *posw;

		for (unsigned int iweather=0; iweather<weather_list.size(); ++iweather)	
		{// weather list loop begin

			// Active Air Temperature: >600°C = Forest taiga (Osawa 2010, S. 53) & 300°C -> northern border (Osawa 2010, S. 47)
			double julindexs = 0;
			double julindexmins = 0;
			double julindexg = 0;
			double julindexming	= 0;
			julindexs = (0.163/(1+exp(12.319-weather_list[iweather]->temp7monthmean)))+0.168;
			julindexmins = (0.163/(1+exp(12.319-weather_list[iweather]->temp7monthmeanmin)))+0.168;
			julindexg = (0.078/(1+exp(14.825-weather_list[iweather]->temp7monthmean)))+0.108;
			julindexming = (0.078/(1+exp(14.825-weather_list[iweather]->temp7monthmeanmin)))+0.108;
			// Growing Season length: 60 days -> northern border (Osawa 2010, S. 47)  
			//CRU_TS data set: 120 days>0°C in the north!!
			
			//***german:
			//Beachte: CRU_TS: ca. 120 Tage über 0 im Norden!!! 
			
			//***german:
			//????
			/* da Julindex aufgrund von Fkt Julitemp-realisiertes Max-Growth gefittet wurde, ist das reale Growth schon eingeflossen.
			Da nur dimensionsloser Faktor gewünscht für spätere Berechnung des Growths hier prozentualer Wert ermittelt zwischen minimalem und maximalem Growth, indem von dem berechneten Wert (julindexg/s) 
			min. Growth abgezogen wird und dann durch die Spanne max. - min. Growth geteilt wird. Daten sind aus Yamal-R-Datei mit weightetem CRUTS-Datensatz*/
			weather_list[iweather]->weatherfactorg=(double) (julindexg-0.108)/(0.1771-0.108); 
			weather_list[iweather]->weatherfactorming=(double) (julindexming-0.108)/(0.1771-0.108);  
			weather_list[iweather]->weatherfactors=(double) (julindexs-0.168)/(0.305-0.168); 
			weather_list[iweather]->weatherfactormins=(double) (julindexmins-0.168)/(0.305-0.168);  
			
			// Test wegen BasalDwachstum, bei Growth, Mort und Etab gucken, dass direkt das maxwaxchstumpro jahr einflieszt daher auch den rest ausgeblendet
			// Langfristige Mittelwerte
			// weatherkennwerte
			// 
			// -30°C January isotherm -> borderline for plant abundance (Osawa 2010, S. 48)
			double jantempsum=0;
			double jantempsummin=0;
			// 
			// 10°C July isotherm -> northern border (Osawa 2010, S. 47)
			double jultempsum=0;
			double jultempsummin=0;
			// // Growing Season length = 60 days -> northern border (Osawa 2010, S. 47) 
			//CRU_TS data set: 120 days>0°C in the north!! (no influence by now?)
			
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
					}
					else
					{
						jantempsum+=weather_list[iweather+i-10]->temp1monthmean;
						jantempsummin+=weather_list[iweather+i-10]->temp1monthmeanmin;
					}
					jultempsum+=weather_list[iweather+i-10]->temp7monthmean;
					jultempsummin+=weather_list[iweather+i-10]->temp7monthmeanmin;
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
					}
					else
					{
						jantempsum+=weather_list[i]->temp1monthmean;
						jantempsummin+=weather_list[i]->temp1monthmeanmin;
					}				
					jultempsum+=weather_list[i]->temp7monthmean;
					jultempsummin+=weather_list[i]->temp7monthmeanmin;
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
				}
				else
				{
					jantempsum=weather_list[iweather]->temp1monthmean;
					jantempsummin=weather_list[iweather]->temp1monthmeanmin;
				}					
				jultempsum=weather_list[iweather]->temp7monthmean;
				jultempsummin=weather_list[iweather]->temp7monthmeanmin;
				nddsum=weather_list[iweather]->vegetationslaenge;
				nddsummin=weather_list[iweather]->vegetationslaengemin;
			}
			weather_list[iweather]->temp1monthmeaniso=jantempsum;
			weather_list[iweather]->temp1monthmeanisomin=jantempsummin;
			weather_list[iweather]->temp7monthmeaniso=jultempsum;
			weather_list[iweather]->temp7monthmeanisomin=jultempsummin;
			weather_list[iweather]->vegetationslaengeiso=(int) floor(nddsum);
			weather_list[iweather]->vegetationslaengeisomin=(int) floor(nddsummin);

			// Calculation of a dryness influence
			// // "Evaporation rates of 1.5 mm/day mean growing season" (Dolmann, 2004)
			double trockenheitsmortbuf=0.0;
			double precgs=	weather_list[iweather]->prec5monthmean
								+ weather_list[iweather]->prec6monthmean
								+ weather_list[iweather]->prec7monthmean
								+ weather_list[iweather]->prec8monthmean;
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

			if (parameter[0].qualiyearlyvis==true)
			{// console output begin
				printf("	weather(%d; iweather=%d): weatherfactorg=%4.4f, weatherfactors=%4.4f ===> \ndroughtmort=%4.5f\n", weather_list[iweather]->jahr, iweather, weather_list[iweather]->weatherfactorg, weather_list[iweather]->weatherfactors, weather_list[iweather]->trockenheitsmort);
				printf("\tJanT10=%4.2f, JuliT10=%4.2f, NDD10=%d\n",weather_list[iweather]->temp1monthmeaniso, weather_list[iweather]->temp7monthmeaniso, weather_list[iweather]->vegetationslaengeiso);
			}// console output end

			
			// Abiotic influences:
			// weather dependent mortality from 10 yearly mean values
			// -30°C january isotherm -> borderline of plant abundance (Osawa 2010, S. 48)
			
			
			weather_list[iweather]->janisothermrestriktiong=0.0;
			weather_list[iweather]->janisothermrestriktions=0.0;
			
			//  10°C july isotherm -> northern border (Osawa 2010, S. 47)
			weather_list[iweather]->julisothermrestriktion=0.0;
			
			// Growing Season length = 60 days -> northern border (Osawa 2010, S. 47)
			weather_list[iweather]->nddrestriktion=0.0;

			/// mean January temp < -40°C?
			if (weather_list[iweather]->temp1monthmeaniso<(parameter[0].janthresholdtempgmel))
			{
				weather_list[iweather]->janisothermrestriktiong=1.0;
			}
			else
			{
				weather_list[iweather]->janisothermrestriktiong=1.0-fabs(9.0*(weather_list[iweather]->temp1monthmeaniso-parameter[0].janthresholdtempgmel)/(-parameter[0].janthresholdtempgmel));	// hier Betrag ueberfluessig da Zahl niemals negativ da nur Zahlen <= -45.0 °C
			}


			/// mean January temp < -40°C?
			if (weather_list[iweather]->temp1monthmeaniso<(-33.0))
			{
				weather_list[iweather]->janisothermrestriktions=1.0;
			}
			else
			{
				weather_list[iweather]->janisothermrestriktions=1.0-fabs(6.6*(weather_list[iweather]->temp1monthmeaniso+33.0)/33.0);	// hier Betrag ueberfluessig da Zahl niemals negativ da nur Zahlen <= -33.0 °C
			}		
			
			/// mean July temp < 10°C?
			if (weather_list[iweather]->temp7monthmeaniso<10.0) 
			{
				weather_list[iweather]->julisothermrestriktion=1.0;
			}
			else 
			{
				weather_list[iweather]->julisothermrestriktion=1.0-fabs(2.0*(weather_list[iweather]->temp7monthmeaniso-10.0)/10.0);	// hier Betrag ueberfluessig da Zahl niemals negativ da nur Zahlen >= 10.0 °C
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
				
		}// weather_list end
	}//world weather list loop end
}










/****************************************************************************************//**
 * \brief 
 *
 *
 *******************************************************************************************/
extern void Weatherinput( struct Parameter *parameter,  int stringlengthmax, vector<vector<Weather*> > &world_weather_list)
{

	// -- -- -- -- Weatherinput START -- -- -- -- //
	char dateinametemp[50];
	char dateinameprec[50];
	
	
	if((parameter[0].windsource!=0)&&(parameter[0].windsource!=4)&&(parameter[0].windsource!=5))
	{
		int findyr1=0,findyr2=-100,jahr=0,cntr=0;
		
		string filename;
		std::ostringstream ss;
		string item;
		
		for (int t=0;t<parameter[0].simduration;t++)
		{ // year step iteration begin:
			
			cntr=0;
			
			jahr=parameter[0].startjahr+t;
			
			if(parameter[0].windsource==1)
			{
				findyr1=1979;findyr2=2012;
			}
			
			
		ss.str("");
		ss.clear();  	
		
		if((jahr<findyr2+1) && (jahr>findyr1-1))
		{
			ss<<jahr;
								
			if(parameter[0].windsource==1)
			{
				filename="input/winddata/winddata"+ss.str()+"_EraInterim.dat";
			}
					
				ifstream fileinp(filename.c_str());

				while(fileinp>>item)
				{
				  cntr++;
				  if(cntr%2)
				  {
					if(stof(item)>=0 && stof(item)<=360)
					{
						wdir.push_back(stof(item));
					}
					else
					{
						wdir.push_back(0);
					}
				  }
				  else
				  {
					  wspd.push_back(stof(item));
				  }
				}
				
				if(cntr>0)
				{
					windspd.push_back(wspd);
					winddir.push_back(wdir);
					globalyears.push_back(jahr);
				}
				
				wspd.clear();
				wdir.clear();
				wspd.shrink_to_fit();
				wdir.shrink_to_fit();
			}
	
		}
		jahr=0;
	}
	

	int aktort=0;
	for (vector<vector<Weather*> >::iterator posw = world_weather_list.begin(); posw != world_weather_list.end(); posw++)
	{
		vector<Weather*>& weather_list = *posw;
		aktort++;

		// depending on the weather choice different files will be opened and read line by line
		if (parameter[0].weatherchoice==21)
		{
			char tempbuf[]="input/TY02_tmpweighted_model.csv";
			char precbuf[]="input/TY02_prcweighted_model.csv";
			strcpy(dateinametemp, tempbuf);
			strcpy(dateinameprec, precbuf);
		}
		if (parameter[0].weatherchoice==22)
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
		getTemp3(aktort, dateinametemp,  weather_list);
		getPrec1(dateinameprec, weather_list,stringlengthmax);
		
		parameter[0].parameterinputvis=true;
	}

	// -- -- -- -- Weatherinput END -- -- -- -- //
	passWeather();

}
