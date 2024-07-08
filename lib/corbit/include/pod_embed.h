
/**
 * @brief Header file for embeded realtime orbit determination
 * @file pod_embed.h
 * @details The declaration of functions used in embeded realtime orbit determination
 * @author Dr. Zhen Li
 * @version 0.0.1
 * @date 2023.12.25
 * @note this file includes all the functions for embeded realtime orbit determination
 * @since 2016.01.15
 * @par Copyright (c)
 * @par History:
 *
 */

#ifndef _POD_EMBED_H_
#define _POD_EMBED_H_

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef EMBED
#include <stdlib.h>
#include <stdio.h>
#endif

#include "corbit_const.h"
#include "mathlib.h"
#include "gnss_config.h"
#include "gnss_observation.h"
#include "gnss_satposition.h"
#include "common_function.h"
#include "orbit_dynamics.h"
#include "eph_planet.h"
#include "earth_rotation_param.h"
#include "obs_processing.h"

#define MAXNUM_FM_PARAM_RT 3
#define MAXNUM_X (3 + 3 + NSYS + MAXSAT_PER_EPOCH * MAXNUM_FREQ) /**<maxnum of unknowns*/

#define MAXNUM_STORE_POS 5
#define MAXNUM_STORE_CLK 5

#ifndef EMBED
    #define NUM_ANT_INFO MAXSAT_GNSS
#else
    #define NUM_ANT_INFO 2
#endif


    /**
     * @brief orbit data
     *
     */
    typedef struct _orbit_data_
    {
        GTime epoch_gpst; // GPS time
        double pos_ecf[3];
        double vel_ecf[3];
        double var_pos[3];
        // double var_vel[3];
        // unsigned int status; // data status
        /* data */
    } DATA_ORBIT;

    /**
     * @brief clk data
     *
     */
    typedef struct _clk_data_
    {
        GTime epoch_gpst;        // GPS time
        double clk_offset[NSYS]; // in m
        // double clk_drift[NSYS];  // in m/s
        // double var_clk_offset[NSYS];
        // unsigned int status[NSYS]; // status
    } DATA_CLK;


    /**
     * @brief the data needed in runtime
     *
     */
    typedef struct _RUNTIME_POD_EMBED_
    {
        int index_object;           /**< object index in the all the satellite list*/
        GTime time_now;             /**< current time in GPST removed the receiver clock offset*/
        OBSDATA_EPOCH obs_epoch[2]; /**< store 3 epochs of observation, 0 is the current obs, memory: 632*3*/
        int obs_num;
        
        EPHBRDC_STORE brdc_storage; /**< all the broadcast ephemeris for GNSS satellite*/
#ifndef EMBED
        EPHPREC_STORE prec_storage; /**< all the precise ephemeris for GNSS satellite*/
        FILE *pf_output;
        char filename_output[1024]; /**< for the output */
#endif

        EOPREC eop_storage[3];      // only store 3 days EOP
        
        // store the latest 10 orbit and clk information
        DATA_ORBIT dataset_orbit[MAXNUM_STORE_POS]; // including predicted pos+vel
        DATA_CLK dataset_clk[MAXNUM_STORE_CLK];     // including the predicted clk offset
        int num_orb_store;
        int num_clk_store;
        
        int satlist_used_in_obsepoch[MAXSAT_PER_EPOCH]; /**< the satellite index in the current obsepoch satlist*/ 
        int nsat_used;
        int satlist_satindex_now[MAXSAT_PER_EPOCH]; /**< the satellite index(global) in the current epoch */
        SOLUTION sln_kine;

        int satlist_used_in_obsepoch_last[MAXSAT_PER_EPOCH]; /**< the satellite index in the last obsepoch satlist*/
        int nsat_used_last;
        int satlist_satindex_last[MAXSAT_PER_EPOCH];
        
        SAT_STATE state_sat_all[MAXSAT_GNSS]; // satellite state of all GNSS satellites, memory: 536 Bytes*MAXSAT_GNSS = 56280 bytes
        
        //the following definition is to avoid dynamic memory allocation
        // ANT_INFO ant_info_all[NUM_ANT_INFO]; // to store all the antenna information of both GNSS satellites and LEO satellites
        // ANT_INFO_STORE ant_info_store; // including both the GNSS and LEO satellite ant info, should be pointed by the global pointer
        ANT_INFO ant_info_rcv;  //  leo satellite receiver antenna information
        ATT_QUATERNION att_rcv;  // leo satellite receiver quaternion, got from satellite platform
        
        EPH_HEADER_JPL jpl_header;  // jpl ephemeris header
        EPH_RECORD_JPL jpl_eph_data[1]; //jpl ephemeris data records  
        
        // these are middle variables for zd_omc computation
        PARAM_TYPE param_list[MAXNUM_X];
        int index_param_X[MAXNUM_X];  // the index of params in the X vector, memeory 74*4=296 bytes
        int num_param;
        
        // these are for the EKF
        double DX[MAXNUM_X * MAXNUM_X]; /**< covariance of the state X, memory 74*74*8=43808 bytes*/
        double X[MAXNUM_X];             /**< state X in the filter. pos+vel+clk+dynamic_param + amb. there are nsys clk offset, memory: 74*8*/

        //for the orbit dynamic filter
        ORBIT_DYN_CONFIG orbit_dyn_config; /**< orbit dynamics control, interface to orbit dynamics*/
        STATE_SPACECRAFT state_satellite;  /**< spacecraft state, the interface for orbit dynamics */
        int orbit_integration_progress;  /**< the progess bar of orbit integration process*/
        double sln_orbit_interval; // interval in seconds for orbit solution, could be 10~30 secs
        double X_dyn[6 + MAXNUM_FM_PARAM_TOTAL];
        double DX_dyn[(6 + MAXNUM_FM_PARAM_TOTAL) * (6 + MAXNUM_FM_PARAM_TOTAL)];

    } RUNTIME_POD_EMBED;

    // update the gnss observations
    // obs_storage is the variable that store all the possibile GNSS observations
    bool update_gnss_observation(OBSDATA_EPOCH* obs_data, RUNTIME_POD_EMBED *runtime_pod);

    bool update_gnss_ephemeris_gps(EPHREC_GPS *ephbrdc_gps_storage);

    bool update_gnss_ephemeris_glo(EPHREC_GLO *ephbrdc_glo_storage);

    bool update_gnss_ephemeris_pre(EPHREC_GLO *ephbrdc_glo_storage);
    
    bool get_initial_pos_vel_clk(RUNTIME_POD_EMBED *pod_runtime);
    void get_initial_ambiguity(RUNTIME_POD_EMBED *runtime_pod);
    void get_initial_ambiguity_nomemory(RUNTIME_POD_EMBED *runtime_pod);
    void get_initial_ambiguity_test(RUNTIME_POD_EMBED *runtime_pod);
    
    void startup_pod_embed(RUNTIME_POD_EMBED *runtime_pod,bool test_env_initialized);
    // main entry of pod_embed
    void run_single_pod_embed(OBSDATA_EPOCH* obs_data, RUNTIME_POD_EMBED *runtime_pod);
    
    // solve the receiver clk offset
    bool pod_embed_solve_clk_offset(OBSDATA_EPOCH *obsdata, RUNTIME_POD_EMBED *runtime_pod);
    bool pod_embed_solve_orbit(OBSDATA_EPOCH *obsdata, RUNTIME_POD_EMBED *runtime_pod);
    bool orbit_dynamic_filter(RUNTIME_POD_EMBED *runtime_pod);
    
    void store_data_orbit(RUNTIME_POD_EMBED *runtime_pod);
    void store_data_clock(RUNTIME_POD_EMBED *runtime_pod);

#ifdef __cplusplus
}
#endif

#endif
