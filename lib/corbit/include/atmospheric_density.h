

#ifndef _ATMOSPHERIC_DENSITY_H_
#define _ATMOSPHERIC_DENSITY_H_

#ifdef __cplusplus
extern "C"
{
#endif


#include <stdlib.h>
#include <math.h>
#include "mathlib.h"

#ifdef EMBED
#define MAXNUM_KPAP_RECORD 32
#else
#define MAXNUM_KPAP_RECORD 10000
#endif

typedef struct _ap_kp_f107_
{
	double mjd; /// the mjd time in UT
	double f107_daily;
	double Kp_daily;
    double Kp_3_hour[8];
    double Ap_daily;
	double Ap_3_hour[8];
} solar_kp_ap_f107;

typedef struct _storage_ap_kp_f107_
{
    solar_kp_ap_f107* records; // array of records
    int num_total;
}solar_kp_ap_f107_storage;

/**
 * @brief particle mass in kg
 * 
 */
// const double c_mass_particle[] = 
// {
//     6.65327e-27,  // He
//     2.65676e-26,  // O
//     4.65173e-26, // N2
//     5.31451e-26, // O2
//     6.63394e-26, // Ar
//     1.67372e-27, // H
//     2.32586e-26,  // N
//     2.65676e-26, // AO,  Anomalous oxygen is hot atomic and singly ionized oxygen believed to be present in Earth's exosphere above 500 km near the poles during their respective summers
// };


/* ------------------------------------------------------------------- */
/* ------------------------------- INPUT ----------------------------- */
/* ------------------------------------------------------------------- */

typedef struct _nrlmsise_flags_ 
{
	int switches[24];
	double sw[24];
	double swc[24];
} nrlmsise_flags;
/*   
 *   Switches: to turn on and off particular variations use these switches.
 *   0 is off, 1 is on, and 2 is main effects off but cross terms on.
 *
 *   Standard values are 0 for switch 0 and 1 for switches 1 to 23. The 
 *   array "switches" needs to be set accordingly by the calling program. 
 *   The arrays sw and swc are set internally.
 *
 *   switches[i]:
 *    i - explanation
 *   -----------------
 *    0 - output in meters and kilograms instead of centimeters and grams
 *    1 - F10.7 effect on mean
 *    2 - time independent
 *    3 - symmetrical annual
 *    4 - symmetrical semiannual
 *    5 - asymmetrical annual
 *    6 - asymmetrical semiannual
 *    7 - diurnal
 *    8 - semidiurnal
 *    9 - daily ap [when this is set to -1 (!) the pointer
 *                  ap_a in struct nrlmsise_input must
 *                  point to a struct ap_array]
 *   10 - all UT/long effects
 *   11 - longitudinal
 *   12 - UT and mixed UT/long
 *   13 - mixed AP/UT/LONG
 *   14 - terdiurnal
 *   15 - departures from diffusive equilibrium
 *   16 - all TINF var
 *   17 - all TLB var
 *   18 - all TN1 var
 *   19 - all S var
 *   20 - all TN2 var
 *   21 - all NLB var
 *   22 - all TN3 var
 *   23 - turbo scale height var
 */

// struct ap_array {
// 	double a[7];   
// };
/* Array containing the following magnetic values:
 *   0 : daily AP
 *   1 : 3 hr AP index for current time
 *   2 : 3 hr AP index for 3 hrs before current time
 *   3 : 3 hr AP index for 6 hrs before current time
 *   4 : 3 hr AP index for 9 hrs before current time
 *   5 : Average of eight 3 hr AP indicies from 12 to 33 hrs 
 *           prior to current time
 *   6 : Average of eight 3 hr AP indicies from 36 to 57 hrs 
 *           prior to current time 
 */


typedef struct _nrlmsise_input_ {
	int year;      /* year, currently ignored */
	int doy;       /* day of year */
	double sec;    /* seconds in day (UT) */
	double alt;    /* altitude in kilometers */
	double g_lat;  /* geodetic latitude */
	double g_long; /* geodetic longitude */
	double lst;    /* local apparent solar time (hours), see note below */
	double f107A;  /* 81 day average of F10.7 flux (centered on doy) */
	double f107;   /* daily F10.7 flux for previous day */
	double ap;     /* magnetic index(daily) */
	// struct ap_array *ap_a; /* see above */
    double ap_a[7];
}nrlmsise_input;
/*
 *   NOTES ON INPUT VARIABLES: 
 *      UT, Local Time, and Longitude are used independently in the
 *      model and are not of equal importance for every situation.  
 *      For the most physically realistic calculation these three
 *      variables should be consistent (lst=sec/3600 + g_long/15).
 *      The Equation of Time departures from the above formula
 *      for apparent local time can be included if available but
 *      are of minor importance.
 *
 *      f107 and f107A values used to generate the model correspond
 *      to the 10.7 cm radio flux at the actual distance of the Earth
 *      from the Sun rather than the radio flux at 1 AU. The following
 *      site provides both classes of values:
 *      ftp://ftp.ngdc.noaa.gov/STP/SOLAR_DATA/SOLAR_RADIO/FLUX/
 *
 *      f107, f107A, and ap effects are neither large nor well
 *      established below 80 km and these parameters should be set to
 *      150., 150., and 4. respectively.
 */



/* ------------------------------------------------------------------- */
/* ------------------------------ OUTPUT ----------------------------- */
/* ------------------------------------------------------------------- */

typedef struct _nrlmsise_output_ {
	double d[9];   /* densities */
	double t[2];   /* temperatures */
}nrlmsise_output;
/* 
 *   OUTPUT VARIABLES:
 *      d[0] - HE NUMBER DENSITY(CM-3)
 *      d[1] - O NUMBER DENSITY(CM-3)
 *      d[2] - N2 NUMBER DENSITY(CM-3)
 *      d[3] - O2 NUMBER DENSITY(CM-3)
 *      d[4] - AR NUMBER DENSITY(CM-3)                       
 *      d[5] - TOTAL MASS DENSITY(GM/CM3) [includes d[8] in td7d]
 *      d[6] - H NUMBER DENSITY(CM-3)
 *      d[7] - N NUMBER DENSITY(CM-3)
 *      d[8] - Anomalous oxygen NUMBER DENSITY(CM-3)
 *      t[0] - EXOSPHERIC TEMPERATURE
 *      t[1] - TEMPERATURE AT ALT
 * 
 *
 *      O, H, and N are set to zero below 72.5 km
 *
 *      t[0], Exospheric temperature, is set to global average for
 *      altitudes below 120 km. The 120 km gradient is left at global
 *      average value for altitudes below 72 km.
 *
 *      d[5], TOTAL MASS DENSITY, is NOT the same for subroutines GTD7 
 *      and GTD7D
 *
 *        SUBROUTINE GTD7 -- d[5] is the sum of the mass densities of the
 *        species labeled by indices 0-4 and 6-7 in output variable d.
 *        This includes He, O, N2, O2, Ar, H, and N but does NOT include
 *        anomalous oxygen (species index 8).
 *
 *        SUBROUTINE GTD7D -- d[5] is the "effective total mass density
 *        for drag" and is the sum of the mass densities of all species
 *        in this model, INCLUDING anomalous oxygen.
 */



/* ------------------------------------------------------------------- */
/* --------------------------- PROTOTYPES ---------------------------- */
/* ------------------------------------------------------------------- */

/* GTD7 */
/*   Neutral Atmosphere Empircial Model from the surface to lower
 *   exosphere.
 */
void gtd7 (nrlmsise_input *input, \
           nrlmsise_flags *flags, \
           nrlmsise_output *output);


/* GTD7D */
/*   This subroutine provides Effective Total Mass Density for output
 *   d[5] which includes contributions from "anomalous oxygen" which can
 *   affect satellite drag above 500 km. See the section "output" for
 *   additional details.
 */
void gtd7d(nrlmsise_input *input, \
           nrlmsise_flags *flags, \
           nrlmsise_output *output);


/* GTS7 */
/*   Thermospheric portion of NRLMSISE-00
 */
void gts7 (nrlmsise_input *input, \
	       nrlmsise_flags *flags, \
	       nrlmsise_output *output);


/* GHP7 */
/*   To specify outputs at a pressure level (press) rather than at
 *   an altitude.
 */
void ghp7 (nrlmsise_input *input, \
           nrlmsise_flags *flags, \
           nrlmsise_output *output, \
           double press);



/* ------------------------------------------------------------------- */
/* ----------------------- COMPILATION TWEAKS ------------------------ */
/* ------------------------------------------------------------------- */

/* "inlining" of functions */
/*   Some compilers (e.g. gcc) allow the inlining of functions into the
 *   calling routine. This means a lot of overhead can be removed, and
 *   the execution of the program runs much faster. However, the filesize
 *   and thus the loading time is increased.
 */
#ifdef INLINE
#define __inline_double static inline double
#else
#define __inline_double double
#endif


    // void tselec(nrlmsise_flags *flags);
    // void gtd7(nrlmsise_input *input, nrlmsise_flags *flags, nrlmsise_output *output);
    // void spline_nrlmsise(double *x, double *y, int n, double yp1, double ypn, double *y2);
    // void splint(double *xa, double *ya, double *y2a, int n, double x, double *y);
    // void splini(double *xa, double *ya, double *y2a, int n, double x, double *y);
    // double dnet(double dd, double dm, double zhm, double xmm, double xm);
    // void glatf(double lat, double *gv, double *reff);
    // double ccor(double alt, double r, double h1, double zh);
    // double ccor2(double alt, double r, double h1, double zh, double h2);
    // void gts7(nrlmsise_input *input, nrlmsise_flags *flags, nrlmsise_output *output);
    // double globe7(double *p, nrlmsise_input *input, nrlmsise_flags *flags);
    // double glob7s(double *p, nrlmsise_input *input, nrlmsise_flags *flags);
    // double densu(double alt, double dlb, double tinf, double tlb, double xm, double alpha, double *tz, double zlb, double s2, int mn1, double *zn1, double *tn1, double *tgn1);
    // double densm(double alt, double d0, double xm, double *tz, int mn3, double *zn3, double *tn3, double *tgn3, int mn2, double *zn2, double *tn2, double *tgn2);

    void set_nrlmsise_input(double year_utc, double doy_utc, double sod_utc, double lat_long_alt[3], double Ap_daily, double *Aph, double f107_daily, double f107A);
    void set_nrlmsise_flags();
    double get_atmospheric_density_nrlmsise(double density[9], double temperature[2]);

    double Kp2Ap(double kp);
    bool get_f107_ap_kp(double mjd, double* f107A, double* f107_pre, double* Ap_daily, double *ap_array);

    extern solar_kp_ap_f107_storage g_solar_kp_ap_f107_storage;

#ifdef __cplusplus
}
#endif

#endif
