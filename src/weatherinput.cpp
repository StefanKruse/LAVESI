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

            Weather pWeather{};

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
					droughtmortbuf=pow(droughtmortbuf/0.3863,2);
                }
            }
            weather_list[iweather].droughtmort = droughtmortbuf;
            if (duerreindexmin > 1) {
                double aattrockenheitmin = (weather_list[iweather].activeairtempmin / 300.0) - 1.0;

                if (aattrockenheitmin > 1) {
                    droughtmortbufmin = droughtmortbufmin + aattrockenheitmin * 0.1;
					droughtmortbufmin=pow(droughtmortbufmin/0.3863,2); //this part is new. min did not exist before.
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
		} else if ((parameter[0].weatherchoice == 1000000401) /* ### FOR 2023051101 ### */ ||
		(parameter[0].weatherchoice == 1000000402) /* ### FOR 2023051102 ### */ ||
		(parameter[0].weatherchoice == 1000000403) /* ### FOR 2023051103 ### */ ||
		(parameter[0].weatherchoice == 1000000404) /* ### FOR 2023051201 ### */ ||
		(parameter[0].weatherchoice == 1000000405) /* ### FOR 2023051202 ### */ ||
		(parameter[0].weatherchoice == 1000000406) /* ### FOR 2023051203 ### */ ||
		(parameter[0].weatherchoice == 1000000407) /* ### FOR 2023051301 ### */ ||
		(parameter[0].weatherchoice == 1000000408) /* ### FOR 2023051302 ### */ ||
		(parameter[0].weatherchoice == 1000000409) /* ### FOR 2023051401 ### */ ||
		(parameter[0].weatherchoice == 1000000410) /* ### FOR 2023051402 ### */ ||
		(parameter[0].weatherchoice == 1000000463) /* ### FOR 2023060603 ### */ ||
		(parameter[0].weatherchoice == 1000000464) /* ### FOR 2023060604 ### */ ||
		(parameter[0].weatherchoice == 1000000465) /* ### FOR 2023060701 ### */ ||
		(parameter[0].weatherchoice == 1000000466) /* ### FOR 2023060702 ### */ ||
		(parameter[0].weatherchoice == 1000000467) /* ### FOR 2023060703 ### */ ||
		(parameter[0].weatherchoice == 1000000468) /* ### FOR 2023060704 ### */ ||
		(parameter[0].weatherchoice == 1000000469) /* ### FOR 2023060705 ### */ ||
		(parameter[0].weatherchoice == 1000000470) /* ### FOR 2023060706 ### */ ||
		(parameter[0].weatherchoice == 1000000471) /* ### FOR 2023060802 ### */ ||
		(parameter[0].weatherchoice == 1000000472)) /* ### FOR 2023060803 ### */ {
			foldername << "cell_40";
		} else if ((parameter[0].weatherchoice == 1000000411) /* ### FOR 2023051701 ### */ ||
		(parameter[0].weatherchoice == 1000000412) /* ### FOR 2023051702 ### */ ||
		(parameter[0].weatherchoice == 1000000419) /* ### FOR 2023051808 ### */ ||
		(parameter[0].weatherchoice == 1000000450) /* ### FOR 2023060101 ### */ ||
		(parameter[0].weatherchoice == 1000000451)) /* ### FOR 2023060102 ### */ {
			foldername << "cell_41";
		} else if ((parameter[0].weatherchoice == 1000000458) /* ### FOR 2023060502 ### */ ||
		(parameter[0].weatherchoice == 1000000459)) /* ### FOR 2023060503 ### */ {
			foldername << "cell_42";
		} else if ((parameter[0].weatherchoice == 1000000457) /* ### FOR 2023060301 ### */ ||
		(parameter[0].weatherchoice == 1000000460) /* ### FOR 2023060504 ### */ ||
		(parameter[0].weatherchoice == 1000000461) /* ### FOR 2023060505 ### */ ||
		(parameter[0].weatherchoice == 1000000462)) /* ### FOR 2023060601 ### */ {
			foldername << "cell_43";
		} else if ((parameter[0].weatherchoice == 1000000413) /* ### FOR 2023051801 ### */ ||
		(parameter[0].weatherchoice == 1000000414) /* ### FOR 2023051802 ### */ ||
		(parameter[0].weatherchoice == 1000000415) /* ### FOR 2023051803 ### */ ||
		(parameter[0].weatherchoice == 1000000416) /* ### FOR 2023051804 ### */ ||
		(parameter[0].weatherchoice == 1000000417) /* ### FOR 2023051806 ### */ ||
		(parameter[0].weatherchoice == 1000000418) /* ### FOR 2023051807 ### */ ||
		(parameter[0].weatherchoice == 1000000420) /* ### FOR 2023051901 ### */ ||
		(parameter[0].weatherchoice == 1000000421) /* ### FOR 2023051902 ### */ ||
		(parameter[0].weatherchoice == 1000000422) /* ### FOR 2023052101 ### */ ||
		(parameter[0].weatherchoice == 1000000423) /* ### FOR 2023052102 ### */ ||
		(parameter[0].weatherchoice == 1000000424) /* ### FOR 2023052103 ### */ ||
		(parameter[0].weatherchoice == 1000000425) /* ### FOR 2023052104 ### */ ||
		(parameter[0].weatherchoice == 1000000426) /* ### FOR 2023052106 ### */ ||
		(parameter[0].weatherchoice == 1000000427) /* ### FOR 2023052107 ### */ ||
		(parameter[0].weatherchoice == 1000000438) /* ### FOR 2023052802 ### */ ||
		(parameter[0].weatherchoice == 1000000439) /* ### FOR 2023052803 ### */ ||
		(parameter[0].weatherchoice == 1000000440) /* ### FOR 2023052804 ### */ ||
		(parameter[0].weatherchoice == 1000000441) /* ### FOR 2023052805 ### */ ||
		(parameter[0].weatherchoice == 1000000442) /* ### FOR 2023052806 ### */ ||
		(parameter[0].weatherchoice == 1000000443) /* ### FOR 2023052807 ### */ ||
		(parameter[0].weatherchoice == 1000000444) /* ### FOR 2023052901 ### */ ||
		(parameter[0].weatherchoice == 1000000445) /* ### FOR 2023052902 ### */ ||
		(parameter[0].weatherchoice == 1000000446) /* ### FOR 2023052903 ### */ ||
		(parameter[0].weatherchoice == 1000000447) /* ### FOR 2023053001 ### */ ||
		(parameter[0].weatherchoice == 1000000448) /* ### FOR 2023053002 ### */ ||
		(parameter[0].weatherchoice == 1000000449) /* ### FOR 2023053003 ### */ ||
		(parameter[0].weatherchoice == 1000000452) /* ### FOR 2023060105 ### */ ||
		(parameter[0].weatherchoice == 1000000453) /* ### FOR 2023060106 ### */ ||
		(parameter[0].weatherchoice == 1000000454) /* ### FOR 2023060201 ### */ ||
		(parameter[0].weatherchoice == 1000000455) /* ### FOR 2023060203 ### */ ||
		(parameter[0].weatherchoice == 1000000456)) /* ### FOR 2023060204 ### */ {
			foldername << "cell_44";
		} else if ((parameter[0].weatherchoice == 50001)) /* ### FOR Close_to_Fairbanks_EN23-611 ### */ {
			foldername << "cell_32";
		} else if ((parameter[0].weatherchoice == 50002) /* ### FOR Fielding_Lake_EN23-675 ### */ ||
		(parameter[0].weatherchoice == 50009)) /* ### FOR Road_to_Central_EN23-651 ### */ {
			foldername << "cell_33";
		} else if ((parameter[0].weatherchoice == 50003) /* ### FOR Fort_Mc_Pherson_East_EN22-021 ### */ ||
		(parameter[0].weatherchoice == 50004)) /* ### FOR Fort_Mc_Pherson_West_EN22-032 ### */ {
			foldername << "cell_20";
		} else if ((parameter[0].weatherchoice == 50005)) /* ### FOR Lake_Illerney_16KP01-V1_V20_Chukotka_2018_001-027 ### */ {
			foldername << "cell_24";
		} else if ((parameter[0].weatherchoice == 50006)) /* ### FOR Lake_Nutenvut_16KP03-V20_V39 ### */ {
			foldername << "cell_19";
		} else if ((parameter[0].weatherchoice == 50007)) /* ### FOR Lake_Ulu_EN21-201_219 ### */ {
			foldername << "cell_17";
		} else if ((parameter[0].weatherchoice == 50008)) /* ### FOR Mucho_Lake_EN22-065 ### */ {
			foldername << "cell_1";
		} else {
			cout << "No wind data for weather choice available, reading data for Chukotka!X" << endl;
            foldername << "wind_Chukotka"; // TODO: make local version possible retaining only few test plots for a proof-of-running
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

            if ((jahr < findyr2 + 1) && (jahr > findyr1 - 1)) {
                ss << jahr;

                if (parameter[0].windsource == 1) {
                    filename = "input/" + foldername.str() + "/winddata" + ss.str() + "_EraInterim.dat";
                } else if (parameter[0].windsource == 10) {
					filename = "input/" + foldername.str() + "/winddata" + ss.str() + "_ERA5.dat";
				} else if (parameter[0].windsource == 999) {
					filename = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_climate_data/wind_last25kyr_fromYearlyData/" + foldername.str() + "/winddata_" + ss.str() + ".dat";
				} else if (parameter[0].windsource == 998) {
					filename = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_climate_data/wind_last25kyr_from100yrMeans/" + foldername.str() + "/winddata_" + ss.str() + ".dat";
				}
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
            }// each year

        jahr = 0;
    }

    int aktort = 0;
    for (vector<vector<Weather>>::iterator posw = world_weather_list.begin(); posw != world_weather_list.end(); posw++) {
        vector<Weather>& weather_list = *posw;
        aktort++;

		// depending on the weather choice different files will be opened and read line by line
		long int plotcodeNum;
		plotcodeNum = parameter[0].weatherchoice - 1000000000;
		
		std::stringstream plotcode;
		plotcode << std::setw(3) << std::setfill('0') << plotcodeNum;

			
		if (parameter[0].weatherchoice > 1000000000 && parameter[0].weatherchoice < 2000000000){
			string tempbuf = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_climate_data/temp_prec_last25kyr_fromYearlyData/" + plotcode.str() + "_temp1.csv";
			string precbuf = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_climate_data/temp_prec_last25kyr_fromYearlyData/" + plotcode.str() + "_prec1.csv";
			strcpy(dateinametemp, tempbuf.c_str());
			strcpy(dateinameprec, precbuf.c_str());
		} else if (parameter[0].weatherchoice > 2000000000 && parameter[0].weatherchoice < 3000000000){
			string tempbuf = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_climate_data/temp_prec_last25kyr_from100yrMeans/" + plotcode.str() + "_temp2.csv";
			string precbuf = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_climate_data/temp_prec_last25kyr_from100yrMeans/" + plotcode.str() + "_prec2.csv";
			strcpy(dateinametemp, tempbuf.c_str());
			strcpy(dateinameprec, precbuf.c_str());
		} else if (parameter[0].weatherchoice == 50001) {
			char tempbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_climate_data/temp_prec_last25kyr_fromYearlyData/Close_to_Fairbanks_EN23-611_temp1.csv";
			char precbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_climate_data/temp_prec_last25kyr_fromYearlyData/Close_to_Fairbanks_EN23-611_prec1.csv";
			strcpy(dateinametemp, tempbuf);
			strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 50002) {
			char tempbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_climate_data/temp_prec_last25kyr_fromYearlyData/Fielding_Lake_EN23-675_temp1.csv";
			char precbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_climate_data/temp_prec_last25kyr_fromYearlyData/Fielding_Lake_EN23-675_prec1.csv";
			strcpy(dateinametemp, tempbuf);
			strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 50003) {
			char tempbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_climate_data/temp_prec_last25kyr_fromYearlyData/Fort_Mc_Pherson_East_EN22-021_temp1.csv";
			char precbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_climate_data/temp_prec_last25kyr_fromYearlyData/Fort_Mc_Pherson_East_EN22-021_prec1.csv";
			strcpy(dateinametemp, tempbuf);
			strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 50004) {
			char tempbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_climate_data/temp_prec_last25kyr_fromYearlyData/Fort_Mc_Pherson_West_EN22-032_temp1.csv";
			char precbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_climate_data/temp_prec_last25kyr_fromYearlyData/Fort_Mc_Pherson_West_EN22-032_prec1.csv";
			strcpy(dateinametemp, tempbuf);
			strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 50005) {
			char tempbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_climate_data/temp_prec_last25kyr_fromYearlyData/Lake_Illerney_16KP01-V1_V20_Chukotka_2018_001-027_temp1.csv";
			char precbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_climate_data/temp_prec_last25kyr_fromYearlyData/Lake_Illerney_16KP01-V1_V20_Chukotka_2018_001-027_prec1.csv";
			strcpy(dateinametemp, tempbuf);
			strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 50006) {
			char tempbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_climate_data/temp_prec_last25kyr_fromYearlyData/Lake_Nutenvut_16KP03-V20_V39_temp1.csv";
			char precbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_climate_data/temp_prec_last25kyr_fromYearlyData/Lake_Nutenvut_16KP03-V20_V39_prec1.csv";
			strcpy(dateinametemp, tempbuf);
			strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 50007) {
			char tempbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_climate_data/temp_prec_last25kyr_fromYearlyData/Lake_Ulu_EN21-201_219_temp1.csv";
			char precbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_climate_data/temp_prec_last25kyr_fromYearlyData/Lake_Ulu_EN21-201_219_prec1.csv";
			strcpy(dateinametemp, tempbuf);
			strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 50008) {
			char tempbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_climate_data/temp_prec_last25kyr_fromYearlyData/Mucho_Lake_EN22-065_temp1.csv";
			char precbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_climate_data/temp_prec_last25kyr_fromYearlyData/Mucho_Lake_EN22-065_prec1.csv";
			strcpy(dateinametemp, tempbuf);
			strcpy(dateinameprec, precbuf);
		} else if (parameter[0].weatherchoice == 50009) {
			char tempbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_climate_data/temp_prec_last25kyr_fromYearlyData/Road_to_Central_EN23-651_temp1.csv";
			char precbuf[] = "/albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_climate_data/temp_prec_last25kyr_fromYearlyData/Road_to_Central_EN23-651_prec1.csv";
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
