

/**
 * @file gnss_rtk.h
 * @brief Declarations for gnss rtk processing
 * @details
 * @author Dr. Zhen Li
 * @version 0.0.1
 * @date 2023.12.25
 * @note this file includes all the functions for gnss rtk processing
 * @since 2016.01.15
 * @copyright
 * @par History:
 *
 */

#ifndef EMBED

#ifndef _GNSS_PPP_H_
#define _GNSS_PPP_H_

#ifdef __cplusplus
extern "C"
{
#endif
#include "gnss_config.h"
#include "obs_processing.h"

// the number of parameter, 3 pos + trop + iono + Ns ambiguity
#define MAX_NX (3 + 3 + 5 + MAXSAT_PER_EPOCH * MAXNUM_FREQ)
#define INIT_ZWD 0.15

    typedef struct _RTK_CONFIG_
    {
        GTime time_epoch;
        double pos_vel_ref[3 + 3];       // the posiiton of reference
        double sigma_pos_vel_ref[3 + 3]; // the a priori sigma for the reference position
        int rtk_mode;                    // dynamic, static, and kinematic
        // int nx_pos = 3;
        // int nx_vel = 3;
        // int nx_trop = 1; // 1 for zwd, 2 for E and U component
        // int nx_iono = 0; // 0 for IF combination
        // int nx_clk  = 0; // double difference should have no rcv clock difference

        // int nx_total = nx_pos + nx_vel + nx_trop + nx_iono + nx_clk;

    } RTK_CONFIG;

    typedef struct _RUNTIME_RTK_
    {
        GTime obstime_rov;
        GTime obstime_ref;
        int satlist_used_rov[MAXSAT_PER_EPOCH]; // the common sat list in the observation
        int satlist_used_ref[MAXSAT_PER_EPOCH]; // the common sat list in the observation
        int nsat_used;                          // the number of common sat
        int index_refsat[NSYS];                 // the index of ref satellite in common sat list[0,1,2,... nsat_used]

        SOLUTION sln_ref, sln_rov; // the solution of reference and rover station

        // the runtime state of gnss data processing
        SAT_STATE runtime_state_ref[MAXSAT_GNSS];
        SAT_STATE runtime_state_rov[MAXSAT_GNSS];

        double interval_sec; /* time difference between current and previous (s) */
        bool IF_combination; // use IF combination or not

        int idx_xs_pos;
        int nx_pos;
        int idx_xs_vel;
        int nx_vel;
        int idx_xs_clk;
        int nx_clk;
        int idx_xs_trop;
        int nx_trop;
        int idx_xs_iono;
        int nx_iono;
        int idx_xs_amb;
        int nx_amb;

        int nx;
        PARAM_TYPE param_types[MAX_NX];
        double X[MAX_NX];
        double P[MAX_NX * MAX_NX];

        // the design matrix
        double H[MAXOBS_PER_SAT * MAXSAT_PER_EPOCH * MAX_NX]; // maxnum obs is  MAXOBS_PER_SAT*MAXSAT_PER_EPOCH, maxnum parameters is MAX_NX
        // the observation vector
        double L[MAXOBS_PER_SAT * MAXSAT_PER_EPOCH];

        int satindex[MAXSAT_PER_EPOCH];

    } RUNTIME_RTK;

    bool rtk_init();

    // single epoch relative positioning
    STATUS_SOL relative_positioning(OBSDATA_EPOCH *obsepoch_ref, OBSDATA_EPOCH *obsepoch_rov, SOLUTION *solt);

    /// @brief choose the satellite to be used
    int choose_common_satellite(OBSDATA_EPOCH *obsepoch_ref, OBSDATA_EPOCH *obsepoch_rov, int *satlist_common_ref, int *satlist_common_rov, int *index_refsat);

    void preprocess_rtk(OBSDATA_EPOCH *obsepoch_ref, OBSDATA_EPOCH *obsepoch_rov);

    // compute the single-difference OMC between ref and rov station
    void compute_omc_sd_station(OBSDATA_EPOCH *obsepoch_ref, OBSDATA_EPOCH *obsepoch_rov);

    // measurement update for the kalman filter
    static void update_measurement(OBSDATA_EPOCH *obsepoch_ref, OBSDATA_EPOCH *obsepoch_rov);

    void initx(RUNTIME_RTK *runtime_rtk, int idx, double xi, double var);
    void update_position(RUNTIME_RTK *runtime_rtk);
    void update_trop(RUNTIME_RTK *runtime_rtk, double tt);
    void update_ambiguity(RUNTIME_RTK *runtime_rtk, OBSDATA_EPOCH *obsepoch_ref, OBSDATA_EPOCH *obsepoch_rov);

    // time update for the kalman filter
    static void update_temporal();

#ifdef __cplusplus
}
#endif

#endif

#endif