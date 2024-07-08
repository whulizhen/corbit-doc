
/**
 * @file ephemeris_fit.h
 * @brief Declarations of broadcast ephemeris generation
 * @details
 * @author Dr. Zhen Li
 * @version 0.0.1
 * @date 2023.12.25
 * @note this file includes all the functions for broadcast ephemeris generation
 * @since 2016.01.15
 * @copyright
 * @par History:
 *
 */

#ifndef EMBED

#ifndef _EPHMERIS_FIT_H_
#define _EPHEMERIS_FIT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdlib.h>
#include <math.h>
#include <string.h>

    /**
     * @brief definition of GPS broadcast ephemeris for the generation of broadcast ephemeris
     */
    typedef struct _ephemeris_gps_
    {
        double toe;  /**< toe */
        double a; /**< semi-major axis */
        double e; /**< eccentricity */
        double i0; /**< inclination */
        double OMEGA0;
        double omega;
        double M0;
        double dn;
        double i_dot;
        double OMEGA_dot;
        double Cus;
        double Cuc;
        double Crs;
        double Crc;
        double Cis;
        double Cic;
    } ephemeris_gps;

    void ephemeris_gps_clear(ephemeris_gps *eph);
    void compute_position_velocity(double t, ephemeris_gps *eph, double pos[3], double vel[3]);
    void set_data(double t, double a_in, double e_in, double i0_in,
                  double OMEGA0_in, double omega_in, double M0_in,
                  double dn_in, double i_dot_in, double OMEGA_dot_in,
                  double Cus_in, double Cuc_in, double Crs_in,
                  double Crc_in, double Cis_in, double Cic_in, ephemeris_gps *eph);

    void partial_xyz_partial_eph(double t, ephemeris_gps *eph, double H[3][15]);

#ifdef __cplusplus
}
#endif

#endif


#endif