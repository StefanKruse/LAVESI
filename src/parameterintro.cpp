#include "LAVESI.h"

using namespace std;

double Parameterinput(const char* uebergabestring, int stringlengthmax, const char* divisionsign, Parameter* parameter) {
    FILE* f;
    f = fopen("parameters.txt", "r");
    if (f == NULL) {
        printf("File with parameters not found!\n");
        exit(1);
    }

    char searchstring[255];
    strcpy(searchstring, uebergabestring);
    char puffer[255], puffer_bak[255], puffer_zahl[255];
    int counter = 1;
    char* wort;
    char* zahl;

    while (fgets(puffer, stringlengthmax, f) != NULL) {
        strcpy(puffer_bak, puffer);
        wort = strtok(puffer, divisionsign);
        while (wort != NULL) {
            // check if the parameter is found, read in parameter
            if (strcmp(wort, searchstring) == 0) {
                strcpy(puffer_zahl, puffer_bak);
            }
            wort = strtok(NULL, divisionsign);
        }
        counter++;
    }

    // conversion from string to double
    strtok(puffer_zahl, divisionsign);
    zahl = strtok(NULL, divisionsign);
    double zahldouble = strtod(zahl, NULL);
    if (parameter[0].parameterinputvis == true) {
        printf("Searching:\t%s <= %4.5f \n", searchstring, zahldouble);
    }

    fclose(f);
    wort = NULL;
    zahl = NULL;
    return zahldouble;
}

void Parameterinput(void) {
    char uebergabestring[stringlengthmax];

    strcpy(uebergabestring, "parameterinputvis");
    if (((int)Parameterinput(&uebergabestring[0], stringlengthmax, divisionsign, &parameter[0])) == 1) {
        parameter[0].parameterinputvis = true;
    } else {
        parameter[0].parameterinputvis = false;
    };

    if (parameter[0].parameterinputvis == true) {
        printf("\n---->\t PARAMETERINPUT\n\nread:	%s <= %s ==> Main \n", uebergabestring, (parameter[0].parameterinputvis) ? "true" : "false");
    }

    strcpy(uebergabestring, "yearlyvis");
    if (((int)Parameterinput(&uebergabestring[0], stringlengthmax, divisionsign, &parameter[0])) == 1) {
        parameter[0].yearlyvis = true;
    } else {
        parameter[0].yearlyvis = false;
    };

    if (parameter[0].parameterinputvis == true) {
        printf("read:	%s <= %s ==> Main \n", uebergabestring, (parameter[0].yearlyvis) ? "true" : "false");
    }

    strcpy(uebergabestring, "qualiyearlyvis");
    if (((int)Parameterinput(&uebergabestring[0], stringlengthmax, divisionsign, &parameter[0])) == 1) {
        parameter[0].qualiyearlyvis = true;
    } else {
        parameter[0].qualiyearlyvis = false;
    };
    if (parameter[0].parameterinputvis == true) {
        printf("read:	%s <= %s ==> Main \n", uebergabestring, (parameter[0].qualiyearlyvis) ? "true" : "false");
    }

    strcpy(uebergabestring, "dataoutput");
    if (((int)Parameterinput(&uebergabestring[0], stringlengthmax, divisionsign, &parameter[0])) == 1) {
        parameter[0].dataoutput = true;
    } else {
        parameter[0].dataoutput = false;
    };
    if (parameter[0].parameterinputvis == true) {
        printf("read:	%s <= %s ==> Main \n", uebergabestring, (parameter[0].dataoutput) ? "true" : "false");
    }

    strcpy(uebergabestring, "outputmode");
    parameter[0].outputmode = (int)(Parameterinput(&uebergabestring[0], stringlengthmax, divisionsign, &parameter[0]));
    if (parameter[0].parameterinputvis == true) {
        printf("read:	%s <= %d ==> Main\n", uebergabestring, parameter[0].outputmode);
    }

    strcpy(uebergabestring, "seedwinddispersalvis");
    if (((int)Parameterinput(&uebergabestring[0], stringlengthmax, divisionsign, &parameter[0])) == 1) {
        parameter[0].seedwinddispersalvis = true;
    } else {
        parameter[0].seedwinddispersalvis = false;
    };
    if (parameter[0].parameterinputvis == true) {
        printf("read:	%s <= %s ==> Main \n", uebergabestring, (parameter[0].seedwinddispersalvis) ? "true" : "false");
    }

    strcpy(uebergabestring, "mortvis");
    if (((int)Parameterinput(&uebergabestring[0], stringlengthmax, divisionsign, &parameter[0])) == 1) {
        parameter[0].mortvis = true;
    } else {
        parameter[0].mortvis = false;
    };
    if (parameter[0].parameterinputvis == true) {
        printf("read:	%s <= %s ==> Main \n", uebergabestring, (parameter[0].mortvis) ? "true" : "false");
    }

    strcpy(uebergabestring, "elevationoffset");
    parameter[0].elevationoffset = Parameterinput(&uebergabestring[0], stringlengthmax, divisionsign, &parameter[0]);
    if (parameter[0].parameterinputvis == true) {
        printf("read:	%s <= %4.3f ==> Main \n", uebergabestring, parameter[0].elevationoffset);
    }

    strcpy(uebergabestring, "slopetwiratio");
    parameter[0].slopetwiratio = Parameterinput(&uebergabestring[0], stringlengthmax, divisionsign, &parameter[0]);
    if (parameter[0].parameterinputvis == true) {
        printf("read:	%s <= %4.3f ==> Main \n", uebergabestring, parameter[0].slopetwiratio);
    }
	
	strcpy(uebergabestring, "firemode");
    parameter[0].firemode = Parameterinput(&uebergabestring[0], stringlengthmax, divisionsign, &parameter[0]);
    if (parameter[0].parameterinputvis == true) {
        printf("read:	%s <= %d ==> Main\n", uebergabestring, parameter[0].firemode);
    }
	strcpy(uebergabestring, "globalfireradius");
    parameter[0].globalfireradius = Parameterinput(&uebergabestring[0], stringlengthmax, divisionsign, &parameter[0]);
    if (parameter[0].parameterinputvis == true) {
        printf("read:	%s <= %d ==> Main\n", uebergabestring, parameter[0].globalfireradius);
    }
	strcpy(uebergabestring, "firelocdistance");
    parameter[0].firelocdistance = Parameterinput(&uebergabestring[0], stringlengthmax, divisionsign, &parameter[0]);
    if (parameter[0].parameterinputvis == true) {
        printf("read:	%s <= %4.3f ==> Main\n", uebergabestring, parameter[0].firelocdistance);
    }
	
	strcpy(uebergabestring, "fireintensitymode");
    parameter[0].fireintensitymode = Parameterinput(&uebergabestring[0], stringlengthmax, divisionsign, &parameter[0]);
    if (parameter[0].parameterinputvis == true) {
        printf("read:	%s <= %4.3f ==> Main\n", uebergabestring, parameter[0].fireintensitymode);
    }
	
    strcpy(uebergabestring, "firegapoutput");
    if (((int)Parameterinput(&uebergabestring[0], stringlengthmax, divisionsign, &parameter[0])) == 1) {
        parameter[0].firegapoutput = true;
    } else {
        parameter[0].firegapoutput = false;
    };
    if (parameter[0].firegapoutput == true) {
        printf("read:	%s <= %s ==> Main \n", uebergabestring, (parameter[0].firegapoutput) ? "true" : "false");
    }
	
	strcpy(uebergabestring, "firegapoutput_threshold");
    parameter[0].firegapoutput_threshold = Parameterinput(&uebergabestring[0], stringlengthmax, divisionsign, &parameter[0]);
    if (parameter[0].parameterinputvis == true) {
        printf("read:	%s <= %4.3f ==> Main\n", uebergabestring, parameter[0].firegapoutput_threshold);
    }
	
	strcpy(uebergabestring, "firegapoutput_years");
    parameter[0].firegapoutput_years = Parameterinput(&uebergabestring[0], stringlengthmax, divisionsign, &parameter[0]);
    if (parameter[0].parameterinputvis == true) {
        printf("read:	%s <= %d ==> Main\n", uebergabestring, parameter[0].firegapoutput_years);
    }
	
	strcpy(uebergabestring, "counter_fire_happened");
    parameter[0].counter_fire_happened = Parameterinput(&uebergabestring[0], stringlengthmax, divisionsign, &parameter[0]);
    if (parameter[0].parameterinputvis == true) {
        printf("read:	%s <= %d ==> Main\n", uebergabestring, parameter[0].counter_fire_happened);
    }

    strcpy(uebergabestring, "temperaturelapse_jan");
    parameter[0].temperaturelapse_jan = Parameterinput(&uebergabestring[0], stringlengthmax, divisionsign, &parameter[0]);
    if (parameter[0].parameterinputvis == true) {
        printf("read:	%s <= %4.3f ==> Main \n", uebergabestring, parameter[0].temperaturelapse_jan);
    }
    strcpy(uebergabestring, "temperaturelapse_jul");
    parameter[0].temperaturelapse_jul = Parameterinput(&uebergabestring[0], stringlengthmax, divisionsign, &parameter[0]);
    if (parameter[0].parameterinputvis == true) {
        printf("read:	%s <= %4.3f ==> Main \n", uebergabestring, parameter[0].temperaturelapse_jul);
    }
    strcpy(uebergabestring, "precipitationlapse_year");
    parameter[0].precipitationlapse_year = Parameterinput(&uebergabestring[0], stringlengthmax, divisionsign, &parameter[0]);
    if (parameter[0].parameterinputvis == true) {
        printf("read:	%s <= %4.3f ==> Main \n", uebergabestring, parameter[0].precipitationlapse_year);
    }

    strcpy(uebergabestring, "evapod");
    parameter[0].evapod = Parameterinput(&uebergabestring[0], stringlengthmax, divisionsign, &parameter[0]);
    if (parameter[0].parameterinputvis == true) {
        printf("read:	%s <= %4.3f ==> Main \n", uebergabestring, parameter[0].evapod);
    }

    strcpy(uebergabestring, "precweather");
    parameter[0].precweather = (int)(Parameterinput(&uebergabestring[0], stringlengthmax, divisionsign, &parameter[0]));
    if (parameter[0].parameterinputvis == true) {
        printf("read:	%s <= %d ==> Main\n", uebergabestring, parameter[0].precweather);
    }

    strcpy(uebergabestring, "precthreshold");
    parameter[0].precthreshold = Parameterinput(&uebergabestring[0], stringlengthmax, divisionsign, &parameter[0]);
    if (parameter[0].parameterinputvis == true) {
        printf("read:	%s <= %4.3f ==> Main \n", uebergabestring, parameter[0].precthreshold);
    }

    strcpy(uebergabestring, "thawing_depth");
    if (((int)Parameterinput(&uebergabestring[0], stringlengthmax, divisionsign, &parameter[0])) == 1) {
        parameter[0].thawing_depth = true;
    } else {
        parameter[0].thawing_depth = false;
    };
    if (parameter[0].parameterinputvis == true) {
        printf("read:	%s <= %s ==> Main \n", uebergabestring, (parameter[0].thawing_depth) ? "true" : "false");
    }

    strcpy(uebergabestring, "demlandscape");
    if (((int)Parameterinput(&uebergabestring[0], stringlengthmax, divisionsign, &parameter[0])) == 1) {
        parameter[0].demlandscape = true;
    } else {
        parameter[0].demlandscape = false;
    };
    if (parameter[0].parameterinputvis == true) {
        printf("read:	%s <= %s ==> Main \n", uebergabestring, (parameter[0].demlandscape) ? "true" : "false");
    }

    strcpy(uebergabestring, "demresolution");
    parameter[0].demresolution = (int)Parameterinput(&uebergabestring[0], stringlengthmax, divisionsign, &parameter[0]);
    if (parameter[0].parameterinputvis == true) {
        printf("read:	%s <= %d ==> Main\n", uebergabestring, parameter[0].demresolution);
    }

    strcpy(uebergabestring, "litterlayer");
    if (((int)Parameterinput(&uebergabestring[0], stringlengthmax, divisionsign, &parameter[0])) == 1) {
        parameter[0].litterlayer = true;
    } else {
        parameter[0].litterlayer = false;
    };
    if (parameter[0].parameterinputvis == true) {
        printf("read:	%s <= %s ==> Main \n", uebergabestring, (parameter[0].litterlayer) ? "true" : "false");
    }

    strcpy(uebergabestring, "allow_pest_disturbances");
    if (((int)Parameterinput(&uebergabestring[0], stringlengthmax, divisionsign, &parameter[0])) == 1) {
        parameter[0].allow_pest_disturbances = true;
    } else {
        parameter[0].allow_pest_disturbances = false;
    };
    if (parameter[0].parameterinputvis == true) {
        printf("read:	%s <= %s ==> Main \n", uebergabestring, (parameter[0].allow_pest_disturbances) ? "true" : "false");
    }

    strcpy(uebergabestring, "pest_disturbances_impactfactor");
    parameter[0].pest_disturbances_impactfactor = Parameterinput(&uebergabestring[0], stringlengthmax, divisionsign, &parameter[0]);
    if (parameter[0].parameterinputvis == true) {
        printf("read:	%s <= %4.3f ==> Main \n", uebergabestring, parameter[0].pest_disturbances_impactfactor);
    }

    strcpy(uebergabestring, "omp_num_threads");
    parameter[0].omp_num_threads = (int)Parameterinput(&uebergabestring[0], stringlengthmax, divisionsign, &parameter[0]);
    if (parameter[0].parameterinputvis == true) {
        printf("read:	%s <= %d ==> Main\n", uebergabestring, parameter[0].omp_num_threads);
    }

    strcpy(uebergabestring, "weatherchoice");
    parameter[0].weatherchoice = (int)Parameterinput(&uebergabestring[0], stringlengthmax, divisionsign, &parameter[0]);
    if (parameter[0].parameterinputvis == true) {
        printf("read:	%s <= %ld ==> Main\n", uebergabestring, parameter[0].weatherchoice);
    }
	
    strcpy(uebergabestring, "plotcentre_lat");
    parameter[0].plotcentre_lat = Parameterinput(&uebergabestring[0], stringlengthmax, divisionsign, &parameter[0]);
    if (parameter[0].parameterinputvis == true) {
        printf("read:	%s <= %4.3f ==> Main \n", uebergabestring, parameter[0].plotcentre_lat);
    }
	strcpy(uebergabestring, "plotcentre_lon");
    parameter[0].plotcentre_lon = Parameterinput(&uebergabestring[0], stringlengthmax, divisionsign, &parameter[0]);
    if (parameter[0].parameterinputvis == true) {
        printf("read:	%s <= %4.3f ==> Main \n", uebergabestring, parameter[0].plotcentre_lon);
    }

    if (parameter[0].parameterinputvis == true) {
        printf("\n\n\t Modellparameter\n\n");
    }

    strcpy(uebergabestring, "lastyearweatherdata");
    parameter[0].lastyearweatherdata = (int)Parameterinput(&uebergabestring[0], stringlengthmax, divisionsign, &parameter[0]);
    if (parameter[0].parameterinputvis == true) {
        printf("read:	%s <= %d ==> Main\n", uebergabestring, parameter[0].lastyearweatherdata);
    }

    strcpy(uebergabestring, "simduration");
    parameter[0].simduration = (int)Parameterinput(&uebergabestring[0], stringlengthmax, divisionsign, &parameter[0]);
    if (parameter[0].parameterinputvis == true) {
        printf("read:	%s <= %d ==> Main\n", uebergabestring, parameter[0].simduration);
    }
    strcpy(uebergabestring, "stopatyear");
    parameter[0].stopatyear = (int)Parameterinput(&uebergabestring[0], stringlengthmax, divisionsign, &parameter[0]);
    if (parameter[0].parameterinputvis == true) {
        printf("read:	%s <= %d ==> Main\n", uebergabestring, parameter[0].stopatyear);
    }

    strcpy(uebergabestring, "runs");
    parameter[0].runs = (int)Parameterinput(&uebergabestring[0], stringlengthmax, divisionsign, &parameter[0]);
    if (parameter[0].parameterinputvis == true) {
        printf("read:	%s <= %d ==> Main\n", uebergabestring, parameter[0].runs);
    }

    strcpy(uebergabestring, "ivortmax");
    parameter[0].ivortmax = (unsigned int)Parameterinput(&uebergabestring[0], stringlengthmax, divisionsign, &parameter[0]);
    if (parameter[0].parameterinputvis == true) {
        printf("read:	%s <= %d ==> Main\n", uebergabestring, parameter[0].ivortmax);
    }

    strcpy(uebergabestring, "stabilperiod");
    if (((int)Parameterinput(&uebergabestring[0], stringlengthmax, divisionsign, &parameter[0])) == 1) {
        parameter[0].stabilperiod = true;
    } else {
        parameter[0].stabilperiod = false;
    };
    if (parameter[0].parameterinputvis == true) {
        printf("read:	%s <= %s ==> Main \n", uebergabestring, (parameter[0].stabilperiod) ? "true" : "false");
    }

    strcpy(uebergabestring, "stabilmovingwindow");
    parameter[0].stabilmovingwindow = Parameterinput(&uebergabestring[0], stringlengthmax, divisionsign, &parameter[0]);
    if (parameter[0].parameterinputvis == true) {
        printf("read:	%s <= %4.3f ==> Main \n", uebergabestring, parameter[0].stabilmovingwindow);
    }

    strcpy(uebergabestring, "stabilpercentchangethreshold");
    parameter[0].stabilpercentchangethreshold = Parameterinput(&uebergabestring[0], stringlengthmax, divisionsign, &parameter[0]);
    if (parameter[0].parameterinputvis == true) {
        printf("read:	%s <= %4.3f ==> Main \n", uebergabestring, parameter[0].stabilpercentchangethreshold);
    }

    strcpy(uebergabestring, "resetyear");
    parameter[0].resetyear = (int)Parameterinput(&uebergabestring[0], stringlengthmax, divisionsign, &parameter[0]);
    if (parameter[0].parameterinputvis == true) {
        printf("read:	%s <= %d ==> Main\n", uebergabestring, parameter[0].resetyear);
    }

    strcpy(uebergabestring, "specpres");
    parameter[0].specpres = (int)Parameterinput(&uebergabestring[0], stringlengthmax, divisionsign, &parameter[0]);
    if (parameter[0].parameterinputvis == true) {
        printf("read:	%s <= %d ==> Main\n", uebergabestring, parameter[0].specpres);
    }
	
    strcpy(uebergabestring, "roi");
    parameter[0].roi = (int)Parameterinput(&uebergabestring[0], stringlengthmax, divisionsign, &parameter[0]);
    if (parameter[0].parameterinputvis == true) {
        printf("read:	%s <= %d ==> Main\n", uebergabestring, parameter[0].roi);
    }

    strcpy(uebergabestring, "mapxlength");
    parameter[0].mapxlength = (int)Parameterinput(&uebergabestring[0], stringlengthmax, divisionsign, &parameter[0]);
    if (parameter[0].parameterinputvis == true) {
        printf("read:	%s <= %d ==> Main\n", uebergabestring, parameter[0].mapxlength);
    }

    strcpy(uebergabestring, "mapylength");
    parameter[0].mapylength = (int)Parameterinput(&uebergabestring[0], stringlengthmax, divisionsign, &parameter[0]);
    if (parameter[0].parameterinputvis == true) {
        printf("read:	%s <= %d ==> Main\n", uebergabestring, parameter[0].mapylength);
    }

    strcpy(uebergabestring, "weathercalcgradient");
    if (((int)Parameterinput(&uebergabestring[0], stringlengthmax, divisionsign, &parameter[0])) == 1) {
        parameter[0].weathercalcgradient = true;
    } else {
        parameter[0].weathercalcgradient = false;
    };
    if (parameter[0].parameterinputvis == true) {
        printf("read:	%s <= %s ==> Main \n", uebergabestring, (parameter[0].weathercalcgradient) ? "true" : "false");
    }

    strcpy(uebergabestring, "lineartransect");
    if (((int)Parameterinput(&uebergabestring[0], stringlengthmax, divisionsign, &parameter[0])) == 1) {
        parameter[0].lineartransect = true;
    } else {
        parameter[0].lineartransect = false;
    };
    if (parameter[0].parameterinputvis == true) {
        printf("read:	%s <= %s ==> Main \n", uebergabestring, (parameter[0].lineartransect) ? "true" : "false");
    }

    strcpy(uebergabestring, "locationshift");
    parameter[0].locationshift = Parameterinput(&uebergabestring[0], stringlengthmax, divisionsign, &parameter[0]);
    if (parameter[0].parameterinputvis == true) {
        printf("read:	%s <= %4.4f ==> Main \n", uebergabestring, parameter[0].locationshift);
    }

    strcpy(uebergabestring, "nposmax");
    parameter[0].nposmax = Parameterinput(&uebergabestring[0], stringlengthmax, divisionsign, &parameter[0]);
    if (parameter[0].parameterinputvis == true) {
        printf("read:	%s <= %4.3f ==> Main \n", uebergabestring, parameter[0].nposmax);
    }

    strcpy(uebergabestring, "nposmin");
    parameter[0].nposmin = Parameterinput(&uebergabestring[0], stringlengthmax, divisionsign, &parameter[0]);
    if (parameter[0].parameterinputvis == true) {
        printf("read:	%s <= %4.3f ==> Main \n", uebergabestring, parameter[0].nposmin);
    }

    strcpy(uebergabestring, "sizemagnif");
    parameter[0].sizemagnif = (int)Parameterinput(&uebergabestring[0], stringlengthmax, divisionsign, &parameter[0]);
    if (parameter[0].parameterinputvis == true) {
        printf("read:	%s <= %d ==> Main\n", uebergabestring, parameter[0].sizemagnif);
    }

    strcpy(uebergabestring, "starttrees");
    parameter[0].starttrees = (int)Parameterinput(&uebergabestring[0], stringlengthmax, divisionsign, &parameter[0]);
    if (parameter[0].parameterinputvis == true) {
        printf("read:	%s <= %d ==> Main\n", uebergabestring, parameter[0].starttrees);
    }

    strcpy(uebergabestring, "hinterland_maxlength");
    parameter[0].hinterland_maxlength = (int)Parameterinput(&uebergabestring[0], stringlengthmax, divisionsign, &parameter[0]);
    if (parameter[0].parameterinputvis == true) {
        printf("read:	%s <= %d ==> Main\n", uebergabestring, parameter[0].hinterland_maxlength);
    }

    strcpy(uebergabestring, "coneage");
    parameter[0].coneage = (int)Parameterinput(&uebergabestring[0], stringlengthmax, divisionsign, &parameter[0]);
    if (parameter[0].parameterinputvis == true) {
        printf("read:	%s <= %d ==> Main\n", uebergabestring, parameter[0].coneage);
    }

    strcpy(uebergabestring, "seedflightrate");
    parameter[0].seedflightrate = Parameterinput(&uebergabestring[0], stringlengthmax, divisionsign, &parameter[0]);
    if (parameter[0].parameterinputvis == true) {
        printf("read:	%s <= %4.3f ==> Main \n", uebergabestring, parameter[0].seedflightrate);
    }

    strcpy(uebergabestring, "dispersalmode");
    parameter[0].dispersalmode = (int)Parameterinput(&uebergabestring[0], stringlengthmax, divisionsign, &parameter[0]);
    if (parameter[0].parameterinputvis == true) {
        printf("read:	%s <= %d ==> Main\n", uebergabestring, parameter[0].dispersalmode);
    }

    strcpy(uebergabestring, "seedtravelbreezeg");
    parameter[0].seedtravelbreezeg = Parameterinput(&uebergabestring[0], stringlengthmax, divisionsign, &parameter[0]);
    if (parameter[0].parameterinputvis == true) {
        printf("read:	%s <= %4.1f ==> Main\n", uebergabestring, parameter[0].seedtravelbreezeg);
    }

    strcpy(uebergabestring, "seedtravelbreezes");
    parameter[0].seedtravelbreezes = Parameterinput(&uebergabestring[0], stringlengthmax, divisionsign, &parameter[0]);
    if (parameter[0].parameterinputvis == true) {
        printf("read:	%s <= %4.1f ==> Main\n", uebergabestring, parameter[0].seedtravelbreezes);
    }

    strcpy(uebergabestring, "seeddescentg");
    parameter[0].seeddescentg = Parameterinput(&uebergabestring[0], stringlengthmax, divisionsign, &parameter[0]);
    if (parameter[0].parameterinputvis == true) {
        printf("read:	%s <= %4.2f ==> Main\n", uebergabestring, parameter[0].seeddescentg);
    }

    strcpy(uebergabestring, "seeddescents");
    parameter[0].seeddescents = Parameterinput(&uebergabestring[0], stringlengthmax, divisionsign, &parameter[0]);
    if (parameter[0].parameterinputvis == true) {
        printf("read:	%s <= %4.2f ==> Main\n", uebergabestring, parameter[0].seeddescents);
    }

    strcpy(uebergabestring, "distanceratio");
    parameter[0].distanceratio = Parameterinput(&uebergabestring[0], stringlengthmax, divisionsign, &parameter[0]);
    if (parameter[0].parameterinputvis == true) {
        printf("read:	%s <= %4.3f ==> Main \n", uebergabestring, parameter[0].mortbg);
    }

    strcpy(uebergabestring, "seedprodfactor");
    parameter[0].seedprodfactor = Parameterinput(&uebergabestring[0], stringlengthmax, divisionsign, &parameter[0]);
    if (parameter[0].parameterinputvis == true) {
        printf("read:	%s <= %4.3f ==> Main\n", uebergabestring, parameter[0].seedprodfactor);
    }

    strcpy(uebergabestring, "germinationrate");
    parameter[0].germinationrate = Parameterinput(&uebergabestring[0], stringlengthmax, divisionsign, &parameter[0]);
    if (parameter[0].parameterinputvis == true) {
        printf("read:	%s <= %4.4f ==> Main \n", uebergabestring, parameter[0].germinationrate);
    }

    strcpy(uebergabestring, "germinatioweatherinfluence");
    parameter[0].germinatioweatherinfluence = Parameterinput(&uebergabestring[0], stringlengthmax, divisionsign, &parameter[0]);
    if (parameter[0].parameterinputvis == true) {
        printf("read:	%s <= %4.3f ==> Main \n", uebergabestring, parameter[0].germinatioweatherinfluence);
    }

    strcpy(uebergabestring, "realseedconnect");
    if (((int)Parameterinput(&uebergabestring[0], stringlengthmax, divisionsign, &parameter[0])) == 1) {
        parameter[0].realseedconnect = true;
    } else {
        parameter[0].realseedconnect = false;
    };

    if (parameter[0].parameterinputvis == true) {
        printf("read:	%s <= %s ==> Main \n", uebergabestring, (parameter[0].realseedconnect) ? "true" : "false");
    }

    strcpy(uebergabestring, "seedwinddispersalmode");
    parameter[0].seedwinddispersalmode = (int)(Parameterinput(&uebergabestring[0], stringlengthmax, divisionsign, &parameter[0]));
    if (parameter[0].parameterinputvis == true) {
        printf("read:	%s <= %d ==> Main\n", uebergabestring, parameter[0].seedwinddispersalmode);
    }

    strcpy(uebergabestring, "seedintro");
    if (((int)Parameterinput(&uebergabestring[0], stringlengthmax, divisionsign, &parameter[0])) == 1) {
        parameter[0].seedintro = true;
    } else {
        parameter[0].seedintro = false;
    };
    if (parameter[0].parameterinputvis == true) {
        printf("read:	%s <= %s ==> Main \n", uebergabestring, (parameter[0].seedintro) ? "true" : "false");
    }

    strcpy(uebergabestring, "seedintropermanent");
    if (((int)Parameterinput(&uebergabestring[0], stringlengthmax, divisionsign, &parameter[0])) == 1) {
        parameter[0].seedintropermanent = true;
    } else {
        parameter[0].seedintropermanent = false;
    };
    if (parameter[0].parameterinputvis == true) {
        printf("read:	%s <= %s ==> Main \n", uebergabestring, (parameter[0].seedintropermanent) ? "true" : "false");
    }

    strcpy(uebergabestring, "seedintronumber");
    parameter[0].seedintronumber = (int)(Parameterinput(&uebergabestring[0], stringlengthmax, divisionsign, &parameter[0]));
	parameter[0].seedintronumber = (int) parameter[0].seedintronumber * ((treerows/100)*(treecols/100)); // per ha
    if (parameter[0].parameterinputvis == true) {
        printf("read:	%s <= %d ==> Main\n", uebergabestring, parameter[0].seedintronumber);
    }

    strcpy(uebergabestring, "seedintronumberpermanent");
    parameter[0].seedintronumberpermanent = (int)(Parameterinput(&uebergabestring[0], stringlengthmax, divisionsign, &parameter[0]));
	parameter[0].seedintronumberpermanent = (int) parameter[0].seedintronumberpermanent * ((treerows/100)*(treecols/100)); // per ha
    if (parameter[0].parameterinputvis == true) {
        printf("read:	%s <= %d ==> Main\n", uebergabestring, parameter[0].seedintronumberpermanent);
    }

    strcpy(uebergabestring, "seedintro_miny");
    parameter[0].seedintro_miny = (unsigned int)(Parameterinput(&uebergabestring[0], stringlengthmax, divisionsign, &parameter[0]));
    if (parameter[0].parameterinputvis == true) {
        printf("read:	%s <= %d ==> Main\n", uebergabestring, parameter[0].seedintro_maxy);
    }
    strcpy(uebergabestring, "seedintro_maxy");
    parameter[0].seedintro_maxy = (unsigned int)(Parameterinput(&uebergabestring[0], stringlengthmax, divisionsign, &parameter[0]));
    if (parameter[0].parameterinputvis == true) {
        printf("read:	%s <= %d ==> Main\n", uebergabestring, parameter[0].seedintro_maxy);
    }
    strcpy(uebergabestring, "seedintro_minx");
    parameter[0].seedintro_minx = (unsigned int)(Parameterinput(&uebergabestring[0], stringlengthmax, divisionsign, &parameter[0]));
    if (parameter[0].parameterinputvis == true) {
        printf("read:	%s <= %d ==> Main\n", uebergabestring, parameter[0].seedintro_maxx);
    }
    strcpy(uebergabestring, "seedintro_maxx");
    parameter[0].seedintro_maxx = (unsigned int)(Parameterinput(&uebergabestring[0], stringlengthmax, divisionsign, &parameter[0]));
    if (parameter[0].parameterinputvis == true) {
        printf("read:	%s <= %d ==> Main\n", uebergabestring, parameter[0].seedintro_maxx);
    }

    strcpy(uebergabestring, "yearswithseedintro");
    parameter[0].yearswithseedintro = (int)(Parameterinput(&uebergabestring[0], stringlengthmax, divisionsign, &parameter[0]));
    if (parameter[0].parameterinputvis == true) {
        printf("read:	%s <= %d ==> Main\n", uebergabestring, parameter[0].yearswithseedintro);
    }

    strcpy(uebergabestring, "seedtravelbetween");
    if (((int)Parameterinput(&uebergabestring[0], stringlengthmax, divisionsign, &parameter[0])) == 1) {
        parameter[0].seedtravelbetween = true;
    } else {
        parameter[0].seedtravelbetween = false;
    };
    if (parameter[0].parameterinputvis == true) {
        printf("read:	%s <= %s ==> Main \n", uebergabestring, (parameter[0].seedtravelbetween) ? "true" : "false");
    }

    if (parameter[0].parameterinputvis == true) {
        printf("\n\n\t Growth\n\n");
    }

    strcpy(uebergabestring, "gdbasalfacqgmel");
    parameter[0].gdbasalfacqgmel = Parameterinput(&uebergabestring[0], stringlengthmax, divisionsign, &parameter[0]);
    if (parameter[0].parameterinputvis == true) {
        printf("read:	%s <= %4.3f ==> Main \n", uebergabestring, parameter[0].gdbasalfacqgmel);
    }

    strcpy(uebergabestring, "gdbasalfacgmel");
    parameter[0].gdbasalfacgmel = Parameterinput(&uebergabestring[0], stringlengthmax, divisionsign, &parameter[0]);
    if (parameter[0].parameterinputvis == true) {
        printf("read:	%s <= %4.3f ==> Main \n", uebergabestring, parameter[0].gdbasalfacgmel);
    }

    strcpy(uebergabestring, "gdbasalconstgmel");
    parameter[0].gdbasalconstgmel = Parameterinput(&uebergabestring[0], stringlengthmax, divisionsign, &parameter[0]);
    if (parameter[0].parameterinputvis == true) {
        printf("read:	%s <= %4.3f ==> Main \n", uebergabestring, parameter[0].gdbasalconstgmel);
    }

    strcpy(uebergabestring, "gdbreastfacqgmel");
    parameter[0].gdbreastfacqgmel = Parameterinput(&uebergabestring[0], stringlengthmax, divisionsign, &parameter[0]);
    if (parameter[0].parameterinputvis == true) {
        printf("read:	%s <= %4.4f ==> Main \n", uebergabestring, parameter[0].gdbreastfacqgmel);
    }

    strcpy(uebergabestring, "gdbreastfacgmel");
    parameter[0].gdbreastfacgmel = Parameterinput(&uebergabestring[0], stringlengthmax, divisionsign, &parameter[0]);
    if (parameter[0].parameterinputvis == true) {
        printf("read:	%s <= %4.3f ==> Main \n", uebergabestring, parameter[0].gdbreastfacgmel);
    }

    strcpy(uebergabestring, "gdbreastconstgmel");
    parameter[0].gdbreastconstgmel = Parameterinput(&uebergabestring[0], stringlengthmax, divisionsign, &parameter[0]);
    if (parameter[0].parameterinputvis == true) {
        printf("read:	%s <= %4.3f ==> Main \n", uebergabestring, parameter[0].gdbreastconstgmel);
    }

    strcpy(uebergabestring, "gdbasalfacqsib");
    parameter[0].gdbasalfacqsib = Parameterinput(&uebergabestring[0], stringlengthmax, divisionsign, &parameter[0]);
    if (parameter[0].parameterinputvis == true) {
        printf("read:	%s <= %4.3f ==> Main \n", uebergabestring, parameter[0].gdbasalfacqsib);
    }

    strcpy(uebergabestring, "gdbasalfacsib");
    parameter[0].gdbasalfacsib = Parameterinput(&uebergabestring[0], stringlengthmax, divisionsign, &parameter[0]);
    if (parameter[0].parameterinputvis == true) {
        printf("read:	%s <= %4.3f ==> Main \n", uebergabestring, parameter[0].gdbasalfacsib);
    }

    strcpy(uebergabestring, "gdbasalconstsib");
    parameter[0].gdbasalconstsib = Parameterinput(&uebergabestring[0], stringlengthmax, divisionsign, &parameter[0]);
    if (parameter[0].parameterinputvis == true) {
        printf("read:	%s <= %4.3f ==> Main \n", uebergabestring, parameter[0].gdbasalconstsib);
    }

    strcpy(uebergabestring, "gdbreastfacqsib");
    parameter[0].gdbreastfacqsib = Parameterinput(&uebergabestring[0], stringlengthmax, divisionsign, &parameter[0]);
    if (parameter[0].parameterinputvis == true) {
        printf("read:	%s <= %4.3f ==> Main \n", uebergabestring, parameter[0].gdbreastfacqsib);
    }

    strcpy(uebergabestring, "gdbreastfacsib");
    parameter[0].gdbreastfacsib = Parameterinput(&uebergabestring[0], stringlengthmax, divisionsign, &parameter[0]);
    if (parameter[0].parameterinputvis == true) {
        printf("read:	%s <= %4.3f ==> Main \n", uebergabestring, parameter[0].gdbreastfacsib);
    }

    strcpy(uebergabestring, "gdbreastconstsib");
    parameter[0].gdbreastconstsib = Parameterinput(&uebergabestring[0], stringlengthmax, divisionsign, &parameter[0]);
    if (parameter[0].parameterinputvis == true) {
        printf("read:	%s <= %4.3f ==> Main \n", uebergabestring, parameter[0].gdbreastconstsib);
    }

    strcpy(uebergabestring, "basalinfluenceoldyoung");
    parameter[0].basalinfluenceoldyoung = Parameterinput(&uebergabestring[0], stringlengthmax, divisionsign, &parameter[0]);
    if (parameter[0].parameterinputvis == true) {
        printf("read:	%s <= %4.4f ==> Main \n", uebergabestring, parameter[0].basalinfluenceoldyoung);
    }

    strcpy(uebergabestring, "relgrowthinfluence");
    parameter[0].relgrowthinfluence = (int)(Parameterinput(&uebergabestring[0], stringlengthmax, divisionsign, &parameter[0]));
    if (parameter[0].parameterinputvis == true) {
        printf("read:	%s <= %d ==> Main\n", uebergabestring, parameter[0].relgrowthinfluence);
    }

    strcpy(uebergabestring, "densitymode");
    parameter[0].densitymode = (int)(Parameterinput(&uebergabestring[0], stringlengthmax, divisionsign, &parameter[0]));
    if (parameter[0].parameterinputvis == true) {
        printf("read:	%s <= %d ==> Main\n", uebergabestring, parameter[0].densitymode);
    }

    strcpy(uebergabestring, "incfac");
    parameter[0].incfac = Parameterinput(&uebergabestring[0], stringlengthmax, divisionsign, &parameter[0]);
    if (parameter[0].parameterinputvis == true) {
        printf("read:	%s <= %4.3f ==> Main \n", uebergabestring, parameter[0].incfac);
    }

    strcpy(uebergabestring, "densityvaluemanipulatorexp");
    parameter[0].densityvaluemanipulatorexp = Parameterinput(&uebergabestring[0], stringlengthmax, divisionsign, &parameter[0]);
    if (parameter[0].parameterinputvis == true) {
        printf("read:	%s <= %4.3f ==> Main \n", uebergabestring, parameter[0].densityvaluemanipulatorexp);
    }

    strcpy(uebergabestring, "calcinfarea");
    parameter[0].calcinfarea = (int)(Parameterinput(&uebergabestring[0], stringlengthmax, divisionsign, &parameter[0]));
    if (parameter[0].parameterinputvis == true) {
        printf("read:	%s <= %d ==> Main\n", uebergabestring, parameter[0].calcinfarea);
    }

    strcpy(uebergabestring, "densitysmallweighing");
    parameter[0].densitysmallweighing = Parameterinput(&uebergabestring[0], stringlengthmax, divisionsign, &parameter[0]);
    if (parameter[0].parameterinputvis == true) {
        printf("read:	%s <= %4.3f ==> Main \n", uebergabestring, parameter[0].densitysmallweighing);
    }

    strcpy(uebergabestring, "densitytreetile");
    parameter[0].densitytreetile = Parameterinput(&uebergabestring[0], stringlengthmax, divisionsign, &parameter[0]);
    if (parameter[0].parameterinputvis == true) {
        printf("read:	%s <= %4.3f ==> Main \n", uebergabestring, parameter[0].densitytreetile);
    }

    strcpy(uebergabestring, "densitytiletree");
    parameter[0].densitytiletree = (int)(Parameterinput(&uebergabestring[0], stringlengthmax, divisionsign, &parameter[0]));
    if (parameter[0].parameterinputvis == true) {
        printf("read:	%s <= %d ==> Main\n", uebergabestring, parameter[0].densitytiletree);
    }

    strcpy(uebergabestring, "desitymaxreduction");
    parameter[0].desitymaxreduction = Parameterinput(&uebergabestring[0], stringlengthmax, divisionsign, &parameter[0]);
    if (parameter[0].parameterinputvis == true) {
        printf("read:	%s <= %4.3f ==> Main \n", uebergabestring, parameter[0].desitymaxreduction);
    }

    strcpy(uebergabestring, "dichtheightrel");
    parameter[0].dichtheightrel = (int)(Parameterinput(&uebergabestring[0], stringlengthmax, divisionsign, &parameter[0]));
    if (parameter[0].parameterinputvis == true) {
        printf("read:	%s <= %d ==> Main\n", uebergabestring, parameter[0].dichtheightrel);
    }

    strcpy(uebergabestring, "densityvaluedbasalinfluence");
    parameter[0].densityvaluedbasalinfluence = Parameterinput(&uebergabestring[0], stringlengthmax, divisionsign, &parameter[0]);
    if (parameter[0].parameterinputvis == true) {
        printf("read:	%s <= %4.3f ==> Main \n", uebergabestring, parameter[0].densityvaluedbasalinfluence);
    }

    strcpy(uebergabestring, "densityvaluemaximumatheight");
    parameter[0].densityvaluemaximumatheight = Parameterinput(&uebergabestring[0], stringlengthmax, divisionsign, &parameter[0]);
    if (parameter[0].parameterinputvis == true) {
        printf("read:	%s <= %4.3f ==> Main \n", uebergabestring, parameter[0].densityvaluemaximumatheight);
    }

    strcpy(uebergabestring, "allometryfunctiontype");
    parameter[0].allometryfunctiontype = (int)Parameterinput(&uebergabestring[0], stringlengthmax, divisionsign, &parameter[0]);
    if (parameter[0].parameterinputvis == true) {
        printf("read:	%s <= %d ==> Main\n", uebergabestring, parameter[0].allometryfunctiontype);
    }

    strcpy(uebergabestring, "dbasalheightalloslope");
    parameter[0].dbasalheightalloslope = Parameterinput(&uebergabestring[0], stringlengthmax, divisionsign, &parameter[0]);
    if (parameter[0].parameterinputvis == true) {
        printf("read:	%s <= %4.4f ==> Main \n", uebergabestring, parameter[0].dbasalheightalloslope);
    }

    strcpy(uebergabestring, "dbasalheightalloexp");
    parameter[0].dbasalheightalloexp = Parameterinput(&uebergabestring[0], stringlengthmax, divisionsign, &parameter[0]);
    if (parameter[0].parameterinputvis == true) {
        printf("read:	%s <= %4.4f ==> Main \n", uebergabestring, parameter[0].dbasalheightalloexp);
    }

    strcpy(uebergabestring, "dbreastheightalloslope");
    parameter[0].dbreastheightalloslope = Parameterinput(&uebergabestring[0], stringlengthmax, divisionsign, &parameter[0]);
    if (parameter[0].parameterinputvis == true) {
        printf("read:	%s <= %4.4f ==> Main \n", uebergabestring, parameter[0].dbreastheightalloslope);
    }

    strcpy(uebergabestring, "dbreastheightalloexp");
    parameter[0].dbreastheightalloexp = Parameterinput(&uebergabestring[0], stringlengthmax, divisionsign, &parameter[0]);
    if (parameter[0].parameterinputvis == true) {
        printf("read:	%s <= %4.4f ==> Main \n", uebergabestring, parameter[0].dbreastheightalloexp);
    }

    strcpy(uebergabestring, "dbasalheightslopenonlin");
    parameter[0].dbasalheightslopenonlin = Parameterinput(&uebergabestring[0], stringlengthmax, divisionsign, &parameter[0]);
    if (parameter[0].parameterinputvis == true) {
        printf("read:	%s <= %4.4f ==> Main \n", uebergabestring, parameter[0].dbasalheightslopenonlin);
    }

    strcpy(uebergabestring, "dbreastheightslopenonlin");
    parameter[0].dbreastheightslopenonlin = Parameterinput(&uebergabestring[0], stringlengthmax, divisionsign, &parameter[0]);
    if (parameter[0].parameterinputvis == true) {
        printf("read:	%s <= %4.4f ==> Main \n", uebergabestring, parameter[0].dbreastheightslopenonlin);
    }

    strcpy(uebergabestring, "mortbg");
    parameter[0].mortbg = Parameterinput(&uebergabestring[0], stringlengthmax, divisionsign, &parameter[0]);
    if (parameter[0].parameterinputvis == true) {
        printf("read:	%s <= %4.3f ==> Main \n", uebergabestring, parameter[0].mortbg);
    }

    strcpy(uebergabestring, "maximumage");
    parameter[0].maximumage = (int)Parameterinput(&uebergabestring[0], stringlengthmax, divisionsign, &parameter[0]);
    if (parameter[0].parameterinputvis == true) {
        printf("read:	%s <= %d ==> Main\n", uebergabestring, parameter[0].maximumage);
    }

    strcpy(uebergabestring, "mortage");
    parameter[0].mortage = Parameterinput(&uebergabestring[0], stringlengthmax, divisionsign, &parameter[0]);
    if (parameter[0].parameterinputvis == true) {
        printf("read:	%s <= %4.4f ==> Main \n", uebergabestring, parameter[0].mortage);
    }

    strcpy(uebergabestring, "mortyouth");
    parameter[0].mortyouth = Parameterinput(&uebergabestring[0], stringlengthmax, divisionsign, &parameter[0]);
    if (parameter[0].parameterinputvis == true) {
        printf("read:	%s <= %4.4f ==> Main \n", uebergabestring, parameter[0].mortyouth);
    }

    strcpy(uebergabestring, "mortyouthinfluenceexp");
    parameter[0].mortyouthinfluenceexp = Parameterinput(&uebergabestring[0], stringlengthmax, divisionsign, &parameter[0]);
    if (parameter[0].parameterinputvis == true) {
        printf("read:	%s <= %4.4f ==> Main \n", uebergabestring, parameter[0].mortyouthinfluenceexp);
    }

    strcpy(uebergabestring, "mgrowth");
    parameter[0].mgrowth = Parameterinput(&uebergabestring[0], stringlengthmax, divisionsign, &parameter[0]);
    if (parameter[0].parameterinputvis == true) {
        printf("read:	%s <= %4.4f ==> Main \n", uebergabestring, parameter[0].mgrowth);
    }

    strcpy(uebergabestring, "relgrowthmortinfluenceexp");
    parameter[0].relgrowthmortinfluenceexp = Parameterinput(&uebergabestring[0], stringlengthmax, divisionsign, &parameter[0]);
    if (parameter[0].parameterinputvis == true) {
        printf("read:	%s <= %4.4f ==> Main \n", uebergabestring, parameter[0].relgrowthmortinfluenceexp);
    }

    strcpy(uebergabestring, "mweather");
    parameter[0].mweather = Parameterinput(&uebergabestring[0], stringlengthmax, divisionsign, &parameter[0]);
    if (parameter[0].parameterinputvis == true) {
        printf("read:	%s <= %4.4f ==> Main \n", uebergabestring, parameter[0].mweather);
    }

    strcpy(uebergabestring, "heightweathermorteinflussexp");
    parameter[0].heightweathermorteinflussexp = Parameterinput(&uebergabestring[0], stringlengthmax, divisionsign, &parameter[0]);
    if (parameter[0].parameterinputvis == true) {
        printf("read:	%s <= %4.4f ==> Main \n", uebergabestring, parameter[0].heightweathermorteinflussexp);
    }

    strcpy(uebergabestring, "mdensity");
    parameter[0].mdensity = Parameterinput(&uebergabestring[0], stringlengthmax, divisionsign, &parameter[0]);
    if (parameter[0].parameterinputvis == true) {
        printf("read:	%s <= %4.4f ==> Main \n", uebergabestring, parameter[0].mdensity);
    }

    strcpy(uebergabestring, "mdrought");
    parameter[0].mdrought = Parameterinput(&uebergabestring[0], stringlengthmax, divisionsign, &parameter[0]);
    if (parameter[0].parameterinputvis == true) {
        printf("read:	%s <= %4.4f ==> Main \n", uebergabestring, parameter[0].mdrought);
    }

    strcpy(uebergabestring, "seedconemort");
    parameter[0].seedconemort = Parameterinput(&uebergabestring[0], stringlengthmax, divisionsign, &parameter[0]);
    if (parameter[0].parameterinputvis == true) {
        printf("read:	%s <= %4.4f ==> Main \n", uebergabestring, parameter[0].seedconemort);
    }

    strcpy(uebergabestring, "seedfloormort");
    parameter[0].seedfloormort = Parameterinput(&uebergabestring[0], stringlengthmax, divisionsign, &parameter[0]);
    if (parameter[0].parameterinputvis == true) {
        printf("read:	%s <= %4.4f ==> Main \n", uebergabestring, parameter[0].seedfloormort);
    }

    strcpy(uebergabestring, "gmelseedmaxage");
    parameter[0].gmelseedmaxage = (int)Parameterinput(&uebergabestring[0], stringlengthmax, divisionsign, &parameter[0]);
    if (parameter[0].parameterinputvis == true) {
        printf("read:	%s <= %d ==> Main\n", uebergabestring, parameter[0].gmelseedmaxage);
    }

    strcpy(uebergabestring, "janthresholdtempgmel");
    parameter[0].janthresholdtempgmel = Parameterinput(&uebergabestring[0], stringlengthmax, divisionsign, &parameter[0]);
    if (parameter[0].parameterinputvis == true) {
        printf("read:	%s <= %4.2f ==> Main\n", uebergabestring, parameter[0].janthresholdtempgmel);
    }

    strcpy(uebergabestring, "weathervariablegmela");
    parameter[0].weathervariablegmela = Parameterinput(&uebergabestring[0], stringlengthmax, divisionsign, &parameter[0]);
    if (parameter[0].parameterinputvis == true) {
        printf("read:	%s <= %4.3f ==> Main\n", uebergabestring, parameter[0].weathervariablegmela);
    }

    strcpy(uebergabestring, "weathervariablegmelb");
    parameter[0].weathervariablegmelb = Parameterinput(&uebergabestring[0], stringlengthmax, divisionsign, &parameter[0]);
    if (parameter[0].parameterinputvis == true) {
        printf("read:	%s <= %4.3f ==> Main\n", uebergabestring, parameter[0].weathervariablegmelb);
    }

    strcpy(uebergabestring, "weathervariablegmelc");
    parameter[0].weathervariablegmelc = Parameterinput(&uebergabestring[0], stringlengthmax, divisionsign, &parameter[0]);
    if (parameter[0].parameterinputvis == true) {
        printf("read:	%s <= %4.3f ==> Main\n", uebergabestring, parameter[0].weathervariablegmelc);
    }

    strcpy(uebergabestring, "windsource");
    parameter[0].windsource = (int)Parameterinput(&uebergabestring[0], stringlengthmax, divisionsign, &parameter[0]);
    if (parameter[0].parameterinputvis == true) {
        printf("read:	%s <= %d ==> Main\n", uebergabestring, parameter[0].windsource);
    }

    strcpy(uebergabestring, "boundaryconditions");
    parameter[0].boundaryconditions = (int)Parameterinput(&uebergabestring[0], stringlengthmax, divisionsign, &parameter[0]);
    if (parameter[0].parameterinputvis == true) {
        printf("read:	%s <= %d ==> Main\n", uebergabestring, parameter[0].boundaryconditions);
    }

    strcpy(uebergabestring, "outputall");
    parameter[0].outputall = (int)Parameterinput(&uebergabestring[0], stringlengthmax, divisionsign, &parameter[0]);
    if (parameter[0].parameterinputvis == true) {
        printf("read:	%s <= %d ==> Main\n", uebergabestring, parameter[0].outputall);
    }

    strcpy(uebergabestring, "pollination");
    parameter[0].pollination = (int)Parameterinput(&uebergabestring[0], stringlengthmax, divisionsign, &parameter[0]);
    if (parameter[0].parameterinputvis == true) {
        printf("read:	%s <= %d ==> Main\n", uebergabestring, parameter[0].pollination);
    }

    strcpy(uebergabestring, "pollendirectionvariance");
    parameter[0].pollendirectionvariance = (double)Parameterinput(&uebergabestring[0], stringlengthmax, divisionsign, &parameter[0]);
    if (parameter[0].parameterinputvis == true) {
        printf("read:	%s <= %4.3f ==> Main\n", uebergabestring, parameter[0].pollendirectionvariance);
    }

    strcpy(uebergabestring, "pollenfall");
    parameter[0].pollenfall = (double)Parameterinput(&uebergabestring[0], stringlengthmax, divisionsign, &parameter[0]);

    strcpy(uebergabestring, "pollengregoryc");
    parameter[0].pollengregoryc = (double)Parameterinput(&uebergabestring[0], stringlengthmax, divisionsign, &parameter[0]);

    strcpy(uebergabestring, "pollengregorym");
    parameter[0].pollengregorym = (double)Parameterinput(&uebergabestring[0], stringlengthmax, divisionsign, &parameter[0]);

    strcpy(uebergabestring, "snowcomputation");
    if (((int)Parameterinput(&uebergabestring[0], stringlengthmax, divisionsign, &parameter[0])) == 1) {
        parameter[0].snowcomputation = true;
    } else {
        parameter[0].snowcomputation = false;
    };
    if (parameter[0].parameterinputvis == true) {
        printf("read:	%s <= %s ==> Main \n", uebergabestring, (parameter[0].weathercalcgradient) ? "true" : "false");
    }


    // further variables
    parameter[0].tempdiffort = 0.0;
    parameter[0].precdiffort = 0.0;
	parameter[0].litterlayerburn_mod = 1.0;
	parameter[0].fireimpactareasize_mod = 1.0;
	parameter[0].envirgrowthimpacttree_mod = 1.0;										   
}

void Getspeciestraits(void) {	
	// first trait information
	FILE *f;
	f = fopen("specieslist.csv","r"); 
	if (f == NULL) {
		printf("speciestrait.csv file not available!\n");
		exit(1);
	}
	
	char puffer[1255];
	int counter=0;
	

	// read in line by line
	while( fgets(puffer,1255,f) !=NULL ) {
		if (counter>=1) {
			speciestrait[counter].number= strtod(strtok(puffer, " "),NULL);
			speciestrait[counter].species= strtok(NULL, " ");
			speciestrait[counter].roi= strtod(strtok(NULL, " "),NULL);
			speciestrait[counter].coneage= strtod(strtok(NULL, " "),NULL) ;
			speciestrait[counter].seedflightrate= strtod(strtok(NULL, " "),NULL) ;
			speciestrait[counter].seedtravelbreeze= strtod(strtok(NULL, " "),NULL) ;
			speciestrait[counter].seeddescent= strtod(strtok(NULL, " "),NULL) ;
			speciestrait[counter].distanceratio= strtod(strtok(NULL, " "),NULL) ;
			speciestrait[counter].seedprodfactor= strtod(strtok(NULL, " "),NULL) ;
			speciestrait[counter].germinationrate= strtod(strtok(NULL, " "),NULL) ;
			speciestrait[counter].germinationweatherinfluence= strtod(strtok(NULL, " "),NULL) ;
			speciestrait[counter].mindiametergrowth= strtod(strtok(NULL, " "),NULL) ;
			speciestrait[counter].meangrowthq75p= strtod(strtok(NULL, " "),NULL) ;																	  
			speciestrait[counter].gdbasalfacq= strtod(strtok(NULL, " "),NULL) ;
			speciestrait[counter].gdbasalfac= strtod(strtok(NULL, " "),NULL) ;
			speciestrait[counter].gdbasalconst= strtod(strtok(NULL, " "),NULL) ;
			speciestrait[counter].gdbreastfacq= strtod(strtok(NULL, " "),NULL) ;
			speciestrait[counter].gdbreastfac= strtod(strtok(NULL, " "),NULL) ;
			speciestrait[counter].gdbreastconst= strtod(strtok(NULL, " "),NULL) ;
			speciestrait[counter].dbasalheightalloslope= strtod(strtok(NULL, " "),NULL) ;
			speciestrait[counter].dbasalheightalloexp= strtod(strtok(NULL, " "),NULL) ;
			speciestrait[counter].dbreastheightalloslope= strtod(strtok(NULL, " "),NULL) ;
			speciestrait[counter].dbreastheightalloexp= strtod(strtok(NULL, " "),NULL) ;
			speciestrait[counter].dbasalheightslopenonlin= strtod(strtok(NULL, " "),NULL) ;
			speciestrait[counter].dbreastheightslopenonlin= strtod(strtok(NULL, " "),NULL) ;
			speciestrait[counter].heightloga= strtod(strtok(NULL, " "),NULL) ;
			speciestrait[counter].heightlogb= strtod(strtok(NULL, " "),NULL) ;
			speciestrait[counter].heightlogc= strtod(strtok(NULL, " "),NULL) ;
			speciestrait[counter].mortbg= strtod(strtok(NULL, " "),NULL) ;
			speciestrait[counter].maximumage= strtod(strtok(NULL, " "),NULL) ;
			speciestrait[counter].mortage= strtod(strtok(NULL, " "),NULL) ;
			speciestrait[counter].mortyouth= strtod(strtok(NULL, " "),NULL) ;
			speciestrait[counter].mortyouthinfluenceexp= strtod(strtok(NULL, " "),NULL) ;
			speciestrait[counter].mgrowth= strtod(strtok(NULL, " "),NULL) ;
			speciestrait[counter].mdensity= strtod(strtok(NULL, " "),NULL) ;
			speciestrait[counter].densityvaluemaximumatheight= strtod(strtok(NULL, " "),NULL) ;
			speciestrait[counter].mweather= strtod(strtok(NULL, " "),NULL) ;
			speciestrait[counter].heightweathermorteinflussexp= strtod(strtok(NULL, " "),NULL) ;
			speciestrait[counter].mwindthrow= strtod(strtok(NULL, " "),NULL) ;
			speciestrait[counter].mdrought= strtod(strtok(NULL, " "),NULL) ;
			speciestrait[counter].seedconemort= strtod(strtok(NULL, " "),NULL) ;
			speciestrait[counter].seedfloormort= strtod(strtok(NULL, " "),NULL) ;
			speciestrait[counter].seedmaxage= strtod(strtok(NULL, " "),NULL) ;
			speciestrait[counter].janthresholdtemp= strtod(strtok(NULL, " "),NULL) ;
			speciestrait[counter].janthresholdtempcalcvalue= strtod(strtok(NULL, " "),NULL) ;
			speciestrait[counter].weathervariablea= strtod(strtok(NULL, " "),NULL) ;
			speciestrait[counter].weathervariableb= strtod(strtok(NULL, " "),NULL) ;
			speciestrait[counter].weathervariablec= strtod(strtok(NULL, " "),NULL) ;
			speciestrait[counter].weathervariabled= strtod(strtok(NULL, " "),NULL) ;
			speciestrait[counter].minactivelayer= strtod(strtok(NULL, " "),NULL) ;
			speciestrait[counter].minsoilwater= strtod(strtok(NULL, " "),NULL) ;
			speciestrait[counter].maxsoilwater= strtod(strtok(NULL, " "),NULL) ;
			speciestrait[counter].rootingdepth= strtod(strtok(NULL, " "),NULL) ;
			speciestrait[counter].relbarkthickness= strtod(strtok(NULL, " "),NULL) ;
			speciestrait[counter].resprouting= strtod(strtok(NULL, " "),NULL) ;
			speciestrait[counter].biomassleafbase= strtod(strtok(NULL, " "),NULL) ;
			speciestrait[counter].biomassleaffaca= strtod(strtok(NULL, " "),NULL) ;
			speciestrait[counter].biomassleaffacb= strtod(strtok(NULL, " "),NULL) ;
			speciestrait[counter].biomasswoodbase= strtod(strtok(NULL, " "),NULL) ;
			speciestrait[counter].biomasswoodfaca= strtod(strtok(NULL, " "),NULL) ;
			speciestrait[counter].biomasswoodfacb= strtod(strtok(NULL, " "),NULL) ;
			speciestrait[counter].lightdemand= strtod(strtok(NULL, " "),NULL) ;
			speciestrait[counter].biomasswoodongree= strtod(strtok(NULL, " "),NULL) ;
			speciestrait[counter].growthmod= 1 ;

		}
		counter++;
	}
	fclose(f);

	// second colonization times
	FILE *f2;
	if(parameter[0].roi==1) {// RU
		f2 = fopen("speciescolonizationtimes_RU.csv","r"); 
	} else if(parameter[0].roi==2) {// CA
		f2 = fopen("speciescolonizationtimes_CA.csv","r"); 
	} else if(parameter[0].roi==3) {// AK
		f2 = fopen("speciescolonizationtimes_AK.csv","r"); 
	}
	if (f2 == NULL) {
		printf("speciescolonizationtimes_AK.csv file not available!\n");
		exit(1);
	}
	
	char puffer2[1255];
	int counter2=0;
	

	// read in line by line
	while( fgets(puffer2,1255,f2) !=NULL) {
		if (counter2>=1) { // skip header line
			speciescolonizationtimes[counter2].location= stoi(strtok(puffer2, " "));
			speciescolonizationtimes[counter2].speciestimes.push_back( 0 );
			for(int i=1; i<=22; i++) {
				speciescolonizationtimes[counter2].speciestimes.push_back( stoi(strtok(NULL, " "),NULL) );
			}

cout << speciescolonizationtimes[counter2].location << " :: " << speciescolonizationtimes[counter2].speciestimes.back() << endl;
		}
		counter2++;
	}
	fclose(f2);
}

void ReadFire(void) {
	cout << "Global fire input startet." << endl;
	
	FILE *f_fire;
if(parameter[0].globalfireradius==300) {
		f_fire = fopen("fireparameters_300km.csv","r"); 
	} else if(parameter[0].globalfireradius==100) {
		f_fire = fopen("fireparameters_100km.csv","r"); 
	} else if(parameter[0].globalfireradius==50) {
		f_fire = fopen("fireparameters_50km.csv","r"); 
	}
	if (f_fire == NULL) {
		printf("fireparameters_...km.csv file not available!\n");
		exit(1);
	}
	
	char buffer[1255];
	int counter = 0;
	
	// read in line by line
	// .. only keep those needed for memory reasons
	int counter_inradius = 0;
	cout << "Reading glocal fire input ... ";
	while( fgets(buffer,1255,f_fire) != NULL ) {
		if (counter>=1) { // skip header line
			//  lat lon model_r2 model_r2_adj model_sig par1 par2 par3 par4 par5 par6 par7 par8 par9 threshold_mild
			int number_i = stoi(strtok(buffer, " "),NULL);
			double lat_i = strtod(strtok(NULL, " " ),NULL);
			double lon_i = strtod(strtok(NULL, " " ),NULL);
			//calculate distance
			double distance = 0;
			bool closelocation = false;
			if( (fabs(parameter[0].plotcentre_lat-lat_i) < parameter[0].firelocdistance) // 1 degree distance allowed and 0.5 degrees steps
				& (fabs(parameter[0].plotcentre_lon-lon_i) < parameter[0].firelocdistance)
			) {
				closelocation = true;
				counter_inradius++;
				// calculate euclidean distance
				distance = sqrt(pow(parameter[0].plotcentre_lat-lat_i,2) + pow(parameter[0].plotcentre_lon,2));
			} else {
				cout << ".";
			}
			
			if(closelocation == true) {
				//store information
				globalfireparameter[counter_inradius].number = number_i;
				globalfireparameter[counter_inradius].distance = distance;
					cout << endl << "In radius, row: " << counter_inradius << " => " << globalfireparameter[counter_inradius].number ;
				globalfireparameter[counter_inradius].latitude = lat_i;
				globalfireparameter[counter_inradius].longitude = lon_i;
					cout << " / Latitude: " << globalfireparameter[counter_inradius].latitude;
					cout << " & Longitude: " << globalfireparameter[counter_inradius].longitude << endl;
					cout << "model_r2: " << strtod(strtok(NULL, " "),NULL);
					cout << " / model_r2_adj: " << strtod(strtok(NULL, " "),NULL);
					cout << " / model_sig: " << strtod(strtok(NULL, " "),NULL) << endl;
				globalfireparameter[counter_inradius].parameter1 = strtod(strtok(NULL, " " ),NULL);
				globalfireparameter[counter_inradius].parameter2 = strtod(strtok(NULL, " " ),NULL);
				globalfireparameter[counter_inradius].parameter3 = strtod(strtok(NULL, " " ),NULL);
				globalfireparameter[counter_inradius].parameter4 = strtod(strtok(NULL, " " ),NULL);
				globalfireparameter[counter_inradius].parameter5 = strtod(strtok(NULL, " " ),NULL);
				globalfireparameter[counter_inradius].parameter6 = strtod(strtok(NULL, " " ),NULL);
				globalfireparameter[counter_inradius].parameter7 = strtod(strtok(NULL, " " ),NULL);
				globalfireparameter[counter_inradius].parameter8 = strtod(strtok(NULL, " " ),NULL);
				globalfireparameter[counter_inradius].parameter9 = strtod(strtok(NULL, " " ),NULL);
				globalfireparameter[counter_inradius].threshold_mild = strtod(strtok(NULL, " " ),NULL);
				// globalfireparameter[counter_inradius].threshold_medium = 0;
				// globalfireparameter[counter_inradius].threshold_severe = 0;
					cout << "threshold_mild: " << globalfireparameter[counter_inradius].threshold_mild << endl;
			}
		}
		counter++;
	}
	fclose(f_fire);
	
	// process read data
	// ... get mean parameters of all and store in location 0 as 1ff are the read in parameters
	// ... calculate sum of distances for weight calculation
	double sum_distance = 0.0;
	for(int counter_inradius_i = 1; counter_inradius_i <= counter_inradius; counter_inradius_i++) {
		sum_distance += globalfireparameter[counter_inradius_i].distance;
	}
	double sum_distance_perm = 0.0;
	for(int counter_inradius_i = 1; counter_inradius_i <= counter_inradius; counter_inradius_i++) {
		sum_distance_perm += sum_distance/globalfireparameter[counter_inradius_i].distance;
	}
	cout << " ... sum_distance = " << sum_distance << endl;
		globalfireparameter[0].parameter1 = 0.0;
		globalfireparameter[0].parameter2 = 0.0;
		globalfireparameter[0].parameter3 = 0.0;
		globalfireparameter[0].parameter4 = 0.0;
		globalfireparameter[0].parameter5 = 0.0;
		globalfireparameter[0].parameter6 = 0.0;
		globalfireparameter[0].parameter7 = 0.0;
		globalfireparameter[0].parameter8 = 0.0;
		globalfireparameter[0].parameter9 = 0.0;
		globalfireparameter[0].threshold_mild = 0.0;
	for(int counter_inradius_i = 1; counter_inradius_i <= counter_inradius; counter_inradius_i++) {
		double weight_i = ((sum_distance/globalfireparameter[counter_inradius_i].distance)/sum_distance_perm);
		globalfireparameter[0].parameter1 += globalfireparameter[counter_inradius_i].parameter1 * weight_i;
		globalfireparameter[0].parameter2 += globalfireparameter[counter_inradius_i].parameter2 * weight_i;
		globalfireparameter[0].parameter3 += globalfireparameter[counter_inradius_i].parameter3 * weight_i;
		globalfireparameter[0].parameter4 += globalfireparameter[counter_inradius_i].parameter4 * weight_i;
		globalfireparameter[0].parameter5 += globalfireparameter[counter_inradius_i].parameter5 * weight_i;
		globalfireparameter[0].parameter6 += globalfireparameter[counter_inradius_i].parameter6 * weight_i;
		globalfireparameter[0].parameter7 += globalfireparameter[counter_inradius_i].parameter7 * weight_i;
		globalfireparameter[0].parameter8 += globalfireparameter[counter_inradius_i].parameter8 * weight_i;
		globalfireparameter[0].parameter9 += globalfireparameter[counter_inradius_i].parameter9 * weight_i;
		if(globalfireparameter[counter_inradius_i].threshold_mild == 99999) {
			globalfireparameter[counter_inradius_i].threshold_mild = 0.0;
		}
		globalfireparameter[0].threshold_mild += globalfireparameter[counter_inradius_i].threshold_mild * weight_i;
	}
	
} // End function ReadFire
