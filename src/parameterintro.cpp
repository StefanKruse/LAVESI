using namespace std;

double Parametereingabe(char *uebergabestring, int stringlengthmax, char *trennzeichen, struct Parameter *parameter)
{
	// Opening the parameter file
	FILE *f;
	f = fopen("parameters.txt","r");
	if (f == NULL)
	{
		printf("Parameterdatei nicht vorhanden!\n");
		exit(1);
	}

	// Look for the parameter name in said file
	char searchstring[255];
	strcpy(searchstring , uebergabestring);

	char puffer[255], puffer_bak[255], puffer_zahl[255];
	int counter=1;
	char *wort;
	char *zahl;

	while( fgets(puffer, stringlengthmax, f) != NULL)
	{
		strcpy(puffer_bak, puffer);
		wort = strtok(puffer, trennzeichen);
		while (wort != NULL)
		{
			// Check if the parameter is found, read in said parameter
			if ( strcmp(wort, searchstring) == 0)
			{
				strcpy(puffer_zahl, puffer_bak);
			}
			wort = strtok(NULL, trennzeichen);
		}
		counter++;
	}

	// Type conversion from string to double
	strtok(puffer_zahl, trennzeichen);
	zahl= strtok(NULL, trennzeichen);
	double zahldouble=strtod(zahl, NULL);
	if (parameter[0].parameterinputvis==true) 
	{
		printf("Suche:\t%s <= %4.5f \n", searchstring, zahldouble);
	}

	fclose(f);
	wort=NULL;
	zahl=NULL;

	return zahldouble;
}



void Parametereinlesen(void)
{
	
	char uebergabestring[stringlengthmax];

	// Switch for functions and display
	strcpy(uebergabestring,"parameterinputvis");
	if (((int) Parametereingabe(&uebergabestring[0], stringlengthmax, &trennzeichen[0], &parameter[0]))==1) 
        {
            parameter[0].parameterinputvis=true;
        }
	else 
        {
            parameter[0].parameterinputvis=false;
        };
	
			if (parameter[0].parameterinputvis ==true) 
			{
                printf("\n---->\t PARAMETEREINGABE WIRD ANGEZEIGT\n\n\t Schage fuer Funktionen und Anzeigen\n\nread:	%s <= %s ==> Main \n", uebergabestring,		(parameter[0].parameterinputvis)?"true":"false");
            }

	strcpy(uebergabestring,"yearlyvis");
	if (((int) Parametereingabe(&uebergabestring[0], stringlengthmax, &trennzeichen[0], &parameter[0]))==1) 
        {
            parameter[0].yearlyvis=true;
        }
	else 
        {
            parameter[0].yearlyvis=false;
        };

			if (parameter[0].parameterinputvis ==true) 
			{
                printf("read:	%s <= %s ==> Main \n", uebergabestring, (parameter[0].yearlyvis)?"true":"false");
            }

	strcpy(uebergabestring,"qualiyearlyvis");
	if (((int) Parametereingabe(&uebergabestring[0], stringlengthmax, &trennzeichen[0], &parameter[0]))==1) 
        {
            parameter[0].qualiyearlyvis=true;
        }
	else 
        {
            parameter[0].qualiyearlyvis=false;
        };
			if (parameter[0].parameterinputvis ==true) 
			{
                printf("read:	%s <= %s ==> Main \n", uebergabestring, (parameter[0].qualiyearlyvis)?"true":"false");
            }
	
	strcpy(uebergabestring,"dataoutput");
		if (((int) Parametereingabe(&uebergabestring[0], stringlengthmax, &trennzeichen[0], &parameter[0]))==1) 
        {
            parameter[0].dataoutput=true;
        }
		else 
        {
            parameter[0].dataoutput=false;
        };
			if (parameter[0].parameterinputvis ==true) 
            {
                printf("read:	%s <= %s ==> Main \n", uebergabestring, (parameter[0].dataoutput)?"true":"false");
            }

	strcpy(uebergabestring,"outputmode");
	parameter[0].outputmode=(int) (Parametereingabe(&uebergabestring[0], stringlengthmax, &trennzeichen[0], &parameter[0]));
			if (parameter[0].parameterinputvis ==true) 
            {
                printf("read:	%s <= %d ==> Main\n", uebergabestring, parameter[0].outputmode);
            }

	strcpy(uebergabestring,"seeddispvis");
	if (((int) Parametereingabe(&uebergabestring[0], stringlengthmax, &trennzeichen[0], &parameter[0]))==1) 
	{
		parameter[0].seeddispvis=true;
	}
	else 
	{
		parameter[0].seeddispvis=false;
	};
			if (parameter[0].parameterinputvis ==true) 
			{
				printf("read:	%s <= %s ==> Main \n", uebergabestring, (parameter[0].seeddispvis)?"true":"false");
			}

	strcpy(uebergabestring,"mortvis");
	if (((int) Parametereingabe(&uebergabestring[0], stringlengthmax, &trennzeichen[0], &parameter[0]))==1) 
        {
            parameter[0].mortvis=true;
        }
	else 
        {
            parameter[0].mortvis=false;
        };
	if (parameter[0].parameterinputvis ==true) 
            {
                printf("read:	%s <= %s ==> Main \n", uebergabestring, (parameter[0].mortvis)?"true":"false");
            }

	// General parameters for the function
	strcpy(uebergabestring,"evapod");
	parameter[0].evapod=Parametereingabe(&uebergabestring[0], stringlengthmax, &trennzeichen[0], &parameter[0]);
	if (parameter[0].parameterinputvis ==true) 
            {
                printf("read:	%s <= %4.3f ==> Main \n", uebergabestring, parameter[0].evapod);
            }

	strcpy(uebergabestring,"precweather");
	parameter[0].precweather=(int) (Parametereingabe(&uebergabestring[0], stringlengthmax, &trennzeichen[0], &parameter[0]));
	if (parameter[0].parameterinputvis ==true) 
		{
                    printf("read:	%s <= %d ==> Main\n", uebergabestring, parameter[0].precweather);
                }

	strcpy(uebergabestring,"precthreshold");
	parameter[0].precthreshold=Parametereingabe(&uebergabestring[0], stringlengthmax, &trennzeichen[0], &parameter[0]);
	if (parameter[0].parameterinputvis ==true) 
		{
                    printf("read:	%s <= %4.3f ==> Main \n", uebergabestring, parameter[0].precthreshold);
                }

	strcpy(uebergabestring,"thawing_depth");
	if (((int) Parametereingabe(&uebergabestring[0], stringlengthmax, &trennzeichen[0], &parameter[0]))==1) 
        {
            parameter[0].thawing_depth=true;
        }
	else 
        {
            parameter[0].thawing_depth=false;
        };

	if (parameter[0].parameterinputvis ==true) 
			{
                printf("read:	%s <= %s ==> Main \n", uebergabestring, (parameter[0].thawing_depth)?"true":"false");
            }

	strcpy(uebergabestring,"vegetation");
	if (((int) Parametereingabe(&uebergabestring[0], stringlengthmax, &trennzeichen[0], &parameter[0]))==1) 
		{parameter[0].vegetation=true;}
	else {parameter[0].vegetation=false;};
	if (parameter[0].parameterinputvis ==true) 
			{
                printf("read:	%s <= %s ==> Main \n", uebergabestring, (parameter[0].vegetation)?"true":"false");
            }

	strcpy(uebergabestring,"omp_num_threads");
	parameter[0].omp_num_threads=(int) Parametereingabe(&uebergabestring[0], stringlengthmax, &trennzeichen[0], &parameter[0]);
	if (parameter[0].parameterinputvis ==true) 
			{
                printf("read:	%s <= %d ==> Main\n", uebergabestring, parameter[0].omp_num_threads);
            }

		
		
	// Modellparameter
	strcpy(uebergabestring,"weatherchoice");
	parameter[0].weatherchoice=(int) Parametereingabe(&uebergabestring[0], stringlengthmax, &trennzeichen[0], &parameter[0]);
	if (parameter[0].parameterinputvis ==true) 
			{
                printf("read:	%s <= %d ==> Main\n", uebergabestring, parameter[0].weatherchoice);
            }

	if (parameter[0].parameterinputvis ==true) 
			{
                printf("\n\n\t Modellparameter\n\n");
            }
	
	strcpy(uebergabestring,"simduration");
	parameter[0].simduration=(int) Parametereingabe(&uebergabestring[0], stringlengthmax, &trennzeichen[0], &parameter[0]);
	if (parameter[0].parameterinputvis ==true) 
			{
                printf("read:	%s <= %d ==> Main\n", uebergabestring, parameter[0].simduration);
            }

	strcpy(uebergabestring,"runs");
	parameter[0].runs=(int) Parametereingabe(&uebergabestring[0], stringlengthmax, &trennzeichen[0], &parameter[0]);
	if (parameter[0].parameterinputvis ==true) 
            {
                printf("read:	%s <= %d ==> Main\n", uebergabestring, parameter[0].runs);
            }

	strcpy(uebergabestring,"ivortmax");
	parameter[0].ivortmax=(unsigned int) Parametereingabe(&uebergabestring[0], stringlengthmax, &trennzeichen[0], &parameter[0]);
	if (parameter[0].parameterinputvis ==true) 
            {
                printf("read:	%s <= %d ==> Main\n", uebergabestring, parameter[0].ivortmax);
            }
	
	strcpy(uebergabestring,"stabilperiod");
	if (((int) Parametereingabe(&uebergabestring[0], stringlengthmax, &trennzeichen[0], &parameter[0]))==1) 
	{parameter[0].stabilperiod=true;}
	else {parameter[0].stabilperiod=false;};
	if (parameter[0].parameterinputvis ==true) 
            {
                printf("read:	%s <= %s ==> Main \n", uebergabestring, (parameter[0].stabilperiod)?"true":"false");
            }
	
	strcpy(uebergabestring,"stabilmovingwindow");
	parameter[0].stabilmovingwindow=Parametereingabe(&uebergabestring[0], stringlengthmax, &trennzeichen[0], &parameter[0]);
	if (parameter[0].parameterinputvis ==true) 
            {
                printf("read:	%s <= %4.3f ==> Main \n", uebergabestring, parameter[0].stabilmovingwindow);
            }

	strcpy(uebergabestring,"stabilpercentchangethreshold");
	parameter[0].stabilpercentchangethreshold=Parametereingabe(&uebergabestring[0], stringlengthmax, &trennzeichen[0], &parameter[0]);
	if (parameter[0].parameterinputvis ==true) 
            {
                printf("read:	%s <= %4.3f ==> Main \n", uebergabestring, parameter[0].stabilpercentchangethreshold);
            }
	
	strcpy(uebergabestring,"resetyear");
	parameter[0].resetyear=(int) Parametereingabe(&uebergabestring[0], stringlengthmax, &trennzeichen[0], &parameter[0]);
	if (parameter[0].parameterinputvis ==true) 
            {
                printf("read:	%s <= %d ==> Main\n", uebergabestring, parameter[0].resetyear);
            }	
	
	strcpy(uebergabestring,"specpres");
	parameter[0].specpres=(int) Parametereingabe(&uebergabestring[0], stringlengthmax, &trennzeichen[0], &parameter[0]);
	if (parameter[0].parameterinputvis ==true) 
            {
                printf("read:	%s <= %d ==> Main\n", uebergabestring, parameter[0].specpres);
            }	
	
	
	// For world lists
	strcpy(uebergabestring,"mapxlength");
	parameter[0].mapxlength=(int) Parametereingabe(&uebergabestring[0], stringlengthmax, &trennzeichen[0], &parameter[0]);
	if (parameter[0].parameterinputvis ==true) 
            {
                printf("read:	%s <= %d ==> Main\n", uebergabestring, parameter[0].mapxlength);
            }

	strcpy(uebergabestring,"mapylength");
	parameter[0].mapylength=(int) Parametereingabe(&uebergabestring[0], stringlengthmax, &trennzeichen[0], &parameter[0]);
	if (parameter[0].parameterinputvis ==true) 
		{
                    printf("read:	%s <= %d ==> Main\n", uebergabestring, parameter[0].mapylength);
                }
	
	// For weather
	strcpy(uebergabestring,"weathercalcgradient");
	if (((int) Parametereingabe(&uebergabestring[0], stringlengthmax, &trennzeichen[0], &parameter[0]))==1) 
		{parameter[0].weathercalcgradient=true;}
	else {parameter[0].weathercalcgradient=false;};
	if (parameter[0].parameterinputvis ==true) 
		{
                    printf("read:	%s <= %s ==> Main \n", uebergabestring, (parameter[0].weathercalcgradient)?"true":"false");
                }
	
	strcpy(uebergabestring,"lineartransect");
	if (((int) Parametereingabe(&uebergabestring[0], stringlengthmax, &trennzeichen[0], &parameter[0]))==1) 
        {
              parameter[0].lineartransect=true;
        }
	else 
        {
            parameter[0].lineartransect=false;
        };
	if (parameter[0].parameterinputvis ==true) 
		{
                    printf("read:	%s <= %s ==> Main \n", uebergabestring, (parameter[0].lineartransect)?"true":"false");
                }

	strcpy(uebergabestring,"nposmax");
	parameter[0].nposmax=Parametereingabe(&uebergabestring[0], stringlengthmax, &trennzeichen[0], &parameter[0]);
	if (parameter[0].parameterinputvis ==true) 
		{
                    printf("read:	%s <= %4.3f ==> Main \n", uebergabestring, parameter[0].nposmax);
                }

	strcpy(uebergabestring,"nposmin");
	parameter[0].nposmin=Parametereingabe(&uebergabestring[0], stringlengthmax, &trennzeichen[0], &parameter[0]);
	if (parameter[0].parameterinputvis ==true) 
		{
                    printf("read:	%s <= %4.3f ==> Main \n", uebergabestring, parameter[0].nposmin);
                }

	strcpy(uebergabestring,"sizemagnif");
	parameter[0].sizemagnif=(int) Parametereingabe(&uebergabestring[0], stringlengthmax, &trennzeichen[0], &parameter[0]);
	if (parameter[0].parameterinputvis ==true) 
		{
                    printf("read:	%s <= %d ==> Main\n", uebergabestring, parameter[0].sizemagnif);
                }
			
	// For initial conditions
	strcpy(uebergabestring,"starttrees");
	parameter[0].starttrees=(int) Parametereingabe(&uebergabestring[0], stringlengthmax, &trennzeichen[0], &parameter[0]);
	if (parameter[0].parameterinputvis ==true) 
		{
                    printf("read:	%s <= %d ==> Main\n", uebergabestring, parameter[0].starttrees);
                }
	
	// Parameters for LARIX GMELINII (Rupr.) Kuzen.
	// Reproduction
	if (parameter[0].parameterinputvis ==true) 
		{
                    printf("\n\n\t Parameter fuer LARIX GMELINII (RUPR.) KUZEN.\n\n\t Fortpflanzung\n");
                }
	strcpy(uebergabestring,"coneage");
	parameter[0].coneage=(int) Parametereingabe(&uebergabestring[0], stringlengthmax, &trennzeichen[0], &parameter[0]);
	if (parameter[0].parameterinputvis ==true) 
		{
                    printf("read:	%s <= %d ==> Main\n", uebergabestring, parameter[0].coneage);
                }

	strcpy(uebergabestring,"seedflightrate");
	parameter[0].seedflightrate=Parametereingabe(&uebergabestring[0], stringlengthmax, &trennzeichen[0], &parameter[0]);
	if (parameter[0].parameterinputvis ==true) 
		{
                    printf("read:	%s <= %4.3f ==> Main \n", uebergabestring, parameter[0].seedflightrate);
                }

	strcpy(uebergabestring,"dispersalmode");
	parameter[0].dispersalmode=(int) Parametereingabe(&uebergabestring[0], stringlengthmax, &trennzeichen[0], &parameter[0]);
	if (parameter[0].parameterinputvis ==true) 
		{
                    printf("read:	%s <= %d ==> Main\n", uebergabestring, parameter[0].dispersalmode);
                }
		
	strcpy(uebergabestring,"seedtravelbreezeg");
	parameter[0].seedtravelbreezeg=Parametereingabe(&uebergabestring[0], stringlengthmax, &trennzeichen[0], &parameter[0]);
	if (parameter[0].parameterinputvis ==true) 
		{
                    printf("read:	%s <= %4.1f ==> Main\n", uebergabestring, parameter[0].seedtravelbreezeg);
                }	

	strcpy(uebergabestring,"seedtravelbreezes");
	parameter[0].seedtravelbreezes=Parametereingabe(&uebergabestring[0], stringlengthmax, &trennzeichen[0], &parameter[0]);
	if (parameter[0].parameterinputvis ==true) 
		{
                    printf("read:	%s <= %4.1f ==> Main\n", uebergabestring, parameter[0].seedtravelbreezes);
                }	

	strcpy(uebergabestring,"seeddescentg");
	parameter[0].seeddescentg=Parametereingabe(&uebergabestring[0], stringlengthmax, &trennzeichen[0], &parameter[0]);
	if (parameter[0].parameterinputvis ==true) 
		{
                    printf("read:	%s <= %4.2f ==> Main\n", uebergabestring, parameter[0].seeddescentg);
                }	

	strcpy(uebergabestring,"seeddescents");
	parameter[0].seeddescents=Parametereingabe(&uebergabestring[0], stringlengthmax, &trennzeichen[0], &parameter[0]);
	if (parameter[0].parameterinputvis ==true) 
		{
                    printf("read:	%s <= %4.2f ==> Main\n", uebergabestring, parameter[0].seeddescents);
                }	
		
	strcpy(uebergabestring,"distanceratio");
	parameter[0].distanceratio=Parametereingabe(&uebergabestring[0], stringlengthmax, &trennzeichen[0], &parameter[0]);
	if (parameter[0].parameterinputvis ==true) 
		{
                    printf("read:	%s <= %4.3f ==> Main \n", uebergabestring, parameter[0].mortbg);
                }

	strcpy(uebergabestring,"seedprodfactor");
	parameter[0].seedprodfactor=Parametereingabe(&uebergabestring[0], stringlengthmax, &trennzeichen[0], &parameter[0]);
	if (parameter[0].parameterinputvis ==true) 
		{
                    printf("read:	%s <= %4.3f ==> Main\n", uebergabestring, parameter[0].seedprodfactor);
                }

	strcpy(uebergabestring,"germinationrate");
	parameter[0].germinationrate=Parametereingabe(&uebergabestring[0], stringlengthmax, &trennzeichen[0], &parameter[0]);
	if (parameter[0].parameterinputvis ==true) 
		{
                    printf("read:	%s <= %4.4f ==> Main \n", uebergabestring, parameter[0].germinationrate);
                }

	strcpy(uebergabestring,"germinatioweatherinfluence");
	parameter[0].germinatioweatherinfluence=Parametereingabe(&uebergabestring[0], stringlengthmax, &trennzeichen[0], &parameter[0]);
	if (parameter[0].parameterinputvis ==true) 
		{
                    printf("read:	%s <= %4.3f ==> Main \n", uebergabestring, parameter[0].germinatioweatherinfluence);
                }

	strcpy(uebergabestring,"realseedconnect");
	if (((int) Parametereingabe(&uebergabestring[0], stringlengthmax, &trennzeichen[0], &parameter[0]))==1) 
        {
            parameter[0].realseedconnect=true;
        }
	else 
        {
            parameter[0].realseedconnect=false;
        };
	
        if (parameter[0].parameterinputvis ==true) 
		{
                    printf("read:	%s <= %s ==> Main \n", uebergabestring, (parameter[0].realseedconnect)?"true":"false");
                }

	strcpy(uebergabestring,"seeddispmode");
	parameter[0].seeddispmode=(int) (Parametereingabe(&uebergabestring[0], stringlengthmax, &trennzeichen[0], &parameter[0]));
	if (parameter[0].parameterinputvis ==true) 
		{
                    printf("read:	%s <= %d ==> Main\n", uebergabestring, parameter[0].seeddispmode);
                }

	strcpy(uebergabestring,"seedintro");
	if (((int) Parametereingabe(&uebergabestring[0], stringlengthmax, &trennzeichen[0], &parameter[0]))==1) 
        {
            parameter[0].seedintro=true;
        }
	else 
        {
            parameter[0].seedintro=false;
        };
	if (parameter[0].parameterinputvis ==true) 
		{
                    printf("read:	%s <= %s ==> Main \n", uebergabestring, (parameter[0].seedintro)?"true":"false");
                }

	strcpy(uebergabestring,"seedintropermanent");
	if (((int) Parametereingabe(&uebergabestring[0], stringlengthmax, &trennzeichen[0], &parameter[0]))==1) 
        {
            parameter[0].seedintropermanent=true;
        }
	else 
        {
            parameter[0].seedintropermanent=false;
        };
                if (parameter[0].parameterinputvis ==true) 
		{
                    printf("read:	%s <= %s ==> Main \n", uebergabestring, (parameter[0].seedintropermanent)?"true":"false");
                }
		
	strcpy(uebergabestring,"seedintronumber");
	parameter[0].seedintronumber=(int) (Parametereingabe(&uebergabestring[0], stringlengthmax, &trennzeichen[0], &parameter[0]));
                if (parameter[0].parameterinputvis ==true) 
		{
                    printf("read:	%s <= %d ==> Main\n", uebergabestring, parameter[0].seedintronumber);
                }

	strcpy(uebergabestring,"seedintronumberpermanent");
	parameter[0].seedintronumberpermanent=(int) (Parametereingabe(&uebergabestring[0], stringlengthmax, &trennzeichen[0], &parameter[0]));
                if (parameter[0].parameterinputvis ==true) 
		{
                    printf("read:	%s <= %d ==> Main\n", uebergabestring, parameter[0].seedintronumberpermanent);
                }
	
	strcpy(uebergabestring,"yearswithseedintro");
	parameter[0].yearswithseedintro=(int) (Parametereingabe(&uebergabestring[0], stringlengthmax, &trennzeichen[0], &parameter[0]));
                if (parameter[0].parameterinputvis ==true) 
		{
                    printf("read:	%s <= %d ==> Main\n", uebergabestring, parameter[0].yearswithseedintro);
                }

	strcpy(uebergabestring,"seedtravelbetween");
        if (((int) Parametereingabe(&uebergabestring[0], stringlengthmax, &trennzeichen[0], &parameter[0]))==1) 
		{
                    parameter[0].seedtravelbetween=true;
                }
	else 
        {
            parameter[0].seedtravelbetween=false;
        };
                if (parameter[0].parameterinputvis ==true) 
		{
                    printf("read:	%s <= %s ==> Main \n", uebergabestring, (parameter[0].seedtravelbetween)?"true":"false");
                }

		// Growth
                if (parameter[0].parameterinputvis ==true) 
		{
                printf("\n\n\t Wachstum\n\n");
        }

	strcpy(uebergabestring,"gdbasalfacqgmel");
	parameter[0].gdbasalfacqgmel= Parametereingabe(&uebergabestring[0], stringlengthmax, &trennzeichen[0], &parameter[0]);
                if (parameter[0].parameterinputvis ==true) 
		{
                    printf("read:	%s <= %4.3f ==> Main \n", uebergabestring, parameter[0].gdbasalfacqgmel);
                }
		
	strcpy(uebergabestring,"gdbasalfacgmel");
	parameter[0].gdbasalfacgmel= Parametereingabe(&uebergabestring[0], stringlengthmax, &trennzeichen[0], &parameter[0]);
                if (parameter[0].parameterinputvis ==true) 
		{
                    printf("read:	%s <= %4.3f ==> Main \n", uebergabestring, parameter[0].gdbasalfacgmel);
                }

	strcpy(uebergabestring,"gdbasalconstgmel");
	parameter[0].gdbasalconstgmel= Parametereingabe(&uebergabestring[0], stringlengthmax, &trennzeichen[0], &parameter[0]);
                if (parameter[0].parameterinputvis ==true) 
		{
                    printf("read:	%s <= %4.3f ==> Main \n", uebergabestring, parameter[0].gdbasalconstgmel);
                }

	strcpy(uebergabestring,"gdbrustfacqgmel");
	parameter[0].gdbrustfacqgmel= Parametereingabe(&uebergabestring[0], stringlengthmax, &trennzeichen[0], &parameter[0]);
                if (parameter[0].parameterinputvis ==true) 
		{
                    printf("read:	%s <= %4.4f ==> Main \n", uebergabestring, parameter[0].gdbrustfacqgmel);
                }		
		
	strcpy(uebergabestring,"gdbrustfacgmel");
	parameter[0].gdbrustfacgmel= Parametereingabe(&uebergabestring[0], stringlengthmax, &trennzeichen[0], &parameter[0]);
                if (parameter[0].parameterinputvis ==true) 
		{
                    printf("read:	%s <= %4.3f ==> Main \n", uebergabestring, parameter[0].gdbrustfacgmel);
                }

	strcpy(uebergabestring,"gdbrustconstgmel");
	parameter[0].gdbrustconstgmel= Parametereingabe(&uebergabestring[0], stringlengthmax, &trennzeichen[0], &parameter[0]);
                if (parameter[0].parameterinputvis ==true) 
		{
                    printf("read:	%s <= %4.3f ==> Main \n", uebergabestring, parameter[0].gdbrustconstgmel);
                }		
		
	strcpy(uebergabestring,"gdbasalfacqsib");
	parameter[0].gdbasalfacqsib= Parametereingabe(&uebergabestring[0], stringlengthmax, &trennzeichen[0], &parameter[0]);
                if (parameter[0].parameterinputvis ==true) 
		{
                    printf("read:	%s <= %4.3f ==> Main \n", uebergabestring, parameter[0].gdbasalfacqsib);
                }		
		
	strcpy(uebergabestring,"gdbasalfacsib");
	parameter[0].gdbasalfacsib= Parametereingabe(&uebergabestring[0], stringlengthmax, &trennzeichen[0], &parameter[0]);
                if (parameter[0].parameterinputvis ==true) 
		{
                    printf("read:	%s <= %4.3f ==> Main \n", uebergabestring, parameter[0].gdbasalfacsib);  
                }

	strcpy(uebergabestring,"gdbasalconstsib");
	parameter[0].gdbasalconstsib= Parametereingabe(&uebergabestring[0], stringlengthmax, &trennzeichen[0], &parameter[0]);
                if (parameter[0].parameterinputvis ==true) 
		{
                    printf("read:	%s <= %4.3f ==> Main \n", uebergabestring, parameter[0].gdbasalconstsib);
                }		

	strcpy(uebergabestring,"gdbrustfacqsib");
	parameter[0].gdbrustfacqsib= Parametereingabe(&uebergabestring[0], stringlengthmax, &trennzeichen[0], &parameter[0]);
                if (parameter[0].parameterinputvis ==true) 
		{
                    printf("read:	%s <= %4.3f ==> Main \n", uebergabestring, parameter[0].gdbrustfacqsib);
                }		
		
	strcpy(uebergabestring,"gdbrustfacsib");
	parameter[0].gdbrustfacsib= Parametereingabe(&uebergabestring[0], stringlengthmax, &trennzeichen[0], &parameter[0]);
                if (parameter[0].parameterinputvis ==true) 
		{
                    printf("read:	%s <= %4.3f ==> Main \n", uebergabestring, parameter[0].gdbrustfacsib);
                }

	strcpy(uebergabestring,"gdbrustconstsib");
	parameter[0].gdbrustconstsib= Parametereingabe(&uebergabestring[0], stringlengthmax, &trennzeichen[0], &parameter[0]);
                if (parameter[0].parameterinputvis ==true) 
		{
                    printf("read:	%s <= %4.3f ==> Main \n", uebergabestring, parameter[0].gdbrustconstsib);
                }	
		
	strcpy(uebergabestring,"basaleinflussaltneu");
	parameter[0].basaleinflussaltneu=Parametereingabe(&uebergabestring[0], stringlengthmax, &trennzeichen[0], &parameter[0]);
                if (parameter[0].parameterinputvis ==true) 
		{
                    printf("read:	%s <= %4.4f ==> Main \n", uebergabestring, parameter[0].basaleinflussaltneu);
                }

	strcpy(uebergabestring,"relwachstumeinfluss");
	parameter[0].relwachstumeinfluss=(int) (Parametereingabe(&uebergabestring[0], stringlengthmax, &trennzeichen[0], &parameter[0]));
                if (parameter[0].parameterinputvis ==true) 
		{
                    printf("read:	%s <= %d ==> Main\n", uebergabestring, parameter[0].relwachstumeinfluss);
                }

	strcpy(uebergabestring,"densitymode");
	parameter[0].densitymode=(int) (Parametereingabe(&uebergabestring[0], stringlengthmax, &trennzeichen[0], &parameter[0]));
                if (parameter[0].parameterinputvis ==true) 
		{
                    printf("read:	%s <= %d ==> Main\n", uebergabestring, parameter[0].densitymode);
                }

	strcpy(uebergabestring,"incfac");
	parameter[0].incfac=Parametereingabe(&uebergabestring[0], stringlengthmax, &trennzeichen[0], &parameter[0]);
                if (parameter[0].parameterinputvis ==true) 
		{
                    printf("read:	%s <= %4.3f ==> Main \n", uebergabestring, parameter[0].incfac);
                }

	strcpy(uebergabestring,"densityvaluemanipulatorexp");
	parameter[0].densityvaluemanipulatorexp=Parametereingabe(&uebergabestring[0], stringlengthmax, &trennzeichen[0], &parameter[0]);
                if (parameter[0].parameterinputvis ==true) 
		{
                    printf("read:	%s <= %4.3f ==> Main \n", uebergabestring, parameter[0].densityvaluemanipulatorexp);
                }
		
	strcpy(uebergabestring,"calcinfarea");
	parameter[0].calcinfarea=(int) (Parametereingabe(&uebergabestring[0], stringlengthmax, &trennzeichen[0], &parameter[0]));
                if (parameter[0].parameterinputvis ==true) 
		{
                    printf("read:	%s <= %d ==> Main\n", uebergabestring, parameter[0].calcinfarea);
                }

	strcpy(uebergabestring,"densitysmallweighing");
	parameter[0].densitysmallweighing=Parametereingabe(&uebergabestring[0], stringlengthmax, &trennzeichen[0], &parameter[0]);
                if (parameter[0].parameterinputvis ==true) 
		{
                    printf("read:	%s <= %4.3f ==> Main \n", uebergabestring, parameter[0].densitysmallweighing);
                }

        strcpy(uebergabestring,"densitytreetile");
	parameter[0].densitytreetile=Parametereingabe(&uebergabestring[0], stringlengthmax, &trennzeichen[0], &parameter[0]);
                if (parameter[0].parameterinputvis ==true) 
		{
                    printf("read:	%s <= %4.3f ==> Main \n", uebergabestring, parameter[0].densitytreetile);
                }

	strcpy(uebergabestring,"densitytiletree");
	parameter[0].densitytiletree=(int) (Parametereingabe(&uebergabestring[0], stringlengthmax, &trennzeichen[0], &parameter[0]));
                if (parameter[0].parameterinputvis ==true) 
		{
                    printf("read:	%s <= %d ==> Main\n", uebergabestring, parameter[0].densitytiletree);
                }

        strcpy(uebergabestring,"desitymaxreduction");
	parameter[0].desitymaxreduction=Parametereingabe(&uebergabestring[0], stringlengthmax, &trennzeichen[0], &parameter[0]);
                if (parameter[0].parameterinputvis ==true) 
		{
                    printf("read:	%s <= %4.3f ==> Main \n", uebergabestring, parameter[0].desitymaxreduction);
                }

        strcpy(uebergabestring,"dichtheightrel");
	parameter[0].dichtheightrel=(int) (Parametereingabe(&uebergabestring[0], stringlengthmax, &trennzeichen[0], &parameter[0]));
                if (parameter[0].parameterinputvis ==true) 
		{
                    printf("read:	%s <= %d ==> Main\n", uebergabestring, parameter[0].dichtheightrel);
                }

        strcpy(uebergabestring,"densityvaluedbasalinfluence");
	parameter[0].densityvaluedbasalinfluence=Parametereingabe(&uebergabestring[0], stringlengthmax, &trennzeichen[0], &parameter[0]);
                if (parameter[0].parameterinputvis ==true) 
		{
                    printf("read:	%s <= %4.3f ==> Main \n", uebergabestring, parameter[0].densityvaluedbasalinfluence);
                }

	strcpy(uebergabestring,"densitywertmaximumbeiheight");
	parameter[0].densitywertmaximumbeiheight=Parametereingabe(&uebergabestring[0], stringlengthmax, &trennzeichen[0], &parameter[0]);
                if (parameter[0].parameterinputvis ==true) 
		{
                    printf("read:	%s <= %4.3f ==> Main \n", uebergabestring, parameter[0].densitywertmaximumbeiheight);
                }

			// Beziehungen zwischen height und Basaldurchmesser
	strcpy(uebergabestring,"allometryfunctiontype");
	parameter[0].allometryfunctiontype=(int) Parametereingabe(&uebergabestring[0], stringlengthmax, &trennzeichen[0], &parameter[0]);
                if (parameter[0].parameterinputvis ==true) 
		{
                    printf("read:	%s <= %d ==> Main\n", uebergabestring, parameter[0].allometryfunctiontype);
                }

	strcpy(uebergabestring,"dbasalheightalloslope");
	parameter[0].dbasalheightalloslope=Parametereingabe(&uebergabestring[0], stringlengthmax, &trennzeichen[0], &parameter[0]);
                if (parameter[0].parameterinputvis ==true) 
		{
                    printf("read:	%s <= %4.4f ==> Main \n", uebergabestring, parameter[0].dbasalheightalloslope);
                }
	
	strcpy(uebergabestring,"dbasalheightalloexp");
	parameter[0].dbasalheightalloexp=Parametereingabe(&uebergabestring[0], stringlengthmax, &trennzeichen[0], &parameter[0]);
                if (parameter[0].parameterinputvis ==true) 
		{
                    printf("read:	%s <= %4.4f ==> Main \n", uebergabestring, parameter[0].dbasalheightalloexp);
                }

	strcpy(uebergabestring,"dbrustheightalloslope");
	parameter[0].dbrustheightalloslope=Parametereingabe(&uebergabestring[0], stringlengthmax, &trennzeichen[0], &parameter[0]);
                if (parameter[0].parameterinputvis ==true) 
		{
                    printf("read:	%s <= %4.4f ==> Main \n", uebergabestring, parameter[0].dbrustheightalloslope);
                }

	strcpy(uebergabestring,"dbrustheightalloexp");
	parameter[0].dbrustheightalloexp=Parametereingabe(&uebergabestring[0], stringlengthmax, &trennzeichen[0], &parameter[0]);
                if (parameter[0].parameterinputvis ==true) 
		{
                    printf("read:	%s <= %4.4f ==> Main \n", uebergabestring, parameter[0].dbrustheightalloexp);
                }

	strcpy(uebergabestring,"dbasalheightslopenonlin");
	parameter[0].dbasalheightslopenonlin=Parametereingabe(&uebergabestring[0], stringlengthmax, &trennzeichen[0], &parameter[0]);
                if (parameter[0].parameterinputvis ==true) 
		{
                    printf("read:	%s <= %4.4f ==> Main \n", uebergabestring, parameter[0].dbasalheightslopenonlin);
                }
	
	strcpy(uebergabestring,"dbrustheightslopenonlin");
	parameter[0].dbrustheightslopenonlin=Parametereingabe(&uebergabestring[0], stringlengthmax, &trennzeichen[0], &parameter[0]);
                if (parameter[0].parameterinputvis ==true) 
		{
                    printf("read:	%s <= %4.4f ==> Main \n", uebergabestring, parameter[0].dbrustheightslopenonlin);
                }

		// Mortalitaet
                if (parameter[0].parameterinputvis ==true) 
		{
                    printf("\n\n\t Mortalitaet\n\n");
                }
	strcpy(uebergabestring,"mortbg");
	parameter[0].mortbg=Parametereingabe(&uebergabestring[0], stringlengthmax, &trennzeichen[0], &parameter[0]);
                if (parameter[0].parameterinputvis ==true) 
		{
                    printf("read:	%s <= %4.3f ==> Main \n", uebergabestring, parameter[0].mortbg);
                }

	strcpy(uebergabestring,"maximumage");
	parameter[0].maximumage=(int) Parametereingabe(&uebergabestring[0], stringlengthmax, &trennzeichen[0], &parameter[0]);
                if (parameter[0].parameterinputvis ==true) 
		{
                    printf("read:	%s <= %d ==> Main\n", uebergabestring, parameter[0].maximumage);
                }

	strcpy(uebergabestring,"mortage");
	parameter[0].mortage=Parametereingabe(&uebergabestring[0], stringlengthmax, &trennzeichen[0], &parameter[0]);
                if (parameter[0].parameterinputvis ==true) 
		{
                    printf("read:	%s <= %4.4f ==> Main \n", uebergabestring, parameter[0].mortage);
                }

	strcpy(uebergabestring,"mortyouth");
	parameter[0].mortyouth=Parametereingabe(&uebergabestring[0], stringlengthmax, &trennzeichen[0], &parameter[0]);
                if (parameter[0].parameterinputvis ==true) 
		{
                    printf("read:	%s <= %4.4f ==> Main \n", uebergabestring, parameter[0].mortyouth);
                }

	strcpy(uebergabestring,"mortyouthinfluenceexp");
	parameter[0].mortyouthinfluenceexp=Parametereingabe(&uebergabestring[0], stringlengthmax, &trennzeichen[0], &parameter[0]);
                if (parameter[0].parameterinputvis ==true) 
		{
                    printf("read:	%s <= %4.4f ==> Main \n", uebergabestring, parameter[0].mortyouthinfluenceexp);
                }

	strcpy(uebergabestring,"mgrowth");
	parameter[0].mgrowth=Parametereingabe(&uebergabestring[0], stringlengthmax, &trennzeichen[0], &parameter[0]);
                if (parameter[0].parameterinputvis ==true) 
		{
                    printf("read:	%s <= %4.4f ==> Main \n", uebergabestring, parameter[0].mgrowth);
                }

	strcpy(uebergabestring,"relwachstummorteinflussexp");
	parameter[0].relwachstummorteinflussexp=Parametereingabe(&uebergabestring[0], stringlengthmax, &trennzeichen[0], &parameter[0]);
                if (parameter[0].parameterinputvis ==true) 
		{
                    printf("read:	%s <= %4.4f ==> Main \n", uebergabestring, parameter[0].relwachstummorteinflussexp);
                }

	strcpy(uebergabestring,"mweather");
	parameter[0].mweather=Parametereingabe(&uebergabestring[0], stringlengthmax, &trennzeichen[0], &parameter[0]);
                if (parameter[0].parameterinputvis ==true) 
		{
                    printf("read:	%s <= %4.4f ==> Main \n", uebergabestring, parameter[0].mweather);
                }

	strcpy(uebergabestring,"heightweathermorteinflussexp");
	parameter[0].heightweathermorteinflussexp=Parametereingabe(&uebergabestring[0], stringlengthmax, &trennzeichen[0], &parameter[0]);
                if (parameter[0].parameterinputvis ==true) 
		{
                    printf("read:	%s <= %4.4f ==> Main \n", uebergabestring, parameter[0].heightweathermorteinflussexp);
                }

	strcpy(uebergabestring,"mdensity");
	parameter[0].mdensity=Parametereingabe(&uebergabestring[0], stringlengthmax, &trennzeichen[0], &parameter[0]);
                if (parameter[0].parameterinputvis ==true) 
		{
                    printf("read:	%s <= %4.4f ==> Main \n", uebergabestring, parameter[0].mdensity);
                }

	strcpy(uebergabestring,"mtrockenheit");
	parameter[0].mtrockenheit=Parametereingabe(&uebergabestring[0], stringlengthmax, &trennzeichen[0], &parameter[0]);
                if (parameter[0].parameterinputvis ==true) 
		{
                    printf("read:	%s <= %4.4f ==> Main \n", uebergabestring, parameter[0].mtrockenheit);
                }

	strcpy(uebergabestring,"seedTreemort");
	parameter[0].seedTreemort=Parametereingabe(&uebergabestring[0], stringlengthmax, &trennzeichen[0], &parameter[0]);
                if (parameter[0].parameterinputvis ==true) 
		{
                    printf("read:	%s <= %4.4f ==> Main \n", uebergabestring, parameter[0].seedTreemort);
                }

	strcpy(uebergabestring,"seedbodenmort");
	parameter[0].seedbodenmort=Parametereingabe(&uebergabestring[0], stringlengthmax, &trennzeichen[0], &parameter[0]);
                if (parameter[0].parameterinputvis ==true) 
		{
                    printf("read:	%s <= %4.4f ==> Main \n", uebergabestring, parameter[0].seedbodenmort);
                }

	strcpy(uebergabestring,"gmelseedmaxage");
	parameter[0].gmelseedmaxage=(int) Parametereingabe(&uebergabestring[0], stringlengthmax, &trennzeichen[0], &parameter[0]);
                if (parameter[0].parameterinputvis ==true) 
                {
                    printf("read:	%s <= %d ==> Main\n", uebergabestring, parameter[0].gmelseedmaxage);
                }

	strcpy(uebergabestring,"janthresholdtempgmel");
	parameter[0].janthresholdtempgmel= Parametereingabe(&uebergabestring[0], stringlengthmax, &trennzeichen[0], &parameter[0]);
                if (parameter[0].parameterinputvis ==true)
                {
                    printf("read:	%s <= %4.2f ==> Main\n", uebergabestring, parameter[0].janthresholdtempgmel);
                }

	strcpy(uebergabestring,"weathervariablegmela");
	parameter[0].weathervariablegmela= Parametereingabe(&uebergabestring[0], stringlengthmax, &trennzeichen[0], &parameter[0]);
                if (parameter[0].parameterinputvis ==true)
                {
                    printf("read:	%s <= %4.3f ==> Main\n", uebergabestring, parameter[0].weathervariablegmela);
                }

	strcpy(uebergabestring,"weathervariablegmelb");
	parameter[0].weathervariablegmelb= Parametereingabe(&uebergabestring[0], stringlengthmax, &trennzeichen[0], &parameter[0]);
                if (parameter[0].parameterinputvis ==true)
                {
                    printf("read:	%s <= %4.3f ==> Main\n", uebergabestring, parameter[0].weathervariablegmelb);
                }

	strcpy(uebergabestring,"weathervariablegmelc");
	parameter[0].weathervariablegmelc= Parametereingabe(&uebergabestring[0], stringlengthmax, &trennzeichen[0], &parameter[0]);
                if (parameter[0].parameterinputvis ==true)
                {
                    printf("read:	%s <= %4.3f ==> Main\n", uebergabestring, parameter[0].weathervariablegmelc);
                }

	strcpy(uebergabestring,"windsource");
	parameter[0].windsource=(int) Parametereingabe(&uebergabestring[0],stringlengthmax, &trennzeichen[0], &parameter[0]);
                if (parameter[0].parameterinputvis ==true)
                {
                    printf("read:	%s <= %d ==> Main\n", uebergabestring, parameter[0].windsource);
                }
	
	strcpy(uebergabestring,"boundaryconditions");
	parameter[0].boundaryconditions=(int) Parametereingabe(&uebergabestring[0],stringlengthmax, &trennzeichen[0], &parameter[0]);
                if (parameter[0].parameterinputvis ==true)
                {
                    printf("read:	%s <= %d ==> Main\n", uebergabestring, parameter[0].boundaryconditions);
                }
	
	strcpy(uebergabestring,"outputall");
	parameter[0].outputall=(int) Parametereingabe(&uebergabestring[0],stringlengthmax, &trennzeichen[0], &parameter[0]);
                if (parameter[0].parameterinputvis ==true)
                {
                    printf("read:	%s <= %d ==> Main\n", uebergabestring, parameter[0].outputall);
                }
	
	
	
	strcpy(uebergabestring,"pollination");
	parameter[0].pollination=(int) Parametereingabe(&uebergabestring[0],stringlengthmax, &trennzeichen[0], &parameter[0]);
                if (parameter[0].parameterinputvis ==true)
                {
                    printf("read:	%s <= %d ==> Main\n", uebergabestring, parameter[0].pollination);
                }
	
			
	strcpy(uebergabestring,"pollendirectionvariance");
	parameter[0].pollendirectionvariance=(double) Parametereingabe(&uebergabestring[0],stringlengthmax, &trennzeichen[0], &parameter[0]);
                if (parameter[0].parameterinputvis ==true)
                {
                    printf("read:	%s <= %4.3f ==> Main\n", uebergabestring, parameter[0].pollendirectionvariance);
                }


	strcpy(uebergabestring,"pollenfall");
	parameter[0].pollenfall=(double) Parametereingabe(&uebergabestring[0],stringlengthmax, &trennzeichen[0], &parameter[0]);
	
	strcpy(uebergabestring,"pollengregoryc");
	parameter[0].pollengregoryc=(double) Parametereingabe(&uebergabestring[0],stringlengthmax, &trennzeichen[0], &parameter[0]);
	
	strcpy(uebergabestring,"pollengregorym");
	parameter[0].pollengregorym=(double) Parametereingabe(&uebergabestring[0],stringlengthmax, &trennzeichen[0], &parameter[0]);
	
	strcpy(uebergabestring, "computationtimevis");
	parameter[0].computationtimevis=(bool) Parametereingabe(&uebergabestring[0],stringlengthmax, &trennzeichen[0], &parameter[0]);
	
        if (parameter[0].parameterinputvis ==true) 
		{
            printf("\n<----\t PARAMETERINPUT\n");
        };
		
	
	// further variables
		parameter[0].tempdiffort=0.0;
		parameter[0].precdiffort=0.0;
}
