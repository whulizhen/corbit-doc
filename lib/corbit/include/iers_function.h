
/**
 * @file iers_function.h
 * @brief Declarations for IERS functions
 * @details These functions are mainly used as assist in convertion from ICRF to ITRF
 * @author Dr. Zhen Li
 * @version 0.0.1
 * @date 2023.12.25
 * @note this file includes all the functions for IERS functions
 * @since 2016.01.15
 * @copyright
 * @par History:
 *
 */

#ifndef _IERS_FUNCTION_H_
#define _IERS_FUNCTION_H_

#ifndef EMBED
#include <stdio.h>
#include <stdlib.h>
#endif

#include <math.h>
#include <stdbool.h>
#include "corbit_const.h"

#ifdef __cplusplus
extern "C"
{
#endif

    void FUNDARG(double T, double *L, double *LP, double *F, double *D, double *OM);
    void DOODSONARG(double JD_UT1_I, double JD_UT1_F, double JD_TT, double FNUT[5], double BETA[6]);

    void CNMTX(double MJD_TT, double *H);
    void PMSDNUT2(double RMJD, double *pm);
    void UTLIBR(double RMJD, double *dut1, double *dlod);
    void ORTHO_EOP(double MJD_TT, double *EOP_correction);

    double getGMST(double JD_UT1_I, double JD_UT1_F, double JD_TT);
    double getGAST(double JD_UT1_I, double JD_UT1_F, double JD_TT, double dpsi);
    
    double getEarthRotationAngle(double JD_UT1_I, double JD_UT1_F);
    double getEarthRotationAngleRate(double JC_TT);
    double getMeanObliquity(double JC_TT);
    double getEECT(double JC_TT);
    double getEE2000(double JC_TT, double dpsi);

    // for solid earth tide displancement corrections ----validated
    int CAL2JD(int IY, int IM, int ID, double *DJM0, double *DJM);
    
    void ARG2(int IYEAR, double DAY, double *ANGLE);
    int MDAY(int iy, int im);
    double etutc(const double year);
    int juldat(const int *it);
    bool LEAP(int iy);
    void TOYMD(int IT1[2], int IT2[3]);
    double normalizeAngle(double a);

#ifndef EMBED

void RG_ZONT2(double JC_TT, double *DUT, double *DLOD, double *DOMEGA);

int admint(const double *ampin, const int idtin[][6], const double *phin, double *amp, double *f, double *p, const int nin, int *nout, const int *itm);

void ST1IDIU(double *XSTA, double *XSUN, double *XMON, double FAC2SUN, double FAC2MON, double *XCORSTA);
void ST1L1(double *XSTA, double *XSUN, double *XMON, double FAC2SUN, double FAC2MON, double *XCORSTA);
void ST1ISEM(double *XSTA, double *XSUN, double *XMON, double FAC2SUN, double FAC2MON, double *XCORSTA);
void STEP2DIU(double *XSTA, double FHR, double T, double *XCORSTA);
void STEP2LON(double *XSTA, double T, double *XCORSTA);
    
// HARDISP function is not good as shells function need to be changed to compile with C language
void HARDISP(double olc[6][11], double iyear, double idoy, double ihour, double iminute, double isec, double *dxtide);

void DEHANTTIDEINEL(double *XSTA, int YR, int MONTH, int DAY, double FHR, double leapsec, double *XSUN, double *XMON, double *DXTIDE);

// function in chapter 9 FCULZD_HPA
void FCULZD_HPA(double latitude, double ellip_ht, double pressure, double wvp, double lambda_um, double *fcul_ztd, double *fcul_zhd, double *fcul_zwd);

double gstime_iau82(double jdut1);

int shells(double *x, int *k, const int n);
int recurs(double *x, const int n, const double *hc, const int nf, const double *om, double *scr);
 
int spline(const int nn, const double *x, const double *u, double *s, double *a);
int eval(const double y, const int nn, const double *x, const double *u, const double *s, double *val);
int tdfrph(const int *idood, const int *itm, double *freq, double *phase);

double FCUL_A(double LATITUDE, double HEIGHT_M, double T_K, double ELEV_DEG);
    
#endif


#ifdef __cplusplus
}
#endif

#endif