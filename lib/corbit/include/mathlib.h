
/**
 * @file mathlib.h
 * @brief Declarations of mathematic operations
 * @details
 * @author Dr. Zhen Li
 * @version 0.0.1
 * @date 2023.12.25
 * @note this file includes all the functions for math and matrix operations.
 * @since 2016.01.15
 * @copyright
 * @par History:
 *
 */

#ifndef _MATHLIB_H_
#define _MATHLIB_H_

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef EMBED
#include <stdlib.h>
#else
// only for the EMBED version matrix inversion
#define SOLVE_MAXNUM_COLS 40
#define SOLVE_MAXNUM_ROWS 80
#define MAXNUM_NPOINTS_INTERPOLATION 30
#endif

#include <stdio.h>
#include <math.h>
#include <string.h>
#include "corbit_const.h"

    /**
     * @brief Quaternion struct
     * @note r is the scaler q0, v is the vector q1, q2, and q3
     */
    typedef struct _quaternion_
    {
        double r;    /**< the anglualr element */
        double v[3]; /**< the vector element */
    } QUATERNION;

    void quaternion2rotation(QUATERNION q, double *R);
    void quaternion_from_rotation(double rotationMatrix[9], QUATERNION *q);
    void quaternion_normalise(QUATERNION *q);

    double interpolation_Lagrange(double *x, double *y, int n, double xt);
    bool interpolation_Lagrange_Barycentric(double *x, double *y, const int n, double xt, double *yt);
    void interpolation_multip_dimension(const int n, const int m, double *x, double *y, double xt, double *yt, double *yt_derivative);

    // void set_rand_seed(long seed);
    double Round(double a);
    int nearest_integer(double x);
    void Rx(double phi, double *R);
    void Ry(double phi, double *R);
    void Rz(double phi, double *R);

    void matrixMultiply(int rA, int cA, double *A, int cB, double *B, double *C);
    void matrixMultiply3(int r1, int r2, int r3, int c3, double *M1, double *M2, double *M3, double *Res, int c1_max, int c2_max, int c3_max);
    void matrixMultiply3_HPHT(int r1, int r2, int c2, double *M1, double *M2, double *Res, int c1_max, int c2_max, int c_res_max);

    void matrixTranspose(int r, int c, double *m, double *t);
    void matrix_identical(int r, double *m);
    void ArrayAddition(int m, double *A1, double *A2, double *A3);

    void crossproduct(int m, int n, const double A[], const double B[], double C[]);
    double dotproduct(int m, int n, const double A[], const double B[]);
    double norm(int n, double *x);
    double norm2(int n, double *x);
    void normalize_vector(int n, double *x);
    void negative_vector(int n, double *x);
    unsigned int quadraticSolver(double *ce, double *roots);

    // linear fit y = a*x +b, a and b are param[0], param[1]
    void linear_fit(double *x, double *y, int np, double param[2]);

    // matrix inversion
    int matrix_zero_check(double *A, int n, int N);
    int ludcmp(double *A, int nn, const int n, int *indx, double *d);
    void lubksb(const double *A, int nn, int n, const int *indx, double *b);
    int matinv(double *A, int nn, const int n);
    int solve_lsq(double *A, double *y, const int n, const int m, double *x, double *Q);
    int solve_lsq_normal(double *BTWB, double *BTWL, int n, double *x, double *Q);
    double get_mean(double *x, const int n, double threshold);
    double get_std(double *x, const int n, double threshold);
    double get_rms(double *x, const int n, double threshold);
    double get_min(double *x, const int n, double threshold);
    double get_max(double *x, const int n, double threshold);

    int filter_(double *x, double *P, double *H, double *v, double *R, int n, int m, double *xp, double *Pp);
    int kalman_filter(double *x, double *P, double *H, double *v, double *R, int n, int m);

    int kalman_filter_embed(double *x, double *P, double *H, double *v, double *R, int n, int m);

#ifndef EMBED

    void HelmertTransform_3D(double *p, double *param, double *q);
    void HelmertParameter_3D(int npoints, double (*q)[3], double (*p)[3], double *param);

#endif

#ifdef __cplusplus
}
#endif

#endif // header guard
