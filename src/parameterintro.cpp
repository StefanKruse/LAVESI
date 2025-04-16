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
	cout << "Global fire input started." << endl;
	
	// get current location coordinates if in parameters not specified (==0)
	if( (parameter[0].plotcentre_lat==0) & (parameter[0].plotcentre_lon==0) ) {
		long int plotcodeNum;
		plotcodeNum = parameter[0].weatherchoice % 10000;
		std::stringstream plotcode;
		plotcode << plotcodeNum;
		
        // define input folder
		if (plotcodeNum == 1001) /* ### FOR 11-CH-02II ### */  {
			parameter[0].plotcentre_lat=71.83993;
			parameter[0].plotcentre_lon=102.88387;
		} else if (plotcodeNum == 1002) /* ### FOR 11-CH-02III ### */  {
			parameter[0].plotcentre_lat=71.84179;
			parameter[0].plotcentre_lon=102.87589;
		} else if (plotcodeNum == 1003) /* ### FOR 11-CH-06I ### */  {
			parameter[0].plotcentre_lat=70.66915;
			parameter[0].plotcentre_lon=97.7121;
		} else if (plotcodeNum == 1004) /* ### FOR 11-CH-06III ### */ {
			parameter[0].plotcentre_lat=70.66498;
			parameter[0].plotcentre_lon=97.7064;
		} else if (plotcodeNum == 1005) /* ### FOR 11-CH-12I ### */  {
			parameter[0].plotcentre_lat=72.3938;
			parameter[0].plotcentre_lon=102.30144;
		} else if (plotcodeNum == 1006) /* ### FOR 11-CH-12II ### */  {
			parameter[0].plotcentre_lat=72.40009;
			parameter[0].plotcentre_lon=102.28725;
		} else if (plotcodeNum == 1007) /* ### FOR 11-CH-17I ### */  {
			parameter[0].plotcentre_lat=72.24235;
			parameter[0].plotcentre_lon=102.24565;
		} else if (plotcodeNum == 1008) /* ### FOR 11-CH-17II ### */  {
			parameter[0].plotcentre_lat=72.24144;
			parameter[0].plotcentre_lon=102.22661;
		} else if (plotcodeNum == 1009) /* ### FOR 12-KO-02a ### */  {
			parameter[0].plotcentre_lat=68.38916;
			parameter[0].plotcentre_lon=161.466171;
		} else if (plotcodeNum == 1010) /* ### FOR 12-KO-02b ### */  {
			parameter[0].plotcentre_lat=68.389936;
			parameter[0].plotcentre_lon=161.448985;
		} else if (plotcodeNum == 1011) /* ### FOR 12-KO-03a ### */  {
			parameter[0].plotcentre_lat=68.516169;
			parameter[0].plotcentre_lon=161.18194;
		} else if (plotcodeNum == 1012) /* ### FOR 12-KO-03b ### */  {
			parameter[0].plotcentre_lat=68.513173;
			parameter[0].plotcentre_lon=161.195505;
		} else if (plotcodeNum == 1013) /* ### FOR 12-KO-04a ### */  {
			parameter[0].plotcentre_lat=69.051323;
			parameter[0].plotcentre_lon=161.206493;
		} else if (plotcodeNum == 1014) /* ### FOR 12-KO-04b ### */  {
			parameter[0].plotcentre_lat=69.05362;
			parameter[0].plotcentre_lon=161.205179;
		} else if (plotcodeNum == 1015) /* ### FOR 12-KO-05 ### */ {
			parameter[0].plotcentre_lat=69.11836;
			parameter[0].plotcentre_lon=161.02342;
		} else if (plotcodeNum == 1016) /* ### FOR 13-TY-02-VI ### */  {
			parameter[0].plotcentre_lat=72.54772;
			parameter[0].plotcentre_lon=105.7316;
		} else if (plotcodeNum == 1017) /* ### FOR 13-TY-02-VII ### */  {
			parameter[0].plotcentre_lat=72.54884;
			parameter[0].plotcentre_lon=105.74576;
		} else if (plotcodeNum == 1018) /* ### FOR 14-OM-02-V1 ### */  {
			parameter[0].plotcentre_lat=70.74418;
			parameter[0].plotcentre_lon=132.698523;
		} else if (plotcodeNum == 1019) /* ### FOR 14-OM-02-V2 ### */  {
			parameter[0].plotcentre_lat=70.72644;
			parameter[0].plotcentre_lon=132.658169;
		} else if (plotcodeNum == 1020) /* ### FOR 14-OM-11-V3 ### */  {
			parameter[0].plotcentre_lat=70.957883;
			parameter[0].plotcentre_lon=132.570074;
		} else if (plotcodeNum == 1021) /* ### FOR 14-OM-20-V4 ### */  {
			parameter[0].plotcentre_lat=70.526707;
			parameter[0].plotcentre_lon=132.914259;
		} else if (plotcodeNum == 1022) /* ### FOR 14-OM-TRANS1 ### */  {
			parameter[0].plotcentre_lat=70.943542;
			parameter[0].plotcentre_lon=132.777408;
		} else if (plotcodeNum == 1023) /* ### FOR 14-OM-TRANS2 ### */  {
			parameter[0].plotcentre_lat=70.939004;
			parameter[0].plotcentre_lon=132.790487;
		} else if (plotcodeNum == 1024) /* ### FOR 14-OM-TRANS3 ### */  {
			parameter[0].plotcentre_lat=70.935714;
			parameter[0].plotcentre_lon=132.820357;
		} else if (plotcodeNum == 1025) /* ### FOR 14-OM-TRANS4 ### */  {
			parameter[0].plotcentre_lat=70.93332;
			parameter[0].plotcentre_lon=132.854538;
		} else if (plotcodeNum == 1026) /* ### FOR 14-OM-TRANS5 ### */  {
			parameter[0].plotcentre_lat=70.935817;
			parameter[0].plotcentre_lon=132.868951;
		} else if (plotcodeNum == 1028) /* ### FOR 14-OM-TRANS6 ### */  {
			parameter[0].plotcentre_lat=70.944295;
			parameter[0].plotcentre_lon=132.8777;
		} else if (plotcodeNum == 1027) /* ### FOR 14-OM-TRANS6-7 ### */ {
			parameter[0].plotcentre_lat=70.948754;
			parameter[0].plotcentre_lon=132.884332;
		} else if (plotcodeNum == 1029) /* ### FOR 16-KP-01-EN18001 ### */  {
			parameter[0].plotcentre_lat=67.39273;
			parameter[0].plotcentre_lon=168.34662;
		} else if (plotcodeNum == 1030) /* ### FOR 16-KP-01-EN18002 ### */  {
			parameter[0].plotcentre_lat=67.386775;
			parameter[0].plotcentre_lon=168.336731;
		} else if (plotcodeNum == 1031) /* ### FOR 16-KP-01-EN18003 ### */  {
			parameter[0].plotcentre_lat=67.39691;
			parameter[0].plotcentre_lon=168.34702;
		} else if (plotcodeNum == 1032) /* ### FOR 16-KP-01-EN18004 ### */  {
			parameter[0].plotcentre_lat=67.397489;
			parameter[0].plotcentre_lon=168.351225;
		} else if (plotcodeNum == 1033) /* ### FOR 16-KP-01-EN18005 ### */  {
			parameter[0].plotcentre_lat=67.419652;
			parameter[0].plotcentre_lon=168.387511;
		} else if (plotcodeNum == 1034) /* ### FOR 16-KP-01-EN18006 ### */  {
			parameter[0].plotcentre_lat=67.414969;
			parameter[0].plotcentre_lon=168.402874;
		} else if (plotcodeNum == 1035) /* ### FOR 16-KP-01-EN18007 ### */  {
			parameter[0].plotcentre_lat=67.403274;
			parameter[0].plotcentre_lon=168.371965;
		} else if (plotcodeNum == 1036) /* ### FOR 16-KP-01-EN18008 ### */  {
			parameter[0].plotcentre_lat=67.402135;
			parameter[0].plotcentre_lon=168.375284;
		} else if (plotcodeNum == 1037) /* ### FOR 16-KP-01-EN18009 ### */  {
			parameter[0].plotcentre_lat=67.400725;
			parameter[0].plotcentre_lon=168.379683;
		} else if (plotcodeNum == 1038) /* ### FOR 16-KP-01-EN18010 ### */  {
			parameter[0].plotcentre_lat=67.402371;
			parameter[0].plotcentre_lon=168.3662;
		} else if (plotcodeNum == 1039) /* ### FOR 16-KP-01-EN18011 ### */  {
			parameter[0].plotcentre_lat=67.404042;
			parameter[0].plotcentre_lon=168.364252;
		} else if (plotcodeNum == 1040) /* ### FOR 16-KP-01-EN18012 ### */  {
			parameter[0].plotcentre_lat=67.402142;
			parameter[0].plotcentre_lon=168.378078;
		} else if (plotcodeNum == 1041) /* ### FOR 16-KP-01-EN18013 ### */  {
			parameter[0].plotcentre_lat=67.405174;
			parameter[0].plotcentre_lon=168.355304;
		} else if (plotcodeNum == 1042) /* ### FOR 16-KP-01-EN18014 ### */  {
			parameter[0].plotcentre_lat=67.395309;
			parameter[0].plotcentre_lon=168.349106;
		} else if (plotcodeNum == 1043) /* ### FOR 16-KP-01-EN18015 ### */  {
			parameter[0].plotcentre_lat=67.420379;
			parameter[0].plotcentre_lon=168.33061;
		} else if (plotcodeNum == 1044) /* ### FOR 16-KP-01-EN18016 ### */  {
			parameter[0].plotcentre_lat=67.426726;
			parameter[0].plotcentre_lon=168.390047;
		} else if (plotcodeNum == 1045) /* ### FOR 16-KP-01-EN18017 ### */  {
			parameter[0].plotcentre_lat=67.43229;
			parameter[0].plotcentre_lon=168.383376;
		} else if (plotcodeNum == 1046) /* ### FOR 16-KP-01-EN18018 ### */  {
			parameter[0].plotcentre_lat=67.456295;
			parameter[0].plotcentre_lon=168.405961;
		} else if (plotcodeNum == 1047) /* ### FOR 16-KP-01-EN18019 ### */  {
			parameter[0].plotcentre_lat=168.405961;
			parameter[0].plotcentre_lon=168.408963;
		} else if (plotcodeNum == 1048) /* ### FOR 16-KP-01-EN18020 ### */  {
			parameter[0].plotcentre_lat=67.459159;
			parameter[0].plotcentre_lon=168.411934;
		} else if (plotcodeNum == 1049) /* ### FOR 16-KP-01-EN18021 ### */  {
			parameter[0].plotcentre_lat=67.392129;
			parameter[0].plotcentre_lon=168.328815;
		} else if (plotcodeNum == 1050) /* ### FOR 16-KP-01-EN18022 ### */  {
			parameter[0].plotcentre_lat=67.401024;
			parameter[0].plotcentre_lon=168.348006;
		} else if (plotcodeNum == 1051) /* ### FOR 16-KP-01-EN18023 ### */  {
			parameter[0].plotcentre_lat=67.399236;
			parameter[0].plotcentre_lon=168.351285;
		} else if (plotcodeNum == 1052) /* ### FOR 16-KP-01-EN18024 ### */  {
			parameter[0].plotcentre_lat=67.370964;
			parameter[0].plotcentre_lon=168.426362;
		} else if (plotcodeNum == 1053) /* ### FOR 16-KP-01-EN18025 ### */  {
			parameter[0].plotcentre_lat=67.367027;
			parameter[0].plotcentre_lon=168.42381;
		} else if (plotcodeNum == 1054) /* ### FOR 16-KP-01-EN18026 ### */  {
			parameter[0].plotcentre_lat=67.396089;
			parameter[0].plotcentre_lon=168.354297;
		} else if (plotcodeNum == 1055) /* ### FOR 16-KP-01-EN18027 ### */  {
			parameter[0].plotcentre_lat=67.393408;
			parameter[0].plotcentre_lon=168.35905;
		} else if (plotcodeNum == 1056) /* ### FOR 16-KP-04-EN18051 ### */  {
			parameter[0].plotcentre_lat=67.80261;
			parameter[0].plotcentre_lon=168.7047;
		} else if (plotcodeNum == 1057) /* ### FOR 16-KP-04-EN18052 ### */  {
			parameter[0].plotcentre_lat=67.79941;
			parameter[0].plotcentre_lon=168.7083;
		} else if (plotcodeNum == 1058) /* ### FOR 16-KP-04-EN18053 ### */  {
			parameter[0].plotcentre_lat=67.79729;
			parameter[0].plotcentre_lon=168.7107;
		} else if (plotcodeNum == 1059) /* ### FOR 16-KP-04-EN18054 ### */  {
			parameter[0].plotcentre_lat=67.79766;
			parameter[0].plotcentre_lon=168.6904;
		} else if (plotcodeNum == 1060) /* ### FOR 16-KP-04-EN18055 ### */ {
			parameter[0].plotcentre_lat=67.79103;
			parameter[0].plotcentre_lon=168.6825;
		} else if (plotcodeNum == 1061) /* ### FOR 16-KP-V01 ### */  {
			parameter[0].plotcentre_lat=67.3618;
			parameter[0].plotcentre_lon=168.2542;
		} else if (plotcodeNum == 1062) /* ### FOR 16-KP-V02 ### */  {
			parameter[0].plotcentre_lat=67.366;
			parameter[0].plotcentre_lon=168.2366;
		} else if (plotcodeNum == 1063) /* ### FOR 16-KP-V03 ### */  {
			parameter[0].plotcentre_lat=67.3664;
			parameter[0].plotcentre_lon=168.2948;
		} else if (plotcodeNum == 1064) /* ### FOR 16-KP-V04 ### */  {
			parameter[0].plotcentre_lat=67.3736;
			parameter[0].plotcentre_lon=168.31;
		} else if (plotcodeNum == 1065) /* ### FOR 16-KP-V05 ### */  {
			parameter[0].plotcentre_lat=67.3769;
			parameter[0].plotcentre_lon=168.3122;
		} else if (plotcodeNum == 1066) /* ### FOR 16-KP-V06 ### */  {
			parameter[0].plotcentre_lat=67.35;
			parameter[0].plotcentre_lon=168.1885;
		} else if (plotcodeNum == 1067) /* ### FOR 16-KP-V07 ### */  {
			parameter[0].plotcentre_lat=67.3456;
			parameter[0].plotcentre_lon=168.1842;
		} else if (plotcodeNum == 1068) /* ### FOR 16-KP-V08 ### */  {
			parameter[0].plotcentre_lat=67.3449;
			parameter[0].plotcentre_lon=168.1802;
		} else if (plotcodeNum == 1069) /* ### FOR 16-KP-V09 ### */  {
			parameter[0].plotcentre_lat=67.3538;
			parameter[0].plotcentre_lon=168.2157;
		} else if (plotcodeNum == 1070) /* ### FOR 16-KP-V10 ### */  {
			parameter[0].plotcentre_lat=67.3452;
			parameter[0].plotcentre_lon=168.2013;
		} else if (plotcodeNum == 1071) /* ### FOR 16-KP-V11 ### */  {
			parameter[0].plotcentre_lat=67.35;
			parameter[0].plotcentre_lon=168.2009;
		} else if (plotcodeNum == 1072) /* ### FOR 16-KP-V12 ### */  {
			parameter[0].plotcentre_lat=67.3531;
			parameter[0].plotcentre_lon=168.2264;
		} else if (plotcodeNum == 1073) /* ### FOR 16-KP-V13 ### */  {
			parameter[0].plotcentre_lat=66.9731;
			parameter[0].plotcentre_lon=163.4177;
		} else if (plotcodeNum == 1074) /* ### FOR 16-KP-V14 ### */  {
			parameter[0].plotcentre_lat=66.9874;
			parameter[0].plotcentre_lon=163.3981;
		} else if (plotcodeNum == 1075) /* ### FOR 16-KP-V15 ### */  {
			parameter[0].plotcentre_lat=66.9914;
			parameter[0].plotcentre_lon=163.3843;
		} else if (plotcodeNum == 1076) /* ### FOR 16-KP-V16 ### */  {
			parameter[0].plotcentre_lat=66.9715;
			parameter[0].plotcentre_lon=163.4021;
		} else if (plotcodeNum == 1077) /* ### FOR 16-KP-V17 ### */  {
			parameter[0].plotcentre_lat=66.9869;
			parameter[0].plotcentre_lon=163.455;
		} else if (plotcodeNum == 1078) /* ### FOR 16-KP-V18 ### */  {
			parameter[0].plotcentre_lat=66.9699;
			parameter[0].plotcentre_lon=163.3845;
		} else if (plotcodeNum == 1079) /* ### FOR 16-KP-V19 ### */  {
			parameter[0].plotcentre_lat=66.9706;
			parameter[0].plotcentre_lon=163.3948;
		} else if (plotcodeNum == 1080) /* ### FOR 16-KP-V20 ### */  {
			parameter[0].plotcentre_lat=65.9249;
			parameter[0].plotcentre_lon=166.3609;
		} else if (plotcodeNum == 1081) /* ### FOR 16-KP-V21 ### */  {
			parameter[0].plotcentre_lat=65.926;
			parameter[0].plotcentre_lon=166.3609;
		} else if (plotcodeNum == 1082) /* ### FOR 16-KP-V22 ### */  {
			parameter[0].plotcentre_lat=65.9352;
			parameter[0].plotcentre_lon=166.3905;
		} else if (plotcodeNum == 1083) /* ### FOR 16-KP-V23 ### */  {
			parameter[0].plotcentre_lat=65.9352;
			parameter[0].plotcentre_lon=166.3933;
		} else if (plotcodeNum == 1084) /* ### FOR 16-KP-V24 ### */  {
			parameter[0].plotcentre_lat=65.9365;
			parameter[0].plotcentre_lon=166.389;
		} else if (plotcodeNum == 1085) /* ### FOR 16-KP-V25 ### */  {
			parameter[0].plotcentre_lat=65.9372;
			parameter[0].plotcentre_lon=166.3906;
		} else if (plotcodeNum == 1086) /* ### FOR 16-KP-V26 ### */  {
			parameter[0].plotcentre_lat=65.9369;
			parameter[0].plotcentre_lon=166.3861;
		} else if (plotcodeNum == 1087) /* ### FOR 16-KP-V27 ### */  {
			parameter[0].plotcentre_lat=65.9369;
			parameter[0].plotcentre_lon=166.385;
		} else if (plotcodeNum == 1088) /* ### FOR 16-KP-V28 ### */  {
			parameter[0].plotcentre_lat=65.9231;
			parameter[0].plotcentre_lon=166.3683;
		} else if (plotcodeNum == 1089) /* ### FOR 16-KP-V29 ### */  {
			parameter[0].plotcentre_lat=65.9252;
			parameter[0].plotcentre_lon=166.3882;
		} else if (plotcodeNum == 1090) /* ### FOR 16-KP-V30 ### */  {
			parameter[0].plotcentre_lat=65.9579;
			parameter[0].plotcentre_lon=166.3333;
		} else if (plotcodeNum == 1091) /* ### FOR 16-KP-V31 ### */  {
			parameter[0].plotcentre_lat=65.9585;
			parameter[0].plotcentre_lon=166.3368;
		} else if (plotcodeNum == 1092) /* ### FOR 16-KP-V32 ### */  {
			parameter[0].plotcentre_lat=65.9468;
			parameter[0].plotcentre_lon=166.3561;
		} else if (plotcodeNum == 1093) /* ### FOR 16-KP-V33 ### */  {
			parameter[0].plotcentre_lat=65.9459;
			parameter[0].plotcentre_lon=166.3577;
		} else if (plotcodeNum == 1094) /* ### FOR 16-KP-V34 ### */  {
			parameter[0].plotcentre_lat=65.9415;
			parameter[0].plotcentre_lon=166.3486;
		} else if (plotcodeNum == 1095) /* ### FOR 16-KP-V35 ### */  {
			parameter[0].plotcentre_lat=65.9329;
			parameter[0].plotcentre_lon=166.2618;
		} else if (plotcodeNum == 1096) /* ### FOR 16-KP-V36 ### */  {
			parameter[0].plotcentre_lat=65.9294;
			parameter[0].plotcentre_lon=166.291;
		} else if (plotcodeNum == 1097) /* ### FOR 16-KP-V37 ### */  {
			parameter[0].plotcentre_lat=65.9002;
			parameter[0].plotcentre_lon=166.419;
		} else if (plotcodeNum == 1098) /* ### FOR 16-KP-V38 ### */  {
			parameter[0].plotcentre_lat=65.9003;
			parameter[0].plotcentre_lon=166.4168;
		} else if (plotcodeNum == 1099) /* ### FOR 16-KP-V39 ### */ {
			parameter[0].plotcentre_lat=65.9217;
			parameter[0].plotcentre_lon=166.3139;
		} else if (plotcodeNum == 1100) /* ### FOR 16-KP-V40 ### */  {
			parameter[0].plotcentre_lat=67.7969;
			parameter[0].plotcentre_lon=168.7096;
		} else if (plotcodeNum == 1101) /* ### FOR 16-KP-V41 ### */  {
			parameter[0].plotcentre_lat=67.8171;
			parameter[0].plotcentre_lon=168.6865;
		} else if (plotcodeNum == 1102) /* ### FOR 16-KP-V42 ### */  {
			parameter[0].plotcentre_lat=67.8171;
			parameter[0].plotcentre_lon=168.6885;
		} else if (plotcodeNum == 1103) /* ### FOR 16-KP-V43 ### */  {
			parameter[0].plotcentre_lat=67.8195;
			parameter[0].plotcentre_lon=168.6976;
		} else if (plotcodeNum == 1104) /* ### FOR 16-KP-V44 ### */  {
			parameter[0].plotcentre_lat=67.8196;
			parameter[0].plotcentre_lon=168.6963;
		} else if (plotcodeNum == 1105) /* ### FOR 16-KP-V45 ### */  {
			parameter[0].plotcentre_lat=67.82;
			parameter[0].plotcentre_lon=168.714;
		} else if (plotcodeNum == 1106) /* ### FOR 16-KP-V46 ### */  {
			parameter[0].plotcentre_lat=67.8199;
			parameter[0].plotcentre_lon=168.7115;
		} else if (plotcodeNum == 1107) /* ### FOR 16-KP-V47 ### */  {
			parameter[0].plotcentre_lat=67.8048;
			parameter[0].plotcentre_lon=168.7037;
		} else if (plotcodeNum == 1108) /* ### FOR 16-KP-V48 ### */  {
			parameter[0].plotcentre_lat=67.8002;
			parameter[0].plotcentre_lon=168.6379;
		} else if (plotcodeNum == 1109) /* ### FOR 16-KP-V49 ### */  {
			parameter[0].plotcentre_lat=67.8026;
			parameter[0].plotcentre_lon=168.6359;
		} else if (plotcodeNum == 1110) /* ### FOR 16-KP-V50 ### */  {
			parameter[0].plotcentre_lat=67.8051;
			parameter[0].plotcentre_lon=168.6297;
		} else if (plotcodeNum == 1111) /* ### FOR 16-KP-V51 ### */  {
			parameter[0].plotcentre_lat=67.8055;
			parameter[0].plotcentre_lon=168.6327;
		} else if (plotcodeNum == 1112) /* ### FOR 16-KP-V52 ### */  {
			parameter[0].plotcentre_lat=67.8069;
			parameter[0].plotcentre_lon=168.6311;
		} else if (plotcodeNum == 1113) /* ### FOR 16-KP-V53 ### */  {
			parameter[0].plotcentre_lat=67.8079;
			parameter[0].plotcentre_lon=168.6323;
		} else if (plotcodeNum == 1114) /* ### FOR 16-KP-V54 ### */  {
			parameter[0].plotcentre_lat=67.8096;
			parameter[0].plotcentre_lon=168.6299;
		} else if (plotcodeNum == 1115) /* ### FOR 16-KP-V55 ### */  {
			parameter[0].plotcentre_lat=67.8091;
			parameter[0].plotcentre_lon=168.6336;
		} else if (plotcodeNum == 1116) /* ### FOR 16-KP-V56 ### */  {
			parameter[0].plotcentre_lat=67.8082;
			parameter[0].plotcentre_lon=168.6355;
		} else if (plotcodeNum == 1117) /* ### FOR 16-KP-V57 ### */  {
			parameter[0].plotcentre_lat=67.8076;
			parameter[0].plotcentre_lon=168.645;
		} else if (plotcodeNum == 1118) /* ### FOR 16-KP-V58 ### */  {
			parameter[0].plotcentre_lat=67.8086;
			parameter[0].plotcentre_lon=168.645;
		} else if (plotcodeNum == 1119) /* ### FOR 18-BIL-00-EN18000 ### */  {
			parameter[0].plotcentre_lat=68.097147;
			parameter[0].plotcentre_lon=166.375447;
		} else if (plotcodeNum == 1120) /* ### FOR 18-BIL-01-EN18028 ### */  {
			parameter[0].plotcentre_lat=68.46781;
			parameter[0].plotcentre_lon=163.357622;
		} else if (plotcodeNum == 1121) /* ### FOR 18-BIL-01-EN18029 ### */  {
			parameter[0].plotcentre_lat=68.465606;
			parameter[0].plotcentre_lon=163.352262;
		} else if (plotcodeNum == 1122) /* ### FOR 18-BIL-02-EN18030 ### */  {
			parameter[0].plotcentre_lat=68.405539;
			parameter[0].plotcentre_lon=164.532731;
		} else if (plotcodeNum == 1123) /* ### FOR 18-BIL-02-EN18031 ### */  {
			parameter[0].plotcentre_lat=68.404918;
			parameter[0].plotcentre_lon=164.545351;
		} else if (plotcodeNum == 1124) /* ### FOR 18-BIL-02-EN18032 ### */  {
			parameter[0].plotcentre_lat=68.404868;
			parameter[0].plotcentre_lon=164.551181;
		} else if (plotcodeNum == 1125) /* ### FOR 18-BIL-02-EN18033 ### */  {
			parameter[0].plotcentre_lat=68.403212;
			parameter[0].plotcentre_lon=164.551805;
		} else if (plotcodeNum == 1126) /* ### FOR 18-BIL-02-EN18034 ### */  {
			parameter[0].plotcentre_lat=68.403486;
			parameter[0].plotcentre_lon=164.548043;
		} else if (plotcodeNum == 1127) /* ### FOR 18-BIL-02-EN18035 ### */ {
			parameter[0].plotcentre_lat=68.403166;
			parameter[0].plotcentre_lon=164.590932;
		} else if (plotcodeNum == 1128) /* ### FOR 18-LD-VP012-Tit-Ary ### */ {
			parameter[0].plotcentre_lat=71.967274;
			parameter[0].plotcentre_lon=127.092825;
		} else if (plotcodeNum == 1129) /* ### FOR B19-T1 ### */  {
			parameter[0].plotcentre_lat=67.58117;
			parameter[0].plotcentre_lon=134.785314;
		} else if (plotcodeNum == 1130) /* ### FOR B19-T2 ### */ {
			parameter[0].plotcentre_lat=67.580618;
			parameter[0].plotcentre_lon=134.78351;
		} else if (plotcodeNum == 1131) /* ### FOR EN18061 ### */  {
			parameter[0].plotcentre_lat=62.076376;
			parameter[0].plotcentre_lon=129.618586;
		} else if (plotcodeNum == 1132) /* ### FOR EN18062 ### */  {
			parameter[0].plotcentre_lat=62.179065;
			parameter[0].plotcentre_lon=127.805796;
		} else if (plotcodeNum == 1133) /* ### FOR EN18063 ### */  {
			parameter[0].plotcentre_lat=63.776636;
			parameter[0].plotcentre_lon=122.501003;
		} else if (plotcodeNum == 1134) /* ### FOR EN18064 ### */  {
			parameter[0].plotcentre_lat=63.814594;
			parameter[0].plotcentre_lon=122.209683;
		} else if (plotcodeNum == 1135) /* ### FOR EN18065 ### */  {
			parameter[0].plotcentre_lat=63.795223;
			parameter[0].plotcentre_lon=122.443715;
		} else if (plotcodeNum == 1136) /* ### FOR EN18066 ### */ {
			parameter[0].plotcentre_lat=63.797119;
			parameter[0].plotcentre_lon=122.438071;
		} else if (plotcodeNum == 1137) /* ### FOR EN18067 ### */  {
			parameter[0].plotcentre_lat=63.076368;
			parameter[0].plotcentre_lon=117.975342;
		} else if (plotcodeNum == 1138) /* ### FOR EN18068 ### */  {
			parameter[0].plotcentre_lat=63.074232;
			parameter[0].plotcentre_lon=117.98207;
		} else if (plotcodeNum == 1139) /* ### FOR EN18069 ### */ {
			parameter[0].plotcentre_lat=63.173288;
			parameter[0].plotcentre_lon=118.132507;
		} else if (plotcodeNum == 1140) /* ### FOR EN18070_centre ### */  {
			parameter[0].plotcentre_lat=63.082476;
			parameter[0].plotcentre_lon=117.985333;
		} else if (plotcodeNum == 1141) /* ### FOR EN18070_edge ### */  {
			parameter[0].plotcentre_lat=63.082983;
			parameter[0].plotcentre_lon=117.984938;
		} else if (plotcodeNum == 1142) /* ### FOR EN18070_end ### */  {
			parameter[0].plotcentre_lat=63.08341;
			parameter[0].plotcentre_lon=117.984574;
		} else if (plotcodeNum == 1143) /* ### FOR EN18070_transition ### */ {
			parameter[0].plotcentre_lat=63.082733;
			parameter[0].plotcentre_lon=117.985156;
		} else if (plotcodeNum == 1144) /* ### FOR EN18071 ### */ {
			parameter[0].plotcentre_lat=62.225093;
			parameter[0].plotcentre_lon=116.275603;
		} else if (plotcodeNum == 1145) /* ### FOR EN18072 ### */ {
			parameter[0].plotcentre_lat=62.199571;
			parameter[0].plotcentre_lon=117.379125;
		} else if (plotcodeNum == 1146) /* ### FOR EN18073 ### */  {
			parameter[0].plotcentre_lat=62.188712;
			parameter[0].plotcentre_lon=117.409917;
		} else if (plotcodeNum == 1147) /* ### FOR EN18074 ### */  {
			parameter[0].plotcentre_lat=62.215192;
			parameter[0].plotcentre_lon=117.021599;
		} else if (plotcodeNum == 1148) /* ### FOR EN18075 ### */ {
			parameter[0].plotcentre_lat=62.696991;
			parameter[0].plotcentre_lon=113.676535;
		} else if (plotcodeNum == 1149) /* ### FOR EN18076 ### */ {
			parameter[0].plotcentre_lat=62.70089;
			parameter[0].plotcentre_lon=113.67341;
		} else if (plotcodeNum == 1150) /* ### FOR EN18077 ### */ {
			parameter[0].plotcentre_lat=61.892568;
			parameter[0].plotcentre_lon=114.288623;
		} else if (plotcodeNum == 1151) /* ### FOR EN18078 ### */  {
			parameter[0].plotcentre_lat=61.575058;
			parameter[0].plotcentre_lon=114.29995;
		} else if (plotcodeNum == 1152) /* ### FOR EN18079 ### */  {
			parameter[0].plotcentre_lat=59.974919;
			parameter[0].plotcentre_lon=112.958985;
		} else if (plotcodeNum == 1153) /* ### FOR EN18080 ### */ {
			parameter[0].plotcentre_lat=59.977106;
			parameter[0].plotcentre_lon=112.961379;
		} else if (plotcodeNum == 1154) /* ### FOR EN18081 ### */  {
			parameter[0].plotcentre_lat=59.970583;
			parameter[0].plotcentre_lon=112.987096;
		} else if (plotcodeNum == 1155) /* ### FOR EN18082 ### */ {
			parameter[0].plotcentre_lat=59.97764;
			parameter[0].plotcentre_lon=112.98218;
		} else if (plotcodeNum == 1156) /* ### FOR EN18083 ### */  {
			parameter[0].plotcentre_lat=59.974714;
			parameter[0].plotcentre_lon=113.002874;
		} else if (plotcodeNum == 1157) /* ### FOR EN21201 ### */  {
			parameter[0].plotcentre_lat=63.217776;
			parameter[0].plotcentre_lon=139.543709;
		} else if (plotcodeNum == 1158) /* ### FOR EN21202 ### */  {
			parameter[0].plotcentre_lat=63.32516;
			parameter[0].plotcentre_lon=141.07455;
		} else if (plotcodeNum == 1159) /* ### FOR EN21203 ### */  {
			parameter[0].plotcentre_lat=63.430107;
			parameter[0].plotcentre_lon=140.412509;
		} else if (plotcodeNum == 1160) /* ### FOR EN21204 ### */  {
			parameter[0].plotcentre_lat=63.44253;
			parameter[0].plotcentre_lon=140.40282;
		} else if (plotcodeNum == 1161) /* ### FOR EN21205 ### */  {
			parameter[0].plotcentre_lat=63.43858;
			parameter[0].plotcentre_lon=140.40688;
		} else if (plotcodeNum == 1162) /* ### FOR EN21206 ### */  {
			parameter[0].plotcentre_lat=63.34379;
			parameter[0].plotcentre_lon=141.07071;
		} else if (plotcodeNum == 1163) /* ### FOR EN21207 ### */  {
			parameter[0].plotcentre_lat=63.344383;
			parameter[0].plotcentre_lon=141.069788;
		} else if (plotcodeNum == 1164) /* ### FOR EN21208 ### */  {
			parameter[0].plotcentre_lat=63.34528;
			parameter[0].plotcentre_lon=141.06827;
		} else if (plotcodeNum == 1165) /* ### FOR EN21209 ### */  {
			parameter[0].plotcentre_lat=63.39854;
			parameter[0].plotcentre_lon=140.55406;
		} else if (plotcodeNum == 1166) /* ### FOR EN21210 ### */  {
			parameter[0].plotcentre_lat=63.397717;
			parameter[0].plotcentre_lon=140.55925;
		} else if (plotcodeNum == 1167) /* ### FOR EN21211 ### */  {
			parameter[0].plotcentre_lat=63.40056;
			parameter[0].plotcentre_lon=140.55357;
		} else if (plotcodeNum == 1168) /* ### FOR EN21212 ### */  {
			parameter[0].plotcentre_lat=63.232626;
			parameter[0].plotcentre_lon=142.962381;
		} else if (plotcodeNum == 1169) /* ### FOR EN21213 ### */  {
			parameter[0].plotcentre_lat=63.230378;
			parameter[0].plotcentre_lon=142.963774;
		} else if (plotcodeNum == 1170) /* ### FOR EN21214 ### */ {
			parameter[0].plotcentre_lat=63.23257;
			parameter[0].plotcentre_lon=142.9577;
		} else if (plotcodeNum == 1171) /* ### FOR EN21215 ### */  {
			parameter[0].plotcentre_lat=63.210719;
			parameter[0].plotcentre_lon=139.540937;
		} else if (plotcodeNum == 1172) /* ### FOR EN21216 ### */  {
			parameter[0].plotcentre_lat=63.212267;
			parameter[0].plotcentre_lon=139.541692;
		} else if (plotcodeNum == 1173) /* ### FOR EN21217 ### */  {
			parameter[0].plotcentre_lat=63.438697;
			parameter[0].plotcentre_lon=140.597609;
		} else if (plotcodeNum == 1174) /* ### FOR EN21218 ### */  {
			parameter[0].plotcentre_lat=63.428277;
			parameter[0].plotcentre_lon=140.579547;
		} else if (plotcodeNum == 1175) /* ### FOR EN21219 ### */ {
			parameter[0].plotcentre_lat=63.425647;
			parameter[0].plotcentre_lon=140.588331;
		} else if (plotcodeNum == 1176) /* ### FOR EN21220 ### */  {
			parameter[0].plotcentre_lat=62.07984;
			parameter[0].plotcentre_lon=132.3668;
		} else if (plotcodeNum == 1177) /* ### FOR EN21221 ### */  {
			parameter[0].plotcentre_lat=62.083241;
			parameter[0].plotcentre_lon=132.372643;
		} else if (plotcodeNum == 1178) /* ### FOR EN21222 ### */  {
			parameter[0].plotcentre_lat=62.08595;
			parameter[0].plotcentre_lon=132.370772;
		} else if (plotcodeNum == 1179) /* ### FOR EN21223 ### */  {
			parameter[0].plotcentre_lat=62.087193;
			parameter[0].plotcentre_lon=132.370561;
		} else if (plotcodeNum == 1180) /* ### FOR EN21224 ### */  {
			parameter[0].plotcentre_lat=62.042778;
			parameter[0].plotcentre_lon=132.388521;
		} else if (plotcodeNum == 1181) /* ### FOR EN21225 ### */  {
			parameter[0].plotcentre_lat=62.044236;
			parameter[0].plotcentre_lon=132.391202;
		} else if (plotcodeNum == 1182) /* ### FOR EN21226 ### */  {
			parameter[0].plotcentre_lat=62.045558;
			parameter[0].plotcentre_lon=132.389098;
		} else if (plotcodeNum == 1183) /* ### FOR EN21227 ### */  {
			parameter[0].plotcentre_lat=62.040546;
			parameter[0].plotcentre_lon=132.396302;
		} else if (plotcodeNum == 1184) /* ### FOR EN21228 ### */  {
			parameter[0].plotcentre_lat=62.384988;
			parameter[0].plotcentre_lon=133.748979;
		} else if (plotcodeNum == 1185) /* ### FOR EN21229 ### */  {
			parameter[0].plotcentre_lat=62.384468;
			parameter[0].plotcentre_lon=133.750727;
		} else if (plotcodeNum == 1186) /* ### FOR EN21230 ### */  {
			parameter[0].plotcentre_lat=62.334507;
			parameter[0].plotcentre_lon=133.688018;
		} else if (plotcodeNum == 1187) /* ### FOR EN21231 ### */ {
			parameter[0].plotcentre_lat=62.334694;
			parameter[0].plotcentre_lon=133.68405;
		} else if (plotcodeNum == 1188) /* ### FOR EN21232 ### */  {
			parameter[0].plotcentre_lat=62.172203;
			parameter[0].plotcentre_lon=130.911195;
		} else if (plotcodeNum == 1189) /* ### FOR EN21233 ### */  {
			parameter[0].plotcentre_lat=62.169607;
			parameter[0].plotcentre_lon=130.903851;
		} else if (plotcodeNum == 1190) /* ### FOR EN21234 ### */  {
			parameter[0].plotcentre_lat=62.287013;
			parameter[0].plotcentre_lon=130.377589;
		} else if (plotcodeNum == 1191) /* ### FOR EN21235 ### */  {
			parameter[0].plotcentre_lat=62.275634;
			parameter[0].plotcentre_lon=130.37659;
		} else if (plotcodeNum == 1192) /* ### FOR EN21236 ### */  {
			parameter[0].plotcentre_lat=62.262231;
			parameter[0].plotcentre_lon=130.327876;
		} else if (plotcodeNum == 1193) /* ### FOR EN21237 ### */  {
			parameter[0].plotcentre_lat=62.13009;
			parameter[0].plotcentre_lon=130.874837;
		} else if (plotcodeNum == 1194) /* ### FOR EN21238 ### */  {
			parameter[0].plotcentre_lat=62.133528;
			parameter[0].plotcentre_lon=130.873521;
		} else if (plotcodeNum == 1195) /* ### FOR EN21239 ### */  {
			parameter[0].plotcentre_lat=62.316127;
			parameter[0].plotcentre_lon=130.116028;
		} else if (plotcodeNum == 1196) /* ### FOR EN21240 ### */  {
			parameter[0].plotcentre_lat=62.353399;
			parameter[0].plotcentre_lon=130.151416;
		} else if (plotcodeNum == 1197) /* ### FOR EN21241 ### */  {
			parameter[0].plotcentre_lat=62.148377;
			parameter[0].plotcentre_lon=130.65177;
		} else if (plotcodeNum == 1198) /* ### FOR EN21242 ### */  {
			parameter[0].plotcentre_lat=62.148415;
			parameter[0].plotcentre_lon=130.653568;
		} else if (plotcodeNum == 1199) /* ### FOR EN21243 ### */  {
			parameter[0].plotcentre_lat=62.149423;
			parameter[0].plotcentre_lon=130.654024;
		} else if (plotcodeNum == 1200) /* ### FOR EN21244 ### */  {
			parameter[0].plotcentre_lat=62.156934;
			parameter[0].plotcentre_lon=130.659589;
		} else if (plotcodeNum == 1201) /* ### FOR EN21245 ### */  {
			parameter[0].plotcentre_lat=61.78444;
			parameter[0].plotcentre_lon=130.48492;
		} else if (plotcodeNum == 1202) /* ### FOR EN21246 ### */  {
			parameter[0].plotcentre_lat=61.78305;
			parameter[0].plotcentre_lon=130.49245;
		} else if (plotcodeNum == 1203) /* ### FOR EN21247 ### */  {
			parameter[0].plotcentre_lat=61.77975;
			parameter[0].plotcentre_lon=130.49998;
		} else if (plotcodeNum == 1204) /* ### FOR EN21248 ### */  {
			parameter[0].plotcentre_lat=61.747877;
			parameter[0].plotcentre_lon=130.530323;
		} else if (plotcodeNum == 1205) /* ### FOR EN21249 ### */  {
			parameter[0].plotcentre_lat=61.745655;
			parameter[0].plotcentre_lon=130.530715;
		} else if (plotcodeNum == 1206) /* ### FOR EN21250 ### */  {
			parameter[0].plotcentre_lat=61.745696;
			parameter[0].plotcentre_lon=130.532625;
		} else if (plotcodeNum == 1207) /* ### FOR EN21251 ### */  {
			parameter[0].plotcentre_lat=61.740083;
			parameter[0].plotcentre_lon=130.528577;
		} else if (plotcodeNum == 1208) /* ### FOR EN21252 ### */  {
			parameter[0].plotcentre_lat=61.897154;
			parameter[0].plotcentre_lon=130.482395;
		} else if (plotcodeNum == 1209) /* ### FOR EN21253 ### */  {
			parameter[0].plotcentre_lat=61.89501;
			parameter[0].plotcentre_lon=130.4848;
		} else if (plotcodeNum == 1210) /* ### FOR EN21254 ### */  {
			parameter[0].plotcentre_lat=61.894779;
			parameter[0].plotcentre_lon=130.488766;
		} else if (plotcodeNum == 1211) /* ### FOR EN21255 ### */  {
			parameter[0].plotcentre_lat=61.769113;
			parameter[0].plotcentre_lon=130.386747;
		} else if (plotcodeNum == 1212) /* ### FOR EN21256 ### */  {
			parameter[0].plotcentre_lat=61.76639;
			parameter[0].plotcentre_lon=130.83875;
		} else if (plotcodeNum == 1213) /* ### FOR EN21257 ### */  {
			parameter[0].plotcentre_lat=61.770502;
			parameter[0].plotcentre_lon=130.391538;
		} else if (plotcodeNum == 1214) /* ### FOR EN21258 ### */  {
			parameter[0].plotcentre_lat=61.899226;
			parameter[0].plotcentre_lon=130.423401;
		} else if (plotcodeNum == 1215) /* ### FOR EN21259 ### */  {
			parameter[0].plotcentre_lat=61.901329;
			parameter[0].plotcentre_lon=130.500516;
		} else if (plotcodeNum == 1216) /* ### FOR EN21260 ### */  {
			parameter[0].plotcentre_lat=61.76387;
			parameter[0].plotcentre_lon=130.47968;
		} else if (plotcodeNum == 1217) /* ### FOR EN21261 ### */  {
			parameter[0].plotcentre_lat=61.766817;
			parameter[0].plotcentre_lon=130.457716;
		} else if (plotcodeNum == 1218) /* ### FOR EN21262 ### */ {
			parameter[0].plotcentre_lat=61.76123;
			parameter[0].plotcentre_lon=130.47043;
		} else if (plotcodeNum == 1219) /* ### FOR TY04VI ### */  {
			parameter[0].plotcentre_lat=72.40887;
			parameter[0].plotcentre_lon=105.44804;
		} else if (plotcodeNum == 1220) /* ### FOR TY04VII ### */ {
			parameter[0].plotcentre_lat=72.40401;
			parameter[0].plotcentre_lon=105.45187;
		} else if (plotcodeNum == 1221) /* ### FOR TY07VI ### */  {
			parameter[0].plotcentre_lat=71.10012;
			parameter[0].plotcentre_lon=100.81295;
		} else if (plotcodeNum == 1222) /* ### FOR TY07VII ### */  {
			parameter[0].plotcentre_lat=71.10598;
			parameter[0].plotcentre_lon=100.8463;
		} else if (plotcodeNum == 1223) /* ### FOR TY09VI ### */  {
			parameter[0].plotcentre_lat=72.15067;
			parameter[0].plotcentre_lon=102.09771;
		} else if (plotcodeNum == 1224) /* ### FOR TY09VII ### */ {
			parameter[0].plotcentre_lat=72.14365;
			parameter[0].plotcentre_lon=102.06259;
		} else if (plotcodeNum == 2001) /* ### FOR EN22-001 ### */  {
			parameter[0].plotcentre_lat=60.53427;
			parameter[0].plotcentre_lon=-134.37613;
		} else if (plotcodeNum == 2002) /* ### FOR EN22-002 ### */  {
			parameter[0].plotcentre_lat=60.81041;
			parameter[0].plotcentre_lon=-135.36891;
		} else if (plotcodeNum == 2003) /* ### FOR EN22-003 ### */  {
			parameter[0].plotcentre_lat=60.44012;
			parameter[0].plotcentre_lon=-133.55534;
		} else if (plotcodeNum == 2004) /* ### FOR EN22-004 ### */  {
			parameter[0].plotcentre_lat=60.44786;
			parameter[0].plotcentre_lon=-133.56215;
		} else if (plotcodeNum == 2005) /* ### FOR EN22-005 ### */  {
			parameter[0].plotcentre_lat=67.912;
			parameter[0].plotcentre_lon=-133.5627;
		} else if (plotcodeNum == 2006) /* ### FOR EN22-006 ### */  {
			parameter[0].plotcentre_lat=68.7259;
			parameter[0].plotcentre_lon=-133.5329;
		} else if (plotcodeNum == 2007) /* ### FOR EN22-007 ### */  {
			parameter[0].plotcentre_lat=68.8797;
			parameter[0].plotcentre_lon=-133.5309;
		} else if (plotcodeNum == 2008) /* ### FOR EN22-008 ### */  {
			parameter[0].plotcentre_lat=68.6187;
			parameter[0].plotcentre_lon=-133.6878;
		} else if (plotcodeNum == 2009) /* ### FOR EN22-009 ### */  {
			parameter[0].plotcentre_lat=68.4355;
			parameter[0].plotcentre_lon=-133.7567;
		} else if (plotcodeNum == 2010) /* ### FOR EN22-010 ### */  {
			parameter[0].plotcentre_lat=68.7227;
			parameter[0].plotcentre_lon=-133.536;
		} else if (plotcodeNum == 2011) /* ### FOR EN22-011 ### */  {
			parameter[0].plotcentre_lat=68.0041;
			parameter[0].plotcentre_lon=-133.4666;
		} else if (plotcodeNum == 2012) /* ### FOR EN22-012 ### */  {
			parameter[0].plotcentre_lat=68.0421;
			parameter[0].plotcentre_lon=-133.488;
		} else if (plotcodeNum == 2013) /* ### FOR EN22-013 ### */  {
			parameter[0].plotcentre_lat=68.225;
			parameter[0].plotcentre_lon=-133.3415;
		} else if (plotcodeNum == 2014) /* ### FOR EN22-014 ### */  {
			parameter[0].plotcentre_lat=68.3063;
			parameter[0].plotcentre_lon=-133.3215;
		} else if (plotcodeNum == 2015) /* ### FOR EN22-015 ### */  {
			parameter[0].plotcentre_lat=67.8567;
			parameter[0].plotcentre_lon=-133.6707;
		} else if (plotcodeNum == 2016) /* ### FOR EN22-016 ### */  {
			parameter[0].plotcentre_lat=67.7922;
			parameter[0].plotcentre_lon=-133.7832;
		} else if (plotcodeNum == 2017) /* ### FOR EN22-017 ### */  {
			parameter[0].plotcentre_lat=67.6427;
			parameter[0].plotcentre_lon=-133.8387;
		} else if (plotcodeNum == 2018) /* ### FOR EN22-018 ### */  {
			parameter[0].plotcentre_lat=67.5732;
			parameter[0].plotcentre_lon=-133.7987;
		} else if (plotcodeNum == 2019) /* ### FOR EN22-019 ### */  {
			parameter[0].plotcentre_lat=67.2069;
			parameter[0].plotcentre_lon=-135.6328;
		} else if (plotcodeNum == 2020) /* ### FOR EN22-020 ### */  {
			parameter[0].plotcentre_lat=67.2064;
			parameter[0].plotcentre_lon=-135.6347;
		} else if (plotcodeNum == 2021) /* ### FOR EN22-021 ### */  {
			parameter[0].plotcentre_lat=67.3888;
			parameter[0].plotcentre_lon=-134.2168;
		} else if (plotcodeNum == 2022) /* ### FOR EN22-022 ### */  {
			parameter[0].plotcentre_lat=67.3826;
			parameter[0].plotcentre_lon=-134.0799;
		} else if (plotcodeNum == 2023) /* ### FOR EN22-023 ### */  {
			parameter[0].plotcentre_lat=67.437;
			parameter[0].plotcentre_lon=-133.7838;
		} else if (plotcodeNum == 2024) /* ### FOR EN22-024 ### */  {
			parameter[0].plotcentre_lat=67.4066;
			parameter[0].plotcentre_lon=-134.4042;
		} else if (plotcodeNum == 2025) /* ### FOR EN22-025 ### */  {
			parameter[0].plotcentre_lat=67.4773;
			parameter[0].plotcentre_lon=-134.6771;
		} else if (plotcodeNum == 2026) /* ### FOR EN22-026 ### */  {
			parameter[0].plotcentre_lat=67.4313;
			parameter[0].plotcentre_lon=-134.8336;
		} else if (plotcodeNum == 2027) /* ### FOR EN22-027 ### */  {
			parameter[0].plotcentre_lat=67.3329;
			parameter[0].plotcentre_lon=-134.9229;
		} else if (plotcodeNum == 2028) /* ### FOR EN22-028 ### */  {
			parameter[0].plotcentre_lat=67.3117;
			parameter[0].plotcentre_lon=-135.0071;
		} else if (plotcodeNum == 2029) /* ### FOR EN22-029 ### */  {
			parameter[0].plotcentre_lat=67.2533;
			parameter[0].plotcentre_lon=-135.1283;
		} else if (plotcodeNum == 2030) /* ### FOR EN22-030 ### */  {
			parameter[0].plotcentre_lat=67.243;
			parameter[0].plotcentre_lon=-135.2106;
		} else if (plotcodeNum == 2031) /* ### FOR EN22-031 ### */  {
			parameter[0].plotcentre_lat=67.2325;
			parameter[0].plotcentre_lon=-135.4108;
		} else if (plotcodeNum == 2032) /* ### FOR EN22-032 ### */  {
			parameter[0].plotcentre_lat=67.1526;
			parameter[0].plotcentre_lon=-135.9295;
		} else if (plotcodeNum == 2033) /* ### FOR EN22-033 ### */  {
			parameter[0].plotcentre_lat=66.93442;
			parameter[0].plotcentre_lon=-136.290468;
		} else if (plotcodeNum == 2034) /* ### FOR EN22-034 ### */  {
			parameter[0].plotcentre_lat=66.90194;
			parameter[0].plotcentre_lon=-136.29453;
		} else if (plotcodeNum == 2035) /* ### FOR EN22-035 ### */  {
			parameter[0].plotcentre_lat=66.90194;
			parameter[0].plotcentre_lon=-136.35462;
		} else if (plotcodeNum == 2036) /* ### FOR EN22-036 ### */ {
			parameter[0].plotcentre_lat=66.82954;
			parameter[0].plotcentre_lon=-136.34838;
		} else if (plotcodeNum == 2037) /* ### FOR EN22-037 ### */  {
			parameter[0].plotcentre_lat=66.62793;
			parameter[0].plotcentre_lon=-136.299693;
		} else if (plotcodeNum == 2038) /* ### FOR EN22-038 ### */  {
			parameter[0].plotcentre_lat=66.2334;
			parameter[0].plotcentre_lon=-136.90315;
		} else if (plotcodeNum == 2039) /* ### FOR EN22-039 ### */  {
			parameter[0].plotcentre_lat=65.926265;
			parameter[0].plotcentre_lon=-137.471415;
		} else if (plotcodeNum == 2040) /* ### FOR EN22-040 ### */  {
			parameter[0].plotcentre_lat=65.5562;
			parameter[0].plotcentre_lon=-138.19272;
		} else if (plotcodeNum == 2041) /* ### FOR EN22-041 ### */  {
			parameter[0].plotcentre_lat=65.39423;
			parameter[0].plotcentre_lon=-138.26985;
		} else if (plotcodeNum == 2042) /* ### FOR EN22-042 ### */  {
			parameter[0].plotcentre_lat=65.2953;
			parameter[0].plotcentre_lon=-138.21966;
		} else if (plotcodeNum == 2043) /* ### FOR EN22-043 ### */  {
			parameter[0].plotcentre_lat=65.25923;
			parameter[0].plotcentre_lon=-138.27142;
		} else if (plotcodeNum == 2044) /* ### FOR EN22-044 ### */  {
			parameter[0].plotcentre_lat=65.20064;
			parameter[0].plotcentre_lon=-138.32876;
		} else if (plotcodeNum == 2045) /* ### FOR EN22-045 ### */  {
			parameter[0].plotcentre_lat=65.15965;
			parameter[0].plotcentre_lon=-138.37144;
		} else if (plotcodeNum == 2046) /* ### FOR EN22-046 ### */  {
			parameter[0].plotcentre_lat=65.06881;
			parameter[0].plotcentre_lon=-138.273222;
		} else if (plotcodeNum == 2047) /* ### FOR EN22-047 ### */  {
			parameter[0].plotcentre_lat=65.067487;
			parameter[0].plotcentre_lon=-138.26969463;
		} else if (plotcodeNum == 2048) /* ### FOR EN22-048 ### */  {
			parameter[0].plotcentre_lat=64.92803;
			parameter[0].plotcentre_lon=-138.27062;
		} else if (plotcodeNum == 2049) /* ### FOR EN22-049 ### */  {
			parameter[0].plotcentre_lat=64.436747;
			parameter[0].plotcentre_lon=-138.278837;
		} else if (plotcodeNum == 2050) /* ### FOR EN22-050 ### */  {
			parameter[0].plotcentre_lat=64.63873;
			parameter[0].plotcentre_lon=-138.38379;
		} else if (plotcodeNum == 2051) /* ### FOR EN22-051 ### */  {
			parameter[0].plotcentre_lat=64.33388;
			parameter[0].plotcentre_lon=-138.44601;
		} else if (plotcodeNum == 2052) /* ### FOR EN22-052 ### */  {
			parameter[0].plotcentre_lat=63.937546;
			parameter[0].plotcentre_lon=-138.452121;
		} else if (plotcodeNum == 2053) /* ### FOR EN22-053 ### */  {
			parameter[0].plotcentre_lat=63.82325;
			parameter[0].plotcentre_lon=-137.94431;
		} else if (plotcodeNum == 2054) /* ### FOR EN22-054 ### */ {
			parameter[0].plotcentre_lat=63.53877;
			parameter[0].plotcentre_lon=-137.15491;
		} else if (plotcodeNum == 2055) /* ### FOR EN22-055 ### */ {
			parameter[0].plotcentre_lat=63.267611;
			parameter[0].plotcentre_lon=-136.568748;
		} else if (plotcodeNum == 2056) /* ### FOR EN22-056 ### */  {
			parameter[0].plotcentre_lat=62.85252;
			parameter[0].plotcentre_lon=-136.76587;
		} else if (plotcodeNum == 2057) /* ### FOR EN22-057 ### */  {
			parameter[0].plotcentre_lat=62.60272;
			parameter[0].plotcentre_lon=-136.85939;
		} else if (plotcodeNum == 2058) /* ### FOR EN22-058 ### */  {
			parameter[0].plotcentre_lat=62.187636;
			parameter[0].plotcentre_lon=-136.261024;
		} else if (plotcodeNum == 2059) /* ### FOR EN22-059 ### */  {
			parameter[0].plotcentre_lat=61.72148;
			parameter[0].plotcentre_lon=-135.95156;
		} else if (plotcodeNum == 2060) /* ### FOR EN22-060 ### */ {
			parameter[0].plotcentre_lat=61.290864;
			parameter[0].plotcentre_lon=-135.524002;
		} else if (plotcodeNum == 2061) /* ### FOR EN22-061 ### */  {
			parameter[0].plotcentre_lat=59.96749;
			parameter[0].plotcentre_lon=-132.07268;
		} else if (plotcodeNum == 2062) /* ### FOR EN22-062 ### */ {
			parameter[0].plotcentre_lat=60.179563;
			parameter[0].plotcentre_lon=-129.833194;
		} else if (plotcodeNum == 2063) /* ### FOR EN22-063 ### */  {
			parameter[0].plotcentre_lat=59.96956;
			parameter[0].plotcentre_lon=-127.497091;
		} else if (plotcodeNum == 2064) /* ### FOR EN22-064 ### */  {
			parameter[0].plotcentre_lat=59.55017;
			parameter[0].plotcentre_lon=-126.44694;
		} else if (plotcodeNum == 2065) /* ### FOR EN22-065 ### */  {
			parameter[0].plotcentre_lat=59.04545;
			parameter[0].plotcentre_lon=-125.775906;
		} else if (plotcodeNum == 2066) /* ### FOR EN22-066 ### */ {
			parameter[0].plotcentre_lat=58.78415;
			parameter[0].plotcentre_lon=-125.66135;
		} else if (plotcodeNum == 2067) /* ### FOR EN22-067 ### */  {
			parameter[0].plotcentre_lat=58.8341;
			parameter[0].plotcentre_lon=-125.02526;
		} else if (plotcodeNum == 2068) /* ### FOR EN22-068 ### */  {
			parameter[0].plotcentre_lat=58.65046;
			parameter[0].plotcentre_lon=-124.74001;
		} else if (plotcodeNum == 2069) /* ### FOR EN22-069 ### */ {
			parameter[0].plotcentre_lat=58.750453;
			parameter[0].plotcentre_lon=-121.832375;
		} else if (plotcodeNum == 2070) /* ### FOR EN22-070 ### */  {
			parameter[0].plotcentre_lat=58.7714;
			parameter[0].plotcentre_lon=-122.25326;
		} else if (plotcodeNum == 2071) /* ### FOR EN22-071 ### */  {
			parameter[0].plotcentre_lat=58.96987;
			parameter[0].plotcentre_lon=-123.173866;
		} else if (plotcodeNum == 2072) /* ### FOR EN22-072 ### */  {
			parameter[0].plotcentre_lat=58.68435;
			parameter[0].plotcentre_lon=-123.76826;
		} else if (plotcodeNum == 2073) /* ### FOR EN22-073 ### */  {
			parameter[0].plotcentre_lat=58.66516;
			parameter[0].plotcentre_lon=-124.3893;
		} else if (plotcodeNum == 2074) /* ### FOR EN22-074 ### */ {
			parameter[0].plotcentre_lat=58.66516;
			parameter[0].plotcentre_lon=-124.3893;
		} else if (plotcodeNum == 2075) /* ### FOR EN22-075 ### */ {
			parameter[0].plotcentre_lat=60.06028;
			parameter[0].plotcentre_lon=-129.23755;
		} else if (plotcodeNum == 2076) /* ### FOR PC1 ### */  {
			parameter[0].plotcentre_lat=72.03465049;
			parameter[0].plotcentre_lon=-125.35697996;
		} else if (plotcodeNum == 2077) /* ### FOR PC10 ### */ {
			parameter[0].plotcentre_lat=69.01561062;
			parameter[0].plotcentre_lon=-122.69251644;
		} else if (plotcodeNum == 2078) /* ### FOR PC11 ### */ {
			parameter[0].plotcentre_lat=69.59394081;
			parameter[0].plotcentre_lon=-139.00630286;
		} else if (plotcodeNum == 2079) /* ### FOR PC2 ### */  {
			parameter[0].plotcentre_lat=72.06173986;
			parameter[0].plotcentre_lon=-122.92350829;
		} else if (plotcodeNum == 2080) /* ### FOR PC3 ### */ {
			parameter[0].plotcentre_lat=72.7119038;
			parameter[0].plotcentre_lon=-121.51186605;
		} else if (plotcodeNum == 2081) /* ### FOR PC4 ### */ {
			parameter[0].plotcentre_lat=73.17399453;
			parameter[0].plotcentre_lon=-122.5389868;
		} else if (plotcodeNum == 2082) /* ### FOR PC5 ### */ {
			parameter[0].plotcentre_lat=74.23409215;
			parameter[0].plotcentre_lon=-123.98918211;
		} else if (plotcodeNum == 2083) /* ### FOR PC6 ### */ {
			parameter[0].plotcentre_lat=73.7739432;
			parameter[0].plotcentre_lon=-117.79289305;
		} else if (plotcodeNum == 2084) /* ### FOR PC7 ### */  {
			parameter[0].plotcentre_lat=69.78258314;
			parameter[0].plotcentre_lon=-130.82096159;
		} else if (plotcodeNum == 2085) /* ### FOR PC8 ### */ {
			parameter[0].plotcentre_lat=69.45618065;
			parameter[0].plotcentre_lon=-129.40402567;
		} else if (plotcodeNum == 2086) /* ### FOR PC9 ### */ {
			parameter[0].plotcentre_lat=69.31188255;
			parameter[0].plotcentre_lon=-126.49275601;
		} else if (plotcodeNum == 3001) /* ### FOR EN23-601 ### */  {
			parameter[0].plotcentre_lat=61.1362653;
			parameter[0].plotcentre_lon=-149.7030367;
		} else if (plotcodeNum == 3002) /* ### FOR EN23-602 ### */  {
			parameter[0].plotcentre_lat=60.9457113;
			parameter[0].plotcentre_lon=-149.3661231;
		} else if (plotcodeNum == 3003) /* ### FOR EN23-604 ### */ {
			parameter[0].plotcentre_lat=62.2263052;
			parameter[0].plotcentre_lon=-150.2399162;
		} else if (plotcodeNum == 3004) /* ### FOR EN23-608 ### */  {
			parameter[0].plotcentre_lat=63.8200482;
			parameter[0].plotcentre_lon=-148.9961324;
		} else if (plotcodeNum == 3005) /* ### FOR EN23-611 ### */  {
			parameter[0].plotcentre_lat=64.7669253;
			parameter[0].plotcentre_lon=-148.2784003;
		} else if (plotcodeNum == 3006) /* ### FOR EN23-612 ### */  {
			parameter[0].plotcentre_lat=64.916255;
			parameter[0].plotcentre_lon=-147.8332011;
		} else if (plotcodeNum == 3007) /* ### FOR EN23-614 ### */  {
			parameter[0].plotcentre_lat=65.4461954;
			parameter[0].plotcentre_lon=-148.7626697;
		} else if (plotcodeNum == 3008) /* ### FOR EN23-619 ### */  {
			parameter[0].plotcentre_lat=66.9526671;
			parameter[0].plotcentre_lon=-150.4040776;
		} else if (plotcodeNum == 3009) /* ### FOR EN23-621 ### */  {
			parameter[0].plotcentre_lat=67.557166;
			parameter[0].plotcentre_lon=-149.8276888;
		} else if (plotcodeNum == 3010) /* ### FOR EN23-623 ### */  {
			parameter[0].plotcentre_lat=68.4545627;
			parameter[0].plotcentre_lon=-149.4969702;
		} else if (plotcodeNum == 3011) /* ### FOR EN23-624 ### */  {
			parameter[0].plotcentre_lat=68.454674;
			parameter[0].plotcentre_lon=-149.4984708;
		} else if (plotcodeNum == 3012) /* ### FOR EN23-634 ### */ {
			parameter[0].plotcentre_lat=66.921885;
			parameter[0].plotcentre_lon=-150.4324403;
		} else if (plotcodeNum == 3013) /* ### FOR EN23-644 ### */  {
			parameter[0].plotcentre_lat=64.7168063;
			parameter[0].plotcentre_lon=-148.3107308;
		} else if (plotcodeNum == 3014) /* ### FOR EN23-645 ### */  {
			parameter[0].plotcentre_lat=64.7300969;
			parameter[0].plotcentre_lon=-148.3336121;
		} else if (plotcodeNum == 3015) /* ### FOR EN23-652 ### */  {
			parameter[0].plotcentre_lat=65.4464244;
			parameter[0].plotcentre_lon=-145.488952;
		} else if (plotcodeNum == 3016) /* ### FOR EN23-660 ### */  {
			parameter[0].plotcentre_lat=65.4060284;
			parameter[0].plotcentre_lon=-145.9695381;
		} else if (plotcodeNum == 3017) /* ### FOR EN23-666 ### */  {
			parameter[0].plotcentre_lat=65.122341;
			parameter[0].plotcentre_lon=-147.489676;
		} else if (plotcodeNum == 3018) /* ### FOR EN23-668 ### */  {
			parameter[0].plotcentre_lat=64.9474215;
			parameter[0].plotcentre_lon=-148.3361207;
		} else if (plotcodeNum == 3019) /* ### FOR EN23-669 ### */ {
			parameter[0].plotcentre_lat=64.916255;
			parameter[0].plotcentre_lon=-147.8332011;
		} else if (plotcodeNum == 3020) /* ### FOR EN23-675 ### */  {
			parameter[0].plotcentre_lat=63.1982616;
			parameter[0].plotcentre_lon=-145.6321766;
		} else if (plotcodeNum == 3021) /* ### FOR EN23-677 ### */ {
			parameter[0].plotcentre_lat=63.8138379;
			parameter[0].plotcentre_lon=-144.9529067;
		} else if (plotcodeNum == 3022) /* ### FOR EN23-689 ### */  {
			parameter[0].plotcentre_lat=61.1167429;
			parameter[0].plotcentre_lon=-145.7304089;
		} else if (plotcodeNum == 3023) /* ### FOR EN23-700 ### */ {
			parameter[0].plotcentre_lat=61.8017931;
			parameter[0].plotcentre_lon=-147.6573713;
		} else if (plotcodeNum == 3024) /* ### FOR PA1 ### */  {
			parameter[0].plotcentre_lat=68.92548017;
			parameter[0].plotcentre_lon=-149.16170409;
		} else if (plotcodeNum == 3025) /* ### FOR PA10 ### */ {
			parameter[0].plotcentre_lat=61.34178219;
			parameter[0].plotcentre_lon=-162.76744883;
		} else if (plotcodeNum == 3026) /* ### FOR PA11 ### */ {
			parameter[0].plotcentre_lat=66.82779915;
			parameter[0].plotcentre_lon=-162.335186;
		} else if (plotcodeNum == 3027) /* ### FOR PA2 ### */  {
			parameter[0].plotcentre_lat=69.91016943;
			parameter[0].plotcentre_lon=-149.19466308;
		} else if (plotcodeNum == 3028) /* ### FOR PA3 ### */ {
			parameter[0].plotcentre_lat=69.94857903;
			parameter[0].plotcentre_lon=-148.53960326;
		} else if (plotcodeNum == 3029) /* ### FOR PA4 ### */  {
			parameter[0].plotcentre_lat=69.0607385;
			parameter[0].plotcentre_lon=-157.55860618;
		} else if (plotcodeNum == 3030) /* ### FOR PA5 ### */ {
			parameter[0].plotcentre_lat=69.74447646;
			parameter[0].plotcentre_lon=-157.28755286;
		} else if (plotcodeNum == 3031) /* ### FOR PA6 ### */ {
			parameter[0].plotcentre_lat=71.27344519;
			parameter[0].plotcentre_lon=-156.69944098;
		} else if (plotcodeNum == 3032) /* ### FOR PA7 ### */ {
			parameter[0].plotcentre_lat=65.40210016;
			parameter[0].plotcentre_lon=-164.69980529;
		} else if (plotcodeNum == 3033) /* ### FOR PA8 ### */ {
			parameter[0].plotcentre_lat=65.41874357;
			parameter[0].plotcentre_lon=-161.73023513;
		} else if (plotcodeNum == 3034) /* ### FOR PA9 ### */ {
			parameter[0].plotcentre_lat=66.04565766;
			parameter[0].plotcentre_lon=-153.81367724;
		} else if (plotcodeNum == 4001) /* ### FOR 2023051101 ### */  {
			parameter[0].plotcentre_lat=28.21899444;
			parameter[0].plotcentre_lon=98.20843056;
		} else if (plotcodeNum == 4002) /* ### FOR 2023051102 ### */  {
			parameter[0].plotcentre_lat=28.20012222;
			parameter[0].plotcentre_lon=98.22613889;
		} else if (plotcodeNum == 4003) /* ### FOR 2023051103 ### */  {
			parameter[0].plotcentre_lat=28.12416667;
			parameter[0].plotcentre_lon=98.32101667;
		} else if (plotcodeNum == 4004) /* ### FOR 2023051201 ### */  {
			parameter[0].plotcentre_lat=27.67631944;
			parameter[0].plotcentre_lon=98.27099167;
		} else if (plotcodeNum == 4005) /* ### FOR 2023051202 ### */  {
			parameter[0].plotcentre_lat=27.67048889;
			parameter[0].plotcentre_lon=98.30581944;
		} else if (plotcodeNum == 4006) /* ### FOR 2023051203 ### */  {
			parameter[0].plotcentre_lat=27.67881389;
			parameter[0].plotcentre_lon=98.29709722;
		} else if (plotcodeNum == 4007) /* ### FOR 2023051301 ### */  {
			parameter[0].plotcentre_lat=27.95829722;
			parameter[0].plotcentre_lon=98.62190278;
		} else if (plotcodeNum == 4008) /* ### FOR 2023051302 ### */  {
			parameter[0].plotcentre_lat=27.96491667;
			parameter[0].plotcentre_lon=98.64077778;
		} else if (plotcodeNum == 4009) /* ### FOR 2023051401 ### */  {
			parameter[0].plotcentre_lat=28.21930833;
			parameter[0].plotcentre_lon=98.49196389;
		} else if (plotcodeNum == 4010) /* ### FOR 2023051402 ### */  {
			parameter[0].plotcentre_lat=28.4191;
			parameter[0].plotcentre_lon=98.457325;
		} else if (plotcodeNum == 4011) /* ### FOR 2023051701 ### */  {
			parameter[0].plotcentre_lat=29.25366111;
			parameter[0].plotcentre_lon=102.2458889;
		} else if (plotcodeNum == 4012) /* ### FOR 2023051702 ### */  {
			parameter[0].plotcentre_lat=29.62946667;
			parameter[0].plotcentre_lon=102.8874972;
		} else if (plotcodeNum == 4019) /* ### FOR 2023051808 ### */  {
			parameter[0].plotcentre_lat=29.74629167;
			parameter[0].plotcentre_lon=102.0643306;
		} else if (plotcodeNum == 4013) /* ### FOR 2023051801 ### */  {
			parameter[0].plotcentre_lat=29.74629167;
			parameter[0].plotcentre_lon=102.0643306;
		} else if (plotcodeNum == 4014) /* ### FOR 2023051802 ### */  {
			parameter[0].plotcentre_lat=29.91957222;
			parameter[0].plotcentre_lon=101.9994944;
		} else if (plotcodeNum == 4015) /* ### FOR 2023051803 ### */  {
			parameter[0].plotcentre_lat=29.90294444;
			parameter[0].plotcentre_lon=101.9982667;
		} else if (plotcodeNum == 4016) /* ### FOR 2023051804 ### */  {
			parameter[0].plotcentre_lat=29.920425;
			parameter[0].plotcentre_lon=102.0106778;
		} else if (plotcodeNum == 4017) /* ### FOR 2023051806 ### */  {
			parameter[0].plotcentre_lat=29.87428333;
			parameter[0].plotcentre_lon=102.0222306;
		} else if (plotcodeNum == 4018) /* ### FOR 2023051807 ### */  {
			parameter[0].plotcentre_lat=29.83933611;
			parameter[0].plotcentre_lon=102.0426861;
		} else if (plotcodeNum == 4020) /* ### FOR 2023051901 ### */  {
			parameter[0].plotcentre_lat=29.845275;
			parameter[0].plotcentre_lon=102.2542528;
		} else if (plotcodeNum == 4021) /* ### FOR 2023051902 ### */  {
			parameter[0].plotcentre_lat=29.86599444;
			parameter[0].plotcentre_lon=102.3142778;
		} else if (plotcodeNum == 4022) /* ### FOR 2023052101 ### */  {
			parameter[0].plotcentre_lat=30.71568333;
			parameter[0].plotcentre_lon=102.7224;
		} else if (plotcodeNum == 4023) /* ### FOR 2023052102 ### */  {
			parameter[0].plotcentre_lat=30.72266944;
			parameter[0].plotcentre_lon=102.7203444;
		} else if (plotcodeNum == 4024) /* ### FOR 2023052103 ### */  {
			parameter[0].plotcentre_lat=30.76291667;
			parameter[0].plotcentre_lon=102.677575;
		} else if (plotcodeNum == 4025) /* ### FOR 2023052104 ### */  {
			parameter[0].plotcentre_lat=30.76241111;
			parameter[0].plotcentre_lon=102.6777972;
		} else if (plotcodeNum == 4026) /* ### FOR 2023052106 ### */  {
			parameter[0].plotcentre_lat=30.73776389;
			parameter[0].plotcentre_lon=102.7145;
		} else if (plotcodeNum == 4027) /* ### FOR 2023052107 ### */  {
			parameter[0].plotcentre_lat=30.73458056;
			parameter[0].plotcentre_lon=102.7252806;
		} else if (plotcodeNum == 4028) /* ### FOR 2023052301 ### */  {
			parameter[0].plotcentre_lat=31.65148889;
			parameter[0].plotcentre_lon=103.8572056;
		} else if (plotcodeNum == 4029) /* ### FOR 2023052302 ### */  {
			parameter[0].plotcentre_lat=31.64728611;
			parameter[0].plotcentre_lon=103.8397278;
		} else if (plotcodeNum == 4030) /* ### FOR 2023052401 ### */  {
			parameter[0].plotcentre_lat=32.51047778;
			parameter[0].plotcentre_lon=103.940725;
		} else if (plotcodeNum == 4031) /* ### FOR 2023052402 ### */  {
			parameter[0].plotcentre_lat=32.51106389;
			parameter[0].plotcentre_lon=103.941225;
		} else if (plotcodeNum == 4032) /* ### FOR 2023052403 ### */  {
			parameter[0].plotcentre_lat=32.51140556;
			parameter[0].plotcentre_lon=103.9513694;
		} else if (plotcodeNum == 4033) /* ### FOR 2023052404 ### */  {
			parameter[0].plotcentre_lat=32.50629722;
			parameter[0].plotcentre_lon=104.0105417;
		} else if (plotcodeNum == 4034) /* ### FOR 2023052501 ### */  {
			parameter[0].plotcentre_lat=32.33987778;
			parameter[0].plotcentre_lon=104.5814056;
		} else if (plotcodeNum == 4035) /* ### FOR 2023052601 ### */  {
			parameter[0].plotcentre_lat=32.74888056;
			parameter[0].plotcentre_lon=103.7943056;
		} else if (plotcodeNum == 4036) /* ### FOR 2023052602 ### */  {
			parameter[0].plotcentre_lat=32.760025;
			parameter[0].plotcentre_lon=103.840975;
		} else if (plotcodeNum == 4037) /* ### FOR 2023052604 ### */ {
			parameter[0].plotcentre_lat=32.74055278;
			parameter[0].plotcentre_lon=103.7429444;
		} else if (plotcodeNum == 4038) /* ### FOR 2023052802 ### */  {
			parameter[0].plotcentre_lat=31.70729722;
			parameter[0].plotcentre_lon=102.3124139;
		} else if (plotcodeNum == 4039) /* ### FOR 2023052803 ### */  {
			parameter[0].plotcentre_lat=31.708625;
			parameter[0].plotcentre_lon=102.3022806;
		} else if (plotcodeNum == 4040) /* ### FOR 2023052804 ### */  {
			parameter[0].plotcentre_lat=31.72001667;
			parameter[0].plotcentre_lon=102.2877833;
		} else if (plotcodeNum == 4041) /* ### FOR 2023052805 ### */  {
			parameter[0].plotcentre_lat=31.73401389;
			parameter[0].plotcentre_lon=102.2819583;
		} else if (plotcodeNum == 4042) /* ### FOR 2023052806 ### */  {
			parameter[0].plotcentre_lat=31.74371389;
			parameter[0].plotcentre_lon=102.2644417;
		} else if (plotcodeNum == 4043) /* ### FOR 2023052807 ### */  {
			parameter[0].plotcentre_lat=31.81218056;
			parameter[0].plotcentre_lon=102.2783167;
		} else if (plotcodeNum == 4044) /* ### FOR 2023052901 ### */  {
			parameter[0].plotcentre_lat=31.53228333;
			parameter[0].plotcentre_lon=102.1535083;
		} else if (plotcodeNum == 4045) /* ### FOR 2023052902 ### */  {
			parameter[0].plotcentre_lat=31.54245833;
			parameter[0].plotcentre_lon=102.162475;
		} else if (plotcodeNum == 4046) /* ### FOR 2023052903 ### */  {
			parameter[0].plotcentre_lat=31.544675;
			parameter[0].plotcentre_lon=102.1713528;
		} else if (plotcodeNum == 4047) /* ### FOR 2023053001 ### */  {
			parameter[0].plotcentre_lat=30.94997222;
			parameter[0].plotcentre_lon=101.9307778;
		} else if (plotcodeNum == 4048) /* ### FOR 2023053002 ### */  {
			parameter[0].plotcentre_lat=30.94311667;
			parameter[0].plotcentre_lon=101.93185;
		} else if (plotcodeNum == 4049) /* ### FOR 2023053003 ### */  {
			parameter[0].plotcentre_lat=30.93399167;
			parameter[0].plotcentre_lon=101.9394917;
		} else if (plotcodeNum == 4050) /* ### FOR 2023060101 ### */  {
			parameter[0].plotcentre_lat=29.47514167;
			parameter[0].plotcentre_lon=100.2192944;
		} else if (plotcodeNum == 4051) /* ### FOR 2023060102 ### */ {
			parameter[0].plotcentre_lat=29.47397778;
			parameter[0].plotcentre_lon=100.2206889;
		} else if (plotcodeNum == 4052) /* ### FOR 2023060105 ### */  {
			parameter[0].plotcentre_lat=29.82456667;
			parameter[0].plotcentre_lon=100.3468222;
		} else if (plotcodeNum == 4053) /* ### FOR 2023060106 ### */  {
			parameter[0].plotcentre_lat=29.90984444;
			parameter[0].plotcentre_lon=100.3141583;
		} else if (plotcodeNum == 4054) /* ### FOR 2023060201 ### */  {
			parameter[0].plotcentre_lat=29.93364167;
			parameter[0].plotcentre_lon=100.3019944;
		} else if (plotcodeNum == 4055) /* ### FOR 2023060203 ### */  {
			parameter[0].plotcentre_lat=29.93364167;
			parameter[0].plotcentre_lon=100.3019944;
		} else if (plotcodeNum == 4056) /* ### FOR 2023060204 ### */ {
			parameter[0].plotcentre_lat=29.93364167;
			parameter[0].plotcentre_lon=100.3019944;
		} else if (plotcodeNum == 4057) /* ### FOR 2023060301 ### */  {
			parameter[0].plotcentre_lat=30.02853889;
			parameter[0].plotcentre_lon=97.44019722;
		} else if (plotcodeNum == 4058) /* ### FOR 2023060502 ### */  {
			parameter[0].plotcentre_lat=29.69742778;
			parameter[0].plotcentre_lon=95.52143889;
		} else if (plotcodeNum == 4059) /* ### FOR 2023060503 ### */ {
			parameter[0].plotcentre_lat=29.71355556;
			parameter[0].plotcentre_lon=95.59481111;
		} else if (plotcodeNum == 4060) /* ### FOR 2023060504 ### */  {
			parameter[0].plotcentre_lat=29.72651667;
			parameter[0].plotcentre_lon=95.64760278;
		} else if (plotcodeNum == 4061) /* ### FOR 2023060505 ### */  {
			parameter[0].plotcentre_lat=29.78323333;
			parameter[0].plotcentre_lon=95.69748056;
		} else if (plotcodeNum == 4062) /* ### FOR 2023060601 ### */ {
			parameter[0].plotcentre_lat=29.783275;
			parameter[0].plotcentre_lon=95.69740833;
		} else if (plotcodeNum == 4063) /* ### FOR 2023060603 ### */  {
			parameter[0].plotcentre_lat=29.48331944;
			parameter[0].plotcentre_lon=96.65973333;
		} else if (plotcodeNum == 4064) /* ### FOR 2023060604 ### */  {
			parameter[0].plotcentre_lat=29.39223889;
			parameter[0].plotcentre_lon=96.84980556;
		} else if (plotcodeNum == 4065) /* ### FOR 2023060701 ### */  {
			parameter[0].plotcentre_lat=28.64443611;
			parameter[0].plotcentre_lon=97.43881944;
		} else if (plotcodeNum == 4066) /* ### FOR 2023060702 ### */  {
			parameter[0].plotcentre_lat=28.79652778;
			parameter[0].plotcentre_lon=97.54050278;
		} else if (plotcodeNum == 4067) /* ### FOR 2023060703 ### */  {
			parameter[0].plotcentre_lat=28.79363611;
			parameter[0].plotcentre_lon=97.58978611;
		} else if (plotcodeNum == 4068) /* ### FOR 2023060704 ### */  {
			parameter[0].plotcentre_lat=28.798875;
			parameter[0].plotcentre_lon=97.61254722;
		} else if (plotcodeNum == 4069) /* ### FOR 2023060705 ### */  {
			parameter[0].plotcentre_lat=28.80268611;
			parameter[0].plotcentre_lon=97.60941111;
		} else if (plotcodeNum == 4070) /* ### FOR 2023060706 ### */  {
			parameter[0].plotcentre_lat=28.76426389;
			parameter[0].plotcentre_lon=97.65651111;
		} else if (plotcodeNum == 4071) /* ### FOR 2023060802 ### */  {
			parameter[0].plotcentre_lat=28.61346389;
			parameter[0].plotcentre_lon=98.08276389;
		} else if (plotcodeNum == 4072) /* ### FOR 2023060803 ### */ {
			parameter[0].plotcentre_lat=28.56665278;
			parameter[0].plotcentre_lon=98.16480833;
		} else if (plotcodeNum == 5008) /* ### FOR Mucho_Lake ### */ {
			parameter[0].plotcentre_lat=59.0452645;
			parameter[0].plotcentre_lon=-125.7761635;
		} else if (plotcodeNum == 5001) /* ### FOR Close_to_Fairbanks ### */ {
			parameter[0].plotcentre_lat=64.7666905;
			parameter[0].plotcentre_lon=-148.2587085;
		} else if (plotcodeNum == 5009) /* ### FOR Road_to_Central ### */  {
			parameter[0].plotcentre_lat=65.44367;
			parameter[0].plotcentre_lon=-145.4957055;
		} else if (plotcodeNum == 5002) /* ### FOR Fielding_Lake ### */ {
			parameter[0].plotcentre_lat=63.198193;
			parameter[0].plotcentre_lon=-145.630242;
		} else if (plotcodeNum == 5007) /* ### FOR Lake_Ulu ### */ {
			parameter[0].plotcentre_lat=63.486946;
			parameter[0].plotcentre_lon=140.351655;
		} else if (plotcodeNum == 5006) /* ### FOR Lake_Nutenvut ### */ {
			parameter[0].plotcentre_lat=65.952533;
			parameter[0].plotcentre_lon=166.3179845;
		} else if (plotcodeNum == 5003) /* ### FOR Fort_Mc_Pherson_East ### */  {
			parameter[0].plotcentre_lat=67.3880265;
			parameter[0].plotcentre_lon=-134.2122725;
		} else if (plotcodeNum == 5004) /* ### FOR Fort_Mc_Pherson_West ### */ {
			parameter[0].plotcentre_lat=67.160952;
			parameter[0].plotcentre_lon=-135.9338975;
		} else if (plotcodeNum == 5005) /* ### FOR Lake_Illerney ### */ {
			parameter[0].plotcentre_lat=67.3689775;
			parameter[0].plotcentre_lon=168.3146835;
		} else {
			cout << "Warning: No coordinates specified!" << endl;
		}
	}
	
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

	cout << "Latitude of plot defined as: " << parameter[0].plotcentre_lat << endl;
	cout << "Longitude of plot defined as: " << parameter[0].plotcentre_lon  << endl;

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
