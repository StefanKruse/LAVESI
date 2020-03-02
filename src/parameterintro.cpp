	#include "LAVESI.h"			   
using namespace std;
//THERE IS A SIMILARLY NAMED VOID FUNCTION BELOW!?
double Parameterinput(char *uebergabestring, int stringlengthmax, char *divisionsign, struct Parameter *parameter)
{
	FILE *f;
	f = fopen("parameters.txt","r");
	if (f == NULL)
	{
		printf("File with parameters not found!\n");
		exit(1);
	}

	char searchstring[255];
	strcpy(searchstring , uebergabestring);
	char puffer[255], puffer_bak[255], puffer_zahl[255];
	int counter=1;
	char *wort;
	char *zahl;

	while( fgets(puffer, stringlengthmax, f) != NULL)
	{
		strcpy(puffer_bak, puffer);
		wort = strtok(puffer, divisionsign);
		while (wort != NULL)
		{
			// check if the parameter is found, read in parameter
			if ( strcmp(wort, searchstring)==0)
			{
				strcpy(puffer_zahl, puffer_bak);
			}
			wort = strtok(NULL, divisionsign);
		}
		counter++;
	}

	// conversion from string to double
	strtok(puffer_zahl, divisionsign);
	zahl= strtok(NULL, divisionsign);
	double zahldouble=strtod(zahl, NULL);
	if (parameter[0].parameterinputvis==true) 
	{
		printf("Searching:\t%s <= %4.5f \n", searchstring, zahldouble);
	}

	fclose(f);
	wort=NULL;
	zahl=NULL;
	return zahldouble;
}



void Parameterinput(void)
{
	char uebergabestring[stringlengthmax];

	strcpy(uebergabestring,"parameterinputvis");
	if (((int) Parameterinput(&uebergabestring[0], stringlengthmax, &divisionsign[0], &parameter[0]))==1) 
	{
		parameter[0].parameterinputvis=true;
	}
	else 
	{
		parameter[0].parameterinputvis=false;
	};
	
	if (parameter[0].parameterinputvis ==true) 
	{
		printf("\n---->\t PARAMETERINPUT\n\nread:	%s <= %s ==> Main \n", uebergabestring,		(parameter[0].parameterinputvis)?"true":"false");
	}

	strcpy(uebergabestring,"yearlyvis");
	if (((int) Parameterinput(&uebergabestring[0], stringlengthmax, &divisionsign[0], &parameter[0]))==1) 
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
	if (((int) Parameterinput(&uebergabestring[0], stringlengthmax, &divisionsign[0], &parameter[0]))==1) 
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
	if (((int) Parameterinput(&uebergabestring[0], stringlengthmax, &divisionsign[0], &parameter[0]))==1) 
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
	parameter[0].outputmode=(int) (Parameterinput(&uebergabestring[0], stringlengthmax, &divisionsign[0], &parameter[0]));
	if (parameter[0].parameterinputvis ==true) 
	{
		printf("read:	%s <= %d ==> Main\n", uebergabestring, parameter[0].outputmode);
	}
	
		strcpy(uebergabestring,"timedoutput");
	if (((int) Parameterinput(&uebergabestring[0], stringlengthmax, &divisionsign[0], &parameter[0]))==1) 
	{
		parameter[0].timedoutput=true;
	}
	else 
	{
		parameter[0].timedoutput=false;
	};
	if (parameter[0].parameterinputvis ==true) 
	{
		printf("read:	%s <= %s ==> Main \n", uebergabestring, (parameter[0].timedoutput)?"true":"false");
	}
	
	strcpy(uebergabestring,"timelimit");
	parameter[0].timelimit=(double) (Parameterinput(&uebergabestring[0], stringlengthmax, &divisionsign[0], &parameter[0]));
	if (parameter[0].parameterinputvis ==true) 
	{
		printf("read:	%s <= %4.3f ==> Main\n", uebergabestring, parameter[0].timelimit);
	}
	
	strcpy(uebergabestring,"seedwinddispersalvis");
	if (((int) Parameterinput(&uebergabestring[0], stringlengthmax, &divisionsign[0], &parameter[0]))==1) 
	{
		parameter[0].seedwinddispersalvis=true;
	}
	else 
	{
		parameter[0].seedwinddispersalvis=false;
	};
	if (parameter[0].parameterinputvis ==true) 
	{
		printf("read:	%s <= %s ==> Main \n", uebergabestring, (parameter[0].seedwinddispersalvis)?"true":"false");
	}

	strcpy(uebergabestring,"mortvis");
	if (((int) Parameterinput(&uebergabestring[0], stringlengthmax, &divisionsign[0], &parameter[0]))==1) 
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

	strcpy(uebergabestring,"evapod");
	parameter[0].evapod=Parameterinput(&uebergabestring[0], stringlengthmax, &divisionsign[0], &parameter[0]);
	if (parameter[0].parameterinputvis ==true) 
	{
		printf("read:	%s <= %4.3f ==> Main \n", uebergabestring, parameter[0].evapod);
	}

	strcpy(uebergabestring,"precweather");
	parameter[0].precweather=(int) (Parameterinput(&uebergabestring[0], stringlengthmax, &divisionsign[0], &parameter[0]));
	if (parameter[0].parameterinputvis ==true) 
	{
		printf("read:	%s <= %d ==> Main\n", uebergabestring, parameter[0].precweather);
	}

	strcpy(uebergabestring,"precthreshold");
	parameter[0].precthreshold=Parameterinput(&uebergabestring[0], stringlengthmax, &divisionsign[0], &parameter[0]);
	if (parameter[0].parameterinputvis ==true) 
	{
		printf("read:	%s <= %4.3f ==> Main \n", uebergabestring, parameter[0].precthreshold);
	}

	strcpy(uebergabestring,"thawing_depth");
	if (((int) Parameterinput(&uebergabestring[0], stringlengthmax, &divisionsign[0], &parameter[0]))==1) 
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
	if (((int) Parameterinput(&uebergabestring[0], stringlengthmax, &divisionsign[0], &parameter[0]))==1) 
	{
		parameter[0].vegetation=true;
	}
	else 
	{
		parameter[0].vegetation=false;
	};
	if (parameter[0].parameterinputvis ==true) 
	{
		printf("read:	%s <= %s ==> Main \n", uebergabestring, (parameter[0].vegetation)?"true":"false");
	}

	strcpy(uebergabestring,"omp_num_threads");
	parameter[0].omp_num_threads=(int) Parameterinput(&uebergabestring[0], stringlengthmax, &divisionsign[0], &parameter[0]);
	if (parameter[0].parameterinputvis ==true) 
	{
		printf("read:	%s <= %d ==> Main\n", uebergabestring, parameter[0].omp_num_threads);
	}

	strcpy(uebergabestring,"weatherchoice");
	parameter[0].weatherchoice=(int) Parameterinput(&uebergabestring[0], stringlengthmax, &divisionsign[0], &parameter[0]);
	if (parameter[0].parameterinputvis ==true) 
	{
		printf("read:	%s <= %d ==> Main\n", uebergabestring, parameter[0].weatherchoice);
	}

	if (parameter[0].parameterinputvis ==true) 
	{
		printf("\n\n\t Modellparameter\n\n");
	}
	
	strcpy(uebergabestring,"simduration");
	parameter[0].simduration=(int) Parameterinput(&uebergabestring[0], stringlengthmax, &divisionsign[0], &parameter[0]);
	if (parameter[0].parameterinputvis ==true) 
	{
		printf("read:	%s <= %d ==> Main\n", uebergabestring, parameter[0].simduration);
	}

	strcpy(uebergabestring,"runs");
	parameter[0].runs=(int) Parameterinput(&uebergabestring[0], stringlengthmax, &divisionsign[0], &parameter[0]);
	if (parameter[0].parameterinputvis ==true) 
	{
		printf("read:	%s <= %d ==> Main\n", uebergabestring, parameter[0].runs);
	}

	strcpy(uebergabestring,"ivortmax");
	parameter[0].ivortmax=(unsigned int) Parameterinput(&uebergabestring[0], stringlengthmax, &divisionsign[0], &parameter[0]);
	if (parameter[0].parameterinputvis ==true) 
	{
		printf("read:	%s <= %d ==> Main\n", uebergabestring, parameter[0].ivortmax);
	}
	
	strcpy(uebergabestring,"stabilperiod");
	if (((int) Parameterinput(&uebergabestring[0], stringlengthmax, &divisionsign[0], &parameter[0]))==1) 
	{
		parameter[0].stabilperiod=true;
	}
	else 
	{
		parameter[0].stabilperiod=false;
	};
	if (parameter[0].parameterinputvis ==true) 
	{
		printf("read:	%s <= %s ==> Main \n", uebergabestring, (parameter[0].stabilperiod)?"true":"false");
	}

	strcpy(uebergabestring,"stabilmovingwindow");
	parameter[0].stabilmovingwindow=Parameterinput(&uebergabestring[0], stringlengthmax, &divisionsign[0], &parameter[0]);
	if (parameter[0].parameterinputvis ==true) 
	{
		printf("read:	%s <= %4.3f ==> Main \n", uebergabestring, parameter[0].stabilmovingwindow);
	}

	strcpy(uebergabestring,"stabilpercentchangethreshold");
	parameter[0].stabilpercentchangethreshold=Parameterinput(&uebergabestring[0], stringlengthmax, &divisionsign[0], &parameter[0]);
	if (parameter[0].parameterinputvis ==true) 
	{
		printf("read:	%s <= %4.3f ==> Main \n", uebergabestring, parameter[0].stabilpercentchangethreshold);
	}
	
	strcpy(uebergabestring,"resetyear");
	parameter[0].resetyear=(int) Parameterinput(&uebergabestring[0], stringlengthmax, &divisionsign[0], &parameter[0]);
	if (parameter[0].parameterinputvis ==true) 
	{
		printf("read:	%s <= %d ==> Main\n", uebergabestring, parameter[0].resetyear);
	}	
	
	strcpy(uebergabestring,"specpres");
	parameter[0].specpres=(int) Parameterinput(&uebergabestring[0], stringlengthmax, &divisionsign[0], &parameter[0]);
	if (parameter[0].parameterinputvis ==true) 
	{
		printf("read:	%s <= %d ==> Main\n", uebergabestring, parameter[0].specpres);
	}	
	
	strcpy(uebergabestring,"mapxlength");
	parameter[0].mapxlength=(int) Parameterinput(&uebergabestring[0], stringlengthmax, &divisionsign[0], &parameter[0]);
	if (parameter[0].parameterinputvis ==true) 
	{
		printf("read:	%s <= %d ==> Main\n", uebergabestring, parameter[0].mapxlength);
	}

	strcpy(uebergabestring,"mapylength");
	parameter[0].mapylength=(int) Parameterinput(&uebergabestring[0], stringlengthmax, &divisionsign[0], &parameter[0]);
	if (parameter[0].parameterinputvis ==true) 
	{
		printf("read:	%s <= %d ==> Main\n", uebergabestring, parameter[0].mapylength);
	}
	
	strcpy(uebergabestring,"weathercalcgradient");
	if (((int) Parameterinput(&uebergabestring[0], stringlengthmax, &divisionsign[0], &parameter[0]))==1) 
	{
		parameter[0].weathercalcgradient=true;
	}
	else 
	{
		parameter[0].weathercalcgradient=false;
	};
	if (parameter[0].parameterinputvis ==true) 
	{
		printf("read:	%s <= %s ==> Main \n", uebergabestring, (parameter[0].weathercalcgradient)?"true":"false");
	}
	
	strcpy(uebergabestring,"lineartransect");
	if (((int) Parameterinput(&uebergabestring[0], stringlengthmax, &divisionsign[0], &parameter[0]))==1) 
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
	parameter[0].nposmax=Parameterinput(&uebergabestring[0], stringlengthmax, &divisionsign[0], &parameter[0]);
	if (parameter[0].parameterinputvis ==true) 
	{
		printf("read:	%s <= %4.3f ==> Main \n", uebergabestring, parameter[0].nposmax);
	}

	strcpy(uebergabestring,"nposmin");
	parameter[0].nposmin=Parameterinput(&uebergabestring[0], stringlengthmax, &divisionsign[0], &parameter[0]);
	if (parameter[0].parameterinputvis ==true) 
	{
		printf("read:	%s <= %4.3f ==> Main \n", uebergabestring, parameter[0].nposmin);
	}

	strcpy(uebergabestring,"sizemagnif");
	parameter[0].sizemagnif=(int) Parameterinput(&uebergabestring[0], stringlengthmax, &divisionsign[0], &parameter[0]);
	if (parameter[0].parameterinputvis ==true) 
	{
		printf("read:	%s <= %d ==> Main\n", uebergabestring, parameter[0].sizemagnif);
	}
	
	strcpy(uebergabestring,"pollengridpoints");
	parameter[0].pollengridpoints=(int) Parameterinput(&uebergabestring[0], stringlengthmax, &divisionsign[0], &parameter[0]);
	if (parameter[0].parameterinputvis ==true) 
	{
		printf("read:	%s <= %d ==> Main\n", uebergabestring, parameter[0].pollengridpoints);
	}
	
	strcpy(uebergabestring,"pollengridxpoints");
	parameter[0].pollengridxpoints=(int) Parameterinput(&uebergabestring[0], stringlengthmax, &divisionsign[0], &parameter[0]);
	if (parameter[0].parameterinputvis ==true) 
	{
		printf("read:	%s <= %d ==> Main\n", uebergabestring, parameter[0].pollengridxpoints);
	}
	
	strcpy(uebergabestring,"pollengridypoints");
	parameter[0].pollengridypoints=(int) Parameterinput(&uebergabestring[0], stringlengthmax, &divisionsign[0], &parameter[0]);
	if (parameter[0].parameterinputvis ==true) 
	{
		printf("read:	%s <= %d ==> Main\n", uebergabestring, parameter[0].pollengridypoints);
	}

	strcpy(uebergabestring,"starttrees");
	parameter[0].starttrees=(int) Parameterinput(&uebergabestring[0], stringlengthmax, &divisionsign[0], &parameter[0]);
	if (parameter[0].parameterinputvis ==true) 
	{
		printf("read:	%s <= %d ==> Main\n", uebergabestring, parameter[0].starttrees);
	}
	strcpy(uebergabestring,"hinterland_maxlength");
	parameter[0].hinterland_maxlength=(int) Parameterinput(&uebergabestring[0], stringlengthmax, &divisionsign[0], &parameter[0]);
	if (parameter[0].parameterinputvis ==true) 
	{
		printf("read:	%s <= %d ==> Main\n", uebergabestring, parameter[0].hinterland_maxlength);
	}
	
	strcpy(uebergabestring,"coneage");
	parameter[0].coneage=(int) Parameterinput(&uebergabestring[0], stringlengthmax, &divisionsign[0], &parameter[0]);
	if (parameter[0].parameterinputvis ==true) 
	{
		printf("read:	%s <= %d ==> Main\n", uebergabestring, parameter[0].coneage);
	}

	strcpy(uebergabestring,"seedflightrate");
	parameter[0].seedflightrate=Parameterinput(&uebergabestring[0], stringlengthmax, &divisionsign[0], &parameter[0]);
	if (parameter[0].parameterinputvis ==true) 
	{
		printf("read:	%s <= %4.3f ==> Main \n", uebergabestring, parameter[0].seedflightrate);
	}

	strcpy(uebergabestring,"dispersalmode");
	parameter[0].dispersalmode=(int) Parameterinput(&uebergabestring[0], stringlengthmax, &divisionsign[0], &parameter[0]);
	if (parameter[0].parameterinputvis ==true) 
	{
		printf("read:	%s <= %d ==> Main\n", uebergabestring, parameter[0].dispersalmode);
	}
		
	strcpy(uebergabestring,"seedtravelbreezeg");
	parameter[0].seedtravelbreezeg=Parameterinput(&uebergabestring[0], stringlengthmax, &divisionsign[0], &parameter[0]);
	if (parameter[0].parameterinputvis ==true) 
	{
		printf("read:	%s <= %4.1f ==> Main\n", uebergabestring, parameter[0].seedtravelbreezeg);
	}	

	strcpy(uebergabestring,"seedtravelbreezes");
	parameter[0].seedtravelbreezes=Parameterinput(&uebergabestring[0], stringlengthmax, &divisionsign[0], &parameter[0]);
	if (parameter[0].parameterinputvis ==true) 
	{
		printf("read:	%s <= %4.1f ==> Main\n", uebergabestring, parameter[0].seedtravelbreezes);
	}	

	strcpy(uebergabestring,"seeddescentg");
	parameter[0].seeddescentg=Parameterinput(&uebergabestring[0], stringlengthmax, &divisionsign[0], &parameter[0]);
	if (parameter[0].parameterinputvis ==true) 
	{
		printf("read:	%s <= %4.2f ==> Main\n", uebergabestring, parameter[0].seeddescentg);
	}	

	strcpy(uebergabestring,"seeddescents");
	parameter[0].seeddescents=Parameterinput(&uebergabestring[0], stringlengthmax, &divisionsign[0], &parameter[0]);
	if (parameter[0].parameterinputvis ==true) 
	{
		printf("read:	%s <= %4.2f ==> Main\n", uebergabestring, parameter[0].seeddescents);
	}	
		
	strcpy(uebergabestring,"distanceratio");
	parameter[0].distanceratio=Parameterinput(&uebergabestring[0], stringlengthmax, &divisionsign[0], &parameter[0]);
	if (parameter[0].parameterinputvis ==true) 
	{
		printf("read:	%s <= %4.3f ==> Main \n", uebergabestring, parameter[0].mortbg);
	}

	strcpy(uebergabestring,"seedprodfactor");
	parameter[0].seedprodfactor=Parameterinput(&uebergabestring[0], stringlengthmax, &divisionsign[0], &parameter[0]);
	if (parameter[0].parameterinputvis ==true) 
	{
		printf("read:	%s <= %4.3f ==> Main\n", uebergabestring, parameter[0].seedprodfactor);
	}

	strcpy(uebergabestring,"germinationrate");
	parameter[0].germinationrate=Parameterinput(&uebergabestring[0], stringlengthmax, &divisionsign[0], &parameter[0]);
	if (parameter[0].parameterinputvis ==true) 
	{
		printf("read:	%s <= %4.4f ==> Main \n", uebergabestring, parameter[0].germinationrate);
	}

	strcpy(uebergabestring,"germinatioweatherinfluence");
	parameter[0].germinatioweatherinfluence=Parameterinput(&uebergabestring[0], stringlengthmax, &divisionsign[0], &parameter[0]);
	if (parameter[0].parameterinputvis ==true) 
	{
		printf("read:	%s <= %4.3f ==> Main \n", uebergabestring, parameter[0].germinatioweatherinfluence);
	}

	strcpy(uebergabestring,"realseedconnect");
	if (((int) Parameterinput(&uebergabestring[0], stringlengthmax, &divisionsign[0], &parameter[0]))==1) 
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

	strcpy(uebergabestring,"seedwinddispersalmode");
	parameter[0].seedwinddispersalmode=(int) (Parameterinput(&uebergabestring[0], stringlengthmax, &divisionsign[0], &parameter[0]));
	if (parameter[0].parameterinputvis ==true) 
	{
		printf("read:	%s <= %d ==> Main\n", uebergabestring, parameter[0].seedwinddispersalmode);
	}

	strcpy(uebergabestring,"seedintro");
	if (((int) Parameterinput(&uebergabestring[0], stringlengthmax, &divisionsign[0], &parameter[0]))==1) 
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
	if (((int) Parameterinput(&uebergabestring[0], stringlengthmax, &divisionsign[0], &parameter[0]))==1) 
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
	parameter[0].seedintronumber=(int) (Parameterinput(&uebergabestring[0], stringlengthmax, &divisionsign[0], &parameter[0]));
	if (parameter[0].parameterinputvis ==true) 
	{
		printf("read:	%s <= %d ==> Main\n", uebergabestring, parameter[0].seedintronumber);
	}

	strcpy(uebergabestring,"seedintronumberpermanent");
	parameter[0].seedintronumberpermanent=(int) (Parameterinput(&uebergabestring[0], stringlengthmax, &divisionsign[0], &parameter[0]));
	if (parameter[0].parameterinputvis ==true) 
	{
		printf("read:	%s <= %d ==> Main\n", uebergabestring, parameter[0].seedintronumberpermanent);
	}
	
	strcpy(uebergabestring,"yearswithseedintro");
	parameter[0].yearswithseedintro=(int) (Parameterinput(&uebergabestring[0], stringlengthmax, &divisionsign[0], &parameter[0]));
	if (parameter[0].parameterinputvis ==true) 
	{
		printf("read:	%s <= %d ==> Main\n", uebergabestring, parameter[0].yearswithseedintro);
	}

	strcpy(uebergabestring,"seedtravelbetween");
	if (((int) Parameterinput(&uebergabestring[0], stringlengthmax, &divisionsign[0], &parameter[0]))==1) 
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

	if (parameter[0].parameterinputvis ==true) 
	{
		printf("\n\n\t Growth\n\n");
	}

	strcpy(uebergabestring,"gdbasalfacqgmel");
	parameter[0].gdbasalfacqgmel= Parameterinput(&uebergabestring[0], stringlengthmax, &divisionsign[0], &parameter[0]);
	if (parameter[0].parameterinputvis ==true) 
	{
		printf("read:	%s <= %4.3f ==> Main \n", uebergabestring, parameter[0].gdbasalfacqgmel);
	}
		
	strcpy(uebergabestring,"gdbasalfacgmel");
	parameter[0].gdbasalfacgmel= Parameterinput(&uebergabestring[0], stringlengthmax, &divisionsign[0], &parameter[0]);
	if (parameter[0].parameterinputvis ==true) 
	{
		printf("read:	%s <= %4.3f ==> Main \n", uebergabestring, parameter[0].gdbasalfacgmel);
	}

	strcpy(uebergabestring,"gdbasalconstgmel");
	parameter[0].gdbasalconstgmel= Parameterinput(&uebergabestring[0], stringlengthmax, &divisionsign[0], &parameter[0]);
	if (parameter[0].parameterinputvis ==true) 
	{
		printf("read:	%s <= %4.3f ==> Main \n", uebergabestring, parameter[0].gdbasalconstgmel);
	}

	strcpy(uebergabestring,"gdbreastfacqgmel");
	parameter[0].gdbreastfacqgmel= Parameterinput(&uebergabestring[0], stringlengthmax, &divisionsign[0], &parameter[0]);
	if (parameter[0].parameterinputvis ==true) 
	{
		printf("read:	%s <= %4.4f ==> Main \n", uebergabestring, parameter[0].gdbreastfacqgmel);
	}		
		
	strcpy(uebergabestring,"gdbreastfacgmel");
	parameter[0].gdbreastfacgmel= Parameterinput(&uebergabestring[0], stringlengthmax, &divisionsign[0], &parameter[0]);
	if (parameter[0].parameterinputvis ==true) 
	{
		printf("read:	%s <= %4.3f ==> Main \n", uebergabestring, parameter[0].gdbreastfacgmel);
	}

	strcpy(uebergabestring,"gdbreastconstgmel");
	parameter[0].gdbreastconstgmel= Parameterinput(&uebergabestring[0], stringlengthmax, &divisionsign[0], &parameter[0]);
	if (parameter[0].parameterinputvis ==true) 
	{
		printf("read:	%s <= %4.3f ==> Main \n", uebergabestring, parameter[0].gdbreastconstgmel);
	}		
		
	strcpy(uebergabestring,"gdbasalfacqsib");
	parameter[0].gdbasalfacqsib= Parameterinput(&uebergabestring[0], stringlengthmax, &divisionsign[0], &parameter[0]);
	if (parameter[0].parameterinputvis ==true) 
	{
		printf("read:	%s <= %4.3f ==> Main \n", uebergabestring, parameter[0].gdbasalfacqsib);
	}		
		
	strcpy(uebergabestring,"gdbasalfacsib");
	parameter[0].gdbasalfacsib= Parameterinput(&uebergabestring[0], stringlengthmax, &divisionsign[0], &parameter[0]);
	if (parameter[0].parameterinputvis ==true) 
	{
		printf("read:	%s <= %4.3f ==> Main \n", uebergabestring, parameter[0].gdbasalfacsib);  
	}

	strcpy(uebergabestring,"gdbasalconstsib");
	parameter[0].gdbasalconstsib= Parameterinput(&uebergabestring[0], stringlengthmax, &divisionsign[0], &parameter[0]);
	if (parameter[0].parameterinputvis ==true) 
	{
		printf("read:	%s <= %4.3f ==> Main \n", uebergabestring, parameter[0].gdbasalconstsib);
	}		

	strcpy(uebergabestring,"gdbreastfacqsib");
	parameter[0].gdbreastfacqsib= Parameterinput(&uebergabestring[0], stringlengthmax, &divisionsign[0], &parameter[0]);
	if (parameter[0].parameterinputvis ==true) 
	{
		printf("read:	%s <= %4.3f ==> Main \n", uebergabestring, parameter[0].gdbreastfacqsib);
	}		
		
	strcpy(uebergabestring,"gdbreastfacsib");
	parameter[0].gdbreastfacsib= Parameterinput(&uebergabestring[0], stringlengthmax, &divisionsign[0], &parameter[0]);
	if (parameter[0].parameterinputvis ==true) 
	{
		printf("read:	%s <= %4.3f ==> Main \n", uebergabestring, parameter[0].gdbreastfacsib);
	}

	strcpy(uebergabestring,"gdbreastconstsib");
	parameter[0].gdbreastconstsib= Parameterinput(&uebergabestring[0], stringlengthmax, &divisionsign[0], &parameter[0]);
	if (parameter[0].parameterinputvis ==true) 
	{
		printf("read:	%s <= %4.3f ==> Main \n", uebergabestring, parameter[0].gdbreastconstsib);
	}	
		
	strcpy(uebergabestring,"basalinfluenceoldyoung");
	parameter[0].basalinfluenceoldyoung=Parameterinput(&uebergabestring[0], stringlengthmax, &divisionsign[0], &parameter[0]);
	if (parameter[0].parameterinputvis ==true) 
	{
		printf("read:	%s <= %4.4f ==> Main \n", uebergabestring, parameter[0].basalinfluenceoldyoung);
	}

	strcpy(uebergabestring,"relgrowthinfluence");
	parameter[0].relgrowthinfluence=(int) (Parameterinput(&uebergabestring[0], stringlengthmax, &divisionsign[0], &parameter[0]));
	if (parameter[0].parameterinputvis ==true) 
	{
		printf("read:	%s <= %d ==> Main\n", uebergabestring, parameter[0].relgrowthinfluence);
	}

	strcpy(uebergabestring,"densitymode");
	parameter[0].densitymode=(int) (Parameterinput(&uebergabestring[0], stringlengthmax, &divisionsign[0], &parameter[0]));
	if (parameter[0].parameterinputvis ==true) 
	{
		printf("read:	%s <= %d ==> Main\n", uebergabestring, parameter[0].densitymode);
	}

	strcpy(uebergabestring,"incfac");
	parameter[0].incfac=Parameterinput(&uebergabestring[0], stringlengthmax, &divisionsign[0], &parameter[0]);
	if (parameter[0].parameterinputvis ==true) 
	{
		printf("read:	%s <= %4.3f ==> Main \n", uebergabestring, parameter[0].incfac);
	}

	strcpy(uebergabestring,"densityvaluemanipulatorexp");
	parameter[0].densityvaluemanipulatorexp=Parameterinput(&uebergabestring[0], stringlengthmax, &divisionsign[0], &parameter[0]);
	if (parameter[0].parameterinputvis ==true) 
	{
		printf("read:	%s <= %4.3f ==> Main \n", uebergabestring, parameter[0].densityvaluemanipulatorexp);
	}
		
	strcpy(uebergabestring,"calcinfarea");
	parameter[0].calcinfarea=(int) (Parameterinput(&uebergabestring[0], stringlengthmax, &divisionsign[0], &parameter[0]));
	if (parameter[0].parameterinputvis ==true) 
	{
		printf("read:	%s <= %d ==> Main\n", uebergabestring, parameter[0].calcinfarea);
	}

	strcpy(uebergabestring,"densitysmallweighing");
	parameter[0].densitysmallweighing=Parameterinput(&uebergabestring[0], stringlengthmax, &divisionsign[0], &parameter[0]);
	if (parameter[0].parameterinputvis ==true) 
	{
		printf("read:	%s <= %4.3f ==> Main \n", uebergabestring, parameter[0].densitysmallweighing);
	}

	strcpy(uebergabestring,"densitytreetile");
	parameter[0].densitytreetile=Parameterinput(&uebergabestring[0], stringlengthmax, &divisionsign[0], &parameter[0]);
	if (parameter[0].parameterinputvis ==true) 
	{
		printf("read:	%s <= %4.3f ==> Main \n", uebergabestring, parameter[0].densitytreetile);
	}

	strcpy(uebergabestring,"densitytiletree");
	parameter[0].densitytiletree=(int) (Parameterinput(&uebergabestring[0], stringlengthmax, &divisionsign[0], &parameter[0]));
	if (parameter[0].parameterinputvis ==true) 
	{
		printf("read:	%s <= %d ==> Main\n", uebergabestring, parameter[0].densitytiletree);
	}

	strcpy(uebergabestring,"desitymaxreduction");
	parameter[0].desitymaxreduction=Parameterinput(&uebergabestring[0], stringlengthmax, &divisionsign[0], &parameter[0]);
	if (parameter[0].parameterinputvis ==true) 
	{
		printf("read:	%s <= %4.3f ==> Main \n", uebergabestring, parameter[0].desitymaxreduction);
	}

	strcpy(uebergabestring,"dichtheightrel");
	parameter[0].dichtheightrel=(int) (Parameterinput(&uebergabestring[0], stringlengthmax, &divisionsign[0], &parameter[0]));
	if (parameter[0].parameterinputvis ==true) 
	{
		printf("read:	%s <= %d ==> Main\n", uebergabestring, parameter[0].dichtheightrel);
	}

	strcpy(uebergabestring,"densityvaluedbasalinfluence");
	parameter[0].densityvaluedbasalinfluence=Parameterinput(&uebergabestring[0], stringlengthmax, &divisionsign[0], &parameter[0]);
	if (parameter[0].parameterinputvis ==true) 
	{
		printf("read:	%s <= %4.3f ==> Main \n", uebergabestring, parameter[0].densityvaluedbasalinfluence);
	}

	strcpy(uebergabestring,"densityvaluemaximumatheight");
	parameter[0].densityvaluemaximumatheight=Parameterinput(&uebergabestring[0], stringlengthmax, &divisionsign[0], &parameter[0]);
	if (parameter[0].parameterinputvis ==true) 
	{
		printf("read:	%s <= %4.3f ==> Main \n", uebergabestring, parameter[0].densityvaluemaximumatheight);
	}

	strcpy(uebergabestring,"allometryfunctiontype");
	parameter[0].allometryfunctiontype=(int) Parameterinput(&uebergabestring[0], stringlengthmax, &divisionsign[0], &parameter[0]);
	if (parameter[0].parameterinputvis ==true) 
	{
		printf("read:	%s <= %d ==> Main\n", uebergabestring, parameter[0].allometryfunctiontype);
	}

	strcpy(uebergabestring,"dbasalheightalloslope");
	parameter[0].dbasalheightalloslope=Parameterinput(&uebergabestring[0], stringlengthmax, &divisionsign[0], &parameter[0]);
	if (parameter[0].parameterinputvis ==true) 
	{
		printf("read:	%s <= %4.4f ==> Main \n", uebergabestring, parameter[0].dbasalheightalloslope);
	}
	
	strcpy(uebergabestring,"dbasalheightalloexp");
	parameter[0].dbasalheightalloexp=Parameterinput(&uebergabestring[0], stringlengthmax, &divisionsign[0], &parameter[0]);
	if (parameter[0].parameterinputvis ==true) 
	{
		printf("read:	%s <= %4.4f ==> Main \n", uebergabestring, parameter[0].dbasalheightalloexp);
	}

	strcpy(uebergabestring,"dbreastheightalloslope");
	parameter[0].dbreastheightalloslope=Parameterinput(&uebergabestring[0], stringlengthmax, &divisionsign[0], &parameter[0]);
	if (parameter[0].parameterinputvis ==true) 
	{
		printf("read:	%s <= %4.4f ==> Main \n", uebergabestring, parameter[0].dbreastheightalloslope);
	}

	strcpy(uebergabestring,"dbreastheightalloexp");
	parameter[0].dbreastheightalloexp=Parameterinput(&uebergabestring[0], stringlengthmax, &divisionsign[0], &parameter[0]);
	if (parameter[0].parameterinputvis ==true) 
	{
		printf("read:	%s <= %4.4f ==> Main \n", uebergabestring, parameter[0].dbreastheightalloexp);
	}

	strcpy(uebergabestring,"dbasalheightslopenonlin");
	parameter[0].dbasalheightslopenonlin=Parameterinput(&uebergabestring[0], stringlengthmax, &divisionsign[0], &parameter[0]);
	if (parameter[0].parameterinputvis ==true) 
	{
		printf("read:	%s <= %4.4f ==> Main \n", uebergabestring, parameter[0].dbasalheightslopenonlin);
	}
	
	strcpy(uebergabestring,"dbreastheightslopenonlin");
	parameter[0].dbreastheightslopenonlin=Parameterinput(&uebergabestring[0], stringlengthmax, &divisionsign[0], &parameter[0]);
	if (parameter[0].parameterinputvis ==true) 
	{
		printf("read:	%s <= %4.4f ==> Main \n", uebergabestring, parameter[0].dbreastheightslopenonlin);
	}

	strcpy(uebergabestring,"mortbg");
	parameter[0].mortbg=Parameterinput(&uebergabestring[0], stringlengthmax, &divisionsign[0], &parameter[0]);
	if (parameter[0].parameterinputvis ==true) 
	{
		printf("read:	%s <= %4.3f ==> Main \n", uebergabestring, parameter[0].mortbg);
	}

	strcpy(uebergabestring,"maximumage");
	parameter[0].maximumage=(int) Parameterinput(&uebergabestring[0], stringlengthmax, &divisionsign[0], &parameter[0]);
	if (parameter[0].parameterinputvis ==true) 
	{
		printf("read:	%s <= %d ==> Main\n", uebergabestring, parameter[0].maximumage);
	}

	strcpy(uebergabestring,"mortage");
	parameter[0].mortage=Parameterinput(&uebergabestring[0], stringlengthmax, &divisionsign[0], &parameter[0]);
	if (parameter[0].parameterinputvis ==true) 
	{
		printf("read:	%s <= %4.4f ==> Main \n", uebergabestring, parameter[0].mortage);
	}

	strcpy(uebergabestring,"mortyouth");
	parameter[0].mortyouth=Parameterinput(&uebergabestring[0], stringlengthmax, &divisionsign[0], &parameter[0]);
	if (parameter[0].parameterinputvis ==true) 
	{
		printf("read:	%s <= %4.4f ==> Main \n", uebergabestring, parameter[0].mortyouth);
	}

	strcpy(uebergabestring,"mortyouthinfluenceexp");
	parameter[0].mortyouthinfluenceexp=Parameterinput(&uebergabestring[0], stringlengthmax, &divisionsign[0], &parameter[0]);
	if (parameter[0].parameterinputvis ==true) 
	{
		printf("read:	%s <= %4.4f ==> Main \n", uebergabestring, parameter[0].mortyouthinfluenceexp);
	}

	strcpy(uebergabestring,"mgrowth");
	parameter[0].mgrowth=Parameterinput(&uebergabestring[0], stringlengthmax, &divisionsign[0], &parameter[0]);
	if (parameter[0].parameterinputvis ==true) 
	{
		printf("read:	%s <= %4.4f ==> Main \n", uebergabestring, parameter[0].mgrowth);
	}

	strcpy(uebergabestring,"relgrowthmortinfluenceexp");
	parameter[0].relgrowthmortinfluenceexp=Parameterinput(&uebergabestring[0], stringlengthmax, &divisionsign[0], &parameter[0]);
	if (parameter[0].parameterinputvis ==true) 
	{
		printf("read:	%s <= %4.4f ==> Main \n", uebergabestring, parameter[0].relgrowthmortinfluenceexp);
	}

	strcpy(uebergabestring,"mweather");
	parameter[0].mweather=Parameterinput(&uebergabestring[0], stringlengthmax, &divisionsign[0], &parameter[0]);
	if (parameter[0].parameterinputvis ==true) 
	{
		printf("read:	%s <= %4.4f ==> Main \n", uebergabestring, parameter[0].mweather);
	}

	strcpy(uebergabestring,"heightweathermorteinflussexp");
	parameter[0].heightweathermorteinflussexp=Parameterinput(&uebergabestring[0], stringlengthmax, &divisionsign[0], &parameter[0]);
	if (parameter[0].parameterinputvis ==true) 
	{
		printf("read:	%s <= %4.4f ==> Main \n", uebergabestring, parameter[0].heightweathermorteinflussexp);
	}

	strcpy(uebergabestring,"mdensity");
	parameter[0].mdensity=Parameterinput(&uebergabestring[0], stringlengthmax, &divisionsign[0], &parameter[0]);
	if (parameter[0].parameterinputvis ==true) 
	{
		printf("read:	%s <= %4.4f ==> Main \n", uebergabestring, parameter[0].mdensity);
	}

	strcpy(uebergabestring,"mdrought");
	parameter[0].mdrought=Parameterinput(&uebergabestring[0], stringlengthmax, &divisionsign[0], &parameter[0]);
	if (parameter[0].parameterinputvis ==true) 
	{
		printf("read:	%s <= %4.4f ==> Main \n", uebergabestring, parameter[0].mdrought);
	}

	strcpy(uebergabestring,"seedconemort");
	parameter[0].seedconemort=Parameterinput(&uebergabestring[0], stringlengthmax, &divisionsign[0], &parameter[0]);
	if (parameter[0].parameterinputvis ==true) 
	{
		printf("read:	%s <= %4.4f ==> Main \n", uebergabestring, parameter[0].seedconemort);
	}

	strcpy(uebergabestring,"seedfloormort");
	parameter[0].seedfloormort=Parameterinput(&uebergabestring[0], stringlengthmax, &divisionsign[0], &parameter[0]);
	if (parameter[0].parameterinputvis ==true) 
	{
		printf("read:	%s <= %4.4f ==> Main \n", uebergabestring, parameter[0].seedfloormort);
	}

	strcpy(uebergabestring,"gmelseedmaxage");
	parameter[0].gmelseedmaxage=(int) Parameterinput(&uebergabestring[0], stringlengthmax, &divisionsign[0], &parameter[0]);
	if (parameter[0].parameterinputvis ==true) 
	{
		printf("read:	%s <= %d ==> Main\n", uebergabestring, parameter[0].gmelseedmaxage);
	}

	strcpy(uebergabestring,"janthresholdtempgmel");
	parameter[0].janthresholdtempgmel= Parameterinput(&uebergabestring[0], stringlengthmax, &divisionsign[0], &parameter[0]);
	if (parameter[0].parameterinputvis ==true)
	{
		printf("read:	%s <= %4.2f ==> Main\n", uebergabestring, parameter[0].janthresholdtempgmel);
	}

	strcpy(uebergabestring,"weathervariablegmela");
	parameter[0].weathervariablegmela= Parameterinput(&uebergabestring[0], stringlengthmax, &divisionsign[0], &parameter[0]);
	if (parameter[0].parameterinputvis ==true)
	{
		printf("read:	%s <= %4.3f ==> Main\n", uebergabestring, parameter[0].weathervariablegmela);
	}

	strcpy(uebergabestring,"weathervariablegmelb");
	parameter[0].weathervariablegmelb= Parameterinput(&uebergabestring[0], stringlengthmax, &divisionsign[0], &parameter[0]);
	if (parameter[0].parameterinputvis ==true)
	{
		printf("read:	%s <= %4.3f ==> Main\n", uebergabestring, parameter[0].weathervariablegmelb);
	}

	strcpy(uebergabestring,"weathervariablegmelc");
	parameter[0].weathervariablegmelc= Parameterinput(&uebergabestring[0], stringlengthmax, &divisionsign[0], &parameter[0]);
	if (parameter[0].parameterinputvis ==true)
	{
		printf("read:	%s <= %4.3f ==> Main\n", uebergabestring, parameter[0].weathervariablegmelc);
	}

	strcpy(uebergabestring,"windsource");
	parameter[0].windsource=(int) Parameterinput(&uebergabestring[0],stringlengthmax, &divisionsign[0], &parameter[0]);
	if (parameter[0].parameterinputvis ==true)
	{
		printf("read:	%s <= %d ==> Main\n", uebergabestring, parameter[0].windsource);
	}
	
	strcpy(uebergabestring,"boundaryconditions");
	parameter[0].boundaryconditions=(int) Parameterinput(&uebergabestring[0],stringlengthmax, &divisionsign[0], &parameter[0]);
	if (parameter[0].parameterinputvis ==true)
	{
		printf("read:	%s <= %d ==> Main\n", uebergabestring, parameter[0].boundaryconditions);
	}
	
	strcpy(uebergabestring,"outputall");
	parameter[0].outputall=(int) Parameterinput(&uebergabestring[0],stringlengthmax, &divisionsign[0], &parameter[0]);
	if (parameter[0].parameterinputvis ==true)
	{
		printf("read:	%s <= %d ==> Main\n", uebergabestring, parameter[0].outputall);
	}
	
	strcpy(uebergabestring,"variabletraits");
	parameter[0].variabletraits=(int) Parameterinput(&uebergabestring[0],stringlengthmax, &divisionsign[0], &parameter[0]);
	if (parameter[0].parameterinputvis ==true)
	{
		printf("read:	%s <= %d ==> Main\n", uebergabestring, parameter[0].pollination);
	}
	
	strcpy(uebergabestring,"pollination");
	parameter[0].pollination=(int) Parameterinput(&uebergabestring[0],stringlengthmax, &divisionsign[0], &parameter[0]);
	if (parameter[0].parameterinputvis ==true)
	{
		printf("read:	%s <= %d ==> Main\n", uebergabestring, parameter[0].pollination);
	}
	
			
	strcpy(uebergabestring,"pollendirectionvariance");
	parameter[0].pollendirectionvariance=(double) Parameterinput(&uebergabestring[0],stringlengthmax, &divisionsign[0], &parameter[0]);
	if (parameter[0].parameterinputvis ==true)
	{
		printf("read:	%s <= %4.3f ==> Main\n", uebergabestring, parameter[0].pollendirectionvariance);
	}


	strcpy(uebergabestring,"pollenfall");
	parameter[0].pollenfall=(double) Parameterinput(&uebergabestring[0],stringlengthmax, &divisionsign[0], &parameter[0]);
	
	strcpy(uebergabestring,"pollengregoryc");
	parameter[0].pollengregoryc=(double) Parameterinput(&uebergabestring[0],stringlengthmax, &divisionsign[0], &parameter[0]);
	
	strcpy(uebergabestring,"pollengregorym");
	parameter[0].pollengregorym=(double) Parameterinput(&uebergabestring[0],stringlengthmax, &divisionsign[0], &parameter[0]);
	
	strcpy(uebergabestring, "computationtimevis");
	parameter[0].computationtimevis=(bool) Parameterinput(&uebergabestring[0],stringlengthmax, &divisionsign[0], &parameter[0]);
		
	
	// further variables
	parameter[0].tempdiffort=0.0;
	parameter[0].precdiffort=0.0;
}


