#include "LAVESI.h"

using namespace std;

/****************************************************************************************//**
 * \brief 
 *
 * calculate simple density value for each tree
 *
 *
 *
 *******************************************************************************************/
void AddTreeDensity(list<Tree*> &tree_list, vector<Envirgrid*> &plot_list)
{
	for (list<Tree*>::iterator pos = tree_list.begin(); pos != tree_list.end(); )
	{
		pTree=(*pos);
		
		int i=(int) floor(pTree->ycoo*parameter[0].sizemagnif);
		int j=(int) floor(pTree->xcoo*parameter[0].sizemagnif);

		double flaechengroesze=0.0;
		if (parameter[0].calcinfarea == 1) //linearly increasing
			flaechengroesze = pTree->dbasal * parameter[0].incfac/100.0; 
		else if (parameter[0].calcinfarea == 2) //linearly increasing
			flaechengroesze = pTree->dbasal * (2/3) * parameter[0].incfac/100.0; 
		else if (parameter[0].calcinfarea == 3) //linearly increasing
			flaechengroesze = pTree->dbasal * (4/3) * parameter[0].incfac/100.0; 
		else if (parameter[0].calcinfarea == 4) //logistic growth function with maximum at 8 m
			flaechengroesze= ( 9/( 1+( ( (1/0.1)-1 )*exp(-0.1*pTree->dbasal) ) ) )-1;
		else if (parameter[0].calcinfarea == 5) //logistic growth function with maximum at 8 m
			flaechengroesze= ( 9/( 1+( ( (1/0.1)-1 )*exp(-0.2*pTree->dbasal) ) ) )-1;
		else if (parameter[0].calcinfarea == 6) //logistic growth function with maximum at 8 m
			flaechengroesze= ( 9/( 1+( ( (1/0.1)-1 )*exp(-0.5*pTree->dbasal) ) ) )-1;
				
		// if the trees influences only one density grid cell
		if ( flaechengroesze<(1.0/parameter[0].sizemagnif) )
		{
			plot_list[i*treecols*parameter[0].sizemagnif+j]->Treedensityvalue += 
									pow(
												pow(flaechengroesze/(1.0/parameter[0].sizemagnif),parameter[0].densitysmallweighing)
												//weighing with diameter
												*
												pow(pTree->dbasal, parameter[0].densitytreetile),
										parameter[0].densityvaluemanipulatorexp);
			plot_list[i*treecols*parameter[0].sizemagnif+j]->Treenumber++;
			// pTree->densitywert=pow(pTree->dbasal, parameter[0].densitytreetile);
			pTree->densitywert=pow(
							pow(pTree->dbasal, parameter[0].densitytreetile)*pow(flaechengroesze/(1.0/parameter[0].sizemagnif),parameter[0].densitysmallweighing),
							parameter[0].densityvaluemanipulatorexp);
		}
		// if the trees influences more than one density grid cell
		else
		{
			// determine dimensions of the grid around the tree
			int xyquerrastpos= (int) floor( flaechengroesze*parameter[0].sizemagnif );
			// determine shifted coordinates and adding up the density value
			double sumdensitywert=0;
			
			for (int rastposi=(i+xyquerrastpos); rastposi>(i-(xyquerrastpos+1)); rastposi--)
			{
				for (int rastposj=(j-xyquerrastpos); rastposj<(j+xyquerrastpos+1); rastposj++)
				{
					if ( ( rastposi<=( (treerows-1)*parameter[0].sizemagnif) && rastposi>=0 ) && ( rastposj<=( (treecols-1)*parameter[0].sizemagnif) && rastposj>=0 ) )
					{
						
						// Distance calculation to determine the influence of the density value in spatial units ...
						// ... and inserting the value at every position
						double entfrastpos=sqrt(pow(double(i-rastposi),2)+pow(double(j-rastposj),2));
						// only if the current grid cell is part of the influence area, a value is assigned
						if (entfrastpos<= (double) xyquerrastpos)
						{
							plot_list[rastposi*treecols*parameter[0].sizemagnif+rastposj]->Treedensityvalue 
								+= 
								pow(
									pow(pTree->dbasal, parameter[0].densitytreetile)/(entfrastpos+1.0),
									parameter[0].densityvaluemanipulatorexp);
								
							plot_list[rastposi*treecols*parameter[0].sizemagnif+rastposj]->Treenumber++;

							sumdensitywert+=pow(
												pow(pTree->dbasal, parameter[0].densitytreetile)/(entfrastpos+1.0),
												parameter[0].densityvaluemanipulatorexp);
						}
					}
				}
			}
							
			pTree->densitywert=sumdensitywert;
		}

		++pos;
	}
}

void PrepareCryogrid(list<Tree*> &tree_list, vector<Cryogrid*> &cryo_list)
{
	// setup and wipe grid
	double sizemagnifcryo =  ((double) parameter[0].sizemagnif) / 50;
	// cout << sizemagnifcryo << " sizemagnifcryo " << endl;

	// TODO: maybe move to intializing
	// cout << (int) (ceil(treerows*sizemagnifcryo) * ceil(treecols*sizemagnifcryo)) << " = cells in cryogrid (plotupdate)" << endl;	
	for(int kartenpos=0; kartenpos < (int) (ceil(treerows*sizemagnifcryo) * ceil(treecols*sizemagnifcryo)); kartenpos++)
	{
		pCryogrid = cryo_list[kartenpos];
		
		pCryogrid->leafarea = 0;
		pCryogrid->stemarea = 0;
		pCryogrid->maxtreeheight = 0;
		pCryogrid->treecount = 0;
		pCryogrid->meantreeheight = 0;
	}
	
	for (list<Tree*>::iterator pos = tree_list.begin(); pos != tree_list.end(); )
	{
		pTree=(*pos);
		
		// int i=(int) floor(pTree->ycoo*parameter[0].sizemagnif/50);
		int i=(int) floor(pTree->ycoo*sizemagnifcryo);
		// int j=(int) floor(pTree->xcoo*parameter[0].sizemagnif/50);
		int j=(int) floor(pTree->xcoo*sizemagnifcryo);
		
		// cout << i << " i & " << j << " j " << endl;
		
		
		// TODO: leaf- and stemarea distribution
		// TODO: impact area for leaf- stemarea check
		double crownradius=exp(0.9193333*log(pTree->dbasal) + 2.4618496) / 100; //in m
		
		// cout << crownradius << endl;
		// cout << cryo_list.size() << " = length cryo_list" << endl;
		// cout << world_cryo_list.size() << " = length world_cryo_list" << endl;
		
		double cryogridcellarea = pow(1.0/sizemagnifcryo, 2);
		// cout << cryogridcellarea << " cellarea | "  << endl ;

		// if the trees influences only one density grid cell
		if ( crownradius < (1.0/sizemagnifcryo) )
		{
			cryo_list[i*ceil(treecols*sizemagnifcryo)+j]->leafarea += 23.99583 * pTree->dbasal * 2 / cryogridcellarea;//TODO check parameterization 
			cryo_list[i*ceil(treecols*sizemagnifcryo)+j]->stemarea += M_PI * pTree->dbasal * sqrt( pow(pTree->dbasal/100,2) + pow(pTree->height/100,2) )  / cryogridcellarea;
			if(pTree->height/100 > cryo_list[i*ceil(treecols*sizemagnifcryo)+j]->maxtreeheight)
				cryo_list[i*ceil(treecols*sizemagnifcryo)+j]->maxtreeheight = pTree->height/100;
			cryo_list[i*ceil(treecols*sizemagnifcryo)+j]->meantreeheight += pTree->height/100;
			cryo_list[i*ceil(treecols*sizemagnifcryo)+j]->treecount++;
		}
		// if the trees influences more than one density grid cell
		else
		{
			// determine dimensions of the grid around the tree
			int xyquerrastpos= (int) floor(crownradius*sizemagnifcryo);
			
			// TODO: check
			double fractionimpactpertile = cryogridcellarea / (M_PI * pow(crownradius,2));
			
			// determine shifted coordinates and adding up the density value
			double leafareaoftreepertile = fractionimpactpertile * 23.99583 * pTree->dbasal * 2;
			double stemareaoftreepertile = fractionimpactpertile * M_PI * pTree->dbasal * sqrt( pow(pTree->dbasal/100,2) + pow(pTree->height/100,2) );
			
			for (int rastposi=(i+xyquerrastpos); rastposi>(i-(xyquerrastpos+1)); rastposi--)
			{
				for (int rastposj=(j-xyquerrastpos); rastposj<(j+xyquerrastpos+1); rastposj++)
				{
					if ( ( rastposi<=( ceil((treerows-1)*sizemagnifcryo)) && rastposi>=0 ) && ( rastposj<=( ceil((treecols-1)*sizemagnifcryo)) && rastposj>=0 ) )
					{
						// Distance calculation to determine the influence of the density value in spatial units ...
						// ... and inserting the value at every position
						double entfrastpos=sqrt(pow(double(i-rastposi),2)+pow(double(j-rastposj),2));
						// only if the current grid cell is part of the influence area, a value is assigned
						if (entfrastpos <= (double) xyquerrastpos)
						{
							cryo_list[rastposi*ceil(treecols*sizemagnifcryo)+rastposj]->leafarea += leafareaoftreepertile;
							cryo_list[rastposi*ceil(treecols*sizemagnifcryo)+rastposj]->stemarea += stemareaoftreepertile;
							if(pTree->height/100 > cryo_list[rastposi*ceil(treecols*sizemagnifcryo)+rastposj]->maxtreeheight)
								cryo_list[rastposi*ceil(treecols*sizemagnifcryo)+rastposj]->maxtreeheight = pTree->height/100;
							cryo_list[rastposi*ceil(treecols*sizemagnifcryo)+rastposj]->meantreeheight += pTree->height/100;
							cryo_list[rastposi*ceil(treecols*sizemagnifcryo)+rastposj]->treecount++;
						}
					}
				}
			}
		}

		++pos;
	}
	// process mean value of tree height
	// cryo_list[i*ceil(treecols*sizemagnifcryo)+j]->meantreeheight/cryo_list[i*ceil(treecols*sizemagnifcryo)+j]->treecount;
	for(int kartenpos=0; kartenpos < (int) (ceil(treerows*sizemagnifcryo) * ceil(treecols*sizemagnifcryo)); kartenpos++)
	{
		pCryogrid = cryo_list[kartenpos];

		pCryogrid->meantreeheight = pCryogrid->meantreeheight / (double) pCryogrid->treecount;
	}

}
	
void UpdateCryogrid(list<Tree*> &tree_list, vector<Cryogrid*> &cryo_list)
{
	// rationale:
	// 1. compile data and write to file 
	// 2. call CryoGrid and estimate permafrost thaw depth
	// 3. read thaw depth and assign thaw depth to Environment-grid by interpolation from 10 x 10 m grid to 0.2 x 0.2 m grid
	
	// declaration of used vectors 
	std::vector<double> leafareai;
	std::vector<double> stemareai;
	std::vector<double> maxtreeheighti;
	std::vector<double> meantreeheighti;

	if(true)
	{// 1. compile data and write to file
		FILE *filepointer;
		string filename;

		// assemble file name:
		ostringstream s1,s2;
		s1 << std::setfill('0') << std::setw(5) << parameter[0].ivort; // TODO: replace or add current year here
		s2 << std::setfill('0') << std::setw(10) << parameter[0].weatherchoice;
		// s3 << std::setfill('0') << std::setw(6) << 1998;
		filename="/legacy/Model/Modelling/CryogridLAVESI/CouplingMaster/output/cryogridoutput_" +s1.str()+"_"+s2.str()+"_aggregated.txt"; // TODO: add relative path
		// s1.str("");s1.clear();s2.str("");s2.clear();s2.str("");s2.clear();

		// trying to open the file for reading
		filepointer = fopen (filename.c_str(), "r+");
		// if fopen fails, open a new file + header output
		if (filepointer == NULL)
		{
			filepointer = fopen (filename.c_str(), "w+");
			
			int yeartocryogrid=1979;
			if(parameter[0].currentyear>1979 && parameter[0].currentyear<=2100)// Range of EraInterim data for CryoGrid Forcing
				yeartocryogrid=parameter[0].currentyear;
			fprintf(filepointer, "%d;current year;;;;", yeartocryogrid);//TODO: check if year is correctly assigned
			fprintf(filepointer, "\n");
			
			fprintf(filepointer, "LAVESI output file for CryoGrid input;");
			fprintf(filepointer, "min;");
			fprintf(filepointer, "25quantile;");
			fprintf(filepointer, "median;");
			fprintf(filepointer, "75quantile;");
			fprintf(filepointer, "max");

			fprintf(filepointer, "\n");

			if (filepointer == NULL)
			{
				fprintf(stderr, "Error: output file is missing!\n");
				exit(1);
			}
		}

		fseek(filepointer,0,SEEK_END);

		// data evaluation and output
		double sizemagnifcryo =  ((double) parameter[0].sizemagnif) /50;
		
		for (int kartenpos=0; kartenpos< (ceil(treerows*sizemagnifcryo) *ceil(treecols*sizemagnifcryo)); kartenpos++)
		{
			pCryogrid=cryo_list[kartenpos];

			leafareai.push_back(pCryogrid->leafarea);
			stemareai.push_back(pCryogrid->stemarea);
			maxtreeheighti.push_back(pCryogrid->maxtreeheight);
			meantreeheighti.push_back(pCryogrid->meantreeheight);
		}
		std::sort (leafareai.begin(), leafareai.end());
		std::sort (stemareai.begin(), stemareai.end());
		std::sort (maxtreeheighti.begin(), maxtreeheighti.end());
		std::sort (meantreeheighti.begin(), meantreeheighti.end());
	
	
	
		// std::cout << "The min is " << leafareai[0] << '\n';
		// std::cout << "The 25% quartile is " << leafareai[leafareai.size()/2/2] << '\n';
		// std::cout << "The median is " << leafareai[leafareai.size()/2] << '\n';
		// std::cout << "The 75% quartile is " << leafareai[leafareai.size()/2+leafareai.size()/2/2] << '\n';
		// std::cout << "The max is " << leafareai[leafareai.size()-1] << '\n';
		fprintf(filepointer, "leafareaindex;");
		fprintf(filepointer, "%10.1f;", leafareai[0]);
		fprintf(filepointer, "%10.1f;", leafareai[leafareai.size()/2/2]);
		fprintf(filepointer, "%10.1f;", leafareai[leafareai.size()/2]);
		fprintf(filepointer, "%10.1f;", leafareai[leafareai.size()/2+leafareai.size()/2/2]);
		fprintf(filepointer, "%10.1f", leafareai[leafareai.size()-1]);
		fprintf(filepointer, "\n");
		
		// Iterate and print values of vector
		// for(double n : leafareai) {
			// std::cout << n << '\n';
		// }
		
		// std::cout << "The min is " << stemareai[0] << '\n';
		// std::cout << "The 25% quartile is " << stemareai[stemareai.size()/2/2] << '\n';
		// std::cout << "The median is " << stemareai[stemareai.size()/2] << '\n';
		// std::cout << "The 75% quartile is " << stemareai[stemareai.size()/2+stemareai.size()/2/2] << '\n';
		// std::cout << "The max is " << stemareai[stemareai.size()-1] << '\n';
		fprintf(filepointer, "stemareaindex;");
		fprintf(filepointer, "%10.2f;", stemareai[0]);
		fprintf(filepointer, "%10.2f;", stemareai[stemareai.size()/2/2]);
		fprintf(filepointer, "%10.2f;", stemareai[stemareai.size()/2]);
		fprintf(filepointer, "%10.2f;", stemareai[stemareai.size()/2+stemareai.size()/2/2]);
		fprintf(filepointer, "%10.2f", stemareai[stemareai.size()-1]);
		fprintf(filepointer, "\n");
		
		// Iterate and print values of vector
		// for(double n : stemareai) {
			// std::cout << n << '\n';
		// }
	
		fprintf(filepointer, "mean50percent2maxtreeheight_m;");
		fprintf(filepointer, "%3.3f;", (meantreeheighti[0] + maxtreeheighti[0])/2);
		fprintf(filepointer, "%3.3f;", (meantreeheighti[meantreeheighti.size()/2/2] + maxtreeheighti[maxtreeheighti.size()/2/2])/2);
		fprintf(filepointer, "%3.3f;", (meantreeheighti[meantreeheighti.size()/2] + maxtreeheighti[maxtreeheighti.size()/2])/2);
		fprintf(filepointer, "%3.3f;", (meantreeheighti[meantreeheighti.size()/2+meantreeheighti.size()/2/2] + maxtreeheighti[maxtreeheighti.size()/2+maxtreeheighti.size()/2/2])/2);
		fprintf(filepointer, "%3.3f", (meantreeheighti[meantreeheighti.size()-1] + maxtreeheighti[maxtreeheighti.size()-1])/2);
		fprintf(filepointer, "\n");
		
		// std::cout << "The min is " << maxtreeheighti[0] << '\n';
		// std::cout << "The 25% quartile is " << maxtreeheighti[maxtreeheighti.size()/2/2] << '\n';
		// std::cout << "The median is " << maxtreeheighti[maxtreeheighti.size()/2] << '\n';
		// std::cout << "The 75% quartile is " << maxtreeheighti[maxtreeheighti.size()/2+maxtreeheighti.size()/2/2] << '\n';
		// std::cout << "The max is " << maxtreeheighti[maxtreeheighti.size()-1] << '\n';
		fprintf(filepointer, "maxtreeheight_m;");
		fprintf(filepointer, "%3.3f;", maxtreeheighti[0]);
		fprintf(filepointer, "%3.3f;", maxtreeheighti[maxtreeheighti.size()/2/2]);
		fprintf(filepointer, "%3.3f;", maxtreeheighti[maxtreeheighti.size()/2]);
		fprintf(filepointer, "%3.3f;", maxtreeheighti[maxtreeheighti.size()/2+maxtreeheighti.size()/2/2]);
		fprintf(filepointer, "%3.3f", maxtreeheighti[maxtreeheighti.size()-1]);
		fprintf(filepointer, "\n");

		// std::cout << "The min is " << meantreeheighti[0] << '\n';
		// std::cout << "The 25% quartile is " << meantreeheighti[meantreeheighti.size()/2/2] << '\n';
		// std::cout << "The median is " << meantreeheighti[meantreeheighti.size()/2] << '\n';
		// std::cout << "The 75% quartile is " << meantreeheighti[meantreeheighti.size()/2+meantreeheighti.size()/2/2] << '\n';
		// std::cout << "The max is " << meantreeheighti[meantreeheighti.size()-1] << '\n';
		fprintf(filepointer, "meantreeheight_m;");
		fprintf(filepointer, "%3.3f;", meantreeheighti[0]);
		fprintf(filepointer, "%3.3f;", meantreeheighti[meantreeheighti.size()/2/2]);
		fprintf(filepointer, "%3.3f;", meantreeheighti[meantreeheighti.size()/2]);
		fprintf(filepointer, "%3.3f;", meantreeheighti[meantreeheighti.size()/2+meantreeheighti.size()/2/2]);
		fprintf(filepointer, "%3.3f", meantreeheighti[meantreeheighti.size()-1]);
		fprintf(filepointer, "\n");

		// Iterate and print values of vector
		// for(double n : maxtreeheighti) {
			// std::cout << n << '\n';
		// }
		// TODO: delete later only for testing
		
		fclose(filepointer);
	}

	if(true)
	{// 2. call CryoGrid and estimate permafrost thaw depth
		// main.m from "/legacy/Model/Modelling/CryogridLAVESI/CouplingMaster/CryoGrid/sample_CryoVeg"
		// data read from in "/legacy/Model/Modelling/CryogridLAVESI/CouplingMaster/output"
		
		cout << "try to called matlab at " << clock() << endl;
		system("matlab -nodisplay -nosplash -nodesktop -r \"run('/legacy/Model/Modelling/CryogridLAVESI/CouplingMaster/CryoGrid/sample_CryoVeg/main_LAVESI.m');exit;\""); // TODO: make path relative
		cout << "after called matlab at " << clock() << endl;
		// exit(1);
	}

	if(true)
	{// 3. read thaw depth and assign thaw depth to Environment-grid by interpolation from 10 x 10 m grid to 0.2 x 0.2 m grid
		// TODO: clean couts
		// from "/legacy/Model/Modelling/CryogridLAVESI/CouplingMaster/output"
		
		// cryogridoutput_00025_0000007001_aggregated.txt_CG.csv
		
		FILE *filepointer;
		string filename;
		ostringstream s1,s2;

		// assemble file name
		// ... use same as above //TODO: call it only one for matching
		s1 << std::setfill('0') << std::setw(5) << parameter[0].ivort; // TODO: replace or add current year here
		s2 << std::setfill('0') << std::setw(10) << parameter[0].weatherchoice;

		filename="/legacy/Model/Modelling/CryogridLAVESI/CouplingMaster/output/cryogridoutput_" +s1.str()+"_"+s2.str()+"_aggregated.txt_CG.csv"; // TODO: add relative path

		// trying to open the file for reading
		filepointer = fopen (filename.c_str(), "r+");
		
		// if fopen fails just do no update of the Envirgrid
		if (filepointer == NULL)
		{
			fprintf(stderr, "Error: cryogidresponse file is missing!\n");
		} else
		{
			// setup leafareaiout aggregated
			std::vector<double> leafareaiout;
			leafareaiout.push_back(leafareai[0]);
			leafareaiout.push_back(leafareai[leafareai.size()/2/2]);
			leafareaiout.push_back(leafareai[leafareai.size()/2]);
			leafareaiout.push_back(leafareai[leafareai.size()/2+leafareai.size()/2/2]);
			leafareaiout.push_back(leafareai[leafareai.size()-1]);

			// read file per line
			char linebuffer[255];
			std::vector<double> activelayerdepthin;

			int counter = 1; // to skip first line of file 
			while (fgets(linebuffer, stringlengthmax, filepointer) != NULL)
			{
				// cout << linebuffer << " - read in -> "; // TODO: only for testing, delete
				
				if (counter>=2)
				{
					activelayerdepthin.push_back(strtod(linebuffer, NULL));
					
					// cout << activelayerdepthin[activelayerdepthin.size()-1] << endl; // print current value // TODO: only for testing, delete
					// cout << leafareaiout[activelayerdepthin.size()-1] << " leafarea " << endl; // print current value // TODO: only for testing, delete
				}
				counter++;
			}
			cout << "length of activelayerdepthin = " << activelayerdepthin.size() << endl;// TODO: only for testing, delete
			
			fclose(filepointer);
						
			// setup linear model for interpolation
			// y{activelayerdepthin} ~ x{leafareaiout}
			
			// calculation
			
				// n vals
				int xn=leafareaiout.size();
				int yn=activelayerdepthin.size();
				// only if equal proceed
				
				// cout << xn << " xn and yn " << yn << endl;
				
				// mean x
			    double sum = 0;
				for(int i = 0; i < xn; i++)
				{
					sum += leafareaiout[i];
					// cout << leafareaiout[i] << " / " << endl;
				}
				double xmean = sum/xn;
				// diff square sums
			    double xvariance;
				for(int i = 0; i < xn; i++)
					xvariance += pow(xmean-leafareaiout[i],2);
				// mean y
			    sum = 0;
				for(int i = 0; i < xn; i++)
				{
					sum += activelayerdepthin[i];
					// cout << activelayerdepthin[i] << " / " << endl;
				}
				double ymean = sum/xn;
				// diff square sums
			    double yxcovariance;
				for(int i = 0; i < xn; i++)
					yxcovariance += (xmean-leafareaiout[i]) * (ymean-activelayerdepthin[i]);
				// slope
				parameter[0].altslope = yxcovariance / xvariance;
				parameter[0].altintercept = ymean - parameter[0].altslope * xmean;
				
				cout << " .. slope = " << parameter[0].altslope << endl;
				cout << " .. intercept = " << parameter[0].altintercept << endl;	
		}
	}
}

double estimateALT(double pai_i, double degredayi_i)
{
	double alt_i = 0.0;
	double spike = -2.85000 + 0.00275*degredayi_i;
	
	if(pai_i<200)
		alt_i = 1.8666667 + 0.0006667*pai_i;
	else if(pai_i>200 & pai_i<350)
		alt_i = 2.0 + ((spike-2.0)/(350-200))*(pai_i-200);
	else if(pai_i>200 & pai_i<350)
		alt_i = spike + ((2.2-spike)/(500-350))*(pai_i-350);
	else if(pai_i>500)
		alt_i = 1.8666667 + 0.0006667*pai_i;
		
	return(alt_i);
}

void UpdateEnvirgridALD(vector<Cryogrid*> &cryo_list, vector<Envirgrid*> &plot_list, vector<Weather*> &weather_list, int yearposition)
{
	
	// estimate data to Cryogrid
	// ... if negative values set to zero
	double sizemagnifcryo =  ((double) parameter[0].sizemagnif) / 50;
	
	if(parameter[0].callcryogrid==true)// when ALT was calculated by CryoGrid
	{
		cout << " .. slope = " << parameter[0].altslope << endl;
		cout << " .. intercept = " << parameter[0].altintercept << endl;	
		for (int kartenpos=0; kartenpos< (ceil(treerows*sizemagnifcryo) *ceil(treecols*sizemagnifcryo)); kartenpos++)
		{
			pCryogrid=cryo_list[kartenpos];

			double activelayeri = parameter[0].altslope * pCryogrid->leafarea + parameter[0].altintercept;
			if(activelayeri<0)
				activelayeri=0;
			pCryogrid->maxthawing_depth = activelayeri * 1000.0; // conversion from meter input to mm
			
			// cout << " leafarea=" << pCryogrid->leafarea << " -> ALD=" << pCryogrid->maxthawing_depth << endl;
		}
	}
	else// prior CryoGrid-parameterized ALT estimation
	{
		// assess climate data (Active Air Temperature (Sum(T>0)), AAT_>0, here named degreday) of current year
		// ... and estimate ALT for each LAI+SAI=PAI value on the grid

		for (int kartenpos=0; kartenpos< (ceil(treerows*sizemagnifcryo) *ceil(treecols*sizemagnifcryo)); kartenpos++)
		{
			pCryogrid=cryo_list[kartenpos];

			double activelayeri = estimateALT(pCryogrid->leafarea+pCryogrid->stemarea, weather_list[yearposition]->degreday);
			if(activelayeri<0)
				activelayeri=0;
			pCryogrid->maxthawing_depth = activelayeri * 1000.0; // conversion from meter input to mm
		}		
	}
	// TODO: clean couts
	for (int i = 0; i < plot_list.size(); i++)
	{		
		// calculate weighted mean of surrounding 3x3 grid Cryogrid-maxthawing_depth values for each Envirgrid-Tile

		// find position in Cryogrid
		double sizemagnifcryo =  ((double) parameter[0].sizemagnif) / 50;

		double yii = (sizemagnifcryo * plot_list[i]->ycoo / parameter[0].sizemagnif);
		double xii = (sizemagnifcryo * plot_list[i]->xcoo / parameter[0].sizemagnif);
		int yi = (int) floor(yii);
		int xi = (int) floor(xii);

		// cout << endl << endl << "  plot coo x,y = " << plot_list[i]->xcoo << "," << plot_list[i]->ycoo << endl;
		// cout << " coo in Cryogrid x,y = " << xi << "," << yi << endl;

		// access surrounding tiles values
		// ... and calculate distance to Envirgrid-tile
		// for testing only if all exist
		if(yi>1 & xi>1 & yi<((sizemagnifcryo*treecols)-1) & xi<((sizemagnifcryo*treecols)-1))
		{	
			// cout << endl << endl << "  plot coo x,y = " << plot_list[i]->xcoo << "," << plot_list[i]->ycoo << endl;
			// cout << " coo in Cryogrid x,y = " << xi << "," << yi << endl;
			
			std::vector<double> cgvals;
			std::vector<double> dists;

			// access data
			// cout << endl << cryo_list[(yi+1)*sizemagnifcryo*treecols+xi-1]->maxthawing_depth; // NW
			// cout << " " << cryo_list[(yi+1)*sizemagnifcryo*treecols+xi]->maxthawing_depth; // N
			// cout << " " << cryo_list[(yi+1)*sizemagnifcryo*treecols+xi+1]->maxthawing_depth; // NE
			cgvals.push_back(cryo_list[(yi+1)*sizemagnifcryo*treecols+xi-1]->maxthawing_depth); // NW
			cgvals.push_back(cryo_list[(yi+1)*sizemagnifcryo*treecols+xi]->maxthawing_depth); // N
			cgvals.push_back(cryo_list[(yi+1)*sizemagnifcryo*treecols+xi+1]->maxthawing_depth); // NE
				// distance to curent tile
				// cout << " - " << sqrt(pow(1-(yii - (double) yi),2) + pow(-1-(xii - (double) xi),2));
				// cout << " " << sqrt(pow(1-(yii - (double) yi),2) + pow(0-(xii - (double) xi),2));
				// cout << " " << sqrt(pow(1-(yii - (double) yi),2) + pow(1-(xii - (double) xi),2));
				dists.push_back(sqrt(pow(1-(yii - (double) yi),2) + pow(-1-(xii - (double) xi),2)));
				dists.push_back(sqrt(pow(1-(yii - (double) yi),2) + pow(0-(xii - (double) xi),2)));
				dists.push_back(sqrt(pow(1-(yii - (double) yi),2) + pow(1-(xii - (double) xi),2)));
			
			// cout << endl << cryo_list[yi*sizemagnifcryo*treecols+xi-1]->maxthawing_depth; // W
			// cout << " " << cryo_list[yi*sizemagnifcryo*treecols+xi]->maxthawing_depth; // actual
			// cout << " " << cryo_list[yi*sizemagnifcryo*treecols+xi+1]->maxthawing_depth; // E
			cgvals.push_back(cryo_list[yi*sizemagnifcryo*treecols+xi-1]->maxthawing_depth); // W
			cgvals.push_back(cryo_list[yi*sizemagnifcryo*treecols+xi]->maxthawing_depth); // actual
			cgvals.push_back(cryo_list[yi*sizemagnifcryo*treecols+xi+1]->maxthawing_depth); // E
				// distance to curent tile
				// cout << " - " << sqrt(pow(0-(yii - (double) yi),2) + pow(-1-(xii - (double) xi),2));
				// cout << " " << sqrt(pow(0-(yii - (double) yi),2) + pow(0-(xii - (double) xi),2));
				// cout << " " << sqrt(pow(0-(yii - (double) yi),2) + pow(1-(xii - (double) xi),2));
				dists.push_back(sqrt(pow(0-(yii - (double) yi),2) + pow(-1-(xii - (double) xi),2)));
				dists.push_back(sqrt(pow(0-(yii - (double) yi),2) + pow(0-(xii - (double) xi),2)));
				dists.push_back(sqrt(pow(0-(yii - (double) yi),2) + pow(1-(xii - (double) xi),2)));

			// cout << endl << cryo_list[(yi-1)*sizemagnifcryo*treecols+xi-1]->maxthawing_depth; // SW
			// cout << " " << cryo_list[(yi-1)*sizemagnifcryo*treecols+xi]->maxthawing_depth; // S
			// cout << " " << cryo_list[(yi-1)*sizemagnifcryo*treecols+xi+1]->maxthawing_depth; // SE
			cgvals.push_back(cryo_list[(yi-1)*sizemagnifcryo*treecols+xi-1]->maxthawing_depth); // SW
			cgvals.push_back(cryo_list[(yi-1)*sizemagnifcryo*treecols+xi]->maxthawing_depth); // S
			cgvals.push_back(cryo_list[(yi-1)*sizemagnifcryo*treecols+xi+1]->maxthawing_depth); // SE
				// distance to curent tile
				// cout << " - " << sqrt(pow(-1-(yii - (double) yi),2) + pow(-1-(xii - (double) xi),2));
				// cout << " " << sqrt(pow(-1-(yii - (double) yi),2) + pow(0-(xii - (double) xi),2));
				// cout << " " << sqrt(pow(-1-(yii - (double) yi),2) + pow(1-(xii - (double) xi),2));
				dists.push_back(sqrt(pow(-1-(yii - (double) yi),2) + pow(-1-(xii - (double) xi),2)));
				dists.push_back(sqrt(pow(-1-(yii - (double) yi),2) + pow(0-(xii - (double) xi),2)));
				dists.push_back(sqrt(pow(-1-(yii - (double) yi),2) + pow(1-(xii - (double) xi),2)));
				
			// cout << endl << dists.size() << endl;
			// cout << cgvals.size() << endl;
			
			// 1 - dist/maxdist
			double maxdist = *max_element(dists.begin(), dists.end());
			double sumdist = accumulate(dists.begin(), dists.end(), 0.0);
			// cout << endl << "maxdist=" << maxdist << endl;
			// cout << endl << "sumdist=" << sumdist << endl;
			double sumdistsscaled = 0;
			for( int veci = 0; veci < cgvals.size() ; veci++)
			{
				sumdistsscaled += 1/(dists[veci]/sumdist);
				dists[veci] = 1/(dists[veci]/sumdist);
			}
			double weightedmean = 0;
			for( int veci = 0; veci < cgvals.size() ; veci++)
			{
				cgvals[veci] = cgvals[veci] * (dists[veci]/sumdistsscaled);
				weightedmean += cgvals[veci];
			}
			
			// cout << " ... ... dists and cgvals ... ... " << endl;
			// for( int veci = 0; veci < cgvals.size() ; veci++)
			// {
				// cout << "veci=" << veci << ": d=" << dists[veci] << "  & ald=" << cgvals[veci] << endl;
			// }
			// weightedmean = weightedmean/cgvals.size();
			// cout << "weightedmean=" << weightedmean << endl;
			
			// cout << endl;
			
			plot_list[i]->maxthawing_depth = weightedmean;
		}
	// cout << endl << " ... end UpdateEnvirgridALD in loop " << endl;
	}
	// cout << endl << " ... end UpdateEnvirgridALD post loop " << endl;
}


/****************************************************************************************//**
 * \brief 
 *
 * loop around every tree to determine it's individual density value \n
 * that's the value with which it influences other trees
 *
 * 3 kinds of density calculation are possible \n
 * 1: no density calculated and densitywert=0.0 \n
 *
 * 2: ZOI-kind of type and pTree->densitywert=1.0-(density_help/plot_list[i*treecols*parameter[0].sizemagnif+j]->Treedensityvalue) or \n
 *							sumdensitywert+=plot_list[rastposi*treecols*parameter[0].sizemagnif+rastposj]->Treedensityvalue; \n
 *							pTree->densitywert=1.0-(density_help/sumdensitywert);	
 *							pTree->densitywert= pTree->densitywert *pow((1.0-(0.01/pTree->dbasal)),parameter[0].densityvaluedbasalinfluence);	
 *
 * 3: random field of ZOI-Type and pTree->densitywert=1.0-(density_help/plot_list[izuf*treecols*parameter[0].sizemagnif+jzuf]->Treedensityvalue) or \n
 *								   sumdensitywert+=plot_list[izuf*treecols*parameter[0].sizemagnif+jzuf]->Treedensityvalue \n
 *								   pTree->densitywert=1.0-(density_help/sumdensitywert);
 *								   pTree->densitywert= pTree->densitywert *pow((1.0-(0.01/pTree->dbasal)),parameter[0].densityvaluedbasalinfluence);
 *******************************************************************************************/

void IndividualTreeDensity(list<Tree*> &tree_list, vector<Envirgrid*> &plot_list)
{
	if(parameter[0].omp_num_threads==1)
	{// only one core
		for (list<Tree*>::iterator pos = tree_list.begin(); pos != tree_list.end(); ++pos)
		{
			pTree=(*pos);

			int i = (int) floor(pTree->ycoo * parameter[0].sizemagnif);
			int j = (int) floor(pTree->xcoo * parameter[0].sizemagnif);

			//DENSITY 1
			if (parameter[0].densitymode == 1)
			{
				pTree->densitywert = 0.0;
			}
			else 
			{
				double flaechengroesze=0.0;
				if (parameter[0].calcinfarea == 1) //linearly increasing
					flaechengroesze = pTree->dbasal * parameter[0].incfac/100.0; 
				else if (parameter[0].calcinfarea == 2) //linearly increasing
					flaechengroesze = pTree->dbasal * (2/3) * parameter[0].incfac/100.0; 
				else if (parameter[0].calcinfarea == 3) //linearly increasing
					flaechengroesze = pTree->dbasal * (4/3) * parameter[0].incfac/100.0; 
				else if (parameter[0].calcinfarea == 4) //logistic growth function with maximum at 8 m
					flaechengroesze= ( 9/( 1+( ( (1/0.1)-1 )*exp(-0.1*pTree->dbasal) ) ) )-1;
				else if (parameter[0].calcinfarea == 5) //logistic growth function with maximum at 8 m
					flaechengroesze= ( 9/( 1+( ( (1/0.1)-1 )*exp(-0.2*pTree->dbasal) ) ) )-1;
				else if (parameter[0].calcinfarea == 6) //logistic growth function with maximum at 8 m
					flaechengroesze= ( 9/( 1+( ( (1/0.1)-1 )*exp(-0.5*pTree->dbasal) ) ) )-1;

				// if the tree only influences one grid cell
				if ( flaechengroesze < (1.0/parameter[0].sizemagnif) )
				{
					// DENSITY 2
					if (parameter[0].densitymode == 2)
					{
						if (plot_list[i*treecols*parameter[0].sizemagnif+j]->Treedensityvalue > 0.0) 
						{
							if (parameter[0].densitytiletree==1)	// sum of values
							{
								pTree->densitywert =	(1.0 - (pTree->densitywert / plot_list[i*treecols*parameter[0].sizemagnif+j]->Treedensityvalue));
									//                           density_desired(at position) / density_currently(at position)
							}
							else if (parameter[0].densitytiletree==2)	// multiplication of values
							{
								pTree->densitywert =	(1.0 - (pTree->densitywert
															/ 
															(plot_list[i*treecols*parameter[0].sizemagnif+j]->Treedensityvalue*pTree->densitywert) 
															)
													);
									//                           density_desired(at position) / density_currently(at position)
							}
						}
						else
						{
							pTree->densitywert=0.0; // no competition
						}
					}
					// DENSITY 3
					else if (parameter[0].densitymode == 3)
					{
						// determine random grid cell position and assign a value
						int izuf= (int) floor( 0.0 + ( (double)  ( ((double) (treerows-1)) * parameter[0].sizemagnif * rand() / (RAND_MAX + 1.0))) );
						int jzuf= (int) floor( 0.0 + ( (double)  ( ((double) (treecols-1)) * parameter[0].sizemagnif * rand() / (RAND_MAX + 1.0))) );
						
						if (plot_list[izuf*treecols*parameter[0].sizemagnif+jzuf]->Treedensityvalue > 0.0) 
						{
							if (parameter[0].densitytiletree==1)	// sum of values
							{
								pTree->densitywert = (1.0 - (pTree->densitywert/ plot_list[izuf*treecols*parameter[0].sizemagnif+jzuf]->Treedensityvalue));
									//                           density_desired(at position) / density_currently(at position)
							}
							else if (parameter[0].densitytiletree==2)	// multiplication of values
							{
								pTree->densitywert =	(1.0 - (pTree->densitywert
															/ 
															(plot_list[i*treecols*parameter[0].sizemagnif+j]->Treedensityvalue*pTree->densitywert) 
															)
													);
							}
						}
						else
						{
							pTree->densitywert = 0.0;
						}
					}
					
					// TODO: clean code
					// calculate the thawing depth influence on the tree growth
					// if ((plot_list[i*treecols*parameter[0].sizemagnif+j]->maxthawing_depth<2000) && (parameter[0].thawing_depth==true)) 
					// {
						pTree->thawing_depthinfluence = plot_list[i*treecols*parameter[0].sizemagnif+j]->maxthawing_depth / 1000.0; // scaling to m (1000.0 mm = 1.0 m)
					// }
					// else
					// {
						// pTree->thawing_depthinfluence=100;
					// }
				}
				else
				{
					// determine dimensions of the grid around the tree
					int xyquerrastpos= (int) floor( flaechengroesze*parameter[0].sizemagnif );
					double maxdist= sqrt(pow(double(xyquerrastpos),2)+pow(double(xyquerrastpos),2));

					// determine shifted coordinates and adding up the density value
					double sumdensitywert=0;
					double sumthawing_depth=0;
					
					unsigned int anzahlflaechen=0;
					
					for (int rastposi=(i+xyquerrastpos); rastposi>(i-(xyquerrastpos+1)); rastposi--)
					{
						for (int rastposj=(j-xyquerrastpos); rastposj<(j+xyquerrastpos+1); rastposj++)
						{
							if ((rastposi<=( (treerows-1)*parameter[0].sizemagnif) && rastposi>=0) && ( rastposj<=( (treecols-1)*parameter[0].sizemagnif) && rastposj>=0))
							{
								// distance calculation to determine the influence of the density value in spatial unit
								// ... similar to value insertion above
								double entfrastpos=sqrt(pow(double(i-rastposi),2)+pow(double(j-rastposj),2));
								
								// only if the current grid cell is a part of the influenced area, a value is assigned
								if (entfrastpos<= (double) xyquerrastpos)
								{
									int icurr=rastposi;
									int jcurr=rastposj;

									// change of i/j vals only if DENSITY 3
									if (parameter[0].densitymode==3)
									{
										// calculate the position from where the density values are taken randomly
										icurr= (int) floor( 0.0 + ( (double)  ( ((double) (treerows-1)) *parameter[0].sizemagnif*rand()/(RAND_MAX + 1.0))) );
										jcurr= (int) floor( 0.0 + ( (double)  ( ((double) (treecols-1)) *parameter[0].sizemagnif*rand()/(RAND_MAX + 1.0))) );
									}
									
									if (parameter[0].densitytiletree==1)	// sum of values
									{
										sumdensitywert+=plot_list[icurr*treecols*parameter[0].sizemagnif+jcurr]->Treedensityvalue
														* (1-entfrastpos/maxdist);
									}
									else if (parameter[0].densitytiletree==2)	// multiplication of values
									{
										// after weighting the additional values by the individual influence values the offset is added
										sumdensitywert+=(plot_list[icurr*treecols*parameter[0].sizemagnif+jcurr]->Treedensityvalue
														-pow(pTree->dbasal, parameter[0].densitytreetile)/(entfrastpos+1.0))
														* pow(pTree->dbasal, parameter[0].densitytreetile)/(entfrastpos+1.0)
														+ pow(pTree->dbasal, parameter[0].densitytreetile)/(entfrastpos+1.0);
									}

									sumthawing_depth+=(double) plot_list[rastposi*treecols*parameter[0].sizemagnif+rastposj]->maxthawing_depth;
									anzahlflaechen++;
								}
							}
						}
					}

					if (sumdensitywert > 0.0) 
						pTree->densitywert = 1.0 - (pTree->densitywert/sumdensitywert);
					//                           density_desired(at position) / density_currently(at position)
					else 
						pTree->densitywert=0.0;
						
					sumthawing_depth=( sumthawing_depth / (double) anzahlflaechen );
					
					// TODO: clean code
					// if (sumthawing_depth  <2000)
						pTree->thawing_depthinfluence = sumthawing_depth / 1000.0; // scaling to m (1000.0 mm = 1.0 m)
					// else
						// pTree->thawing_depthinfluence = 100;
				}

				pTree->densitywert= pTree->densitywert*pow((1.0-(0.01/pTree->dbasal)),parameter[0].densityvaluedbasalinfluence);
				
				if (parameter[0].dichtheightrel==1) 
				{
					pTree->densitywert= pTree->densitywert*( -1.0/130.0*pTree->height + 1.0 );
				} 
				else if (parameter[0].dichtheightrel==2) 
				{
					pTree->densitywert= pTree->densitywert*( -1.0/200.0*pTree->height + 1.0 );
				} 
				else if (parameter[0].dichtheightrel==3) 
				{
					double hrelbuf=( -1.0/200.0*pTree->height + 1.0 );
					if(hrelbuf<0.1) 
						hrelbuf=0.1;
					pTree->densitywert= pTree->densitywert*hrelbuf;
				} 
				else if (parameter[0].dichtheightrel==4)
				{
					if (pTree->height<40)
						pTree->densitywert=1.0;
					else if ((pTree->height>=40) & (pTree->height<200))
						pTree->densitywert=0.5;
					else if (pTree->height>=200)
						pTree->densitywert=0.0;
				} 
				else if (parameter[0].dichtheightrel==5) 
				{
					if (pTree->height<40)
						pTree->densitywert=1.0;
					else if ((pTree->height>=40) & (pTree->height<200))
						pTree->densitywert=0.55;
					else if (pTree->height>=200)
						pTree->densitywert=0.1;
				} 
				else if (parameter[0].dichtheightrel==6) 
				{
					if (pTree->height<40)
						pTree->densitywert=1.0;
					else if ((pTree->height>=40) & (pTree->height<200))
						pTree->densitywert=0.9;
					else if (pTree->height>=200)
						pTree->densitywert=0.8;
				}
				else if (parameter[0].dichtheightrel==10) // added to fit the height classes distribution properly
				{
					// density value decreases by increasing tree height
					// ... linearly
					double densitywertminimum=0.1;
					double ageseinfluss=( -1*(1-densitywertminimum)/100.0* (double) pTree->age + 1.0 );
					if(ageseinfluss<densitywertminimum)
					{
						ageseinfluss=densitywertminimum;
					}
					pTree->densitywert= pTree->densitywert*ageseinfluss;
				}
				else if (parameter[0].dichtheightrel==11) // added to fit the height classes distribution properly
				{
					// density value decreases by increasing tree height
					// ... linearly
					double densitywertminimum=0.5;
					double ageseinfluss=( -1*(1-densitywertminimum)/100.0* (double) pTree->age + 1.0 );
					if(ageseinfluss<densitywertminimum)
					{
						ageseinfluss=densitywertminimum;
					}
					pTree->densitywert= pTree->densitywert*ageseinfluss;
				}
				else if (parameter[0].dichtheightrel==12) // added to fit the height classes distribution properly
				{
					// density value decreases by increasing tree height
					// ... linearly
					double densitywertminimum=0.1;
					double ageseinfluss=( -1*(1-densitywertminimum)/50.0* (double) pTree->age + 1.0 );
					if(ageseinfluss<densitywertminimum)
					{
						ageseinfluss=densitywertminimum;
					}
					pTree->densitywert= pTree->densitywert*ageseinfluss;
				}
				else if (parameter[0].dichtheightrel==13) // added to fit the height classes distribution properly
				{
					// density value decreases by increasing tree height
					// ... linearly
					double densitywertminimum=0.5;
					double ageseinfluss=( -1*(1-densitywertminimum)/50.0* (double) pTree->age + 1.0 );
					if(ageseinfluss<densitywertminimum)
					{
						ageseinfluss=densitywertminimum;
					}
					pTree->densitywert= pTree->densitywert*ageseinfluss;
				}
				else if (parameter[0].dichtheightrel==20) // added to fit the height classes distribution properly
				{
					// density value decreases by increasing tree height
					double densitywertminimum=0.1;
					double ageseinfluss=(1.0/pow(( (double) pTree->age +1.0), 0.5));
					if(ageseinfluss<densitywertminimum)
					{
						ageseinfluss=densitywertminimum;
					}
					pTree->densitywert= pTree->densitywert*ageseinfluss;

				}
				else if (parameter[0].dichtheightrel==21) // added to fit the height classes distribution properly
				{
					// density value decreases by increasing tree height
					double densitywertminimum=0.5;
					double ageseinfluss=(1.0/pow(( (double) pTree->age +1.0), 0.15));
					if(ageseinfluss<densitywertminimum)
					{
						ageseinfluss=densitywertminimum;
					}
					pTree->densitywert= pTree->densitywert*ageseinfluss;

				}
				else if (parameter[0].dichtheightrel==22) // added to fit the height classes distribution properly
				{
					// density value decreases by increasing tree height
					double densitywertminimum=0.1;
					double ageseinfluss=(1.0/pow(( (double) pTree->age +1.0), 0.6));
					if(ageseinfluss<densitywertminimum)
					{
						ageseinfluss=densitywertminimum;
					}
					pTree->densitywert= pTree->densitywert*ageseinfluss;

				}
				else if (parameter[0].dichtheightrel==23) // added to fit the height classes distribution properly
				{
					// density value decreases by increasing tree height
					double densitywertminimum=0.5;
					double ageseinfluss=(1.0/pow(( (double) pTree->age +1.0), 0.175));
					if(ageseinfluss<densitywertminimum)
					{
						ageseinfluss=densitywertminimum;
					}
					pTree->densitywert= pTree->densitywert*ageseinfluss;

				}				
				
				if (pTree->densitywert<0) 
					pTree->densitywert=0.0;

				if (pTree->densitywert>parameter[0].desitymaxreduction) 
					pTree->densitywert=parameter[0].desitymaxreduction;
			} 
		}
	}// only one core
	
	if(parameter[0].omp_num_threads>1)
	{// more than one core
		omp_set_dynamic(0); //disable dynamic teams
		omp_set_num_threads(parameter[0].omp_num_threads); //set the number of helpers
			
		#pragma omp parallel private(pTree)
		{
			int thread_count=omp_get_num_threads();
			int thread_num=omp_get_thread_num();
			size_t chunk_size=tree_list.size()/thread_count;
			auto begin=tree_list.begin();
			std::advance(begin,thread_num*chunk_size);
			auto end=begin;
			
			if(thread_num==(thread_count-1)) // last thread takes the remaining elements
			{
				end = tree_list.end();
			} 
			else
			{
				std::advance(end, chunk_size);
			}
			
			// wait for all threads to initialize and then proceed
			#pragma omp barrier
			
			for(auto it = begin; it != end; ++it)
			{
				pTree=(*it);

				int i = (int) floor(pTree->ycoo * parameter[0].sizemagnif);
				int j = (int) floor(pTree->xcoo * parameter[0].sizemagnif);

				//DENSITY 1
				if (parameter[0].densitymode==1)
				{
					pTree->densitywert = 0.0;
				}
				else 
				{
					double flaechengroesze=0.0;
					if (parameter[0].calcinfarea == 1) //linearly increasing
						flaechengroesze = pTree->dbasal * parameter[0].incfac/100.0; 
					else if (parameter[0].calcinfarea == 2) //linearly increasing
						flaechengroesze = pTree->dbasal * (2/3) * parameter[0].incfac/100.0; 
					else if (parameter[0].calcinfarea == 3) //linearly increasing
						flaechengroesze = pTree->dbasal * (4/3) * parameter[0].incfac/100.0; 
					else if (parameter[0].calcinfarea == 4) //logistic growth function with maximum at 8 m
						flaechengroesze= ( 9/( 1+( ( (1/0.1)-1 )*exp(-0.1*pTree->dbasal) ) ) )-1;
					else if (parameter[0].calcinfarea == 5) //logistic growth function with maximum at 8 m
						flaechengroesze= ( 9/( 1+( ( (1/0.1)-1 )*exp(-0.2*pTree->dbasal) ) ) )-1;
					else if (parameter[0].calcinfarea == 6) //logistic growth function with maximum at 8 m
						flaechengroesze= ( 9/( 1+( ( (1/0.1)-1 )*exp(-0.5*pTree->dbasal) ) ) )-1;

					// if the tree only influences one grid cell
					if ( flaechengroesze < (1.0/parameter[0].sizemagnif) )
					{
						// DENSITY 2
						if (parameter[0].densitymode == 2)
						{
							if (plot_list[i*treecols*parameter[0].sizemagnif+j]->Treedensityvalue > 0.0) 
							{
								if (parameter[0].densitytiletree==1)	// sum of values
								{
									pTree->densitywert =	(1.0 - (pTree->densitywert / plot_list[i*treecols*parameter[0].sizemagnif+j]->Treedensityvalue));
									//                           density_desired(at position) / density_currently(at position)
								}
								else if (parameter[0].densitytiletree==2)	// multiplication of values
								{
									pTree->densitywert =	(1.0 - (pTree->densitywert
																/ 
																(plot_list[i*treecols*parameter[0].sizemagnif+j]->Treedensityvalue*pTree->densitywert) 
																)
														);
								//                         density_desired(at position) / density_currently(at position)
								}
							}
							else
							{
								pTree->densitywert=0.0; // no competition effect
							}
						}
						// DENSITY 3
						else if (parameter[0].densitymode == 3)
						{
							// determine the grid position randomly
							int izuf= (int) floor( 0.0 + ( (double)  ( ((double) (treerows-1)) * parameter[0].sizemagnif * rand() / (RAND_MAX + 1.0))) );
							int jzuf= (int) floor( 0.0 + ( (double)  ( ((double) (treecols-1)) * parameter[0].sizemagnif * rand() / (RAND_MAX + 1.0))) );
							
							if (plot_list[izuf*treecols*parameter[0].sizemagnif+jzuf]->Treedensityvalue > 0.0) 
							{
								if (parameter[0].densitytiletree==1)	// sum of values
								{
									pTree->densitywert = (1.0 - (pTree->densitywert/ plot_list[izuf*treecols*parameter[0].sizemagnif+jzuf]->Treedensityvalue));
									//                           density_desired(at position) / density_currently(at position)
								}
								else if (parameter[0].densitytiletree==2)	// multiplication of values
								{
									pTree->densitywert =	(1.0 - (pTree->densitywert
																/ 
																(plot_list[i*treecols*parameter[0].sizemagnif+j]->Treedensityvalue*pTree->densitywert) 
																)
														);
								}
							}
							else 
							{
								pTree->densitywert = 0.0;
							}
						}
						
						// TODO: clean code
						// calculate the influence of the thawing depth on the tree growth
						// if ((plot_list[i*treecols*parameter[0].sizemagnif+j]->maxthawing_depth<2000) && (parameter[0].thawing_depth==true))
						// {
							pTree->thawing_depthinfluence = plot_list[i*treecols*parameter[0].sizemagnif+j]->maxthawing_depth / 1000.0; // scaling to m (1000.0 mm = 1.0 m)
						// }
						// else
						// {
							// pTree->thawing_depthinfluence=100;
						// }
					}
					// ... if the tree influences more than one section
					else
					{
						// determine dimensions of the considered grid around a tree
						int xyquerrastpos= (int) floor( flaechengroesze*parameter[0].sizemagnif );
						double maxdist= sqrt(pow(double(xyquerrastpos),2)+pow(double(xyquerrastpos),2));

						// determine rescaled coordinates and summation of the density value
						double sumdensitywert=0;
						double sumthawing_depth=0;
						
						unsigned int anzahlflaechen=0;
						
						for (int rastposi=(i+xyquerrastpos); rastposi>(i-(xyquerrastpos+1)); rastposi--)
						{
							for (int rastposj=(j-xyquerrastpos); rastposj<(j+xyquerrastpos+1); rastposj++)
							{
								if ((rastposi<=( (treerows-1)*parameter[0].sizemagnif) && rastposi>=0) && ( rastposj<=( (treecols-1)*parameter[0].sizemagnif) && rastposj>=0))
								{
									double entfrastpos=sqrt(pow(double(i-rastposi),2)+pow(double(j-rastposj),2));
									
									if (entfrastpos<= (double) xyquerrastpos)
									{
										int icurr=rastposi;
										int jcurr=rastposj;

										// change of i/j vals only if DENSITY 3
										if (parameter[0].densitymode==3)
										{
											// determine the position of density values randomly
											icurr= (int) floor( 0.0 + ( (double)  ( ((double) (treerows-1)) *parameter[0].sizemagnif*rand()/(RAND_MAX + 1.0))) );
											jcurr= (int) floor( 0.0 + ( (double)  ( ((double) (treecols-1)) *parameter[0].sizemagnif*rand()/(RAND_MAX + 1.0))) );
										}
										
										if (parameter[0].densitytiletree==1)
										{
											sumdensitywert+=plot_list[icurr*treecols*parameter[0].sizemagnif+jcurr]->Treedensityvalue
															* (1-entfrastpos/maxdist);	
															// added the values influence as becoming weaker in the periphery, otherwise the density value influence would be overestimated
										}
										else if (parameter[0].densitytiletree==2)
										{
											// after weighting the additional values by the individual influence values the offset is added
											sumdensitywert+=(plot_list[icurr*treecols*parameter[0].sizemagnif+jcurr]->Treedensityvalue
															-pow(pTree->dbasal, parameter[0].densitytreetile)/(entfrastpos+1.0) )
															* pow(pTree->dbasal, parameter[0].densitytreetile)/(entfrastpos+1.0)															
															+ pow(pTree->dbasal, parameter[0].densitytreetile)/(entfrastpos+1.0);
										}

										sumthawing_depth+=(double) plot_list[rastposi*treecols*parameter[0].sizemagnif+rastposj]->maxthawing_depth;
										anzahlflaechen++;										
									}
								}
							}
						}

						if (sumdensitywert > 0.0) 
							pTree->densitywert = 1.0 - (pTree->densitywert/sumdensitywert);
						else 
							pTree->densitywert=0.0;

						sumthawing_depth=( sumthawing_depth / (double) anzahlflaechen );
						
						// TODO: clean code
						// if (sumthawing_depth < 2000)
							pTree->thawing_depthinfluence = sumthawing_depth / 1000.0; // scaling to m (1000.0 mm = 1.0 m)
						// else
							// pTree->thawing_depthinfluence = 100;
					}

					pTree->densitywert= pTree->densitywert
					*pow((1.0-(0.01/pTree->dbasal)),parameter[0].densityvaluedbasalinfluence);	// Optional: increasing influence by increasing tree height
					
					if (parameter[0].dichtheightrel==1) 
					{
						pTree->densitywert= pTree->densitywert*( -1.0/130.0*pTree->height + 1.0 );
					} 
					else if (parameter[0].dichtheightrel==2) 
					{
						pTree->densitywert= pTree->densitywert*( -1.0/200.0*pTree->height + 1.0 );
					} 
					else if (parameter[0].dichtheightrel==3) 
					{
						double hrelbuf=( -1.0/200.0*pTree->height + 1.0 );
						if(hrelbuf<0.1) 
							hrelbuf=0.1;
						pTree->densitywert= pTree->densitywert*hrelbuf;
					} 
					else if (parameter[0].dichtheightrel==4)
					{
						if (pTree->height<40)
							pTree->densitywert=1.0;
						else if ((pTree->height>=40) & (pTree->height<200))
							pTree->densitywert=0.5;
						else if (pTree->height>=200)
							pTree->densitywert=0.0;
					} 
					else if (parameter[0].dichtheightrel==5) 
					{
						if (pTree->height<40)
							pTree->densitywert=1.0;
						else if ((pTree->height>=40) & (pTree->height<200))
							pTree->densitywert=0.55;
						else if (pTree->height>=200)
							pTree->densitywert=0.1;
					} 
					else if (parameter[0].dichtheightrel==6) 
					{
						if (pTree->height<40)
							pTree->densitywert=1.0;
						else if ((pTree->height>=40) & (pTree->height<200))
							pTree->densitywert=0.9;
						else if (pTree->height>=200)
							pTree->densitywert=0.8;
					}
					else if (parameter[0].dichtheightrel==10) // added to fit the height classes distribution properly
					{
						// density value decreases by increasing tree height
						// ... linearly
						double densitywertminimum=0.1;
						double ageseinfluss=( -1*(1-densitywertminimum)/100.0* (double) pTree->age + 1.0 );
						if(ageseinfluss<densitywertminimum)
						{
							ageseinfluss=densitywertminimum;
						}
						pTree->densitywert= pTree->densitywert*ageseinfluss;
					}
					else if (parameter[0].dichtheightrel==11) // added to fit the height classes distribution properly
					{
						// density value decreases by increasing tree height
						// ... linearly
						double densitywertminimum=0.5;
						double ageseinfluss=( -1*(1-densitywertminimum)/100.0* (double) pTree->age + 1.0 );
						if(ageseinfluss<densitywertminimum)
						{
							ageseinfluss=densitywertminimum;
						}
						pTree->densitywert= pTree->densitywert*ageseinfluss;
					}
					else if (parameter[0].dichtheightrel==12) // added to fit the height classes distribution properly
					{
						// density value decreases by increasing tree height
						// ... linearly
						double densitywertminimum=0.1;
						double ageseinfluss=( -1*(1-densitywertminimum)/50.0* (double) pTree->age + 1.0 );
						if(ageseinfluss<densitywertminimum)
						{
							ageseinfluss=densitywertminimum;
						}
						pTree->densitywert= pTree->densitywert*ageseinfluss;
					}
					else if (parameter[0].dichtheightrel==13) // added to fit the height classes distribution properly
					{
						// density value decreases by increasing tree height
						// ... linearly
						double densitywertminimum=0.5;
						double ageseinfluss=( -1*(1-densitywertminimum)/50.0* (double) pTree->age + 1.0 );
						if(ageseinfluss<densitywertminimum)
						{
							ageseinfluss=densitywertminimum;
						}
						pTree->densitywert= pTree->densitywert*ageseinfluss;
					}
					else if (parameter[0].dichtheightrel==20) // added to fit the height classes distribution properly
					{
						// density value decreases by increasing tree height
						double densitywertminimum=0.1;
						double ageseinfluss=(1.0/pow(( (double) pTree->age +1.0), 0.5));
						if(ageseinfluss<densitywertminimum)
						{
							ageseinfluss=densitywertminimum;
						}
						pTree->densitywert= pTree->densitywert*ageseinfluss;

					}
					else if (parameter[0].dichtheightrel==21) // added to fit the height classes distribution properly
					{
						// density value decreases by increasing tree height
						double densitywertminimum=0.5;
						double ageseinfluss=(1.0/pow(( (double) pTree->age +1.0), 0.15));
						if(ageseinfluss<densitywertminimum)
						{
							ageseinfluss=densitywertminimum;
						}
						pTree->densitywert= pTree->densitywert*ageseinfluss;

					}
					else if (parameter[0].dichtheightrel==22) // added to fit the height classes distribution properly
					{
						// density value decreases by increasing tree height
						double densitywertminimum=0.1;
						double ageseinfluss=(1.0/pow(( (double) pTree->age +1.0), 0.6));
						if(ageseinfluss<densitywertminimum)
						{
							ageseinfluss=densitywertminimum;
						}
						pTree->densitywert= pTree->densitywert*ageseinfluss;

					}
					else if (parameter[0].dichtheightrel==23) 
						// added to fit the height classes distribution properly
					{
						// density value decreases by increasing tree height
						double densitywertminimum=0.5;
						double ageseinfluss=(1.0/pow(( (double) pTree->age +1.0), 0.175));
						if(ageseinfluss<densitywertminimum)
						{
							ageseinfluss=densitywertminimum;
						}
						pTree->densitywert= pTree->densitywert*ageseinfluss;

					}
					
					if (pTree->densitywert<0) 
						pTree->densitywert=0.0;

					// set to maximal value if density value is greater than it (rescaling)
					if (pTree->densitywert>parameter[0].desitymaxreduction) 
						pTree->densitywert=parameter[0].desitymaxreduction;
				}
			}
		}// parallel region
	}// more than one core
}




/****************************************************************************************//**
 * \brief reset Maps "Envirgrid"
 *
 * reset Treedensity und Treenumber \n
 * calculate vegetation density (auflagenstärke) if vegetation==true \n
 * calculate max active layer depth
 *
 *
 *******************************************************************************************/

void ResetMaps(int yearposition, vector<Envirgrid*> &plot_list, vector<Weather*> &weather_list)
{
	if(parameter[0].omp_num_threads==1)
	{// only one core
		for (int kartenpos=0; kartenpos< (treerows*parameter[0].sizemagnif*treecols*parameter[0].sizemagnif); kartenpos++)
		{
			pEnvirgrid=plot_list[kartenpos];
			pEnvirgrid->Treenumber=0;
			// pEnvirgrid->leafarea=0;
			// pEnvirgrid->stemarea=0;
			// pEnvirgrid->maxtreeheight=0;
			
			if (parameter[0].vegetation==true)
			{
				double auflagenwachstumsrate =0.05
											 +( 1.0/( ((1.0/0.01)-(1.0/0.95))
													  *exp(-(1.0/200.0)*(double) pEnvirgrid->maxthawing_depth) 
													  +(1/0.95)) ); 
				
				pEnvirgrid->litterheight+= (unsigned short) (auflagenwachstumsrate*60.0);

				pEnvirgrid->litterheightmean = (unsigned short) ( (double) 
												 (pEnvirgrid->litterheight8
												 +pEnvirgrid->litterheight7
												 +pEnvirgrid->litterheight6
												 +pEnvirgrid->litterheight5
												 +pEnvirgrid->litterheight4
												 +pEnvirgrid->litterheight3
												 +pEnvirgrid->litterheight2
												 +pEnvirgrid->litterheight1
												 +pEnvirgrid->litterheight0
												 +pEnvirgrid->litterheight)
												 /10.0);

				pEnvirgrid->litterheight8 = pEnvirgrid->litterheight7;
				pEnvirgrid->litterheight7 = pEnvirgrid->litterheight6;
				pEnvirgrid->litterheight6 = pEnvirgrid->litterheight5;
				pEnvirgrid->litterheight5 = pEnvirgrid->litterheight4;
				pEnvirgrid->litterheight4 = pEnvirgrid->litterheight3;
				pEnvirgrid->litterheight3 = pEnvirgrid->litterheight2;
				pEnvirgrid->litterheight2 = pEnvirgrid->litterheight1;
				pEnvirgrid->litterheight1 = pEnvirgrid->litterheight0;
				pEnvirgrid->litterheight0 = pEnvirgrid->litterheight;
			}
			
			if (parameter[0].thawing_depth==true && parameter[0].CryoGrid_thawing_depth==false) // in case of CryoGrid_thawing_depth==true estimation will be done externally
			{
				// calculation of an insulation by organic material (damping reduces thawing_depth, formula taken from literature)
				double daempfung = (1.0/4000.0) * (double) pEnvirgrid->litterheightmean; // 1/4000 =slope to reach the maximum value at appr. 4000
				
				if (daempfung>=0.9) 
					daempfung=0.9;
				
/* TODO: clean up, was for testing
// cout << pEnvirgrid->Treedensityvalue << " ... ";
				// pEnvirgrid->maxthawing_depth= (unsigned short) ( 1000.0*(1.0-daempfung)*0.050*sqrt(weather_list[yearposition]->degreday));	// 1000 (scaling from m to mm)*edaphicfactor=0.050 (SD=0.019)
				if(pEnvirgrid->Treedensityvalue>0)
				{
					double treedensityinfluencepermafrost=(1/pEnvirgrid->Treedensityvalue);
					if(treedensityinfluencepermafrost<0.25)
					{
						treedensityinfluencepermafrost=0.25;
					} else if (treedensityinfluencepermafrost>=0.25 & treedensityinfluencepermafrost<0.5)
					{
						treedensityinfluencepermafrost=0.5;
					} else if (treedensityinfluencepermafrost>=0.5 & treedensityinfluencepermafrost<0.75)
					{
						treedensityinfluencepermafrost=0.75;
					} else
					{
						treedensityinfluencepermafrost=1;
					}
// cout << treedensityinfluencepermafrost << endl;
						
						
						
					pEnvirgrid->maxthawing_depth= (unsigned short) ( 1000.0*(1.0-daempfung)*0.050*sqrt(weather_list[yearposition]->degreday) ) * treedensityinfluencepermafrost;//test here different values 1/ must maybe maxvalue of treedensityvalues
				} else
				{*/
					pEnvirgrid->maxthawing_depth= (unsigned short) ( 1000.0*(1.0-daempfung)*0.050*sqrt(weather_list[yearposition]->degreday) );
				// }
			}
		
			pEnvirgrid->Treedensityvalue=0;//moved here from above so that the Treedensityvalue can be used for evalutaion of active layer

		}
	}// only one core

	if(parameter[0].omp_num_threads>1)
	{// more than one core
		omp_set_dynamic(1);
		omp_set_num_threads(parameter[0].omp_num_threads); //set the number of helpers
		
		#pragma omp parallel for private(pEnvirgrid)
		for(int kartenpos=0; kartenpos< (treerows*parameter[0].sizemagnif*treecols*parameter[0].sizemagnif); kartenpos++)
		{
			pEnvirgrid=plot_list[kartenpos];
			pEnvirgrid->Treenumber=0;
			// pEnvirgrid->leafarea=0;
			// pEnvirgrid->stemarea=0;
			// pEnvirgrid->maxtreeheight=0;
			
			if (parameter[0].vegetation==true && parameter[0].spinupphase==false)
			{
				double auflagenwachstumsrate =0.05
											 +( 1.0/( ((1.0/0.01)-(1.0/0.95))
													  *exp(-(1.0/200.0)*(double) pEnvirgrid->maxthawing_depth) 
													  +(1/0.95)) ); 
													  // logistic growth: capacity=0.95; N0=0.01; r=1/200; offset= 0.05
				
				pEnvirgrid->litterheight+= (unsigned short) (auflagenwachstumsrate*60.0);	
				// in 0.1 mm steps; 6mm growth annualy from 30 cm growth in 50 years (literature value)

				pEnvirgrid->litterheightmean = (unsigned short) ( (double) 
												 (pEnvirgrid->litterheight8
												 +pEnvirgrid->litterheight7
												 +pEnvirgrid->litterheight6
												 +pEnvirgrid->litterheight5
												 +pEnvirgrid->litterheight4
												 +pEnvirgrid->litterheight3
												 +pEnvirgrid->litterheight2
												 +pEnvirgrid->litterheight1
												 +pEnvirgrid->litterheight0
												 +pEnvirgrid->litterheight)
												 /10.0); 

				pEnvirgrid->litterheight8 = pEnvirgrid->litterheight7;
				pEnvirgrid->litterheight7 = pEnvirgrid->litterheight6;
				pEnvirgrid->litterheight6 = pEnvirgrid->litterheight5;
				pEnvirgrid->litterheight5 = pEnvirgrid->litterheight4;
				pEnvirgrid->litterheight4 = pEnvirgrid->litterheight3;
				pEnvirgrid->litterheight3 = pEnvirgrid->litterheight2;
				pEnvirgrid->litterheight2 = pEnvirgrid->litterheight1;
				pEnvirgrid->litterheight1 = pEnvirgrid->litterheight0;
				pEnvirgrid->litterheight0 = pEnvirgrid->litterheight;
			}			

			if (parameter[0].thawing_depth==true && parameter[0].CryoGrid_thawing_depth==false) // in case of CryoGrid_thawing_depth==true estimation will be done externally
			{
				// calculation of an insulation by organic material (damping reduces thawing_depth, formula taken from literature)
				double daempfung = (1.0/4000.0) * (double) pEnvirgrid->litterheightmean; // 1/4000 =slope to reach the maximum value at appr. 4000
				
				if (daempfung>=0.9) 
					daempfung=0.9;

/* TODO: clean was for testing
// cout << pEnvirgrid->Treedensityvalue << " ... ";
				// pEnvirgrid->maxthawing_depth= (unsigned short) ( 1000.0*(1.0-daempfung)*0.050*sqrt(weather_list[yearposition]->degreday));	// 1000 (scaling from m to mm)*edaphicfactor=0.050 (SD=0.019)
				if(pEnvirgrid->Treedensityvalue>0)
				{
					double treedensityinfluencepermafrost=(1/pEnvirgrid->Treedensityvalue);
					if(treedensityinfluencepermafrost<0.25)
					{
						treedensityinfluencepermafrost=0.25;
					} else if (treedensityinfluencepermafrost>=0.25 & treedensityinfluencepermafrost<0.5)
					{
						treedensityinfluencepermafrost=0.5;
					} else if (treedensityinfluencepermafrost>=0.5 & treedensityinfluencepermafrost<0.75)
					{
						treedensityinfluencepermafrost=0.75;
					} else
					{
						treedensityinfluencepermafrost=1;
					}
// cout << treedensityinfluencepermafrost << endl;
						
					pEnvirgrid->maxthawing_depth= (unsigned short) ( 1000.0*(1.0-daempfung)*0.050*sqrt(weather_list[yearposition]->degreday) ) * treedensityinfluencepermafrost;//test here different values 1/ must maybe maxvalue of treedensityvalues
				} else
				{*/
					pEnvirgrid->maxthawing_depth= (unsigned short) ( 1000.0*(1.0-daempfung)*0.050*sqrt(weather_list[yearposition]->degreday) );
				// }
			}
			
			pEnvirgrid->Treedensityvalue=0;

		}
	}// more than one core
}






/****************************************************************************************//**
 * \brief update density maps and active layer depth
 *
 * 
 *
 *
 *******************************************************************************************/
 
void Environmentupdate(struct Parameter *parameter, int yearposition, vector<vector<Envirgrid*> > &world_plot_list, vector<list<Tree*> > &world_tree_list, vector<vector<Weather*> > &world_weather_list)
{
	int aktort=0;

	for(vector<vector<Envirgrid*> >::iterator posw = world_plot_list.begin(); posw != world_plot_list.end(); ++posw)
	{
		vector<Envirgrid*>& plot_list = *posw;

		vector<list<Tree*> >::iterator world_positon_b = (world_tree_list.begin()+aktort);
		list<Tree*>& tree_list = *world_positon_b;

		vector<vector<Weather*> >::iterator posiwelt = (world_weather_list.begin()+aktort);
		vector<Weather*>& weather_list = *posiwelt;

		vector<vector<Cryogrid*> >::iterator worldposcryogrid = (world_cryo_list.begin()+aktort);
		vector<Cryogrid*>& cryo_list = *worldposcryogrid;

		aktort++;

		double time_start_0=omp_get_wtime();
		ResetMaps(yearposition, plot_list, weather_list);
		double time_ResetMaps=omp_get_wtime();
		
		// TODO: call only in certain years
		// external update of active layer thickness values
		if( (parameter[0].callcryogrid==false & parameter[0].CryoGrid_thawing_depth==true) | parameter[0].ivort%25==0)
			PrepareCryogrid(tree_list, cryo_list);		// collect information of trees
		
		if(parameter[0].callcryogrid==true & parameter[0].CryoGrid_thawing_depth==true & (( parameter[0].spinupphase==true & parameter[0].ivort == 500 ) | ( parameter[0].spinupphase==false &  parameter[0].ivort%25 == 0)))
		// if(parameter[0].CryoGrid_thawing_depth==true &  parameter[0].ivort%25 == 0)// TODO: currently too slow so that only at certain times it can be called
		{
			UpdateCryogrid(tree_list, cryo_list);		// export data and call Cryogrid instance and collect back output
			parameter[0].cryogridcalled=true;
		}
		
		if(parameter[0].callcryogrid==false & parameter[0].CryoGrid_thawing_depth==true)
			UpdateEnvirgridALD(cryo_list, plot_list, weather_list, yearposition);
		else if(parameter[0].cryogridcalled==true & parameter[0].ivort%25 == 0)// interpolate for Envirgrid-tiles from Cryogrid active layer depth, use former estimation values after once called CryoGrid
			UpdateEnvirgridALD(cryo_list, plot_list, weather_list, yearposition);
			
		
		
		AddTreeDensity(tree_list, plot_list);
		double time_AddTreeDensity=omp_get_wtime();

		IndividualTreeDensity(tree_list, plot_list);
		
		if(parameter[0].computationtimevis==1)
		{
			openupdate:
			FILE *fp5;
			fp5=fopen("t_N_plotupdate.txt","a+");
				if(fp5==0)
				{
				goto openupdate;
				}
				fprintf(fp5,"%d;%lu;%10.20f;%10.20f;%10.20f\n",
					parameter[0].ivort, 
					tree_list.size(),
					time_ResetMaps-time_start_0,
					time_AddTreeDensity-time_ResetMaps, 
					omp_get_wtime()-time_AddTreeDensity
				);
			fclose(fp5);
		}

	}

}