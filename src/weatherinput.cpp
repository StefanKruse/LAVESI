#include "LAVESI.h"
#include <string>
#include <sstream>
#include <iomanip>

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

void Getdisturbanceclimresponse(void) {	// read file and compute 
	FILE *f;
	f = fopen("pest_outbreak_prediction.csv","r"); 
	if (f == NULL) {
		printf("pest_outbreak_prediction.csv file not available!\n");
		exit(1);
	}
	
	char puffer[4*250000];
	int counter=0;
	

	// read in line by line
	while( fgets(puffer,4*250000,f) !=NULL ) {
		if (counter>=1) {// first row is header ... content separated by spaces
			// ... one row per pest, first column is name, second and following for each month and temperature and precipitation in range of -50 to +50 °C or mm by 0.1 steps => 1001 steps * 12 months * 2 (temperature, precipitation)
			
			pesttrait[counter].pestspecies = counter;								// number
			pesttrait[counter].pestspeciesname = strtok(puffer, " ");				// name
			
			for(unsigned short int climvari=1;climvari<=2;climvari++) {// for temp and prec
				// cout << "\n => climvari= " << climvari;
				for(unsigned short int monthi=1;monthi<=12;monthi++) {// for 12 months
					// cout << "\tmonthi= " << monthi << endl;
					for(double climstepi=-500;climstepi<=500;climstepi++) {// for 0.1 climsteps from -50 to +50
						// cout << " | climstepi= " << climstepi;
						if(climvari==1 && monthi==1) // add once value step
							pesttrait[counter].weathervalsteps.push_back(climstepi/10);
						if(climvari==1) {//temp
							if(monthi==1) pesttrait[counter].jan_temp_resp.push_back(strtod(strtok(NULL, " "),NULL));
							// cout << pesttrait[counter].jan_temp_resp[pesttrait[counter].jan_temp_resp.size()] << " ";
							if(monthi==2) pesttrait[counter].feb_temp_resp.push_back(strtod(strtok(NULL, " "),NULL));
							if(monthi==3) pesttrait[counter].mar_temp_resp.push_back(strtod(strtok(NULL, " "),NULL));
							if(monthi==4) pesttrait[counter].apr_temp_resp.push_back(strtod(strtok(NULL, " "),NULL));
							if(monthi==5) pesttrait[counter].may_temp_resp.push_back(strtod(strtok(NULL, " "),NULL));
							if(monthi==6) pesttrait[counter].jun_temp_resp.push_back(strtod(strtok(NULL, " "),NULL));
							if(monthi==7) pesttrait[counter].jul_temp_resp.push_back(strtod(strtok(NULL, " "),NULL));
							if(monthi==8) pesttrait[counter].aug_temp_resp.push_back(strtod(strtok(NULL, " "),NULL));
							if(monthi==9) pesttrait[counter].sep_temp_resp.push_back(strtod(strtok(NULL, " "),NULL));
							if(monthi==10) pesttrait[counter].oct_temp_resp.push_back(strtod(strtok(NULL, " "),NULL));
							if(monthi==11) pesttrait[counter].nov_temp_resp.push_back(strtod(strtok(NULL, " "),NULL));
							if(monthi==12) pesttrait[counter].dec_temp_resp.push_back(strtod(strtok(NULL, " "),NULL));
						} else if(climvari==2){//prec
							if(monthi==1) pesttrait[counter].jan_prec_resp.push_back(strtod(strtok(NULL, " "),NULL));
							if(monthi==2) pesttrait[counter].feb_prec_resp.push_back(strtod(strtok(NULL, " "),NULL));
							if(monthi==3) pesttrait[counter].mar_prec_resp.push_back(strtod(strtok(NULL, " "),NULL));
							if(monthi==4) pesttrait[counter].apr_prec_resp.push_back(strtod(strtok(NULL, " "),NULL));
							if(monthi==5) pesttrait[counter].may_prec_resp.push_back(strtod(strtok(NULL, " "),NULL));
							if(monthi==6) pesttrait[counter].jun_prec_resp.push_back(strtod(strtok(NULL, " "),NULL));
							if(monthi==7) pesttrait[counter].jul_prec_resp.push_back(strtod(strtok(NULL, " "),NULL));
							if(monthi==8) pesttrait[counter].aug_prec_resp.push_back(strtod(strtok(NULL, " "),NULL));
							if(monthi==9) pesttrait[counter].sep_prec_resp.push_back(strtod(strtok(NULL, " "),NULL));
							if(monthi==10) pesttrait[counter].oct_prec_resp.push_back(strtod(strtok(NULL, " "),NULL));
							if(monthi==11) pesttrait[counter].nov_prec_resp.push_back(strtod(strtok(NULL, " "),NULL));
							if(monthi==12) pesttrait[counter].dec_prec_resp.push_back(strtod(strtok(NULL, " "),NULL));
						} else {
							cout << " no pest data to read " << endl;
						}
					}// month loop
				}// climstep loop
			}// temp prec loop
			
			// test output 
			cout << "Pest species number: " << pesttrait[counter].pestspecies <<
					" -> species name: " << pesttrait[counter].pestspeciesname << endl;
					// << endl;
			// cout << "pesttrait[counter].jan_temp_resp contains:";
			// for (unsigned i=0; i<pesttrait[counter].jan_temp_resp.size() ; i++)
				// cout << ' ' << pesttrait[counter].jan_temp_resp[i];
			// cout << '\n';
		  
		}
		counter++;
	}
	fclose(f);
}

void Getdisturbanceimpact(vector<Weather>& weather_list) {// process pest climate to year-by-year impact
	// use current weather and extract for each pestspecies the probability of occurance
	// pesttrait[counter].pestspecies // number 1-max number in initial version 7
	for(unsigned short int pestspeciesi=1;pestspeciesi<=parameter[0].pest_species_max;pestspeciesi++) {
		cout << " ... processing pestspecies (#=" <<  pestspeciesi << ") : " << pesttrait[pestspeciesi].pestspeciesname << endl;
		// cout << "weather_list.size() = " << weather_list.size() << endl;
		    for (unsigned int iweather = 0; iweather < weather_list.size(); ++iweather) {
				// assess current weather and sum the probabilities (or mean?)
				double pestoutbreakprobability_i = 0.0;
				
				for(unsigned short int climvari=1;climvari<=2;climvari++) {// for temp and prec
					for(unsigned short int monthi=1;monthi<=12;monthi++) {// for 12 months
						if(climvari==1) {//temp
							if(monthi==1) {
								// code snippet to find closest position
								unsigned short int climpos_i = round(weather_list[iweather].temp1monthmean * 10) + 500;
								pestoutbreakprobability_i += pesttrait[pestspeciesi].jan_temp_resp[climpos_i];
							}
							if(monthi==2) {
								unsigned short int climpos_i = round(weather_list[iweather].temp2monthmean * 10) + 500;
								pestoutbreakprobability_i += pesttrait[pestspeciesi].feb_temp_resp[climpos_i];
							}
							if(monthi==3) {
								unsigned short int climpos_i = round(weather_list[iweather].temp3monthmean * 10) + 500;
								pestoutbreakprobability_i += pesttrait[pestspeciesi].mar_temp_resp[climpos_i];
							}
							if(monthi==4) {
								unsigned short int climpos_i = round(weather_list[iweather].temp4monthmean * 10) + 500;
								pestoutbreakprobability_i += pesttrait[pestspeciesi].apr_temp_resp[climpos_i];
							}
							if(monthi==5) {
								unsigned short int climpos_i = round(weather_list[iweather].temp5monthmean * 10) + 500;
								pestoutbreakprobability_i += pesttrait[pestspeciesi].may_temp_resp[climpos_i];
							}
							if(monthi==6) {
								unsigned short int climpos_i = round(weather_list[iweather].temp6monthmean * 10) + 500;
								pestoutbreakprobability_i += pesttrait[pestspeciesi].jun_temp_resp[climpos_i];
							}
							if(monthi==7) {
								unsigned short int climpos_i = round(weather_list[iweather].temp7monthmean * 10) + 500;
								pestoutbreakprobability_i += pesttrait[pestspeciesi].jul_temp_resp[climpos_i];
							}
							if(monthi==8) {
								unsigned short int climpos_i = round(weather_list[iweather].temp8monthmean * 10) + 500;
								pestoutbreakprobability_i += pesttrait[pestspeciesi].aug_temp_resp[climpos_i];
							}
							if(monthi==9) {
								unsigned short int climpos_i = round(weather_list[iweather].temp9monthmean * 10) + 500;
								pestoutbreakprobability_i += pesttrait[pestspeciesi].sep_temp_resp[climpos_i];
							}
							if(monthi==10) {
								unsigned short int climpos_i = round(weather_list[iweather].temp10monthmean * 10) + 500;
								pestoutbreakprobability_i += pesttrait[pestspeciesi].oct_temp_resp[climpos_i];
							}
							if(monthi==11) {
								unsigned short int climpos_i = round(weather_list[iweather].temp11monthmean * 10) + 500;
								pestoutbreakprobability_i += pesttrait[pestspeciesi].nov_temp_resp[climpos_i];
							}
							if(monthi==12) {
								unsigned short int climpos_i = round(weather_list[iweather].temp12monthmean * 10) + 500;
								pestoutbreakprobability_i += pesttrait[pestspeciesi].dec_temp_resp[climpos_i];
							}
						} else if(climvari==2) {//prec
							if(monthi==1) {
								unsigned short int climpos_i = round(weather_list[iweather].prec1monthmean * 10) + 500;
								pestoutbreakprobability_i += pesttrait[pestspeciesi].jan_prec_resp[climpos_i];
							}
							if(monthi==2) {
								unsigned short int climpos_i = round(weather_list[iweather].prec2monthmean * 10) + 500;
								pestoutbreakprobability_i += pesttrait[pestspeciesi].feb_prec_resp[climpos_i];
							}
							if(monthi==3) {
								unsigned short int climpos_i = round(weather_list[iweather].prec3monthmean * 10) + 500;
								pestoutbreakprobability_i += pesttrait[pestspeciesi].mar_prec_resp[climpos_i];
							}
							if(monthi==4) {
								unsigned short int climpos_i = round(weather_list[iweather].prec4monthmean * 10) + 500;
								pestoutbreakprobability_i += pesttrait[pestspeciesi].apr_prec_resp[climpos_i];
							}
							if(monthi==5) {
								unsigned short int climpos_i = round(weather_list[iweather].prec5monthmean * 10) + 500;
								pestoutbreakprobability_i += pesttrait[pestspeciesi].may_prec_resp[climpos_i];
							}
							if(monthi==6) {
								unsigned short int climpos_i = round(weather_list[iweather].prec6monthmean * 10) + 500;
								pestoutbreakprobability_i += pesttrait[pestspeciesi].jun_prec_resp[climpos_i];
							}
							if(monthi==7) {
								unsigned short int climpos_i = round(weather_list[iweather].prec7monthmean * 10) + 500;
								pestoutbreakprobability_i += pesttrait[pestspeciesi].jul_prec_resp[climpos_i];
							}
							if(monthi==8) {
								unsigned short int climpos_i = round(weather_list[iweather].prec8monthmean * 10) + 500;
								pestoutbreakprobability_i += pesttrait[pestspeciesi].aug_prec_resp[climpos_i];
							}
							if(monthi==9) {
								unsigned short int climpos_i = round(weather_list[iweather].prec9monthmean * 10) + 500;
								pestoutbreakprobability_i += pesttrait[pestspeciesi].sep_prec_resp[climpos_i];
							}
							if(monthi==10) {
								unsigned short int climpos_i = round(weather_list[iweather].prec10monthmean * 10) + 500;
								pestoutbreakprobability_i += pesttrait[pestspeciesi].oct_prec_resp[climpos_i];
							}
							if(monthi==11) {
								unsigned short int climpos_i = round(weather_list[iweather].prec11monthmean * 10) + 500;
								pestoutbreakprobability_i += pesttrait[pestspeciesi].nov_prec_resp[climpos_i];
							}
							if(monthi==12) {
								unsigned short int climpos_i = round(weather_list[iweather].prec12monthmean * 10) + 500;
								pestoutbreakprobability_i += pesttrait[pestspeciesi].dec_prec_resp[climpos_i];
							}
						} else {
							cout << " no pest data to read " << endl;
						}
					}// month loop
				}// temp prec loop
				// cout << " pestoutbreakprobability_i (pest=" << pestspeciesi << ") = " << pestoutbreakprobability_i << endl;
				weather_list[iweather].pestoutbreakprobability.push_back(pestoutbreakprobability_i);
			}// for each year
			cout << " weather_list[0].pestoutbreakprobability.size() = " << weather_list[0].pestoutbreakprobability.size() << endl;
	}// for each pest species
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

	int plotcodeNum;
	plotcodeNum = parameter[0].weatherchoice % 10000;
	
	std::stringstream plotcode;
	plotcode << plotcodeNum;
	
    if ((parameter[0].windsource != 0) && (parameter[0].windsource != 4) && (parameter[0].windsource != 5)) {
        int findyr1 = 0, findyr2 = -100, jahr = 0, cntr = 0;

        string filename;
        ostringstream foldername;
        std::ostringstream ss;
        string item;

        // define input folder
        if ((plotcodeNum == 1148) /* ### FOR EN18075 ### */ ||
		(plotcodeNum == 1149) /* ### FOR EN18076 ### */ ||
		(plotcodeNum == 1150) /* ### FOR EN18077 ### */ ||
		(plotcodeNum == 1151) /* ### FOR EN18078 ### */ ||
		(plotcodeNum == 1152) /* ### FOR EN18079 ### */ ||
		(plotcodeNum == 1153) /* ### FOR EN18080 ### */ ||
		(plotcodeNum == 1154) /* ### FOR EN18081 ### */ ||
		(plotcodeNum == 1155) /* ### FOR EN18082 ### */ ||
		(plotcodeNum == 1149) /* ### FOR EN18076 ### */ ||
		(plotcodeNum == 1150) /* ### FOR EN18077 ### */ ||
		(plotcodeNum == 1151) /* ### FOR EN18078 ### */ ||
		(plotcodeNum == 1152) /* ### FOR EN18079 ### */ ||
		(plotcodeNum == 1153) /* ### FOR EN18080 ### */ ||
		(plotcodeNum == 1154) /* ### FOR EN18081 ### */ ||
		(plotcodeNum == 1155) /* ### FOR EN18082 ### */ ||
		(plotcodeNum == 1156)) /* ### FOR EN18083 ### */ {
			foldername << "cell_7";
		} else if ((plotcodeNum == 1144) /* ### FOR EN18071 ### */ ||
		(plotcodeNum == 1145) /* ### FOR EN18072 ### */ ||
		(plotcodeNum == 1146) /* ### FOR EN18073 ### */ ||
		(plotcodeNum == 1147)) /* ### FOR EN18074 ### */ {
			foldername << "cell_8";
		} else if ((plotcodeNum == 1132)) /* ### FOR EN18062 ### */ {
			foldername << "cell_9";
		} else if ((plotcodeNum == 1131) /* ### FOR EN18061 ### */ ||
		(plotcodeNum == 1176) /* ### FOR EN21220 ### */ ||
		(plotcodeNum == 1177) /* ### FOR EN21221 ### */ ||
		(plotcodeNum == 1178) /* ### FOR EN21222 ### */ ||
		(plotcodeNum == 1179) /* ### FOR EN21223 ### */ ||
		(plotcodeNum == 1180) /* ### FOR EN21224 ### */ ||
		(plotcodeNum == 1181) /* ### FOR EN21225 ### */ ||
		(plotcodeNum == 1182) /* ### FOR EN21226 ### */ ||
		(plotcodeNum == 1183) /* ### FOR EN21227 ### */ ||
		(plotcodeNum == 1188) /* ### FOR EN21232 ### */ ||
		(plotcodeNum == 1189) /* ### FOR EN21233 ### */ ||
		(plotcodeNum == 1190) /* ### FOR EN21234 ### */ ||
		(plotcodeNum == 1191) /* ### FOR EN21235 ### */ ||
		(plotcodeNum == 1192) /* ### FOR EN21236 ### */ ||
		(plotcodeNum == 1193) /* ### FOR EN21237 ### */ ||
		(plotcodeNum == 1194) /* ### FOR EN21238 ### */ ||
		(plotcodeNum == 1195) /* ### FOR EN21239 ### */ ||
		(plotcodeNum == 1196) /* ### FOR EN21240 ### */ ||
		(plotcodeNum == 1197) /* ### FOR EN21241 ### */ ||
		(plotcodeNum == 1198) /* ### FOR EN21242 ### */ ||
		(plotcodeNum == 1199) /* ### FOR EN21243 ### */ ||
		(plotcodeNum == 1200) /* ### FOR EN21244 ### */ ||
		(plotcodeNum == 1201) /* ### FOR EN21245 ### */ ||
		(plotcodeNum == 1202) /* ### FOR EN21246 ### */ ||
		(plotcodeNum == 1203) /* ### FOR EN21247 ### */ ||
		(plotcodeNum == 1204) /* ### FOR EN21248 ### */ ||
		(plotcodeNum == 1205) /* ### FOR EN21249 ### */ ||
		(plotcodeNum == 1206) /* ### FOR EN21250 ### */ ||
		(plotcodeNum == 1207) /* ### FOR EN21251 ### */ ||
		(plotcodeNum == 1208) /* ### FOR EN21252 ### */ ||
		(plotcodeNum == 1209) /* ### FOR EN21253 ### */ ||
		(plotcodeNum == 1210) /* ### FOR EN21254 ### */ ||
		(plotcodeNum == 1211) /* ### FOR EN21255 ### */ ||
		(plotcodeNum == 1212) /* ### FOR EN21256 ### */ ||
		(plotcodeNum == 1213) /* ### FOR EN21257 ### */ ||
		(plotcodeNum == 1214) /* ### FOR EN21258 ### */ ||
		(plotcodeNum == 1215) /* ### FOR EN21259 ### */ ||
		(plotcodeNum == 1216) /* ### FOR EN21260 ### */ ||
		(plotcodeNum == 1217) /* ### FOR EN21261 ### */ ||
		(plotcodeNum == 1218)) /* ### FOR EN21262 ### */ {
			foldername << "cell_10";
		} else if ((plotcodeNum == 1184) /* ### FOR EN21228 ### */ ||
		(plotcodeNum == 1185) /* ### FOR EN21229 ### */ ||
		(plotcodeNum == 1186) /* ### FOR EN21230 ### */ ||
		(plotcodeNum == 1187)) /* ### FOR EN21231 ### */ {
			foldername << "cell_11";
		} else if ((plotcodeNum == 1137) /* ### FOR EN18067 ### */ ||
		(plotcodeNum == 1138) /* ### FOR EN18068 ### */ ||
		(plotcodeNum == 1140) /* ### FOR EN18070_centre ### */ ||
		(plotcodeNum == 1141) /* ### FOR EN18070_edge ### */ ||
		(plotcodeNum == 1142) /* ### FOR EN18070_end ### */ ||
		(plotcodeNum == 1143)) /* ### FOR EN18070_transition ### */ {
			foldername << "cell_14";
		} else if ((plotcodeNum == 1139)) /* ### FOR EN18069 ### */ {
			foldername << "cell_15";
		} else if ((plotcodeNum == 1133) /* ### FOR EN18063 ### */ ||
		(plotcodeNum == 1134) /* ### FOR EN18064 ### */ ||
		(plotcodeNum == 1135) /* ### FOR EN18065 ### */ ||
		(plotcodeNum == 1136)) /* ### FOR EN18066 ### */ {
			foldername << "cell_16";
		} else if ((plotcodeNum == 1157) /* ### FOR EN21201 ### */ ||
		(plotcodeNum == 1159) /* ### FOR EN21203 ### */ ||
		(plotcodeNum == 1160) /* ### FOR EN21204 ### */ ||
		(plotcodeNum == 1161) /* ### FOR EN21205 ### */ ||
		(plotcodeNum == 1165) /* ### FOR EN21209 ### */ ||
		(plotcodeNum == 1166) /* ### FOR EN21210 ### */ ||
		(plotcodeNum == 1167) /* ### FOR EN21211 ### */ ||
		(plotcodeNum == 1171) /* ### FOR EN21215 ### */ ||
		(plotcodeNum == 1172) /* ### FOR EN21216 ### */ ||
		(plotcodeNum == 1173) /* ### FOR EN21217 ### */ ||
		(plotcodeNum == 1174) /* ### FOR EN21218 ### */ ||
		(plotcodeNum == 1175)) /* ### FOR EN21219 ### */ {
			foldername << "cell_17";
		} else if ((plotcodeNum == 1158) /* ### FOR EN21202 ### */ ||
		(plotcodeNum == 1162) /* ### FOR EN21206 ### */ ||
		(plotcodeNum == 1163) /* ### FOR EN21207 ### */ ||
		(plotcodeNum == 1164) /* ### FOR EN21208 ### */ ||
		(plotcodeNum == 1168) /* ### FOR EN21212 ### */ ||
		(plotcodeNum == 1169) /* ### FOR EN21213 ### */ ||
		(plotcodeNum == 1170)) /* ### FOR EN21214 ### */ {
			foldername << "cell_18";
		} else if ((plotcodeNum == 1080) /* ### FOR 16-KP-V20 ### */ ||
		(plotcodeNum == 1081) /* ### FOR 16-KP-V21 ### */ ||
		(plotcodeNum == 1082) /* ### FOR 16-KP-V22 ### */ ||
		(plotcodeNum == 1083) /* ### FOR 16-KP-V23 ### */ ||
		(plotcodeNum == 1084) /* ### FOR 16-KP-V24 ### */ ||
		(plotcodeNum == 1085) /* ### FOR 16-KP-V25 ### */ ||
		(plotcodeNum == 1086) /* ### FOR 16-KP-V26 ### */ ||
		(plotcodeNum == 1087) /* ### FOR 16-KP-V27 ### */ ||
		(plotcodeNum == 1088) /* ### FOR 16-KP-V28 ### */ ||
		(plotcodeNum == 1089) /* ### FOR 16-KP-V29 ### */ ||
		(plotcodeNum == 1090) /* ### FOR 16-KP-V30 ### */ ||
		(plotcodeNum == 1091) /* ### FOR 16-KP-V31 ### */ ||
		(plotcodeNum == 1092) /* ### FOR 16-KP-V32 ### */ ||
		(plotcodeNum == 1093) /* ### FOR 16-KP-V33 ### */ ||
		(plotcodeNum == 1094) /* ### FOR 16-KP-V34 ### */ ||
		(plotcodeNum == 1095) /* ### FOR 16-KP-V35 ### */ ||
		(plotcodeNum == 1096) /* ### FOR 16-KP-V36 ### */ ||
		(plotcodeNum == 1097) /* ### FOR 16-KP-V37 ### */ ||
		(plotcodeNum == 1098) /* ### FOR 16-KP-V38 ### */ ||
		(plotcodeNum == 1099)) /* ### FOR 16-KP-V39 ### */ {
			foldername << "cell_19";
		} else if ((plotcodeNum == 1129) /* ### FOR B19-T1 ### */ ||
		(plotcodeNum == 1130)) /* ### FOR B19-T2 ### */ {
			foldername << "cell_21";
		} else if ((plotcodeNum == 1009) /* ### FOR 12-KO-02a ### */ ||
		(plotcodeNum == 1010) /* ### FOR 12-KO-02b ### */ ||
		(plotcodeNum == 1011) /* ### FOR 12-KO-03a ### */ ||
		(plotcodeNum == 1012) /* ### FOR 12-KO-03b ### */ ||
		(plotcodeNum == 1013) /* ### FOR 12-KO-04a ### */ ||
		(plotcodeNum == 1014) /* ### FOR 12-KO-04b ### */ ||
		(plotcodeNum == 1015)) /* ### FOR 12-KO-05 ### */ {
			foldername << "cell_22";
		} else if ((plotcodeNum == 1073) /* ### FOR 16-KP-V13 ### */ ||
		(plotcodeNum == 1074) /* ### FOR 16-KP-V14 ### */ ||
		(plotcodeNum == 1075) /* ### FOR 16-KP-V15 ### */ ||
		(plotcodeNum == 1076) /* ### FOR 16-KP-V16 ### */ ||
		(plotcodeNum == 1077) /* ### FOR 16-KP-V17 ### */ ||
		(plotcodeNum == 1078) /* ### FOR 16-KP-V18 ### */ ||
		(plotcodeNum == 1079) /* ### FOR 16-KP-V19 ### */ ||
		(plotcodeNum == 1119) /* ### FOR 18-BIL-00-EN18000 ### */ ||
		(plotcodeNum == 1120) /* ### FOR 18-BIL-01-EN18028 ### */ ||
		(plotcodeNum == 1121) /* ### FOR 18-BIL-01-EN18029 ### */ ||
		(plotcodeNum == 1122) /* ### FOR 18-BIL-02-EN18030 ### */ ||
		(plotcodeNum == 1123) /* ### FOR 18-BIL-02-EN18031 ### */ ||
		(plotcodeNum == 1124) /* ### FOR 18-BIL-02-EN18032 ### */ ||
		(plotcodeNum == 1125) /* ### FOR 18-BIL-02-EN18033 ### */ ||
		(plotcodeNum == 1126) /* ### FOR 18-BIL-02-EN18034 ### */ ||
		(plotcodeNum == 1127)) /* ### FOR 18-BIL-02-EN18035 ### */ {
			foldername << "cell_23";
		} else if ((plotcodeNum == 1061) /* ### FOR 16-KP-V01 ### */ ||
		(plotcodeNum == 1062) /* ### FOR 16-KP-V02 ### */ ||
		(plotcodeNum == 1063) /* ### FOR 16-KP-V03 ### */ ||
		(plotcodeNum == 1064) /* ### FOR 16-KP-V04 ### */ ||
		(plotcodeNum == 1065) /* ### FOR 16-KP-V05 ### */ ||
		(plotcodeNum == 1066) /* ### FOR 16-KP-V06 ### */ ||
		(plotcodeNum == 1067) /* ### FOR 16-KP-V07 ### */ ||
		(plotcodeNum == 1068) /* ### FOR 16-KP-V08 ### */ ||
		(plotcodeNum == 1069) /* ### FOR 16-KP-V09 ### */ ||
		(plotcodeNum == 1070) /* ### FOR 16-KP-V10 ### */ ||
		(plotcodeNum == 1071) /* ### FOR 16-KP-V11 ### */ ||
		(plotcodeNum == 1072) /* ### FOR 16-KP-V12 ### */ ||
		(plotcodeNum == 1100) /* ### FOR 16-KP-V40 ### */ ||
		(plotcodeNum == 1101) /* ### FOR 16-KP-V41 ### */ ||
		(plotcodeNum == 1102) /* ### FOR 16-KP-V42 ### */ ||
		(plotcodeNum == 1103) /* ### FOR 16-KP-V43 ### */ ||
		(plotcodeNum == 1104) /* ### FOR 16-KP-V44 ### */ ||
		(plotcodeNum == 1105) /* ### FOR 16-KP-V45 ### */ ||
		(plotcodeNum == 1106) /* ### FOR 16-KP-V46 ### */ ||
		(plotcodeNum == 1107) /* ### FOR 16-KP-V47 ### */ ||
		(plotcodeNum == 1108) /* ### FOR 16-KP-V48 ### */ ||
		(plotcodeNum == 1109) /* ### FOR 16-KP-V49 ### */ ||
		(plotcodeNum == 1110) /* ### FOR 16-KP-V50 ### */ ||
		(plotcodeNum == 1111) /* ### FOR 16-KP-V51 ### */ ||
		(plotcodeNum == 1112) /* ### FOR 16-KP-V52 ### */ ||
		(plotcodeNum == 1113) /* ### FOR 16-KP-V53 ### */ ||
		(plotcodeNum == 1114) /* ### FOR 16-KP-V54 ### */ ||
		(plotcodeNum == 1115) /* ### FOR 16-KP-V55 ### */ ||
		(plotcodeNum == 1116) /* ### FOR 16-KP-V56 ### */ ||
		(plotcodeNum == 1117) /* ### FOR 16-KP-V57 ### */ ||
		(plotcodeNum == 1118) /* ### FOR 16-KP-V58 ### */ ||
		(plotcodeNum == 1029) /* ### FOR 16-KP-01-EN18001 ### */ ||
		(plotcodeNum == 1030) /* ### FOR 16-KP-01-EN18002 ### */ ||
		(plotcodeNum == 1031) /* ### FOR 16-KP-01-EN18003 ### */ ||
		(plotcodeNum == 1032) /* ### FOR 16-KP-01-EN18004 ### */ ||
		(plotcodeNum == 1033) /* ### FOR 16-KP-01-EN18005 ### */ ||
		(plotcodeNum == 1034) /* ### FOR 16-KP-01-EN18006 ### */ ||
		(plotcodeNum == 1035) /* ### FOR 16-KP-01-EN18007 ### */ ||
		(plotcodeNum == 1036) /* ### FOR 16-KP-01-EN18008 ### */ ||
		(plotcodeNum == 1037) /* ### FOR 16-KP-01-EN18009 ### */ ||
		(plotcodeNum == 1038) /* ### FOR 16-KP-01-EN18010 ### */ ||
		(plotcodeNum == 1039) /* ### FOR 16-KP-01-EN18011 ### */ ||
		(plotcodeNum == 1040) /* ### FOR 16-KP-01-EN18012 ### */ ||
		(plotcodeNum == 1041) /* ### FOR 16-KP-01-EN18013 ### */ ||
		(plotcodeNum == 1042) /* ### FOR 16-KP-01-EN18014 ### */ ||
		(plotcodeNum == 1043) /* ### FOR 16-KP-01-EN18015 ### */ ||
		(plotcodeNum == 1044) /* ### FOR 16-KP-01-EN18016 ### */ ||
		(plotcodeNum == 1045) /* ### FOR 16-KP-01-EN18017 ### */ ||
		(plotcodeNum == 1046) /* ### FOR 16-KP-01-EN18018 ### */ ||
		(plotcodeNum == 1047) /* ### FOR 16-KP-01-EN18019 ### */ ||
		(plotcodeNum == 1048) /* ### FOR 16-KP-01-EN18020 ### */ ||
		(plotcodeNum == 1049) /* ### FOR 16-KP-01-EN18021 ### */ ||
		(plotcodeNum == 1050) /* ### FOR 16-KP-01-EN18022 ### */ ||
		(plotcodeNum == 1051) /* ### FOR 16-KP-01-EN18023 ### */ ||
		(plotcodeNum == 1052) /* ### FOR 16-KP-01-EN18024 ### */ ||
		(plotcodeNum == 1053) /* ### FOR 16-KP-01-EN18025 ### */ ||
		(plotcodeNum == 1054) /* ### FOR 16-KP-01-EN18026 ### */ ||
		(plotcodeNum == 1055) /* ### FOR 16-KP-01-EN18027 ### */ ||
		(plotcodeNum == 1056) /* ### FOR 16-KP-04-EN18051 ### */ ||
		(plotcodeNum == 1057) /* ### FOR 16-KP-04-EN18052 ### */ ||
		(plotcodeNum == 1058) /* ### FOR 16-KP-04-EN18053 ### */ ||
		(plotcodeNum == 1059) /* ### FOR 16-KP-04-EN18054 ### */ ||
		(plotcodeNum == 1060)) /* ### FOR 16-KP-04-EN18055 ### */ {
			foldername << "cell_24";
		} else if ((plotcodeNum == 1003) /* ### FOR 11-CH-06I ### */ ||
		(plotcodeNum == 1004)) /* ### FOR 11-CH-06III ### */ {
			foldername << "cell_25";
		} else if ((plotcodeNum == 1001) /* ### FOR 11-CH-02II ### */ ||
		(plotcodeNum == 1002) /* ### FOR 11-CH-02III ### */ ||
		(plotcodeNum == 1005) /* ### FOR 11-CH-12I ### */ ||
		(plotcodeNum == 1006) /* ### FOR 11-CH-12II ### */ ||
		(plotcodeNum == 1007) /* ### FOR 11-CH-17I ### */ ||
		(plotcodeNum == 1008) /* ### FOR 11-CH-17II ### */ ||
		(plotcodeNum == 1221) /* ### FOR TY07VI ### */ ||
		(plotcodeNum == 1222) /* ### FOR TY07VII ### */ ||
		(plotcodeNum == 1223) /* ### FOR TY09VI ### */ ||
		(plotcodeNum == 1224)) /* ### FOR TY09VII ### */ {
			foldername << "cell_26";
		} else if ((plotcodeNum == 1016) /* ### FOR 13-TY-02-VI ### */ ||
		(plotcodeNum == 1017) /* ### FOR 13-TY-02-VII ### */ ||
		(plotcodeNum == 1219) /* ### FOR TY04VI ### */ ||
		(plotcodeNum == 1220)) /* ### FOR TY04VII ### */ {
			foldername << "cell_27";
		} else if ((plotcodeNum == 1128)) /* ### FOR 18-LD-VP012-Tit-Ary ### */ {
			foldername << "cell_28";
		} else if ((plotcodeNum == 1018) /* ### FOR 14-OM-02-V1 ### */ ||
		(plotcodeNum == 1019) /* ### FOR 14-OM-02-V2 ### */ ||
		(plotcodeNum == 1020) /* ### FOR 14-OM-11-V3 ### */ ||
		(plotcodeNum == 1021) /* ### FOR 14-OM-20-V4 ### */ ||
		(plotcodeNum == 1022) /* ### FOR 14-OM-TRANS1 ### */ ||
		(plotcodeNum == 1023) /* ### FOR 14-OM-TRANS2 ### */ ||
		(plotcodeNum == 1024) /* ### FOR 14-OM-TRANS3 ### */ ||
		(plotcodeNum == 1025) /* ### FOR 14-OM-TRANS4 ### */ ||
		(plotcodeNum == 1026) /* ### FOR 14-OM-TRANS5 ### */ ||
		(plotcodeNum == 1028) /* ### FOR 14-OM-TRANS6 ### */ ||
		(plotcodeNum == 1027)) /* ### FOR 14-OM-TRANS6-7 ### */ {
			foldername << "cell_29";
		} else if ((plotcodeNum == 2065) /* ### FOR EN22-065 ### */ ||
		(plotcodeNum == 2066)) /* ### FOR EN22-066 ### */ {
			foldername << "cell_1";
		} else if ((plotcodeNum == 2067) /* ### FOR EN22-067 ### */ ||
		(plotcodeNum == 2068) /* ### FOR EN22-068 ### */ ||
		(plotcodeNum == 2070) /* ### FOR EN22-070 ### */ ||
		(plotcodeNum == 2071) /* ### FOR EN22-071 ### */ ||
		(plotcodeNum == 2072) /* ### FOR EN22-072 ### */ ||
		(plotcodeNum == 2073) /* ### FOR EN22-073 ### */ ||
		(plotcodeNum == 2074)) /* ### FOR EN22-074 ### */ {
			foldername << "cell_2";
		} else if ((plotcodeNum == 2069)) /* ### FOR EN22-069 ### */ {
			foldername << "cell_3";
		} else if ((plotcodeNum == 2001) /* ### FOR EN22-001 ### */ ||
		(plotcodeNum == 2002) /* ### FOR EN22-002 ### */ ||
		(plotcodeNum == 2003) /* ### FOR EN22-003 ### */ ||
		(plotcodeNum == 2004) /* ### FOR EN22-004 ### */ ||
		(plotcodeNum == 2056) /* ### FOR EN22-056 ### */ ||
		(plotcodeNum == 2057) /* ### FOR EN22-057 ### */ ||
		(plotcodeNum == 2058) /* ### FOR EN22-058 ### */ ||
		(plotcodeNum == 2059) /* ### FOR EN22-059 ### */ ||
		(plotcodeNum == 2060)) /* ### FOR EN22-060 ### */ {
			foldername << "cell_4";
		} else if ((plotcodeNum == 2061) /* ### FOR EN22-061 ### */ ||
		(plotcodeNum == 2062)) /* ### FOR EN22-062 ### */ {
			foldername << "cell_5";
		} else if ((plotcodeNum == 2063) /* ### FOR EN22-063 ### */ ||
		(plotcodeNum == 2064) /* ### FOR EN22-064 ### */ ||
		(plotcodeNum == 2075)) /* ### FOR EN22-075 ### */ {
			foldername << "cell_6";
		} else if ((plotcodeNum == 2038) /* ### FOR EN22-038 ### */ ||
		(plotcodeNum == 2039) /* ### FOR EN22-039 ### */ ||
		(plotcodeNum == 2040) /* ### FOR EN22-040 ### */ ||
		(plotcodeNum == 2041) /* ### FOR EN22-041 ### */ ||
		(plotcodeNum == 2042) /* ### FOR EN22-042 ### */ ||
		(plotcodeNum == 2043) /* ### FOR EN22-043 ### */ ||
		(plotcodeNum == 2044) /* ### FOR EN22-044 ### */ ||
		(plotcodeNum == 2045) /* ### FOR EN22-045 ### */ ||
		(plotcodeNum == 2046) /* ### FOR EN22-046 ### */ ||
		(plotcodeNum == 2047) /* ### FOR EN22-047 ### */ ||
		(plotcodeNum == 2048) /* ### FOR EN22-048 ### */ ||
		(plotcodeNum == 2049) /* ### FOR EN22-049 ### */ ||
		(plotcodeNum == 2050) /* ### FOR EN22-050 ### */ ||
		(plotcodeNum == 2051) /* ### FOR EN22-051 ### */ ||
		(plotcodeNum == 2052) /* ### FOR EN22-052 ### */ ||
		(plotcodeNum == 2053) /* ### FOR EN22-053 ### */ ||
		(plotcodeNum == 2054)) /* ### FOR EN22-054 ### */ {
			foldername << "cell_12";
		} else if ((plotcodeNum == 2037) /* ### FOR EN22-037 ### */ ||
		(plotcodeNum == 2055)) /* ### FOR EN22-055 ### */ {
			foldername << "cell_13";
		} else if ((plotcodeNum == 2005) /* ### FOR EN22-005 ### */ ||
		(plotcodeNum == 2006) /* ### FOR EN22-006 ### */ ||
		(plotcodeNum == 2007) /* ### FOR EN22-007 ### */ ||
		(plotcodeNum == 2008) /* ### FOR EN22-008 ### */ ||
		(plotcodeNum == 2009) /* ### FOR EN22-009 ### */ ||
		(plotcodeNum == 2010) /* ### FOR EN22-010 ### */ ||
		(plotcodeNum == 2011) /* ### FOR EN22-011 ### */ ||
		(plotcodeNum == 2012) /* ### FOR EN22-012 ### */ ||
		(plotcodeNum == 2013) /* ### FOR EN22-013 ### */ ||
		(plotcodeNum == 2014) /* ### FOR EN22-014 ### */ ||
		(plotcodeNum == 2015) /* ### FOR EN22-015 ### */ ||
		(plotcodeNum == 2016) /* ### FOR EN22-016 ### */ ||
		(plotcodeNum == 2017) /* ### FOR EN22-017 ### */ ||
		(plotcodeNum == 2018) /* ### FOR EN22-018 ### */ ||
		(plotcodeNum == 2019) /* ### FOR EN22-019 ### */ ||
		(plotcodeNum == 2020) /* ### FOR EN22-020 ### */ ||
		(plotcodeNum == 2021) /* ### FOR EN22-021 ### */ ||
		(plotcodeNum == 2022) /* ### FOR EN22-022 ### */ ||
		(plotcodeNum == 2023) /* ### FOR EN22-023 ### */ ||
		(plotcodeNum == 2024) /* ### FOR EN22-024 ### */ ||
		(plotcodeNum == 2025) /* ### FOR EN22-025 ### */ ||
		(plotcodeNum == 2026) /* ### FOR EN22-026 ### */ ||
		(plotcodeNum == 2027) /* ### FOR EN22-027 ### */ ||
		(plotcodeNum == 2028) /* ### FOR EN22-028 ### */ ||
		(plotcodeNum == 2029) /* ### FOR EN22-029 ### */ ||
		(plotcodeNum == 2030) /* ### FOR EN22-030 ### */ ||
		(plotcodeNum == 2031) /* ### FOR EN22-031 ### */ ||
		(plotcodeNum == 2032) /* ### FOR EN22-032 ### */ ||
		(plotcodeNum == 2033) /* ### FOR EN22-033 ### */ ||
		(plotcodeNum == 2034) /* ### FOR EN22-034 ### */ ||
		(plotcodeNum == 2035) /* ### FOR EN22-035 ### */ ||
		(plotcodeNum == 2036)) /* ### FOR EN22-036 ### */ {
			foldername << "cell_20";
		} else if ((plotcodeNum == 2078)) /* ### FOR PC11 ### */ {
			foldername << "cell_57";
		} else if ((plotcodeNum == 2084) /* ### FOR PC7 ### */ ||
		(plotcodeNum == 2085)) /* ### FOR PC8 ### */ {
			foldername << "cell_58";
		} else if ((plotcodeNum == 2086)) /* ### FOR PC9 ### */ {
			foldername << "cell_59";
		} else if ((plotcodeNum == 2077)) /* ### FOR PC10 ### */ {
			foldername << "cell_60";
		} else if ((plotcodeNum == 2076) /* ### FOR PC1 ### */ ||
		(plotcodeNum == 2079) /* ### FOR PC2 ### */ ||
		(plotcodeNum == 2081)) /* ### FOR PC4 ### */ {
			foldername << "cell_62";
		} else if ((plotcodeNum == 2080)) /* ### FOR PC3 ### */ {
			foldername << "cell_63";
		} else if ((plotcodeNum == 2083)) /* ### FOR PC6 ### */ {
			foldername << "cell_64";
		} else if ((plotcodeNum == 2082)) /* ### FOR PC5 ### */ {
			foldername << "cell_65";
		} else if ((plotcodeNum == 3001) /* ### FOR EN23-601 ### */ ||
		(plotcodeNum == 3002) /* ### FOR EN23-602 ### */ ||
		(plotcodeNum == 3003)) /* ### FOR EN23-604 ### */ {
			foldername << "cell_30";
		} else if ((plotcodeNum == 3022) /* ### FOR EN23-689 ### */ ||
		(plotcodeNum == 3023)) /* ### FOR EN23-700 ### */ {
			foldername << "cell_31";
		} else if ((plotcodeNum == 3004) /* ### FOR EN23-608 ### */ ||
		(plotcodeNum == 3005) /* ### FOR EN23-611 ### */ ||
		(plotcodeNum == 3007) /* ### FOR EN23-614 ### */ ||
		(plotcodeNum == 3013) /* ### FOR EN23-644 ### */ ||
		(plotcodeNum == 3014) /* ### FOR EN23-645 ### */ ||
		(plotcodeNum == 3018) /* ### FOR EN23-668 ### */ ||
		(plotcodeNum == 3019)) /* ### FOR EN23-669 ### */ {
			foldername << "cell_32";
		} else if ((plotcodeNum == 3006) /* ### FOR EN23-612 ### */ ||
		(plotcodeNum == 3015) /* ### FOR EN23-652 ### */ ||
		(plotcodeNum == 3016) /* ### FOR EN23-660 ### */ ||
		(plotcodeNum == 3017) /* ### FOR EN23-666 ### */ ||
		(plotcodeNum == 3020) /* ### FOR EN23-675 ### */ ||
		(plotcodeNum == 3021)) /* ### FOR EN23-677 ### */ {
			foldername << "cell_33";
		} else if ((plotcodeNum == 3008) /* ### FOR EN23-619 ### */ ||
		(plotcodeNum == 3009) /* ### FOR EN23-621 ### */ ||
		(plotcodeNum == 3010) /* ### FOR EN23-623 ### */ ||
		(plotcodeNum == 3011) /* ### FOR EN23-624 ### */ ||
		(plotcodeNum == 3012)) /* ### FOR EN23-634 ### */ {
			foldername << "cell_34";
		} else if ((plotcodeNum == 3025)) /* ### FOR PA10 ### */ {
			foldername << "cell_50";
		} else if ((plotcodeNum == 3032)) /* ### FOR PA7 ### */ {
			foldername << "cell_51";
		} else if ((plotcodeNum == 3033)) /* ### FOR PA8 ### */ {
			foldername << "cell_52";
		} else if ((plotcodeNum == 3034)) /* ### FOR PA9 ### */ {
			foldername << "cell_53";
		} else if ((plotcodeNum == 3026)) /* ### FOR PA11 ### */ {
			foldername << "cell_54";
		} else if ((plotcodeNum == 3029) /* ### FOR PA4 ### */ ||
		(plotcodeNum == 3030)) /* ### FOR PA5 ### */ {
			foldername << "cell_55";
		} else if ((plotcodeNum == 3024) /* ### FOR PA1 ### */ ||
		(plotcodeNum == 3027) /* ### FOR PA2 ### */ ||
		(plotcodeNum == 3028)) /* ### FOR PA3 ### */ {
			foldername << "cell_56";
		} else if ((plotcodeNum == 3031)) /* ### FOR PA6 ### */ {
			foldername << "cell_61";
		} else if ((plotcodeNum == 4001) /* ### FOR 2023051101 ### */ ||
		(plotcodeNum == 4002) /* ### FOR 2023051102 ### */ ||
		(plotcodeNum == 4003) /* ### FOR 2023051103 ### */ ||
		(plotcodeNum == 4004) /* ### FOR 2023051201 ### */ ||
		(plotcodeNum == 4005) /* ### FOR 2023051202 ### */ ||
		(plotcodeNum == 4006) /* ### FOR 2023051203 ### */ ||
		(plotcodeNum == 4007) /* ### FOR 2023051301 ### */ ||
		(plotcodeNum == 4008) /* ### FOR 2023051302 ### */ ||
		(plotcodeNum == 4009) /* ### FOR 2023051401 ### */ ||
		(plotcodeNum == 4010) /* ### FOR 2023051402 ### */ ||
		(plotcodeNum == 4063) /* ### FOR 2023060603 ### */ ||
		(plotcodeNum == 4064) /* ### FOR 2023060604 ### */ ||
		(plotcodeNum == 4065) /* ### FOR 2023060701 ### */ ||
		(plotcodeNum == 4066) /* ### FOR 2023060702 ### */ ||
		(plotcodeNum == 4067) /* ### FOR 2023060703 ### */ ||
		(plotcodeNum == 4068) /* ### FOR 2023060704 ### */ ||
		(plotcodeNum == 4069) /* ### FOR 2023060705 ### */ ||
		(plotcodeNum == 4070) /* ### FOR 2023060706 ### */ ||
		(plotcodeNum == 4071) /* ### FOR 2023060802 ### */ ||
		(plotcodeNum == 4072)) /* ### FOR 2023060803 ### */ {
			foldername << "cell_40";
		} else if ((plotcodeNum == 4011) /* ### FOR 2023051701 ### */ ||
		(plotcodeNum == 4012) /* ### FOR 2023051702 ### */ ||
		(plotcodeNum == 4019) /* ### FOR 2023051808 ### */ ||
		(plotcodeNum == 4050) /* ### FOR 2023060101 ### */ ||
		(plotcodeNum == 4051)) /* ### FOR 2023060102 ### */ {
			foldername << "cell_41";
		} else if ((plotcodeNum == 4058) /* ### FOR 2023060502 ### */ ||
		(plotcodeNum == 4059)) /* ### FOR 2023060503 ### */ {
			foldername << "cell_42";
		} else if ((plotcodeNum == 4057) /* ### FOR 2023060301 ### */ ||
		(plotcodeNum == 4060) /* ### FOR 2023060504 ### */ ||
		(plotcodeNum == 4061) /* ### FOR 2023060505 ### */ ||
		(plotcodeNum == 4062)) /* ### FOR 2023060601 ### */ {
			foldername << "cell_43";
		} else if ((plotcodeNum == 4013) /* ### FOR 2023051801 ### */ ||
		(plotcodeNum == 4014) /* ### FOR 2023051802 ### */ ||
		(plotcodeNum == 4015) /* ### FOR 2023051803 ### */ ||
		(plotcodeNum == 4016) /* ### FOR 2023051804 ### */ ||
		(plotcodeNum == 4017) /* ### FOR 2023051806 ### */ ||
		(plotcodeNum == 4018) /* ### FOR 2023051807 ### */ ||
		(plotcodeNum == 4020) /* ### FOR 2023051901 ### */ ||
		(plotcodeNum == 4021) /* ### FOR 2023051902 ### */ ||
		(plotcodeNum == 4022) /* ### FOR 2023052101 ### */ ||
		(plotcodeNum == 4023) /* ### FOR 2023052102 ### */ ||
		(plotcodeNum == 4024) /* ### FOR 2023052103 ### */ ||
		(plotcodeNum == 4025) /* ### FOR 2023052104 ### */ ||
		(plotcodeNum == 4026) /* ### FOR 2023052106 ### */ ||
		(plotcodeNum == 4027) /* ### FOR 2023052107 ### */ ||
		(plotcodeNum == 4038) /* ### FOR 2023052802 ### */ ||
		(plotcodeNum == 4039) /* ### FOR 2023052803 ### */ ||
		(plotcodeNum == 4040) /* ### FOR 2023052804 ### */ ||
		(plotcodeNum == 4041) /* ### FOR 2023052805 ### */ ||
		(plotcodeNum == 4042) /* ### FOR 2023052806 ### */ ||
		(plotcodeNum == 4043) /* ### FOR 2023052807 ### */ ||
		(plotcodeNum == 4044) /* ### FOR 2023052901 ### */ ||
		(plotcodeNum == 4045) /* ### FOR 2023052902 ### */ ||
		(plotcodeNum == 4046) /* ### FOR 2023052903 ### */ ||
		(plotcodeNum == 4047) /* ### FOR 2023053001 ### */ ||
		(plotcodeNum == 4048) /* ### FOR 2023053002 ### */ ||
		(plotcodeNum == 4049) /* ### FOR 2023053003 ### */ ||
		(plotcodeNum == 4052) /* ### FOR 2023060105 ### */ ||
		(plotcodeNum == 4053) /* ### FOR 2023060106 ### */ ||
		(plotcodeNum == 4054) /* ### FOR 2023060201 ### */ ||
		(plotcodeNum == 4055) /* ### FOR 2023060203 ### */ ||
		(plotcodeNum == 4056)) /* ### FOR 2023060204 ### */ {
			foldername << "cell_44";
		} else if ((plotcodeNum == 4028) /* ### FOR 2023052301 ### */ ||
		(plotcodeNum == 4029) /* ### FOR 2023052302 ### */ ||
		(plotcodeNum == 4030) /* ### FOR 2023052401 ### */ ||
		(plotcodeNum == 4031) /* ### FOR 2023052402 ### */ ||
		(plotcodeNum == 4032) /* ### FOR 2023052403 ### */ ||
		(plotcodeNum == 4033) /* ### FOR 2023052404 ### */ ||
		(plotcodeNum == 4034) /* ### FOR 2023052501 ### */ ||
		(plotcodeNum == 4035) /* ### FOR 2023052601 ### */ ||
		(plotcodeNum == 4036) /* ### FOR 2023052602 ### */ ||
		(plotcodeNum == 4037)) /* ### FOR 2023052604 ### */ {
			foldername << "cell_45";
		} else if ((plotcodeNum == 5008)) /* ### FOR Mucho_Lake ### */ {
			foldername << "cell_1";
		} else if ((plotcodeNum == 5001)) /* ### FOR Close_to_Fairbanks ### */ {
			foldername << "cell_32";
		} else if ((plotcodeNum == 5009) /* ### FOR Road_to_Central ### */ ||
		(plotcodeNum == 5002)) /* ### FOR Fielding_Lake ### */ {
			foldername << "cell_33";
		} else if ((plotcodeNum == 5007)) /* ### FOR Lake_Ulu ### */ {
			foldername << "cell_17";
		} else if ((plotcodeNum == 5006)) /* ### FOR Lake_Nutenvut ### */ {
			foldername << "cell_19";
		} else if ((plotcodeNum == 5003) /* ### FOR Fort_Mc_Pherson_East ### */ ||
		(plotcodeNum == 5004)) /* ### FOR Fort_Mc_Pherson_West ### */ {
			foldername << "cell_20";
		} else if ((plotcodeNum == 5005)) /* ### FOR Lake_Illerney ### */ {
			foldername << "cell_24";																														
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
				findyr2 = 25070;
			} else if (parameter[0].windsource == 998) {
				findyr1 = 1;
				findyr2 = 25070;
			}

            ss.str("");
            ss.clear();
			
			ostringstream path; 
			long int plotcodeNum_scenario;
			plotcodeNum_scenario = parameter[0].weatherchoice % 100000000;
	
			if (plotcodeNum_scenario < 100000) {
				path << "past25kyr_until2020_wind";
			} else if (plotcodeNum_scenario > 26000000 && plotcodeNum_scenario < 27000000) {
				path << "past25kyr_until2100_wind";
			} else if (plotcodeNum_scenario > 45000000 && plotcodeNum_scenario < 46000000) {
				path << "past25kyr_until2100_wind";
			} else if (plotcodeNum_scenario > 85000000 && plotcodeNum_scenario < 86000000) {
				path << "past25kyr_until2100_wind";
			}
			
			ostringstream region;
			
			if (plotcodeNum > 1000 && plotcodeNum < 2000) {
				region << "Siberia";
			} else if (plotcodeNum > 2000 && plotcodeNum < 3000) {
				region << "Canada";
			} else if (plotcodeNum > 3000 && plotcodeNum < 4000) {
				region << "Alaska";
			} else if (plotcodeNum > 4000 && plotcodeNum < 5000) {
				region << "MountainTreeline";
			}
			
			

            if ((jahr < findyr2 + 1) && (jahr > findyr1 - 1)) {
                ss << jahr;

                if (parameter[0].windsource == 1) {
                    filename = "input/" + foldername.str() + "/winddata" + ss.str() + "_EraInterim.dat";
                } else if (parameter[0].windsource == 10) {
					filename = "input/" + foldername.str() + "/winddata" + ss.str() + "_ERA5.dat";
				} else if (parameter[0].windsource == 999 && plotcodeNum_scenario < 100000){
					filename = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_climate_data/" + path.str() + "/TransientMPI-ESM_Glac1d-P3_fromYearlyData/" + region.str() + "/" + foldername.str() + "/winddata_" + ss.str() + ".dat";
				} else if (parameter[0].windsource == 999 && plotcodeNum_scenario > 26000000 && plotcodeNum_scenario < 27000000){
					filename = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_climate_data/" + path.str() + "/TransientMPI-ESM_Glac1d-P3_fromYearlyData/CMIP6_ssp126/" + region.str() + "/" + foldername.str() + "/winddata_" + ss.str() + ".dat";
				} else if (parameter[0].windsource == 999 && plotcodeNum_scenario > 45000000 && plotcodeNum_scenario < 46000000){
					filename = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_climate_data/" + path.str() + "/TransientMPI-ESM_Glac1d-P3_fromYearlyData/CMIP6_ssp245/" + region.str() + "/" + foldername.str() + "/winddata_" + ss.str() + ".dat";
				} else if (parameter[0].windsource == 999 && plotcodeNum_scenario > 85000000 && plotcodeNum_scenario < 86000000){
					filename = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_climate_data/" + path.str() + "/TransientMPI-ESM_Glac1d-P3_fromYearlyData/CMIP6_ssp585/" + region.str() + "/" + foldername.str() + "/winddata_" + ss.str() + ".dat";
				} else if (parameter[0].windsource == 998) {
					filename = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_climate_data/" + path.str() + "/TransientMPI-ESM1_2_from100yrMeans/" + region.str() + "/" + foldername.str() + "/winddata_" + ss.str() + ".dat";
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
		if (parameter[0].weatherchoice > 1000001000 && parameter[0].weatherchoice < 1000002000){
			string tempbuf = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_climate_data/past25kyr_until2020_temp_prec/TransientMPI-ESM_Glac1d-P3_fromYearlyData/Siberia/" + plotcode.str() + "_temp1.csv";
			string precbuf = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_climate_data/past25kyr_until2020_temp_prec/TransientMPI-ESM_Glac1d-P3_fromYearlyData/Siberia/" + plotcode.str() + "_prec1.csv";
			strcpy(dateinametemp, tempbuf.c_str());
			strcpy(dateinameprec, precbuf.c_str());
		} else if (parameter[0].weatherchoice > 1000002000 && parameter[0].weatherchoice < 1000003000){
			string tempbuf = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_climate_data/past25kyr_until2020_temp_prec/TransientMPI-ESM_Glac1d-P3_fromYearlyData/Canada/" + plotcode.str() + "_temp1.csv";
			string precbuf = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_climate_data/past25kyr_until2020_temp_prec/TransientMPI-ESM_Glac1d-P3_fromYearlyData/Canada/" + plotcode.str() + "_prec1.csv";
			strcpy(dateinametemp, tempbuf.c_str());
			strcpy(dateinameprec, precbuf.c_str());
		} else if (parameter[0].weatherchoice > 1000003000 && parameter[0].weatherchoice < 1000004000){
			string tempbuf = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_climate_data/past25kyr_until2020_temp_prec/TransientMPI-ESM_Glac1d-P3_fromYearlyData/Alaska/" + plotcode.str() + "_temp1.csv";
			string precbuf = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_climate_data/past25kyr_until2020_temp_prec/TransientMPI-ESM_Glac1d-P3_fromYearlyData/Alaska/" + plotcode.str() + "_prec1.csv";
			strcpy(dateinametemp, tempbuf.c_str());
			strcpy(dateinameprec, precbuf.c_str());
		} else if (parameter[0].weatherchoice > 1000004000 && parameter[0].weatherchoice < 1000005000){
			string tempbuf = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_climate_data/past25kyr_until2020_temp_prec/TransientMPI-ESM_Glac1d-P3_fromYearlyData/HengduanMt/" + plotcode.str() + "_temp1.csv";
			string precbuf = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_climate_data/past25kyr_until2020_temp_prec/TransientMPI-ESM_Glac1d-P3_fromYearlyData/HengduanMt/" + plotcode.str() + "_prec1.csv";
			strcpy(dateinametemp, tempbuf.c_str());
			strcpy(dateinameprec, precbuf.c_str());
		} else if (parameter[0].weatherchoice > 1000005000 && parameter[0].weatherchoice < 1000006000){
			string tempbuf = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_climate_data/past25kyr_until2020_temp_prec/TransientMPI-ESM_Glac1d-P3_fromYearlyData/MountainTreeline/" + plotcode.str() + "_temp1.csv";
			string precbuf = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_climate_data/past25kyr_until2020_temp_prec/TransientMPI-ESM_Glac1d-P3_fromYearlyData/MountainTreeline/" + plotcode.str() + "_prec1.csv";
			strcpy(dateinametemp, tempbuf.c_str());
			strcpy(dateinameprec, precbuf.c_str());
		} else if (parameter[0].weatherchoice > 1026001000 && parameter[0].weatherchoice < 1026002000){
			string tempbuf = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_climate_data/past25kyr_until2100_temp_prec/TransientMPI-ESM_Glac1d-P3_fromYearlyData/CMIP6_ssp126/Siberia/" + plotcode.str() + "_temp26_cd1.csv";
			string precbuf = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_climate_data/past25kyr_until2100_temp_prec/TransientMPI-ESM_Glac1d-P3_fromYearlyData/CMIP6_ssp126/Siberia/" + plotcode.str() + "_prec26_cd1.csv";
			strcpy(dateinametemp, tempbuf.c_str());
			strcpy(dateinameprec, precbuf.c_str());
		} else if (parameter[0].weatherchoice > 1026002000 && parameter[0].weatherchoice < 1026003000){
			string tempbuf = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_climate_data/past25kyr_until2100_temp_prec/TransientMPI-ESM_Glac1d-P3_fromYearlyData/CMIP6_ssp126/Canada/" + plotcode.str() + "_temp26_cd1.csv";
			string precbuf = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_climate_data/past25kyr_until2100_temp_prec/TransientMPI-ESM_Glac1d-P3_fromYearlyData/CMIP6_ssp126/Canada/" + plotcode.str() + "_prec26_cd1.csv";
			strcpy(dateinametemp, tempbuf.c_str());
			strcpy(dateinameprec, precbuf.c_str());
		} else if (parameter[0].weatherchoice > 1026003000 && parameter[0].weatherchoice < 1026004000){
			string tempbuf = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_climate_data/past25kyr_until2100_temp_prec/TransientMPI-ESM_Glac1d-P3_fromYearlyData/CMIP6_ssp126/Alaska/" + plotcode.str() + "_temp26_cd1.csv";
			string precbuf = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_climate_data/past25kyr_until2100_temp_prec/TransientMPI-ESM_Glac1d-P3_fromYearlyData/CMIP6_ssp126/Alaska/" + plotcode.str() + "_prec26_cd1.csv";
			strcpy(dateinametemp, tempbuf.c_str());
			strcpy(dateinameprec, precbuf.c_str());
		} else if (parameter[0].weatherchoice > 1026004000 && parameter[0].weatherchoice < 1026005000){
			string tempbuf = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_climate_data/past25kyr_until2100_temp_prec/TransientMPI-ESM_Glac1d-P3_fromYearlyData/CMIP6_ssp126/HengduanMt/" + plotcode.str() + "_temp26_cd1.csv";
			string precbuf = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_climate_data/past25kyr_until2100_temp_prec/TransientMPI-ESM_Glac1d-P3_fromYearlyData/CMIP6_ssp126/HengduanMt/" + plotcode.str() + "_prec26_cd1.csv";
			strcpy(dateinametemp, tempbuf.c_str());
			strcpy(dateinameprec, precbuf.c_str());
		} else if (parameter[0].weatherchoice > 1026005000 && parameter[0].weatherchoice < 1026006000){
			string tempbuf = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_climate_data/past25kyr_until2100_temp_prec/TransientMPI-ESM_Glac1d-P3_fromYearlyData/CMIP6_ssp126/MountainTreeline/" + plotcode.str() + "_temp26_cd1.csv";
			string precbuf = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_climate_data/past25kyr_until2100_temp_prec/TransientMPI-ESM_Glac1d-P3_fromYearlyData/CMIP6_ssp126/MountainTreeline/" + plotcode.str() + "_prec26_cd1.csv";
			strcpy(dateinametemp, tempbuf.c_str());
			strcpy(dateinameprec, precbuf.c_str());
		} else if (parameter[0].weatherchoice > 1045001000 && parameter[0].weatherchoice < 1045002000){
			string tempbuf = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_climate_data/past25kyr_until2100_temp_prec/TransientMPI-ESM_Glac1d-P3_fromYearlyData/CMIP6_ssp245/Siberia/" + plotcode.str() + "_temp45_cd1.csv";
			string precbuf = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_climate_data/past25kyr_until2100_temp_prec/TransientMPI-ESM_Glac1d-P3_fromYearlyData/CMIP6_ssp245/Siberia/" + plotcode.str() + "_prec45_cd1.csv";
			strcpy(dateinametemp, tempbuf.c_str());
			strcpy(dateinameprec, precbuf.c_str());
		} else if (parameter[0].weatherchoice > 1045002000 && parameter[0].weatherchoice < 1045003000){
			string tempbuf = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_climate_data/past25kyr_until2100_temp_prec/TransientMPI-ESM_Glac1d-P3_fromYearlyData/CMIP6_ssp245/Canada/" + plotcode.str() + "_temp45_cd1.csv";
			string precbuf = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_climate_data/past25kyr_until2100_temp_prec/TransientMPI-ESM_Glac1d-P3_fromYearlyData/CMIP6_ssp245/Canada/" + plotcode.str() + "_prec45_cd1.csv";
			strcpy(dateinametemp, tempbuf.c_str());
			strcpy(dateinameprec, precbuf.c_str());
		} else if (parameter[0].weatherchoice > 1045003000 && parameter[0].weatherchoice < 1045004000){
			string tempbuf = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_climate_data/past25kyr_until2100_temp_prec/TransientMPI-ESM_Glac1d-P3_fromYearlyData/CMIP6_ssp245/Alaska/" + plotcode.str() + "_temp45_cd1.csv";
			string precbuf = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_climate_data/past25kyr_until2100_temp_prec/TransientMPI-ESM_Glac1d-P3_fromYearlyData/CMIP6_ssp245/Alaska/" + plotcode.str() + "_prec45_cd1.csv";
			strcpy(dateinametemp, tempbuf.c_str());
			strcpy(dateinameprec, precbuf.c_str());
		} else if (parameter[0].weatherchoice > 1045004000 && parameter[0].weatherchoice < 1045005000){
			string tempbuf = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_climate_data/past25kyr_until2100_temp_prec/TransientMPI-ESM_Glac1d-P3_fromYearlyData/CMIP6_ssp245/HengduanMt/" + plotcode.str() + "_temp45_cd1.csv";
			string precbuf = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_climate_data/past25kyr_until2100_temp_prec/TransientMPI-ESM_Glac1d-P3_fromYearlyData/CMIP6_ssp245/HengduanMt/" + plotcode.str() + "_prec45_cd1.csv";
			strcpy(dateinametemp, tempbuf.c_str());
			strcpy(dateinameprec, precbuf.c_str());
		} else if (parameter[0].weatherchoice > 1045005000 && parameter[0].weatherchoice < 1045006000){
			string tempbuf = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_climate_data/past25kyr_until2100_temp_prec/TransientMPI-ESM_Glac1d-P3_fromYearlyData/CMIP6_ssp245/MountainTreeline/" + plotcode.str() + "_temp45_cd1.csv";
			string precbuf = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_climate_data/past25kyr_until2100_temp_prec/TransientMPI-ESM_Glac1d-P3_fromYearlyData/CMIP6_ssp245/MountainTreeline/" + plotcode.str() + "_prec45_cd1.csv";
			strcpy(dateinametemp, tempbuf.c_str());
			strcpy(dateinameprec, precbuf.c_str());
		} else if (parameter[0].weatherchoice > 1085001000 && parameter[0].weatherchoice < 1085002000){
			string tempbuf = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_climate_data/past25kyr_until2100_temp_prec/TransientMPI-ESM_Glac1d-P3_fromYearlyData/CMIP6_ssp585/Siberia/" + plotcode.str() + "_temp85_cd1.csv";
			string precbuf = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_climate_data/past25kyr_until2100_temp_prec/TransientMPI-ESM_Glac1d-P3_fromYearlyData/CMIP6_ssp585/Siberia/" + plotcode.str() + "_prec85_cd1.csv";
			strcpy(dateinametemp, tempbuf.c_str());
			strcpy(dateinameprec, precbuf.c_str());
		} else if (parameter[0].weatherchoice > 1085002000 && parameter[0].weatherchoice < 1085003000){
			string tempbuf = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_climate_data/past25kyr_until2100_temp_prec/TransientMPI-ESM_Glac1d-P3_fromYearlyData/CMIP6_ssp585/Canada/" + plotcode.str() + "_temp85_cd1.csv";
			string precbuf = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_climate_data/past25kyr_until2100_temp_prec/TransientMPI-ESM_Glac1d-P3_fromYearlyData/CMIP6_ssp585/Canada/" + plotcode.str() + "_prec85_cd1.csv";
			strcpy(dateinametemp, tempbuf.c_str());
			strcpy(dateinameprec, precbuf.c_str());
		} else if (parameter[0].weatherchoice > 1085003000 && parameter[0].weatherchoice < 1085004000){
			string tempbuf = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_climate_data/past25kyr_until2100_temp_prec/TransientMPI-ESM_Glac1d-P3_fromYearlyData/CMIP6_ssp585/Alaska/" + plotcode.str() + "_temp85_cd1.csv";
			string precbuf = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_climate_data/past25kyr_until2100_temp_prec/TransientMPI-ESM_Glac1d-P3_fromYearlyData/CMIP6_ssp585/Alaska/" + plotcode.str() + "_prec85_cd1.csv";
			strcpy(dateinametemp, tempbuf.c_str());
			strcpy(dateinameprec, precbuf.c_str());
		} else if (parameter[0].weatherchoice > 1085004000 && parameter[0].weatherchoice < 1085005000){
			string tempbuf = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_climate_data/past25kyr_until2100_temp_prec/TransientMPI-ESM_Glac1d-P3_fromYearlyData/CMIP6_ssp585/HengduanMt/" + plotcode.str() + "_temp85_cd1.csv";
			string precbuf = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_climate_data/past25kyr_until2100_temp_prec/TransientMPI-ESM_Glac1d-P3_fromYearlyData/CMIP6_ssp585/HengduanMt/" + plotcode.str() + "_prec85_cd1.csv";
			strcpy(dateinametemp, tempbuf.c_str());
			strcpy(dateinameprec, precbuf.c_str());
		} else if (parameter[0].weatherchoice > 1085005000 && parameter[0].weatherchoice < 1085006000){
			string tempbuf = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_climate_data/past25kyr_until2100_temp_prec/TransientMPI-ESM_Glac1d-P3_fromYearlyData/CMIP6_ssp585/MountainTreeline/" + plotcode.str() + "_temp85_cd1.csv";
			string precbuf = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_climate_data/past25kyr_until2100_temp_prec/TransientMPI-ESM_Glac1d-P3_fromYearlyData/CMIP6_ssp585/MountainTreeline/" + plotcode.str() + "_prec85_cd1.csv";
			strcpy(dateinametemp, tempbuf.c_str());
			strcpy(dateinameprec, precbuf.c_str());
		} else if (parameter[0].weatherchoice > 2000001000 && parameter[0].weatherchoice < 2000002000){
			string tempbuf = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_climate_data/past25kyr_until2020_temp_prec/TransientMPI-ESM1_2_from100yrMeans/Siberia/" + plotcode.str() + "_temp2.csv";
			string precbuf = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_climate_data/past25kyr_until2020_temp_prec/TransientMPI-ESM1_2_from100yrMeans/Siberia/" + plotcode.str() + "_prec2.csv";
			strcpy(dateinametemp, tempbuf.c_str());
			strcpy(dateinameprec, precbuf.c_str());
		} else if (parameter[0].weatherchoice > 2000002000 && parameter[0].weatherchoice < 2000003000){
			string tempbuf = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_climate_data/past25kyr_until2020_temp_prec/TransientMPI-ESM1_2_from100yrMeans/Canada/" + plotcode.str() + "_temp2.csv";
			string precbuf = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_climate_data/past25kyr_until2020_temp_prec/TransientMPI-ESM1_2_from100yrMeans/Canada/" + plotcode.str() + "_prec2.csv";
			strcpy(dateinametemp, tempbuf.c_str());
			strcpy(dateinameprec, precbuf.c_str());
		}

        if (parameter[0].demlandscape) {
            parameter[0].tempjandiffort = parameter[0].temperaturelapse_jan;
            parameter[0].tempjandiffortmin = parameter[0].temperaturelapse_jan * 1000;  // calculate changes per 1000 m to use later for tree growth estimation
            parameter[0].tempjuldiffort = parameter[0].temperaturelapse_jul;
            parameter[0].tempjuldiffortmin = parameter[0].temperaturelapse_jul * 1000;
            parameter[0].tempdiffort = 0;
            parameter[0].precdiffort = parameter[0].precipitationlapse_year;
            parameter[0].precdiffortmin = parameter[0].precipitationlapse_year * 1000;
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
    } // end world loop

    passWeather();
	
	if(parameter[0].allow_pest_disturbances) { // read and compute pest outbreaks based on weather
		Getdisturbanceclimresponse();
		cout << "Pest species present in simulation:";
		for (int pest_species_counter = 1; pest_species_counter < 99; pest_species_counter++) {
			cout << "pest_species_counter= " << pest_species_counter << " -> " << pesttrait[pest_species_counter].pestspecies << "   ";
			if(pesttrait[pest_species_counter].pestspecies != pest_species_counter) {
				parameter[0].pest_species_max = pest_species_counter-1;
				break;
			}
		}
		cout << parameter[0].pest_species_max << endl;
		
	    int aktort = 0;
		for (vector<vector<Weather>>::iterator posw = world_weather_list.begin(); posw != world_weather_list.end(); posw++) {
			vector<Weather>& weather_list = *posw;
			aktort++;

			Getdisturbanceimpact(weather_list);
		}// for each weather element
	}

}

