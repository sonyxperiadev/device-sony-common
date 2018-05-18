/*
 *   Polynomial Regression Data Fit
 *   Algorithm Copyright(C) 2012, Paul Lutus
 *
 *   C implementation, updates and additions
 *   Copyright(C) 2018, AngeloGioacchino Del Regno <kholk11@gmail.com>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <sys/types.h>
#include "polyreg.h"


#include <private/android_filesystem_config.h>
#include <utils/Log.h>

#define LOG_TAG		"PolyReg"

void gj_divide(double **A, int i, int j, int m) {
	for (int q = j + 1; q < m; q++) {
		A[i][q] /= A[i][j];
	}
	A[i][j] = 1;
}

void gj_eliminate(double **A, int i, int j, int n, int m) {
	for (int k = 0; k < n; k++) {
		if (k != i && A[k][j] != 0) {
			for (int q = j + 1; q < m; q++) {
				A[k][q] -= A[k][j] * A[i][q];
			}
			A[k][j] = 0;
		}
	}
}

static void gj_echelonize(double **A, int n, int m) {
	int i = 0;
	int j = 0;
	int k;
	double *swap;
	while (i < n && j < m) {
		k = i;
		while (k < n && A[k][j] == 0) {
			k++;
		}
		if (k < n) {
			if (k != i) {
				swap = A[i];
				A[i] = A[k];
				A[k] = swap;
			}
			if (A[i][j] != 1) {
				gj_divide(A, i, j, m);
			}
			// eliminate all other non-zero entries
			gj_eliminate(A, i, j, n, m);
			i++;
		}
		j++;
	}
}

// produce a single y result for a given x
static double regress(double x, int npairs, double *terms) {
	double a = 0;
	int exp = 0;
	int i;
	for (i = 0; i < npairs; i++) {
		a += terms[i] * pow(x, exp);
		exp++;
	}
	return a;
}

// correlation coefficient
double corr_coeff(struct pair_data *data, int npairs, double *terms) {
	double r = 0;
	int n = npairs;
	int i;
	double sx = 0, sx2 = 0, sy = 0, sy2 = 0, sxy = 0;
	double x, y, div;

	for (i = 0; i < n; i++) {
		x = regress(data[i].x, npairs, terms);
		y = data[i].y;
		sx += x;
		sy += y;
		sxy += x * y;
		sx2 += x * x;
		sy2 += y * y;
	}
	div = sqrt((sx2 - (sx * sx) / n) * (sy2 - (sy * sy) / n));
	if (div != 0) {
		r = pow((sxy - (sx * sy) / n) / div, 2);
	}
	return r;
}

// standard error
double std_error(struct pair_data *data, int npairs, double *terms) {
	double r = 0;
	int n = npairs;
	int i;
	if (n > 2) {
		double a = 0;
		for (i = 0; i < n; i++) {
			a += pow((regress(data[i].x, npairs, terms) - data[i].y), 2);
		}
		r = sqrt(a / (n - 2));
	}
	return r;
}

// create regression coefficients
// for provided data set
int compute_coefficients(struct pair_data *data,
			 int npairs, int pin, double *terms) {
	int p = pin + 1;
	int r, c;
	int rs = (2 * p) - 1;
	int i = 0, j = 0, count = 0;
	double **m;
	double *mpc;

	m = (double**)calloc(p+2, sizeof(double*));
	memset(m, 0, p+1);
	for (i = 0; i <= p+1; i++) {
		m[i] = (double*)calloc(p+1, sizeof(double));
		memset(m[i], 0, p+1);
	}
	mpc  = (double*)calloc(rs+1, sizeof(double));

	mpc[0] = npairs;

	for (i = 0; i <= npairs; i++) {
		for (r = 1; r < rs; r++) {
			mpc[r] += pow(data[i].x, r);
		}
		m[0][p] += data[i].y;
		for (r = 1; r < p; r++) {
			m[r][p] += pow(data[i].x, r) * data[i].y;
		}
	}

	for (r = 0; r < p; r++) {
		for (c = 0; c < p; c++) {
			m[r][c] = mpc[r + c];
		}
	}

	gj_echelonize(m, p, p+1);

	for (i = 0; i <= p; i++) {
		terms[count] = m[i][p];
		count++;
	}

	free(m);
	free(mpc);

	return count;
}

double polyreg_f(double x, double *terms, int degree) {
	int i = 0;
	double ret = 0.0f;

	for (i = 0; i <= degree; i++)
		ret += terms[i] * pow(x, i);

	return ret;
}
