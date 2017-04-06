#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <math.h>
#include <algorithm>
#include <cmath>
#include <sstream>
#include <iomanip>
#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>  
#include<ctime>
#define unitrand rand/(RAND_MAX+1.0)

// #include <netcdf>
// #include <netcdfcpp.h>
// #include <netcdf.h>
// #include <ncvalues.h>

using namespace std;
//     using namespace netCDF;
//    using namespace netCDF::exceptions;

// float pi=3.14159;
//wichtig:

// float windDir(float u,float v){
//   float r;
//   r=fmod((270-atan2(v,u)*180/pi),360);
//    if(v>0)	r=((180/pi)*atan(u/v)+180);
//    if(u<0 & v<0)	r=((180/pi)*atan(u/v)+0);
//    if(u>0 & v<0)	r=((180/pi)*atan(u/v)+360);
//   return r;
// }

float winddir(int yr){
  
  int i,cntr=0;
  
  vector<int> year, month, day, hour,assignedwdir,assignedwspd, assigned;
  
  vector<float> lat,lng,elev,wdir,wspd,temp,press,u,v;
  
  float wdirsum=0,wspdsum=0;
  
  vector<vector<double> > values;
  
  vector<double> valueline;

  
    ifstream file("khatanga_hourly1.csv");
    string item,era; 
    
    std::ostringstream ss;
    
    for (string line; getline(file, line); ){
        istringstream file(line);
        while (getline(file, item, ',')){
             valueline.push_back(atof(item.c_str())); 	//ZEILENWEISE
	     cntr++;
        //atof converts string to double(!)//what does atof do to "NA"???-> Number~10**-15
	}
        values.push_back(valueline);	//pushback Spaltenvektoren in die Gesamtmatrix
        valueline.clear();
		}

    int N=(cntr/15);
    int Ninsg=(7*365*(2012-1937));//das ist 3-stündig!!!
    
//      cout<<"N="<<N<<" von "<<Ninsg<<" Werten vorhanden (1937 bis 2012 täglich 7 Werte)"<<endl;
    
    //ALLOCATION-> needs to be done for every vector used
    year.resize(N);month.resize(N);day.resize(N);hour.resize(N);
    lat.resize(N);lng.resize(N);elev.resize(N);wdir.resize(N);
    wspd.resize(N);temp.resize(N);press.resize(N);
    assignedwdir.resize(N),assignedwspd.resize(N), assigned.resize(N);
    u.resize(N);v.resize(N);///SCHONMAL ALLE WERTE drinhaben->Ninsg
//     lambda.resize(N);
    
    cntr=0;
    wdirsum=0;
    
     for(i=0;i<N;i++){
       year[i]=values[i][2];
       month[i]=values[i][3];
       day[i]=values[i][4];
       hour[i]=values[i][5];
       elev[i]=values[i][9];
       if(values[i][11]!=atof("NA")){
	 wspd[i]=values[i][11]*0.51444;//Knoten zu m/s
	 assignedwspd[i]=1;
       }else{assignedwspd[i]=0;}
       if(values[i][10]!=atof("NA")){
	 wdir[i]=values[i][10];
	 assignedwdir[i]=1;
// 	 u[i]=-wspd[i]*sin((wdir[i])*pi/180);
// 	 v[i]=-wspd[i]*cos((wdir[i])*pi/180);
	 //Windgeschwindigkeiten=0 -> winddir=NA abfangen:
       }else if(values[i][10]==atof("NA") and values[i][11]==0){ 
	 assignedwdir[i]=1; assignedwspd[i]=1; wdir[i]=0;wspd[i]=0;
//        }else if(values[i][10]
       }else{assignedwdir[i]=0;}
       //cout<<wdir[i]<<endl;
      assigned[i]=assignedwdir[i]*assignedwspd[i];
     }
  
  for(i=0;i<N;i++){
    if((yr==year[i])&&(assigned[i]==1)) wdirsum+=wdir[i];wspdsum+=wspd[i];cntr++;
    }
    wdirsum/=cntr;
//    }
  return wdirsum;
}

float windspd(int yr){
  
  int i,cntr=0;
  vector<int> year, month, day, hour,assignedwdir,assignedwspd, assigned;
  vector<float> lat,lng,elev,wdir,wspd,temp,press,u,v;
  float wdirsum=0,wspdsum=0;
  vector<vector<double> > values;
    vector<double> valueline;
  
    ifstream file("khatanga_hourly1.csv");
    string item,era; 
    
    std::ostringstream ss;
    
    for (string line; getline(file, line); ){
        istringstream file(line);
        while (getline(file, item, ',')){
             valueline.push_back(atof(item.c_str())); 	//ZEILENWEISE
	     cntr++;
        //atof converts string to double(!)//what does atof do to "NA"???-> Number~10**-15
	}
        values.push_back(valueline);	//pushback Spaltenvektoren in die Gesamtmatrix
        valueline.clear();}

    int N=(cntr/15);
    int Ninsg=(7*365*(2012-1937));//das ist 3-stündig!!!
    
//      cout<<"N="<<N<<" von "<<Ninsg<<" Werten vorhanden (1937 bis 2012 täglich 7 Werte)"<<endl;
    
    //ALLOCATION-> needs to be done for every vector used
    year.resize(N);month.resize(N);day.resize(N);hour.resize(N);
    lat.resize(N);lng.resize(N);elev.resize(N);wdir.resize(N);
    wspd.resize(N);temp.resize(N);press.resize(N);
    assignedwdir.resize(N),assignedwspd.resize(N), assigned.resize(N);
    u.resize(N);v.resize(N);///SCHONMAL ALLE WERTE drinhaben->Ninsg
//     lambda.resize(N);
    
    cntr=0;
    wdirsum=0;
    
     for(i=0;i<N;i++){
       year[i]=values[i][2];
       month[i]=values[i][3];
       day[i]=values[i][4];
       hour[i]=values[i][5];
       elev[i]=values[i][9];
       if(values[i][11]!=atof("NA")){
	 wspd[i]=values[i][11]*0.51444;//Knoten zu m/s
	 assignedwspd[i]=1;
       }else{assignedwspd[i]=0;}
       if(values[i][10]!=atof("NA")){
	 wdir[i]=values[i][10];
	 assignedwdir[i]=1;
// 	 u[i]=-wspd[i]*sin((wdir[i])*pi/180);
// 	 v[i]=-wspd[i]*cos((wdir[i])*pi/180);
	 //Windgeschwindigkeiten=0 -> winddir=NA abfangen:
       }else if(values[i][10]==atof("NA") and values[i][11]==0){ 
	 assignedwdir[i]=1; assignedwspd[i]=1; wdir[i]=0;wspd[i]=0;
//        }else if(values[i][10]
       }else{assignedwdir[i]=0;}
       //cout<<wdir[i]<<endl;
      assigned[i]=assignedwdir[i]*assignedwspd[i];
     }
  
//    for(j=yr;j=2013-1937;j++){
  for(i=0;i<N;i++){
    if((yr==year[i])&&(assigned[i]==1)) wdirsum+=wdir[i];wspdsum+=wspd[i];cntr++;
    }
    wspdsum/=cntr;
//    }
  return wspdsum;
}