using namespace std;


double NormverteilungRN(float mean, float sdev)
{
// Normalverteilte Zahl ziehen und 
	//The polar form of the Box-Muller transformation is both faster and more robust numerically. The algorithmic description of it is:
	float x1, x2, w, y1;//, y2;
	do
	{
		x1 = 2.0 * (0.0 +(float) (1.0*rand()/(RAND_MAX + 1.0) ) ) - 1.0;
		x2 = 2.0 * (0.0 +(float) (1.0*rand()/(RAND_MAX + 1.0) ) ) - 1.0;
		w = x1 * x1 + x2 * x2;
	} while ( w >= 1.0 );

	w = sqrtf( (-2.0 * logf( w ) ) / w );
	y1 = x1 * w;
// 	y2 = x2 * w;
	float rn;
	rn= mean + y1 * sdev; // mean=m, sdev=s
	//printf("rn=%4.4f ", rn);
	return rn;
}
