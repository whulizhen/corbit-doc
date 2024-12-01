


/**
 * @brief Header file for TLE computation
 * @file tle.h
 * @details The declaration of tle functions
 * @author Dr. Zhen Li
 * @version 0.0.1
 * @date 2023.12.25
 * @note this file includes all the functions for TLE computation
 * @since 2016.01.15
 * @par Copyright (c)
 * @par History:
 *
 */

#ifndef EMBED


/*------------------------------------------------------------------------------
 * tle.c: NORAD TLE (two line element) functions
 *
 *          Copyright (C) 2012-2013 by T.TAKASU, All rights reserved.
 *
 * references:
 *     [1] F.R.Hoots and R.L.Roehrich, Spacetrack report No.3, Models for
 *         propagation of NORAD element sets, December 1980
 *     [2] D.A.Vallado, P.Crawford, R.Hujsak and T.S.Kelso, Revisiting
 *         Spacetrack Report #3, AIAA 2006-6753, 2006
 *     [3] CelesTrak (http://www.celestrak.com)
 *
 * version : $Revision:$ $Date:$
 * history : 2012/11/01 1.0  new
 *           2013/01/25 1.1  fix bug on binary search
 *           2014/08/26 1.2  fix bug on tle_pos() to get tle by satid or desig
 *-----------------------------------------------------------------------------*/
#ifndef _TLE_H_
#define _TLE_H_

#ifdef __cplusplus
extern "C"
{
#endif
#include "corbit_const.h"
#include "gtime.h"
#include "earth_rotation_param.h"
#include "mathlib.h"

typedef struct
{                   /* norad two line element data type */
    char name[32];  /* common name */
    char alias[32]; /* alias name */
    char satno[16]; /* satellilte catalog number */
    char satclass;  /* classification */
    char desig[16]; /* international designator */
    // double epoch;   /* element set epoch (UTC) */
    GTime epoch_mjd_utc; // element set epoch (UTC) in mjd
    double ndot;    /* 1st derivative of mean motion */
    double nddot;   /* 2st derivative of mean motion */
    double bstar;   /* B* drag term */
    int etype;      /* element set type */
    int eleno;      /* element number */
    double inc;     /* orbit inclination (deg) */
    double OMG;     /* right ascension of ascending node (deg) */
    double ecc;     /* eccentricity */
    double omg;     /* argument of perigee (deg) */
    double M;       /* mean anomaly (deg) */
    double n;       /* mean motion (rev/day) */
    int rev;        /* revolution number at epoch */
} tled_t;

typedef struct
{                 /* NORAD TLE (two line element) type */
    int n, nmax;  /* number/max number of two line element data */
    tled_t *data; /* NORAD TLE data */
} tle_t;

// static void SGP4_STR3(double tsince, const tled_t *data, double *rs);
extern int tle_read(const char *file, tle_t *tle);
extern int tle2xyz(const tled_t *tledata, GTime time_utc, double *pos_vel);

int tle_test();

#ifdef __cplusplus
}
#endif

#endif

#endif
