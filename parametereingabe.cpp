/****************************************************************************************//**
 * \brief Search for parameter names in the parameter file and return the value
 *
 *
 *
 *
 *******************************************************************************************/
double Parametereingabe(char *uebergabestring, int wortlaengemax, char *trennzeichen, struct Parameter *parameter)
{
	// open file with parameter values
	FILE *f;
	f = fopen("parameter.txt","r");
	if (f == NULL)
	{
		printf("LAVESI exited - parameter file parameter.txt not found!\n");
		exit(1);
	}

	// declaration of local variables
	char searchstring[255];
	strcpy(searchstring , uebergabestring);
	char puffer[255], puffer_bak[255], puffer_zahl[255];
	int counter=1;
	char *wort;
	char *zahl;

	// iterate line-wise throuch the file and extract the parameter value
	while( fgets(puffer, wortlaengemax, f) != NULL)
	{
		strcpy(puffer_bak, puffer);
		wort = strtok(puffer, trennzeichen);
		while (wort != NULL)
		{
			if ( strcmp(wort, searchstring) == 0)
			{
				strcpy(puffer_zahl, puffer_bak);
			}
			wort = strtok(NULL, trennzeichen);
		}
		counter++;
	}

	// converse the value to a double
	strtok(puffer_zahl, trennzeichen);
	zahl= strtok(NULL, trennzeichen);
	double zahldouble=strtod(zahl, NULL);
	if (parameter[0].parameterlesenanzeige==true) 
	{
		printf("Searching:\t%s <= %4.5f \n", searchstring, zahldouble);
	}

	fclose(f);
	wort=NULL;
	zahl=NULL;

	return zahldouble;
}

/****************************************************************************************//**
 * \brief Determine the parameter names to import
 *
 *
 *
 *
 *******************************************************************************************/
void Parametereinlesen(void)
{
	char uebergabestring[wortlaengemax];

	// logical switches for functions and console output
	strcpy(uebergabestring,"parameterlesenanzeige");
	if (((int) Parametereingabe(&uebergabestring[0], wortlaengemax, &trennzeichen[0], &parameter[0]))==1) 
		{parameter[0].parameterlesenanzeige=true;}
	else {parameter[0].parameterlesenanzeige=false;};
	if (parameter[0].parameterlesenanzeige ==true) 
		{printf("\n---->\t Show parameter importn\n\nImport:	%s <= %s ==> Main \n", uebergabestring,		(parameter[0].parameterlesenanzeige)?"true":"false");}

	strcpy(uebergabestring,"jahranzeige");
	if (((int) Parametereingabe(&uebergabestring[0], wortlaengemax, &trennzeichen[0], &parameter[0]))==1) 
		{parameter[0].jahranzeige=true;}
	else {parameter[0].jahranzeige=false;};
	if (parameter[0].parameterlesenanzeige ==true) 
		{printf("Import:	%s <= %s ==> Main \n", uebergabestring, (parameter[0].jahranzeige)?"true":"false");}

	strcpy(uebergabestring,"qualijahranzeige");
	if (((int) Parametereingabe(&uebergabestring[0], wortlaengemax, &trennzeichen[0], &parameter[0]))==1) 
		{parameter[0].qualijahranzeige=true;}
	else {parameter[0].qualijahranzeige=false;};
	if (parameter[0].parameterlesenanzeige ==true) 
		{printf("Import:	%s <= %s ==> Main \n", uebergabestring, (parameter[0].qualijahranzeige)?"true":"false");}
	
	strcpy(uebergabestring,"datenausgabe");
	if (((int) Parametereingabe(&uebergabestring[0], wortlaengemax, &trennzeichen[0], &parameter[0]))==1) {parameter[0].datenausgabe=true;}
	else {parameter[0].datenausgabe=false;};
	if (parameter[0].parameterlesenanzeige ==true) {printf("Import:	%s <= %s ==> Main \n", uebergabestring, (parameter[0].datenausgabe)?"true":"false");}

	strcpy(uebergabestring,"ausgabemodus");
	parameter[0].ausgabemodus=(int) (Parametereingabe(&uebergabestring[0], wortlaengemax, &trennzeichen[0], &parameter[0]));
	if (parameter[0].parameterlesenanzeige ==true) 
		{printf("Import:	%s <= %d ==> Main\n", uebergabestring, parameter[0].ausgabemodus);}

	strcpy(uebergabestring,"samenausbranzeige");
	if (((int) Parametereingabe(&uebergabestring[0], wortlaengemax, &trennzeichen[0], &parameter[0]))==1) 
		{parameter[0].samenausbranzeige=true;}
	else {parameter[0].samenausbranzeige=false;};

	if (parameter[0].parameterlesenanzeige ==true) 
		{printf("Import:	%s <= %s ==> Main \n", uebergabestring, (parameter[0].samenausbranzeige)?"true":"false");}

	strcpy(uebergabestring,"mortanzeige");
	if (((int) Parametereingabe(&uebergabestring[0], wortlaengemax, &trennzeichen[0], &parameter[0]))==1) 
		{parameter[0].mortanzeige=true;}
	else {parameter[0].mortanzeige=false;};
	
	if (parameter[0].parameterlesenanzeige ==true) 
		{printf("Import:	%s <= %s ==> Main \n", uebergabestring, (parameter[0].mortanzeige)?"true":"false");}

	// general parameters for climate import and processing
	strcpy(uebergabestring,"evapod");
	parameter[0].evapod=Parametereingabe(&uebergabestring[0], wortlaengemax, &trennzeichen[0], &parameter[0]);
	if (parameter[0].parameterlesenanzeige ==true) 
		{printf("Import:	%s <= %4.3f ==> Main \n", uebergabestring, parameter[0].evapod);}

	strcpy(uebergabestring,"precwetter");
	parameter[0].precwetter=(int) (Parametereingabe(&uebergabestring[0], wortlaengemax, &trennzeichen[0], &parameter[0]));
	if (parameter[0].parameterlesenanzeige ==true) 
		{printf("Import:	%s <= %d ==> Main\n", uebergabestring, parameter[0].precwetter);}

	strcpy(uebergabestring,"precgrenzwert");
	parameter[0].precgrenzwert=Parametereingabe(&uebergabestring[0], wortlaengemax, &trennzeichen[0], &parameter[0]);
	if (parameter[0].parameterlesenanzeige ==true) 
		{printf("Import:	%s <= %4.3f ==> Main \n", uebergabestring, parameter[0].precgrenzwert);}

	strcpy(uebergabestring,"precindexstrength");
	parameter[0].precindexstrength=Parametereingabe(&uebergabestring[0], wortlaengemax, &trennzeichen[0], &parameter[0]);
	if (parameter[0].parameterlesenanzeige ==true) 
		{printf("Import:	%s <= %4.3f ==> Main \n", uebergabestring, parameter[0].precindexstrength);}

	strcpy(uebergabestring,"precindexexponent");
	parameter[0].precindexexponent=Parametereingabe(&uebergabestring[0], wortlaengemax, &trennzeichen[0], &parameter[0]);
	if (parameter[0].parameterlesenanzeige ==true) 
		{printf("Import:	%s <= %4.3f ==> Main \n", uebergabestring, parameter[0].precindexexponent);}

	// parameters for model run specification
	strcpy(uebergabestring,"wetterwahl");
	parameter[0].wetterwahl=(int) Parametereingabe(&uebergabestring[0], wortlaengemax, &trennzeichen[0], &parameter[0]);
	if (parameter[0].parameterlesenanzeige ==true) 
		{printf("Import:	%s <= %d ==> Main\n", uebergabestring, parameter[0].wetterwahl);}

	if (parameter[0].parameterlesenanzeige ==true)
		{printf("\n\n\t Model run specifications\n\n");}
	
	strcpy(uebergabestring,"simdauer");
	parameter[0].simdauer=(int) Parametereingabe(&uebergabestring[0], wortlaengemax, &trennzeichen[0], &parameter[0]);
	if (parameter[0].parameterlesenanzeige ==true) 
		{printf("Import:	%s <= %d ==> Main\n", uebergabestring, parameter[0].simdauer);}

	strcpy(uebergabestring,"runs");
	parameter[0].runs=(int) Parametereingabe(&uebergabestring[0], wortlaengemax, &trennzeichen[0], &parameter[0]);
	if (parameter[0].parameterlesenanzeige ==true) {printf("Import:	%s <= %d ==> Main\n", uebergabestring, parameter[0].runs);}

	strcpy(uebergabestring,"ivortmax");
	parameter[0].ivortmax=(unsigned int) Parametereingabe(&uebergabestring[0], wortlaengemax, &trennzeichen[0], &parameter[0]);
	if (parameter[0].parameterlesenanzeige ==true) 
		{printf("Import:	%s <= %d ==> Main\n", uebergabestring, parameter[0].ivortmax);}
	
	strcpy(uebergabestring,"stabilperiod");
	if (((int) Parametereingabe(&uebergabestring[0], wortlaengemax, &trennzeichen[0], &parameter[0]))==1) 
	{parameter[0].stabilperiod=true;}
	else {parameter[0].stabilperiod=false;};
	if (parameter[0].parameterlesenanzeige ==true) 
	{printf("Import:	%s <= %s ==> Main \n", uebergabestring, (parameter[0].stabilperiod)?"true":"false");}
	
	strcpy(uebergabestring,"stabilmovingwindow");
	parameter[0].stabilmovingwindow=Parametereingabe(&uebergabestring[0], wortlaengemax, &trennzeichen[0], &parameter[0]);
	if (parameter[0].parameterlesenanzeige ==true) 
	{printf("Import:	%s <= %4.3f ==> Main \n", uebergabestring, parameter[0].stabilmovingwindow);}

	strcpy(uebergabestring,"stabilpercentchangethreshold");
	parameter[0].stabilpercentchangethreshold=Parametereingabe(&uebergabestring[0], wortlaengemax, &trennzeichen[0], &parameter[0]);
	if (parameter[0].parameterlesenanzeige ==true) 
	{printf("Import:	%s <= %4.3f ==> Main \n", uebergabestring, parameter[0].stabilpercentchangethreshold);}
	
	strcpy(uebergabestring,"resetyear");
	parameter[0].resetyear=(int) Parametereingabe(&uebergabestring[0], wortlaengemax, &trennzeichen[0], &parameter[0]);
	if (parameter[0].parameterlesenanzeige ==true) {printf("Import:	%s <= %d ==> Main\n", uebergabestring, parameter[0].resetyear);}	
	
	// plot number specifications
	strcpy(uebergabestring,"mapxlength");
	parameter[0].mapxlength=(int) Parametereingabe(&uebergabestring[0], wortlaengemax, &trennzeichen[0], &parameter[0]);
	if (parameter[0].parameterlesenanzeige ==true) {printf("Import:	%s <= %d ==> Main\n", uebergabestring, parameter[0].mapxlength);}

	strcpy(uebergabestring,"mapylength");
	parameter[0].mapylength=(int) Parametereingabe(&uebergabestring[0], wortlaengemax, &trennzeichen[0], &parameter[0]);
	if (parameter[0].parameterlesenanzeige ==true) 
		{printf("Import:	%s <= %d ==> Main\n", uebergabestring, parameter[0].mapylength);}
	
	// climate processing specifications
	strcpy(uebergabestring,"weathercalcgradient");
	if (((int) Parametereingabe(&uebergabestring[0], wortlaengemax, &trennzeichen[0], &parameter[0]))==1) 
		{parameter[0].weathercalcgradient=true;}
	else {parameter[0].weathercalcgradient=false;};
	if (parameter[0].parameterlesenanzeige ==true) 
		{printf("Import:	%s <= %s ==> Main \n", uebergabestring, (parameter[0].weathercalcgradient)?"true":"false");}
	
	strcpy(uebergabestring,"lineartransekt");
	if (((int) Parametereingabe(&uebergabestring[0], wortlaengemax, &trennzeichen[0], &parameter[0]))==1) 
		{parameter[0].lineartransekt=true;}
	else {parameter[0].lineartransekt=false;};
	if (parameter[0].parameterlesenanzeige ==true) 
		{printf("Import:	%s <= %s ==> Main \n", uebergabestring, (parameter[0].lineartransekt)?"true":"false");}

	strcpy(uebergabestring,"Nposmax");
	parameter[0].Nposmax=Parametereingabe(&uebergabestring[0], wortlaengemax, &trennzeichen[0], &parameter[0]);
	if (parameter[0].parameterlesenanzeige ==true) 
		{printf("Import:	%s <= %4.3f ==> Main \n", uebergabestring, parameter[0].Nposmax);}

	strcpy(uebergabestring,"Nposmin");
	parameter[0].Nposmin=Parametereingabe(&uebergabestring[0], wortlaengemax, &trennzeichen[0], &parameter[0]);
	if (parameter[0].parameterlesenanzeige ==true) 
		{printf("Import:	%s <= %4.3f ==> Main \n", uebergabestring, parameter[0].Nposmin);}

	strcpy(uebergabestring,"sizemagnif");
	parameter[0].sizemagnif=(int) Parametereingabe(&uebergabestring[0], wortlaengemax, &trennzeichen[0], &parameter[0]);
	if (parameter[0].parameterlesenanzeige ==true) 
		{printf("Import:	%s <= %d ==> Main\n", uebergabestring, parameter[0].sizemagnif);}
			
	strcpy(uebergabestring,"startlag");
	parameter[0].startlag=(int) Parametereingabe(&uebergabestring[0], wortlaengemax, &trennzeichen[0], &parameter[0]);
	if (parameter[0].parameterlesenanzeige ==true) 
		{printf("Import:	%s <= %d ==> Main\n", uebergabestring, parameter[0].startlag);}

	strcpy(uebergabestring,"stabilfirstweatheryears");
	parameter[0].stabilfirstweatheryears=(int) Parametereingabe(&uebergabestring[0], wortlaengemax, &trennzeichen[0], &parameter[0]);
	if (parameter[0].parameterlesenanzeige ==true) 
		{printf("Import:	%s <= %d ==> Main\n", uebergabestring, parameter[0].stabilfirstweatheryears);}

	
	// Parameter for LARIX GMELINII (Rupr.) Rupr.
	if (parameter[0].parameterlesenanzeige ==true) 
		{printf("\n\n\t Parameter for LARIX GMELINII (RUPR.) RUPR.\n");}
	strcpy(uebergabestring,"zapfenalter");
	parameter[0].zapfenalter=(int) Parametereingabe(&uebergabestring[0], wortlaengemax, &trennzeichen[0], &parameter[0]);
	if (parameter[0].parameterlesenanzeige ==true) 
		{printf("Import:	%s <= %d ==> Main\n", uebergabestring, parameter[0].zapfenalter);}

	strcpy(uebergabestring,"samenflugrate");
	parameter[0].samenflugrate=Parametereingabe(&uebergabestring[0], wortlaengemax, &trennzeichen[0], &parameter[0]);
	if (parameter[0].parameterlesenanzeige ==true) 
		{printf("Import:	%s <= %4.3f ==> Main \n", uebergabestring, parameter[0].samenflugrate);}

	strcpy(uebergabestring,"dispersalmode");
	parameter[0].dispersalmode=(int) Parametereingabe(&uebergabestring[0], wortlaengemax, &trennzeichen[0], &parameter[0]);
	if (parameter[0].parameterlesenanzeige ==true) 
		{printf("Import:	%s <= %d ==> Main\n", uebergabestring, parameter[0].dispersalmode);}

	strcpy(uebergabestring,"entfernungsteiler");
	parameter[0].entfernungsteiler=Parametereingabe(&uebergabestring[0], wortlaengemax, &trennzeichen[0], &parameter[0]);
	if (parameter[0].parameterlesenanzeige ==true) 
		{printf("Import:	%s <= %4.3f ==> Main \n", uebergabestring, parameter[0].mortbg);}

	strcpy(uebergabestring,"samenprodfaktor");
	parameter[0].samenprodfaktor=Parametereingabe(&uebergabestring[0], wortlaengemax, &trennzeichen[0], &parameter[0]);
	if (parameter[0].parameterlesenanzeige ==true) 
		{printf("Import:	%s <= %4.3f ==> Main\n", uebergabestring, parameter[0].samenprodfaktor);}

	strcpy(uebergabestring,"keimungsrate");
	parameter[0].keimungsrate=Parametereingabe(&uebergabestring[0], wortlaengemax, &trennzeichen[0], &parameter[0]);
	if (parameter[0].parameterlesenanzeige ==true) 
		{printf("Import:	%s <= %4.4f ==> Main \n", uebergabestring, parameter[0].keimungsrate);}

	strcpy(uebergabestring,"keimungwettereinfluss");
	parameter[0].keimungwettereinfluss=Parametereingabe(&uebergabestring[0], wortlaengemax, &trennzeichen[0], &parameter[0]);
	if (parameter[0].parameterlesenanzeige ==true) 
		{printf("Import:	%s <= %4.3f ==> Main \n", uebergabestring, parameter[0].keimungwettereinfluss);}

	strcpy(uebergabestring,"realseedconnect");
	if (((int) Parametereingabe(&uebergabestring[0], wortlaengemax, &trennzeichen[0], &parameter[0]))==1) {parameter[0].realseedconnect=true;}
	else {parameter[0].realseedconnect=false;};
	if (parameter[0].parameterlesenanzeige ==true) 
		{printf("Import:	%s <= %s ==> Main \n", uebergabestring, (parameter[0].realseedconnect)?"true":"false");}

	strcpy(uebergabestring,"seeddispmode");
	parameter[0].seeddispmode=(int) (Parametereingabe(&uebergabestring[0], wortlaengemax, &trennzeichen[0], &parameter[0]));
	if (parameter[0].parameterlesenanzeige ==true) 
		{printf("Import:	%s <= %d ==> Main\n", uebergabestring, parameter[0].seeddispmode);}

	strcpy(uebergabestring,"seedintro");
	if (((int) Parametereingabe(&uebergabestring[0], wortlaengemax, &trennzeichen[0], &parameter[0]))==1) {parameter[0].seedintro=true;}
	else {parameter[0].seedintro=false;};
	if (parameter[0].parameterlesenanzeige ==true) 
		{printf("Import:	%s <= %s ==> Main \n", uebergabestring, (parameter[0].seedintro)?"true":"false");}

	strcpy(uebergabestring,"seedintropermanent");
	if (((int) Parametereingabe(&uebergabestring[0], wortlaengemax, &trennzeichen[0], &parameter[0]))==1) {parameter[0].seedintropermanent=true;}
	else {parameter[0].seedintropermanent=false;};
	if (parameter[0].parameterlesenanzeige ==true) 
		{printf("Import:	%s <= %s ==> Main \n", uebergabestring, (parameter[0].seedintropermanent)?"true":"false");}
		
	strcpy(uebergabestring,"etabbg");
	parameter[0].etabbg=(int) (Parametereingabe(&uebergabestring[0], wortlaengemax, &trennzeichen[0], &parameter[0]));
	if (parameter[0].parameterlesenanzeige ==true) 
		{printf("Import:	%s <= %d ==> Main\n", uebergabestring, parameter[0].etabbg);}

	strcpy(uebergabestring,"etabbgpermanent");
	parameter[0].etabbgpermanent=(int) (Parametereingabe(&uebergabestring[0], wortlaengemax, &trennzeichen[0], &parameter[0]));
	if (parameter[0].parameterlesenanzeige ==true) 
		{printf("Import:	%s <= %d ==> Main\n", uebergabestring, parameter[0].etabbgpermanent);}
	
	strcpy(uebergabestring,"jahremitsameneintrag");
	parameter[0].jahremitsameneintrag=(int) (Parametereingabe(&uebergabestring[0], wortlaengemax, &trennzeichen[0], &parameter[0]));
	if (parameter[0].parameterlesenanzeige ==true) 
		{printf("Import:	%s <= %d ==> Main\n", uebergabestring, parameter[0].jahremitsameneintrag);}

	strcpy(uebergabestring,"seedtravelbetween");
	if (((int) Parametereingabe(&uebergabestring[0], wortlaengemax, &trennzeichen[0], &parameter[0]))==1) 
		{parameter[0].seedtravelbetween=true;}
	else {parameter[0].seedtravelbetween=false;};
	if (parameter[0].parameterlesenanzeige ==true) 
		{printf("Import:	%s <= %s ==> Main \n", uebergabestring, (parameter[0].seedtravelbetween)?"true":"false");}

	if (parameter[0].parameterlesenanzeige ==true) 
		{printf("\n\n\t Wachstum\n\n");}
	strcpy(uebergabestring,"gdbasal");
	parameter[0].gdbasal=Parametereingabe(&uebergabestring[0], wortlaengemax, &trennzeichen[0], &parameter[0]);
	if (parameter[0].parameterlesenanzeige ==true) 
		{printf("Import:	%s <= %4.3f ==> Main \n", uebergabestring, parameter[0].gdbasal);}

	strcpy(uebergabestring,"gdbasalsapl");
	parameter[0].gdbasalsapl=Parametereingabe(&uebergabestring[0], wortlaengemax, &trennzeichen[0], &parameter[0]);
	if (parameter[0].parameterlesenanzeige ==true) 
		{printf("Import:	%s <= %4.3f ==> Main \n", uebergabestring, parameter[0].gdbasalsapl);}

	strcpy(uebergabestring,"gdbrust");
	parameter[0].gdbrust=Parametereingabe(&uebergabestring[0], wortlaengemax, &trennzeichen[0], &parameter[0]);
	if (parameter[0].parameterlesenanzeige ==true) 
		{printf("Import:	%s <= %4.3f ==> Main \n", uebergabestring, parameter[0].gdbrust);}

	strcpy(uebergabestring,"basaleinflussaltneu");
	parameter[0].basaleinflussaltneu=Parametereingabe(&uebergabestring[0], wortlaengemax, &trennzeichen[0], &parameter[0]);
	if (parameter[0].parameterlesenanzeige ==true) 
		{printf("Import:	%s <= %4.4f ==> Main \n", uebergabestring, parameter[0].basaleinflussaltneu);}

	strcpy(uebergabestring,"relwachstumeinfluss");
	parameter[0].relwachstumeinfluss=(int) (Parametereingabe(&uebergabestring[0], wortlaengemax, &trennzeichen[0], &parameter[0]));
	if (parameter[0].parameterlesenanzeige ==true) 
		{printf("Import:	%s <= %d ==> Main\n", uebergabestring, parameter[0].relwachstumeinfluss);}

	strcpy(uebergabestring,"aatnddratio");
	parameter[0].aatnddratio=Parametereingabe(&uebergabestring[0], wortlaengemax, &trennzeichen[0], &parameter[0]);
	if (parameter[0].parameterlesenanzeige ==true) {printf("Import:	%s <= %4.3f ==> Main \n", uebergabestring, parameter[0].aatnddratio);}

	strcpy(uebergabestring,"dichteart");
	parameter[0].dichteart=(int) (Parametereingabe(&uebergabestring[0], wortlaengemax, &trennzeichen[0], &parameter[0]));
	if (parameter[0].parameterlesenanzeige ==true) 
		{printf("Import:	%s <= %d ==> Main\n", uebergabestring, parameter[0].dichteart);}

	strcpy(uebergabestring,"incfac");
	parameter[0].incfac=Parametereingabe(&uebergabestring[0], wortlaengemax, &trennzeichen[0], &parameter[0]);
	if (parameter[0].parameterlesenanzeige ==true) 
		{printf("Import:	%s <= %4.3f ==> Main \n", uebergabestring, parameter[0].incfac);}

	strcpy(uebergabestring,"dichtewertmanipulatorexp");
	parameter[0].dichtewertmanipulatorexp=Parametereingabe(&uebergabestring[0], wortlaengemax, &trennzeichen[0], &parameter[0]);
	if (parameter[0].parameterlesenanzeige ==true) 
		{printf("Import:	%s <= %4.3f ==> Main \n", uebergabestring, parameter[0].dichtewertmanipulatorexp);}
		
	strcpy(uebergabestring,"calcinfarea");
	parameter[0].calcinfarea=(int) (Parametereingabe(&uebergabestring[0], wortlaengemax, &trennzeichen[0], &parameter[0]));
	if (parameter[0].parameterlesenanzeige ==true) 
		{printf("Import:	%s <= %d ==> Main\n", uebergabestring, parameter[0].calcinfarea);}

	strcpy(uebergabestring,"densitysmallweighing");
	parameter[0].densitysmallweighing=Parametereingabe(&uebergabestring[0], wortlaengemax, &trennzeichen[0], &parameter[0]);
	if (parameter[0].parameterlesenanzeige ==true) 
		{printf("Import:	%s <= %4.3f ==> Main \n", uebergabestring, parameter[0].densitysmallweighing);}

		strcpy(uebergabestring,"densitytreetile");
	parameter[0].densitytreetile=Parametereingabe(&uebergabestring[0], wortlaengemax, &trennzeichen[0], &parameter[0]);
	if (parameter[0].parameterlesenanzeige ==true) 
		{printf("Import:	%s <= %4.3f ==> Main \n", uebergabestring, parameter[0].densitytreetile);}

	strcpy(uebergabestring,"densitytiletree");
	parameter[0].densitytiletree=(int) (Parametereingabe(&uebergabestring[0], wortlaengemax, &trennzeichen[0], &parameter[0]));
	if (parameter[0].parameterlesenanzeige ==true) 
		{printf("Import:	%s <= %d ==> Main\n", uebergabestring, parameter[0].densitytiletree);}

		strcpy(uebergabestring,"desitymaxreduction");
	parameter[0].desitymaxreduction=Parametereingabe(&uebergabestring[0], wortlaengemax, &trennzeichen[0], &parameter[0]);
	if (parameter[0].parameterlesenanzeige ==true) 
		{printf("Import:	%s <= %4.3f ==> Main \n", uebergabestring, parameter[0].desitymaxreduction);}

		strcpy(uebergabestring,"dichtheightrel");
	parameter[0].dichtheightrel=(int) (Parametereingabe(&uebergabestring[0], wortlaengemax, &trennzeichen[0], &parameter[0]));
	if (parameter[0].parameterlesenanzeige ==true) 
		{printf("Import:	%s <= %d ==> Main\n", uebergabestring, parameter[0].dichtheightrel);}

		strcpy(uebergabestring,"dichtewertdbasaleinfluss");
	parameter[0].dichtewertdbasaleinfluss=Parametereingabe(&uebergabestring[0], wortlaengemax, &trennzeichen[0], &parameter[0]);
	if (parameter[0].parameterlesenanzeige ==true) 
		{printf("Import:	%s <= %4.3f ==> Main \n", uebergabestring, parameter[0].dichtewertdbasaleinfluss);}

	strcpy(uebergabestring,"dichtewertmaximumbeihoehe");
	parameter[0].dichtewertmaximumbeihoehe=Parametereingabe(&uebergabestring[0], wortlaengemax, &trennzeichen[0], &parameter[0]);
	if (parameter[0].parameterlesenanzeige ==true) 
		{printf("Import:	%s <= %4.3f ==> Main \n", uebergabestring, parameter[0].dichtewertmaximumbeihoehe);}

	strcpy(uebergabestring,"allometriefunktionstyp");
	parameter[0].allometriefunktionstyp=(int) Parametereingabe(&uebergabestring[0], wortlaengemax, &trennzeichen[0], &parameter[0]);
	if (parameter[0].parameterlesenanzeige ==true) 
		{printf("Import:	%s <= %d ==> Main\n", uebergabestring, parameter[0].allometriefunktionstyp);}

	strcpy(uebergabestring,"dbasalhoeheallosteig");
	parameter[0].dbasalhoeheallosteig=Parametereingabe(&uebergabestring[0], wortlaengemax, &trennzeichen[0], &parameter[0]);
	if (parameter[0].parameterlesenanzeige ==true) 
		{printf("Import:	%s <= %4.4f ==> Main \n", uebergabestring, parameter[0].dbasalhoeheallosteig);}
	
	strcpy(uebergabestring,"dbasalhoehealloexp");
	parameter[0].dbasalhoehealloexp=Parametereingabe(&uebergabestring[0], wortlaengemax, &trennzeichen[0], &parameter[0]);
	if (parameter[0].parameterlesenanzeige ==true) 
		{printf("Import:	%s <= %4.4f ==> Main \n", uebergabestring, parameter[0].dbasalhoehealloexp);}

	strcpy(uebergabestring,"dbrusthoeheallosteig");
	parameter[0].dbrusthoeheallosteig=Parametereingabe(&uebergabestring[0], wortlaengemax, &trennzeichen[0], &parameter[0]);
	if (parameter[0].parameterlesenanzeige ==true) 
		{printf("Import:	%s <= %4.4f ==> Main \n", uebergabestring, parameter[0].dbrusthoeheallosteig);}
	
	strcpy(uebergabestring,"dbrusthoehealloexp");
	parameter[0].dbrusthoehealloexp=Parametereingabe(&uebergabestring[0], wortlaengemax, &trennzeichen[0], &parameter[0]);
	if (parameter[0].parameterlesenanzeige ==true) 
		{printf("Import:	%s <= %4.4f ==> Main \n", uebergabestring, parameter[0].dbrusthoehealloexp);}

	strcpy(uebergabestring,"dbasalhoehesteignonlin");
	parameter[0].dbasalhoehesteignonlin=Parametereingabe(&uebergabestring[0], wortlaengemax, &trennzeichen[0], &parameter[0]);
	if (parameter[0].parameterlesenanzeige ==true) 
		{printf("Import:	%s <= %4.4f ==> Main \n", uebergabestring, parameter[0].dbasalhoehesteignonlin);}
	
	strcpy(uebergabestring,"dbrusthoehesteignonlin");
	parameter[0].dbrusthoehesteignonlin=Parametereingabe(&uebergabestring[0], wortlaengemax, &trennzeichen[0], &parameter[0]);
	if (parameter[0].parameterlesenanzeige ==true) 
		{printf("Import:	%s <= %4.4f ==> Main \n", uebergabestring, parameter[0].dbrusthoehesteignonlin);}

		// mortality rate parameters
	if (parameter[0].parameterlesenanzeige ==true) 
		{printf("\n\n\t Mortality\n\n");}
	strcpy(uebergabestring,"mortbg");
	parameter[0].mortbg=Parametereingabe(&uebergabestring[0], wortlaengemax, &trennzeichen[0], &parameter[0]);
	if (parameter[0].parameterlesenanzeige ==true) 
		{printf("Import:	%s <= %4.3f ==> Main \n", uebergabestring, parameter[0].mortbg);}

	strcpy(uebergabestring,"hoechstalter");
	parameter[0].hoechstalter=(int) Parametereingabe(&uebergabestring[0], wortlaengemax, &trennzeichen[0], &parameter[0]);
	if (parameter[0].parameterlesenanzeige ==true) 
		{printf("Import:	%s <= %d ==> Main\n", uebergabestring, parameter[0].hoechstalter);}

	strcpy(uebergabestring,"malt");
	parameter[0].malt=Parametereingabe(&uebergabestring[0], wortlaengemax, &trennzeichen[0], &parameter[0]);
	if (parameter[0].parameterlesenanzeige ==true) 
		{printf("Import:	%s <= %4.4f ==> Main \n", uebergabestring, parameter[0].malt);}

	strcpy(uebergabestring,"mjung");
	parameter[0].mjung=Parametereingabe(&uebergabestring[0], wortlaengemax, &trennzeichen[0], &parameter[0]);
	if (parameter[0].parameterlesenanzeige ==true) 
		{printf("Import:	%s <= %4.4f ==> Main \n", uebergabestring, parameter[0].mjung);}

	strcpy(uebergabestring,"jugendmorteinflussexp");
	parameter[0].jugendmorteinflussexp=Parametereingabe(&uebergabestring[0], wortlaengemax, &trennzeichen[0], &parameter[0]);
	if (parameter[0].parameterlesenanzeige ==true) 
		{printf("Import:	%s <= %4.4f ==> Main \n", uebergabestring, parameter[0].jugendmorteinflussexp);}

	strcpy(uebergabestring,"mgrowth");
	parameter[0].mgrowth=Parametereingabe(&uebergabestring[0], wortlaengemax, &trennzeichen[0], &parameter[0]);
	if (parameter[0].parameterlesenanzeige ==true) 
		{printf("Import:	%s <= %4.4f ==> Main \n", uebergabestring, parameter[0].mgrowth);}

	strcpy(uebergabestring,"relwachstummorteinflussexp");
	parameter[0].relwachstummorteinflussexp=Parametereingabe(&uebergabestring[0], wortlaengemax, &trennzeichen[0], &parameter[0]);
	if (parameter[0].parameterlesenanzeige ==true) 
		{printf("Import:	%s <= %4.4f ==> Main \n", uebergabestring, parameter[0].relwachstummorteinflussexp);}

	strcpy(uebergabestring,"mwetter");
	parameter[0].mwetter=Parametereingabe(&uebergabestring[0], wortlaengemax, &trennzeichen[0], &parameter[0]);
	if (parameter[0].parameterlesenanzeige ==true) 
		{printf("Import:	%s <= %4.4f ==> Main \n", uebergabestring, parameter[0].mwetter);}

	strcpy(uebergabestring,"hoehewettermorteinflussexp");
	parameter[0].hoehewettermorteinflussexp=Parametereingabe(&uebergabestring[0], wortlaengemax, &trennzeichen[0], &parameter[0]);
	if (parameter[0].parameterlesenanzeige ==true) 
		{printf("Import:	%s <= %4.4f ==> Main \n", uebergabestring, parameter[0].hoehewettermorteinflussexp);}

	strcpy(uebergabestring,"mdichte");
	parameter[0].mdichte=Parametereingabe(&uebergabestring[0], wortlaengemax, &trennzeichen[0], &parameter[0]);
	if (parameter[0].parameterlesenanzeige ==true) 
		{printf("Import:	%s <= %4.4f ==> Main \n", uebergabestring, parameter[0].mdichte);}

	strcpy(uebergabestring,"mtrockenheit");
	parameter[0].mtrockenheit=Parametereingabe(&uebergabestring[0], wortlaengemax, &trennzeichen[0], &parameter[0]);
	if (parameter[0].parameterlesenanzeige ==true) 
		{printf("Import:	%s <= %4.4f ==> Main \n", uebergabestring, parameter[0].mtrockenheit);}

	strcpy(uebergabestring,"samenbaummort");
	parameter[0].samenbaummort=Parametereingabe(&uebergabestring[0], wortlaengemax, &trennzeichen[0], &parameter[0]);
	if (parameter[0].parameterlesenanzeige ==true) 
		{printf("Import:	%s <= %4.4f ==> Main \n", uebergabestring, parameter[0].samenbaummort);}

	strcpy(uebergabestring,"samenbodenmort");
	parameter[0].samenbodenmort=Parametereingabe(&uebergabestring[0], wortlaengemax, &trennzeichen[0], &parameter[0]);
	if (parameter[0].parameterlesenanzeige ==true) 
		{printf("Import:	%s <= %4.4f ==> Main \n", uebergabestring, parameter[0].samenbodenmort);}

	if (parameter[0].parameterlesenanzeige ==true) 
		{printf("\n<----\t PARAMETEREINGABE\n");};
		
	// globally set climate variables
	parameter[0].tempdiffort=0.0;
	parameter[0].precdiffort=0.0;
			
}

