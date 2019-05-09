#pragma once
#include "CoreMinimal.h"

//#include "AllowWindowsPlatformTypes.h"
//#include <stdio.h>
//#include <math.h>
//#include <vector>
//#include "HideWindowsPlatformTypes.h"

using namespace std;

#define TOL 1e-30 /* smallest value allowed in cholesky_decomp() */

struct LMstat {
	int verbose;
	int max_it;
	double init_lambda;
	double up_factor;
	double down_factor;
	double target_derr;
	int final_it;
	double final_err;
	double final_derr;
};

class SYNOPTICON_API Levmar
{
public:
	Levmar();
	~Levmar();

	static void levmarq_init(LMstat *lmstat);

	static int levmarq(int npar, double *par, int ny, double *y, double *dysq,
		double(*func)(double *, int, void *),
		void(*grad)(double *, double *, int, void *),
		void *fdata, LMstat *lmstat);

	static double error_func(double *par, int ny, double *y, double *dysq,
		double(*func)(double *, int, void *), void *fdata);

	static void solve_axb_cholesky(int n, double** l, double* x, double* b);

	static int cholesky_decomp(int n, double** l, double** a);

	static void initArray(int row, int col, double** arr);
	static void deleteArray(int row, int col, double** arr);
};
