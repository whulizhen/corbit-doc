
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

#define MAXNUM_STORE_POS 10
#define MAXNUM_STORE_CLK 10

#define MAXNUM_INTEGRATION_OUTPUT  (10)
#define MAXNUM_INTEGRATION_STATE (6 + 36 + 6 * MAXNUM_FM_PARAM_TOTAL)

#ifndef EMBED
    #define NUM_ANT_INFO MAXSAT_GNSS
#else
    #define NUM_ANT_INFO 2
#endif

    /**
     * @brief the telemetry information of the pod_embed
     * 
     */
    typedef struct _pod_embed_telemetry_
    {
        char time_gps_now[50];  // GPS time now
        char status_orbit;  // status of the solution, S for spp,  P for PPP,  D for dynamic,  F for orbit forcast,
        char status_clock;  // status of the solution, S for spp,  P for PPP,  D for dynamic,  F for orbit forcast,

    }POD_EMBED_TELEMETRY;

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
        double clk_offset[NSYS]; // in sec
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
        RTCM rtcm_storage; /**< the rtcm storage  for GNSS satellites */
        
#ifndef EMBED
        EPHPREC_STORE prec_storage; /**< all the precise ephemeris for GNSS satellite*/
#endif
        
        EOPREC eop_storage[MAXSIZE_EOP];      // only store 3 days EOP
        
        // store the latest 10 orbit and clk information
        DATA_ORBIT dataset_orbit[MAXNUM_STORE_POS]; // including predicted pos+vel
        DATA_CLK dataset_clk[MAXNUM_STORE_CLK];     // including the predicted clk offset
        int num_orb_store;
        int num_clk_store;
        unsigned int num_epoch_count;
        SOLUTION sln_kine;
        STATUS_SOL sol_status;

        // *!!!!satlist_used_in_obsepoch, nsat_used and satlist_satindex_now SHOULD NOT BE CHANGED as it related to the order of parameters (X and DX)
        int nsat_used;
        int satlist_used_in_obsepoch[MAXSAT_PER_EPOCH]; /**< the satellite index in the current obsepoch satlist*/ 
        int satlist_satindex_now[MAXSAT_PER_EPOCH]; /**< the satellite index(global) in the current epoch */
        bool satindex_list_indicator_now[MAXSAT_PER_EPOCH];

        int nsat_used_last;
        int satlist_used_in_obsepoch_last[MAXSAT_PER_EPOCH]; /**< the satellite index in the last obsepoch satlist*/
        int satlist_satindex_last[MAXSAT_PER_EPOCH];
        bool satindex_list_indicator_last[MAXSAT_PER_EPOCH];
        
        SAT_STATE state_sat_all[MAXSAT_GNSS]; // satellite state of all GNSS satellites, memory: 536 Bytes*MAXSAT_GNSS = 56280 bytes
        
        //the following definition is to avoid dynamic memory allocation
        ANT_INFO* ant_info_rcv; // pointer to current ant_info
        ANT_INFO ant_info_all[2]; //  leo satellite receiver with 2 antenna information
        ANT_INFO_STORE ant_info_store;
        
        ATT_QUATERNION att_rcv; // leo satellite receiver quaternion, got from satellite platform

        EPH_HEADER_JPL jpl_header;  // jpl ephemeris header
        EPH_RECORD_JPL jpl_eph_data[15]; //jpl ephemeris data records num is 14, determined by  line 590 in file eph_planet.c (function load_eph_storage_default)
        
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
        STATE_SPACECRAFT state_orbit_prediction; /**< store the orbit prediction results */
        GTime initial_condition_epoch_utc;
        double initial_condition_eci[6];
        bool kinematic_or_dynamic;
        bool initial_condition_status;
        // the following are to store the orbit integration state
        double integration_state[MAXNUM_INTEGRATION_OUTPUT * MAXNUM_INTEGRATION_STATE]; /**< ystate[6] + PHI[36] + S[6*np] */
        double integration_epoch[MAXNUM_INTEGRATION_OUTPUT];
        double time_correlation;
        bool started_dynamic_filter;
        int orbit_integration_progress; /**< the progess bar of orbit integration process*/
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
    // void get_initial_ambiguity(RUNTIME_POD_EMBED *runtime_pod);
    // void get_initial_ambiguity_nomemory(RUNTIME_POD_EMBED *runtime_pod);
    void get_initial_ambiguity_test(RUNTIME_POD_EMBED *runtime_pod);
    
    void startup_pod_embed(RUNTIME_POD_EMBED *runtime_pod,bool test_env_initialized);
    // main entry of pod_embed
    void run_single_pod_embed(OBSDATA_EPOCH* obs_data, RUNTIME_POD_EMBED *runtime_pod);
    
    // solve the receiver clk offset
    bool pod_embed_solve_clk_offset(OBSDATA_EPOCH *obsdata, RUNTIME_POD_EMBED *runtime_pod);
    bool pod_embed_solve_orbit(OBSDATA_EPOCH *obsdata, RUNTIME_POD_EMBED *runtime_pod);
    bool orbit_dynamic_filter(RUNTIME_POD_EMBED *runtime_pod);

    void store_integration(RUNTIME_POD_EMBED *runtime_pod, int integration_progress);
    void store_data_orbit(RUNTIME_POD_EMBED *runtime_pod);
    void store_data_clock(RUNTIME_POD_EMBED *runtime_pod);

#ifdef __cplusplus
}
#endif

#endif
