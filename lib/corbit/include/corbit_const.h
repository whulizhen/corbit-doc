

/**
 * @file corbit_const.h
 * @brief @brief Constant definitions for the corbit library.
 * @version 0.0.1
 * @date 2016-2024
 * 
 * This file contains constant definitions used in the corbit library.
 * 
 * @author Dr. Zhen Li
 * @copyright
 * Copyright (C) 2016-2024 by Zhen Li
 * All rights reserved.
 * @par History:
 *
 */

#ifndef _CORBIT_CONST_H_
#define _CORBIT_CONST_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "ulog.h"

#if (_WIN32 || WIN64)
#define MY_FILE_NAME(x) strrchr(x, '\\') ? strrchr(x, '\\') + 1 : x
#else
#define MY_FILE_NAME(x) strrchr(x, '/') ? strrchr(x, '/') + 1 : x
#endif

// get the file name, line number and function name
#define GET_FILE_LINE_FUNC(x, str_out) sprintf(str_out, "%s:%d:%s()<%s>", MY_FILE_NAME(__FILE__), __LINE__, __func__, (x))


#if defined(_MSC_VER) && !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#endif


    // type definitions
    //  typedef double double;
    //  typedef int int;
    //  typedef unsigned short Uint;
    //  typedef char char;
    //  typedef bool BOOL;

#define VER_CORBIT "0.0.1"                                                          /**< corbit library version*/
#define COPYRIGHT_CORIBT "Copyright (C) 2016-2024 by Zhen Li\n All rights reserved." /**< corbit copy right*/


#define PI 3.1415926535897932384626433832795 /**< PI */
#define PI2 (2.0 * PI)                       /**< 2PI */
#define PI_half  (PI/2.0) 
#define Deg2Rad (PI / 180.0)                 /**< Radians per degree */
#define Rad2Deg (180.0 / PI)                 /**< Degrees per radian */
#define Rad2ArcSec  (3600.0 * 180.0 / PI)           /**< Arcseconds per radian */
#define ArcSec2Rad  (1.0/(3600.0 * 180.0 / PI))     /**< radian per Arcseconds */

#define AU 149597870000.0   /**< Astronomical unit [m]; IAU 1976  */
#define C_Light 299792458.0 /**< Speed of light  [m/s]; IAU 1976  */

#define SQR(x) ((x) * (x))                                 /**< square(x)*/
#define SQRT(x) ((x) <= 0.0 || (x) != (x) ? 0.0 : sqrt(x)) /**< square root x*/
#define MAX(x, y) ((x) > (y) ? (x) : (y))                  /**< maximum*/
#define MIN(x, y) ((x) < (y) ? (x) : (y))                  /**< minimum*/
#define ROUND(x) (int)floor((x) + 0.5)                     /**< round*/

/**
 * @brief Physical parameters of the Earth, Sun and Moon
 * */
#define R_WGS84 6378137.0            /**< Radius Earth [m]; WGS-84 */
#define F_WGS84 (1.0 / 298.257223563)  /**< Flattening; WGS-84*/
#define R_Sun 696000.0e3             /**< Radius Sun [m]*/
#define R_Moon 1738.0e3              /**< Radius Moon [m]*/
#define Omega_WGS 7.2921151467e-5    /**< [rad/s], the earth rotation rate */
#define GM_Earth 398600.4415e+9      /**< [m^3/s^2]; JGM3  */
#define GM_Sun 1.32712438e+20        /**< [m^3/s^2]; IAU 1976 */
#define GM_Moon (GM_Earth / 81.300587) /**< [m^3/s^2]; DE200  */

#ifdef __cplusplus
}
#endif

#endif