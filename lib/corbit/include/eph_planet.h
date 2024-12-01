

/**
 * @file eph_planet.h
 * @brief Declarations of JPL planet ephemeris computation
 * @details
 * @author Dr. Zhen Li
 * @version 0.0.1
 * @date 2023.12.25
 * @note this file includes all the functions for JPL planet ephemeris computation
 * <p> refer to:
 * <p> https://ssd.jpl.nasa.gov/?planet_eph_export
 * <p> ftp://ssd.jpl.nasa.gov/pub/eph/planets/ascii/
 * <p> ftp://ssd.jpl.nasa.gov/pub/eph/planets/bsp
 * <p> https://naif.jpl.nasa.gov/naif/toolkit_C_MacIntel_OSX_AppleC_64bit.html
 * <p> https://arxiv.org/abs/1507.04291
 * <p> https://www.cv.nrao.edu/~rfisher/Ephemerides/ephem_descr.html
 * <p> https://www.cv.nrao.edu/~rfisher/Ephemerides/ephem_use.html
 * @since 2016.01.15
 * @copyright
 * @par History:
 *
 */

/***************************************************************************
*******                  JPLBIN.H   v.1.4                          *********
****************************************************************************
**  This header file is used by TESTEPH program.                          **
**  It is NECESSARY TO ADJUST IT MANUALLY for different ephemerides.      **
****************************************************************************
**  Written: May 28, 1997 by PAD  **  Last modified: March 13,2014 by PAD **
****************************************************************************
**  PAD: dr. Piotr A. Dybczynski,          e-mail: dybol@amu.edu.pl       **
**   Astronomical Observatory of the A.Mickiewicz Univ., Poznan, Poland   **
***************************************************************************/

#ifndef _EPH_PLANET_H_
#define _EPH_PLANET_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <math.h>
#include "common_function.h"

/* UNCOMMENT ONE AND ONLY ONE OF THE FOLLOWING DENUM DEFINITIONS: */
/*#define DENUM 200*/
/*#define DENUM 403*/
/*#define DENUM 404*/
/*#define DENUM 405*/
/*#define DENUM 406*/
/*#define DENUM 421*/
/*#define DENUM 422*/
/*#define DENUM 430*/
// #define DENUM 431
#define DENUM 438

#if DENUM == 200
#define KSIZE 1652
#elif DENUM == 403 || DENUM == 405 || DENUM == 421 || DENUM == 422 || DENUM == 430 || DENUM == 431 || DENUM == 438
#define KSIZE 2036
#elif DENUM == 404 || DENUM == 406
#define KSIZE 1456
#endif

#define NRECL 4
#define RECSIZE (NRECL * KSIZE)
#define NCOEFF (KSIZE / 2)

#ifdef EMBED
#define NMAX_CONST 1
#define MAXNUM_RECORD_JPLEPH 30
#else
#define NMAX_CONST 1000
#define MAXNUM_RECORD_JPLEPH 2048
#endif

    /**
     * @brief definitions of all planets in jpl ephemeris
     * @note
     * the order of planets is the same as that in group 1050
     */
    typedef enum _PLANET_JPL_
    {
        EARTH = 0,                /**< Earth, it is not listed in the JPL ephemeris planet list */
        MERCURY = 1,            /**< Mercury */
        VENUS = 2,                /**< Venus */
        EM_BARY = 3,            /**< Earth-Moon barycentre referenced at solar system barycentre */
        MARS = 4,                /**< Mars */
        JUPITER = 5,            /**< Jupiter */
        SATURN = 6,                /**< Saturn */
        URANUS = 7,                /**< Uranus */
        NEPTUNE = 8,            /**< Neptune */
        PLUTO = 9,                /**< Pluto */
        MOON = 10,                /**< Moon, geocentric position of the Moon is stored in the ephemeris */
        SUN = 11,                /**< Sun */
        EARTH_NUTATION = 12,    /**< Earth Nutations in longitude and obliquity (IAU 1980 model) */
        LUNAR_LIBRATION = 13,    /**< Lunar mantle libration */
        LUNAR_ANGULAR_VEL = 14, /**< Lunar mantle angular velocity*/
        TTmTDB = 15,            /**< TT-TDB (at geocenter) */
        SOLAR_BARY = 16,        /**< solar system barycentre */
        TOTAL_PLANET = 17        /**<the total number celestial bodies */
    } PLANET_JPL;

    /**
     * @brief the definition of EPH_HEADER_JPL
     * @note we use long int instead of int for DOS-Linux compatibility
     */
    typedef struct _eph_hearder_jpl_
    {
        int con_num;                   /**<  num of constants */
        int ipt[TOTAL_PLANET][3];       /**< group 1050, the start_pos, num_coeff, and num_blocks for each body */
        double au;                       /**< value of AU */
        double emrat;                   /**< value of Earth-Moon mass rate */
        char con_name[NMAX_CONST][20]; /**< constant name, group 1040 */
        double con_val[NMAX_CONST];       /**< constant values, group 1041 */
        double ss[3];                   /**< group 1030, ss[0]: start julian date, ss[1]: end julian date, ss[2]: span days = 32.0,
                                            Word (1,i) is the starting location in each data record of the chebychev coefficients belonging to the ith item. start from 1, not 0.
                                            Word (2,i) is the number of chebychev coefficients per component of the ith item,
                                            Word (3,i) is the number of complete sets of coefficients in each data record for the ith item.
                                       */
    } EPH_HEADER_JPL;

    /**
     * @brief definition of one data record of JPL ephemeris
     */
    typedef struct _eph_record_jpl_
    {
        int index; /**< index of the record */
        int coef_num; /**< the total number of coefficients in the data block, mostly 1018 */
        double chebychev_coef[NCOEFF]; /**<  one data record block, record[0] is the start JD and record[1] is the end JD */
    } EPH_RECORD_JPL;

    
#ifdef __cpluscplus
    extern "C"
    {
#endif
        void initialize_global_eph_planet();
        void split_double(double tt, double fr[2]);
        void interp(PLANET_JPL planet, double coef_record[], double t[2], int ncm, int ifl, double *posvel);

        bool load_eph_storage_default(EPH_HEADER_JPL *eph_header, EPH_RECORD_JPL *eph_storage);
        bool load_eph_header_default(EPH_HEADER_JPL *eph_header);

#ifndef EMBED
        bool load_eph_storage_file(char *eph_filename, EPH_HEADER_JPL *eph_header, EPH_RECORD_JPL *eph_data);
        void EphTest(char *testfilename);
#endif
        bool compute_planet_pos_vel(double tt_jd, PLANET_JPL ncent, PLANET_JPL ntarg, double *pv);

        void compute_pos_sun_moon_eci(double mjd_tt, double *rsun_eci, double *rmoon_eci);

        /// @brief definition of global variables
        extern const double PLANET_GM[TOTAL_PLANET + 1];
        extern EPH_HEADER_JPL *g_eph_header;
        // the eph data storage
        extern EPH_RECORD_JPL *g_ephData_storage;
        extern int g_num_eph_record;

#ifdef __cplusplus
    }
#endif

#endif
