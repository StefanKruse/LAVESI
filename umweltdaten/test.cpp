#include<stdio.h>
#include<iostream>
#include<fstream>
#include<stdlib.h>
#include<string>
#include<cstring>
#include<sstream>
//#include<unistd.h>	// beinhaltet getcwd()-Funktion erstmal auskommentiert

// fuer Zufallszahlen und exit()-Funktion
#include<cstdlib>
#include<ctime>
#include<time.h>

// fuer mathematische Funktionen
#define _USE_MATH_DEFINES
#include<math.h>

// fuer Container (vector)
#include<vector>
#include<list>

using namespace std;

int main()
{
	int rowtest=50,coltest=50;
	int i,j,k,l,N1,rowcntr=0,colcntr=0;
	//double **vegetationtype=malloc(Ntest * sizeof *vegetationtype + (Ntest * (Ntest * sizeof **vegetationtype)));//malloc returns a void value, can not be transformed in C++
	double **vegetationtype= new double*[rowtest];
for(int i = 0; i < rowtest; ++i){vegetationtype[i] = new double[coltest];}
	vector<vector<double>> values;
	vector<double> valueline ;
	
	string item;
	
	double buffer;
	char *puffer;
	
	string filename="test.txt";
	
	ifstream input(filename.c_str());
	
	
	for(j=0;j<rowtest;j++){
		for(i=0;i<coltest;i++){//initialization w dummy 66
			vegetationtype[j][i]=66;
		}
	}
	
	
	for(string line;getline(input,line);){
		istringstream input(line);
		colcntr=0;
		while(getline(input,item,';')){
			valueline.push_back(atof(item.c_str()));
			colcntr++;
		//	cout<<item<<" ";
		}
		//cout<<endl;
		values.push_back(valueline);
		valueline.clear();
		rowcntr++;
	}


	input.close();
	
	//cout<<colcntr<<" "<<rowcntr<<endl;
	
	for(k=0;k<rowcntr;k++){
	for(l=0;l<colcntr;l++){
		cout<<values[k][l]<<" ";
	}
	cout<<endl;
	}
	
	
if(rowcntr<rowtest){//kleine Umweltdatenmatrix auf große Datenmatrix
	cout<<"kl auf gr\n";
	for(k=0;k<rowcntr;k++){
	for(l=0;l<colcntr;l++){
		for(j=k*floor(rowcntr/rowtest);j<(k+1)*(int)(rowcntr/rowtest);j++){
			cout<<j<<endl;
			for(i=l*floor(colcntr/coltest);i<(l+1)*(int)(colcntr/coltest);i++){
				vegetationtype[j][i]=values[k][l];
				}
			}
			//cout<<values[k][l];
	}
	//cout<<endl;
	}
	
	for(j=0;j<rowtest;j++){
		for(i=0;i<coltest;i++){//replacement of dummy 66
			if((vegetationtype[j][i]==66)&&(i!=0)&&(vegetationtype[j][i-1]!=66)){
				vegetationtype[j][i]=vegetationtype[j][i-1];
			}else if((vegetationtype[j][i]==66)&&(j!=0)&&(i==0)&&(vegetationtype[j-1][i]!=66)){
				vegetationtype[j][i]=vegetationtype[j-1][i];//naja
			}
		}
	}
	
	
		for(j=0;j<rowtest;j++){
		for(i=0;i<coltest;i++){//replacement of dummy 66,
			if((vegetationtype[j][i]==66)&&(i==0)&&(j!=0)){
				vegetationtype[j][i]=vegetationtype[j-1][i];
			}else if((vegetationtype[j][i]==66)&&(i!=0)&&(j==0)){
				vegetationtype[j][i]=vegetationtype[j][i-1];//naja
			}else if((vegetationtype[j][i]==66)&&(j!=0)&&(i!=0)){vegetationtype[j][i]=vegetationtype[j][i-1];}
		}
		}
		
		//smoothing:
		for(j=0;j<rowtest;j++){
		for(i=0;i<coltest;i++){
			if((i==0)&&(j!=0)&&(vegetationtype[j][i]!=vegetationtype[j-1][i])){
				vegetationtype[j][i]*=3.0;//Gewichtung
				vegetationtype[j][i]+=vegetationtype[j-1][i];
				vegetationtype[j][i]*=0.25;
			}else if((vegetationtype[j][i]==vegetationtype[j][i-1])&&(i!=0)&&(j==0)){
				vegetationtype[j][i]*=3;
				vegetationtype[j][i]+=vegetationtype[j][i-1];
				vegetationtype[j][i]*=0.25;
			}else if((j!=0)&&(i!=0)&&(vegetationtype[j][i]!=vegetationtype[j-1][i-1])){
				vegetationtype[j][i]*=2;
				vegetationtype[j][i]+=vegetationtype[j-1][i-1]+vegetationtype[j-1][i]+vegetationtype[j][i-1];
				vegetationtype[j][i]*=0.2;}
		}
		}
		
}else{//rowcntr>rowtest
	//große Umweltdatenm auf kleine Datenm (fehlerhaft)
	cout<<"gr auf kl\n";
	
	
	// ///04.08.16:KORRIGIERTE VERSION gross auf klein::
	
	for(i=0;i<coltest;i++){
		for(j=0;j<rowtest;j++){
			vegetationtype[j][i]-=66;
			for(l=i*(int)((colcntr-1)/(coltest-1));l<(i+1)*(int)((colcntr-1)/(coltest));l++){
				for(k=j*(int)((rowcntr-1)/(rowtest-1));k<(j+1)*(int)((rowcntr-1)/(rowtest));k++){
					vegetationtype[j][i]+=values[k][l];
				}	
			}
			vegetationtype[j][i]/=(2*(colcntr*rowcntr)/(coltest*rowtest));
		}	
	}
	//::, smoothing entfällt
	
	
	//for(k=0;k<rowcntr;k++){
	//for(l=0;l<colcntr;l++){
	//	for(j=k*(int)(rowtest/rowcntr);j<(k+1)*(int)(rowtest/rowcntr);j++){
	//		cout<<j<<endl;
	//		for(i=l*floor(coltest/colcntr);i<(l+1)*(int)(coltest/colcntr);i++){
	//			vegetationtype[j][i]=values[k][l];
	//			}
	//		}
	//}
	//}
	
		for(j=0;j<rowtest;j++){
		for(i=0;i<coltest;i++){//replacement of dummy 66,
			if((vegetationtype[j][i]==66)&&(i==0)&&(j!=0)){
				vegetationtype[j][i]=vegetationtype[j-1][i];
			}else if((vegetationtype[j][i]==66)&&(i!=0)&&(j==0)){
				vegetationtype[j][i]=vegetationtype[j][i-1];//naja
			}else if((vegetationtype[j][i]==66)&&(j!=0)&&(i!=0)){vegetationtype[j][i]=vegetationtype[j][i-1];}
		}
		}
		
		//smoothing:
		/*for(j=0;j<rowtest;j++){
		for(i=0;i<coltest;i++){
			if((i==0)&&(j!=0)&&(vegetationtype[j][i]!=vegetationtype[j-1][i])){
				vegetationtype[j][i]*=3.0;
				vegetationtype[j][i]+=vegetationtype[j-1][i];
				vegetationtype[j][i]*=0.25;
			}else if((vegetationtype[j][i]==vegetationtype[j][i-1])&&(i!=0)&&(j==0)){
				vegetationtype[j][i]*=3;
				vegetationtype[j][i]+=vegetationtype[j][i-1];
				vegetationtype[j][i]*=0.25;
			}else if((j!=0)&&(i!=0)&&(vegetationtype[j][i]!=vegetationtype[j-1][i-1])){
				vegetationtype[j][i]*=2;
				vegetationtype[j][i]+=vegetationtype[j-1][i-1]+vegetationtype[j-1][i]+vegetationtype[j][i-1];
				vegetationtype[j][i]*=0.2;}
		}
		}*/
		
		
}


	//printf statt speichern
	for(j=0;j<rowtest;j++){
		for(i=0;i<coltest;i++){
			cout<<vegetationtype[j][i]<<" ";
		}
		cout<<endl;
	}
		
}