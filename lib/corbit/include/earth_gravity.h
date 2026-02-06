

/**
 * @file earth_gravity.h
 * @brief Declarations of earth gravity computation
 * @details
 * @author Dr. Zhen Li
 * @version 0.0.1
 * @date 2023.12.25
 * @note this file includes all the functions for earth gravity computation
 * @since 2016.01.15
 * @copyright
 * @par History:
 *
 */

#ifndef _EARTH_GRAVITY_H_
#define _EARTH_GRAVITY_H_



#ifdef __cplusplus
extern "C"
{
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mathlib.h"
#include "earth_rotation_param.h"

// compute the earth gravity acc
#ifdef EMBED
#define MAX_DegreeN 70 /**< maximum degree of earth gravity */
#define MAX_OrderM 70  /**< maximum order of earth gravity */
#else
#define MAX_DegreeN 120 /**< maximum degree of earth gravity */
#define MAX_OrderM 120  /**< maximum order of earth gravity */
#endif

#define MAX_size_coef ((1 + (MAX_DegreeN + 1)) * (MAX_DegreeN + 1) / 2) /**< maximum number of coefficients of earth gravity*/


/*
there are the raw denormalized coefficients for the polar tide, oecan tide and solid earth tide corrections
Ntotal = (1 + N)*N/2 + (M+1) - 3, M<=N, M starts from 0, N starts from 2
N=10, M = 10,  Ntotal = 63
idx(n,m) = (1 + n)*n/2 + (m+1) - 3 - 1,  idx(2,0)=0
*/
#ifdef EMBED
#define MAX_DegreeN_correction 10
#define MAX_OrderM_correction  10
#else
#define MAX_DegreeN_correction 50
#define MAX_OrderM_correction  50
#endif

#define MAX_size_coef_correction (  (1 + MAX_DegreeN_correction)*MAX_DegreeN_correction/2 + (MAX_OrderM_correction+1) - 3 )


/*doodson number is a special efficient coding approach, ref: https://articles.adsabs.harvard.edu/pdf/1971CeMec...4..190M
* every tidal wave receives an argument of the form: a*tau + b*s + c*h + d*p + e*N' + f*ps
* a = 1 for diural tesseral waves, -4< b,c,d,e,f<4;
* to avoid negative numbers, a, b+5, c+5, d+5, e+5, f+5 ; X for 10, E for 11
* doodson's number is a(b+5)(c+5).(d+5)(e+5)(f+5)
* for example: tau-2s+2h-p  becomes 137.455
*/
struct _doodson_
{
    char darwin_symbol[5];
    int argument_doodson_num[6]; // (tau, s, h, p, N', ps)
    int sn_idx [2]; 
    // the start and end index in the array of struct _ocean_tide_CS_, need to be updated manually when add or remove _ocean_tide_CS_
};

struct _ocean_tide_CS_
{
    unsigned int nm[2]; // degree n and order m
    double CS[4]; // DelC+, DelS+, DelC-, DelS-
};

#ifndef EMBED
    bool load_earth_coef_EGM(char *filename);
    bool load_ocean_tide_fes2014b(char *filename, struct _doodson_* doodson_info, struct _ocean_tide_CS_* ocean_tide_CS, int* num_constituent, int* num_CS_array);
#endif
    
    void initialize_global_earth_gravity();
    
    void compute_dadr_earth_gravity(int NN, int MM, double *pos_ecef, double *dadr_ecf);
    void compute_acc_earth_gravity(int NN, int MM, double *pos_ecef, double *a_ecef);
    void compute_acc_dadr_earth_gravity(int NN, int MM, double mjd_utc, double leap_sec, double *satpos_ecef, double *sunpos_ecf, double *moonpos_ecf, double *acc_ecf, double *dadr_ecf);
    double normalFactor(int n, int m);
    double legendrePoly(int n, int m, double u);
    void denormaliseCS(int N, double *Cn, double *Sn);
    void computeNormalFactor();
    void generateVW(int NN, int MM, double *pos_ecef);

    //void correct_normal_CS();
    void correct_CS_OceanTide_embed(double JD_UT1_I, double JD_UT1_F, double JD_TT,double* dC, double* dS);
    void correct_CS_OceanTide(double JD_UT1_I, double JD_UT1_F, double JD_TT,double* dC, double* dS);
    void correct_CS_conventional_model_EGM2008(double mjd_utc, double *c_C20, double *c_C30, double *c_C40, double *c_C21, double *c_S21);
    void correct_CS_solidEarthTide(double JD_UT1_I, double JD_UT1_F, double JD_TT, double *sunpos_ecef, double *moonpos_ecef, double *dCn, double *dSn);
    void correct_PoleTide(double mjd_utc, double xp, double yp, double *dC21, double *dS21);
    

    //external use of these variables
    extern  struct _doodson_* g_ptr_doodson_info;
    extern  struct _ocean_tide_CS_* g_ptr_ocean_tide_CS;
    extern int g_num_doodson_info;
    extern int g_num_ocean_tide_CS;

#ifdef __cplusplus
}
#endif

#endif