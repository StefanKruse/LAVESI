#include "LAVESI.h"

using namespace std;

// TODO temporary here
extern vector<vector<Weather>> world_weather_list;
extern vector<double> wdir, wspd;
extern vector<vector<double>> windspd;
extern vector<vector<double>> winddir;
extern vector<int> globalyears;

void getPrec(char dateinameprec[50], vector<Weather>& weather_list, int maximal_word_length) {
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
				char dateinametemp[50], 
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

			// Fire index calculation for each month
			double T1 = weather_list[iweather].temp1monthmean;
			double P1 = weather_list[iweather].prec1monthmean;
			weather_list[iweather].fireindex1 = 0.10336*(((T1 + 6.19) * 0.6291112) + ((P1 - 34.99) * 0.7773153)) + (-0.17637*(((T1 + 6.19) * -0.7773153) + ((P1 - 34.99) * 0.6291112)))+0.69034;
			
			double T2 = weather_list[iweather].temp2monthmean;
			double P2 = weather_list[iweather].prec2monthmean;
			weather_list[iweather].fireindex2 = 0.10336*(((T2 + 6.19) * 0.6291112) + ((P2 - 34.99) * 0.7773153)) + (-0.17637*(((T2 + 6.19) * -0.7773153) + ((P2 - 34.99) * 0.6291112)))+0.69034;
			
			double T3 = weather_list[iweather].temp3monthmean;
			double P3 = weather_list[iweather].prec3monthmean;
			weather_list[iweather].fireindex3 = 0.10336*(((T3 + 6.19) * 0.6291112) + ((P3 - 34.99) * 0.7773153)) + (-0.17637*(((T3 + 6.19) * -0.7773153) + ((P3 - 34.99) * 0.6291112)))+0.69034;
			
			double T4 = weather_list[iweather].temp4monthmean;
			double P4 = weather_list[iweather].prec4monthmean;
			weather_list[iweather].fireindex4 = 0.10336*(((T4 + 6.19) * 0.6291112) + ((P4 - 34.99) * 0.7773153)) + (-0.17637*(((T4 + 6.19) * -0.7773153) + ((P4 - 34.99) * 0.6291112)))+0.69034;
			
			double T5 = weather_list[iweather].temp5monthmean;
			double P5 = weather_list[iweather].prec5monthmean;
			weather_list[iweather].fireindex5 = 0.10336*(((T5 + 6.19) * 0.6291112) + ((P5 - 34.99) * 0.7773153)) + (-0.17637*(((T5 + 6.19) * -0.7773153) + ((P5 - 34.99) * 0.6291112)))+0.69034;
			
			double T6 = weather_list[iweather].temp6monthmean;
			double P6 = weather_list[iweather].prec6monthmean;
			weather_list[iweather].fireindex6 = 0.10336*(((T6 + 6.19) * 0.6291112) + ((P6 - 34.99) * 0.7773153)) + (-0.17637*(((T6 + 6.19) * -0.7773153) + ((P6 - 34.99) * 0.6291112)))+0.69034;
			
			double T7 = weather_list[iweather].temp7monthmean;
			double P7 = weather_list[iweather].prec7monthmean;
			weather_list[iweather].fireindex7 = 0.10336*(((T7 + 6.19) * 0.6291112) + ((P7 - 34.99) * 0.7773153)) + (-0.17637*(((T7 + 6.19) * -0.7773153) + ((P7 - 34.99) * 0.6291112)))+0.69034;
			
			double T8 = weather_list[iweather].temp8monthmean;
			double P8 = weather_list[iweather].prec8monthmean;
			weather_list[iweather].fireindex8 = 0.10336*(((T8 + 6.19) * 0.6291112) + ((P8 - 34.99) * 0.7773153)) + (-0.17637*(((T8 + 6.19) * -0.7773153) + ((P8 - 34.99) * 0.6291112)))+0.69034;
			
			double T9 = weather_list[iweather].temp9monthmean;
			double P9 = weather_list[iweather].prec9monthmean;
			weather_list[iweather].fireindex9 = 0.10336*(((T9 + 6.19) * 0.6291112) + ((P9 - 34.99) * 0.7773153)) + (-0.17637*(((T9 + 6.19) * -0.7773153) + ((P9 - 34.99) * 0.6291112)))+0.69034;
			
			double T10 = weather_list[iweather].temp10monthmean;
			double P10 = weather_list[iweather].prec10monthmean;
			weather_list[iweather].fireindex10 = 0.10336*(((T10 + 6.19) * 0.6291112) + ((P10 - 34.99) * 0.7773153)) + (-0.17637*(((T10 + 6.19) * -0.7773153) + ((P10 - 34.99) * 0.6291112)))+0.69034;
			
			double T11 = weather_list[iweather].temp11monthmean;
			double P11 = weather_list[iweather].prec11monthmean;
			weather_list[iweather].fireindex11 = 0.10336*(((T11 + 6.19) * 0.6291112) + ((P11 - 34.99) * 0.7773153)) + (-0.17637*(((T11 + 6.19) * -0.7773153) + ((P11 - 34.99) * 0.6291112)))+0.69034;
			
			double T12 = weather_list[iweather].temp12monthmean;
			double P12 = weather_list[iweather].prec12monthmean;
			weather_list[iweather].fireindex12 = 0.10336*(((T12 + 6.19) * 0.6291112) + ((P12 - 34.99) * 0.7773153)) + (-0.17637*(((T12 + 6.19) * -0.7773153) + ((P12 - 34.99) * 0.6291112)))+0.69034;

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
        if (
            // Taimyr Peninsula
            (parameter[0].weatherchoice == 501300021) || (parameter[0].weatherchoice == 501300022) || (parameter[0].weatherchoice == 501300023)
            || (parameter[0].weatherchoice == 501300024) || (parameter[0].weatherchoice == 501300025) || (parameter[0].weatherchoice == 501300026)
            || (parameter[0].weatherchoice == 501300027) || (parameter[0].weatherchoice == 501300028) || (parameter[0].weatherchoice == 21)
            || (parameter[0].weatherchoice == 22) || (parameter[0].weatherchoice == 23) || (parameter[0].weatherchoice == 24)
            || (parameter[0].weatherchoice == 7001)) {
            foldername << "wind_Taimyr";
        } else if (
            // Buor Khaya Peninsula
            (parameter[0].weatherchoice == 501300031) || (parameter[0].weatherchoice == 501300032) || (parameter[0].weatherchoice == 501300033)
            || (parameter[0].weatherchoice == 501300034) || (parameter[0].weatherchoice == 501300035) || (parameter[0].weatherchoice == 501300036)
            || (parameter[0].weatherchoice == 501300037) || (parameter[0].weatherchoice == 501300038)) {
            foldername << "wind_BuorKhaya";
        } else if (
            // Kolyma River Basin
            (parameter[0].weatherchoice == 501300041) || (parameter[0].weatherchoice == 501300042) || (parameter[0].weatherchoice == 501300043)
            || (parameter[0].weatherchoice == 501300044) || (parameter[0].weatherchoice == 501300045) || (parameter[0].weatherchoice == 501300046)
            || (parameter[0].weatherchoice == 501300047) || (parameter[0].weatherchoice == 501300048)) {
            foldername << "wind_Kolyma";
        } else if (
            // Chukotka
            (parameter[0].weatherchoice == 501300051) || (parameter[0].weatherchoice == 501300052) || (parameter[0].weatherchoice == 501300053)
            || (parameter[0].weatherchoice == 501300054) || (parameter[0].weatherchoice == 501300055) || (parameter[0].weatherchoice == 501300056)
            || (parameter[0].weatherchoice == 501300057) || (parameter[0].weatherchoice == 501300058) || (parameter[0].weatherchoice == 2300451)
            || (parameter[0].weatherchoice == 2300851)
            || (parameter[0].weatherchoice == 2500511)|| (parameter[0].weatherchoice == 2500521)|| (parameter[0].weatherchoice == 2500531)) {
            foldername << "wind_Chukotka";
        } else if (
            // Spasskaya Pad
            (parameter[0].weatherchoice == 1901201801) || (parameter[0].weatherchoice == 1201801) || (parameter[0].weatherchoice == 1201802) || (parameter[0].weatherchoice == 1250002)) {
            foldername << "wind_SpasskayaPad";
        } else {
			cout << " no wind data for weather choice available, read for Chukotka!" << endl;
            foldername << "wind_Chukotka";
		}

        for (int t = 0; t < parameter[0].simduration; t++) {
            cntr = 0;

            jahr = parameter[0].startjahr + t;

            if (parameter[0].windsource == 1) {
                findyr1 = 1979;
                findyr2 = 2018;
            }

            ss.str("");
            ss.clear();

            if ((jahr < findyr2 + 1) && (jahr > findyr1 - 1)) {
                ss << jahr;

                if (parameter[0].windsource == 1) {
                    filename = "input/" + foldername.str() + "/winddata" + ss.str() + "_EraInterim.dat";
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

        } else if (parameter[0].weatherchoice == 2300451) {
            char tempbuf[] = "input/trans_Chukotka_shift00_temp_1901-2300_rcp45increasing.csv";
            char precbuf[] = "input/trans_Chukotka_shift00_prec_1901-2300_rcp45increasing.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
        } else if (parameter[0].weatherchoice == 2300851) {
            char tempbuf[] = "input/trans_Chukotka_shift00_temp_1901-2300_rcp85increasing.csv";
            char precbuf[] = "input/trans_Chukotka_shift00_prec_1901-2300_rcp85increasing.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);

            // forcing with 501:1900 reconstruction, 1901:2018 CRU TS 4 reanalysis and 2019:3000 CMIP5 predictions until 2500 and remain at final sim years or
            // back to 1901-2000 period until 3000; naming conventions:
            // ... 501 == start year
            // ... ...3000 == final year
            // ... .......2 == Transect 2 Taimyr Peninsula
            // ... .......3 == Transect 3 Buor Khaya
            // ... .......4 == Transect 4 Kolyma
            // ... .......5 == Transect 5 Chukotka
            // ... ........1 == rcp2.6
            // ... ........2 == rcp4.5
            // ... ........3 == rcp8.5
            // ... ........4 == rcp2.6 half => 1.3
            // ... ........5 == rcp2.6 + cooling after 2300 back to 1901:1987
            // ... ........6 == rcp2.6 half => 1.3 +  cooling after 2300 back to 1901:1987

            // RCP2.6
        } else if (parameter[0].weatherchoice == 501300021) {  // last number added
            char tempbuf[] = "input/transectTaimyr Peninsula_RCP26temp_501-3000.csv";
            char precbuf[] = "input/transectTaimyr Peninsula_RCP26prec_501-3000.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
        } else if (parameter[0].weatherchoice == 501300031) {  // last number added
            char tempbuf[] = "input/transectBuor Khaya_RCP26temp_501-3000.csv";
            char precbuf[] = "input/transectBuor Khaya_RCP26prec_501-3000.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
        } else if (parameter[0].weatherchoice == 501300041) {  // last number added
            char tempbuf[] = "input/transectKolyma_RCP26temp_501-3000.csv";
            char precbuf[] = "input/transectKolyma_RCP26prec_501-3000.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
        } else if (parameter[0].weatherchoice == 501300051) {  // last number added
            char tempbuf[] = "input/transectChukotka_RCP26temp_501-3000.csv";
            char precbuf[] = "input/transectChukotka_RCP26prec_501-3000.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
        } else if (parameter[0].weatherchoice == 2500511) {  // new last number == for simulation of repeat 2076:2125 time slice
            char tempbuf[] = "input/transectChukotka_RCP26temp_2076-2125repeated_501-3000.csv";
            char precbuf[] = "input/transectChukotka_RCP26prec_2076-2125repeated_501-3000.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
			
            // RCP4.5
        } else if (parameter[0].weatherchoice == 501300022) {  // last number added
            char tempbuf[] = "input/transectTaimyr Peninsula_RCP45temp_501-3000.csv";
            char precbuf[] = "input/transectTaimyr Peninsula_RCP45prec_501-3000.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
        } else if (parameter[0].weatherchoice == 501300032) {  // last number added
            char tempbuf[] = "input/transectBuor Khaya_RCP45temp_501-3000.csv";
            char precbuf[] = "input/transectBuor Khaya_RCP45prec_501-3000.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
        } else if (parameter[0].weatherchoice == 501300042) {  // last number added
            char tempbuf[] = "input/transectKolyma_RCP45temp_501-3000.csv";
            char precbuf[] = "input/transectKolyma_RCP45prec_501-3000.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
        } else if (parameter[0].weatherchoice == 501300052) {  // last number added
            char tempbuf[] = "input/transectChukotka_RCP45temp_501-3000.csv";
            char precbuf[] = "input/transectChukotka_RCP45prec_501-3000.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 2500521) {  // new last number == for simulation of repeat 2076:2125 time slice
            char tempbuf[] = "input/transectChukotka_RCP45temp_2076-2125repeated_501-3000.csv";
            char precbuf[] = "input/transectChukotka_RCP45prec_2076-2125repeated_501-3000.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
			
            // RCP8.5
        } else if (parameter[0].weatherchoice == 501300023) {  // last number added
            char tempbuf[] = "input/transectTaimyr Peninsula_RCP85temp_501-3000.csv";
            char precbuf[] = "input/transectTaimyr Peninsula_RCP85prec_501-3000.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
        } else if (parameter[0].weatherchoice == 501300033) {  // last number added
            char tempbuf[] = "input/transectBuor Khaya_RCP85temp_501-3000.csv";
            char precbuf[] = "input/transectBuor Khaya_RCP85prec_501-3000.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
        } else if (parameter[0].weatherchoice == 501300043) {  // last number added
            char tempbuf[] = "input/transectKolyma_RCP85temp_501-3000.csv";
            char precbuf[] = "input/transectKolyma_RCP85prec_501-3000.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
        } else if (parameter[0].weatherchoice == 501300053) {  // last number added
            char tempbuf[] = "input/transectChukotka_RCP85temp_501-3000.csv";
            char precbuf[] = "input/transectChukotka_RCP85prec_501-3000.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
        } else if (parameter[0].weatherchoice == 2500531) {  // new last number == for simulation of repeat 2076:2125 time slice
            char tempbuf[] = "input/transectChukotka_RCP85temp_2076-2125repeated_501-3000.csv";
            char precbuf[] = "input/transectChukotka_RCP85prec_2076-2125repeated_501-3000.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);

            // RCP2.6 half as strong warming named 1.3
        } else if (parameter[0].weatherchoice == 501300024) {  // last number added
            char tempbuf[] = "input/transectTaimyr Peninsula_RCP13temp_501-3000.csv";
            char precbuf[] = "input/transectTaimyr Peninsula_RCP26prec_501-3000.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
        } else if (parameter[0].weatherchoice == 501300034) {  // last number added
            char tempbuf[] = "input/transectBuor Khaya_RCP13temp_501-3000.csv";
            char precbuf[] = "input/transectBuor Khaya_RCP26prec_501-3000.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
        } else if (parameter[0].weatherchoice == 501300044) {  // last number added
            char tempbuf[] = "input/transectKolyma_RCP13temp_501-3000.csv";
            char precbuf[] = "input/transectKolyma_RCP26prec_501-3000.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
        } else if (parameter[0].weatherchoice == 501300054) {  // last number added
            char tempbuf[] = "input/transectChukotka_RCP13temp_501-3000.csv";
            char precbuf[] = "input/transectChukotka_RCP26prec_501-3000.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);

            //  cooling back down to 1901:1987
            // RCP2.6 but with cooling back down to 1901:1987
        } else if (parameter[0].weatherchoice == 501300025) {  // last number added
            char tempbuf[] = "input/transectTaimyr Peninsula_RCP26cooltemp_501-3000.csv";
            char precbuf[] = "input/transectTaimyr Peninsula_RCP26prec_501-3000.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
        } else if (parameter[0].weatherchoice == 501300035) {  // last number added
            char tempbuf[] = "input/transectBuor Khaya_RCP26cooltemp_501-3000.csv";
            char precbuf[] = "input/transectBuor Khaya_RCP26prec_501-3000.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
        } else if (parameter[0].weatherchoice == 501300045) {  // last number added
            char tempbuf[] = "input/transectKolyma_RCP26cooltemp_501-3000.csv";
            char precbuf[] = "input/transectKolyma_RCP26prec_501-3000.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
        } else if (parameter[0].weatherchoice == 501300055) {  // last number added
            char tempbuf[] = "input/transectChukotka_RCP26cooltemp_501-3000.csv";
            char precbuf[] = "input/transectChukotka_RCP26prec_501-3000.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);

            // RCP2.6 half as strong warming named 1.3 but with cooling back down to 1901:1987
        } else if (parameter[0].weatherchoice == 501300026) {  // last number added
            char tempbuf[] = "input/transectTaimyr Peninsula_RCP13cooltemp_501-3000.csv";
            char precbuf[] = "input/transectTaimyr Peninsula_RCP26prec_501-3000.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
        } else if (parameter[0].weatherchoice == 501300036) {  // last number added
            char tempbuf[] = "input/transectBuor Khaya_RCP13cooltemp_501-3000.csv";
            char precbuf[] = "input/transectBuor Khaya_RCP26prec_501-3000.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
        } else if (parameter[0].weatherchoice == 501300046) {  // last number added
            char tempbuf[] = "input/transectKolyma_RCP13cooltemp_501-3000.csv";
            char precbuf[] = "input/transectKolyma_RCP26prec_501-3000.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
        } else if (parameter[0].weatherchoice == 501300056) {  // last number added
            char tempbuf[] = "input/transectChukotka_RCP13cooltemp_501-3000.csv";
            char precbuf[] = "input/transectChukotka_RCP26prec_501-3000.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);

            // RCP4.5 but with cooling back down to 1901:1987
        } else if (parameter[0].weatherchoice == 501300027) {  // last number added
            char tempbuf[] = "input/transectTaimyr Peninsula_RCP45cooltemp_501-3000.csv";
            char precbuf[] = "input/transectTaimyr Peninsula_RCP45prec_501-3000.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
        } else if (parameter[0].weatherchoice == 501300037) {  // last number added
            char tempbuf[] = "input/transectBuor Khaya_RCP45cooltemp_501-3000.csv";
            char precbuf[] = "input/transectBuor Khaya_RCP45prec_501-3000.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
        } else if (parameter[0].weatherchoice == 501300047) {  // last number added
            char tempbuf[] = "input/transectKolyma_RCP45cooltemp_501-3000.csv";
            char precbuf[] = "input/transectKolyma_RCP45prec_501-3000.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
        } else if (parameter[0].weatherchoice == 501300057) {  // last number added
            char tempbuf[] = "input/transectChukotka_RCP45cooltemp_501-3000.csv";
            char precbuf[] = "input/transectChukotka_RCP45prec_501-3000.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);

            // RCP8.5 but with cooling back down to 1901:1987
        } else if (parameter[0].weatherchoice == 501300028) {  // last number added
            char tempbuf[] = "input/transectTaimyr Peninsula_RCP85cooltemp_501-3000.csv";
            char precbuf[] = "input/transectTaimyr Peninsula_RCP85prec_501-3000.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
        } else if (parameter[0].weatherchoice == 501300038) {  // last number added
            char tempbuf[] = "input/transectBuor Khaya_RCP85cooltemp_501-3000.csv";
            char precbuf[] = "input/transectBuor Khaya_RCP85prec_501-3000.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
        } else if (parameter[0].weatherchoice == 501300048) {  // last number added
            char tempbuf[] = "input/transectKolyma_RCP85cooltemp_501-3000.csv";
            char precbuf[] = "input/transectKolyma_RCP85prec_501-3000.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
        } else if (parameter[0].weatherchoice == 501300058) {  // last number added
            char tempbuf[] = "input/transectChukotka_RCP85cooltemp_501-3000.csv";
            char precbuf[] = "input/transectChukotka_RCP85prec_501-3000.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);

			// For Lake Khamra fire module test
        } else if (parameter[0].weatherchoice == 18232) {
            char tempbuf[] = "input/18232_khamra_temp.csv";
            char precbuf[] = "input/18232_khamra_prec.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
        } else if (parameter[0].weatherchoice == 182321) {
            char tempbuf[] = "input/18232_khamra_trace_temp.csv";
            char precbuf[] = "input/18232_khamra_trace_prec.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
        } else if (parameter[0].weatherchoice == 182322) {
            char tempbuf[] = "input/18232_khamra_traceCORR_temp.csv";
            char precbuf[] = "input/18232_khamra_traceCORR_prec.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
        } else if (parameter[0].weatherchoice == 182323) {
            char tempbuf[] = "input/18232_khamra_trace_temp_2990_og.csv";
            char precbuf[] = "input/18232_khamra_trace_prec_2990_og.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
        } else if (parameter[0].weatherchoice == 182324) {
            char tempbuf[] = "input/18232_khamra_traceCORR_temp_2990.csv";
            char precbuf[] = "input/18232_khamra_traceCORR_prec_2990.csv";

		// last two numbers 01 only CRU+ERAinterim, 02 TRACE+CRU+ERAinterim
        } else if (parameter[0].weatherchoice == 1901201801) {  // new for CryoGrid coupling
            char tempbuf[] = "input/cryogrid_cruera_1901_2018_SpasskayaPad_temp.csv";
            char precbuf[] = "input/cryogrid_cruera_1901_2018_SpasskayaPad_prec.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
        } else if (parameter[0].weatherchoice == 1201801) {  // new for CryoGrid coupling
            char tempbuf[] = "input/cryogrid_cruera_0001_2018_SpasskayaPad_temp.csv";
            char precbuf[] = "input/cryogrid_cruera_0001_2018_SpasskayaPad_prec.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
        } else if (parameter[0].weatherchoice == 1201802) {  // new for CryoGrid coupling ... with TRACE from 0-1901 or 30 for precipitation
            char tempbuf[] = "input/cryogrid_tracecruera_1_2018_SpasskayaPad_temp.csv";
            char precbuf[] = "input/cryogrid_tracecruera_1_2018_SpasskayaPad_prec.csv";
            strcpy(dateinametemp, tempbuf);
            strcpy(dateinameprec, precbuf);
         } else if (parameter[0].weatherchoice == 1250002) {  // new for CryoGrid coupling ... with TRACE from 0-1901 or 30 for precipitation
            char tempbuf[] = "input/cryogrid_tracecruera_1_2500_SpasskayaPad_temp.csv";
            char precbuf[] = "input/cryogrid_tracecruera_1_2500_SpasskayaPad_prec.csv";

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

