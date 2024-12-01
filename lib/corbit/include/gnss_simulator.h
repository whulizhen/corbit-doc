

/**
 * @file gnss_simulator.h
 * @brief Declarations of all the gnss simulations
 * @details
 * @author Dr. Zhen Li
 * @version 0.0.1
 * @date 2023.12.25
 * @note this file includes all the functions for gnss simulation
 * @since 2016.01.15
 * @copyright
 * @par History:
 *
 */

//not for the embed version
#ifndef EMBED

#ifndef _GNSS_SIMULATOR_H_
#define _GNSS_SIMULATOR_H_


#ifdef __cplusplus
extern "C"
{
#endif

#include "gnss_config.h"
#include "gnss_observation.h"
#include "gnss_satposition.h"




typedef struct _config_simulator_
{
    GTime start_gpst;
    GTime end_gpst;
    double interval; // in seconds
    double ele_cutoff;         /**< cutoff elevation in radian */
    
    char filename_user[MAXLEN_STRING];  // the file path of the user position, velocity [in ecef] and clock offset
    char filename_ephemeris_brdc[MAXLEN_STRING];
    char filename_ephemeris_prec[MAXLEN_STRING];
    int  eph_opt;  // ephemeris option

    double noise_range;  // pseodorange in meter
    double noise_phase;   // carrier phase in meter
    double noise_doppler; // the Doppler in Hz
    double noise_cnr;  // the CNR dbm

    sysGNSS sys_used[NSYS]; /**< all the gnss system used */
    int num_sys;            /**< number of gnss system used */

    int num_obs_freq_code[NSYS]; /**< num of obs for each sat in this sysGNSS */
    int num_freq[NSYS];          /**< number of frequency in the observation for each satellite system */
    TYPE_FREQ_CODE obs_type_freq_code_used[NSYS][MAXOBS_PER_SAT]; /**< obs frequency used for each satellite system*/
}CONFIG_SIMULATOR;







    void simulate();

    //deal with the waypoint files
    void read_waypoints();
    void write_waypoints();

#ifdef __cplusplus
}
#endif

#endif

#endif
