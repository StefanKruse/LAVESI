#include "LAVESI.h"

using namespace std;

// TODO temporary here
extern vector<vector<Weather>> world_weather_list;
extern vector<double> wdir, wspd;
extern vector<vector<double>> windspd;
extern vector<vector<double>> winddir;
extern vector<int> globalyears;

void getPrec(char dateinameprec[250], vector<Weather>& weather_list, int maximal_word_length) {
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

            precipitationsumbuf = (prec1monthmeanbuf + prec2monthmeanbuf + prec3monthmeanbuf + prec4monthmeanbuf + prec5monthmeanbuf + prec6monthmeanbuf
                                   + prec7monthmeanbuf + prec8monthmeanbuf + prec9monthmeanbuf + prec10monthmeanbuf + prec11monthmeanbuf + prec12monthmeanbuf);

            auto& pWeather = weather_list[counter - 2];

            pWeather.prec1monthmean = prec1monthmeanbuf + parameter[0].precdiffort / 12;
            pWeather.prec2monthmean = prec2monthmeanbuf + parameter[0].precdiffort / 12;
            pWeather.prec3monthmean = prec3monthmeanbuf + parameter[0].precdiffort / 12;
            pWeather.prec4monthmean = prec4monthmeanbuf + parameter[0].precdiffort / 12;
            pWeather.prec5monthmean = prec5monthmeanbuf + parameter[0].precdiffort / 12;
            pWeather.prec6monthmean = prec6monthmeanbuf + parameter[0].precdiffort / 12;
            pWeather.prec7monthmean = prec7monthmeanbuf + parameter[0].precdiffort / 12;
            pWeather.prec8monthmean = prec8monthmeanbuf + parameter[0].precdiffort / 12;
            pWeather.prec9monthmean = prec9monthmeanbuf + parameter[0].precdiffort / 12;
            pWeather.prec10monthmean = prec10monthmeanbuf + parameter[0].precdiffort / 12;
            pWeather.prec11monthmean = prec11monthmeanbuf + parameter[0].precdiffort / 12;
            pWeather.prec12monthmean = prec12monthmeanbuf + parameter[0].precdiffort / 12;

            pWeather.prec1monthmeanmin = prec1monthmeanbuf + parameter[0].precdiffortmin / 12;
            pWeather.prec2monthmeanmin = prec2monthmeanbuf + parameter[0].precdiffortmin / 12;
            pWeather.prec3monthmeanmin = prec3monthmeanbuf + parameter[0].precdiffortmin / 12;
            pWeather.prec4monthmeanmin = prec4monthmeanbuf + parameter[0].precdiffortmin / 12;
            pWeather.prec5monthmeanmin = prec5monthmeanbuf + parameter[0].precdiffortmin / 12;
            pWeather.prec6monthmeanmin = prec6monthmeanbuf + parameter[0].precdiffortmin / 12;
            pWeather.prec7monthmeanmin = prec7monthmeanbuf + parameter[0].precdiffortmin / 12;
            pWeather.prec8monthmeanmin = prec8monthmeanbuf + parameter[0].precdiffortmin / 12;
            pWeather.prec9monthmeanmin = prec9monthmeanbuf + parameter[0].precdiffortmin / 12;
            pWeather.prec10monthmeanmin = prec10monthmeanbuf + parameter[0].precdiffortmin / 12;
            pWeather.prec11monthmeanmin = prec11monthmeanbuf + parameter[0].precdiffortmin / 12;
            pWeather.prec12monthmeanmin = prec12monthmeanbuf + parameter[0].precdiffortmin / 12;

            pWeather.precipitationsum = precipitationsumbuf + parameter[0].precdiffort;
            pWeather.precipitationsummin = precipitationsumbuf + parameter[0].precdiffortmin;
        }
        counter++;
    }

    fclose(fp);
}

void getTemp(	//int aktort, 
				char dateinametemp[250], 
				vector<Weather>& weather_list) {
    // TODO: check whether this grid-based simulation setup is still necessary or can be removed
    // int aktortyworldcoo = (double)(aktort - 1) / parameter[0].mapxlength;
    // int aktortxworldcoo = (aktort - 1) - (aktortyworldcoo * parameter[0].mapxlength);

    // if (parameter[0].mapylength > 1 && parameter[0].weathercalcgradient == true) {
    // double Nposcenter = (parameter[0].nposmax + parameter[0].nposmin) / 2;
    // double mapylengthdummy = parameter[0].mapylength;
    // double Nposakt = parameter[0].nposmax - ((parameter[0].nposmax - parameter[0].nposmin) * aktortyworldcoo / (mapylengthdummy - 1.0));

    // parameter[0].precdiffort = -5.3699 * (Nposakt - Nposcenter);
    // parameter[0].tempdiffort = -0.3508 * (Nposakt - Nposcenter);
    // }

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

            Weather pWeather;

            // pWeather.yworldcoo = aktortyworldcoo;
            // pWeather.xworldcoo = aktortxworldcoo;
            pWeather.jahr = counter + parameter[0].startjahr - 2;
            pWeather.tempyearmean = tempyearmeanbuf + parameter[0].tempdiffort;
            if (parameter[0].tempjandiffort != 0.0)
                pWeather.temp1monthmean = temp1monthmeanbuf + parameter[0].tempjandiffort;
            else
                pWeather.temp1monthmean = temp1monthmeanbuf + parameter[0].tempdiffort;
            if (parameter[0].tempjandiffortmin != 0.0)
                pWeather.temp1monthmeanmin = temp1monthmeanbuf + parameter[0].tempjandiffortmin;
            else
                pWeather.temp1monthmeanmin = temp1monthmeanbuf + parameter[0].tempdiffortmin;
            pWeather.temp2monthmean = temp2monthmeanbuf + parameter[0].tempdiffort;
            pWeather.temp3monthmean = temp3monthmeanbuf + parameter[0].tempdiffort;
            pWeather.temp4monthmean = temp4monthmeanbuf + parameter[0].tempdiffort;
            pWeather.temp5monthmean = temp5monthmeanbuf + parameter[0].tempdiffort;
            pWeather.temp6monthmean = temp6monthmeanbuf + parameter[0].tempdiffort;
            if (parameter[0].tempjuldiffort != 0.0)
                pWeather.temp7monthmean = temp7monthmeanbuf + parameter[0].tempjuldiffort;
            else
                pWeather.temp7monthmean = temp7monthmeanbuf + parameter[0].tempdiffort;
            if (parameter[0].tempjuldiffortmin != 0.0)
                pWeather.temp7monthmeanmin = temp7monthmeanbuf + parameter[0].tempjuldiffortmin;
            else
                pWeather.temp7monthmeanmin = temp7monthmeanbuf + parameter[0].tempdiffortmin;
            pWeather.temp8monthmean = temp8monthmeanbuf + parameter[0].tempdiffort;
            pWeather.temp9monthmean = temp9monthmeanbuf + parameter[0].tempdiffort;
            pWeather.temp10monthmean = temp10monthmeanbuf + parameter[0].tempdiffort;
            pWeather.temp11monthmean = temp11monthmeanbuf + parameter[0].tempdiffort;
            pWeather.temp12monthmean = temp12monthmeanbuf + parameter[0].tempdiffort;

            double sumacttemp = 0;
            double sumacttempmin = 0;
            double sumdegreday = 0;
            double sumdegredaymin = 0;
            int ndegreday = 0;
            int ndegredaymin = 0;
            double jantemp = pWeather.temp1monthmean;
            double jantempmin = pWeather.temp1monthmeanmin;
            double julitemp = pWeather.temp7monthmean;
            double julitempmin = pWeather.temp7monthmeanmin;

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
                if ((i >= 1) && (i <= 15)) {
                    weight = ((double)1.0 * ((15.0 - i) / 184.0));
                }
                if ((i >= 16) && (i <= 196)) {
                    weight = ((double)1.0 * ((i - 15.0) / 181.0));
                }
                if ((i >= 197) && (i <= 365)) {
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

            pWeather.activeairtemp = sumacttemp * 0.84630;
            pWeather.activeairtempmin = sumacttempmin * 0.84630;
            pWeather.vegetationperiodlength = (int)round(ndegreday * 0.88432);
            pWeather.vegetationperiodlengthmin = (int)round(ndegredaymin * 0.88432);
            pWeather.degreday = sumdegreday * 0.87583;
            pWeather.degredaymin = sumdegredaymin * 0.87583;

            weather_list.emplace_back(std::move(pWeather));
        }
        counter++;
    }
    fclose(f);
}

void passWeather() {
    for (vector<vector<Weather>>::iterator posw = world_weather_list.begin(); posw != world_weather_list.end(); posw++) {
        vector<Weather>& weather_list = *posw;

        for (unsigned int iweather = 0; iweather < weather_list.size(); ++iweather) {
            double jantempsum = 0;
            double jantempsummin = 0;
            double jultempsum = 0;
            double jultempsummin = 0;
            double nddsum = 0;
            double nddsummin = 0;

            if (iweather > 9) {
                for (int i = 1; i < (10 + 1); ++i) {
                    jantempsum += weather_list[iweather + i - 10].temp1monthmean;
                    jantempsummin += weather_list[iweather + i - 10].temp1monthmeanmin;
                    jultempsum += weather_list[iweather + i - 10].temp7monthmean;
                    jultempsummin += weather_list[iweather + i - 10].temp7monthmeanmin;
                    nddsum += weather_list[iweather + i - 10].vegetationperiodlength;
                    nddsummin += weather_list[iweather + i - 10].vegetationperiodlengthmin;
                }
                jantempsum = jantempsum / 10;
                jantempsummin = jantempsummin / 10;
                jultempsum = jultempsum / 10;
                jultempsummin = jultempsummin / 10;
                nddsum = nddsum / 10;
                nddsummin = nddsummin / 10;
            } else if (iweather > 0 && iweather <= 9) {
                for (unsigned int i = 0; i < (iweather + 1); ++i) {
                    jantempsum += weather_list[i].temp1monthmean;
                    jantempsummin += weather_list[i].temp1monthmeanmin;
                    jultempsum += weather_list[i].temp7monthmean;
                    jultempsummin += weather_list[i].temp7monthmeanmin;
                    nddsum += weather_list[i].vegetationperiodlength;
                    nddsummin += weather_list[i].vegetationperiodlengthmin;
                }
                jantempsum = jantempsum / (iweather + 1);
                jantempsummin = jantempsummin / (iweather + 1);
                jultempsum = jultempsum / (iweather + 1);
                jultempsummin = jultempsummin / (iweather + 1);
                nddsum = nddsum / (iweather + 1);
                nddsummin = nddsummin / (iweather + 1);
            } else {
                jantempsum = weather_list[iweather].temp1monthmean;
                jantempsummin = weather_list[iweather].temp1monthmeanmin;
                jultempsum = weather_list[iweather].temp7monthmean;
                jultempsummin = weather_list[iweather].temp7monthmeanmin;
                nddsum = weather_list[iweather].vegetationperiodlength;
                nddsummin = weather_list[iweather].vegetationperiodlengthmin;
            }
            weather_list[iweather].temp1monthmeaniso = jantempsum;
            weather_list[iweather].temp1monthmeanisomin = jantempsummin;
            weather_list[iweather].temp7monthmeaniso = jultempsum;
            weather_list[iweather].temp7monthmeanisomin = jultempsummin;
            weather_list[iweather].vegetationperiodlengthiso = nddsum;
            weather_list[iweather].vegetationperiodlengthisomin = nddsummin;

            // calculation of a drought influence
            double droughtmortbuf = 0.0;
            double droughtmortbufmin = 0.0;
            double precgs = weather_list[iweather].prec5monthmean + weather_list[iweather].prec6monthmean + weather_list[iweather].prec7monthmean
                            + weather_list[iweather].prec8monthmean;
            double precgsmin = weather_list[iweather].prec5monthmeanmin + weather_list[iweather].prec6monthmeanmin + weather_list[iweather].prec7monthmeanmin
                               + weather_list[iweather].prec8monthmeanmin;
            double duerreindex = parameter[0].evapod * weather_list[iweather].vegetationperiodlength / precgs;
            double duerreindexmin = parameter[0].evapod * weather_list[iweather].vegetationperiodlengthmin / precgsmin;
            if (duerreindex > 1) {
                double aattrockenheit = (weather_list[iweather].activeairtemp / 300.0) - 1.0;

                if (aattrockenheit > 1) {
                    droughtmortbuf = droughtmortbuf + aattrockenheit * 0.1;
                }
            }
            weather_list[iweather].droughtmort = droughtmortbuf;
            if (duerreindexmin > 1) {
                double aattrockenheitmin = (weather_list[iweather].activeairtempmin / 300.0) - 1.0;

                if (aattrockenheitmin > 1) {
                    droughtmortbufmin = droughtmortbufmin + aattrockenheitmin * 0.1;
                }
            }
            weather_list[iweather].droughtmortmin = droughtmortbufmin;

            // calculate restrictions
            weather_list[iweather].julisothermrestriktion = 0.0;
            weather_list[iweather].julisothermrestriktionmin = 0.0;
            weather_list[iweather].nddrestriktion = 0.0;
            weather_list[iweather].nddrestriktionmin = 0.0;

			// multiple species integration
			weather_list[iweather].weatherfactor.resize(99,0);
			weather_list[iweather].weatherfactormin.resize(99,0);
			weather_list[iweather].janisothermrestriktion.resize(99,0);
			weather_list[iweather].janisothermrestriktionmin.resize(99,0);
			for (int species_counter = 1; species_counter < 99; species_counter++) {
				if(speciestrait[species_counter].number == 0)
					break;
				
				// for growth
				weather_list[iweather].weatherfactor[species_counter]=(double) (((speciestrait[species_counter].weathervariablea/(1+exp(speciestrait[species_counter].weathervariableb-weather_list[iweather].temp7monthmean)))+speciestrait[species_counter].weathervariablec)-speciestrait[species_counter].weathervariablec)/(speciestrait[species_counter].weathervariabled-speciestrait[species_counter].weathervariablec);
				weather_list[iweather].weatherfactormin[species_counter]=(double) (((speciestrait[species_counter].weathervariablea/(1+exp(speciestrait[species_counter].weathervariableb-weather_list[iweather].temp7monthmeanmin)))+speciestrait[species_counter].weathervariablec)-speciestrait[species_counter].weathervariablec)/(speciestrait[species_counter].weathervariabled-speciestrait[species_counter].weathervariablec);
				
				// restrictions
				if (weather_list[iweather].temp1monthmeaniso < (speciestrait[species_counter].janthresholdtemp)) {
					weather_list[iweather].janisothermrestriktion[species_counter]=1.0;
				} else {
					weather_list[iweather].janisothermrestriktion[species_counter]=1.0-fabs(speciestrait[species_counter].janthresholdtempcalcvalue*(weather_list[iweather].temp1monthmeaniso-speciestrait[species_counter].janthresholdtemp)/(-speciestrait[species_counter].janthresholdtemp));
				}
				if (weather_list[iweather].temp1monthmeanisomin < (speciestrait[species_counter].janthresholdtemp)) {
					weather_list[iweather].janisothermrestriktionmin[species_counter]=1.0;
				} else {
					weather_list[iweather].janisothermrestriktionmin[species_counter]=1.0-fabs(speciestrait[species_counter].janthresholdtempcalcvalue*(weather_list[iweather].temp1monthmeanisomin-speciestrait[species_counter].janthresholdtemp)/(-speciestrait[species_counter].janthresholdtemp));
				}
				
				// output for a quick check
				// cout << speciestrait[species_counter].number << "::" << speciestrait[species_counter].species << " => ";
				// cout << " ; wfac: " << weather_list[iweather].weatherfactor[species_counter];
				// cout << " ; wfacmin: " << weather_list[iweather].weatherfactormin[species_counter];
				// cout << " ; janthresh: " << weather_list[iweather].janisothermrestriktion[species_counter] << endl;
				// cout << " ; janthreshmin: " << weather_list[iweather].janisothermrestriktionmin[species_counter] << endl;
			}

            // July temp for both
            if (weather_list[iweather].temp7monthmeaniso < 10.0) {
                weather_list[iweather].julisothermrestriktion = 1.0;
            } else {
                weather_list[iweather].julisothermrestriktion = 1.0 - fabs(2.0 * (weather_list[iweather].temp7monthmeaniso - 10.0) / 10.0);
            }
            if (weather_list[iweather].temp7monthmeanisomin < 10.0) {
                weather_list[iweather].julisothermrestriktionmin = 1.0;
            } else {
                weather_list[iweather].julisothermrestriktionmin = 1.0 - fabs(2.0 * (weather_list[iweather].temp7monthmeanisomin - 10.0) / 10.0);
            }

            // ndd for both
            if (weather_list[iweather].vegetationperiodlengthiso < 60.0) {
                weather_list[iweather].nddrestriktion = 1.0;
            } else {
                weather_list[iweather].nddrestriktion = 1.0 - fabs((((double)weather_list[iweather].vegetationperiodlengthiso) - 60.0) / 60.0);
            }
            if (weather_list[iweather].vegetationperiodlengthisomin < 60.0) {
                weather_list[iweather].nddrestriktionmin = 1.0;
            } else {
                weather_list[iweather].nddrestriktionmin = 1.0 - fabs((((double)weather_list[iweather].vegetationperiodlengthisomin) - 60.0) / 60.0);
            }

			// Fire index calculation for each month (for Lake Satagay)
			double T1 = weather_list[iweather].temp1monthmean;
			double P1 = weather_list[iweather].prec1monthmean;
			weather_list[iweather].fireindex1 = 0.11816*(((T1 + 12.03) * 0.7213351) + ((P1 - 39.14) * 0.6925862)) + (-0.12372*(((T1 + 12.03) * -0.6925862) + ((P1 - 39.14) * 0.7213351)))+1.46818;
			
			double T2 = weather_list[iweather].temp2monthmean;
			double P2 = weather_list[iweather].prec2monthmean;
			weather_list[iweather].fireindex2 = 0.11816*(((T2 + 12.03) * 0.7213351) + ((P2 - 39.14) * 0.6925862)) + (-0.12372*(((T2 + 12.03) * -0.6925862) + ((P2 - 39.14) * 0.7213351)))+1.46818;
			
			double T3 = weather_list[iweather].temp3monthmean;
			double P3 = weather_list[iweather].prec3monthmean;
			weather_list[iweather].fireindex3 = 0.11816*(((T3 + 12.03) * 0.7213351) + ((P3 - 39.14) * 0.6925862)) + (-0.12372*(((T3 + 12.03) * -0.6925862) + ((P3 - 39.14) * 0.7213351)))+1.46818;
			
			double T4 = weather_list[iweather].temp4monthmean;
			double P4 = weather_list[iweather].prec4monthmean;
			weather_list[iweather].fireindex4 = 0.11816*(((T4 + 12.03) * 0.7213351) + ((P4 - 39.14) * 0.6925862)) + (-0.12372*(((T4 + 12.03) * -0.6925862) + ((P4 - 39.14) * 0.7213351)))+1.46818;
			
			double T5 = weather_list[iweather].temp5monthmean;
			double P5 = weather_list[iweather].prec5monthmean;
			weather_list[iweather].fireindex5 = 0.11816*(((T5 + 12.03) * 0.7213351) + ((P5 - 39.14) * 0.6925862)) + (-0.12372*(((T5 + 12.03) * -0.6925862) + ((P5 - 39.14) * 0.7213351)))+1.46818;
			
			double T6 = weather_list[iweather].temp6monthmean;
			double P6 = weather_list[iweather].prec6monthmean;
			weather_list[iweather].fireindex6 = 0.11816*(((T6 + 12.03) * 0.7213351) + ((P6 - 39.14) * 0.6925862)) + (-0.12372*(((T6 + 12.03) * -0.6925862) + ((P6 - 39.14) * 0.7213351)))+1.46818;
			
			double T7 = weather_list[iweather].temp7monthmean;
			double P7 = weather_list[iweather].prec7monthmean;
			weather_list[iweather].fireindex7 = 0.11816*(((T7 + 12.03) * 0.7213351) + ((P7 - 39.14) * 0.6925862)) + (-0.12372*(((T7 + 12.03) * -0.6925862) + ((P7 - 39.14) * 0.7213351)))+1.46818;
			
			double T8 = weather_list[iweather].temp8monthmean;
			double P8 = weather_list[iweather].prec8monthmean;
			weather_list[iweather].fireindex8 = 0.11816*(((T8 + 12.03) * 0.7213351) + ((P8 - 39.14) * 0.6925862)) + (-0.12372*(((T8 + 12.03) * -0.6925862) + ((P8 - 39.14) * 0.7213351)))+1.46818;
			
			double T9 = weather_list[iweather].temp9monthmean;
			double P9 = weather_list[iweather].prec9monthmean;
			weather_list[iweather].fireindex9 = 0.11816*(((T9 + 12.03) * 0.7213351) + ((P9 - 39.14) * 0.6925862)) + (-0.12372*(((T9 + 12.03) * -0.6925862) + ((P9 - 39.14) * 0.7213351)))+1.46818;
			
			double T10 = weather_list[iweather].temp10monthmean;
			double P10 = weather_list[iweather].prec10monthmean;
			weather_list[iweather].fireindex10 = 0.11816*(((T10 + 12.03) * 0.7213351) + ((P10 - 39.14) * 0.6925862)) + (-0.12372*(((T10 + 12.03) * -0.6925862) + ((P10 - 39.14) * 0.7213351)))+1.46818;
			
			double T11 = weather_list[iweather].temp11monthmean;
			double P11 = weather_list[iweather].prec11monthmean;
			weather_list[iweather].fireindex11 = 0.11816*(((T11 + 12.03) * 0.7213351) + ((P11 - 39.14) * 0.6925862)) + (-0.12372*(((T11 + 12.03) * -0.6925862) + ((P11 - 39.14) * 0.7213351)))+1.46818;
			
			double T12 = weather_list[iweather].temp12monthmean;
			double P12 = weather_list[iweather].prec12monthmean;
			weather_list[iweather].fireindex12 = 0.11816*(((T12 + 12.03) * 0.7213351) + ((P12 - 39.14) * 0.6925862)) + (-0.12372*(((T12 + 12.03) * -0.6925862) + ((P12 - 39.14) * 0.7213351)))+1.46818;

			//Fire index calculation for each month (for Lake Khamra)
			// double T1 = weather_list[iweather].temp1monthmean;
			// double P1 = weather_list[iweather].prec1monthmean;
			// weather_list[iweather].fireindex1 = 0.10336*(((T1 + 6.19) * 0.6291112) + ((P1 - 34.99) * 0.7773153)) + (-0.17637*(((T1 + 6.19) * -0.7773153) + ((P1 - 34.99) * 0.6291112)))+0.69034;
			
			// double T2 = weather_list[iweather].temp2monthmean;
			// double P2 = weather_list[iweather].prec2monthmean;
			// weather_list[iweather].fireindex2 = 0.10336*(((T2 + 6.19) * 0.6291112) + ((P2 - 34.99) * 0.7773153)) + (-0.17637*(((T2 + 6.19) * -0.7773153) + ((P2 - 34.99) * 0.6291112)))+0.69034;
			
			// double T3 = weather_list[iweather].temp3monthmean;
			// double P3 = weather_list[iweather].prec3monthmean;
			// weather_list[iweather].fireindex3 = 0.10336*(((T3 + 6.19) * 0.6291112) + ((P3 - 34.99) * 0.7773153)) + (-0.17637*(((T3 + 6.19) * -0.7773153) + ((P3 - 34.99) * 0.6291112)))+0.69034;
			
			// double T4 = weather_list[iweather].temp4monthmean;
			// double P4 = weather_list[iweather].prec4monthmean;
			// weather_list[iweather].fireindex4 = 0.10336*(((T4 + 6.19) * 0.6291112) + ((P4 - 34.99) * 0.7773153)) + (-0.17637*(((T4 + 6.19) * -0.7773153) + ((P4 - 34.99) * 0.6291112)))+0.69034;
			
			// double T5 = weather_list[iweather].temp5monthmean;
			// double P5 = weather_list[iweather].prec5monthmean;
			// weather_list[iweather].fireindex5 = 0.10336*(((T5 + 6.19) * 0.6291112) + ((P5 - 34.99) * 0.7773153)) + (-0.17637*(((T5 + 6.19) * -0.7773153) + ((P5 - 34.99) * 0.6291112)))+0.69034;
			
			// double T6 = weather_list[iweather].temp6monthmean;
			// double P6 = weather_list[iweather].prec6monthmean;
			// weather_list[iweather].fireindex6 = 0.10336*(((T6 + 6.19) * 0.6291112) + ((P6 - 34.99) * 0.7773153)) + (-0.17637*(((T6 + 6.19) * -0.7773153) + ((P6 - 34.99) * 0.6291112)))+0.69034;
			
			// double T7 = weather_list[iweather].temp7monthmean;
			// double P7 = weather_list[iweather].prec7monthmean;
			// weather_list[iweather].fireindex7 = 0.10336*(((T7 + 6.19) * 0.6291112) + ((P7 - 34.99) * 0.7773153)) + (-0.17637*(((T7 + 6.19) * -0.7773153) + ((P7 - 34.99) * 0.6291112)))+0.69034;
			
			// double T8 = weather_list[iweather].temp8monthmean;
			// double P8 = weather_list[iweather].prec8monthmean;
			// weather_list[iweather].fireindex8 = 0.10336*(((T8 + 6.19) * 0.6291112) + ((P8 - 34.99) * 0.7773153)) + (-0.17637*(((T8 + 6.19) * -0.7773153) + ((P8 - 34.99) * 0.6291112)))+0.69034;
			
			// double T9 = weather_list[iweather].temp9monthmean;
			// double P9 = weather_list[iweather].prec9monthmean;
			// weather_list[iweather].fireindex9 = 0.10336*(((T9 + 6.19) * 0.6291112) + ((P9 - 34.99) * 0.7773153)) + (-0.17637*(((T9 + 6.19) * -0.7773153) + ((P9 - 34.99) * 0.6291112)))+0.69034;
			
			// double T10 = weather_list[iweather].temp10monthmean;
			// double P10 = weather_list[iweather].prec10monthmean;
			// weather_list[iweather].fireindex10 = 0.10336*(((T10 + 6.19) * 0.6291112) + ((P10 - 34.99) * 0.7773153)) + (-0.17637*(((T10 + 6.19) * -0.7773153) + ((P10 - 34.99) * 0.6291112)))+0.69034;
			
			// double T11 = weather_list[iweather].temp11monthmean;
			// double P11 = weather_list[iweather].prec11monthmean;
			// weather_list[iweather].fireindex11 = 0.10336*(((T11 + 6.19) * 0.6291112) + ((P11 - 34.99) * 0.7773153)) + (-0.17637*(((T11 + 6.19) * -0.7773153) + ((P11 - 34.99) * 0.6291112)))+0.69034;
			
			// double T12 = weather_list[iweather].temp12monthmean;
			// double P12 = weather_list[iweather].prec12monthmean;
			// weather_list[iweather].fireindex12 = 0.10336*(((T12 + 6.19) * 0.6291112) + ((P12 - 34.99) * 0.7773153)) + (-0.17637*(((T12 + 6.19) * -0.7773153) + ((P12 - 34.99) * 0.6291112)))+0.69034;
			
			// JJA temperature and precipitation for output
			weather_list[iweather].tempmeanjja = (weather_list[iweather].temp6monthmean + weather_list[iweather].temp7monthmean + weather_list[iweather].temp8monthmean) / 3;
			weather_list[iweather].precipitationsumjja = weather_list[iweather].prec6monthmean + weather_list[iweather].prec7monthmean + weather_list[iweather].prec8monthmean;
			
            // output to check weather
            FILE* fdir;
            char filenamechar[50];
            sprintf(filenamechar, "data_weatherprocessingcheck");
            string output = "output/" + string(filenamechar) + ".csv";
            fdir = fopen(output.c_str(), "a+");

            fprintf(fdir,
                    "%4.4f \t %4.4f \t %4.4f \t %4.4f \t %4.4f \t %4.4f \t %4.4f \t %4.4f \t %4.4f \t %4.4f \t %4.4f \t %4.4f \t %4.4f \t %4.4f \t %4.4f \t "
                    "%4.4f \t %4.4f \t %4.4f \t %4.4f \t %4.4f \t %4.4f \t %4.4f \t \n",
                    weather_list[iweather].temp1monthmeaniso, weather_list[iweather].temp1monthmeanisomin, weather_list[iweather].temp7monthmeaniso,
                    weather_list[iweather].temp7monthmeanisomin, weather_list[iweather].droughtmort, weather_list[iweather].droughtmortmin,
                    (double)weather_list[iweather].vegetationperiodlengthiso, (double)weather_list[iweather].vegetationperiodlengthisomin, precgs, precgsmin,
                    weather_list[iweather].janisothermrestriktion[1], weather_list[iweather].janisothermrestriktionmin[1],
                    weather_list[iweather].janisothermrestriktion[2], weather_list[iweather].janisothermrestriktionmin[2],
                    weather_list[iweather].julisothermrestriktion, weather_list[iweather].julisothermrestriktionmin, weather_list[iweather].nddrestriktion,
                    weather_list[iweather].nddrestriktionmin, weather_list[iweather].weatherfactor[1], weather_list[iweather].weatherfactormin[1],
                    weather_list[iweather].weatherfactor[2], weather_list[iweather].weatherfactormin[2]);

            fclose(fdir);
			
            if (parameter[0].qualiyearlyvis == true) {
                printf("	weather(%d; iweather=%d): weatherfactorg=%4.4f, weatherfactors=%4.4f ===> \ndroughtmort=%4.5f\n", weather_list[iweather].jahr, iweather, weather_list[iweather].weatherfactor[1], weather_list[iweather].weatherfactor[2], weather_list[iweather].droughtmort);
                printf("\tJanT10=%4.2f, JuliT10=%4.2f, NDD10=%d\n", weather_list[iweather].temp1monthmeaniso, weather_list[iweather].temp7monthmeaniso, weather_list[iweather].vegetationperiodlengthiso);
            }
        }
    }
}

extern void Weatherinput(Parameter* parameter, int stringlengthmax, vector<vector<Weather>>& world_weather_list) {
    char dateinametemp[250];
    char dateinameprec[250];

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
        ostringstream foldername;
        std::ostringstream ss;
        string item;

        // define input folder
        if ((parameter[0].weatherchoice == 1000000283) /* ### FOR EN22-065 ### */ ||
		(parameter[0].weatherchoice == 1000000284)) /* ### FOR EN22-066 ### */ {
		foldername << "cell_1";
		} else if ((parameter[0].weatherchoice == 1000000285) /* ### FOR EN22-067 ### */ ||
		(parameter[0].weatherchoice == 1000000286) /* ### FOR EN22-068 ### */ ||
		(parameter[0].weatherchoice == 1000000288) /* ### FOR EN22-070 ### */ ||
		(parameter[0].weatherchoice == 1000000289) /* ### FOR EN22-071 ### */ ||
		(parameter[0].weatherchoice == 1000000290) /* ### FOR EN22-072 ### */ ||
		(parameter[0].weatherchoice == 1000000291) /* ### FOR EN22-073 ### */ ||
		(parameter[0].weatherchoice == 1000000292)) /* ### FOR EN22-074 ### */ {
		foldername << "cell_2";
		} else if ((parameter[0].weatherchoice == 1000000287)) /* ### FOR EN22-069 ### */ {
		foldername << "cell_3";
		} else if ((parameter[0].weatherchoice == 1000000219) /* ### FOR EN22-001 ### */ ||
		(parameter[0].weatherchoice == 1000000220) /* ### FOR EN22-002 ### */ ||
		(parameter[0].weatherchoice == 1000000221) /* ### FOR EN22-003 ### */ ||
		(parameter[0].weatherchoice == 1000000222) /* ### FOR EN22-004 ### */ ||
		(parameter[0].weatherchoice == 1000000274) /* ### FOR EN22-056 ### */ ||
		(parameter[0].weatherchoice == 1000000275) /* ### FOR EN22-057 ### */ ||
		(parameter[0].weatherchoice == 1000000276) /* ### FOR EN22-058 ### */ ||
		(parameter[0].weatherchoice == 1000000277) /* ### FOR EN22-059 ### */ ||
		(parameter[0].weatherchoice == 1000000278)) /* ### FOR EN22-060 ### */ {
		foldername << "cell_4";
		} else if ((parameter[0].weatherchoice == 1000000279) /* ### FOR EN22-061 ### */ ||
		(parameter[0].weatherchoice == 1000000280)) /* ### FOR EN22-062 ### */ {
		foldername << "cell_5";
		} else if ((parameter[0].weatherchoice == 1000000281) /* ### FOR EN22-063 ### */ ||
		(parameter[0].weatherchoice == 1000000282) /* ### FOR EN22-064 ### */ ||
		(parameter[0].weatherchoice == 1000000293)) /* ### FOR EN22-075 ### */ {
		foldername << "cell_6";
		} else if ((parameter[0].weatherchoice == 1000000148) /* ### FOR EN18075 ### */ ||
		(parameter[0].weatherchoice == 1000000149) /* ### FOR EN18076 ### */ ||
		(parameter[0].weatherchoice == 1000000150) /* ### FOR EN18077 ### */ ||
		(parameter[0].weatherchoice == 1000000151) /* ### FOR EN18078 ### */ ||
		(parameter[0].weatherchoice == 1000000152) /* ### FOR EN18079 ### */ ||
		(parameter[0].weatherchoice == 1000000153) /* ### FOR EN18080 ### */ ||
		(parameter[0].weatherchoice == 1000000154) /* ### FOR EN18081 ### */ ||
		(parameter[0].weatherchoice == 1000000155) /* ### FOR EN18082 ### */ ||
		(parameter[0].weatherchoice == 1000000156)) /* ### FOR EN18083 ### */ {
			cout << "usingWinddata!" << endl;
		foldername << "cell_7";
		} else if ((parameter[0].weatherchoice == 1000000144) /* ### FOR EN18071 ### */ ||
		(parameter[0].weatherchoice == 1000000145) /* ### FOR EN18072 ### */ ||
		(parameter[0].weatherchoice == 1000000146) /* ### FOR EN18073 ### */ ||
		(parameter[0].weatherchoice == 1000000147)) /* ### FOR EN18074 ### */ {
		foldername << "cell_8";
		} else if ((parameter[0].weatherchoice == 1000000132)) /* ### FOR EN18062 ### */ {
		foldername << "cell_9";
		} else if ((parameter[0].weatherchoice == 1000000131) /* ### FOR EN18061 ### */ ||
		(parameter[0].weatherchoice == 1000000176) /* ### FOR EN21220 ### */ ||
		(parameter[0].weatherchoice == 1000000177) /* ### FOR EN21221 ### */ ||
		(parameter[0].weatherchoice == 1000000178) /* ### FOR EN21222 ### */ ||
		(parameter[0].weatherchoice == 1000000179) /* ### FOR EN21223 ### */ ||
		(parameter[0].weatherchoice == 1000000180) /* ### FOR EN21224 ### */ ||
		(parameter[0].weatherchoice == 1000000181) /* ### FOR EN21225 ### */ ||
		(parameter[0].weatherchoice == 1000000182) /* ### FOR EN21226 ### */ ||
		(parameter[0].weatherchoice == 1000000183) /* ### FOR EN21227 ### */ ||
		(parameter[0].weatherchoice == 1000000188) /* ### FOR EN21232 ### */ ||
		(parameter[0].weatherchoice == 1000000189) /* ### FOR EN21233 ### */ ||
		(parameter[0].weatherchoice == 1000000190) /* ### FOR EN21234 ### */ ||
		(parameter[0].weatherchoice == 1000000191) /* ### FOR EN21235 ### */ ||
		(parameter[0].weatherchoice == 1000000192) /* ### FOR EN21236 ### */ ||
		(parameter[0].weatherchoice == 1000000193) /* ### FOR EN21237 ### */ ||
		(parameter[0].weatherchoice == 1000000194) /* ### FOR EN21238 ### */ ||
		(parameter[0].weatherchoice == 1000000195) /* ### FOR EN21239 ### */ ||
		(parameter[0].weatherchoice == 1000000196) /* ### FOR EN21240 ### */ ||
		(parameter[0].weatherchoice == 1000000197) /* ### FOR EN21241 ### */ ||
		(parameter[0].weatherchoice == 1000000198) /* ### FOR EN21242 ### */ ||
		(parameter[0].weatherchoice == 1000000199) /* ### FOR EN21243 ### */ ||
		(parameter[0].weatherchoice == 1000000200) /* ### FOR EN21244 ### */ ||
		(parameter[0].weatherchoice == 1000000201) /* ### FOR EN21245 ### */ ||
		(parameter[0].weatherchoice == 1000000202) /* ### FOR EN21246 ### */ ||
		(parameter[0].weatherchoice == 1000000203) /* ### FOR EN21247 ### */ ||
		(parameter[0].weatherchoice == 1000000204) /* ### FOR EN21248 ### */ ||
		(parameter[0].weatherchoice == 1000000205) /* ### FOR EN21249 ### */ ||
		(parameter[0].weatherchoice == 1000000206) /* ### FOR EN21250 ### */ ||
		(parameter[0].weatherchoice == 1000000207) /* ### FOR EN21251 ### */ ||
		(parameter[0].weatherchoice == 1000000208) /* ### FOR EN21252 ### */ ||
		(parameter[0].weatherchoice == 1000000209) /* ### FOR EN21253 ### */ ||
		(parameter[0].weatherchoice == 1000000210) /* ### FOR EN21254 ### */ ||
		(parameter[0].weatherchoice == 1000000211) /* ### FOR EN21255 ### */ ||
		(parameter[0].weatherchoice == 1000000212) /* ### FOR EN21256 ### */ ||
		(parameter[0].weatherchoice == 1000000213) /* ### FOR EN21257 ### */ ||
		(parameter[0].weatherchoice == 1000000214) /* ### FOR EN21258 ### */ ||
		(parameter[0].weatherchoice == 1000000215) /* ### FOR EN21259 ### */ ||
		(parameter[0].weatherchoice == 1000000216) /* ### FOR EN21260 ### */ ||
		(parameter[0].weatherchoice == 1000000217) /* ### FOR EN21261 ### */ ||
		(parameter[0].weatherchoice == 1000000218)) /* ### FOR EN21262 ### */ {
		foldername << "cell_10";
		} else if ((parameter[0].weatherchoice == 1000000184) /* ### FOR EN21228 ### */ ||
		(parameter[0].weatherchoice == 1000000185) /* ### FOR EN21229 ### */ ||
		(parameter[0].weatherchoice == 1000000186) /* ### FOR EN21230 ### */ ||
		(parameter[0].weatherchoice == 1000000187)) /* ### FOR EN21231 ### */ {
		foldername << "cell_11";
		} else if ((parameter[0].weatherchoice == 1000000256) /* ### FOR EN22-038 ### */ ||
		(parameter[0].weatherchoice == 1000000257) /* ### FOR EN22-039 ### */ ||
		(parameter[0].weatherchoice == 1000000258) /* ### FOR EN22-040 ### */ ||
		(parameter[0].weatherchoice == 1000000259) /* ### FOR EN22-041 ### */ ||
		(parameter[0].weatherchoice == 1000000260) /* ### FOR EN22-042 ### */ ||
		(parameter[0].weatherchoice == 1000000261) /* ### FOR EN22-043 ### */ ||
		(parameter[0].weatherchoice == 1000000262) /* ### FOR EN22-044 ### */ ||
		(parameter[0].weatherchoice == 1000000263) /* ### FOR EN22-045 ### */ ||
		(parameter[0].weatherchoice == 1000000264) /* ### FOR EN22-046 ### */ ||
		(parameter[0].weatherchoice == 1000000265) /* ### FOR EN22-047 ### */ ||
		(parameter[0].weatherchoice == 1000000266) /* ### FOR EN22-048 ### */ ||
		(parameter[0].weatherchoice == 1000000267) /* ### FOR EN22-049 ### */ ||
		(parameter[0].weatherchoice == 1000000268) /* ### FOR EN22-050 ### */ ||
		(parameter[0].weatherchoice == 1000000269) /* ### FOR EN22-051 ### */ ||
		(parameter[0].weatherchoice == 1000000270) /* ### FOR EN22-052 ### */ ||
		(parameter[0].weatherchoice == 1000000271) /* ### FOR EN22-053 ### */ ||
		(parameter[0].weatherchoice == 1000000272)) /* ### FOR EN22-054 ### */ {
		foldername << "cell_12";
		} else if ((parameter[0].weatherchoice == 1000000255) /* ### FOR EN22-037 ### */ ||
		(parameter[0].weatherchoice == 1000000273)) /* ### FOR EN22-055 ### */ {
		foldername << "cell_13";
		} else if ((parameter[0].weatherchoice == 1000000137) /* ### FOR EN18067 ### */ ||
		(parameter[0].weatherchoice == 1000000138) /* ### FOR EN18068 ### */ ||
		(parameter[0].weatherchoice == 1000000140) /* ### FOR EN18070_centre ### */ ||
		(parameter[0].weatherchoice == 1000000141) /* ### FOR EN18070_edge ### */ ||
		(parameter[0].weatherchoice == 1000000142) /* ### FOR EN18070_end ### */ ||
		(parameter[0].weatherchoice == 1000000143)) /* ### FOR EN18070_transition ### */ {
		foldername << "cell_14";
		} else if ((parameter[0].weatherchoice == 1000000139)) /* ### FOR EN18069 ### */ {
		foldername << "cell_15";
		} else if ((parameter[0].weatherchoice == 1000000133) /* ### FOR EN18063 ### */ ||
		(parameter[0].weatherchoice == 1000000134) /* ### FOR EN18064 ### */ ||
		(parameter[0].weatherchoice == 1000000135) /* ### FOR EN18065 ### */ ||
		(parameter[0].weatherchoice == 1000000136)) /* ### FOR EN18066 ### */ {
		foldername << "cell_16";
		} else if ((parameter[0].weatherchoice == 1000000157) /* ### FOR EN21201 ### */ ||
		(parameter[0].weatherchoice == 1000000159) /* ### FOR EN21203 ### */ ||
		(parameter[0].weatherchoice == 1000000160) /* ### FOR EN21204 ### */ ||
		(parameter[0].weatherchoice == 1000000161) /* ### FOR EN21205 ### */ ||
		(parameter[0].weatherchoice == 1000000165) /* ### FOR EN21209 ### */ ||
		(parameter[0].weatherchoice == 1000000166) /* ### FOR EN21210 ### */ ||
		(parameter[0].weatherchoice == 1000000167) /* ### FOR EN21211 ### */ ||
		(parameter[0].weatherchoice == 1000000171) /* ### FOR EN21215 ### */ ||
		(parameter[0].weatherchoice == 1000000172) /* ### FOR EN21216 ### */ ||
		(parameter[0].weatherchoice == 1000000173) /* ### FOR EN21217 ### */ ||
		(parameter[0].weatherchoice == 1000000174) /* ### FOR EN21218 ### */ ||
		(parameter[0].weatherchoice == 1000000175)) /* ### FOR EN21219 ### */ {
		foldername << "cell_17";
		} else if ((parameter[0].weatherchoice == 1000000158) /* ### FOR EN21202 ### */ ||
		(parameter[0].weatherchoice == 1000000162) /* ### FOR EN21206 ### */ ||
		(parameter[0].weatherchoice == 1000000163) /* ### FOR EN21207 ### */ ||
		(parameter[0].weatherchoice == 1000000164) /* ### FOR EN21208 ### */ ||
		(parameter[0].weatherchoice == 1000000168) /* ### FOR EN21212 ### */ ||
		(parameter[0].weatherchoice == 1000000169) /* ### FOR EN21213 ### */ ||
		(parameter[0].weatherchoice == 1000000170)) /* ### FOR EN21214 ### */ {
		foldername << "cell_18";
		} else if ((parameter[0].weatherchoice == 1000000080) /* ### FOR 16-KP-V20 ### */ ||
		(parameter[0].weatherchoice == 1000000081) /* ### FOR 16-KP-V21 ### */ ||
		(parameter[0].weatherchoice == 1000000082) /* ### FOR 16-KP-V22 ### */ ||
		(parameter[0].weatherchoice == 1000000083) /* ### FOR 16-KP-V23 ### */ ||
		(parameter[0].weatherchoice == 1000000084) /* ### FOR 16-KP-V24 ### */ ||
		(parameter[0].weatherchoice == 1000000085) /* ### FOR 16-KP-V25 ### */ ||
		(parameter[0].weatherchoice == 1000000086) /* ### FOR 16-KP-V26 ### */ ||
		(parameter[0].weatherchoice == 1000000087) /* ### FOR 16-KP-V27 ### */ ||
		(parameter[0].weatherchoice == 1000000088) /* ### FOR 16-KP-V28 ### */ ||
		(parameter[0].weatherchoice == 1000000089) /* ### FOR 16-KP-V29 ### */ ||
		(parameter[0].weatherchoice == 1000000090) /* ### FOR 16-KP-V30 ### */ ||
		(parameter[0].weatherchoice == 1000000091) /* ### FOR 16-KP-V31 ### */ ||
		(parameter[0].weatherchoice == 1000000092) /* ### FOR 16-KP-V32 ### */ ||
		(parameter[0].weatherchoice == 1000000093) /* ### FOR 16-KP-V33 ### */ ||
		(parameter[0].weatherchoice == 1000000094) /* ### FOR 16-KP-V34 ### */ ||
		(parameter[0].weatherchoice == 1000000095) /* ### FOR 16-KP-V35 ### */ ||
		(parameter[0].weatherchoice == 1000000096) /* ### FOR 16-KP-V36 ### */ ||
		(parameter[0].weatherchoice == 1000000097) /* ### FOR 16-KP-V37 ### */ ||
		(parameter[0].weatherchoice == 1000000098) /* ### FOR 16-KP-V38 ### */ ||
		(parameter[0].weatherchoice == 1000000099)) /* ### FOR 16-KP-V39 ### */ {
		foldername << "cell_19";
		} else if ((parameter[0].weatherchoice == 1000000223) /* ### FOR EN22-005 ### */ ||
		(parameter[0].weatherchoice == 1000000224) /* ### FOR EN22-006 ### */ ||
		(parameter[0].weatherchoice == 1000000225) /* ### FOR EN22-007 ### */ ||
		(parameter[0].weatherchoice == 1000000226) /* ### FOR EN22-008 ### */ ||
		(parameter[0].weatherchoice == 1000000227) /* ### FOR EN22-009 ### */ ||
		(parameter[0].weatherchoice == 1000000228) /* ### FOR EN22-010 ### */ ||
		(parameter[0].weatherchoice == 1000000229) /* ### FOR EN22-011 ### */ ||
		(parameter[0].weatherchoice == 1000000230) /* ### FOR EN22-012 ### */ ||
		(parameter[0].weatherchoice == 1000000231) /* ### FOR EN22-013 ### */ ||
		(parameter[0].weatherchoice == 1000000232) /* ### FOR EN22-014 ### */ ||
		(parameter[0].weatherchoice == 1000000233) /* ### FOR EN22-015 ### */ ||
		(parameter[0].weatherchoice == 1000000234) /* ### FOR EN22-016 ### */ ||
		(parameter[0].weatherchoice == 1000000235) /* ### FOR EN22-017 ### */ ||
		(parameter[0].weatherchoice == 1000000236) /* ### FOR EN22-018 ### */ ||
		(parameter[0].weatherchoice == 1000000237) /* ### FOR EN22-019 ### */ ||
		(parameter[0].weatherchoice == 1000000238) /* ### FOR EN22-020 ### */ ||
		(parameter[0].weatherchoice == 1000000239) /* ### FOR EN22-021 ### */ ||
		(parameter[0].weatherchoice == 1000000240) /* ### FOR EN22-022 ### */ ||
		(parameter[0].weatherchoice == 1000000241) /* ### FOR EN22-023 ### */ ||
		(parameter[0].weatherchoice == 1000000242) /* ### FOR EN22-024 ### */ ||
		(parameter[0].weatherchoice == 1000000243) /* ### FOR EN22-025 ### */ ||
		(parameter[0].weatherchoice == 1000000244) /* ### FOR EN22-026 ### */ ||
		(parameter[0].weatherchoice == 1000000245) /* ### FOR EN22-027 ### */ ||
		(parameter[0].weatherchoice == 1000000246) /* ### FOR EN22-028 ### */ ||
		(parameter[0].weatherchoice == 1000000247) /* ### FOR EN22-029 ### */ ||
		(parameter[0].weatherchoice == 1000000248) /* ### FOR EN22-030 ### */ ||
		(parameter[0].weatherchoice == 1000000249) /* ### FOR EN22-031 ### */ ||
		(parameter[0].weatherchoice == 1000000250) /* ### FOR EN22-032 ### */ ||
		(parameter[0].weatherchoice == 1000000251) /* ### FOR EN22-033 ### */ ||
		(parameter[0].weatherchoice == 1000000252) /* ### FOR EN22-034 ### */ ||
		(parameter[0].weatherchoice == 1000000253) /* ### FOR EN22-035 ### */ ||
		(parameter[0].weatherchoice == 1000000254)) /* ### FOR EN22-036 ### */ {
		foldername << "cell_20";
		} else if ((parameter[0].weatherchoice == 1000000129) /* ### FOR B19-T1 ### */ ||
		(parameter[0].weatherchoice == 1000000130)) /* ### FOR B19-T2 ### */ {
		foldername << "cell_21";
		} else if ((parameter[0].weatherchoice == 1000000009) /* ### FOR 12-KO-02a ### */ ||
		(parameter[0].weatherchoice == 1000000010) /* ### FOR 12-KO-02b ### */ ||
		(parameter[0].weatherchoice == 1000000011) /* ### FOR 12-KO-03a ### */ ||
		(parameter[0].weatherchoice == 1000000012) /* ### FOR 12-KO-03b ### */ ||
		(parameter[0].weatherchoice == 1000000013) /* ### FOR 12-KO-04a ### */ ||
		(parameter[0].weatherchoice == 1000000014) /* ### FOR 12-KO-04b ### */ ||
		(parameter[0].weatherchoice == 1000000015)) /* ### FOR 12-KO-05 ### */ {
		foldername << "cell_22";
		} else if ((parameter[0].weatherchoice == 1000000073) /* ### FOR 16-KP-V13 ### */ ||
		(parameter[0].weatherchoice == 1000000074) /* ### FOR 16-KP-V14 ### */ ||
		(parameter[0].weatherchoice == 1000000075) /* ### FOR 16-KP-V15 ### */ ||
		(parameter[0].weatherchoice == 1000000076) /* ### FOR 16-KP-V16 ### */ ||
		(parameter[0].weatherchoice == 1000000077) /* ### FOR 16-KP-V17 ### */ ||
		(parameter[0].weatherchoice == 1000000078) /* ### FOR 16-KP-V18 ### */ ||
		(parameter[0].weatherchoice == 1000000079) /* ### FOR 16-KP-V19 ### */ ||
		(parameter[0].weatherchoice == 1000000119) /* ### FOR 18-BIL-00-EN18000 ### */ ||
		(parameter[0].weatherchoice == 1000000120) /* ### FOR 18-BIL-01-EN18028 ### */ ||
		(parameter[0].weatherchoice == 1000000121) /* ### FOR 18-BIL-01-EN18029 ### */ ||
		(parameter[0].weatherchoice == 1000000122) /* ### FOR 18-BIL-02-EN18030 ### */ ||
		(parameter[0].weatherchoice == 1000000123) /* ### FOR 18-BIL-02-EN18031 ### */ ||
		(parameter[0].weatherchoice == 1000000124) /* ### FOR 18-BIL-02-EN18032 ### */ ||
		(parameter[0].weatherchoice == 1000000125) /* ### FOR 18-BIL-02-EN18033 ### */ ||
		(parameter[0].weatherchoice == 1000000126) /* ### FOR 18-BIL-02-EN18034 ### */ ||
		(parameter[0].weatherchoice == 1000000127)) /* ### FOR 18-BIL-02-EN18035 ### */ {
		foldername << "cell_23";
		} else if ((parameter[0].weatherchoice == 1000000061) /* ### FOR 16-KP-V01 ### */ ||
		(parameter[0].weatherchoice == 1000000062) /* ### FOR 16-KP-V02 ### */ ||
		(parameter[0].weatherchoice == 1000000063) /* ### FOR 16-KP-V03 ### */ ||
		(parameter[0].weatherchoice == 1000000064) /* ### FOR 16-KP-V04 ### */ ||
		(parameter[0].weatherchoice == 1000000065) /* ### FOR 16-KP-V05 ### */ ||
		(parameter[0].weatherchoice == 1000000066) /* ### FOR 16-KP-V06 ### */ ||
		(parameter[0].weatherchoice == 1000000067) /* ### FOR 16-KP-V07 ### */ ||
		(parameter[0].weatherchoice == 1000000068) /* ### FOR 16-KP-V08 ### */ ||
		(parameter[0].weatherchoice == 1000000069) /* ### FOR 16-KP-V09 ### */ ||
		(parameter[0].weatherchoice == 1000000070) /* ### FOR 16-KP-V10 ### */ ||
		(parameter[0].weatherchoice == 1000000071) /* ### FOR 16-KP-V11 ### */ ||
		(parameter[0].weatherchoice == 1000000072) /* ### FOR 16-KP-V12 ### */ ||
		(parameter[0].weatherchoice == 1000000100) /* ### FOR 16-KP-V40 ### */ ||
		(parameter[0].weatherchoice == 1000000101) /* ### FOR 16-KP-V41 ### */ ||
		(parameter[0].weatherchoice == 1000000102) /* ### FOR 16-KP-V42 ### */ ||
		(parameter[0].weatherchoice == 1000000103) /* ### FOR 16-KP-V43 ### */ ||
		(parameter[0].weatherchoice == 1000000104) /* ### FOR 16-KP-V44 ### */ ||
		(parameter[0].weatherchoice == 1000000105) /* ### FOR 16-KP-V45 ### */ ||
		(parameter[0].weatherchoice == 1000000106) /* ### FOR 16-KP-V46 ### */ ||
		(parameter[0].weatherchoice == 1000000107) /* ### FOR 16-KP-V47 ### */ ||
		(parameter[0].weatherchoice == 1000000108) /* ### FOR 16-KP-V48 ### */ ||
		(parameter[0].weatherchoice == 1000000109) /* ### FOR 16-KP-V49 ### */ ||
		(parameter[0].weatherchoice == 1000000110) /* ### FOR 16-KP-V50 ### */ ||
		(parameter[0].weatherchoice == 1000000111) /* ### FOR 16-KP-V51 ### */ ||
		(parameter[0].weatherchoice == 1000000112) /* ### FOR 16-KP-V52 ### */ ||
		(parameter[0].weatherchoice == 1000000113) /* ### FOR 16-KP-V53 ### */ ||
		(parameter[0].weatherchoice == 1000000114) /* ### FOR 16-KP-V54 ### */ ||
		(parameter[0].weatherchoice == 1000000115) /* ### FOR 16-KP-V55 ### */ ||
		(parameter[0].weatherchoice == 1000000116) /* ### FOR 16-KP-V56 ### */ ||
		(parameter[0].weatherchoice == 1000000117) /* ### FOR 16-KP-V57 ### */ ||
		(parameter[0].weatherchoice == 1000000118) /* ### FOR 16-KP-V58 ### */ ||
		(parameter[0].weatherchoice == 1000000029) /* ### FOR 16-KP-01-EN18001 ### */ ||
		(parameter[0].weatherchoice == 1000000030) /* ### FOR 16-KP-01-EN18002 ### */ ||
		(parameter[0].weatherchoice == 1000000031) /* ### FOR 16-KP-01-EN18003 ### */ ||
		(parameter[0].weatherchoice == 1000000032) /* ### FOR 16-KP-01-EN18004 ### */ ||
		(parameter[0].weatherchoice == 1000000033) /* ### FOR 16-KP-01-EN18005 ### */ ||
		(parameter[0].weatherchoice == 1000000034) /* ### FOR 16-KP-01-EN18006 ### */ ||
		(parameter[0].weatherchoice == 1000000035) /* ### FOR 16-KP-01-EN18007 ### */ ||
		(parameter[0].weatherchoice == 1000000036) /* ### FOR 16-KP-01-EN18008 ### */ ||
		(parameter[0].weatherchoice == 1000000037) /* ### FOR 16-KP-01-EN18009 ### */ ||
		(parameter[0].weatherchoice == 1000000038) /* ### FOR 16-KP-01-EN18010 ### */ ||
		(parameter[0].weatherchoice == 1000000039) /* ### FOR 16-KP-01-EN18011 ### */ ||
		(parameter[0].weatherchoice == 1000000040) /* ### FOR 16-KP-01-EN18012 ### */ ||
		(parameter[0].weatherchoice == 1000000041) /* ### FOR 16-KP-01-EN18013 ### */ ||
		(parameter[0].weatherchoice == 1000000042) /* ### FOR 16-KP-01-EN18014 ### */ ||
		(parameter[0].weatherchoice == 1000000043) /* ### FOR 16-KP-01-EN18015 ### */ ||
		(parameter[0].weatherchoice == 1000000044) /* ### FOR 16-KP-01-EN18016 ### */ ||
		(parameter[0].weatherchoice == 1000000045) /* ### FOR 16-KP-01-EN18017 ### */ ||
		(parameter[0].weatherchoice == 1000000046) /* ### FOR 16-KP-01-EN18018 ### */ ||
		(parameter[0].weatherchoice == 1000000047) /* ### FOR 16-KP-01-EN18019 ### */ ||
		(parameter[0].weatherchoice == 1000000048) /* ### FOR 16-KP-01-EN18020 ### */ ||
		(parameter[0].weatherchoice == 1000000049) /* ### FOR 16-KP-01-EN18021 ### */ ||
		(parameter[0].weatherchoice == 1000000050) /* ### FOR 16-KP-01-EN18022 ### */ ||
		(parameter[0].weatherchoice == 1000000051) /* ### FOR 16-KP-01-EN18023 ### */ ||
		(parameter[0].weatherchoice == 1000000052) /* ### FOR 16-KP-01-EN18024 ### */ ||
		(parameter[0].weatherchoice == 1000000053) /* ### FOR 16-KP-01-EN18025 ### */ ||
		(parameter[0].weatherchoice == 1000000054) /* ### FOR 16-KP-01-EN18026 ### */ ||
		(parameter[0].weatherchoice == 1000000055) /* ### FOR 16-KP-01-EN18027 ### */ ||
		(parameter[0].weatherchoice == 1000000056) /* ### FOR 16-KP-04-EN18051 ### */ ||
		(parameter[0].weatherchoice == 1000000057) /* ### FOR 16-KP-04-EN18052 ### */ ||
		(parameter[0].weatherchoice == 1000000058) /* ### FOR 16-KP-04-EN18053 ### */ ||
		(parameter[0].weatherchoice == 1000000059) /* ### FOR 16-KP-04-EN18054 ### */ ||
		(parameter[0].weatherchoice == 1000000060)) /* ### FOR 16-KP-04-EN18055 ### */ {
		foldername << "cell_24";
		} else if ((parameter[0].weatherchoice == 1000000003) /* ### FOR 11-CH-06I ### */ ||
		(parameter[0].weatherchoice == 1000000004)) /* ### FOR 11-CH-06III ### */ {
		foldername << "cell_25";
		} else if ((parameter[0].weatherchoice == 1000000001) /* ### FOR 11-CH-02II ### */ ||
		(parameter[0].weatherchoice == 1000000002) /* ### FOR 11-CH-02III ### */ ||
		(parameter[0].weatherchoice == 1000000005) /* ### FOR 11-CH-12I ### */ ||
		(parameter[0].weatherchoice == 1000000006) /* ### FOR 11-CH-12II ### */ ||
		(parameter[0].weatherchoice == 1000000007) /* ### FOR 11-CH-17I ### */ ||
		(parameter[0].weatherchoice == 1000000008) /* ### FOR 11-CH-17II ### */ ||
		(parameter[0].weatherchoice == 1000000296) /* ### FOR TY07VI ### */ ||
		(parameter[0].weatherchoice == 1000000297) /* ### FOR TY07VII ### */ ||
		(parameter[0].weatherchoice == 1000000298) /* ### FOR TY09VI ### */ ||
		(parameter[0].weatherchoice == 1000000299)) /* ### FOR TY09VII ### */ {
		foldername << "cell_26";
		} else if ((parameter[0].weatherchoice == 1000000016) /* ### FOR 13-TY-02-VI ### */ ||
		(parameter[0].weatherchoice == 1000000017) /* ### FOR 13-TY-02-VII ### */ ||
		(parameter[0].weatherchoice == 1000000294) /* ### FOR TY04VI ### */ ||
		(parameter[0].weatherchoice == 1000000295)) /* ### FOR TY04VII ### */ {
		foldername << "cell_27";
		} else if ((parameter[0].weatherchoice == 1000000128)) /* ### FOR 18-LD-VP012-Tit-Ary ### */ {
		foldername << "cell_28";
		} else if ((parameter[0].weatherchoice == 1000000018) /* ### FOR 14-OM-02-V1 ### */ ||
		(parameter[0].weatherchoice == 1000000019) /* ### FOR 14-OM-02-V2 ### */ ||
		(parameter[0].weatherchoice == 1000000020) /* ### FOR 14-OM-11-V3 ### */ ||
		(parameter[0].weatherchoice == 1000000021) /* ### FOR 14-OM-20-V4 ### */ ||
		(parameter[0].weatherchoice == 1000000022) /* ### FOR 14-OM-TRANS1 ### */ ||
		(parameter[0].weatherchoice == 1000000023) /* ### FOR 14-OM-TRANS2 ### */ ||
		(parameter[0].weatherchoice == 1000000024) /* ### FOR 14-OM-TRANS3 ### */ ||
		(parameter[0].weatherchoice == 1000000025) /* ### FOR 14-OM-TRANS4 ### */ ||
		(parameter[0].weatherchoice == 1000000026) /* ### FOR 14-OM-TRANS5 ### */ ||
		(parameter[0].weatherchoice == 1000000028) /* ### FOR 14-OM-TRANS6 ### */ ||
		(parameter[0].weatherchoice == 1000000027)) /* ### FOR 14-OM-TRANS6-7 ### */ {
		foldername << "cell_29";
		} else if ((parameter[0].weatherchoice == 2000000283) /* ### FOR EN22-065 ### */ ||
		(parameter[0].weatherchoice == 2000000284)) /* ### FOR EN22-066 ### */ {
		foldername << "cell_1";
		} else if ((parameter[0].weatherchoice == 2000000285) /* ### FOR EN22-067 ### */ ||
		(parameter[0].weatherchoice == 2000000286) /* ### FOR EN22-068 ### */ ||
		(parameter[0].weatherchoice == 2000000288) /* ### FOR EN22-070 ### */ ||
		(parameter[0].weatherchoice == 2000000289) /* ### FOR EN22-071 ### */ ||
		(parameter[0].weatherchoice == 2000000290) /* ### FOR EN22-072 ### */ ||
		(parameter[0].weatherchoice == 2000000291) /* ### FOR EN22-073 ### */ ||
		(parameter[0].weatherchoice == 2000000292)) /* ### FOR EN22-074 ### */ {
		foldername << "cell_2";
		} else if ((parameter[0].weatherchoice == 2000000287)) /* ### FOR EN22-069 ### */ {
		foldername << "cell_3";
		} else if ((parameter[0].weatherchoice == 2000000219) /* ### FOR EN22-001 ### */ ||
		(parameter[0].weatherchoice == 2000000220) /* ### FOR EN22-002 ### */ ||
		(parameter[0].weatherchoice == 2000000221) /* ### FOR EN22-003 ### */ ||
		(parameter[0].weatherchoice == 2000000222) /* ### FOR EN22-004 ### */ ||
		(parameter[0].weatherchoice == 2000000274) /* ### FOR EN22-056 ### */ ||
		(parameter[0].weatherchoice == 2000000275) /* ### FOR EN22-057 ### */ ||
		(parameter[0].weatherchoice == 2000000276) /* ### FOR EN22-058 ### */ ||
		(parameter[0].weatherchoice == 2000000277) /* ### FOR EN22-059 ### */ ||
		(parameter[0].weatherchoice == 2000000278)) /* ### FOR EN22-060 ### */ {
		foldername << "cell_4";
		} else if ((parameter[0].weatherchoice == 2000000279) /* ### FOR EN22-061 ### */ ||
		(parameter[0].weatherchoice == 2000000280)) /* ### FOR EN22-062 ### */ {
		foldername << "cell_5";
		} else if ((parameter[0].weatherchoice == 2000000281) /* ### FOR EN22-063 ### */ ||
		(parameter[0].weatherchoice == 2000000282) /* ### FOR EN22-064 ### */ ||
		(parameter[0].weatherchoice == 2000000293)) /* ### FOR EN22-075 ### */ {
		foldername << "cell_6";
		} else if ((parameter[0].weatherchoice == 2000000148) /* ### FOR EN18075 ### */ ||
		(parameter[0].weatherchoice == 2000000149) /* ### FOR EN18076 ### */ ||
		(parameter[0].weatherchoice == 2000000150) /* ### FOR EN18077 ### */ ||
		(parameter[0].weatherchoice == 2000000151) /* ### FOR EN18078 ### */ ||
		(parameter[0].weatherchoice == 2000000152) /* ### FOR EN18079 ### */ ||
		(parameter[0].weatherchoice == 2000000153) /* ### FOR EN18080 ### */ ||
		(parameter[0].weatherchoice == 2000000154) /* ### FOR EN18081 ### */ ||
		(parameter[0].weatherchoice == 2000000155) /* ### FOR EN18082 ### */ ||
		(parameter[0].weatherchoice == 2000000156)) /* ### FOR EN18083 ### */ {
			cout << "usingWinddata2!" << endl;
		foldername << "cell_7";
		} else if ((parameter[0].weatherchoice == 2000000144) /* ### FOR EN18071 ### */ ||
		(parameter[0].weatherchoice == 2000000145) /* ### FOR EN18072 ### */ ||
		(parameter[0].weatherchoice == 2000000146) /* ### FOR EN18073 ### */ ||
		(parameter[0].weatherchoice == 2000000147)) /* ### FOR EN18074 ### */ {
		foldername << "cell_8";
		} else if ((parameter[0].weatherchoice == 2000000132)) /* ### FOR EN18062 ### */ {
		foldername << "cell_9";
		} else if ((parameter[0].weatherchoice == 2000000131) /* ### FOR EN18061 ### */ ||
		(parameter[0].weatherchoice == 2000000176) /* ### FOR EN21220 ### */ ||
		(parameter[0].weatherchoice == 2000000177) /* ### FOR EN21221 ### */ ||
		(parameter[0].weatherchoice == 2000000178) /* ### FOR EN21222 ### */ ||
		(parameter[0].weatherchoice == 2000000179) /* ### FOR EN21223 ### */ ||
		(parameter[0].weatherchoice == 2000000180) /* ### FOR EN21224 ### */ ||
		(parameter[0].weatherchoice == 2000000181) /* ### FOR EN21225 ### */ ||
		(parameter[0].weatherchoice == 2000000182) /* ### FOR EN21226 ### */ ||
		(parameter[0].weatherchoice == 2000000183) /* ### FOR EN21227 ### */ ||
		(parameter[0].weatherchoice == 2000000188) /* ### FOR EN21232 ### */ ||
		(parameter[0].weatherchoice == 2000000189) /* ### FOR EN21233 ### */ ||
		(parameter[0].weatherchoice == 2000000190) /* ### FOR EN21234 ### */ ||
		(parameter[0].weatherchoice == 2000000191) /* ### FOR EN21235 ### */ ||
		(parameter[0].weatherchoice == 2000000192) /* ### FOR EN21236 ### */ ||
		(parameter[0].weatherchoice == 2000000193) /* ### FOR EN21237 ### */ ||
		(parameter[0].weatherchoice == 2000000194) /* ### FOR EN21238 ### */ ||
		(parameter[0].weatherchoice == 2000000195) /* ### FOR EN21239 ### */ ||
		(parameter[0].weatherchoice == 2000000196) /* ### FOR EN21240 ### */ ||
		(parameter[0].weatherchoice == 2000000197) /* ### FOR EN21241 ### */ ||
		(parameter[0].weatherchoice == 2000000198) /* ### FOR EN21242 ### */ ||
		(parameter[0].weatherchoice == 2000000199) /* ### FOR EN21243 ### */ ||
		(parameter[0].weatherchoice == 2000000200) /* ### FOR EN21244 ### */ ||
		(parameter[0].weatherchoice == 2000000201) /* ### FOR EN21245 ### */ ||
		(parameter[0].weatherchoice == 2000000202) /* ### FOR EN21246 ### */ ||
		(parameter[0].weatherchoice == 2000000203) /* ### FOR EN21247 ### */ ||
		(parameter[0].weatherchoice == 2000000204) /* ### FOR EN21248 ### */ ||
		(parameter[0].weatherchoice == 2000000205) /* ### FOR EN21249 ### */ ||
		(parameter[0].weatherchoice == 2000000206) /* ### FOR EN21250 ### */ ||
		(parameter[0].weatherchoice == 2000000207) /* ### FOR EN21251 ### */ ||
		(parameter[0].weatherchoice == 2000000208) /* ### FOR EN21252 ### */ ||
		(parameter[0].weatherchoice == 2000000209) /* ### FOR EN21253 ### */ ||
		(parameter[0].weatherchoice == 2000000210) /* ### FOR EN21254 ### */ ||
		(parameter[0].weatherchoice == 2000000211) /* ### FOR EN21255 ### */ ||
		(parameter[0].weatherchoice == 2000000212) /* ### FOR EN21256 ### */ ||
		(parameter[0].weatherchoice == 2000000213) /* ### FOR EN21257 ### */ ||
		(parameter[0].weatherchoice == 2000000214) /* ### FOR EN21258 ### */ ||
		(parameter[0].weatherchoice == 2000000215) /* ### FOR EN21259 ### */ ||
		(parameter[0].weatherchoice == 2000000216) /* ### FOR EN21260 ### */ ||
		(parameter[0].weatherchoice == 2000000217) /* ### FOR EN21261 ### */ ||
		(parameter[0].weatherchoice == 2000000218)) /* ### FOR EN21262 ### */ {
		foldername << "cell_10";
		} else if ((parameter[0].weatherchoice == 2000000184) /* ### FOR EN21228 ### */ ||
		(parameter[0].weatherchoice == 2000000185) /* ### FOR EN21229 ### */ ||
		(parameter[0].weatherchoice == 2000000186) /* ### FOR EN21230 ### */ ||
		(parameter[0].weatherchoice == 2000000187)) /* ### FOR EN21231 ### */ {
		foldername << "cell_11";
		} else if ((parameter[0].weatherchoice == 2000000256) /* ### FOR EN22-038 ### */ ||
		(parameter[0].weatherchoice == 2000000257) /* ### FOR EN22-039 ### */ ||
		(parameter[0].weatherchoice == 2000000258) /* ### FOR EN22-040 ### */ ||
		(parameter[0].weatherchoice == 2000000259) /* ### FOR EN22-041 ### */ ||
		(parameter[0].weatherchoice == 2000000260) /* ### FOR EN22-042 ### */ ||
		(parameter[0].weatherchoice == 2000000261) /* ### FOR EN22-043 ### */ ||
		(parameter[0].weatherchoice == 2000000262) /* ### FOR EN22-044 ### */ ||
		(parameter[0].weatherchoice == 2000000263) /* ### FOR EN22-045 ### */ ||
		(parameter[0].weatherchoice == 2000000264) /* ### FOR EN22-046 ### */ ||
		(parameter[0].weatherchoice == 2000000265) /* ### FOR EN22-047 ### */ ||
		(parameter[0].weatherchoice == 2000000266) /* ### FOR EN22-048 ### */ ||
		(parameter[0].weatherchoice == 2000000267) /* ### FOR EN22-049 ### */ ||
		(parameter[0].weatherchoice == 2000000268) /* ### FOR EN22-050 ### */ ||
		(parameter[0].weatherchoice == 2000000269) /* ### FOR EN22-051 ### */ ||
		(parameter[0].weatherchoice == 2000000270) /* ### FOR EN22-052 ### */ ||
		(parameter[0].weatherchoice == 2000000271) /* ### FOR EN22-053 ### */ ||
		(parameter[0].weatherchoice == 2000000272)) /* ### FOR EN22-054 ### */ {
		foldername << "cell_12";
		} else if ((parameter[0].weatherchoice == 2000000255) /* ### FOR EN22-037 ### */ ||
		(parameter[0].weatherchoice == 2000000273)) /* ### FOR EN22-055 ### */ {
		foldername << "cell_13";
		} else if ((parameter[0].weatherchoice == 2000000137) /* ### FOR EN18067 ### */ ||
		(parameter[0].weatherchoice == 2000000138) /* ### FOR EN18068 ### */ ||
		(parameter[0].weatherchoice == 2000000140) /* ### FOR EN18070_centre ### */ ||
		(parameter[0].weatherchoice == 2000000141) /* ### FOR EN18070_edge ### */ ||
		(parameter[0].weatherchoice == 2000000142) /* ### FOR EN18070_end ### */ ||
		(parameter[0].weatherchoice == 2000000143)) /* ### FOR EN18070_transition ### */ {
		foldername << "cell_14";
		} else if ((parameter[0].weatherchoice == 2000000139)) /* ### FOR EN18069 ### */ {
		foldername << "cell_15";
		} else if ((parameter[0].weatherchoice == 2000000133) /* ### FOR EN18063 ### */ ||
		(parameter[0].weatherchoice == 2000000134) /* ### FOR EN18064 ### */ ||
		(parameter[0].weatherchoice == 2000000135) /* ### FOR EN18065 ### */ ||
		(parameter[0].weatherchoice == 2000000136)) /* ### FOR EN18066 ### */ {
		foldername << "cell_16";
		} else if ((parameter[0].weatherchoice == 2000000157) /* ### FOR EN21201 ### */ ||
		(parameter[0].weatherchoice == 2000000159) /* ### FOR EN21203 ### */ ||
		(parameter[0].weatherchoice == 2000000160) /* ### FOR EN21204 ### */ ||
		(parameter[0].weatherchoice == 2000000161) /* ### FOR EN21205 ### */ ||
		(parameter[0].weatherchoice == 2000000165) /* ### FOR EN21209 ### */ ||
		(parameter[0].weatherchoice == 2000000166) /* ### FOR EN21210 ### */ ||
		(parameter[0].weatherchoice == 2000000167) /* ### FOR EN21211 ### */ ||
		(parameter[0].weatherchoice == 2000000171) /* ### FOR EN21215 ### */ ||
		(parameter[0].weatherchoice == 2000000172) /* ### FOR EN21216 ### */ ||
		(parameter[0].weatherchoice == 2000000173) /* ### FOR EN21217 ### */ ||
		(parameter[0].weatherchoice == 2000000174) /* ### FOR EN21218 ### */ ||
		(parameter[0].weatherchoice == 2000000175)) /* ### FOR EN21219 ### */ {
		foldername << "cell_17";
		} else if ((parameter[0].weatherchoice == 2000000158) /* ### FOR EN21202 ### */ ||
		(parameter[0].weatherchoice == 2000000162) /* ### FOR EN21206 ### */ ||
		(parameter[0].weatherchoice == 2000000163) /* ### FOR EN21207 ### */ ||
		(parameter[0].weatherchoice == 2000000164) /* ### FOR EN21208 ### */ ||
		(parameter[0].weatherchoice == 2000000168) /* ### FOR EN21212 ### */ ||
		(parameter[0].weatherchoice == 2000000169) /* ### FOR EN21213 ### */ ||
		(parameter[0].weatherchoice == 2000000170)) /* ### FOR EN21214 ### */ {
		foldername << "cell_18";
		} else if ((parameter[0].weatherchoice == 2000000080) /* ### FOR 16-KP-V20 ### */ ||
		(parameter[0].weatherchoice == 2000000081) /* ### FOR 16-KP-V21 ### */ ||
		(parameter[0].weatherchoice == 2000000082) /* ### FOR 16-KP-V22 ### */ ||
		(parameter[0].weatherchoice == 2000000083) /* ### FOR 16-KP-V23 ### */ ||
		(parameter[0].weatherchoice == 2000000084) /* ### FOR 16-KP-V24 ### */ ||
		(parameter[0].weatherchoice == 2000000085) /* ### FOR 16-KP-V25 ### */ ||
		(parameter[0].weatherchoice == 2000000086) /* ### FOR 16-KP-V26 ### */ ||
		(parameter[0].weatherchoice == 2000000087) /* ### FOR 16-KP-V27 ### */ ||
		(parameter[0].weatherchoice == 2000000088) /* ### FOR 16-KP-V28 ### */ ||
		(parameter[0].weatherchoice == 2000000089) /* ### FOR 16-KP-V29 ### */ ||
		(parameter[0].weatherchoice == 2000000090) /* ### FOR 16-KP-V30 ### */ ||
		(parameter[0].weatherchoice == 2000000091) /* ### FOR 16-KP-V31 ### */ ||
		(parameter[0].weatherchoice == 2000000092) /* ### FOR 16-KP-V32 ### */ ||
		(parameter[0].weatherchoice == 2000000093) /* ### FOR 16-KP-V33 ### */ ||
		(parameter[0].weatherchoice == 2000000094) /* ### FOR 16-KP-V34 ### */ ||
		(parameter[0].weatherchoice == 2000000095) /* ### FOR 16-KP-V35 ### */ ||
		(parameter[0].weatherchoice == 2000000096) /* ### FOR 16-KP-V36 ### */ ||
		(parameter[0].weatherchoice == 2000000097) /* ### FOR 16-KP-V37 ### */ ||
		(parameter[0].weatherchoice == 2000000098) /* ### FOR 16-KP-V38 ### */ ||
		(parameter[0].weatherchoice == 2000000099)) /* ### FOR 16-KP-V39 ### */ {
		foldername << "cell_19";
		} else if ((parameter[0].weatherchoice == 2000000223) /* ### FOR EN22-005 ### */ ||
		(parameter[0].weatherchoice == 2000000224) /* ### FOR EN22-006 ### */ ||
		(parameter[0].weatherchoice == 2000000225) /* ### FOR EN22-007 ### */ ||
		(parameter[0].weatherchoice == 2000000226) /* ### FOR EN22-008 ### */ ||
		(parameter[0].weatherchoice == 2000000227) /* ### FOR EN22-009 ### */ ||
		(parameter[0].weatherchoice == 2000000228) /* ### FOR EN22-010 ### */ ||
		(parameter[0].weatherchoice == 2000000229) /* ### FOR EN22-011 ### */ ||
		(parameter[0].weatherchoice == 2000000230) /* ### FOR EN22-012 ### */ ||
		(parameter[0].weatherchoice == 2000000231) /* ### FOR EN22-013 ### */ ||
		(parameter[0].weatherchoice == 2000000232) /* ### FOR EN22-014 ### */ ||
		(parameter[0].weatherchoice == 2000000233) /* ### FOR EN22-015 ### */ ||
		(parameter[0].weatherchoice == 2000000234) /* ### FOR EN22-016 ### */ ||
		(parameter[0].weatherchoice == 2000000235) /* ### FOR EN22-017 ### */ ||
		(parameter[0].weatherchoice == 2000000236) /* ### FOR EN22-018 ### */ ||
		(parameter[0].weatherchoice == 2000000237) /* ### FOR EN22-019 ### */ ||
		(parameter[0].weatherchoice == 2000000238) /* ### FOR EN22-020 ### */ ||
		(parameter[0].weatherchoice == 2000000239) /* ### FOR EN22-021 ### */ ||
		(parameter[0].weatherchoice == 2000000240) /* ### FOR EN22-022 ### */ ||
		(parameter[0].weatherchoice == 2000000241) /* ### FOR EN22-023 ### */ ||
		(parameter[0].weatherchoice == 2000000242) /* ### FOR EN22-024 ### */ ||
		(parameter[0].weatherchoice == 2000000243) /* ### FOR EN22-025 ### */ ||
		(parameter[0].weatherchoice == 2000000244) /* ### FOR EN22-026 ### */ ||
		(parameter[0].weatherchoice == 2000000245) /* ### FOR EN22-027 ### */ ||
		(parameter[0].weatherchoice == 2000000246) /* ### FOR EN22-028 ### */ ||
		(parameter[0].weatherchoice == 2000000247) /* ### FOR EN22-029 ### */ ||
		(parameter[0].weatherchoice == 2000000248) /* ### FOR EN22-030 ### */ ||
		(parameter[0].weatherchoice == 2000000249) /* ### FOR EN22-031 ### */ ||
		(parameter[0].weatherchoice == 2000000250) /* ### FOR EN22-032 ### */ ||
		(parameter[0].weatherchoice == 2000000251) /* ### FOR EN22-033 ### */ ||
		(parameter[0].weatherchoice == 2000000252) /* ### FOR EN22-034 ### */ ||
		(parameter[0].weatherchoice == 2000000253) /* ### FOR EN22-035 ### */ ||
		(parameter[0].weatherchoice == 2000000254)) /* ### FOR EN22-036 ### */ {
		foldername << "cell_20";
		} else if ((parameter[0].weatherchoice == 2000000129) /* ### FOR B19-T1 ### */ ||
		(parameter[0].weatherchoice == 2000000130)) /* ### FOR B19-T2 ### */ {
		foldername << "cell_21";
		} else if ((parameter[0].weatherchoice == 2000000009) /* ### FOR 12-KO-02a ### */ ||
		(parameter[0].weatherchoice == 2000000010) /* ### FOR 12-KO-02b ### */ ||
		(parameter[0].weatherchoice == 2000000011) /* ### FOR 12-KO-03a ### */ ||
		(parameter[0].weatherchoice == 2000000012) /* ### FOR 12-KO-03b ### */ ||
		(parameter[0].weatherchoice == 2000000013) /* ### FOR 12-KO-04a ### */ ||
		(parameter[0].weatherchoice == 2000000014) /* ### FOR 12-KO-04b ### */ ||
		(parameter[0].weatherchoice == 2000000015)) /* ### FOR 12-KO-05 ### */ {
		foldername << "cell_22";
		} else if ((parameter[0].weatherchoice == 2000000073) /* ### FOR 16-KP-V13 ### */ ||
		(parameter[0].weatherchoice == 2000000074) /* ### FOR 16-KP-V14 ### */ ||
		(parameter[0].weatherchoice == 2000000075) /* ### FOR 16-KP-V15 ### */ ||
		(parameter[0].weatherchoice == 2000000076) /* ### FOR 16-KP-V16 ### */ ||
		(parameter[0].weatherchoice == 2000000077) /* ### FOR 16-KP-V17 ### */ ||
		(parameter[0].weatherchoice == 2000000078) /* ### FOR 16-KP-V18 ### */ ||
		(parameter[0].weatherchoice == 2000000079) /* ### FOR 16-KP-V19 ### */ ||
		(parameter[0].weatherchoice == 2000000119) /* ### FOR 18-BIL-00-EN18000 ### */ ||
		(parameter[0].weatherchoice == 2000000120) /* ### FOR 18-BIL-01-EN18028 ### */ ||
		(parameter[0].weatherchoice == 2000000121) /* ### FOR 18-BIL-01-EN18029 ### */ ||
		(parameter[0].weatherchoice == 2000000122) /* ### FOR 18-BIL-02-EN18030 ### */ ||
		(parameter[0].weatherchoice == 2000000123) /* ### FOR 18-BIL-02-EN18031 ### */ ||
		(parameter[0].weatherchoice == 2000000124) /* ### FOR 18-BIL-02-EN18032 ### */ ||
		(parameter[0].weatherchoice == 2000000125) /* ### FOR 18-BIL-02-EN18033 ### */ ||
		(parameter[0].weatherchoice == 2000000126) /* ### FOR 18-BIL-02-EN18034 ### */ ||
		(parameter[0].weatherchoice == 2000000127)) /* ### FOR 18-BIL-02-EN18035 ### */ {
		foldername << "cell_23";
		} else if ((parameter[0].weatherchoice == 2000000061) /* ### FOR 16-KP-V01 ### */ ||
		(parameter[0].weatherchoice == 2000000062) /* ### FOR 16-KP-V02 ### */ ||
		(parameter[0].weatherchoice == 2000000063) /* ### FOR 16-KP-V03 ### */ ||
		(parameter[0].weatherchoice == 2000000064) /* ### FOR 16-KP-V04 ### */ ||
		(parameter[0].weatherchoice == 2000000065) /* ### FOR 16-KP-V05 ### */ ||
		(parameter[0].weatherchoice == 2000000066) /* ### FOR 16-KP-V06 ### */ ||
		(parameter[0].weatherchoice == 2000000067) /* ### FOR 16-KP-V07 ### */ ||
		(parameter[0].weatherchoice == 2000000068) /* ### FOR 16-KP-V08 ### */ ||
		(parameter[0].weatherchoice == 2000000069) /* ### FOR 16-KP-V09 ### */ ||
		(parameter[0].weatherchoice == 2000000070) /* ### FOR 16-KP-V10 ### */ ||
		(parameter[0].weatherchoice == 2000000071) /* ### FOR 16-KP-V11 ### */ ||
		(parameter[0].weatherchoice == 2000000072) /* ### FOR 16-KP-V12 ### */ ||
		(parameter[0].weatherchoice == 2000000100) /* ### FOR 16-KP-V40 ### */ ||
		(parameter[0].weatherchoice == 2000000101) /* ### FOR 16-KP-V41 ### */ ||
		(parameter[0].weatherchoice == 2000000102) /* ### FOR 16-KP-V42 ### */ ||
		(parameter[0].weatherchoice == 2000000103) /* ### FOR 16-KP-V43 ### */ ||
		(parameter[0].weatherchoice == 2000000104) /* ### FOR 16-KP-V44 ### */ ||
		(parameter[0].weatherchoice == 2000000105) /* ### FOR 16-KP-V45 ### */ ||
		(parameter[0].weatherchoice == 2000000106) /* ### FOR 16-KP-V46 ### */ ||
		(parameter[0].weatherchoice == 2000000107) /* ### FOR 16-KP-V47 ### */ ||
		(parameter[0].weatherchoice == 2000000108) /* ### FOR 16-KP-V48 ### */ ||
		(parameter[0].weatherchoice == 2000000109) /* ### FOR 16-KP-V49 ### */ ||
		(parameter[0].weatherchoice == 2000000110) /* ### FOR 16-KP-V50 ### */ ||
		(parameter[0].weatherchoice == 2000000111) /* ### FOR 16-KP-V51 ### */ ||
		(parameter[0].weatherchoice == 2000000112) /* ### FOR 16-KP-V52 ### */ ||
		(parameter[0].weatherchoice == 2000000113) /* ### FOR 16-KP-V53 ### */ ||
		(parameter[0].weatherchoice == 2000000114) /* ### FOR 16-KP-V54 ### */ ||
		(parameter[0].weatherchoice == 2000000115) /* ### FOR 16-KP-V55 ### */ ||
		(parameter[0].weatherchoice == 2000000116) /* ### FOR 16-KP-V56 ### */ ||
		(parameter[0].weatherchoice == 2000000117) /* ### FOR 16-KP-V57 ### */ ||
		(parameter[0].weatherchoice == 2000000118) /* ### FOR 16-KP-V58 ### */ ||
		(parameter[0].weatherchoice == 2000000029) /* ### FOR 16-KP-01-EN18001 ### */ ||
		(parameter[0].weatherchoice == 2000000030) /* ### FOR 16-KP-01-EN18002 ### */ ||
		(parameter[0].weatherchoice == 2000000031) /* ### FOR 16-KP-01-EN18003 ### */ ||
		(parameter[0].weatherchoice == 2000000032) /* ### FOR 16-KP-01-EN18004 ### */ ||
		(parameter[0].weatherchoice == 2000000033) /* ### FOR 16-KP-01-EN18005 ### */ ||
		(parameter[0].weatherchoice == 2000000034) /* ### FOR 16-KP-01-EN18006 ### */ ||
		(parameter[0].weatherchoice == 2000000035) /* ### FOR 16-KP-01-EN18007 ### */ ||
		(parameter[0].weatherchoice == 2000000036) /* ### FOR 16-KP-01-EN18008 ### */ ||
		(parameter[0].weatherchoice == 2000000037) /* ### FOR 16-KP-01-EN18009 ### */ ||
		(parameter[0].weatherchoice == 2000000038) /* ### FOR 16-KP-01-EN18010 ### */ ||
		(parameter[0].weatherchoice == 2000000039) /* ### FOR 16-KP-01-EN18011 ### */ ||
		(parameter[0].weatherchoice == 2000000040) /* ### FOR 16-KP-01-EN18012 ### */ ||
		(parameter[0].weatherchoice == 2000000041) /* ### FOR 16-KP-01-EN18013 ### */ ||
		(parameter[0].weatherchoice == 2000000042) /* ### FOR 16-KP-01-EN18014 ### */ ||
		(parameter[0].weatherchoice == 2000000043) /* ### FOR 16-KP-01-EN18015 ### */ ||
		(parameter[0].weatherchoice == 2000000044) /* ### FOR 16-KP-01-EN18016 ### */ ||
		(parameter[0].weatherchoice == 2000000045) /* ### FOR 16-KP-01-EN18017 ### */ ||
		(parameter[0].weatherchoice == 2000000046) /* ### FOR 16-KP-01-EN18018 ### */ ||
		(parameter[0].weatherchoice == 2000000047) /* ### FOR 16-KP-01-EN18019 ### */ ||
		(parameter[0].weatherchoice == 2000000048) /* ### FOR 16-KP-01-EN18020 ### */ ||
		(parameter[0].weatherchoice == 2000000049) /* ### FOR 16-KP-01-EN18021 ### */ ||
		(parameter[0].weatherchoice == 2000000050) /* ### FOR 16-KP-01-EN18022 ### */ ||
		(parameter[0].weatherchoice == 2000000051) /* ### FOR 16-KP-01-EN18023 ### */ ||
		(parameter[0].weatherchoice == 2000000052) /* ### FOR 16-KP-01-EN18024 ### */ ||
		(parameter[0].weatherchoice == 2000000053) /* ### FOR 16-KP-01-EN18025 ### */ ||
		(parameter[0].weatherchoice == 2000000054) /* ### FOR 16-KP-01-EN18026 ### */ ||
		(parameter[0].weatherchoice == 2000000055) /* ### FOR 16-KP-01-EN18027 ### */ ||
		(parameter[0].weatherchoice == 2000000056) /* ### FOR 16-KP-04-EN18051 ### */ ||
		(parameter[0].weatherchoice == 2000000057) /* ### FOR 16-KP-04-EN18052 ### */ ||
		(parameter[0].weatherchoice == 2000000058) /* ### FOR 16-KP-04-EN18053 ### */ ||
		(parameter[0].weatherchoice == 2000000059) /* ### FOR 16-KP-04-EN18054 ### */ ||
		(parameter[0].weatherchoice == 2000000060)) /* ### FOR 16-KP-04-EN18055 ### */ {
		foldername << "cell_24";
		} else if ((parameter[0].weatherchoice == 2000000003) /* ### FOR 11-CH-06I ### */ ||
		(parameter[0].weatherchoice == 2000000004)) /* ### FOR 11-CH-06III ### */ {
		foldername << "cell_25";
		} else if ((parameter[0].weatherchoice == 2000000001) /* ### FOR 11-CH-02II ### */ ||
		(parameter[0].weatherchoice == 2000000002) /* ### FOR 11-CH-02III ### */ ||
		(parameter[0].weatherchoice == 2000000005) /* ### FOR 11-CH-12I ### */ ||
		(parameter[0].weatherchoice == 2000000006) /* ### FOR 11-CH-12II ### */ ||
		(parameter[0].weatherchoice == 2000000007) /* ### FOR 11-CH-17I ### */ ||
		(parameter[0].weatherchoice == 2000000008) /* ### FOR 11-CH-17II ### */ ||
		(parameter[0].weatherchoice == 2000000296) /* ### FOR TY07VI ### */ ||
		(parameter[0].weatherchoice == 2000000297) /* ### FOR TY07VII ### */ ||
		(parameter[0].weatherchoice == 2000000298) /* ### FOR TY09VI ### */ ||
		(parameter[0].weatherchoice == 2000000299)) /* ### FOR TY09VII ### */ {
		foldername << "cell_26";
		} else if ((parameter[0].weatherchoice == 2000000016) /* ### FOR 13-TY-02-VI ### */ ||
		(parameter[0].weatherchoice == 2000000017) /* ### FOR 13-TY-02-VII ### */ ||
		(parameter[0].weatherchoice == 2000000294) /* ### FOR TY04VI ### */ ||
		(parameter[0].weatherchoice == 2000000295)) /* ### FOR TY04VII ### */ {
		foldername << "cell_27";
		} else if ((parameter[0].weatherchoice == 2000000128)) /* ### FOR 18-LD-VP012-Tit-Ary ### */ {
		foldername << "cell_28";
		} else if ((parameter[0].weatherchoice == 2000000018) /* ### FOR 14-OM-02-V1 ### */ ||
		(parameter[0].weatherchoice == 2000000019) /* ### FOR 14-OM-02-V2 ### */ ||
		(parameter[0].weatherchoice == 2000000020) /* ### FOR 14-OM-11-V3 ### */ ||
		(parameter[0].weatherchoice == 2000000021) /* ### FOR 14-OM-20-V4 ### */ ||
		(parameter[0].weatherchoice == 2000000022) /* ### FOR 14-OM-TRANS1 ### */ ||
		(parameter[0].weatherchoice == 2000000023) /* ### FOR 14-OM-TRANS2 ### */ ||
		(parameter[0].weatherchoice == 2000000024) /* ### FOR 14-OM-TRANS3 ### */ ||
		(parameter[0].weatherchoice == 2000000025) /* ### FOR 14-OM-TRANS4 ### */ ||
		(parameter[0].weatherchoice == 2000000026) /* ### FOR 14-OM-TRANS5 ### */ ||
		(parameter[0].weatherchoice == 2000000028) /* ### FOR 14-OM-TRANS6 ### */ ||
		(parameter[0].weatherchoice == 2000000027)) /* ### FOR 14-OM-TRANS6-7 ### */ {
		foldername << "cell_29";
		} else {
			cout << "No wind data for weather choice available, reading data for Chukotka!X" << endl;
            foldername << "wind_Chukotka";
		}



        for (int t = 0; t < parameter[0].simduration; t++) {
            cntr = 0;

            jahr = parameter[0].startjahr + t;

            if (parameter[0].windsource == 1) {
                findyr1 = 1979;
                findyr2 = 2018;
            } else if (parameter[0].windsource == 10) {
				findyr1 = 2000;
                findyr2 = 2020;
			} else if (parameter[0].windsource == 999) {
				findyr1 = 1;
				findyr2 = 25000;
			} else if (parameter[0].windsource == 998) {
				findyr1 = 1;
				findyr2 = 25000;
			}

            ss.str("");
            ss.clear();

            if ((jahr < findyr2 + 1) && (jahr > findyr1 - 1)) {
                ss << jahr;

                if (parameter[0].windsource == 1) {
                    filename = "input/" + foldername.str() + "/winddata" + ss.str() + "_EraInterim.dat";
                } else if (parameter[0].windsource == 10) {
					filename = "input/" + foldername.str() + "/winddata" + ss.str() + "_ERA5.dat";
				} else if (parameter[0].windsource == 999) {
					filename = "/albedo/work/user/lufarkas/LAVESI_HPC/winddata/" + foldername.str() + "/winddata_" + ss.str() + ".dat";
				} else if (parameter[0].windsource == 998) {
					filename = "/albedo/work/user/lufarkas/LAVESI_HPC/winddata2/" + foldername.str() + "/winddata_" + ss.str() + ".dat";
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
    for (vector<vector<Weather>>::iterator posw = world_weather_list.begin(); posw != world_weather_list.end(); posw++) {
        vector<Weather>& weather_list = *posw;
        aktort++;

        // depending on the weather choice different files will be opened and read line by line
        if (parameter[0].weatherchoice == 1000000001) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/001_temp1.csv"; //x=990, y=990 ### FOR 11-CH-02II ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/001_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000002) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/002_temp1.csv"; //x=990, y=990 ### FOR 11-CH-02III ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/002_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000003) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/003_temp1.csv"; //x=990, y=990 ### FOR 11-CH-06I ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/003_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000004) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/004_temp1.csv"; //x=990, y=990 ### FOR 11-CH-06III ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/004_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000005) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/005_temp1.csv"; //x=990, y=990 ### FOR 11-CH-12I ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/005_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000006) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/006_temp1.csv"; //x=990, y=990 ### FOR 11-CH-12II ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/006_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000007) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/007_temp1.csv"; //x=990, y=990 ### FOR 11-CH-17I ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/007_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000008) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/008_temp1.csv"; //x=990, y=990 ### FOR 11-CH-17II ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/008_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000009) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/009_temp1.csv"; //x=990, y=990 ### FOR 12-KO-02a ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/009_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000010) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/010_temp1.csv"; //x=990, y=990 ### FOR 12-KO-02b ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/010_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000011) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/011_temp1.csv"; //x=990, y=990 ### FOR 12-KO-03a ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/011_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000012) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/012_temp1.csv"; //x=990, y=990 ### FOR 12-KO-03b ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/012_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000013) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/013_temp1.csv"; //x=990, y=990 ### FOR 12-KO-04a ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/013_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000014) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/014_temp1.csv"; //x=990, y=990 ### FOR 12-KO-04b ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/014_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000015) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/015_temp1.csv"; //x=990, y=990 ### FOR 12-KO-05 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/015_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000016) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/016_temp1.csv"; //x=990, y=990 ### FOR 13-TY-02-VI ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/016_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000017) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/017_temp1.csv"; //x=990, y=990 ### FOR 13-TY-02-VII ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/017_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000018) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/018_temp1.csv"; //x=990, y=990 ### FOR 14-OM-02-V1 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/018_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000019) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/019_temp1.csv"; //x=990, y=990 ### FOR 14-OM-02-V2 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/019_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000020) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/020_temp1.csv"; //x=990, y=990 ### FOR 14-OM-11-V3 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/020_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000021) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/021_temp1.csv"; //x=990, y=990 ### FOR 14-OM-20-V4 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/021_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000022) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/022_temp1.csv"; //x=990, y=990 ### FOR 14-OM-TRANS1 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/022_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000023) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/023_temp1.csv"; //x=990, y=990 ### FOR 14-OM-TRANS2 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/023_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000024) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/024_temp1.csv"; //x=990, y=990 ### FOR 14-OM-TRANS3 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/024_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000025) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/025_temp1.csv"; //x=990, y=990 ### FOR 14-OM-TRANS4 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/025_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000026) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/026_temp1.csv"; //x=990, y=990 ### FOR 14-OM-TRANS5 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/026_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000027) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/027_temp1.csv"; //x=990, y=990 ### FOR 14-OM-TRANS6-7 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/027_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000028) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/028_temp1.csv"; //x=990, y=990 ### FOR 14-OM-TRANS6 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/028_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000029) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/029_temp1.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18001 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/029_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000030) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/030_temp1.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18002 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/030_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000031) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/031_temp1.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18003 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/031_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000032) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/032_temp1.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18004 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/032_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000033) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/033_temp1.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18005 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/033_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000034) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/034_temp1.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18006 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/034_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000035) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/035_temp1.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18007 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/035_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000036) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/036_temp1.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18008 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/036_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000037) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/037_temp1.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18009 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/037_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000038) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/038_temp1.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18010 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/038_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000039) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/039_temp1.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18011 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/039_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000040) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/040_temp1.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18012 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/040_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000041) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/041_temp1.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18013 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/041_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000042) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/042_temp1.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18014 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/042_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000043) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/043_temp1.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18015 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/043_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000044) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/044_temp1.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18016 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/044_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000045) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/045_temp1.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18017 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/045_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000046) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/046_temp1.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18018 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/046_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000047) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/047_temp1.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18019 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/047_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000048) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/048_temp1.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18020 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/048_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000049) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/049_temp1.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18021 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/049_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000050) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/050_temp1.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18022 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/050_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000051) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/051_temp1.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18023 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/051_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000052) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/052_temp1.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18024 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/052_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000053) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/053_temp1.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18025 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/053_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000054) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/054_temp1.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18026 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/054_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000055) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/055_temp1.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18027 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/055_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000056) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/056_temp1.csv"; //x=990, y=990 ### FOR 16-KP-04-EN18051 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/056_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000057) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/057_temp1.csv"; //x=990, y=990 ### FOR 16-KP-04-EN18052 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/057_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000058) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/058_temp1.csv"; //x=990, y=990 ### FOR 16-KP-04-EN18053 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/058_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000059) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/059_temp1.csv"; //x=990, y=990 ### FOR 16-KP-04-EN18054 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/059_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000060) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/060_temp1.csv"; //x=990, y=990 ### FOR 16-KP-04-EN18055 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/060_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000061) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/061_temp1.csv"; //x=990, y=990 ### FOR 16-KP-V01 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/061_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000062) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/062_temp1.csv"; //x=990, y=990 ### FOR 16-KP-V02 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/062_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000063) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/063_temp1.csv"; //x=990, y=990 ### FOR 16-KP-V03 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/063_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000064) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/064_temp1.csv"; //x=990, y=990 ### FOR 16-KP-V04 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/064_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000065) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/065_temp1.csv"; //x=990, y=990 ### FOR 16-KP-V05 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/065_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000066) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/066_temp1.csv"; //x=990, y=990 ### FOR 16-KP-V06 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/066_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000067) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/067_temp1.csv"; //x=990, y=990 ### FOR 16-KP-V07 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/067_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000068) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/068_temp1.csv"; //x=990, y=990 ### FOR 16-KP-V08 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/068_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000069) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/069_temp1.csv"; //x=990, y=990 ### FOR 16-KP-V09 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/069_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000070) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/070_temp1.csv"; //x=990, y=990 ### FOR 16-KP-V10 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/070_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000071) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/071_temp1.csv"; //x=990, y=990 ### FOR 16-KP-V11 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/071_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000072) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/072_temp1.csv"; //x=990, y=990 ### FOR 16-KP-V12 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/072_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000073) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/073_temp1.csv"; //x=990, y=990 ### FOR 16-KP-V13 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/073_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000074) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/074_temp1.csv"; //x=990, y=990 ### FOR 16-KP-V14 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/074_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000075) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/075_temp1.csv"; //x=990, y=990 ### FOR 16-KP-V15 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/075_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000076) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/076_temp1.csv"; //x=990, y=990 ### FOR 16-KP-V16 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/076_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000077) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/077_temp1.csv"; //x=990, y=990 ### FOR 16-KP-V17 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/077_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000078) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/078_temp1.csv"; //x=990, y=990 ### FOR 16-KP-V18 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/078_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000079) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/079_temp1.csv"; //x=990, y=990 ### FOR 16-KP-V19 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/079_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000080) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/080_temp1.csv"; //x=990, y=990 ### FOR 16-KP-V20 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/080_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000081) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/081_temp1.csv"; //x=990, y=990 ### FOR 16-KP-V21 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/081_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000082) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/082_temp1.csv"; //x=990, y=990 ### FOR 16-KP-V22 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/082_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000083) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/083_temp1.csv"; //x=990, y=990 ### FOR 16-KP-V23 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/083_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000084) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/084_temp1.csv"; //x=990, y=990 ### FOR 16-KP-V24 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/084_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000085) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/085_temp1.csv"; //x=990, y=990 ### FOR 16-KP-V25 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/085_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000086) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/086_temp1.csv"; //x=990, y=990 ### FOR 16-KP-V26 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/086_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000087) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/087_temp1.csv"; //x=990, y=990 ### FOR 16-KP-V27 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/087_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000088) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/088_temp1.csv"; //x=990, y=990 ### FOR 16-KP-V28 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/088_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000089) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/089_temp1.csv"; //x=990, y=990 ### FOR 16-KP-V29 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/089_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000090) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/090_temp1.csv"; //x=990, y=990 ### FOR 16-KP-V30 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/090_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000091) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/091_temp1.csv"; //x=990, y=990 ### FOR 16-KP-V31 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/091_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000092) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/092_temp1.csv"; //x=990, y=990 ### FOR 16-KP-V32 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/092_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000093) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/093_temp1.csv"; //x=990, y=990 ### FOR 16-KP-V33 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/093_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000094) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/094_temp1.csv"; //x=990, y=990 ### FOR 16-KP-V34 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/094_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000095) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/095_temp1.csv"; //x=990, y=990 ### FOR 16-KP-V35 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/095_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000096) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/096_temp1.csv"; //x=990, y=990 ### FOR 16-KP-V36 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/096_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000097) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/097_temp1.csv"; //x=990, y=990 ### FOR 16-KP-V37 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/097_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000098) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/098_temp1.csv"; //x=990, y=990 ### FOR 16-KP-V38 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/098_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000099) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/099_temp1.csv"; //x=990, y=990 ### FOR 16-KP-V39 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/099_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000100) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/100_temp1.csv"; //x=990, y=990 ### FOR 16-KP-V40 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/100_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000101) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/101_temp1.csv"; //x=990, y=990 ### FOR 16-KP-V41 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/101_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000102) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/102_temp1.csv"; //x=990, y=990 ### FOR 16-KP-V42 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/102_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000103) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/103_temp1.csv"; //x=990, y=990 ### FOR 16-KP-V43 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/103_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000104) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/104_temp1.csv"; //x=990, y=990 ### FOR 16-KP-V44 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/104_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000105) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/105_temp1.csv"; //x=990, y=990 ### FOR 16-KP-V45 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/105_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000106) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/106_temp1.csv"; //x=990, y=990 ### FOR 16-KP-V46 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/106_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000107) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/107_temp1.csv"; //x=990, y=990 ### FOR 16-KP-V47 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/107_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000108) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/108_temp1.csv"; //x=990, y=990 ### FOR 16-KP-V48 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/108_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000109) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/109_temp1.csv"; //x=990, y=990 ### FOR 16-KP-V49 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/109_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000110) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/110_temp1.csv"; //x=990, y=990 ### FOR 16-KP-V50 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/110_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000111) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/111_temp1.csv"; //x=990, y=990 ### FOR 16-KP-V51 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/111_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000112) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/112_temp1.csv"; //x=990, y=990 ### FOR 16-KP-V52 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/112_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000113) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/113_temp1.csv"; //x=990, y=990 ### FOR 16-KP-V53 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/113_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000114) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/114_temp1.csv"; //x=990, y=990 ### FOR 16-KP-V54 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/114_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000115) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/115_temp1.csv"; //x=990, y=990 ### FOR 16-KP-V55 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/115_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000116) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/116_temp1.csv"; //x=990, y=990 ### FOR 16-KP-V56 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/116_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000117) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/117_temp1.csv"; //x=990, y=990 ### FOR 16-KP-V57 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/117_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000118) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/118_temp1.csv"; //x=990, y=990 ### FOR 16-KP-V58 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/118_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000119) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/119_temp1.csv"; //x=990, y=990 ### FOR 18-BIL-00-EN18000 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/119_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000120) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/120_temp1.csv"; //x=990, y=990 ### FOR 18-BIL-01-EN18028 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/120_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000121) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/121_temp1.csv"; //x=990, y=990 ### FOR 18-BIL-01-EN18029 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/121_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000122) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/122_temp1.csv"; //x=990, y=990 ### FOR 18-BIL-02-EN18030 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/122_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000123) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/123_temp1.csv"; //x=990, y=990 ### FOR 18-BIL-02-EN18031 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/123_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000124) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/124_temp1.csv"; //x=990, y=990 ### FOR 18-BIL-02-EN18032 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/124_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000125) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/125_temp1.csv"; //x=990, y=990 ### FOR 18-BIL-02-EN18033 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/125_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000126) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/126_temp1.csv"; //x=990, y=990 ### FOR 18-BIL-02-EN18034 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/126_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000127) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/127_temp1.csv"; //x=990, y=990 ### FOR 18-BIL-02-EN18035 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/127_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000128) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/128_temp1.csv"; //x=990, y=990 ### FOR 18-LD-VP012-Tit-Ary ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/128_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000129) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/129_temp1.csv"; //x=990, y=990 ### FOR B19-T1 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/129_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000130) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/130_temp1.csv"; //x=990, y=990 ### FOR B19-T2 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/130_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000131) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/131_temp1.csv"; //x=990, y=990 ### FOR EN18061 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/131_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000132) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/132_temp1.csv"; //x=990, y=990 ### FOR EN18062 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/132_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000133) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/133_temp1.csv"; //x=990, y=990 ### FOR EN18063 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/133_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000134) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/134_temp1.csv"; //x=990, y=990 ### FOR EN18064 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/134_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000135) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/135_temp1.csv"; //x=990, y=990 ### FOR EN18065 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/135_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000136) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/136_temp1.csv"; //x=990, y=990 ### FOR EN18066 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/136_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000137) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/137_temp1.csv"; //x=990, y=990 ### FOR EN18067 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/137_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000138) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/138_temp1.csv"; //x=990, y=990 ### FOR EN18068 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/138_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000139) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/139_temp1.csv"; //x=990, y=990 ### FOR EN18069 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/139_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000140) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/140_temp1.csv"; //x=990, y=990 ### FOR EN18070_centre ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/140_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000141) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/141_temp1.csv"; //x=990, y=990 ### FOR EN18070_edge ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/141_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000142) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/142_temp1.csv"; //x=990, y=990 ### FOR EN18070_end ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/142_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000143) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/143_temp1.csv"; //x=990, y=990 ### FOR EN18070_transition ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/143_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000144) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/144_temp1.csv"; //x=990, y=990 ### FOR EN18071 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/144_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000145) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/145_temp1.csv"; //x=990, y=990 ### FOR EN18072 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/145_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000146) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/146_temp1.csv"; //x=990, y=990 ### FOR EN18073 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/146_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000147) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/147_temp1.csv"; //x=990, y=990 ### FOR EN18074 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/147_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000148) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/148_temp1.csv"; //x=990, y=990 ### FOR EN18075 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/148_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000149) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/149_temp1.csv"; //x=990, y=990 ### FOR EN18076 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/149_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000150) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/150_temp1.csv"; //x=990, y=990 ### FOR EN18077 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/150_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000151) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/151_temp1.csv"; //x=990, y=990 ### FOR EN18078 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/151_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000152) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/152_temp1.csv"; //x=990, y=990 ### FOR EN18079 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/152_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000153) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/153_temp1.csv"; //x=990, y=990 ### FOR EN18080 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/153_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000154) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/154_temp1.csv"; //x=990, y=990 ### FOR EN18081 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/154_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000155) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/155_temp1.csv"; //x=990, y=990 ### FOR EN18082 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/155_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000156) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/156_temp1.csv"; //x=990, y=990 ### FOR EN18083 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/156_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000157) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/157_temp1.csv"; //x=990, y=990 ### FOR EN21201 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/157_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000158) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/158_temp1.csv"; //x=990, y=990 ### FOR EN21202 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/158_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000159) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/159_temp1.csv"; //x=990, y=990 ### FOR EN21203 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/159_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000160) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/160_temp1.csv"; //x=990, y=990 ### FOR EN21204 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/160_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000161) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/161_temp1.csv"; //x=990, y=990 ### FOR EN21205 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/161_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000162) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/162_temp1.csv"; //x=990, y=990 ### FOR EN21206 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/162_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000163) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/163_temp1.csv"; //x=990, y=990 ### FOR EN21207 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/163_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000164) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/164_temp1.csv"; //x=990, y=990 ### FOR EN21208 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/164_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000165) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/165_temp1.csv"; //x=990, y=990 ### FOR EN21209 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/165_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000166) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/166_temp1.csv"; //x=990, y=990 ### FOR EN21210 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/166_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000167) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/167_temp1.csv"; //x=990, y=990 ### FOR EN21211 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/167_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000168) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/168_temp1.csv"; //x=990, y=990 ### FOR EN21212 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/168_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000169) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/169_temp1.csv"; //x=990, y=990 ### FOR EN21213 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/169_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000170) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/170_temp1.csv"; //x=990, y=990 ### FOR EN21214 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/170_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000171) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/171_temp1.csv"; //x=990, y=990 ### FOR EN21215 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/171_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000172) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/172_temp1.csv"; //x=990, y=990 ### FOR EN21216 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/172_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000173) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/173_temp1.csv"; //x=990, y=990 ### FOR EN21217 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/173_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000174) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/174_temp1.csv"; //x=990, y=990 ### FOR EN21218 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/174_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000175) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/175_temp1.csv"; //x=990, y=990 ### FOR EN21219 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/175_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000176) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/176_temp1.csv"; //x=990, y=990 ### FOR EN21220 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/176_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000177) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/177_temp1.csv"; //x=990, y=990 ### FOR EN21221 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/177_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000178) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/178_temp1.csv"; //x=990, y=990 ### FOR EN21222 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/178_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000179) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/179_temp1.csv"; //x=990, y=990 ### FOR EN21223 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/179_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000180) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/180_temp1.csv"; //x=990, y=990 ### FOR EN21224 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/180_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000181) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/181_temp1.csv"; //x=990, y=990 ### FOR EN21225 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/181_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000182) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/182_temp1.csv"; //x=990, y=990 ### FOR EN21226 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/182_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000183) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/183_temp1.csv"; //x=990, y=990 ### FOR EN21227 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/183_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000184) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/184_temp1.csv"; //x=990, y=990 ### FOR EN21228 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/184_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000185) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/185_temp1.csv"; //x=990, y=990 ### FOR EN21229 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/185_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000186) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/186_temp1.csv"; //x=990, y=990 ### FOR EN21230 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/186_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000187) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/187_temp1.csv"; //x=990, y=990 ### FOR EN21231 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/187_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000188) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/188_temp1.csv"; //x=990, y=990 ### FOR EN21232 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/188_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000189) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/189_temp1.csv"; //x=990, y=990 ### FOR EN21233 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/189_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000190) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/190_temp1.csv"; //x=990, y=990 ### FOR EN21234 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/190_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000191) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/191_temp1.csv"; //x=990, y=990 ### FOR EN21235 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/191_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000192) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/192_temp1.csv"; //x=990, y=990 ### FOR EN21236 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/192_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000193) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/193_temp1.csv"; //x=990, y=990 ### FOR EN21237 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/193_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000194) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/194_temp1.csv"; //x=990, y=990 ### FOR EN21238 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/194_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000195) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/195_temp1.csv"; //x=990, y=990 ### FOR EN21239 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/195_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000196) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/196_temp1.csv"; //x=990, y=990 ### FOR EN21240 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/196_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000197) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/197_temp1.csv"; //x=990, y=990 ### FOR EN21241 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/197_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000198) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/198_temp1.csv"; //x=990, y=990 ### FOR EN21242 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/198_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000199) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/199_temp1.csv"; //x=990, y=990 ### FOR EN21243 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/199_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000200) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/200_temp1.csv"; //x=990, y=990 ### FOR EN21244 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/200_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000201) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/201_temp1.csv"; //x=990, y=990 ### FOR EN21245 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/201_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000202) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/202_temp1.csv"; //x=990, y=990 ### FOR EN21246 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/202_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000203) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/203_temp1.csv"; //x=990, y=990 ### FOR EN21247 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/203_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000204) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/204_temp1.csv"; //x=990, y=990 ### FOR EN21248 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/204_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000205) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/205_temp1.csv"; //x=990, y=990 ### FOR EN21249 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/205_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000206) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/206_temp1.csv"; //x=990, y=990 ### FOR EN21250 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/206_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000207) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/207_temp1.csv"; //x=990, y=990 ### FOR EN21251 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/207_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000208) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/208_temp1.csv"; //x=990, y=990 ### FOR EN21252 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/208_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000209) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/209_temp1.csv"; //x=990, y=990 ### FOR EN21253 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/209_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000210) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/210_temp1.csv"; //x=990, y=990 ### FOR EN21254 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/210_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000211) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/211_temp1.csv"; //x=990, y=990 ### FOR EN21255 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/211_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000212) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/212_temp1.csv"; //x=990, y=990 ### FOR EN21256 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/212_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000213) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/213_temp1.csv"; //x=990, y=990 ### FOR EN21257 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/213_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000214) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/214_temp1.csv"; //x=990, y=990 ### FOR EN21258 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/214_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000215) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/215_temp1.csv"; //x=990, y=990 ### FOR EN21259 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/215_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000216) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/216_temp1.csv"; //x=990, y=990 ### FOR EN21260 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/216_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000217) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/217_temp1.csv"; //x=990, y=990 ### FOR EN21261 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/217_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000218) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/218_temp1.csv"; //x=990, y=990 ### FOR EN21262 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/218_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000219) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/219_temp1.csv"; //x=990, y=990 ### FOR EN22-001 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/219_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000220) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/220_temp1.csv"; //x=990, y=990 ### FOR EN22-002 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/220_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000221) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/221_temp1.csv"; //x=990, y=990 ### FOR EN22-003 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/221_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000222) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/222_temp1.csv"; //x=990, y=990 ### FOR EN22-004 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/222_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000223) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/223_temp1.csv"; //x=990, y=990 ### FOR EN22-005 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/223_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000224) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/224_temp1.csv"; //x=990, y=990 ### FOR EN22-006 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/224_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000225) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/225_temp1.csv"; //x=990, y=990 ### FOR EN22-007 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/225_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000226) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/226_temp1.csv"; //x=990, y=990 ### FOR EN22-008 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/226_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000227) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/227_temp1.csv"; //x=990, y=990 ### FOR EN22-009 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/227_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000228) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/228_temp1.csv"; //x=990, y=990 ### FOR EN22-010 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/228_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000229) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/229_temp1.csv"; //x=990, y=990 ### FOR EN22-011 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/229_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000230) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/230_temp1.csv"; //x=990, y=990 ### FOR EN22-012 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/230_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000231) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/231_temp1.csv"; //x=990, y=990 ### FOR EN22-013 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/231_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000232) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/232_temp1.csv"; //x=990, y=990 ### FOR EN22-014 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/232_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000233) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/233_temp1.csv"; //x=990, y=990 ### FOR EN22-015 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/233_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000234) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/234_temp1.csv"; //x=990, y=990 ### FOR EN22-016 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/234_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000235) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/235_temp1.csv"; //x=990, y=990 ### FOR EN22-017 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/235_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000236) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/236_temp1.csv"; //x=990, y=990 ### FOR EN22-018 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/236_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000237) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/237_temp1.csv"; //x=990, y=990 ### FOR EN22-019 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/237_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000238) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/238_temp1.csv"; //x=990, y=990 ### FOR EN22-020 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/238_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000239) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/239_temp1.csv"; //x=990, y=990 ### FOR EN22-021 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/239_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000240) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/240_temp1.csv"; //x=990, y=990 ### FOR EN22-022 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/240_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000241) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/241_temp1.csv"; //x=990, y=990 ### FOR EN22-023 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/241_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000242) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/242_temp1.csv"; //x=990, y=990 ### FOR EN22-024 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/242_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000243) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/243_temp1.csv"; //x=990, y=990 ### FOR EN22-025 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/243_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000244) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/244_temp1.csv"; //x=990, y=990 ### FOR EN22-026 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/244_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000245) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/245_temp1.csv"; //x=990, y=990 ### FOR EN22-027 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/245_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000246) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/246_temp1.csv"; //x=990, y=990 ### FOR EN22-028 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/246_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000247) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/247_temp1.csv"; //x=990, y=990 ### FOR EN22-029 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/247_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000248) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/248_temp1.csv"; //x=990, y=990 ### FOR EN22-030 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/248_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000249) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/249_temp1.csv"; //x=990, y=990 ### FOR EN22-031 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/249_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000250) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/250_temp1.csv"; //x=990, y=990 ### FOR EN22-032 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/250_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000251) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/251_temp1.csv"; //x=990, y=990 ### FOR EN22-033 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/251_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000252) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/252_temp1.csv"; //x=990, y=990 ### FOR EN22-034 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/252_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000253) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/253_temp1.csv"; //x=990, y=990 ### FOR EN22-035 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/253_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000254) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/254_temp1.csv"; //x=990, y=990 ### FOR EN22-036 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/254_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000255) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/255_temp1.csv"; //x=990, y=990 ### FOR EN22-037 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/255_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000256) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/256_temp1.csv"; //x=990, y=990 ### FOR EN22-038 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/256_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000257) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/257_temp1.csv"; //x=990, y=990 ### FOR EN22-039 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/257_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000258) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/258_temp1.csv"; //x=990, y=990 ### FOR EN22-040 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/258_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000259) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/259_temp1.csv"; //x=990, y=990 ### FOR EN22-041 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/259_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000260) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/260_temp1.csv"; //x=990, y=990 ### FOR EN22-042 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/260_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000261) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/261_temp1.csv"; //x=990, y=990 ### FOR EN22-043 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/261_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000262) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/262_temp1.csv"; //x=990, y=990 ### FOR EN22-044 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/262_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000263) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/263_temp1.csv"; //x=990, y=990 ### FOR EN22-045 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/263_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000264) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/264_temp1.csv"; //x=990, y=990 ### FOR EN22-046 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/264_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000265) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/265_temp1.csv"; //x=990, y=990 ### FOR EN22-047 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/265_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000266) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/266_temp1.csv"; //x=990, y=990 ### FOR EN22-048 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/266_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000267) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/267_temp1.csv"; //x=990, y=990 ### FOR EN22-049 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/267_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000268) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/268_temp1.csv"; //x=990, y=990 ### FOR EN22-050 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/268_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000269) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/269_temp1.csv"; //x=990, y=990 ### FOR EN22-051 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/269_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000270) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/270_temp1.csv"; //x=990, y=990 ### FOR EN22-052 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/270_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000271) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/271_temp1.csv"; //x=990, y=990 ### FOR EN22-053 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/271_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000272) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/272_temp1.csv"; //x=990, y=990 ### FOR EN22-054 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/272_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000273) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/273_temp1.csv"; //x=990, y=990 ### FOR EN22-055 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/273_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000274) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/274_temp1.csv"; //x=990, y=990 ### FOR EN22-056 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/274_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000275) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/275_temp1.csv"; //x=990, y=990 ### FOR EN22-057 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/275_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000276) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/276_temp1.csv"; //x=990, y=990 ### FOR EN22-058 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/276_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000277) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/277_temp1.csv"; //x=990, y=990 ### FOR EN22-059 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/277_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000278) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/278_temp1.csv"; //x=990, y=990 ### FOR EN22-060 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/278_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000279) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/279_temp1.csv"; //x=990, y=990 ### FOR EN22-061 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/279_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000280) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/280_temp1.csv"; //x=990, y=990 ### FOR EN22-062 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/280_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000281) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/281_temp1.csv"; //x=990, y=990 ### FOR EN22-063 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/281_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000282) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/282_temp1.csv"; //x=990, y=990 ### FOR EN22-064 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/282_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000283) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/283_temp1.csv"; //x=990, y=990 ### FOR EN22-065 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/283_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000284) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/284_temp1.csv"; //x=990, y=990 ### FOR EN22-066 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/284_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000285) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/285_temp1.csv"; //x=990, y=990 ### FOR EN22-067 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/285_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000286) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/286_temp1.csv"; //x=990, y=990 ### FOR EN22-068 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/286_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000287) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/287_temp1.csv"; //x=990, y=990 ### FOR EN22-069 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/287_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000288) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/288_temp1.csv"; //x=990, y=990 ### FOR EN22-070 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/288_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000289) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/289_temp1.csv"; //x=990, y=990 ### FOR EN22-071 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/289_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000290) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/290_temp1.csv"; //x=990, y=990 ### FOR EN22-072 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/290_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000291) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/291_temp1.csv"; //x=990, y=990 ### FOR EN22-073 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/291_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000292) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/292_temp1.csv"; //x=990, y=990 ### FOR EN22-074 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/292_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000293) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/293_temp1.csv"; //x=990, y=990 ### FOR EN22-075 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/293_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000294) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/294_temp1.csv"; //x=990, y=990 ### FOR TY04VI ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/294_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000295) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/295_temp1.csv"; //x=990, y=990 ### FOR TY04VII ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/295_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000296) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/296_temp1.csv"; //x=990, y=990 ### FOR TY07VI ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/296_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000297) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/297_temp1.csv"; //x=990, y=990 ### FOR TY07VII ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/297_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000298) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/298_temp1.csv"; //x=990, y=990 ### FOR TY09VI ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/298_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 1000000299) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/299_temp1.csv"; //x=990, y=990 ### FOR TY09VII ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/fromYearlyData/299_prec1.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000001) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/001_temp2.csv"; //x=990, y=990 ### FOR 11-CH-02II ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/001_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000002) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/002_temp2.csv"; //x=990, y=990 ### FOR 11-CH-02III ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/002_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000003) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/003_temp2.csv"; //x=990, y=990 ### FOR 11-CH-06I ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/003_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000004) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/004_temp2.csv"; //x=990, y=990 ### FOR 11-CH-06III ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/004_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000005) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/005_temp2.csv"; //x=990, y=990 ### FOR 11-CH-12I ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/005_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000006) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/006_temp2.csv"; //x=990, y=990 ### FOR 11-CH-12II ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/006_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000007) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/007_temp2.csv"; //x=990, y=990 ### FOR 11-CH-17I ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/007_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000008) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/008_temp2.csv"; //x=990, y=990 ### FOR 11-CH-17II ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/008_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000009) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/009_temp2.csv"; //x=990, y=990 ### FOR 12-KO-02a ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/009_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000010) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/010_temp2.csv"; //x=990, y=990 ### FOR 12-KO-02b ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/010_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000011) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/011_temp2.csv"; //x=990, y=990 ### FOR 12-KO-03a ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/011_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000012) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/012_temp2.csv"; //x=990, y=990 ### FOR 12-KO-03b ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/012_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000013) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/013_temp2.csv"; //x=990, y=990 ### FOR 12-KO-04a ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/013_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000014) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/014_temp2.csv"; //x=990, y=990 ### FOR 12-KO-04b ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/014_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000015) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/015_temp2.csv"; //x=990, y=990 ### FOR 12-KO-05 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/015_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000016) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/016_temp2.csv"; //x=990, y=990 ### FOR 13-TY-02-VI ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/016_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000017) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/017_temp2.csv"; //x=990, y=990 ### FOR 13-TY-02-VII ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/017_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000018) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/018_temp2.csv"; //x=990, y=990 ### FOR 14-OM-02-V1 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/018_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000019) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/019_temp2.csv"; //x=990, y=990 ### FOR 14-OM-02-V2 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/019_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000020) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/020_temp2.csv"; //x=990, y=990 ### FOR 14-OM-11-V3 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/020_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000021) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/021_temp2.csv"; //x=990, y=990 ### FOR 14-OM-20-V4 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/021_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000022) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/022_temp2.csv"; //x=990, y=990 ### FOR 14-OM-TRANS1 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/022_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000023) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/023_temp2.csv"; //x=990, y=990 ### FOR 14-OM-TRANS2 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/023_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000024) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/024_temp2.csv"; //x=990, y=990 ### FOR 14-OM-TRANS3 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/024_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000025) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/025_temp2.csv"; //x=990, y=990 ### FOR 14-OM-TRANS4 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/025_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000026) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/026_temp2.csv"; //x=990, y=990 ### FOR 14-OM-TRANS5 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/026_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000027) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/027_temp2.csv"; //x=990, y=990 ### FOR 14-OM-TRANS6-7 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/027_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000028) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/028_temp2.csv"; //x=990, y=990 ### FOR 14-OM-TRANS6 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/028_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000029) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/029_temp2.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18001 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/029_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000030) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/030_temp2.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18002 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/030_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000031) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/031_temp2.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18003 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/031_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000032) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/032_temp2.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18004 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/032_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000033) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/033_temp2.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18005 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/033_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000034) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/034_temp2.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18006 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/034_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000035) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/035_temp2.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18007 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/035_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000036) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/036_temp2.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18008 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/036_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000037) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/037_temp2.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18009 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/037_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000038) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/038_temp2.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18010 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/038_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000039) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/039_temp2.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18011 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/039_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000040) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/040_temp2.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18012 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/040_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000041) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/041_temp2.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18013 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/041_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000042) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/042_temp2.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18014 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/042_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000043) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/043_temp2.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18015 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/043_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000044) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/044_temp2.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18016 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/044_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000045) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/045_temp2.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18017 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/045_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000046) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/046_temp2.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18018 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/046_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000047) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/047_temp2.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18019 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/047_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000048) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/048_temp2.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18020 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/048_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000049) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/049_temp2.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18021 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/049_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000050) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/050_temp2.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18022 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/050_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000051) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/051_temp2.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18023 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/051_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000052) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/052_temp2.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18024 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/052_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000053) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/053_temp2.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18025 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/053_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000054) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/054_temp2.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18026 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/054_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000055) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/055_temp2.csv"; //x=990, y=990 ### FOR 16-KP-01-EN18027 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/055_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000056) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/056_temp2.csv"; //x=990, y=990 ### FOR 16-KP-04-EN18051 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/056_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000057) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/057_temp2.csv"; //x=990, y=990 ### FOR 16-KP-04-EN18052 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/057_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000058) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/058_temp2.csv"; //x=990, y=990 ### FOR 16-KP-04-EN18053 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/058_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000059) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/059_temp2.csv"; //x=990, y=990 ### FOR 16-KP-04-EN18054 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/059_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000060) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/060_temp2.csv"; //x=990, y=990 ### FOR 16-KP-04-EN18055 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/060_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000061) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/061_temp2.csv"; //x=990, y=990 ### FOR 16-KP-V01 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/061_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000062) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/062_temp2.csv"; //x=990, y=990 ### FOR 16-KP-V02 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/062_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000063) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/063_temp2.csv"; //x=990, y=990 ### FOR 16-KP-V03 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/063_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000064) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/064_temp2.csv"; //x=990, y=990 ### FOR 16-KP-V04 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/064_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000065) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/065_temp2.csv"; //x=990, y=990 ### FOR 16-KP-V05 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/065_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000066) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/066_temp2.csv"; //x=990, y=990 ### FOR 16-KP-V06 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/066_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000067) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/067_temp2.csv"; //x=990, y=990 ### FOR 16-KP-V07 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/067_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000068) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/068_temp2.csv"; //x=990, y=990 ### FOR 16-KP-V08 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/068_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000069) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/069_temp2.csv"; //x=990, y=990 ### FOR 16-KP-V09 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/069_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000070) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/070_temp2.csv"; //x=990, y=990 ### FOR 16-KP-V10 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/070_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000071) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/071_temp2.csv"; //x=990, y=990 ### FOR 16-KP-V11 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/071_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000072) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/072_temp2.csv"; //x=990, y=990 ### FOR 16-KP-V12 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/072_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000073) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/073_temp2.csv"; //x=990, y=990 ### FOR 16-KP-V13 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/073_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000074) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/074_temp2.csv"; //x=990, y=990 ### FOR 16-KP-V14 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/074_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000075) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/075_temp2.csv"; //x=990, y=990 ### FOR 16-KP-V15 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/075_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000076) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/076_temp2.csv"; //x=990, y=990 ### FOR 16-KP-V16 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/076_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000077) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/077_temp2.csv"; //x=990, y=990 ### FOR 16-KP-V17 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/077_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000078) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/078_temp2.csv"; //x=990, y=990 ### FOR 16-KP-V18 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/078_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000079) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/079_temp2.csv"; //x=990, y=990 ### FOR 16-KP-V19 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/079_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000080) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/080_temp2.csv"; //x=990, y=990 ### FOR 16-KP-V20 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/080_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000081) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/081_temp2.csv"; //x=990, y=990 ### FOR 16-KP-V21 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/081_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000082) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/082_temp2.csv"; //x=990, y=990 ### FOR 16-KP-V22 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/082_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000083) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/083_temp2.csv"; //x=990, y=990 ### FOR 16-KP-V23 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/083_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000084) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/084_temp2.csv"; //x=990, y=990 ### FOR 16-KP-V24 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/084_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000085) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/085_temp2.csv"; //x=990, y=990 ### FOR 16-KP-V25 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/085_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000086) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/086_temp2.csv"; //x=990, y=990 ### FOR 16-KP-V26 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/086_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000087) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/087_temp2.csv"; //x=990, y=990 ### FOR 16-KP-V27 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/087_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000088) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/088_temp2.csv"; //x=990, y=990 ### FOR 16-KP-V28 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/088_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000089) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/089_temp2.csv"; //x=990, y=990 ### FOR 16-KP-V29 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/089_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000090) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/090_temp2.csv"; //x=990, y=990 ### FOR 16-KP-V30 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/090_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000091) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/091_temp2.csv"; //x=990, y=990 ### FOR 16-KP-V31 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/091_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000092) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/092_temp2.csv"; //x=990, y=990 ### FOR 16-KP-V32 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/092_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000093) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/093_temp2.csv"; //x=990, y=990 ### FOR 16-KP-V33 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/093_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000094) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/094_temp2.csv"; //x=990, y=990 ### FOR 16-KP-V34 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/094_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000095) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/095_temp2.csv"; //x=990, y=990 ### FOR 16-KP-V35 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/095_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000096) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/096_temp2.csv"; //x=990, y=990 ### FOR 16-KP-V36 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/096_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000097) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/097_temp2.csv"; //x=990, y=990 ### FOR 16-KP-V37 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/097_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000098) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/098_temp2.csv"; //x=990, y=990 ### FOR 16-KP-V38 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/098_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000099) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/099_temp2.csv"; //x=990, y=990 ### FOR 16-KP-V39 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/099_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000100) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/100_temp2.csv"; //x=990, y=990 ### FOR 16-KP-V40 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/100_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000101) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/101_temp2.csv"; //x=990, y=990 ### FOR 16-KP-V41 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/101_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000102) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/102_temp2.csv"; //x=990, y=990 ### FOR 16-KP-V42 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/102_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000103) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/103_temp2.csv"; //x=990, y=990 ### FOR 16-KP-V43 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/103_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000104) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/104_temp2.csv"; //x=990, y=990 ### FOR 16-KP-V44 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/104_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000105) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/105_temp2.csv"; //x=990, y=990 ### FOR 16-KP-V45 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/105_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000106) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/106_temp2.csv"; //x=990, y=990 ### FOR 16-KP-V46 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/106_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000107) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/107_temp2.csv"; //x=990, y=990 ### FOR 16-KP-V47 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/107_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000108) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/108_temp2.csv"; //x=990, y=990 ### FOR 16-KP-V48 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/108_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000109) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/109_temp2.csv"; //x=990, y=990 ### FOR 16-KP-V49 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/109_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000110) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/110_temp2.csv"; //x=990, y=990 ### FOR 16-KP-V50 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/110_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000111) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/111_temp2.csv"; //x=990, y=990 ### FOR 16-KP-V51 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/111_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000112) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/112_temp2.csv"; //x=990, y=990 ### FOR 16-KP-V52 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/112_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000113) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/113_temp2.csv"; //x=990, y=990 ### FOR 16-KP-V53 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/113_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000114) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/114_temp2.csv"; //x=990, y=990 ### FOR 16-KP-V54 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/114_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000115) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/115_temp2.csv"; //x=990, y=990 ### FOR 16-KP-V55 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/115_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000116) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/116_temp2.csv"; //x=990, y=990 ### FOR 16-KP-V56 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/116_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000117) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/117_temp2.csv"; //x=990, y=990 ### FOR 16-KP-V57 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/117_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000118) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/118_temp2.csv"; //x=990, y=990 ### FOR 16-KP-V58 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/118_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000119) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/119_temp2.csv"; //x=990, y=990 ### FOR 18-BIL-00-EN18000 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/119_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000120) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/120_temp2.csv"; //x=990, y=990 ### FOR 18-BIL-01-EN18028 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/120_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000121) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/121_temp2.csv"; //x=990, y=990 ### FOR 18-BIL-01-EN18029 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/121_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000122) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/122_temp2.csv"; //x=990, y=990 ### FOR 18-BIL-02-EN18030 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/122_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000123) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/123_temp2.csv"; //x=990, y=990 ### FOR 18-BIL-02-EN18031 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/123_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000124) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/124_temp2.csv"; //x=990, y=990 ### FOR 18-BIL-02-EN18032 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/124_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000125) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/125_temp2.csv"; //x=990, y=990 ### FOR 18-BIL-02-EN18033 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/125_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000126) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/126_temp2.csv"; //x=990, y=990 ### FOR 18-BIL-02-EN18034 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/126_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000127) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/127_temp2.csv"; //x=990, y=990 ### FOR 18-BIL-02-EN18035 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/127_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000128) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/128_temp2.csv"; //x=990, y=990 ### FOR 18-LD-VP012-Tit-Ary ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/128_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000129) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/129_temp2.csv"; //x=990, y=990 ### FOR B19-T1 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/129_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000130) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/130_temp2.csv"; //x=990, y=990 ### FOR B19-T2 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/130_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000131) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/131_temp2.csv"; //x=990, y=990 ### FOR EN18061 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/131_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000132) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/132_temp2.csv"; //x=990, y=990 ### FOR EN18062 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/132_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000133) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/133_temp2.csv"; //x=990, y=990 ### FOR EN18063 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/133_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000134) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/134_temp2.csv"; //x=990, y=990 ### FOR EN18064 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/134_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000135) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/135_temp2.csv"; //x=990, y=990 ### FOR EN18065 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/135_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000136) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/136_temp2.csv"; //x=990, y=990 ### FOR EN18066 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/136_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000137) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/137_temp2.csv"; //x=990, y=990 ### FOR EN18067 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/137_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000138) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/138_temp2.csv"; //x=990, y=990 ### FOR EN18068 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/138_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000139) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/139_temp2.csv"; //x=990, y=990 ### FOR EN18069 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/139_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000140) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/140_temp2.csv"; //x=990, y=990 ### FOR EN18070_centre ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/140_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000141) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/141_temp2.csv"; //x=990, y=990 ### FOR EN18070_edge ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/141_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000142) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/142_temp2.csv"; //x=990, y=990 ### FOR EN18070_end ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/142_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000143) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/143_temp2.csv"; //x=990, y=990 ### FOR EN18070_transition ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/143_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000144) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/144_temp2.csv"; //x=990, y=990 ### FOR EN18071 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/144_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000145) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/145_temp2.csv"; //x=990, y=990 ### FOR EN18072 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/145_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000146) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/146_temp2.csv"; //x=990, y=990 ### FOR EN18073 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/146_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000147) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/147_temp2.csv"; //x=990, y=990 ### FOR EN18074 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/147_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000148) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/148_temp2.csv"; //x=990, y=990 ### FOR EN18075 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/148_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000149) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/149_temp2.csv"; //x=990, y=990 ### FOR EN18076 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/149_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000150) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/150_temp2.csv"; //x=990, y=990 ### FOR EN18077 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/150_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000151) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/151_temp2.csv"; //x=990, y=990 ### FOR EN18078 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/151_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000152) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/152_temp2.csv"; //x=990, y=990 ### FOR EN18079 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/152_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000153) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/153_temp2.csv"; //x=990, y=990 ### FOR EN18080 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/153_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000154) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/154_temp2.csv"; //x=990, y=990 ### FOR EN18081 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/154_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000155) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/155_temp2.csv"; //x=990, y=990 ### FOR EN18082 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/155_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000156) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/156_temp2.csv"; //x=990, y=990 ### FOR EN18083 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/156_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000157) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/157_temp2.csv"; //x=990, y=990 ### FOR EN21201 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/157_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000158) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/158_temp2.csv"; //x=990, y=990 ### FOR EN21202 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/158_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000159) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/159_temp2.csv"; //x=990, y=990 ### FOR EN21203 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/159_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000160) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/160_temp2.csv"; //x=990, y=990 ### FOR EN21204 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/160_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000161) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/161_temp2.csv"; //x=990, y=990 ### FOR EN21205 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/161_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000162) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/162_temp2.csv"; //x=990, y=990 ### FOR EN21206 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/162_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000163) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/163_temp2.csv"; //x=990, y=990 ### FOR EN21207 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/163_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000164) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/164_temp2.csv"; //x=990, y=990 ### FOR EN21208 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/164_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000165) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/165_temp2.csv"; //x=990, y=990 ### FOR EN21209 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/165_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000166) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/166_temp2.csv"; //x=990, y=990 ### FOR EN21210 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/166_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000167) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/167_temp2.csv"; //x=990, y=990 ### FOR EN21211 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/167_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000168) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/168_temp2.csv"; //x=990, y=990 ### FOR EN21212 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/168_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000169) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/169_temp2.csv"; //x=990, y=990 ### FOR EN21213 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/169_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000170) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/170_temp2.csv"; //x=990, y=990 ### FOR EN21214 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/170_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000171) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/171_temp2.csv"; //x=990, y=990 ### FOR EN21215 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/171_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000172) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/172_temp2.csv"; //x=990, y=990 ### FOR EN21216 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/172_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000173) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/173_temp2.csv"; //x=990, y=990 ### FOR EN21217 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/173_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000174) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/174_temp2.csv"; //x=990, y=990 ### FOR EN21218 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/174_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000175) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/175_temp2.csv"; //x=990, y=990 ### FOR EN21219 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/175_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000176) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/176_temp2.csv"; //x=990, y=990 ### FOR EN21220 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/176_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000177) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/177_temp2.csv"; //x=990, y=990 ### FOR EN21221 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/177_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000178) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/178_temp2.csv"; //x=990, y=990 ### FOR EN21222 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/178_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000179) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/179_temp2.csv"; //x=990, y=990 ### FOR EN21223 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/179_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000180) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/180_temp2.csv"; //x=990, y=990 ### FOR EN21224 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/180_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000181) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/181_temp2.csv"; //x=990, y=990 ### FOR EN21225 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/181_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000182) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/182_temp2.csv"; //x=990, y=990 ### FOR EN21226 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/182_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000183) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/183_temp2.csv"; //x=990, y=990 ### FOR EN21227 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/183_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000184) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/184_temp2.csv"; //x=990, y=990 ### FOR EN21228 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/184_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000185) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/185_temp2.csv"; //x=990, y=990 ### FOR EN21229 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/185_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000186) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/186_temp2.csv"; //x=990, y=990 ### FOR EN21230 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/186_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000187) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/187_temp2.csv"; //x=990, y=990 ### FOR EN21231 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/187_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000188) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/188_temp2.csv"; //x=990, y=990 ### FOR EN21232 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/188_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000189) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/189_temp2.csv"; //x=990, y=990 ### FOR EN21233 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/189_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000190) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/190_temp2.csv"; //x=990, y=990 ### FOR EN21234 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/190_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000191) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/191_temp2.csv"; //x=990, y=990 ### FOR EN21235 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/191_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000192) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/192_temp2.csv"; //x=990, y=990 ### FOR EN21236 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/192_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000193) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/193_temp2.csv"; //x=990, y=990 ### FOR EN21237 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/193_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000194) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/194_temp2.csv"; //x=990, y=990 ### FOR EN21238 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/194_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000195) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/195_temp2.csv"; //x=990, y=990 ### FOR EN21239 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/195_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000196) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/196_temp2.csv"; //x=990, y=990 ### FOR EN21240 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/196_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000197) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/197_temp2.csv"; //x=990, y=990 ### FOR EN21241 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/197_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000198) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/198_temp2.csv"; //x=990, y=990 ### FOR EN21242 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/198_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000199) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/199_temp2.csv"; //x=990, y=990 ### FOR EN21243 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/199_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000200) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/200_temp2.csv"; //x=990, y=990 ### FOR EN21244 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/200_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000201) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/201_temp2.csv"; //x=990, y=990 ### FOR EN21245 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/201_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000202) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/202_temp2.csv"; //x=990, y=990 ### FOR EN21246 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/202_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000203) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/203_temp2.csv"; //x=990, y=990 ### FOR EN21247 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/203_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000204) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/204_temp2.csv"; //x=990, y=990 ### FOR EN21248 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/204_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000205) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/205_temp2.csv"; //x=990, y=990 ### FOR EN21249 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/205_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000206) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/206_temp2.csv"; //x=990, y=990 ### FOR EN21250 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/206_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000207) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/207_temp2.csv"; //x=990, y=990 ### FOR EN21251 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/207_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000208) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/208_temp2.csv"; //x=990, y=990 ### FOR EN21252 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/208_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000209) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/209_temp2.csv"; //x=990, y=990 ### FOR EN21253 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/209_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000210) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/210_temp2.csv"; //x=990, y=990 ### FOR EN21254 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/210_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000211) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/211_temp2.csv"; //x=990, y=990 ### FOR EN21255 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/211_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000212) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/212_temp2.csv"; //x=990, y=990 ### FOR EN21256 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/212_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000213) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/213_temp2.csv"; //x=990, y=990 ### FOR EN21257 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/213_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000214) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/214_temp2.csv"; //x=990, y=990 ### FOR EN21258 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/214_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000215) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/215_temp2.csv"; //x=990, y=990 ### FOR EN21259 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/215_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000216) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/216_temp2.csv"; //x=990, y=990 ### FOR EN21260 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/216_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000217) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/217_temp2.csv"; //x=990, y=990 ### FOR EN21261 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/217_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000218) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/218_temp2.csv"; //x=990, y=990 ### FOR EN21262 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/218_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000219) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/219_temp2.csv"; //x=990, y=990 ### FOR EN22-001 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/219_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000220) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/220_temp2.csv"; //x=990, y=990 ### FOR EN22-002 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/220_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000221) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/221_temp2.csv"; //x=990, y=990 ### FOR EN22-003 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/221_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000222) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/222_temp2.csv"; //x=990, y=990 ### FOR EN22-004 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/222_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000223) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/223_temp2.csv"; //x=990, y=990 ### FOR EN22-005 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/223_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000224) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/224_temp2.csv"; //x=990, y=990 ### FOR EN22-006 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/224_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000225) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/225_temp2.csv"; //x=990, y=990 ### FOR EN22-007 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/225_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000226) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/226_temp2.csv"; //x=990, y=990 ### FOR EN22-008 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/226_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000227) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/227_temp2.csv"; //x=990, y=990 ### FOR EN22-009 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/227_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000228) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/228_temp2.csv"; //x=990, y=990 ### FOR EN22-010 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/228_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000229) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/229_temp2.csv"; //x=990, y=990 ### FOR EN22-011 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/229_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000230) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/230_temp2.csv"; //x=990, y=990 ### FOR EN22-012 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/230_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000231) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/231_temp2.csv"; //x=990, y=990 ### FOR EN22-013 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/231_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000232) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/232_temp2.csv"; //x=990, y=990 ### FOR EN22-014 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/232_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000233) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/233_temp2.csv"; //x=990, y=990 ### FOR EN22-015 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/233_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000234) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/234_temp2.csv"; //x=990, y=990 ### FOR EN22-016 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/234_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000235) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/235_temp2.csv"; //x=990, y=990 ### FOR EN22-017 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/235_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000236) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/236_temp2.csv"; //x=990, y=990 ### FOR EN22-018 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/236_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000237) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/237_temp2.csv"; //x=990, y=990 ### FOR EN22-019 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/237_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000238) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/238_temp2.csv"; //x=990, y=990 ### FOR EN22-020 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/238_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000239) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/239_temp2.csv"; //x=990, y=990 ### FOR EN22-021 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/239_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000240) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/240_temp2.csv"; //x=990, y=990 ### FOR EN22-022 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/240_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000241) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/241_temp2.csv"; //x=990, y=990 ### FOR EN22-023 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/241_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000242) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/242_temp2.csv"; //x=990, y=990 ### FOR EN22-024 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/242_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000243) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/243_temp2.csv"; //x=990, y=990 ### FOR EN22-025 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/243_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000244) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/244_temp2.csv"; //x=990, y=990 ### FOR EN22-026 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/244_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000245) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/245_temp2.csv"; //x=990, y=990 ### FOR EN22-027 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/245_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000246) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/246_temp2.csv"; //x=990, y=990 ### FOR EN22-028 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/246_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000247) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/247_temp2.csv"; //x=990, y=990 ### FOR EN22-029 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/247_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000248) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/248_temp2.csv"; //x=990, y=990 ### FOR EN22-030 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/248_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000249) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/249_temp2.csv"; //x=990, y=990 ### FOR EN22-031 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/249_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000250) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/250_temp2.csv"; //x=990, y=990 ### FOR EN22-032 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/250_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000251) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/251_temp2.csv"; //x=990, y=990 ### FOR EN22-033 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/251_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000252) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/252_temp2.csv"; //x=990, y=990 ### FOR EN22-034 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/252_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000253) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/253_temp2.csv"; //x=990, y=990 ### FOR EN22-035 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/253_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000254) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/254_temp2.csv"; //x=990, y=990 ### FOR EN22-036 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/254_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000255) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/255_temp2.csv"; //x=990, y=990 ### FOR EN22-037 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/255_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000256) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/256_temp2.csv"; //x=990, y=990 ### FOR EN22-038 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/256_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000257) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/257_temp2.csv"; //x=990, y=990 ### FOR EN22-039 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/257_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000258) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/258_temp2.csv"; //x=990, y=990 ### FOR EN22-040 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/258_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000259) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/259_temp2.csv"; //x=990, y=990 ### FOR EN22-041 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/259_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000260) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/260_temp2.csv"; //x=990, y=990 ### FOR EN22-042 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/260_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000261) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/261_temp2.csv"; //x=990, y=990 ### FOR EN22-043 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/261_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000262) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/262_temp2.csv"; //x=990, y=990 ### FOR EN22-044 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/262_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000263) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/263_temp2.csv"; //x=990, y=990 ### FOR EN22-045 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/263_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000264) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/264_temp2.csv"; //x=990, y=990 ### FOR EN22-046 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/264_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000265) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/265_temp2.csv"; //x=990, y=990 ### FOR EN22-047 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/265_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000266) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/266_temp2.csv"; //x=990, y=990 ### FOR EN22-048 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/266_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000267) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/267_temp2.csv"; //x=990, y=990 ### FOR EN22-049 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/267_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000268) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/268_temp2.csv"; //x=990, y=990 ### FOR EN22-050 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/268_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000269) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/269_temp2.csv"; //x=990, y=990 ### FOR EN22-051 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/269_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000270) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/270_temp2.csv"; //x=990, y=990 ### FOR EN22-052 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/270_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000271) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/271_temp2.csv"; //x=990, y=990 ### FOR EN22-053 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/271_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000272) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/272_temp2.csv"; //x=990, y=990 ### FOR EN22-054 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/272_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000273) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/273_temp2.csv"; //x=990, y=990 ### FOR EN22-055 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/273_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000274) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/274_temp2.csv"; //x=990, y=990 ### FOR EN22-056 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/274_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000275) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/275_temp2.csv"; //x=990, y=990 ### FOR EN22-057 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/275_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000276) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/276_temp2.csv"; //x=990, y=990 ### FOR EN22-058 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/276_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000277) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/277_temp2.csv"; //x=990, y=990 ### FOR EN22-059 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/277_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000278) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/278_temp2.csv"; //x=990, y=990 ### FOR EN22-060 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/278_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000279) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/279_temp2.csv"; //x=990, y=990 ### FOR EN22-061 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/279_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000280) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/280_temp2.csv"; //x=990, y=990 ### FOR EN22-062 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/280_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000281) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/281_temp2.csv"; //x=990, y=990 ### FOR EN22-063 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/281_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000282) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/282_temp2.csv"; //x=990, y=990 ### FOR EN22-064 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/282_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000283) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/283_temp2.csv"; //x=990, y=990 ### FOR EN22-065 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/283_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000284) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/284_temp2.csv"; //x=990, y=990 ### FOR EN22-066 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/284_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000285) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/285_temp2.csv"; //x=990, y=990 ### FOR EN22-067 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/285_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000286) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/286_temp2.csv"; //x=990, y=990 ### FOR EN22-068 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/286_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000287) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/287_temp2.csv"; //x=990, y=990 ### FOR EN22-069 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/287_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000288) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/288_temp2.csv"; //x=990, y=990 ### FOR EN22-070 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/288_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000289) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/289_temp2.csv"; //x=990, y=990 ### FOR EN22-071 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/289_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000290) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/290_temp2.csv"; //x=990, y=990 ### FOR EN22-072 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/290_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000291) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/291_temp2.csv"; //x=990, y=990 ### FOR EN22-073 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/291_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000292) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/292_temp2.csv"; //x=990, y=990 ### FOR EN22-074 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/292_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000293) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/293_temp2.csv"; //x=990, y=990 ### FOR EN22-075 ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/293_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000294) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/294_temp2.csv"; //x=990, y=990 ### FOR TY04VI ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/294_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000295) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/295_temp2.csv"; //x=990, y=990 ### FOR TY04VII ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/295_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000296) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/296_temp2.csv"; //x=990, y=990 ### FOR TY07VI ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/296_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000297) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/297_temp2.csv"; //x=990, y=990 ### FOR TY07VII ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/297_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000298) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/298_temp2.csv"; //x=990, y=990 ### FOR TY09VI ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/298_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2000000299) {
		  char tempbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/299_temp2.csv"; //x=990, y=990 ### FOR TY09VII ###
		  char precbuf[] = "/albedo/work/user/lufarkas/LAVESI_HPC/LAVESI_input_climate_data/from100yrMeans/299_prec2.csv";
		  strcpy(dateinametemp, tempbuf);
		  strcpy(dateinameprec, precbuf);
		}



        if (parameter[0].demlandscape) {
            // elevation adjustment
            double current_elevation;
            double minele = 300;
            double maxele = 1300;
            // calculate steps by the set number of plots
            double stepele = (maxele - minele) / (parameter[0].mapylength - 1);
            // assume if >1 y-simulation plots that elevation range will be tested
            if (parameter[0].mapylength > 1)
                current_elevation = parameter[0].elevationoffset + minele + stepele * (aktort - 1);
            else
                current_elevation = parameter[0].elevationoffset;

            parameter[0].tempjandiffort = parameter[0].temperaturelapse_jan * current_elevation;
            parameter[0].tempjandiffortmin =
                parameter[0].temperaturelapse_jan * (current_elevation + 1000);  // calculate changes per 1000 m to use later for tree growth estimation
            parameter[0].tempjuldiffort = parameter[0].temperaturelapse_jul * current_elevation;
            parameter[0].tempjuldiffortmin = parameter[0].temperaturelapse_jul * (current_elevation + 1000);
            parameter[0].tempdiffort = 0;
            parameter[0].precdiffort = parameter[0].precipitationlapse_year * current_elevation;
            parameter[0].precdiffortmin = parameter[0].precipitationlapse_year * (current_elevation + 1000);
        } else if (parameter[0].lineartransect) {
            parameter[0].tempjandiffort = parameter[0].temperaturelapse_jan * parameter[0].locationshift;  // in m: negative values for northward/colder areas
            parameter[0].tempjandiffortmin = parameter[0].temperaturelapse_jan * (parameter[0].locationshift + (double)treerows);
            parameter[0].tempjuldiffort = parameter[0].temperaturelapse_jul * parameter[0].locationshift;
            parameter[0].tempjuldiffortmin = parameter[0].temperaturelapse_jul * (parameter[0].locationshift + (double)treerows);
            parameter[0].tempdiffort = 0;
            parameter[0].precdiffort = parameter[0].precipitationlapse_year * parameter[0].locationshift;
            parameter[0].precdiffortmin = parameter[0].precipitationlapse_year * (parameter[0].locationshift + (double)treerows);
        }

        getTemp(	//aktort, 
					dateinametemp, 
					weather_list);
        getPrec(dateinameprec, weather_list, stringlengthmax);
    }

    passWeather();
}

