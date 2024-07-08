

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


// compute the earth gravity acc
#ifdef EMBED
#define MAX_DegreeN 30 /**< maximum degree of earth gravity */
#define MAX_OrderM 30  /**< maximum order of earth gravity */
#else
#define MAX_DegreeN 120 /**< maximum degree of earth gravity */
#define MAX_OrderM 120  /**< maximum order of earth gravity */
#endif

#define MAX_size_coef ((1 + (MAX_DegreeN + 1)) * (MAX_DegreeN + 1) / 2) /**< maximum number of coefficients of earth gravity*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mathlib.h"
#include "earth_rotation_param.h"


#ifndef EMBED
    bool load_earth_coef_EGM(char *filename);
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
    void correct_CS_TimeVarying(double mjd_utc, double *c_C20, double *c_C30, double *c_C40, double *c_C21, double *c_S21);
    void correct_CS_solidEarthTide(double JD_UT1_I, double JD_UT1_F, double JD_TT, double *sunpos_ecef, double *moonpos_ecef, double *dC, double *dS);
    void correct_PoleTide(double mjd_utc, double xp, double yp, double *dC21, double *dS21);

#ifdef __cplusplus
}
#endif

#endif