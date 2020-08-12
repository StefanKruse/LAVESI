#include "LAVESI.h"

using namespace std;

// TODO temporary
extern vector<vector<Weather*>> world_weather_list;
extern vector<double> wdir, wspd;
extern vector<vector<double>> windspd;
extern vector<vector<double>> winddir;
extern vector<int> globalyears;

void getTemp1(int aktort, char dateinametemp[50], vector<Weather*>& weather_list) {
    int aktortyworldcoo = (int)floor((double)(aktort - 1) / parameter[0].mapxlength);
    int aktortxworldcoo = (aktort - 1) - (aktortyworldcoo * parameter[0].mapxlength);

    if (parameter[0].mapylength > 1 && parameter[0].weathercalcgradient == true) {
        double Nposcenter = (parameter[0].nposmax + parameter[0].nposmin) / 2;
        double mapylengthdummy = parameter[0].mapylength;
        double Nposakt = parameter[0].nposmax - ((parameter[0].nposmax - parameter[0].nposmin) * aktortyworldcoo / (mapylengthdummy - 1.0));

        parameter[0].tempdiffort = -0.3508 * (Nposakt - Nposcenter);
        parameter[0].precdiffort = -5.3699 * (Nposakt - Nposcenter);
    }

    if (parameter[0].lineartransect == true) {
        parameter[0].tempdiffortmin = -0.3508 * treerows / (111120);
        parameter[0].precdiffortmin = -5.3699 * treerows / (111120);
    }

    FILE* f;
    f = fopen(dateinametemp, "r");
    if (f == NULL) {
        printf("Temperature file not available!\n");
        exit(1);
    }

    char puffer[255];
    int counter = 1;
    double tempyearmeanbuf, temp1monthmeanbuf, temp2monthmeanbuf, temp3monthmeanbuf, temp4monthmeanbuf, temp5monthmeanbuf, temp6monthmeanbuf, temp7monthmeanbuf,
        temp8monthmeanbuf, temp9monthmeanbuf, temp10monthmeanbuf, temp11monthmeanbuf, temp12monthmeanbuf;

    // read in line by line, partially preprocess data
    while (fgets(puffer, 255, f) != NULL) {
        if (counter >= 2) {
            temp1monthmeanbuf = strtod(strtok(puffer, " "), NULL);
            temp2monthmeanbuf = strtod(strtok(NULL, " "), NULL);
            temp3monthmeanbuf = strtod(strtok(NULL, " "), NULL);
            temp4monthmeanbuf = strtod(strtok(NULL, " "), NULL);
            temp5monthmeanbuf = strtod(strtok(NULL, " "), NULL);
            temp6monthmeanbuf = strtod(strtok(NULL, " "), NULL);
            temp7monthmeanbuf = strtod(strtok(NULL, " "), NULL);
            temp8monthmeanbuf = strtod(strtok(NULL, " "), NULL);
            temp9monthmeanbuf = strtod(strtok(NULL, " "), NULL);
            temp10monthmeanbuf = strtod(strtok(NULL, " "), NULL);
            temp11monthmeanbuf = strtod(strtok(NULL, " "), NULL);
            temp12monthmeanbuf = strtod(strtok(NULL, " "), NULL);

            tempyearmeanbuf = (temp1monthmeanbuf + temp2monthmeanbuf + temp3monthmeanbuf + temp4monthmeanbuf + temp5monthmeanbuf + temp6monthmeanbuf
                               + temp7monthmeanbuf + temp8monthmeanbuf + temp9monthmeanbuf + temp10monthmeanbuf + temp11monthmeanbuf + temp12monthmeanbuf)
                              / 12;

            auto pWeather = new Weather();

            pWeather->yworldcoo = aktortyworldcoo;
            pWeather->xworldcoo = aktortxworldcoo;
            pWeather->jahr = counter + parameter[0].startjahr - 2;
            pWeather->tempyearmean = tempyearmeanbuf + parameter[0].tempdiffort;
            pWeather->temp1monthmean = temp1monthmeanbuf + parameter[0].tempdiffort;
            pWeather->temp1monthmeanmin = temp1monthmeanbuf + parameter[0].tempdiffort + parameter[0].tempdiffortmin;
            pWeather->temp2monthmean = temp2monthmeanbuf + parameter[0].tempdiffort;
            pWeather->temp3monthmean = temp3monthmeanbuf + parameter[0].tempdiffort;
            pWeather->temp4monthmean = temp4monthmeanbuf + parameter[0].tempdiffort;
            pWeather->temp5monthmean = temp5monthmeanbuf + parameter[0].tempdiffort;
            pWeather->temp6monthmean = temp6monthmeanbuf + parameter[0].tempdiffort;
            pWeather->temp7monthmean = temp7monthmeanbuf + parameter[0].tempdiffort;
            pWeather->temp7monthmeanmin = temp7monthmeanbuf + parameter[0].tempdiffort + parameter[0].tempdiffortmin;
            pWeather->temp8monthmean = temp8monthmeanbuf + parameter[0].tempdiffort;
            pWeather->temp9monthmean = temp9monthmeanbuf + parameter[0].tempdiffort;
            pWeather->temp10monthmean = temp10monthmeanbuf + parameter[0].tempdiffort;
            pWeather->temp11monthmean = temp11monthmeanbuf + parameter[0].tempdiffort;
            pWeather->temp12monthmean = temp12monthmeanbuf + parameter[0].tempdiffort;

            double sumacttemp = 0, sumacttempmin = 0, sumdegreday = 0;
            int ndegreday = 0, ndegredaymin = 0;

            // daily mean temperature estimation from mean July temperature:
            double julitemp = pWeather->temp7monthmean;
            double julitempmin = pWeather->temp7monthmeanmin;

            for (int i = 1; i <= 365; i++) {
                double tagestemp = 0;

                if (parameter[0].weatherchoice < 10) {
                    tagestemp = ((20.5 * sin((0.61 * ((double)i / (365.0 / 12.0))) + 3.8)) - 21.0 + julitemp);
                } else {
                    tagestemp = (julitemp + 24 * sin((2 * M_PI) / 365 * (i - 100))) - 25;
                }

                if (tagestemp > 0) {
                    ndegreday++;
                    sumdegreday += tagestemp;
                    if (tagestemp > 10) {
                        sumacttemp = sumacttemp + tagestemp;
                    }
                }
                double tagestempmin = tagestemp - julitemp + julitempmin;
                if (tagestempmin > 0) {
                    ndegredaymin++;
                    if (tagestempmin > 10) {
                        sumacttempmin = sumacttempmin + tagestempmin;
                    }
                }
            }
            pWeather->activeairtemp = sumacttemp;
            pWeather->vegetationperiodlength = ndegreday;
            pWeather->activeairtempmin = sumacttempmin;
            pWeather->vegetationperiodlengthmin = ndegredaymin;
            pWeather->degreday = sumdegreday;

            weather_list.push_back(pWeather);
        }
        counter++;
    }

    fclose(f);
}

void getPrec1(char dateinameprec[50], vector<Weather*>& weather_list, int maximal_word_length) {
    FILE* fp;
    fp = fopen(dateinameprec, "r");

    if (fp == NULL) {
        printf("Precipitation file not available!\n");
        exit(1);
    }

    int counter = 1;
    char puffer[255];
    double precipitationsumbuf;
    double prec1monthmeanbuf, prec2monthmeanbuf, prec3monthmeanbuf, prec4monthmeanbuf, prec5monthmeanbuf, prec6monthmeanbuf, prec7monthmeanbuf,
        prec8monthmeanbuf, prec9monthmeanbuf, prec10monthmeanbuf, prec11monthmeanbuf, prec12monthmeanbuf;

    while (fgets(puffer, maximal_word_length, fp) != NULL) {
        if (counter >= 2) {
            prec1monthmeanbuf = strtod(strtok(puffer, " "), NULL);
            prec2monthmeanbuf = strtod(strtok(NULL, " "), NULL);
            prec3monthmeanbuf = strtod(strtok(NULL, " "), NULL);
            prec4monthmeanbuf = strtod(strtok(NULL, " "), NULL);
            prec5monthmeanbuf = strtod(strtok(NULL, " "), NULL);
            prec6monthmeanbuf = strtod(strtok(NULL, " "), NULL);
            prec7monthmeanbuf = strtod(strtok(NULL, " "), NULL);
            prec8monthmeanbuf = strtod(strtok(NULL, " "), NULL);
            prec9monthmeanbuf = strtod(strtok(NULL, " "), NULL);
            prec10monthmeanbuf = strtod(strtok(NULL, " "), NULL);
            prec11monthmeanbuf = strtod(strtok(NULL, " "), NULL);
            prec12monthmeanbuf = strtod(strtok(NULL, " "), NULL);

            precipitationsumbuf = (prec1monthmeanbuf + prec2monthmeanbuf + prec3monthmeanbuf + prec4monthmeanbuf + prec5monthmeanbuf + prec6monthmeanbuf + prec7monthmeanbuf + prec8monthmeanbuf + prec9monthmeanbuf + prec10monthmeanbuf + prec11monthmeanbuf + prec12monthmeanbuf);

            auto pWeather = weather_list[counter - 2];

            pWeather->prec1monthmean = prec1monthmeanbuf + parameter[0].precdiffort / 12;
            pWeather->prec2monthmean = prec2monthmeanbuf + parameter[0].precdiffort / 12;
            pWeather->prec3monthmean = prec3monthmeanbuf + parameter[0].precdiffort / 12;
            pWeather->prec4monthmean = prec4monthmeanbuf + parameter[0].precdiffort / 12;
            pWeather->prec5monthmean = prec5monthmeanbuf + parameter[0].precdiffort / 12;
            pWeather->prec6monthmean = prec6monthmeanbuf + parameter[0].precdiffort / 12;
            pWeather->prec7monthmean = prec7monthmeanbuf + parameter[0].precdiffort / 12;
            pWeather->prec8monthmean = prec8monthmeanbuf + parameter[0].precdiffort / 12;
            pWeather->prec9monthmean = prec9monthmeanbuf + parameter[0].precdiffort / 12;
            pWeather->prec10monthmean = prec10monthmeanbuf + parameter[0].precdiffort / 12;
            pWeather->prec11monthmean = prec11monthmeanbuf + parameter[0].precdiffort / 12;
            pWeather->prec12monthmean = prec12monthmeanbuf + parameter[0].precdiffort / 12;
			
            pWeather->prec1monthmeanmin = prec1monthmeanbuf + parameter[0].precdiffort / 12 + parameter[0].precdiffortmin / 12;
            pWeather->prec2monthmeanmin = prec2monthmeanbuf + parameter[0].precdiffort / 12 + parameter[0].precdiffortmin / 12;
            pWeather->prec3monthmeanmin = prec3monthmeanbuf + parameter[0].precdiffort / 12 + parameter[0].precdiffortmin / 12;
            pWeather->prec4monthmeanmin = prec4monthmeanbuf + parameter[0].precdiffort / 12 + parameter[0].precdiffortmin / 12;
            pWeather->prec5monthmeanmin = prec5monthmeanbuf + parameter[0].precdiffort / 12 + parameter[0].precdiffortmin / 12;
            pWeather->prec6monthmeanmin = prec6monthmeanbuf + parameter[0].precdiffort / 12 + parameter[0].precdiffortmin / 12;
            pWeather->prec7monthmeanmin = prec7monthmeanbuf + parameter[0].precdiffort / 12 + parameter[0].precdiffortmin / 12;
            pWeather->prec8monthmeanmin = prec8monthmeanbuf + parameter[0].precdiffort / 12 + parameter[0].precdiffortmin / 12;
            pWeather->prec9monthmeanmin = prec9monthmeanbuf + parameter[0].precdiffort / 12 + parameter[0].precdiffortmin / 12;
            pWeather->prec10monthmeanmin = prec10monthmeanbuf + parameter[0].precdiffort / 12 + parameter[0].precdiffortmin / 12;
            pWeather->prec11monthmeanmin = prec11monthmeanbuf + parameter[0].precdiffort / 12 + parameter[0].precdiffortmin / 12;
            pWeather->prec12monthmeanmin = prec12monthmeanbuf + parameter[0].precdiffort / 12 + parameter[0].precdiffortmin / 12;
            
			pWeather->precipitationsum = precipitationsumbuf + parameter[0].precdiffort;
			pWeather->precipitationsummin = precipitationsumbuf + parameter[0].precdiffort + parameter[0].precdiffortmin;
        }
        counter++;
    }

    fclose(fp);
}

void getTemp2(int aktort, char dateinametemp[50], vector<Weather*>& weather_list, int maximal_word_length) {
    int aktortyworldcoo = (int)floor((double)(aktort - 1) / parameter[0].mapxlength);
    int aktortxworldcoo = (aktort - 1) - (aktortyworldcoo * parameter[0].mapxlength);

    if (parameter[0].mapylength > 1 && parameter[0].weathercalcgradient == true) {
        double Nposcenter = (parameter[0].nposmax + parameter[0].nposmin) / 2;

        double mapylengthdummy = parameter[0].mapylength;
        double Nposakt = parameter[0].nposmax - ((parameter[0].nposmax - parameter[0].nposmin) * aktortyworldcoo / (mapylengthdummy - 1.0));

        parameter[0].tempdiffort = -0.3508 * (Nposakt - Nposcenter);
        parameter[0].precdiffort = -5.3699 * (Nposakt - Nposcenter);
    } else {
        parameter[0].tempdiffort = 0.0;
        parameter[0].precdiffort = 0.0;
    }

    if (parameter[0].lineartransect == true) {
        parameter[0].tempdiffortmin = -0.3508 * treerows / (111120);
        parameter[0].precdiffortmin = -5.3699 * treerows / (111120);
    }

    FILE* f;
    f = fopen(dateinametemp, "r");
    if (f == NULL) {
        printf("Temperature file not available!\n");
        exit(1);
    }

    char puffer[255];
    int counter = 1;
    double jahrbuf, tempyearmeanbuf;

    // read in line by line, partially preprocess data
    while (fgets(puffer, maximal_word_length, f) != NULL) {
        if (counter >= 2) {
            jahrbuf = strtod(strtok(puffer, " "), NULL);
            tempyearmeanbuf = strtod(strtok(NULL, " "), NULL);

            auto pWeather = new Weather();

            pWeather->yworldcoo = aktortyworldcoo;
            pWeather->xworldcoo = aktortxworldcoo;
            pWeather->jahr = (int)floor(jahrbuf);
            pWeather->tempyearmean = tempyearmeanbuf + parameter[0].tempdiffort;
            pWeather->tempyearmeanmin = tempyearmeanbuf + parameter[0].tempdiffort + parameter[0].tempdiffortmin;

            double sumacttemp = 0;
            double sumacttempmin = 0;
            double sumdegreday = 0;
            int ndegreday = 0;
            int ndegredaymin = 0;

            double tempyearmeanbuf = pWeather->tempyearmean;
            double tempyearmeanminbuf = pWeather->tempyearmeanmin;
            double temp1monthmeanbuf = 0, temp1monthmeanminbuf = 0;
            double temp7monthmeanbuf = 0, temp7monthmeanminbuf = 0;

            // parameters of the sine function for daily temperature estimation
            double ausdehnung = 0.567980348064239;
            double verschiebung = 4.03428152771763;
            double amplianstieg = -0.585543287044634;
            double ampliyschn = 15.5976677143297;
            double yabsanstieg = -0.881262021169085;
            double yabsschn = -0.00908805024019213;
            double ampli = 1.05 * (ampliyschn + amplianstieg * tempyearmeanbuf);
            double amplimin = 1.05 * (ampliyschn + amplianstieg * tempyearmeanminbuf);
            double yabs = yabsschn + yabsanstieg * tempyearmeanbuf;
            double yabsmin = yabsschn + yabsanstieg * tempyearmeanminbuf;

            for (int i = 1; i <= 365; i++) {
                double tagestemp = ((ampli * sin((ausdehnung * ((double)i / (365.0 / 12.0))) + verschiebung)) - yabs);
                if (tagestemp > 0) {
                    ndegreday++;
                    sumdegreday += tagestemp;
                    if (tagestemp > 10) {
                        sumacttemp = sumacttemp + tagestemp;
                    }
                }
                double tagestempmin = ((tagestemp + yabs) / ampli) * amplimin - yabsmin;
                if (tagestempmin > 0) {
                    ndegredaymin++;
                    if (tagestempmin > 10) {
                        sumacttempmin = sumacttempmin + tagestempmin;
                    }
                }

                if ((i >= 1) & (i <= 31)) {
                    temp1monthmeanbuf = temp1monthmeanbuf + tagestemp;
                    temp1monthmeanminbuf = temp1monthmeanminbuf + tagestempmin;
                }

                if ((i >= 182) & (i <= 212)) {
                    temp7monthmeanbuf = temp7monthmeanbuf + tagestemp;
                    temp7monthmeanminbuf = temp7monthmeanminbuf + tagestempmin;
                }
            }

            pWeather->activeairtemp = sumacttemp;
            pWeather->degreday = sumdegreday;
            pWeather->vegetationperiodlength = ndegreday;
            pWeather->activeairtempmin = sumacttempmin;
            pWeather->vegetationperiodlengthmin = ndegredaymin;
            pWeather->temp1monthmean = temp1monthmeanbuf / 31;
            pWeather->temp1monthmeanmin = temp1monthmeanminbuf / 31;
            pWeather->temp7monthmean = temp7monthmeanbuf / 31;
            pWeather->temp7monthmeanmin = temp7monthmeanminbuf / 31;

            weather_list.push_back(pWeather);
        }
        counter++;
    }

    fclose(f);
}

void getPrec2(char dateinameprec[50], vector<Weather*>& weather_list, int maximal_word_length) {
    FILE* fp;
    fp = fopen(dateinameprec, "r");
    if (fp == NULL) {
        printf("Niederschlagsdatei nicht vorhanden!\n");
        exit(1);
    }

    int counter = 1;
    char puffer[255];
    double precipitationsumbuf;
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

    while (fgets(puffer, maximal_word_length, fp) != NULL) {
        if (counter >= 2) {
            strtod(strtok(puffer, " "), NULL);
            prec1monthmeanbuf = strtod(strtok(NULL, " "), NULL);
            prec2monthmeanbuf = strtod(strtok(NULL, " "), NULL);
            prec3monthmeanbuf = strtod(strtok(NULL, " "), NULL);
            prec4monthmeanbuf = strtod(strtok(NULL, " "), NULL);
            prec5monthmeanbuf = strtod(strtok(NULL, " "), NULL);
            prec6monthmeanbuf = strtod(strtok(NULL, " "), NULL);
            prec7monthmeanbuf = strtod(strtok(NULL, " "), NULL);
            prec8monthmeanbuf = strtod(strtok(NULL, " "), NULL);
            prec9monthmeanbuf = strtod(strtok(NULL, " "), NULL);
            prec10monthmeanbuf = strtod(strtok(NULL, " "), NULL);
            prec11monthmeanbuf = strtod(strtok(NULL, " "), NULL);
            prec12monthmeanbuf = strtod(strtok(NULL, " "), NULL);

            precipitationsumbuf = (prec1monthmeanbuf + prec2monthmeanbuf + prec3monthmeanbuf + prec4monthmeanbuf + prec5monthmeanbuf + prec6monthmeanbuf
                                   + prec7monthmeanbuf + prec8monthmeanbuf + prec9monthmeanbuf + prec10monthmeanbuf + prec11monthmeanbuf + prec12monthmeanbuf);

            auto pWeather = weather_list[counter - 2];

            pWeather->prec1monthmean = prec1monthmeanbuf + parameter[0].precdiffort / 12;
            pWeather->prec2monthmean = prec2monthmeanbuf + parameter[0].precdiffort / 12;
            pWeather->prec3monthmean = prec3monthmeanbuf + parameter[0].precdiffort / 12;
            pWeather->prec4monthmean = prec4monthmeanbuf + parameter[0].precdiffort / 12;
            pWeather->prec5monthmean = prec5monthmeanbuf + parameter[0].precdiffort / 12;
            pWeather->prec6monthmean = prec6monthmeanbuf + parameter[0].precdiffort / 12;
            pWeather->prec7monthmean = prec7monthmeanbuf + parameter[0].precdiffort / 12;
            pWeather->prec8monthmean = prec8monthmeanbuf + parameter[0].precdiffort / 12;
            pWeather->prec9monthmean = prec9monthmeanbuf + parameter[0].precdiffort / 12;
            pWeather->prec10monthmean = prec10monthmeanbuf + parameter[0].precdiffort / 12;
            pWeather->prec11monthmean = prec11monthmeanbuf + parameter[0].precdiffort / 12;
            pWeather->prec12monthmean = prec12monthmeanbuf + parameter[0].precdiffort / 12;
            pWeather->precipitationsum = precipitationsumbuf + parameter[0].precdiffort;
            pWeather->precipitationsummin = precipitationsumbuf + parameter[0].precdiffort + parameter[0].precdiffortmin;
        }
        counter++;
    }

    fclose(fp);
}

void getTemp3(int aktort, char dateinametemp[50], vector<Weather*>& weather_list) {
    int aktortyworldcoo = (int)floor((double)(aktort - 1) / parameter[0].mapxlength);
    int aktortxworldcoo = (aktort - 1) - (aktortyworldcoo * parameter[0].mapxlength);

    if (parameter[0].mapylength > 1 && parameter[0].weathercalcgradient == true) {
        double Nposcenter = (parameter[0].nposmax + parameter[0].nposmin) / 2;
        double mapylengthdummy = parameter[0].mapylength;
        double Nposakt = parameter[0].nposmax - ((parameter[0].nposmax - parameter[0].nposmin) * aktortyworldcoo / (mapylengthdummy - 1.0));

        // conversion degrees to metres "1 degree = 60 x 1852 m = 111120 m"
        parameter[0].precdiffort = -5.3699 * (Nposakt - Nposcenter);
        parameter[0].tempdiffort = -0.3508 * (Nposakt - Nposcenter);
    }

    if (parameter[0].lineartransect == true) {
        // neu um die Grenze an eine stelle zu rücken. Mitte = eingeladene Serie
        // ... 2° N nach norden verschoben
        // parameter[0].precdiffort = -5.3699 * (2.0);
        // parameter[0].tempdiffort = -0.3508 * (2.0);
        // parameter[0].precdiffort = 0.0;
        // parameter[0].tempdiffort = 0.0;

        // parameter[0].tempdiffortmin = -0.3508 * -1 * treerows / (111120);
        // parameter[0].precdiffortmin = -5.3699 * -1 * treerows / (111120);
		// TODO: clean up
    }

    FILE* f;
    f = fopen(dateinametemp, "r");
    if (f == NULL) {
        printf("Temperature file not available!\n");
        exit(1);
    }

    char puffer[255];
    int counter = 1;
    double tempyearmeanbuf, temp1monthmeanbuf, temp2monthmeanbuf, temp3monthmeanbuf, temp4monthmeanbuf, temp5monthmeanbuf, temp6monthmeanbuf, temp7monthmeanbuf,
        temp8monthmeanbuf, temp9monthmeanbuf, temp10monthmeanbuf, temp11monthmeanbuf, temp12monthmeanbuf;

    // read in line by line, partially preprocess data
    while (fgets(puffer, 255, f) != NULL) {
        if (counter >= 2) {
            temp1monthmeanbuf = strtod(strtok(puffer, " "), NULL);
            temp2monthmeanbuf = strtod(strtok(NULL, " "), NULL);
            temp3monthmeanbuf = strtod(strtok(NULL, " "), NULL);
            temp4monthmeanbuf = strtod(strtok(NULL, " "), NULL);
            temp5monthmeanbuf = strtod(strtok(NULL, " "), NULL);
            temp6monthmeanbuf = strtod(strtok(NULL, " "), NULL);
            temp7monthmeanbuf = strtod(strtok(NULL, " "), NULL);
            temp8monthmeanbuf = strtod(strtok(NULL, " "), NULL);
            temp9monthmeanbuf = strtod(strtok(NULL, " "), NULL);
            temp10monthmeanbuf = strtod(strtok(NULL, " "), NULL);
            temp11monthmeanbuf = strtod(strtok(NULL, " "), NULL);
            temp12monthmeanbuf = strtod(strtok(NULL, " "), NULL);

            tempyearmeanbuf = (temp1monthmeanbuf + temp2monthmeanbuf + temp3monthmeanbuf + temp4monthmeanbuf + temp5monthmeanbuf + temp6monthmeanbuf + temp7monthmeanbuf + temp8monthmeanbuf + temp9monthmeanbuf + temp10monthmeanbuf + temp11monthmeanbuf + temp12monthmeanbuf) / 12;

            auto pWeather = new Weather();

            pWeather->yworldcoo = aktortyworldcoo;
            pWeather->xworldcoo = aktortxworldcoo;
            pWeather->jahr = counter + parameter[0].startjahr - 2;
            pWeather->tempyearmean = tempyearmeanbuf + parameter[0].tempdiffort;
			if(parameter[0].tempjandiffort!=0.0)
				pWeather->temp1monthmean = temp1monthmeanbuf + parameter[0].tempjandiffort;
			else
				pWeather->temp1monthmean = temp1monthmeanbuf + parameter[0].tempdiffort;
			if(parameter[0].tempjandiffortmin!=0.0)
				pWeather->temp1monthmeanmin = temp1monthmeanbuf + parameter[0].tempjandiffort + parameter[0].tempjandiffortmin;
			else
				pWeather->temp1monthmeanmin = temp1monthmeanbuf + parameter[0].tempdiffort + parameter[0].tempdiffortmin;
            pWeather->temp2monthmean = temp2monthmeanbuf + parameter[0].tempdiffort;
            pWeather->temp3monthmean = temp3monthmeanbuf + parameter[0].tempdiffort;
            pWeather->temp4monthmean = temp4monthmeanbuf + parameter[0].tempdiffort;
            pWeather->temp5monthmean = temp5monthmeanbuf + parameter[0].tempdiffort;
            pWeather->temp6monthmean = temp6monthmeanbuf + parameter[0].tempdiffort;
			if(parameter[0].tempjuldiffortmin!=0.0)
				pWeather->temp7monthmean = temp7monthmeanbuf + parameter[0].tempjuldiffort;
			else
				pWeather->temp7monthmean = temp7monthmeanbuf + parameter[0].tempdiffort;
			if(parameter[0].tempjuldiffortmin!=0.0)
				pWeather->temp7monthmeanmin = temp7monthmeanbuf + parameter[0].tempjuldiffort + parameter[0].tempjuldiffortmin;
			else
				pWeather->temp7monthmeanmin = temp7monthmeanbuf + parameter[0].tempdiffort + parameter[0].tempdiffortmin;
            pWeather->temp8monthmean = temp8monthmeanbuf + parameter[0].tempdiffort;
            pWeather->temp9monthmean = temp9monthmeanbuf + parameter[0].tempdiffort;
            pWeather->temp10monthmean = temp10monthmeanbuf + parameter[0].tempdiffort;
            pWeather->temp11monthmean = temp11monthmeanbuf + parameter[0].tempdiffort;
            pWeather->temp12monthmean = temp12monthmeanbuf + parameter[0].tempdiffort;

            double sumacttemp = 0;
			double sumacttempmin = 0;
			double sumdegreday = 0;
			double sumdegredaymin = 0;
            int ndegreday = 0;
			int ndegredaymin = 0;
            double jantemp = pWeather->temp1monthmean;
            double jantempmin = pWeather->temp1monthmean;
            double julitemp = pWeather->temp7monthmean;
            double julitempmin = pWeather->temp7monthmeanmin;

            for (int i = 1; i <= 365; i++) {
                // calculate each day once for each January or July temperature oriented function
                double tagestempjan = 0.0;
                double tagestempjanmin = 0.0;
                double tagestempjul = 0.0;
                double tagestempjulmin = 0.0;
                double tagestemp = 0.0;
                double tagestempmin = 0.0;

                tagestempjan = ((22.55131 * sin(0.49102 * ((double)i / (365.0 / 12.0)) + 4.62730)) + 22.04378 + jantemp);
                tagestempjul = ((22.55261 * sin(0.49063 * ((double)i / (365.0 / 12.0)) + 4.62760)) - 22.27861 + julitemp);
                tagestempjanmin = ((22.55131 * sin(0.49102 * ((double)i / (365.0 / 12.0)) + 4.62730)) + 22.04378 + jantempmin);
                tagestempjulmin = ((22.55261 * sin(0.49063 * ((double)i / (365.0 / 12.0)) + 4.62760)) - 22.27861 + julitempmin);

                // merge daily temperatures of January and July year-functions
                double weight = 0.0;
                if ((i >= 1) & (i <= 15)) {
                    weight = ((double)1.0 * ((15.0 - i) / 184.0));
                }
                if ((i >= 16) & (i <= 196)) {
                    weight = ((double)1.0 * ((i - 15.0) / 181.0));
                }
                if ((i >= 197) & (i <= 365)) {
                    weight = ((double)1.0 * ((365.0 + 15.0 - i) / 184.0));
                }

                tagestemp = tagestempjan * (1 - weight) + tagestempjul * weight;
                if (tagestemp > 0) {
                    ndegreday++;
                    sumdegreday += tagestemp;
                    if (tagestemp > 10) {
                        sumacttemp = sumacttemp + tagestemp;
                    }
                }
				
                tagestempmin = tagestempjanmin * (1 - weight) + tagestempjulmin * weight;
                if (tagestempmin > 0) {
                    ndegredaymin++;
                    sumdegredaymin += tagestempmin;
                    if (tagestempmin > 10) {
                        sumacttempmin = sumacttempmin + tagestempmin;
                    }
                }
            }

            pWeather->activeairtemp = (int)round(sumacttemp * 0.84630);
            pWeather->activeairtempmin = (int)round(sumacttempmin * 0.84630);
            pWeather->vegetationperiodlength = (int)round(ndegreday * 0.88432);
            pWeather->vegetationperiodlengthmin = (int)round(ndegredaymin * 0.88432);
            pWeather->degreday = (int)round(sumdegreday * 0.87583);
            pWeather->degredaymin = (int)round(sumdegredaymin * 0.87583);

            weather_list.push_back(pWeather);
        }
        counter++;
    }
    fclose(f);
}

void passWeather() {
    for (vector<vector<Weather*>>::iterator posw = world_weather_list.begin(); posw != world_weather_list.end(); posw++) {
        vector<Weather*>& weather_list = *posw;

        for (unsigned int iweather = 0; iweather < weather_list.size(); ++iweather) {
            double julindexs = 0;
            double julindexmins = 0;
            double julindexg = 0;
            double julindexming = 0;
            julindexs = (0.163 / (1 + exp(12.319 - weather_list[iweather]->temp7monthmean))) + 0.168;
            julindexmins = (0.163 / (1 + exp(12.319 - weather_list[iweather]->temp7monthmeanmin))) + 0.168;
            julindexg = (0.078 / (1 + exp(14.825 - weather_list[iweather]->temp7monthmean))) + 0.108;
            julindexming = (0.078 / (1 + exp(14.825 - weather_list[iweather]->temp7monthmeanmin))) + 0.108;

            weather_list[iweather]->weatherfactorg = (double)(julindexg - 0.108) / (0.1771 - 0.108);
            weather_list[iweather]->weatherfactorming = (double)(julindexming - 0.108) / (0.1771 - 0.108);
            weather_list[iweather]->weatherfactors = (double)(julindexs - 0.168) / (0.305 - 0.168);
            weather_list[iweather]->weatherfactormins = (double)(julindexmins - 0.168) / (0.305 - 0.168);

            double jantempsum = 0;
            double jantempsummin = 0;
            double jultempsum = 0;
            double jultempsummin = 0;
            double nddsum = 0;
            double nddsummin = 0;

            if (iweather > 9) {
                for (int i = 1; i < (10 + 1); ++i) {
                    jantempsum += weather_list[iweather + i - 10]->temp1monthmean;
                    jantempsummin += weather_list[iweather + i - 10]->temp1monthmeanmin;
                    jultempsum += weather_list[iweather + i - 10]->temp7monthmean;
                    jultempsummin += weather_list[iweather + i - 10]->temp7monthmeanmin;
                    nddsum += weather_list[iweather + i - 10]->vegetationperiodlength;
                    nddsummin += weather_list[iweather + i - 10]->vegetationperiodlengthmin;
                }
                jantempsum = jantempsum / 10;
                jantempsummin = jantempsummin / 10;
                jultempsum = jultempsum / 10;
                jultempsummin = jultempsummin / 10;
                nddsum = nddsum / 10;
                nddsummin = nddsummin / 10;
            } else if (iweather > 0 && iweather <= 9) {
                for (unsigned int i = 0; i < (iweather + 1); ++i) {
                    jantempsum += weather_list[i]->temp1monthmean;
                    jantempsummin += weather_list[i]->temp1monthmeanmin;
                    jultempsum += weather_list[i]->temp7monthmean;
                    jultempsummin += weather_list[i]->temp7monthmeanmin;
                    nddsum += weather_list[i]->vegetationperiodlength;
                    nddsummin += weather_list[i]->vegetationperiodlengthmin;
                }
                jantempsum = jantempsum / (iweather + 1);
                jantempsummin = jantempsummin / (iweather + 1);
                jultempsum = jultempsum / (iweather + 1);
                jultempsummin = jultempsummin / (iweather + 1);
                nddsum = nddsum / (iweather + 1);
                nddsummin = nddsummin / (iweather + 1);
            } else {
                jantempsum = weather_list[iweather]->temp1monthmean;
                jantempsummin = weather_list[iweather]->temp1monthmeanmin;
                jultempsum = weather_list[iweather]->temp7monthmean;
                jultempsummin = weather_list[iweather]->temp7monthmeanmin;
                nddsum = weather_list[iweather]->vegetationperiodlength;
                nddsummin = weather_list[iweather]->vegetationperiodlengthmin;
            }
            weather_list[iweather]->temp1monthmeaniso = jantempsum;
            weather_list[iweather]->temp1monthmeanisomin = jantempsummin;
            weather_list[iweather]->temp7monthmeaniso = jultempsum;
            weather_list[iweather]->temp7monthmeanisomin = jultempsummin;
            weather_list[iweather]->vegetationperiodlengthiso = (int)floor(nddsum);
            weather_list[iweather]->vegetationperiodlengthisomin = (int)floor(nddsummin);

            // calculation of a drought influence
            double droughtmortbuf = 0.0;
            double droughtmortbufmin = 0.0;
            double precgs = weather_list[iweather]->prec5monthmean + weather_list[iweather]->prec6monthmean + weather_list[iweather]->prec7monthmean + weather_list[iweather]->prec8monthmean;
            double precgsmin = weather_list[iweather]->prec5monthmeanmin + weather_list[iweather]->prec6monthmeanmin + weather_list[iweather]->prec7monthmeanmin + weather_list[iweather]->prec8monthmeanmin;
            double duerreindex = parameter[0].evapod * weather_list[iweather]->vegetationperiodlength / precgs;
            double duerreindexmin = parameter[0].evapod * weather_list[iweather]->vegetationperiodlengthmin / precgsmin;
            if (duerreindex > 1) {
                double aattrockenheit = (weather_list[iweather]->activeairtemp / 300.0) - 1.0;

                if (aattrockenheit > 1) {
                    droughtmortbuf = droughtmortbuf + aattrockenheit * 0.1;
                }
            }
            weather_list[iweather]->droughtmort = droughtmortbuf;
            if (duerreindexmin > 1) {
                double aattrockenheitmin = (weather_list[iweather]->activeairtempmin / 300.0) - 1.0;

                if (aattrockenheitmin > 1) {
                    droughtmortbufmin = droughtmortbufmin + aattrockenheitmin * 0.1;
                }
            }
            weather_list[iweather]->droughtmortmin = droughtmortbufmin;

            if (parameter[0].qualiyearlyvis == true) {
                printf("	weather(%d; iweather=%d): weatherfactorg=%4.4f, weatherfactors=%4.4f ===> \ndroughtmort=%4.5f\n", weather_list[iweather]->jahr,
                       iweather, weather_list[iweather]->weatherfactorg, weather_list[iweather]->weatherfactors, weather_list[iweather]->droughtmort);
                printf("\tJanT10=%4.2f, JuliT10=%4.2f, NDD10=%d\n", weather_list[iweather]->temp1monthmeaniso, weather_list[iweather]->temp7monthmeaniso,
                       weather_list[iweather]->vegetationperiodlengthiso);
            }

            // calculate restrictions
            weather_list[iweather]->janisothermrestriktiong = 0.0;
            weather_list[iweather]->janisothermrestriktiongmin = 0.0;
            weather_list[iweather]->janisothermrestriktions = 0.0;
            weather_list[iweather]->janisothermrestriktionsmin = 0.0;
            weather_list[iweather]->julisothermrestriktion = 0.0;
            weather_list[iweather]->julisothermrestriktionmin = 0.0;
            weather_list[iweather]->nddrestriktion = 0.0;
            weather_list[iweather]->nddrestriktionmin = 0.0;

			// Larix gmelinii
            if (weather_list[iweather]->temp1monthmeaniso < (parameter[0].janthresholdtempgmel)) {
                weather_list[iweather]->janisothermrestriktiong = 1.0;
            } else {
                weather_list[iweather]->janisothermrestriktiong =
                    1.0 - fabs(9.0 * (weather_list[iweather]->temp1monthmeaniso - parameter[0].janthresholdtempgmel) / (-parameter[0].janthresholdtempgmel));
            }
            if (weather_list[iweather]->temp1monthmeanisomin < (parameter[0].janthresholdtempgmel)) {
                weather_list[iweather]->janisothermrestriktiongmin = 1.0;
            } else {
                weather_list[iweather]->janisothermrestriktiongmin =
                    1.0 - fabs(9.0 * (weather_list[iweather]->temp1monthmeanisomin - parameter[0].janthresholdtempgmel) / (-parameter[0].janthresholdtempgmel));
            }


			// Larix sibirica
            if (weather_list[iweather]->temp1monthmeaniso < (-33.0)) {
                weather_list[iweather]->janisothermrestriktions = 1.0;
            } else {
                weather_list[iweather]->janisothermrestriktions = 1.0 - fabs(6.6 * (weather_list[iweather]->temp1monthmeaniso + 33.0) / 33.0);
            }
            if (weather_list[iweather]->temp1monthmeanisomin < (-33.0)) {
                weather_list[iweather]->janisothermrestriktionsmin = 1.0;
            } else {
                weather_list[iweather]->janisothermrestriktionsmin = 1.0 - fabs(6.6 * (weather_list[iweather]->temp1monthmeanisomin + 33.0) / 33.0);
            }


			// July temp for both
            if (weather_list[iweather]->temp7monthmeaniso < 10.0) {
                weather_list[iweather]->julisothermrestriktion = 1.0;
            } else {
                weather_list[iweather]->julisothermrestriktion = 1.0 - fabs(2.0 * (weather_list[iweather]->temp7monthmeaniso - 10.0) / 10.0);
            }
            if (weather_list[iweather]->temp7monthmeanisomin < 10.0) {
                weather_list[iweather]->julisothermrestriktionmin = 1.0;
            } else {
                weather_list[iweather]->julisothermrestriktionmin = 1.0 - fabs(2.0 * (weather_list[iweather]->temp7monthmeanisomin - 10.0) / 10.0);
            }

			// ndd for both
            if (weather_list[iweather]->vegetationperiodlengthiso < 60.0) {
                weather_list[iweather]->nddrestriktion = 1.0;
            } else {
                weather_list[iweather]->nddrestriktion = 1.0 - fabs((((double)weather_list[iweather]->vegetationperiodlengthiso) - 60.0) / 60.0);
            }
            if (weather_list[iweather]->vegetationperiodlengthisomin < 60.0) {
                weather_list[iweather]->nddrestriktionmin = 1.0;
            } else {
                weather_list[iweather]->nddrestriktionmin = 1.0 - fabs((((double)weather_list[iweather]->vegetationperiodlengthisomin) - 60.0) / 60.0);
            }
        
		
		
		
		
		// output to check weather
		FILE* fdir;
		char filenamechar[50];
		sprintf(filenamechar, "data_weatherprocessingcheck");
		string output = "output/" + string(filenamechar) + ".csv";
		fdir = fopen(output.c_str(), "a+");
		
		fprintf(fdir, 
			"%10.20f \t %10.20f \t %10.20f \t %10.20f \t %10.20f \t %10.20f \t %10.20f \t %10.20f \t %10.20f \t %10.20f \t %10.20f \t %10.20f \t %10.20f \t %10.20f \t %10.20f \t %10.20f \t %10.20f \t %10.20f \t \n", 
			weather_list[iweather]->temp1monthmeaniso,
			weather_list[iweather]->temp1monthmeanisomin,
			weather_list[iweather]->temp7monthmeaniso,
			weather_list[iweather]->temp7monthmeanisomin,
			weather_list[iweather]->droughtmort,
			weather_list[iweather]->droughtmortmin,
			(double) weather_list[iweather]->vegetationperiodlengthiso,
			(double) weather_list[iweather]->vegetationperiodlengthisomin,
			precgs,
			precgsmin,
			weather_list[iweather]->janisothermrestriktiong,
			weather_list[iweather]->janisothermrestriktiongmin,
			weather_list[iweather]->janisothermrestriktions,
			weather_list[iweather]->janisothermrestriktionsmin,
			weather_list[iweather]->julisothermrestriktion,
			weather_list[iweather]->julisothermrestriktionmin,
			weather_list[iweather]->nddrestriktion,
			weather_list[iweather]->nddrestriktionmin
		);
		
		fclose(fdir);
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		}
    }
}

extern void Weatherinput(struct Parameter* parameter, int stringlengthmax, vector<vector<Weather*>>& world_weather_list) {
    char dateinametemp[100];
    char dateinameprec[100];

	parameter[0].tempjandiffort = 0.0;
	parameter[0].tempjuldiffort = 0.0;
	parameter[0].tempdiffort = 0.0;
	parameter[0].precdiffort = 0.0;
	parameter[0].tempjandiffortmin = 0.0;
	parameter[0].tempjuldiffortmin = 0.0;
	parameter[0].tempdiffortmin = 0.0;
	parameter[0].precdiffortmin = 0.0;
	
    if ((parameter[0].windsource != 0) && (parameter[0].windsource != 4) && (parameter[0].windsource != 5)) {
        int findyr1 = 0, findyr2 = -100, jahr = 0, cntr = 0;

        string filename;
        std::ostringstream ss;
        string item;

        for (int t = 0; t < parameter[0].simduration; t++) {
            cntr = 0;

            jahr = parameter[0].startjahr + t;

            if (parameter[0].windsource == 1) {
                findyr1 = 1979;
                findyr2 = 2012;
            }

            ss.str("");
            ss.clear();

            if ((jahr < findyr2 + 1) && (jahr > findyr1 - 1)) {
                ss << jahr;

                if (parameter[0].windsource == 1) {
                    filename = "input/winddata/winddata" + ss.str() + "_EraInterim.dat";
                }

                ifstream fileinp(filename.c_str());

                while (fileinp >> item) {
                    cntr++;
                    if (cntr % 2) {
                        if (stof(item) >= 0 && stof(item) <= 360) {
                            wdir.push_back(stof(item));
                        } else {
                            wdir.push_back(0);
                        }
                    } else {
                        wspd.push_back(stof(item));
                    }
                }

                if (cntr > 0) {
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

        jahr = 0;
    }

    int aktort = 0;
    for (vector<vector<Weather*>>::iterator posw = world_weather_list.begin(); posw != world_weather_list.end(); posw++) {
        vector<Weather*>& weather_list = *posw;
        aktort++;

        // depending on the weather choice different files will be opened and read line by line
        if (parameter[0].weatherchoice == 21) {
            char tempbuf[] = "input/TY02_tmpweighted_model.csv";
            char precbuf[] = "input/TY02_prcweighted_model.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
        } else if (parameter[0].weatherchoice == 22) {
            char tempbuf[] = "input/CH17_tmpweighted_model.csv";
            char precbuf[] = "input/CH17_prcweighted_model.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
        } else if (parameter[0].weatherchoice == 23) {
            char tempbuf[] = "input/CH02_tmpweighted_model.csv";
            char precbuf[] = "input/CH02_prcweighted_model.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
        } else if (parameter[0].weatherchoice == 24) {
            char tempbuf[] = "input/CH06_tmpweighted_model.csv";
            char precbuf[] = "input/CH06_prcweighted_model.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
        } else if (parameter[0].weatherchoice == 7001) {
            char tempbuf[] = "input/Coredata_complete_tmp_forw_ens01.csv";
            char precbuf[] = "input/Coredata_complete_prc.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
        } else if (parameter[0].weatherchoice == 202001) {//1901:2100 so 200 years long
            char tempbuf[] = "input/trans_Polar Urals_temp_1901-2100_rcp85.csv";
            char precbuf[] = "input/trans_Polar Urals_prec_1901-2100_rcp85.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
			parameter[0].tempdiffortmin = (0.08233378/1000) * treerows;
			parameter[0].precdiffortmin = (0.31567919/1000) * treerows;
        } else if (parameter[0].weatherchoice == 202002) {
            char tempbuf[] = "input/trans_Taimyr Peninsula_temp_1901-2100_rcp85.csv";
            char precbuf[] = "input/trans_Taimyr Peninsula_prec_1901-2100_rcp85.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
			parameter[0].tempdiffortmin = (0.05940048/1000) * treerows;
			parameter[0].precdiffortmin = (0.11536748/1000) * treerows;
        } else if (parameter[0].weatherchoice == 202003) {
            char tempbuf[] = "input/trans_Buor Khaya_temp_1901-2100_rcp85.csv";
            char precbuf[] = "input/trans_Buor Khaya_prec_1901-2100_rcp85.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
			parameter[0].tempdiffortmin = (-0.02204296/1000) * treerows;
			parameter[0].precdiffortmin = (0.00842138/1000) * treerows;
        } else if (parameter[0].weatherchoice == 202004) {
            char tempbuf[] = "input/trans_Kolyma_temp_1901-2100_rcp85.csv";
            char precbuf[] = "input/trans_Kolyma_prec_1901-2100_rcp85.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
			parameter[0].tempdiffortmin = (0.04269392/1000) * treerows;
			parameter[0].precdiffortmin = (0.21769910/1000) * treerows;
        } else if (parameter[0].weatherchoice == 202005) {
            char tempbuf[] = "input/trans_Chukotka_temp_1901-2100_rcp85.csv";
            char precbuf[] = "input/trans_Chukotka_prec_1901-2100_rcp85.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
			parameter[0].tempdiffortmin = (0.15173684/1000) * treerows;
			parameter[0].precdiffortmin = (-0.27962335/1000) * treerows;
        } else if (parameter[0].weatherchoice == 210001) {//1901:2100 so 200 years long
            char tempbuf[] = "input/trans_Polar Urals_temp_1901-2100_rcp85.csv";
            char precbuf[] = "input/trans_Polar Urals_prec_1901-2100_rcp85.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
			// baseline weather manipulation
		    parameter[0].tempdiffort = (0.08233378/1000) * -85000;// in m: negative values for northward/colder areas
			parameter[0].precdiffort = (0.31567919/1000) * -85000;
			// transect weather variation
			parameter[0].tempdiffortmin = (0.08233378/1000) * treerows;
			parameter[0].precdiffortmin = (0.31567919/1000) * treerows;
        } else if (parameter[0].weatherchoice == 210002) {
            char tempbuf[] = "input/trans_Taimyr Peninsula_temp_1901-2100_rcp85.csv";
            char precbuf[] = "input/trans_Taimyr Peninsula_prec_1901-2100_rcp85.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
			// baseline weather manipulation
		    parameter[0].tempdiffort = (0.05940048/1000) * -75000;// in m: negative values for northward/colder areas
			parameter[0].precdiffort = (0.11536748/1000) * -75000;
			// transect weather variation
			parameter[0].tempdiffortmin = (0.05940048/1000) * treerows;
			parameter[0].precdiffortmin = (0.11536748/1000) * treerows;
        } else if (parameter[0].weatherchoice == 210003) {
            char tempbuf[] = "input/trans_Buor Khaya_temp_1901-2100_rcp85.csv";
            char precbuf[] = "input/trans_Buor Khaya_prec_1901-2100_rcp85.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
			// baseline weather manipulation
		    parameter[0].tempdiffort = (-0.02204296/1000) * 100000;// in m: negative values for northward/colder areas
			parameter[0].precdiffort = (0.00842138/1000) * 100000;
			// transect weather variation
			parameter[0].tempdiffortmin = (-0.02204296/1000) * treerows;
			parameter[0].precdiffortmin = (0.00842138/1000) * treerows;
        } else if (parameter[0].weatherchoice == 210004) {
            char tempbuf[] = "input/trans_Kolyma_temp_1901-2100_rcp85.csv";
            char precbuf[] = "input/trans_Kolyma_prec_1901-2100_rcp85.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
			// baseline weather manipulation
		    parameter[0].tempdiffort = (0.04269392/1000) * -55000;// in m: negative values for northward/colder areas
			parameter[0].precdiffort = (0.21769910/1000) * -55000;
			// transect weather variation
			parameter[0].tempdiffortmin = (0.04269392/1000) * treerows;
			parameter[0].precdiffortmin = (0.21769910/1000) * treerows;
        } else if (parameter[0].weatherchoice == 210005) {
            char tempbuf[] = "input/trans_Chukotka_temp_1901-2100_rcp85.csv";
            char precbuf[] = "input/trans_Chukotka_prec_1901-2100_rcp85.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
			// baseline weather manipulation
		    parameter[0].tempdiffort = (0.15173684/1000) * -55000;// in m: negative values for northward/colder areas
			parameter[0].precdiffort = (-0.27962335/1000) * -55000;
			// transect weather variation
			parameter[0].tempdiffortmin = (0.15173684/1000) * treerows;
			parameter[0].precdiffortmin = (-0.27962335/1000) * treerows;
        } else if (parameter[0].weatherchoice == 2100011) {//-1999:2100 so 4100 years long
            char tempbuf[] = "input/trans_Polar Urals_temp_minus1999-2100_rcp85.csv";
            char precbuf[] = "input/trans_Polar Urals_prec_minus1999-2100_rcp85.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
			// baseline weather manipulation
		    parameter[0].tempdiffort = (0.08233378/1000) * -85000;// in m: negative values for northward/colder areas
			parameter[0].precdiffort = (0.31567919/1000) * -85000;
			// transect weather variation
			parameter[0].tempdiffortmin = (0.08233378/1000) * treerows;
			parameter[0].precdiffortmin = (0.31567919/1000) * treerows;
        } else if (parameter[0].weatherchoice == 2100021) {
            char tempbuf[] = "input/trans_Taimyr Peninsula_temp_minus1999-2100_rcp85.csv";
            char precbuf[] = "input/trans_Taimyr Peninsula_prec_minus1999-2100_rcp85.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
			// baseline weather manipulation
		    parameter[0].tempdiffort = (0.05940048/1000) * -75000;// in m: negative values for northward/colder areas
			parameter[0].precdiffort = (0.11536748/1000) * -75000;
			// transect weather variation
			parameter[0].tempdiffortmin = (0.05940048/1000) * treerows;
			parameter[0].precdiffortmin = (0.11536748/1000) * treerows;
        } else if (parameter[0].weatherchoice == 2100031) {
            char tempbuf[] = "input/trans_Buor Khaya_temp_minus1999-2100_rcp85.csv";
            char precbuf[] = "input/trans_Buor Khaya_prec_minus1999-2100_rcp85.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
			// baseline weather manipulation
		    parameter[0].tempdiffort = (-0.02204296/1000) * 100000;// in m: negative values for northward/colder areas
			parameter[0].precdiffort = (0.00842138/1000) * 100000;
			// transect weather variation
			parameter[0].tempdiffortmin = (-0.02204296/1000) * treerows;
			parameter[0].precdiffortmin = (0.00842138/1000) * treerows;
        } else if (parameter[0].weatherchoice == 2100041) {
            char tempbuf[] = "input/trans_Kolyma_temp_minus1999-2100_rcp85.csv";
            char precbuf[] = "input/trans_Kolyma_prec_minus1999-2100_rcp85.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
			// baseline weather manipulation
		    parameter[0].tempdiffort = (0.04269392/1000) * -55000;// in m: negative values for northward/colder areas
			parameter[0].precdiffort = (0.21769910/1000) * -55000;
			// transect weather variation
			parameter[0].tempdiffortmin = (0.04269392/1000) * treerows;
			parameter[0].precdiffortmin = (0.21769910/1000) * treerows;
        } else if (parameter[0].weatherchoice == 2100051) {
            char tempbuf[] = "input/trans_Chukotka_temp_minus1999-2100_rcp85.csv";
            char precbuf[] = "input/trans_Chukotka_prec_minus1999-2100_rcp85.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
			// baseline weather manipulation
		    parameter[0].tempdiffort = (0.15173684/1000) * -55000;// in m: negative values for northward/colder areas
			parameter[0].precdiffort = (-0.27962335/1000) * -55000;
			// transect weather variation
			parameter[0].tempdiffortmin = (0.15173684/1000) * treerows;
			parameter[0].precdiffortmin = (-0.27962335/1000) * treerows;
         } else if (parameter[0].weatherchoice == 2100012) {//-499:2100 so 2600 years long
            char tempbuf[] = "input/trans_Polar Urals_temp_minus499-2100_rcp85.csv";
            char precbuf[] = "input/trans_Polar Urals_prec_minus499-2100_rcp85.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
			// baseline weather manipulation
		    parameter[0].tempdiffort = (0.08233378/1000) * -85000;// in m: negative values for northward/colder areas
			parameter[0].precdiffort = (0.31567919/1000) * -85000;
			// transect weather variation
			parameter[0].tempdiffortmin = (0.08233378/1000) * treerows;
			parameter[0].precdiffortmin = (0.31567919/1000) * treerows;
        } else if (parameter[0].weatherchoice == 2100022) {
            char tempbuf[] = "input/trans_Taimyr Peninsula_temp_minus499-2100_rcp85.csv";
            char precbuf[] = "input/trans_Taimyr Peninsula_prec_minus499-2100_rcp85.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
			// baseline weather manipulation
		    parameter[0].tempdiffort = (0.05940048/1000) * -75000;// in m: negative values for northward/colder areas
			parameter[0].precdiffort = (0.11536748/1000) * -75000;
			// transect weather variation
			parameter[0].tempdiffortmin = (0.05940048/1000) * treerows;
			parameter[0].precdiffortmin = (0.11536748/1000) * treerows;
        } else if (parameter[0].weatherchoice == 2100032) {
            char tempbuf[] = "input/trans_Buor Khaya_temp_minus499-2100_rcp85.csv";
            char precbuf[] = "input/trans_Buor Khaya_prec_minus499-2100_rcp85.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
			// baseline weather manipulation
		    parameter[0].tempdiffort = (-0.02204296/1000) * 100000;// in m: negative values for northward/colder areas
			parameter[0].precdiffort = (0.00842138/1000) * 100000;
			// transect weather variation
			parameter[0].tempdiffortmin = (-0.02204296/1000) * treerows;
			parameter[0].precdiffortmin = (0.00842138/1000) * treerows;
        } else if (parameter[0].weatherchoice == 2100042) {
            char tempbuf[] = "input/trans_Kolyma_temp_minus499-2100_rcp85.csv";
            char precbuf[] = "input/trans_Kolyma_prec_minus499-2100_rcp85.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
			// baseline weather manipulation
		    parameter[0].tempdiffort = (0.04269392/1000) * -55000;// in m: negative values for northward/colder areas
			parameter[0].precdiffort = (0.21769910/1000) * -55000;
			// transect weather variation
			parameter[0].tempdiffortmin = (0.04269392/1000) * treerows;
			parameter[0].precdiffortmin = (0.21769910/1000) * treerows;
        } else if (parameter[0].weatherchoice == 2100052) {
            char tempbuf[] = "input/trans_Chukotka_temp_minus499-2100_rcp85.csv";
            char precbuf[] = "input/trans_Chukotka_prec_minus499-2100_rcp85.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
			// baseline weather manipulation
		    // parameter[0].tempdiffort = (0.15173684/1000) * -55000;// in m: negative values for northward/colder areas
			// parameter[0].precdiffort = (-0.27962335/1000) * -55000;
		    parameter[0].tempdiffort = (0.15173684/1000) * -20000;// in m: negative values for northward/colder areas
			parameter[0].precdiffort = (-0.27962335/1000) * -20000;
			// transect weather variation
			parameter[0].tempdiffortmin = (0.15173684/1000) * treerows;
			parameter[0].precdiffortmin = (-0.27962335/1000) * treerows;
        } else if (parameter[0].weatherchoice == 2100013) {//-499:2100 so 2600 years long ; no base min clim
            char tempbuf[] = "input/trans_Polar Urals_temp_minus499-2100_rcp85.csv";
            char precbuf[] = "input/trans_Polar Urals_prec_minus499-2100_rcp85.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
			// baseline weather manipulation
		    parameter[0].tempdiffort = (0.08233378/1000) * 0;// in m: negative values for northward/colder areas
			parameter[0].precdiffort = (0.31567919/1000) * 0;
			// transect weather variation
			parameter[0].tempdiffortmin = (0.08233378/1000) * treerows;
			parameter[0].precdiffortmin = (0.31567919/1000) * treerows;
        } else if (parameter[0].weatherchoice == 2100023) {
            char tempbuf[] = "input/trans_Taimyr Peninsula_temp_minus499-2100_rcp85.csv";
            char precbuf[] = "input/trans_Taimyr Peninsula_prec_minus499-2100_rcp85.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
			// baseline weather manipulation
		    parameter[0].tempdiffort = (0.05940048/1000) * 0;// in m: negative values for northward/colder areas
			parameter[0].precdiffort = (0.11536748/1000) * 0;
			// transect weather variation
			parameter[0].tempdiffortmin = (0.05940048/1000) * treerows;
			parameter[0].precdiffortmin = (0.11536748/1000) * treerows;
        } else if (parameter[0].weatherchoice == 2100033) {
            char tempbuf[] = "input/trans_Buor Khaya_temp_minus499-2100_rcp85.csv";
            char precbuf[] = "input/trans_Buor Khaya_prec_minus499-2100_rcp85.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
			// baseline weather manipulation
		    parameter[0].tempdiffort = (-0.02204296/1000) * 0;// in m: negative values for northward/colder areas
			parameter[0].precdiffort = (0.00842138/1000) * 0;
			// transect weather variation
			parameter[0].tempdiffortmin = (-0.02204296/1000) * treerows;
			parameter[0].precdiffortmin = (0.00842138/1000) * treerows;
        } else if (parameter[0].weatherchoice == 2100043) {
            char tempbuf[] = "input/trans_Kolyma_temp_minus499-2100_rcp85.csv";
            char precbuf[] = "input/trans_Kolyma_prec_minus499-2100_rcp85.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
			// baseline weather manipulation
		    parameter[0].tempdiffort = (0.04269392/1000) * 0;// in m: negative values for northward/colder areas
			parameter[0].precdiffort = (0.21769910/1000) * 0;
			// transect weather variation
			parameter[0].tempdiffortmin = (0.04269392/1000) * treerows;
			parameter[0].precdiffortmin = (0.21769910/1000) * treerows;
        } else if (parameter[0].weatherchoice == 2100053) {
            char tempbuf[] = "input/trans_Chukotka_temp_minus499-2100_rcp85.csv";
            char precbuf[] = "input/trans_Chukotka_prec_minus499-2100_rcp85.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
			// baseline weather manipulation
		    parameter[0].tempdiffort = (0.15173684/1000) * 0;// in m: negative values for northward/colder areas
			parameter[0].precdiffort = (-0.27962335/1000) * 0;
			// transect weather variation
			parameter[0].tempdiffortmin = (0.15173684/1000) * treerows;
			parameter[0].precdiffortmin = (-0.27962335/1000) * treerows;
         } else if (parameter[0].weatherchoice == 2100014) {//-499:2100 so 2600 years long; weaker gradients
            char tempbuf[] = "input/trans_Polar Urals_temp_minus499-2100_rcp85.csv";
            char precbuf[] = "input/trans_Polar Urals_prec_minus499-2100_rcp85.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
			// baseline weather manipulation
		    parameter[0].tempdiffort = (0.08233378/1000) * -85000;// in m: negative values for northward/colder areas
			parameter[0].precdiffort = (0.31567919/1000) * -85000;
			// transect weather variation
			parameter[0].tempdiffortmin = (0.08233378/1000) * treerows * 0.9;
			parameter[0].precdiffortmin = (0.31567919/1000) * treerows * 0.9;
        } else if (parameter[0].weatherchoice == 2100024) {
            char tempbuf[] = "input/trans_Taimyr Peninsula_temp_minus499-2100_rcp85.csv";
            char precbuf[] = "input/trans_Taimyr Peninsula_prec_minus499-2100_rcp85.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
			// baseline weather manipulation
		    parameter[0].tempdiffort = (0.05940048/1000) * -75000;// in m: negative values for northward/colder areas
			parameter[0].precdiffort = (0.11536748/1000) * -75000;
			// transect weather variation
			parameter[0].tempdiffortmin = (0.05940048/1000) * treerows * 0.9;
			parameter[0].precdiffortmin = (0.11536748/1000) * treerows * 0.9;
        } else if (parameter[0].weatherchoice == 2100034) {
            char tempbuf[] = "input/trans_Buor Khaya_temp_minus499-2100_rcp85.csv";
            char precbuf[] = "input/trans_Buor Khaya_prec_minus499-2100_rcp85.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
			// baseline weather manipulation
		    parameter[0].tempdiffort = (-0.02204296/1000) * 100000;// in m: negative values for northward/colder areas
			parameter[0].precdiffort = (0.00842138/1000) * 100000;
			// transect weather variation
			parameter[0].tempdiffortmin = (-0.02204296/1000) * treerows * 0.9;
			parameter[0].precdiffortmin = (0.00842138/1000) * treerows * 0.9;
        } else if (parameter[0].weatherchoice == 2100044) {
            char tempbuf[] = "input/trans_Kolyma_temp_minus499-2100_rcp85.csv";
            char precbuf[] = "input/trans_Kolyma_prec_minus499-2100_rcp85.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
			// baseline weather manipulation
		    parameter[0].tempdiffort = (0.04269392/1000) * -55000;// in m: negative values for northward/colder areas
			parameter[0].precdiffort = (0.21769910/1000) * -55000;
			// transect weather variation
			parameter[0].tempdiffortmin = (0.04269392/1000) * treerows * 0.9;
			parameter[0].precdiffortmin = (0.21769910/1000) * treerows * 0.9;
        } else if (parameter[0].weatherchoice == 2100054) {
            char tempbuf[] = "input/trans_Chukotka_temp_minus499-2100_rcp85.csv";
            char precbuf[] = "input/trans_Chukotka_prec_minus499-2100_rcp85.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
			// baseline weather manipulation
		    // parameter[0].tempdiffort = (0.15173684/1000) * -55000;// in m: negative values for northward/colder areas
			// parameter[0].precdiffort = (-0.27962335/1000) * -55000;
		    parameter[0].tempdiffort = (0.15173684/1000) * -20000;// in m: negative values for northward/colder areas
			parameter[0].precdiffort = (-0.27962335/1000) * -20000;
			// transect weather variation
			parameter[0].tempdiffortmin = (0.15173684/1000) * treerows * 0.9;
			parameter[0].precdiffortmin = (-0.27962335/1000) * treerows * 0.9;
			
        } else if (parameter[0].weatherchoice == 2100015) {//-499:2100 so 2600 years long; weaker gradients and no climate adjustment
            char tempbuf[] = "input/trans_Polar Urals_temp_minus499-2100_rcp85.csv";
            char precbuf[] = "input/trans_Polar Urals_prec_minus499-2100_rcp85.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
			// baseline weather manipulation
		    parameter[0].tempdiffort = (0.08233378/1000) * 0;// in m: negative values for northward/colder areas
			parameter[0].precdiffort = (0.31567919/1000) * 0;
			// transect weather variation
			parameter[0].tempdiffortmin = (0.08233378/1000) * treerows * 0.9;
			parameter[0].precdiffortmin = (0.31567919/1000) * treerows * 0.9;
        } else if (parameter[0].weatherchoice == 2100025) {
            char tempbuf[] = "input/trans_Taimyr Peninsula_temp_minus499-2100_rcp85.csv";
            char precbuf[] = "input/trans_Taimyr Peninsula_prec_minus499-2100_rcp85.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
			// baseline weather manipulation
		    parameter[0].tempdiffort = (0.05940048/1000) * 0;// in m: negative values for northward/colder areas
			parameter[0].precdiffort = (0.11536748/1000) * 0;
			// transect weather variation
			parameter[0].tempdiffortmin = (0.05940048/1000) * treerows * 0.9;
			parameter[0].precdiffortmin = (0.11536748/1000) * treerows * 0.9;
        } else if (parameter[0].weatherchoice == 2100035) {
            char tempbuf[] = "input/trans_Buor Khaya_temp_minus499-2100_rcp85.csv";
            char precbuf[] = "input/trans_Buor Khaya_prec_minus499-2100_rcp85.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
			// baseline weather manipulation
		    parameter[0].tempdiffort = (-0.02204296/1000) * 0;// in m: negative values for northward/colder areas
			parameter[0].precdiffort = (0.00842138/1000) * 0;
			// transect weather variation
			parameter[0].tempdiffortmin = (-0.02204296/1000) * treerows * 0.9;
			parameter[0].precdiffortmin = (0.00842138/1000) * treerows * 0.9;
        } else if (parameter[0].weatherchoice == 2100045) {
            char tempbuf[] = "input/trans_Kolyma_temp_minus499-2100_rcp85.csv";
            char precbuf[] = "input/trans_Kolyma_prec_minus499-2100_rcp85.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
			// baseline weather manipulation
		    parameter[0].tempdiffort = (0.04269392/1000) * 0;// in m: negative values for northward/colder areas
			parameter[0].precdiffort = (0.21769910/1000) * 0;
			// transect weather variation
			parameter[0].tempdiffortmin = (0.04269392/1000) * treerows * 0.9;
			parameter[0].precdiffortmin = (0.21769910/1000) * treerows * 0.9;
        } else if (parameter[0].weatherchoice == 2100055) {
            char tempbuf[] = "input/trans_Chukotka_temp_minus499-2100_rcp85.csv";
            char precbuf[] = "input/trans_Chukotka_prec_minus499-2100_rcp85.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
			// baseline weather manipulation
		    parameter[0].tempdiffort = (0.15173684/1000) * 0;// in m: negative values for northward/colder areas
			parameter[0].precdiffort = (-0.27962335/1000) * 0;
			// transect weather variation
			parameter[0].tempdiffortmin = (0.15173684/1000) * treerows * 0.9;
			parameter[0].precdiffortmin = (-0.27962335/1000) * treerows * 0.9;

         } else if (parameter[0].weatherchoice == 2100016) {//-499:2100 so 2600 years long shift northwards
            char tempbuf[] = "input/trans_Polar Urals_temp_minus499-2100_rcp85.csv";
            char precbuf[] = "input/trans_Polar Urals_prec_minus499-2100_rcp85.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
			// baseline weather manipulation
		    parameter[0].tempdiffort = (0.08233378/1000) * -85000;// in m: negative values for northward/colder areas
			parameter[0].precdiffort = (0.31567919/1000) * -85000;
			// transect weather variation
			parameter[0].tempdiffortmin = (0.08233378/1000) * treerows;
			parameter[0].precdiffortmin = (0.31567919/1000) * treerows;
        } else if (parameter[0].weatherchoice == 2100026) {
            char tempbuf[] = "input/trans_Taimyr Peninsula_temp_minus499-2100_rcp85.csv";
            char precbuf[] = "input/trans_Taimyr Peninsula_prec_minus499-2100_rcp85.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
			// baseline weather manipulation
		    parameter[0].tempdiffort = (0.05940048/1000) * -85000;// in m: negative values for northward/colder areas
			parameter[0].precdiffort = (0.11536748/1000) * -85000;
			// transect weather variation
			parameter[0].tempdiffortmin = (0.05940048/1000) * treerows;
			parameter[0].precdiffortmin = (0.11536748/1000) * treerows;
        } else if (parameter[0].weatherchoice == 2100036) {
            char tempbuf[] = "input/trans_Buor Khaya_temp_minus499-2100_rcp85.csv";
            char precbuf[] = "input/trans_Buor Khaya_prec_minus499-2100_rcp85.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
			// baseline weather manipulation
		    parameter[0].tempdiffort = (-0.02204296/1000) * 100000;// in m: negative values for northward/colder areas
			parameter[0].precdiffort = (0.00842138/1000) * 100000;
			// transect weather variation
			parameter[0].tempdiffortmin = (-0.02204296/1000) * treerows;
			parameter[0].precdiffortmin = (0.00842138/1000) * treerows;
        } else if (parameter[0].weatherchoice == 2100046) {
            char tempbuf[] = "input/trans_Kolyma_temp_minus499-2100_rcp85.csv";
            char precbuf[] = "input/trans_Kolyma_prec_minus499-2100_rcp85.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
			// baseline weather manipulation
		    parameter[0].tempdiffort = (0.04269392/1000) * -55000;// in m: negative values for northward/colder areas
			parameter[0].precdiffort = (0.21769910/1000) * -55000;
			// transect weather variation
			parameter[0].tempdiffortmin = (0.04269392/1000) * treerows;
			parameter[0].precdiffortmin = (0.21769910/1000) * treerows;
        } else if (parameter[0].weatherchoice == 2100056) {
            char tempbuf[] = "input/trans_Chukotka_temp_minus499-2100_rcp85.csv";
            char precbuf[] = "input/trans_Chukotka_prec_minus499-2100_rcp85.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
			// baseline weather manipulation
		    // parameter[0].tempdiffort = (0.15173684/1000) * -55000;// in m: negative values for northward/colder areas
			// parameter[0].precdiffort = (-0.27962335/1000) * -55000;
		    parameter[0].tempdiffort = (0.15173684/1000) * -55000;// in m: negative values for northward/colder areas
			parameter[0].precdiffort = (-0.27962335/1000) * -55000;
			// transect weather variation
			parameter[0].tempdiffortmin = (0.15173684/1000) * treerows;
			parameter[0].precdiffortmin = (-0.27962335/1000) * treerows;

         } else if (parameter[0].weatherchoice == 2100111) {//-499:2100 so 2600 years long - january july temp for right representation of transects
            char tempbuf[] = "input/trans_Polar Urals_temp_minus499-2100_rcp85.csv";
            char precbuf[] = "input/trans_Polar Urals_prec_minus499-2100_rcp85.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
			// baseline weather manipulation
		    parameter[0].tempjandiffort = (0.049983234/1000) * 0;// in m: negative values for northward/colder areas
		    parameter[0].tempjuldiffort = (0.08816501/1000) * 0;
		    parameter[0].tempdiffort = (0.08233378/1000) * 0;
			parameter[0].precdiffort = (0.31567919/1000) * 0;
			// transect weather variation
			parameter[0].tempjandiffortmin = (0.049983234/1000) * treerows;
			parameter[0].tempjuldiffortmin = (0.08816501/1000) * treerows;
			parameter[0].tempdiffortmin = (0.08233378/1000) * treerows;
			parameter[0].precdiffortmin = (0.31567919/1000) * treerows;
        } else if (parameter[0].weatherchoice == 2100121) {
            char tempbuf[] = "input/trans_Taimyr Peninsula_temp_minus499-2100_rcp85.csv";
            char precbuf[] = "input/trans_Taimyr Peninsula_prec_minus499-2100_rcp85.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
			// baseline weather manipulation
		    parameter[0].tempjandiffort = (0.001262349/1000) * 0;// in m: negative values for northward/colder areas
		    parameter[0].tempjuldiffort = (0.05785078/1000) * 0;
		    parameter[0].tempdiffort = (0.05940048/1000) * 0;
			parameter[0].precdiffort = (0.11536748/1000) * 0;
			// transect weather variation
			parameter[0].tempjandiffortmin = (0.001262349/1000) * treerows;
			parameter[0].tempjuldiffortmin = (0.05785078/1000) * treerows;
			parameter[0].tempdiffortmin = (0.05940048/1000) * treerows;
			parameter[0].precdiffortmin = (0.11536748/1000) * treerows;
        } else if (parameter[0].weatherchoice == 2100131) {
            char tempbuf[] = "input/trans_Buor Khaya_temp_minus499-2100_rcp85.csv";
            char precbuf[] = "input/trans_Buor Khaya_prec_minus499-2100_rcp85.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
			// baseline weather manipulation
		    parameter[0].tempjandiffort = (-0.133395946/1000) * 0;// in m: negative values for northward/colder areas
		    parameter[0].tempjuldiffort = (0.05886988/1000) * 0;
		    parameter[0].tempdiffort = (-0.02204296/1000) * 0;
			parameter[0].precdiffort = (0.00842138/1000) * 0;
			// transect weather variation
			parameter[0].tempjandiffortmin = (-0.133395946/1000) * treerows;
			parameter[0].tempjuldiffortmin = (0.05886988/1000) * treerows;
			parameter[0].tempdiffortmin = (-0.02204296/1000) * treerows;
			parameter[0].precdiffortmin = (0.00842138/1000) * treerows;
         } else if (parameter[0].weatherchoice == 2100141) {
            char tempbuf[] = "input/trans_Kolyma_temp_minus499-2100_rcp85.csv";
            char precbuf[] = "input/trans_Kolyma_prec_minus499-2100_rcp85.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
			// baseline weather manipulation
		    parameter[0].tempjandiffort = (0.001623102/1000) * 0;// in m: negative values for northward/colder areas
		    parameter[0].tempjuldiffort = (0.12172445/1000) * 0;
		    parameter[0].tempdiffort = (0.04269392/1000) * 0;
			parameter[0].precdiffort = (0.21769910/1000) * 0;
			// transect weather variation
			parameter[0].tempjandiffortmin = (0.001623102/1000) * treerows;
			parameter[0].tempjuldiffortmin = (0.12172445/1000) * treerows;
			parameter[0].tempdiffortmin = (0.04269392/1000) * treerows;
			parameter[0].precdiffortmin = (0.21769910/1000) * treerows;
        } else if (parameter[0].weatherchoice == 2100151) {
            char tempbuf[] = "input/trans_Chukotka_temp_minus499-2100_rcp85.csv";
            char precbuf[] = "input/trans_Chukotka_prec_minus499-2100_rcp85.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
			// baseline weather manipulation
		    parameter[0].tempjandiffort = (0.128972760/1000) * 0;// in m: negative values for northward/colder areas
		    parameter[0].tempjuldiffort = (0.05026903/1000) * 0;
		    parameter[0].tempdiffort = (0.15173684/1000) * 0;
			parameter[0].precdiffort = (-0.27962335/1000) * 0;
			// transect weather variation
			parameter[0].tempjandiffortmin = (0.128972760/1000) * treerows;
			parameter[0].tempjuldiffortmin = (0.05026903/1000) * treerows;
			parameter[0].tempdiffortmin = (0.15173684/1000) * treerows;
			parameter[0].precdiffortmin = (-0.27962335/1000) * treerows;
			
         } else if (parameter[0].weatherchoice == 2100112) {//-499:2100 so 2600 years long - january july temp for right representation of transects
            char tempbuf[] = "input/trans_Polar Urals_temp_minus499-2100_rcp85.csv";
            char precbuf[] = "input/trans_Polar Urals_prec_minus499-2100_rcp85.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
			// baseline weather manipulation
		    parameter[0].tempjandiffort = (0.049983234/1000) * -50000;// in m: negative values for northward/colder areas
		    parameter[0].tempjuldiffort = (0.08816501/1000) * -50000;
		    parameter[0].tempdiffort = (0.08233378/1000) * -50000;
			parameter[0].precdiffort = (0.31567919/1000) * -50000;
			// transect weather variation
			parameter[0].tempjandiffortmin = (0.049983234/1000) * treerows;
			parameter[0].tempjuldiffortmin = (0.08816501/1000) * treerows;
			parameter[0].tempdiffortmin = (0.08233378/1000) * treerows;
			parameter[0].precdiffortmin = (0.31567919/1000) * treerows;
        } else if (parameter[0].weatherchoice == 2100122) {
            char tempbuf[] = "input/trans_Taimyr Peninsula_temp_minus499-2100_rcp85.csv";
            char precbuf[] = "input/trans_Taimyr Peninsula_prec_minus499-2100_rcp85.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
			// baseline weather manipulation
		    parameter[0].tempjandiffort = (0.001262349/1000) * -50000;// in m: negative values for northward/colder areas
		    parameter[0].tempjuldiffort = (0.05785078/1000) * -50000;
		    parameter[0].tempdiffort = (0.05940048/1000) * -50000;
			parameter[0].precdiffort = (0.11536748/1000) * -50000;
			// transect weather variation
			parameter[0].tempjandiffortmin = (0.001262349/1000) * treerows;
			parameter[0].tempjuldiffortmin = (0.05785078/1000) * treerows;
			parameter[0].tempdiffortmin = (0.05940048/1000) * treerows;
			parameter[0].precdiffortmin = (0.11536748/1000) * treerows;
        } else if (parameter[0].weatherchoice == 2100132) {
            char tempbuf[] = "input/trans_Buor Khaya_temp_minus499-2100_rcp85.csv";
            char precbuf[] = "input/trans_Buor Khaya_prec_minus499-2100_rcp85.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
			// baseline weather manipulation
		    parameter[0].tempjandiffort = (-0.133395946/1000) * -50000;// in m: negative values for northward/colder areas
		    parameter[0].tempjuldiffort = (0.05886988/1000) * -50000;
		    parameter[0].tempdiffort = (-0.02204296/1000) * -50000;
			parameter[0].precdiffort = (0.00842138/1000) * -50000;
			// transect weather variation
			parameter[0].tempjandiffortmin = (-0.133395946/1000) * treerows;
			parameter[0].tempjuldiffortmin = (0.05886988/1000) * treerows;
			parameter[0].tempdiffortmin = (-0.02204296/1000) * treerows;
			parameter[0].precdiffortmin = (0.00842138/1000) * treerows;
         } else if (parameter[0].weatherchoice == 2100142) {
            char tempbuf[] = "input/trans_Kolyma_temp_minus499-2100_rcp85.csv";
            char precbuf[] = "input/trans_Kolyma_prec_minus499-2100_rcp85.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
			// baseline weather manipulation
		    parameter[0].tempjandiffort = (0.001623102/1000) * -50000;// in m: negative values for northward/colder areas
		    parameter[0].tempjuldiffort = (0.12172445/1000) * -50000;
		    parameter[0].tempdiffort = (0.04269392/1000) * -50000;
			parameter[0].precdiffort = (0.21769910/1000) * -50000;
			// transect weather variation
			parameter[0].tempjandiffortmin = (0.001623102/1000) * treerows;
			parameter[0].tempjuldiffortmin = (0.12172445/1000) * treerows;
			parameter[0].tempdiffortmin = (0.04269392/1000) * treerows;
			parameter[0].precdiffortmin = (0.21769910/1000) * treerows;
        } else if (parameter[0].weatherchoice == 2100152) {
            char tempbuf[] = "input/trans_Chukotka_temp_minus499-2100_rcp85.csv";
            char precbuf[] = "input/trans_Chukotka_prec_minus499-2100_rcp85.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
			// baseline weather manipulation
		    parameter[0].tempjandiffort = (0.128972760/1000) * -50000;// in m: negative values for northward/colder areas
		    parameter[0].tempjuldiffort = (0.05026903/1000) * -50000;
		    parameter[0].tempdiffort = (0.15173684/1000) * -50000;
			parameter[0].precdiffort = (-0.27962335/1000) * -50000;
			// transect weather variation
			parameter[0].tempjandiffortmin = (0.128972760/1000) * treerows;
			parameter[0].tempjuldiffortmin = (0.05026903/1000) * treerows;
			parameter[0].tempdiffortmin = (0.15173684/1000) * treerows;
			parameter[0].precdiffortmin = (-0.27962335/1000) * treerows;
			
			
			
			
			
			
        } else if (parameter[0].weatherchoice == 2100212) {//-499:2100 so 2600 years long - january july temp for right representation of transects +++ 
		// gridcell to NORTH
            char tempbuf[] = "input/trans_Polar Urals_shift0N_temp_minus499-2100_rcp85.csv";
            char precbuf[] = "input/trans_Polar Urals_shift0N_prec_minus499-2100_rcp85.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
			// baseline weather manipulation
		    parameter[0].tempjandiffort = (0.049983234/1000) * 0;// in m: negative values for northward/colder areas
		    parameter[0].tempjuldiffort = (0.08816501/1000) * 0;
		    parameter[0].tempdiffort = (0.08233378/1000) * 0;
			parameter[0].precdiffort = (0.31567919/1000) * 0;
			// transect weather variation
			parameter[0].tempjandiffortmin = (0.049983234/1000) * treerows;
			parameter[0].tempjuldiffortmin = (0.08816501/1000) * treerows;
			parameter[0].tempdiffortmin = (0.08233378/1000) * treerows;
			parameter[0].precdiffortmin = (0.31567919/1000) * treerows;			
        } else if (parameter[0].weatherchoice == 2100222) {
            char tempbuf[] = "input/trans_Taimyr Peninsula_shift0N_temp_minus499-2100_rcp85.csv";
            char precbuf[] = "input/trans_Taimyr Peninsula_shift0N_prec_minus499-2100_rcp85.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
			// baseline weather manipulation
		    parameter[0].tempjandiffort = (0.001262349/1000) * 0;// in m: negative values for northward/colder areas
		    parameter[0].tempjuldiffort = (0.05785078/1000) * 0;
		    parameter[0].tempdiffort = (0.05940048/1000) * 0;
			parameter[0].precdiffort = (0.11536748/1000) * 0;
			// transect weather variation
			parameter[0].tempjandiffortmin = (0.001262349/1000) * treerows;
			parameter[0].tempjuldiffortmin = (0.05785078/1000) * treerows;
			parameter[0].tempdiffortmin = (0.05940048/1000) * treerows;
			parameter[0].precdiffortmin = (0.11536748/1000) * treerows;
        } else if (parameter[0].weatherchoice == 2100232) {
            char tempbuf[] = "input/trans_Buor Khaya_shift0N_temp_minus499-2100_rcp85.csv";
            char precbuf[] = "input/trans_Buor Khaya_shift0N_prec_minus499-2100_rcp85.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
			// baseline weather manipulation
		    parameter[0].tempjandiffort = (-0.133395946/1000) * 0;// in m: negative values for northward/colder areas
		    parameter[0].tempjuldiffort = (0.05886988/1000) * 0;
		    parameter[0].tempdiffort = (-0.02204296/1000) * 0;
			parameter[0].precdiffort = (0.00842138/1000) * 0;
			// transect weather variation
			parameter[0].tempjandiffortmin = (-0.133395946/1000) * treerows;
			parameter[0].tempjuldiffortmin = (0.05886988/1000) * treerows;
			parameter[0].tempdiffortmin = (-0.02204296/1000) * treerows;
			parameter[0].precdiffortmin = (0.00842138/1000) * treerows;
         } else if (parameter[0].weatherchoice == 2100242) {
            char tempbuf[] = "input/trans_Kolyma_shift0N_temp_minus499-2100_rcp85.csv";
            char precbuf[] = "input/trans_Kolyma_shift0N_prec_minus499-2100_rcp85.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
			// baseline weather manipulation
		    parameter[0].tempjandiffort = (0.001623102/1000) * 0;// in m: negative values for northward/colder areas
		    parameter[0].tempjuldiffort = (0.12172445/1000) * 0;
		    parameter[0].tempdiffort = (0.04269392/1000) * 0;
			parameter[0].precdiffort = (0.21769910/1000) * 0;
			// transect weather variation
			parameter[0].tempjandiffortmin = (0.001623102/1000) * treerows;
			parameter[0].tempjuldiffortmin = (0.12172445/1000) * treerows;
			parameter[0].tempdiffortmin = (0.04269392/1000) * treerows;
			parameter[0].precdiffortmin = (0.21769910/1000) * treerows;
        } else if (parameter[0].weatherchoice == 2100252) {
            char tempbuf[] = "input/trans_Chukotka_shift0N_temp_minus499-2100_rcp85.csv";
            char precbuf[] = "input/trans_Chukotka_shift0N_prec_minus499-2100_rcp85.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
			// baseline weather manipulation
		    parameter[0].tempjandiffort = (0.128972760/1000) * 0;// in m: negative values for northward/colder areas
		    parameter[0].tempjuldiffort = (0.05026903/1000) * 0;
		    parameter[0].tempdiffort = (0.15173684/1000) * 0;
			parameter[0].precdiffort = (-0.27962335/1000) * 0;
			// transect weather variation
			parameter[0].tempjandiffortmin = (0.128972760/1000) * treerows;
			parameter[0].tempjuldiffortmin = (0.05026903/1000) * treerows;
			parameter[0].tempdiffortmin = (0.15173684/1000) * treerows;
			parameter[0].precdiffortmin = (-0.27962335/1000) * treerows;
			
			






	// section of multiple scenarios
		} else if (parameter[0].weatherchoice == 2100315) {//499:2300 so 1800 years long - january july temp for right representation of transects +++ last number gridcell for RCP8.5
		// increasing scenario while 412 is stabilized ones
            char tempbuf[] = "input/trans_Polar Urals_shift00_temp_499-2300_rcp85increasing.csv";
            char precbuf[] = "input/trans_Polar Urals_shift00_prec_499-2300_rcp85increasing.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
			// baseline weather manipulation
		    parameter[0].tempjandiffort = (0.049983234/1000) * parameter[0].locationshift;// in m: negative values for northward/colder areas
		    parameter[0].tempjuldiffort = (0.08816501/1000) * parameter[0].locationshift;
			parameter[0].precdiffort = (0.31567919/1000) * parameter[0].locationshift;
			// transect weather variation
			parameter[0].tempjandiffortmin = (0.049983234/1000) * -1.0 * treerows;
			parameter[0].tempjuldiffortmin = (0.08816501/1000) * -1.0 * treerows;
			parameter[0].precdiffortmin = (0.31567919/1000) * -1.0 * treerows;			
        } else if (parameter[0].weatherchoice == 2100325) {
            char tempbuf[] = "input/trans_Taimyr Peninsula_shift00_temp_499-2300_rcp85increasing.csv";
            char precbuf[] = "input/trans_Taimyr Peninsula_shift00_prec_499-2300_rcp85increasing.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
			// baseline weather manipulation
		    parameter[0].tempjandiffort = (0.001262349/1000) * parameter[0].locationshift;// in m: negative values for northward/colder areas
		    parameter[0].tempjuldiffort = (0.05785078/1000) * parameter[0].locationshift;
			parameter[0].precdiffort = (0.11536748/1000) * parameter[0].locationshift;
			// transect weather variation
			parameter[0].tempjandiffortmin = (0.001262349/1000) * -1.0 * treerows;
			parameter[0].tempjuldiffortmin = (0.05785078/1000) * -1.0 * treerows;
			parameter[0].precdiffortmin = (0.11536748/1000) * -1.0 * treerows;
        } else if (parameter[0].weatherchoice == 2100335) {
            char tempbuf[] = "input/trans_Buor Khaya_shift00_temp_499-2300_rcp85increasing.csv";
            char precbuf[] = "input/trans_Buor Khaya_shift00_prec_499-2300_rcp85increasing.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
			// baseline weather manipulation
		    parameter[0].tempjandiffort = (-0.133395946/1000) * parameter[0].locationshift;// in m: negative values for northward/colder areas
		    parameter[0].tempjuldiffort = (0.05886988/1000) * parameter[0].locationshift;
			parameter[0].precdiffort = (0.00842138/1000) * parameter[0].locationshift;
			// transect weather variation
			parameter[0].tempjandiffortmin = (-0.133395946/1000) * -1.0 * treerows;
			parameter[0].tempjuldiffortmin = (0.05886988/1000) * -1.0 * treerows;
			parameter[0].precdiffortmin = (0.00842138/1000) * -1.0 * treerows;
         } else if (parameter[0].weatherchoice == 2100345) {
            char tempbuf[] = "input/trans_Kolyma_shift00_temp_499-2300_rcp85increasing.csv";
            char precbuf[] = "input/trans_Kolyma_shift00_prec_499-2300_rcp85increasing.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
			// baseline weather manipulation
		    parameter[0].tempjandiffort = (0.001623102/1000) * parameter[0].locationshift;// in m: negative values for northward/colder areas
		    parameter[0].tempjuldiffort = (0.12172445/1000) * parameter[0].locationshift;
			parameter[0].precdiffort = (0.21769910/1000) * parameter[0].locationshift;
			// transect weather variation
			parameter[0].tempjandiffortmin = (0.001623102/1000) * -1.0 * treerows;
			parameter[0].tempjuldiffortmin = (0.12172445/1000) * -1.0 * treerows;
			parameter[0].precdiffortmin = (0.21769910/1000) * -1.0 * treerows;
        } else if (parameter[0].weatherchoice == 2100355) {
            char tempbuf[] = "input/trans_Chukotka_shift00_temp_499-2300_rcp85increasing.csv";
            char precbuf[] = "input/trans_Chukotka_shift00_prec_499-2300_rcp85increasing.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
			// baseline weather manipulation
		    parameter[0].tempjandiffort = (0.128972760/1000) * parameter[0].locationshift;// in m: negative values for northward/colder areas
		    parameter[0].tempjuldiffort = (0.05026903/1000) * parameter[0].locationshift;
			parameter[0].precdiffort = (-0.27962335/1000) * parameter[0].locationshift;
			// transect weather variation
			parameter[0].tempjandiffortmin = (0.128972760/1000) * -1.0 * treerows;
			parameter[0].tempjuldiffortmin = (0.05026903/1000) * -1.0 * treerows;
			parameter[0].precdiffortmin = (-0.27962335/1000) * -1.0 * treerows;
			
			
			
			
		} else if (parameter[0].weatherchoice == 2100415) {//499:2300 so 1800 years long - january july temp for right representation of transects +++ last number gridcell for RCP8.5
		// increasing scenario while 412 is stabilized ones
            char tempbuf[] = "input/trans_Polar Urals_shift00_temp_499-2300_rcp85stabilized.csv";
            char precbuf[] = "input/trans_Polar Urals_shift00_prec_499-2300_rcp85stabilized.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
			// baseline weather manipulation
		    parameter[0].tempjandiffort = (0.049983234/1000) * parameter[0].locationshift;// in m: negative values for northward/colder areas
		    parameter[0].tempjuldiffort = (0.08816501/1000) * parameter[0].locationshift;
			parameter[0].precdiffort = (0.31567919/1000) * parameter[0].locationshift;
			// transect weather variation
			parameter[0].tempjandiffortmin = (0.049983234/1000) * -1.0 * treerows;
			parameter[0].tempjuldiffortmin = (0.08816501/1000) * -1.0 * treerows;
			parameter[0].precdiffortmin = (0.31567919/1000) * -1.0 * treerows;			
        } else if (parameter[0].weatherchoice == 2100425) {
            char tempbuf[] = "input/trans_Taimyr Peninsula_shift00_temp_499-2300_rcp85stabilized.csv";
            char precbuf[] = "input/trans_Taimyr Peninsula_shift00_prec_499-2300_rcp85stabilized.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
			// baseline weather manipulation
		    parameter[0].tempjandiffort = (0.001262349/1000) * parameter[0].locationshift;// in m: negative values for northward/colder areas
		    parameter[0].tempjuldiffort = (0.05785078/1000) * parameter[0].locationshift;
			parameter[0].precdiffort = (0.11536748/1000) * parameter[0].locationshift;
			// transect weather variation
			parameter[0].tempjandiffortmin = (0.001262349/1000) * -1.0 * treerows;
			parameter[0].tempjuldiffortmin = (0.05785078/1000) * -1.0 * treerows;
			parameter[0].precdiffortmin = (0.11536748/1000) * -1.0 * treerows;
        } else if (parameter[0].weatherchoice == 2100435) {
            char tempbuf[] = "input/trans_Buor Khaya_shift00_temp_499-2300_rcp85stabilized.csv";
            char precbuf[] = "input/trans_Buor Khaya_shift00_prec_499-2300_rcp85stabilized.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
			// baseline weather manipulation
		    parameter[0].tempjandiffort = (-0.133395946/1000) * parameter[0].locationshift;// in m: negative values for northward/colder areas
		    parameter[0].tempjuldiffort = (0.05886988/1000) * parameter[0].locationshift;
			parameter[0].precdiffort = (0.00842138/1000) * parameter[0].locationshift;
			// transect weather variation
			parameter[0].tempjandiffortmin = (-0.133395946/1000) * -1.0 * treerows;
			parameter[0].tempjuldiffortmin = (0.05886988/1000) * -1.0 * treerows;
			parameter[0].precdiffortmin = (0.00842138/1000) * -1.0 * treerows;
         } else if (parameter[0].weatherchoice == 2100445) {
            char tempbuf[] = "input/trans_Kolyma_shift00_temp_499-2300_rcp85stabilized.csv";
            char precbuf[] = "input/trans_Kolyma_shift00_prec_499-2300_rcp85stabilized.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
			// baseline weather manipulation
		    parameter[0].tempjandiffort = (0.001623102/1000) * parameter[0].locationshift;// in m: negative values for northward/colder areas
		    parameter[0].tempjuldiffort = (0.12172445/1000) * parameter[0].locationshift;
			parameter[0].precdiffort = (0.21769910/1000) * parameter[0].locationshift;
			// transect weather variation
			parameter[0].tempjandiffortmin = (0.001623102/1000) * -1.0 * treerows;
			parameter[0].tempjuldiffortmin = (0.12172445/1000) * -1.0 * treerows;
			parameter[0].precdiffortmin = (0.21769910/1000) * -1.0 * treerows;
        } else if (parameter[0].weatherchoice == 2100455) {
            char tempbuf[] = "input/trans_Chukotka_shift00_temp_499-2300_rcp85stabilized.csv";
            char precbuf[] = "input/trans_Chukotka_shift00_prec_499-2300_rcp85stabilized.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
			// baseline weather manipulation
		    parameter[0].tempjandiffort = (0.128972760/1000) * parameter[0].locationshift;// in m: negative values for northward/colder areas
		    parameter[0].tempjuldiffort = (0.05026903/1000) * parameter[0].locationshift;
			parameter[0].precdiffort = (-0.27962335/1000) * parameter[0].locationshift;
			// transect weather variation
			parameter[0].tempjandiffortmin = (0.128972760/1000) * -1.0 * treerows;
			parameter[0].tempjuldiffortmin = (0.05026903/1000) * -1.0 * treerows;
			parameter[0].precdiffortmin = (-0.27962335/1000) * -1.0 * treerows;
			
			
		} else if (parameter[0].weatherchoice == 2100515) {//499:2300 so 1800 years long - january july temp for right representation of transects +++ last number gridcell for RCP8.5
		// increasing scenario while 412 is stabilized ones
            char tempbuf[] = "input/trans_Polar Urals_shift00_temp_499-2300_rcp45increasing.csv";
            char precbuf[] = "input/trans_Polar Urals_shift00_prec_499-2300_rcp45increasing.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
			// baseline weather manipulation
		    parameter[0].tempjandiffort = (0.049983234/1000) * parameter[0].locationshift;// in m: negative values for northward/colder areas
		    parameter[0].tempjuldiffort = (0.08816501/1000) * parameter[0].locationshift;
			parameter[0].precdiffort = (0.31567919/1000) * parameter[0].locationshift;
			// transect weather variation
			parameter[0].tempjandiffortmin = (0.049983234/1000) * -1.0 * treerows;
			parameter[0].tempjuldiffortmin = (0.08816501/1000) * -1.0 * treerows;
			parameter[0].precdiffortmin = (0.31567919/1000) * -1.0 * treerows;			
        } else if (parameter[0].weatherchoice == 2100525) {
            char tempbuf[] = "input/trans_Taimyr Peninsula_shift00_temp_499-2300_rcp45increasing.csv";
            char precbuf[] = "input/trans_Taimyr Peninsula_shift00_prec_499-2300_rcp45increasing.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
			// baseline weather manipulation
		    parameter[0].tempjandiffort = (0.001262349/1000) * parameter[0].locationshift;// in m: negative values for northward/colder areas
		    parameter[0].tempjuldiffort = (0.05785078/1000) * parameter[0].locationshift;
			parameter[0].precdiffort = (0.11536748/1000) * parameter[0].locationshift;
			// transect weather variation
			parameter[0].tempjandiffortmin = (0.001262349/1000) * -1.0 * treerows;
			parameter[0].tempjuldiffortmin = (0.05785078/1000) * -1.0 * treerows;
			parameter[0].precdiffortmin = (0.11536748/1000) * -1.0 * treerows;
        } else if (parameter[0].weatherchoice == 2100535) {
            char tempbuf[] = "input/trans_Buor Khaya_shift00_temp_499-2300_rcp45increasing.csv";
            char precbuf[] = "input/trans_Buor Khaya_shift00_prec_499-2300_rcp45increasing.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
			// baseline weather manipulation
		    parameter[0].tempjandiffort = (-0.133395946/1000) * parameter[0].locationshift;// in m: negative values for northward/colder areas
		    parameter[0].tempjuldiffort = (0.05886988/1000) * parameter[0].locationshift;
			parameter[0].precdiffort = (0.00842138/1000) * parameter[0].locationshift;
			// transect weather variation
			parameter[0].tempjandiffortmin = (-0.133395946/1000) * -1.0 * treerows;
			parameter[0].tempjuldiffortmin = (0.05886988/1000) * -1.0 * treerows;
			parameter[0].precdiffortmin = (0.00842138/1000) * -1.0 * treerows;
         } else if (parameter[0].weatherchoice == 2100545) {
            char tempbuf[] = "input/trans_Kolyma_shift00_temp_499-2300_rcp45increasing.csv";
            char precbuf[] = "input/trans_Kolyma_shift00_prec_499-2300_rcp45increasing.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
			// baseline weather manipulation
		    parameter[0].tempjandiffort = (0.001623102/1000) * parameter[0].locationshift;// in m: negative values for northward/colder areas
		    parameter[0].tempjuldiffort = (0.12172445/1000) * parameter[0].locationshift;
			parameter[0].precdiffort = (0.21769910/1000) * parameter[0].locationshift;
			// transect weather variation
			parameter[0].tempjandiffortmin = (0.001623102/1000) * -1.0 * treerows;
			parameter[0].tempjuldiffortmin = (0.12172445/1000) * -1.0 * treerows;
			parameter[0].precdiffortmin = (0.21769910/1000) * -1.0 * treerows;
        } else if (parameter[0].weatherchoice == 2100555) {
            char tempbuf[] = "input/trans_Chukotka_shift00_temp_499-2300_rcp45increasing.csv";
            char precbuf[] = "input/trans_Chukotka_shift00_prec_499-2300_rcp45increasing.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
			// baseline weather manipulation
		    parameter[0].tempjandiffort = (0.128972760/1000) * parameter[0].locationshift;// in m: negative values for northward/colder areas
		    parameter[0].tempjuldiffort = (0.05026903/1000) * parameter[0].locationshift;
			parameter[0].precdiffort = (-0.27962335/1000) * parameter[0].locationshift;
			// transect weather variation
			parameter[0].tempjandiffortmin = (0.128972760/1000) * -1.0 * treerows;
			parameter[0].tempjuldiffortmin = (0.05026903/1000) * -1.0 * treerows;
			parameter[0].precdiffortmin = (-0.27962335/1000) * -1.0 * treerows;
			
			
			
			
		} else if (parameter[0].weatherchoice == 2100615) {//499:2300 so 1800 years long - january july temp for right representation of transects +++ last number gridcell for RCP8.5
		// increasing scenario while 412 is stabilized ones
            char tempbuf[] = "input/trans_Polar Urals_shift00_temp_499-2300_rcp45stabilized.csv";
            char precbuf[] = "input/trans_Polar Urals_shift00_prec_499-2300_rcp45stabilized.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
			// baseline weather manipulation
		    parameter[0].tempjandiffort = (0.049983234/1000) * parameter[0].locationshift;// in m: negative values for northward/colder areas
		    parameter[0].tempjuldiffort = (0.08816501/1000) * parameter[0].locationshift;
			parameter[0].precdiffort = (0.31567919/1000) * parameter[0].locationshift;
			// transect weather variation
			parameter[0].tempjandiffortmin = (0.049983234/1000) * -1.0 * treerows;
			parameter[0].tempjuldiffortmin = (0.08816501/1000) * -1.0 * treerows;
			parameter[0].precdiffortmin = (0.31567919/1000) * -1.0 * treerows;			
        } else if (parameter[0].weatherchoice == 2100625) {
            char tempbuf[] = "input/trans_Taimyr Peninsula_shift00_temp_499-2300_rcp45stabilized.csv";
            char precbuf[] = "input/trans_Taimyr Peninsula_shift00_prec_499-2300_rcp45stabilized.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
			// baseline weather manipulation
		    parameter[0].tempjandiffort = (0.001262349/1000) * parameter[0].locationshift;// in m: negative values for northward/colder areas
		    parameter[0].tempjuldiffort = (0.05785078/1000) * parameter[0].locationshift;
			parameter[0].precdiffort = (0.11536748/1000) * parameter[0].locationshift;
			// transect weather variation
			parameter[0].tempjandiffortmin = (0.001262349/1000) * -1.0 * treerows;
			parameter[0].tempjuldiffortmin = (0.05785078/1000) * -1.0 * treerows;
			parameter[0].precdiffortmin = (0.11536748/1000) * -1.0 * treerows;
        } else if (parameter[0].weatherchoice == 2100635) {
            char tempbuf[] = "input/trans_Buor Khaya_shift00_temp_499-2300_rcp45stabilized.csv";
            char precbuf[] = "input/trans_Buor Khaya_shift00_prec_499-2300_rcp45stabilized.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
			// baseline weather manipulation
		    parameter[0].tempjandiffort = (-0.133395946/1000) * parameter[0].locationshift;// in m: negative values for northward/colder areas
		    parameter[0].tempjuldiffort = (0.05886988/1000) * parameter[0].locationshift;
			parameter[0].precdiffort = (0.00842138/1000) * parameter[0].locationshift;
			// transect weather variation
			parameter[0].tempjandiffortmin = (-0.133395946/1000) * -1.0 * treerows;
			parameter[0].tempjuldiffortmin = (0.05886988/1000) * -1.0 * treerows;
			parameter[0].precdiffortmin = (0.00842138/1000) * -1.0 * treerows;
         } else if (parameter[0].weatherchoice == 2100645) {
            char tempbuf[] = "input/trans_Kolyma_shift00_temp_499-2300_rcp45stabilized.csv";
            char precbuf[] = "input/trans_Kolyma_shift00_prec_499-2300_rcp45stabilized.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
			// baseline weather manipulation
		    parameter[0].tempjandiffort = (0.001623102/1000) * parameter[0].locationshift;// in m: negative values for northward/colder areas
		    parameter[0].tempjuldiffort = (0.12172445/1000) * parameter[0].locationshift;
			parameter[0].precdiffort = (0.21769910/1000) * parameter[0].locationshift;
			// transect weather variation
			parameter[0].tempjandiffortmin = (0.001623102/1000) * -1.0 * treerows;
			parameter[0].tempjuldiffortmin = (0.12172445/1000) * -1.0 * treerows;
			parameter[0].precdiffortmin = (0.21769910/1000) * -1.0 * treerows;
        } else if (parameter[0].weatherchoice == 2100655) {
            char tempbuf[] = "input/trans_Chukotka_shift00_temp_499-2300_rcp45stabilized.csv";
            char precbuf[] = "input/trans_Chukotka_shift00_prec_499-2300_rcp45stabilized.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
			// baseline weather manipulation
		    parameter[0].tempjandiffort = (0.128972760/1000) * parameter[0].locationshift;// in m: negative values for northward/colder areas
		    parameter[0].tempjuldiffort = (0.05026903/1000) * parameter[0].locationshift;
			parameter[0].precdiffort = (-0.27962335/1000) * parameter[0].locationshift;
			// transect weather variation
			parameter[0].tempjandiffortmin = (0.128972760/1000) * -1.0 * treerows;
			parameter[0].tempjuldiffortmin = (0.05026903/1000) * -1.0 * treerows;
			parameter[0].precdiffortmin = (-0.27962335/1000) * -1.0 * treerows;







		} else if (parameter[0].weatherchoice == 2100312) {//499:2300 so 1800 years long - january july temp for right representation of transects +++ last number gridcell for RCP8.5
		// increasing scenario while 412 is stabilized ones
            char tempbuf[] = "input/trans_Polar Urals_shift0N_temp_499-2300_rcp85increasing.csv";
            char precbuf[] = "input/trans_Polar Urals_shift0N_prec_499-2300_rcp85increasing.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
			// baseline weather manipulation
		    parameter[0].tempjandiffort = (0.049983234/1000) * parameter[0].locationshift;// in m: negative values for northward/colder areas
		    parameter[0].tempjuldiffort = (0.08816501/1000) * parameter[0].locationshift;
			parameter[0].precdiffort = (0.31567919/1000) * parameter[0].locationshift;
			// transect weather variation
			parameter[0].tempjandiffortmin = (0.049983234/1000) * -1.0 * treerows;
			parameter[0].tempjuldiffortmin = (0.08816501/1000) * -1.0 * treerows;
			parameter[0].precdiffortmin = (0.31567919/1000) * -1.0 * treerows;			
        } else if (parameter[0].weatherchoice == 2100322) {
            char tempbuf[] = "input/trans_Taimyr Peninsula_shift0N_temp_499-2300_rcp85increasing.csv";
            char precbuf[] = "input/trans_Taimyr Peninsula_shift0N_prec_499-2300_rcp85increasing.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
			// baseline weather manipulation
		    parameter[0].tempjandiffort = (0.001262349/1000) * parameter[0].locationshift;// in m: negative values for northward/colder areas
		    parameter[0].tempjuldiffort = (0.05785078/1000) * parameter[0].locationshift;
			parameter[0].precdiffort = (0.11536748/1000) * parameter[0].locationshift;
			// transect weather variation
			parameter[0].tempjandiffortmin = (0.001262349/1000) * -1.0 * treerows;
			parameter[0].tempjuldiffortmin = (0.05785078/1000) * -1.0 * treerows;
			parameter[0].precdiffortmin = (0.11536748/1000) * -1.0 * treerows;
        } else if (parameter[0].weatherchoice == 2100332) {
            char tempbuf[] = "input/trans_Buor Khaya_shift0N_temp_499-2300_rcp85increasing.csv";
            char precbuf[] = "input/trans_Buor Khaya_shift0N_prec_499-2300_rcp85increasing.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
			// baseline weather manipulation
		    parameter[0].tempjandiffort = (-0.133395946/1000) * parameter[0].locationshift;// in m: negative values for northward/colder areas
		    parameter[0].tempjuldiffort = (0.05886988/1000) * parameter[0].locationshift;
			parameter[0].precdiffort = (0.00842138/1000) * parameter[0].locationshift;
			// transect weather variation
			parameter[0].tempjandiffortmin = (-0.133395946/1000) * -1.0 * treerows;
			parameter[0].tempjuldiffortmin = (0.05886988/1000) * -1.0 * treerows;
			parameter[0].precdiffortmin = (0.00842138/1000) * -1.0 * treerows;
         } else if (parameter[0].weatherchoice == 2100342) {
            char tempbuf[] = "input/trans_Kolyma_shift0N_temp_499-2300_rcp85increasing.csv";
            char precbuf[] = "input/trans_Kolyma_shift0N_prec_499-2300_rcp85increasing.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
			// baseline weather manipulation
		    parameter[0].tempjandiffort = (0.001623102/1000) * parameter[0].locationshift;// in m: negative values for northward/colder areas
		    parameter[0].tempjuldiffort = (0.12172445/1000) * parameter[0].locationshift;
			parameter[0].precdiffort = (0.21769910/1000) * parameter[0].locationshift;
			// transect weather variation
			parameter[0].tempjandiffortmin = (0.001623102/1000) * -1.0 * treerows;
			parameter[0].tempjuldiffortmin = (0.12172445/1000) * -1.0 * treerows;
			parameter[0].precdiffortmin = (0.21769910/1000) * -1.0 * treerows;
        } else if (parameter[0].weatherchoice == 2100352) {
            char tempbuf[] = "input/trans_Chukotka_shift0N_temp_499-2300_rcp85increasing.csv";
            char precbuf[] = "input/trans_Chukotka_shift0N_prec_499-2300_rcp85increasing.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
			// baseline weather manipulation
		    parameter[0].tempjandiffort = (0.128972760/1000) * parameter[0].locationshift;// in m: negative values for northward/colder areas
		    parameter[0].tempjuldiffort = (0.05026903/1000) * parameter[0].locationshift;
			parameter[0].precdiffort = (-0.27962335/1000) * parameter[0].locationshift;
			// transect weather variation
			parameter[0].tempjandiffortmin = (0.128972760/1000) * -1.0 * treerows;
			parameter[0].tempjuldiffortmin = (0.05026903/1000) * -1.0 * treerows;
			parameter[0].precdiffortmin = (-0.27962335/1000) * -1.0 * treerows;
			
			
			
			
		} else if (parameter[0].weatherchoice == 2100412) {//499:2300 so 1800 years long - january july temp for right representation of transects +++ last number gridcell for RCP8.5
		// increasing scenario while 412 is stabilized ones
            char tempbuf[] = "input/trans_Polar Urals_shift0N_temp_499-2300_rcp85stabilized.csv";
            char precbuf[] = "input/trans_Polar Urals_shift0N_prec_499-2300_rcp85stabilized.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
			// baseline weather manipulation
		    parameter[0].tempjandiffort = (0.049983234/1000) * parameter[0].locationshift;// in m: negative values for northward/colder areas
		    parameter[0].tempjuldiffort = (0.08816501/1000) * parameter[0].locationshift;
			parameter[0].precdiffort = (0.31567919/1000) * parameter[0].locationshift;
			// transect weather variation
			parameter[0].tempjandiffortmin = (0.049983234/1000) * -1.0 * treerows;
			parameter[0].tempjuldiffortmin = (0.08816501/1000) * -1.0 * treerows;
			parameter[0].precdiffortmin = (0.31567919/1000) * -1.0 * treerows;			
        } else if (parameter[0].weatherchoice == 2100422) {
            char tempbuf[] = "input/trans_Taimyr Peninsula_shift0N_temp_499-2300_rcp85stabilized.csv";
            char precbuf[] = "input/trans_Taimyr Peninsula_shift0N_prec_499-2300_rcp85stabilized.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
			// baseline weather manipulation
		    parameter[0].tempjandiffort = (0.001262349/1000) * parameter[0].locationshift;// in m: negative values for northward/colder areas
		    parameter[0].tempjuldiffort = (0.05785078/1000) * parameter[0].locationshift;
			parameter[0].precdiffort = (0.11536748/1000) * parameter[0].locationshift;
			// transect weather variation
			parameter[0].tempjandiffortmin = (0.001262349/1000) * -1.0 * treerows;
			parameter[0].tempjuldiffortmin = (0.05785078/1000) * -1.0 * treerows;
			parameter[0].precdiffortmin = (0.11536748/1000) * -1.0 * treerows;
        } else if (parameter[0].weatherchoice == 2100432) {
            char tempbuf[] = "input/trans_Buor Khaya_shift0N_temp_499-2300_rcp85stabilized.csv";
            char precbuf[] = "input/trans_Buor Khaya_shift0N_prec_499-2300_rcp85stabilized.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
			// baseline weather manipulation
		    parameter[0].tempjandiffort = (-0.133395946/1000) * parameter[0].locationshift;// in m: negative values for northward/colder areas
		    parameter[0].tempjuldiffort = (0.05886988/1000) * parameter[0].locationshift;
			parameter[0].precdiffort = (0.00842138/1000) * parameter[0].locationshift;
			// transect weather variation
			parameter[0].tempjandiffortmin = (-0.133395946/1000) * -1.0 * treerows;
			parameter[0].tempjuldiffortmin = (0.05886988/1000) * -1.0 * treerows;
			parameter[0].precdiffortmin = (0.00842138/1000) * -1.0 * treerows;
         } else if (parameter[0].weatherchoice == 2100442) {
            char tempbuf[] = "input/trans_Kolyma_shift0N_temp_499-2300_rcp85stabilized.csv";
            char precbuf[] = "input/trans_Kolyma_shift0N_prec_499-2300_rcp85stabilized.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
			// baseline weather manipulation
		    parameter[0].tempjandiffort = (0.001623102/1000) * parameter[0].locationshift;// in m: negative values for northward/colder areas
		    parameter[0].tempjuldiffort = (0.12172445/1000) * parameter[0].locationshift;
			parameter[0].precdiffort = (0.21769910/1000) * parameter[0].locationshift;
			// transect weather variation
			parameter[0].tempjandiffortmin = (0.001623102/1000) * -1.0 * treerows;
			parameter[0].tempjuldiffortmin = (0.12172445/1000) * -1.0 * treerows;
			parameter[0].precdiffortmin = (0.21769910/1000) * -1.0 * treerows;
        } else if (parameter[0].weatherchoice == 2100452) {
            char tempbuf[] = "input/trans_Chukotka_shift0N_temp_499-2300_rcp85stabilized.csv";
            char precbuf[] = "input/trans_Chukotka_shift0N_prec_499-2300_rcp85stabilized.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
			// baseline weather manipulation
		    parameter[0].tempjandiffort = (0.128972760/1000) * parameter[0].locationshift;// in m: negative values for northward/colder areas
		    parameter[0].tempjuldiffort = (0.05026903/1000) * parameter[0].locationshift;
			parameter[0].precdiffort = (-0.27962335/1000) * parameter[0].locationshift;
			// transect weather variation
			parameter[0].tempjandiffortmin = (0.128972760/1000) * -1.0 * treerows;
			parameter[0].tempjuldiffortmin = (0.05026903/1000) * -1.0 * treerows;
			parameter[0].precdiffortmin = (-0.27962335/1000) * -1.0 * treerows;
			
			
		} else if (parameter[0].weatherchoice == 2100512) {//499:2300 so 1800 years long - january july temp for right representation of transects +++ last number gridcell for RCP8.5
		// increasing scenario while 412 is stabilized ones
            char tempbuf[] = "input/trans_Polar Urals_shift0N_temp_499-2300_rcp45increasing.csv";
            char precbuf[] = "input/trans_Polar Urals_shift0N_prec_499-2300_rcp45increasing.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
			// baseline weather manipulation
		    parameter[0].tempjandiffort = (0.049983234/1000) * parameter[0].locationshift;// in m: negative values for northward/colder areas
		    parameter[0].tempjuldiffort = (0.08816501/1000) * parameter[0].locationshift;
			parameter[0].precdiffort = (0.31567919/1000) * parameter[0].locationshift;
			// transect weather variation
			parameter[0].tempjandiffortmin = (0.049983234/1000) * -1.0 * treerows;
			parameter[0].tempjuldiffortmin = (0.08816501/1000) * -1.0 * treerows;
			parameter[0].precdiffortmin = (0.31567919/1000) * -1.0 * treerows;			
        } else if (parameter[0].weatherchoice == 2100522) {
            char tempbuf[] = "input/trans_Taimyr Peninsula_shift0N_temp_499-2300_rcp45increasing.csv";
            char precbuf[] = "input/trans_Taimyr Peninsula_shift0N_prec_499-2300_rcp45increasing.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
			// baseline weather manipulation
		    parameter[0].tempjandiffort = (0.001262349/1000) * parameter[0].locationshift;// in m: negative values for northward/colder areas
		    parameter[0].tempjuldiffort = (0.05785078/1000) * parameter[0].locationshift;
			parameter[0].precdiffort = (0.11536748/1000) * parameter[0].locationshift;
			// transect weather variation
			parameter[0].tempjandiffortmin = (0.001262349/1000) * -1.0 * treerows;
			parameter[0].tempjuldiffortmin = (0.05785078/1000) * -1.0 * treerows;
			parameter[0].precdiffortmin = (0.11536748/1000) * -1.0 * treerows;
        } else if (parameter[0].weatherchoice == 2100532) {
            char tempbuf[] = "input/trans_Buor Khaya_shift0N_temp_499-2300_rcp45increasing.csv";
            char precbuf[] = "input/trans_Buor Khaya_shift0N_prec_499-2300_rcp45increasing.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
			// baseline weather manipulation
		    parameter[0].tempjandiffort = (-0.133395946/1000) * parameter[0].locationshift;// in m: negative values for northward/colder areas
		    parameter[0].tempjuldiffort = (0.05886988/1000) * parameter[0].locationshift;
			parameter[0].precdiffort = (0.00842138/1000) * parameter[0].locationshift;
			// transect weather variation
			parameter[0].tempjandiffortmin = (-0.133395946/1000) * -1.0 * treerows;
			parameter[0].tempjuldiffortmin = (0.05886988/1000) * -1.0 * treerows;
			parameter[0].precdiffortmin = (0.00842138/1000) * -1.0 * treerows;
         } else if (parameter[0].weatherchoice == 2100542) {
            char tempbuf[] = "input/trans_Kolyma_shift0N_temp_499-2300_rcp45increasing.csv";
            char precbuf[] = "input/trans_Kolyma_shift0N_prec_499-2300_rcp45increasing.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
			// baseline weather manipulation
		    parameter[0].tempjandiffort = (0.001623102/1000) * parameter[0].locationshift;// in m: negative values for northward/colder areas
		    parameter[0].tempjuldiffort = (0.12172445/1000) * parameter[0].locationshift;
			parameter[0].precdiffort = (0.21769910/1000) * parameter[0].locationshift;
			// transect weather variation
			parameter[0].tempjandiffortmin = (0.001623102/1000) * -1.0 * treerows;
			parameter[0].tempjuldiffortmin = (0.12172445/1000) * -1.0 * treerows;
			parameter[0].precdiffortmin = (0.21769910/1000) * -1.0 * treerows;
        } else if (parameter[0].weatherchoice == 2100552) {
            char tempbuf[] = "input/trans_Chukotka_shift0N_temp_499-2300_rcp45increasing.csv";
            char precbuf[] = "input/trans_Chukotka_shift0N_prec_499-2300_rcp45increasing.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
			// baseline weather manipulation
		    parameter[0].tempjandiffort = (0.128972760/1000) * parameter[0].locationshift;// in m: negative values for northward/colder areas
		    parameter[0].tempjuldiffort = (0.05026903/1000) * parameter[0].locationshift;
			parameter[0].precdiffort = (-0.27962335/1000) * parameter[0].locationshift;
			// transect weather variation
			parameter[0].tempjandiffortmin = (0.128972760/1000) * -1.0 * treerows;
			parameter[0].tempjuldiffortmin = (0.05026903/1000) * -1.0 * treerows;
			parameter[0].precdiffortmin = (-0.27962335/1000) * -1.0 * treerows;
			
			
			
			
		} else if (parameter[0].weatherchoice == 2100612) {//499:2300 so 1800 years long - january july temp for right representation of transects +++ last number gridcell for RCP8.5
		// increasing scenario while 412 is stabilized ones
            char tempbuf[] = "input/trans_Polar Urals_shift0N_temp_499-2300_rcp45stabilized.csv";
            char precbuf[] = "input/trans_Polar Urals_shift0N_prec_499-2300_rcp45stabilized.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
			// baseline weather manipulation
		    parameter[0].tempjandiffort = (0.049983234/1000) * parameter[0].locationshift;// in m: negative values for northward/colder areas
		    parameter[0].tempjuldiffort = (0.08816501/1000) * parameter[0].locationshift;
			parameter[0].precdiffort = (0.31567919/1000) * parameter[0].locationshift;
			// transect weather variation
			parameter[0].tempjandiffortmin = (0.049983234/1000) * -1.0 * treerows;
			parameter[0].tempjuldiffortmin = (0.08816501/1000) * -1.0 * treerows;
			parameter[0].precdiffortmin = (0.31567919/1000) * -1.0 * treerows;			
        } else if (parameter[0].weatherchoice == 2100622) {
            char tempbuf[] = "input/trans_Taimyr Peninsula_shift0N_temp_499-2300_rcp45stabilized.csv";
            char precbuf[] = "input/trans_Taimyr Peninsula_shift0N_prec_499-2300_rcp45stabilized.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
			// baseline weather manipulation
		    parameter[0].tempjandiffort = (0.001262349/1000) * parameter[0].locationshift;// in m: negative values for northward/colder areas
		    parameter[0].tempjuldiffort = (0.05785078/1000) * parameter[0].locationshift;
			parameter[0].precdiffort = (0.11536748/1000) * parameter[0].locationshift;
			// transect weather variation
			parameter[0].tempjandiffortmin = (0.001262349/1000) * -1.0 * treerows;
			parameter[0].tempjuldiffortmin = (0.05785078/1000) * -1.0 * treerows;
			parameter[0].precdiffortmin = (0.11536748/1000) * -1.0 * treerows;
        } else if (parameter[0].weatherchoice == 2100632) {
            char tempbuf[] = "input/trans_Buor Khaya_shift0N_temp_499-2300_rcp45stabilized.csv";
            char precbuf[] = "input/trans_Buor Khaya_shift0N_prec_499-2300_rcp45stabilized.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
			// baseline weather manipulation
		    parameter[0].tempjandiffort = (-0.133395946/1000) * parameter[0].locationshift;// in m: negative values for northward/colder areas
		    parameter[0].tempjuldiffort = (0.05886988/1000) * parameter[0].locationshift;
			parameter[0].precdiffort = (0.00842138/1000) * parameter[0].locationshift;
			// transect weather variation
			parameter[0].tempjandiffortmin = (-0.133395946/1000) * -1.0 * treerows;
			parameter[0].tempjuldiffortmin = (0.05886988/1000) * -1.0 * treerows;
			parameter[0].precdiffortmin = (0.00842138/1000) * -1.0 * treerows;
         } else if (parameter[0].weatherchoice == 2100642) {
            char tempbuf[] = "input/trans_Kolyma_shift0N_temp_499-2300_rcp45stabilized.csv";
            char precbuf[] = "input/trans_Kolyma_shift0N_prec_499-2300_rcp45stabilized.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
			// baseline weather manipulation
		    parameter[0].tempjandiffort = (0.001623102/1000) * parameter[0].locationshift;// in m: negative values for northward/colder areas
		    parameter[0].tempjuldiffort = (0.12172445/1000) * parameter[0].locationshift;
			parameter[0].precdiffort = (0.21769910/1000) * parameter[0].locationshift;
			// transect weather variation
			parameter[0].tempjandiffortmin = (0.001623102/1000) * -1.0 * treerows;
			parameter[0].tempjuldiffortmin = (0.12172445/1000) * -1.0 * treerows;
			parameter[0].precdiffortmin = (0.21769910/1000) * -1.0 * treerows;
        } else if (parameter[0].weatherchoice == 2100652) {
            char tempbuf[] = "input/trans_Chukotka_shift0N_temp_499-2300_rcp45stabilized.csv";
            char precbuf[] = "input/trans_Chukotka_shift0N_prec_499-2300_rcp45stabilized.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
			// baseline weather manipulation
		    parameter[0].tempjandiffort = (0.128972760/1000) * parameter[0].locationshift;// in m: negative values for northward/colder areas
		    parameter[0].tempjuldiffort = (0.05026903/1000) * parameter[0].locationshift;
			parameter[0].precdiffort = (-0.27962335/1000) * parameter[0].locationshift;
			// transect weather variation
			parameter[0].tempjandiffortmin = (0.128972760/1000) * -1.0 * treerows;
			parameter[0].tempjuldiffortmin = (0.05026903/1000) * -1.0 * treerows;
			parameter[0].precdiffortmin = (-0.27962335/1000) * -1.0 * treerows;








	// decreasing series
		} else if (parameter[0].weatherchoice == 2100715) {//499:2300 so 1800 years long - january july temp for right representation of transects +++ last number gridcell for RCP8.5
		// decreasing scenario while 412 is stabilized ones
            char tempbuf[] = "input/trans_Polar Urals_shift00_temp_499-2300_rcp85decreasing.csv";
            char precbuf[] = "input/trans_Polar Urals_shift00_prec_499-2300_rcp85decreasing.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
			// baseline weather manipulation
		    parameter[0].tempjandiffort = (0.049983234/1000) * parameter[0].locationshift;// in m: negative values for northward/colder areas
		    parameter[0].tempjuldiffort = (0.08816501/1000) * parameter[0].locationshift;
			parameter[0].precdiffort = (0.31567919/1000) * parameter[0].locationshift;
			// transect weather variation
			parameter[0].tempjandiffortmin = (0.049983234/1000) * -1.0 * treerows;
			parameter[0].tempjuldiffortmin = (0.08816501/1000) * -1.0 * treerows;
			parameter[0].precdiffortmin = (0.31567919/1000) * -1.0 * treerows;			
        } else if (parameter[0].weatherchoice == 2100725) {
            char tempbuf[] = "input/trans_Taimyr Peninsula_shift00_temp_499-2300_rcp85decreasing.csv";
            char precbuf[] = "input/trans_Taimyr Peninsula_shift00_prec_499-2300_rcp85decreasing.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
			// baseline weather manipulation
		    parameter[0].tempjandiffort = (0.001262349/1000) * parameter[0].locationshift;// in m: negative values for northward/colder areas
		    parameter[0].tempjuldiffort = (0.05785078/1000) * parameter[0].locationshift;
			parameter[0].precdiffort = (0.11536748/1000) * parameter[0].locationshift;
			// transect weather variation
			parameter[0].tempjandiffortmin = (0.001262349/1000) * -1.0 * treerows;
			parameter[0].tempjuldiffortmin = (0.05785078/1000) * -1.0 * treerows;
			parameter[0].precdiffortmin = (0.11536748/1000) * -1.0 * treerows;
        } else if (parameter[0].weatherchoice == 2100735) {
            char tempbuf[] = "input/trans_Buor Khaya_shift00_temp_499-2300_rcp85decreasing.csv";
            char precbuf[] = "input/trans_Buor Khaya_shift00_prec_499-2300_rcp85decreasing.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
			// baseline weather manipulation
		    parameter[0].tempjandiffort = (-0.133395946/1000) * parameter[0].locationshift;// in m: negative values for northward/colder areas
		    parameter[0].tempjuldiffort = (0.05886988/1000) * parameter[0].locationshift;
			parameter[0].precdiffort = (0.00842138/1000) * parameter[0].locationshift;
			// transect weather variation
			parameter[0].tempjandiffortmin = (-0.133395946/1000) * -1.0 * treerows;
			parameter[0].tempjuldiffortmin = (0.05886988/1000) * -1.0 * treerows;
			parameter[0].precdiffortmin = (0.00842138/1000) * -1.0 * treerows;
         } else if (parameter[0].weatherchoice == 2100745) {
            char tempbuf[] = "input/trans_Kolyma_shift00_temp_499-2300_rcp85decreasing.csv";
            char precbuf[] = "input/trans_Kolyma_shift00_prec_499-2300_rcp85decreasing.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
			// baseline weather manipulation
		    parameter[0].tempjandiffort = (0.001623102/1000) * parameter[0].locationshift;// in m: negative values for northward/colder areas
		    parameter[0].tempjuldiffort = (0.12172445/1000) * parameter[0].locationshift;
			parameter[0].precdiffort = (0.21769910/1000) * parameter[0].locationshift;
			// transect weather variation
			parameter[0].tempjandiffortmin = (0.001623102/1000) * -1.0 * treerows;
			parameter[0].tempjuldiffortmin = (0.12172445/1000) * -1.0 * treerows;
			parameter[0].precdiffortmin = (0.21769910/1000) * -1.0 * treerows;
        } else if (parameter[0].weatherchoice == 2100755) {
            char tempbuf[] = "input/trans_Chukotka_shift00_temp_499-2300_rcp85decreasing.csv";
            char precbuf[] = "input/trans_Chukotka_shift00_prec_499-2300_rcp85decreasing.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
			// baseline weather manipulation
		    parameter[0].tempjandiffort = (0.128972760/1000) * parameter[0].locationshift;// in m: negative values for northward/colder areas
		    parameter[0].tempjuldiffort = (0.05026903/1000) * parameter[0].locationshift;
			parameter[0].precdiffort = (-0.27962335/1000) * parameter[0].locationshift;
			// transect weather variation
			parameter[0].tempjandiffortmin = (0.128972760/1000) * -1.0 * treerows;
			parameter[0].tempjuldiffortmin = (0.05026903/1000) * -1.0 * treerows;
			parameter[0].precdiffortmin = (-0.27962335/1000) * -1.0 * treerows;
			
		} else if (parameter[0].weatherchoice == 2100712) {//499:2300 so 1800 years long - january july temp for right representation of transects +++ last number gridcell for RCP8.5
		// decreasing scenario while 412 is stabilized ones
            char tempbuf[] = "input/trans_Polar Urals_shift0N_temp_499-2300_rcp85decreasing.csv";
            char precbuf[] = "input/trans_Polar Urals_shift0N_prec_499-2300_rcp85decreasing.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
			// baseline weather manipulation
		    parameter[0].tempjandiffort = (0.049983234/1000) * parameter[0].locationshift;// in m: negative values for northward/colder areas
		    parameter[0].tempjuldiffort = (0.08816501/1000) * parameter[0].locationshift;
			parameter[0].precdiffort = (0.31567919/1000) * parameter[0].locationshift;
			// transect weather variation
			parameter[0].tempjandiffortmin = (0.049983234/1000) * -1.0 * treerows;
			parameter[0].tempjuldiffortmin = (0.08816501/1000) * -1.0 * treerows;
			parameter[0].precdiffortmin = (0.31567919/1000) * -1.0 * treerows;			
        } else if (parameter[0].weatherchoice == 2100722) {
            char tempbuf[] = "input/trans_Taimyr Peninsula_shift0N_temp_499-2300_rcp85decreasing.csv";
            char precbuf[] = "input/trans_Taimyr Peninsula_shift0N_prec_499-2300_rcp85decreasing.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
			// baseline weather manipulation
		    parameter[0].tempjandiffort = (0.001262349/1000) * parameter[0].locationshift;// in m: negative values for northward/colder areas
		    parameter[0].tempjuldiffort = (0.05785078/1000) * parameter[0].locationshift;
			parameter[0].precdiffort = (0.11536748/1000) * parameter[0].locationshift;
			// transect weather variation
			parameter[0].tempjandiffortmin = (0.001262349/1000) * -1.0 * treerows;
			parameter[0].tempjuldiffortmin = (0.05785078/1000) * -1.0 * treerows;
			parameter[0].precdiffortmin = (0.11536748/1000) * -1.0 * treerows;
        } else if (parameter[0].weatherchoice == 2100732) {
            char tempbuf[] = "input/trans_Buor Khaya_shift0N_temp_499-2300_rcp85decreasing.csv";
            char precbuf[] = "input/trans_Buor Khaya_shift0N_prec_499-2300_rcp85decreasing.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
			// baseline weather manipulation
		    parameter[0].tempjandiffort = (-0.133395946/1000) * parameter[0].locationshift;// in m: negative values for northward/colder areas
		    parameter[0].tempjuldiffort = (0.05886988/1000) * parameter[0].locationshift;
			parameter[0].precdiffort = (0.00842138/1000) * parameter[0].locationshift;
			// transect weather variation
			parameter[0].tempjandiffortmin = (-0.133395946/1000) * -1.0 * treerows;
			parameter[0].tempjuldiffortmin = (0.05886988/1000) * -1.0 * treerows;
			parameter[0].precdiffortmin = (0.00842138/1000) * -1.0 * treerows;
         } else if (parameter[0].weatherchoice == 2100742) {
            char tempbuf[] = "input/trans_Kolyma_shift0N_temp_499-2300_rcp85decreasing.csv";
            char precbuf[] = "input/trans_Kolyma_shift0N_prec_499-2300_rcp85decreasing.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
			// baseline weather manipulation
		    parameter[0].tempjandiffort = (0.001623102/1000) * parameter[0].locationshift;// in m: negative values for northward/colder areas
		    parameter[0].tempjuldiffort = (0.12172445/1000) * parameter[0].locationshift;
			parameter[0].precdiffort = (0.21769910/1000) * parameter[0].locationshift;
			// transect weather variation
			parameter[0].tempjandiffortmin = (0.001623102/1000) * -1.0 * treerows;
			parameter[0].tempjuldiffortmin = (0.12172445/1000) * -1.0 * treerows;
			parameter[0].precdiffortmin = (0.21769910/1000) * -1.0 * treerows;
        } else if (parameter[0].weatherchoice == 2100752) {
            char tempbuf[] = "input/trans_Chukotka_shift0N_temp_499-2300_rcp85decreasing.csv";
            char precbuf[] = "input/trans_Chukotka_shift0N_prec_499-2300_rcp85decreasing.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
			// baseline weather manipulation
		    parameter[0].tempjandiffort = (0.128972760/1000) * parameter[0].locationshift;// in m: negative values for northward/colder areas
		    parameter[0].tempjuldiffort = (0.05026903/1000) * parameter[0].locationshift;
			parameter[0].precdiffort = (-0.27962335/1000) * parameter[0].locationshift;
			// transect weather variation
			parameter[0].tempjandiffortmin = (0.128972760/1000) * -1.0 * treerows;
			parameter[0].tempjuldiffortmin = (0.05026903/1000) * -1.0 * treerows;
			parameter[0].precdiffortmin = (-0.27962335/1000) * -1.0 * treerows;
	// decreasing rcp 4.5
		} else if (parameter[0].weatherchoice == 2100815) {//499:2300 so 1800 years long - january july temp for right representation of transects +++ last number gridcell for RCP8.5
		// decreasing scenario while 412 is stabilized ones
            char tempbuf[] = "input/trans_Polar Urals_shift00_temp_499-2300_rcp45decreasing.csv";
            char precbuf[] = "input/trans_Polar Urals_shift00_prec_499-2300_rcp45decreasing.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
			// baseline weather manipulation
		    parameter[0].tempjandiffort = (0.049983234/1000) * parameter[0].locationshift;// in m: negative values for northward/colder areas
		    parameter[0].tempjuldiffort = (0.08816501/1000) * parameter[0].locationshift;
			parameter[0].precdiffort = (0.31567919/1000) * parameter[0].locationshift;
			// transect weather variation
			parameter[0].tempjandiffortmin = (0.049983234/1000) * -1.0 * treerows;
			parameter[0].tempjuldiffortmin = (0.08816501/1000) * -1.0 * treerows;
			parameter[0].precdiffortmin = (0.31567919/1000) * -1.0 * treerows;			
        } else if (parameter[0].weatherchoice == 2100825) {
            char tempbuf[] = "input/trans_Taimyr Peninsula_shift00_temp_499-2300_rcp45decreasing.csv";
            char precbuf[] = "input/trans_Taimyr Peninsula_shift00_prec_499-2300_rcp45decreasing.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
			// baseline weather manipulation
		    parameter[0].tempjandiffort = (0.001262349/1000) * parameter[0].locationshift;// in m: negative values for northward/colder areas
		    parameter[0].tempjuldiffort = (0.05785078/1000) * parameter[0].locationshift;
			parameter[0].precdiffort = (0.11536748/1000) * parameter[0].locationshift;
			// transect weather variation
			parameter[0].tempjandiffortmin = (0.001262349/1000) * -1.0 * treerows;
			parameter[0].tempjuldiffortmin = (0.05785078/1000) * -1.0 * treerows;
			parameter[0].precdiffortmin = (0.11536748/1000) * -1.0 * treerows;
        } else if (parameter[0].weatherchoice == 2100835) {
            char tempbuf[] = "input/trans_Buor Khaya_shift00_temp_499-2300_rcp45decreasing.csv";
            char precbuf[] = "input/trans_Buor Khaya_shift00_prec_499-2300_rcp45decreasing.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
			// baseline weather manipulation
		    parameter[0].tempjandiffort = (-0.133395946/1000) * parameter[0].locationshift;// in m: negative values for northward/colder areas
		    parameter[0].tempjuldiffort = (0.05886988/1000) * parameter[0].locationshift;
			parameter[0].precdiffort = (0.00842138/1000) * parameter[0].locationshift;
			// transect weather variation
			parameter[0].tempjandiffortmin = (-0.133395946/1000) * -1.0 * treerows;
			parameter[0].tempjuldiffortmin = (0.05886988/1000) * -1.0 * treerows;
			parameter[0].precdiffortmin = (0.00842138/1000) * -1.0 * treerows;
         } else if (parameter[0].weatherchoice == 2100845) {
            char tempbuf[] = "input/trans_Kolyma_shift00_temp_499-2300_rcp45decreasing.csv";
            char precbuf[] = "input/trans_Kolyma_shift00_prec_499-2300_rcp45decreasing.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
			// baseline weather manipulation
		    parameter[0].tempjandiffort = (0.001623102/1000) * parameter[0].locationshift;// in m: negative values for northward/colder areas
		    parameter[0].tempjuldiffort = (0.12172445/1000) * parameter[0].locationshift;
			parameter[0].precdiffort = (0.21769910/1000) * parameter[0].locationshift;
			// transect weather variation
			parameter[0].tempjandiffortmin = (0.001623102/1000) * -1.0 * treerows;
			parameter[0].tempjuldiffortmin = (0.12172445/1000) * -1.0 * treerows;
			parameter[0].precdiffortmin = (0.21769910/1000) * -1.0 * treerows;
        } else if (parameter[0].weatherchoice == 2100855) {
            char tempbuf[] = "input/trans_Chukotka_shift00_temp_499-2300_rcp45decreasing.csv";
            char precbuf[] = "input/trans_Chukotka_shift00_prec_499-2300_rcp45decreasing.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
			// baseline weather manipulation
		    parameter[0].tempjandiffort = (0.128972760/1000) * parameter[0].locationshift;// in m: negative values for northward/colder areas
		    parameter[0].tempjuldiffort = (0.05026903/1000) * parameter[0].locationshift;
			parameter[0].precdiffort = (-0.27962335/1000) * parameter[0].locationshift;
			// transect weather variation
			parameter[0].tempjandiffortmin = (0.128972760/1000) * -1.0 * treerows;
			parameter[0].tempjuldiffortmin = (0.05026903/1000) * -1.0 * treerows;
			parameter[0].precdiffortmin = (-0.27962335/1000) * -1.0 * treerows;
			
		} else if (parameter[0].weatherchoice == 2100812) {//499:2300 so 1800 years long - january july temp for right representation of transects +++ last number gridcell for RCP8.5
		// decreasing scenario while 412 is stabilized ones
            char tempbuf[] = "input/trans_Polar Urals_shift0N_temp_499-2300_rcp45decreasing.csv";
            char precbuf[] = "input/trans_Polar Urals_shift0N_prec_499-2300_rcp45decreasing.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
			// baseline weather manipulation
		    parameter[0].tempjandiffort = (0.049983234/1000) * parameter[0].locationshift;// in m: negative values for northward/colder areas
		    parameter[0].tempjuldiffort = (0.08816501/1000) * parameter[0].locationshift;
			parameter[0].precdiffort = (0.31567919/1000) * parameter[0].locationshift;
			// transect weather variation
			parameter[0].tempjandiffortmin = (0.049983234/1000) * -1.0 * treerows;
			parameter[0].tempjuldiffortmin = (0.08816501/1000) * -1.0 * treerows;
			parameter[0].precdiffortmin = (0.31567919/1000) * -1.0 * treerows;			
        } else if (parameter[0].weatherchoice == 2100822) {
            char tempbuf[] = "input/trans_Taimyr Peninsula_shift0N_temp_499-2300_rcp45decreasing.csv";
            char precbuf[] = "input/trans_Taimyr Peninsula_shift0N_prec_499-2300_rcp45decreasing.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
			// baseline weather manipulation
		    parameter[0].tempjandiffort = (0.001262349/1000) * parameter[0].locationshift;// in m: negative values for northward/colder areas
		    parameter[0].tempjuldiffort = (0.05785078/1000) * parameter[0].locationshift;
			parameter[0].precdiffort = (0.11536748/1000) * parameter[0].locationshift;
			// transect weather variation
			parameter[0].tempjandiffortmin = (0.001262349/1000) * -1.0 * treerows;
			parameter[0].tempjuldiffortmin = (0.05785078/1000) * -1.0 * treerows;
			parameter[0].precdiffortmin = (0.11536748/1000) * -1.0 * treerows;
        } else if (parameter[0].weatherchoice == 2100832) {
            char tempbuf[] = "input/trans_Buor Khaya_shift0N_temp_499-2300_rcp45decreasing.csv";
            char precbuf[] = "input/trans_Buor Khaya_shift0N_prec_499-2300_rcp45decreasing.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
			// baseline weather manipulation
		    parameter[0].tempjandiffort = (-0.133395946/1000) * parameter[0].locationshift;// in m: negative values for northward/colder areas
		    parameter[0].tempjuldiffort = (0.05886988/1000) * parameter[0].locationshift;
			parameter[0].precdiffort = (0.00842138/1000) * parameter[0].locationshift;
			// transect weather variation
			parameter[0].tempjandiffortmin = (-0.133395946/1000) * -1.0 * treerows;
			parameter[0].tempjuldiffortmin = (0.05886988/1000) * -1.0 * treerows;
			parameter[0].precdiffortmin = (0.00842138/1000) * -1.0 * treerows;
         } else if (parameter[0].weatherchoice == 2100842) {
            char tempbuf[] = "input/trans_Kolyma_shift0N_temp_499-2300_rcp45decreasing.csv";
            char precbuf[] = "input/trans_Kolyma_shift0N_prec_499-2300_rcp45decreasing.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
			// baseline weather manipulation
		    parameter[0].tempjandiffort = (0.001623102/1000) * parameter[0].locationshift;// in m: negative values for northward/colder areas
		    parameter[0].tempjuldiffort = (0.12172445/1000) * parameter[0].locationshift;
			parameter[0].precdiffort = (0.21769910/1000) * parameter[0].locationshift;
			// transect weather variation
			parameter[0].tempjandiffortmin = (0.001623102/1000) * -1.0 * treerows;
			parameter[0].tempjuldiffortmin = (0.12172445/1000) * -1.0 * treerows;
			parameter[0].precdiffortmin = (0.21769910/1000) * -1.0 * treerows;
        } else if (parameter[0].weatherchoice == 2100852) {
            char tempbuf[] = "input/trans_Chukotka_shift0N_temp_499-2300_rcp45decreasing.csv";
            char precbuf[] = "input/trans_Chukotka_shift0N_prec_499-2300_rcp45decreasing.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
			// baseline weather manipulation
		    parameter[0].tempjandiffort = (0.128972760/1000) * parameter[0].locationshift;// in m: negative values for northward/colder areas
		    parameter[0].tempjuldiffort = (0.05026903/1000) * parameter[0].locationshift;
			parameter[0].precdiffort = (-0.27962335/1000) * parameter[0].locationshift;
			// transect weather variation
			parameter[0].tempjandiffortmin = (0.128972760/1000) * -1.0 * treerows;
			parameter[0].tempjuldiffortmin = (0.05026903/1000) * -1.0 * treerows;
			parameter[0].precdiffortmin = (-0.27962335/1000) * -1.0 * treerows;

// CRU based climate gradient 
// ... for Chukotka it is diff between 00 and NE gridcell for the other 00 and N0
        } else if (parameter[0].weatherchoice == 21007121) {// last number added for CRU-Gridcell differences
            char tempbuf[] = "input/trans_Polar Urals_shift0N_temp_499-2300_rcp85decreasing.csv";
            char precbuf[] = "input/trans_Polar Urals_shift0N_prec_499-2300_rcp85decreasing.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
			// baseline weather manipulation
		    parameter[0].tempjandiffort = (-0.03432693/1000) * parameter[0].locationshift;// in m: negative values for northward/colder areas
		    parameter[0].tempjuldiffort = (-0.04108829/1000) * parameter[0].locationshift;
			parameter[0].precdiffort = (-0.1177749/1000) * parameter[0].locationshift;
			// transect weather variation
			parameter[0].tempjandiffortmin = (-0.03432693/1000) * treerows;
			parameter[0].tempjuldiffortmin = (-0.04108829/1000) * treerows;
			parameter[0].precdiffortmin = (-0.1177749/1000) * treerows;
        } else if (parameter[0].weatherchoice == 21007151) {// last number added
            char tempbuf[] = "input/trans_Polar Urals_shift00_temp_499-2300_rcp85decreasing.csv";
            char precbuf[] = "input/trans_Polar Urals_shift00_prec_499-2300_rcp85decreasing.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
			// baseline weather manipulation
		    parameter[0].tempjandiffort = (-0.03432693/1000) * parameter[0].locationshift;// in m: negative values for northward/colder areas
		    parameter[0].tempjuldiffort = (-0.04108829/1000) * parameter[0].locationshift;
			parameter[0].precdiffort = (-0.1177749/1000) * parameter[0].locationshift;
			// transect weather variation
			parameter[0].tempjandiffortmin = (-0.03432693/1000) * treerows;
			parameter[0].tempjuldiffortmin = (-0.04108829/1000) * treerows;
			parameter[0].precdiffortmin = (-0.1177749/1000) * treerows;
	
        } else if (parameter[0].weatherchoice == 21007221) {// last number added for CRU-Gridcell differences
            char tempbuf[] = "input/trans_Taimyr Peninsula_shift0N_temp_499-2300_rcp85decreasing.csv";
            char precbuf[] = "input/trans_Taimyr Peninsula_shift0N_prec_499-2300_rcp85decreasing.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
			// baseline weather manipulation
		    parameter[0].tempjandiffort = (-0.007512627/1000) * parameter[0].locationshift;// in m: negative values for northward/colder areas
		    parameter[0].tempjuldiffort = (-0.04108829/1000) * parameter[0].locationshift;
			parameter[0].precdiffort = (-0.1177749/1000) * parameter[0].locationshift;
			// transect weather variation
			parameter[0].tempjandiffortmin = (-0.007512627/1000) * treerows;
			parameter[0].tempjuldiffortmin = (-0.04108829/1000) * treerows;
			parameter[0].precdiffortmin = (-0.1177749/1000) * treerows;
        } else if (parameter[0].weatherchoice == 21007251) {// last number added
            char tempbuf[] = "input/trans_Taimyr Peninsula_shift00_temp_499-2300_rcp85decreasing.csv";
            char precbuf[] = "input/trans_Taimyr Peninsula_shift00_prec_499-2300_rcp85decreasing.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
			// baseline weather manipulation
		    parameter[0].tempjandiffort = (-0.007512627/1000) * parameter[0].locationshift;// in m: negative values for northward/colder areas
		    parameter[0].tempjuldiffort = (-0.005374418/1000) * parameter[0].locationshift;
			parameter[0].precdiffort = (-0.03461587/1000) * parameter[0].locationshift;
			// transect weather variation
			parameter[0].tempjandiffortmin = (-0.007512627/1000) * treerows;
			parameter[0].tempjuldiffortmin = (-0.005374418/1000) * treerows;
			parameter[0].precdiffortmin = (-0.03461587/1000) * treerows;
	
        } else if (parameter[0].weatherchoice == 21007321) {// last number added for CRU-Gridcell differences
            char tempbuf[] = "input/trans_Buor Khaya_shift0N_temp_499-2300_rcp85decreasing.csv";
            char precbuf[] = "input/trans_Buor Khaya_shift0N_prec_499-2300_rcp85decreasing.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
			// baseline weather manipulation
		    parameter[0].tempjandiffort = (-0.0003467366/1000) * parameter[0].locationshift;// in m: negative values for northward/colder areas
		    parameter[0].tempjuldiffort = (-0.0037563135/1000) * parameter[0].locationshift;
			parameter[0].precdiffort = (-0.03461587/1000) * parameter[0].locationshift;
			// transect weather variation
			parameter[0].tempjandiffortmin = (-0.0003467366/1000) * treerows;
			parameter[0].tempjuldiffortmin = (-0.0037563135/1000) * treerows;
			parameter[0].precdiffortmin = (-0.03461587/1000) * treerows;
        } else if (parameter[0].weatherchoice == 21007351) {// last number added
            char tempbuf[] = "input/trans_Buor Khaya_shift00_temp_499-2300_rcp85decreasing.csv";
            char precbuf[] = "input/trans_Buor Khaya_shift00_prec_499-2300_rcp85decreasing.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
			// baseline weather manipulation
		    parameter[0].tempjandiffort = (-0.0003467366/1000) * parameter[0].locationshift;// in m: negative values for northward/colder areas
		    parameter[0].tempjuldiffort = (-0.0037563135/1000) * parameter[0].locationshift;
			parameter[0].precdiffort = (-0.03461587/1000) * parameter[0].locationshift;
			// transect weather variation
			parameter[0].tempjandiffortmin = (-0.0003467366/1000) * treerows;
			parameter[0].tempjuldiffortmin = (-0.0037563135/1000) * treerows;
			parameter[0].precdiffortmin = (-0.03461587/1000) * treerows;
	
        } else if (parameter[0].weatherchoice == 21007421) {// last number added for CRU-Gridcell differences
            char tempbuf[] = "input/trans_Kolyma_shift0N_temp_499-2300_rcp85decreasing.csv";
            char precbuf[] = "input/trans_Kolyma_shift0N_prec_499-2300_rcp85decreasing.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
			// baseline weather manipulation
		    parameter[0].tempjandiffort = (-0.008957363/1000) * parameter[0].locationshift;// in m: negative values for northward/colder areas
		    parameter[0].tempjuldiffort = (-0.003582945/1000) * parameter[0].locationshift;
			parameter[0].precdiffort = (-0.2445649/1000) * parameter[0].locationshift;
			// transect weather variation
			parameter[0].tempjandiffortmin = (-0.008957363/1000) * treerows;
			parameter[0].tempjuldiffortmin = (-0.003582945/1000) * treerows;
			parameter[0].precdiffortmin = (-0.2445649/1000) * treerows;
        } else if (parameter[0].weatherchoice == 21007451) {// last number added
            char tempbuf[] = "input/trans_Kolyma_shift00_temp_499-2300_rcp85decreasing.csv";
            char precbuf[] = "input/trans_Kolyma_shift00_prec_499-2300_rcp85decreasing.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
			// baseline weather manipulation
		    parameter[0].tempjandiffort = (-0.008957363/1000) * parameter[0].locationshift;// in m: negative values for northward/colder areas
		    parameter[0].tempjuldiffort = (-0.003582945/1000) * parameter[0].locationshift;
			parameter[0].precdiffort = (-0.2445649/1000) * parameter[0].locationshift;
			// transect weather variation
			parameter[0].tempjandiffortmin = (-0.008957363/1000) * treerows;
			parameter[0].tempjuldiffortmin = (-0.003582945/1000) * treerows;
			parameter[0].precdiffortmin = (-0.2445649/1000) * treerows;
	
        } else if (parameter[0].weatherchoice == 21007521) {// last number added for CRU-Gridcell differences
            char tempbuf[] = "input/trans_Chukotka_shift0N_temp_499-2300_rcp85decreasing.csv";
            char precbuf[] = "input/trans_Chukotka_shift0N_prec_499-2300_rcp85decreasing.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
			// baseline weather manipulation
		    parameter[0].tempjandiffort = (-0.03304366/1000) * parameter[0].locationshift;// in m: negative values for northward/colder areas
		    parameter[0].tempjuldiffort = (-0.03185582/1000) * parameter[0].locationshift;
			parameter[0].precdiffort = (-0.06041807/1000) * parameter[0].locationshift;
			// transect weather variation
			parameter[0].tempjandiffortmin = (-0.03304366/1000) * treerows;
			parameter[0].tempjuldiffortmin = (-0.03185582/1000) * treerows;
			parameter[0].precdiffortmin = (-0.06041807/1000) * treerows;
        } else if (parameter[0].weatherchoice == 21007551) {// last number added
            char tempbuf[] = "input/trans_Chukotka_shift00_temp_499-2300_rcp85decreasing.csv";
            char precbuf[] = "input/trans_Chukotka_shift00_prec_499-2300_rcp85decreasing.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
			// baseline weather manipulation
		    parameter[0].tempjandiffort = (-0.03304366/1000) * parameter[0].locationshift;// in m: negative values for northward/colder areas
		    parameter[0].tempjuldiffort = (-0.03185582/1000) * parameter[0].locationshift;
			parameter[0].precdiffort = (-0.06041807/1000) * parameter[0].locationshift;
			// transect weather variation
			parameter[0].tempjandiffortmin = (-0.03304366/1000) * treerows;
			parameter[0].tempjuldiffortmin = (-0.03185582/1000) * treerows;
			parameter[0].precdiffortmin = (-0.06041807/1000) * treerows;
	

		
        }
        getTemp3(aktort, dateinametemp, weather_list);
        getPrec1(dateinameprec, weather_list, stringlengthmax);

        parameter[0].parameterinputvis = true;
    }

    passWeather();
}
