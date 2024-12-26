

/**
 * @file obs_processing.h
 * @brief Declarations for observation processing functions
 * @details These functions are for observation processing functions
 * @author Dr. Zhen Li
 * @version 0.0.1
 * @date 2023.12.25
 * @note this file includes all the functions for observation processing
 * @since 2016.01.15
 * @copyright
 * @par History:
 *
 */

#ifndef _OBS_PROCESSING_H_
#define _OBS_PROCESSING_H_

#ifdef __cplusplus
extern "C"
{
#endif


#ifndef EMBED
#include <stdio.h>
#include <stdlib.h>
#endif

#include "corbit_const.h"
#include "gnss_observation.h"
#include "gnss_satposition.h"
#include "common_function.h"
#include "mathlib.h"
#include "eph_planet.h"
#include "earth_rotation_param.h"
    

    /**
     * @brief param type enum definiton for the estimation
     */
    typedef enum _param_type_
    {
        //on the tranmission side, mainly receivers
        param_RX_POS_X,  /**< receiver position x */
        param_RX_POS_Y, /**< receiver position y */
        param_RX_POS_Z, /**< receiver position z */
        param_RX_VEL_X, /**< receiver velocity x */
        param_RX_VEL_Y, /**< receiver velocity y */
        param_RX_VEL_Z, /**< receiver velocity z */
        param_RX_GPS_CLK, /**< receiver clock offset from GPS */
        param_RX_BDS_CLK, /**< receiver clock offset from BDS */
        param_RX_GAL_CLK, /**< receiver clock offset from GAL */
        //on the reception side, mainly GNSS satellites
        param_TX_POS_X,  /**< satellite position x */
        param_TX_POS_Y, /**< satellite position y */
        param_TX_POS_Z, /**< satellite position z */
        param_TX_VEL_X, /**< satellite velocity x */
        param_TX_VEL_Y, /**< satellite velocity y  */
        param_TX_VEL_Z, /**< satellite velocity z  */
        param_TX_GPS_CLK, /**< satellite clock offset GPS  */
        param_TX_BDS_CLK, /**< satellite clock offset BDS  */
        param_TX_GAL_CLK, /**< satellite clock offset GAL  */
        param_ZWD, /**< the zwd for tropsheric delay  */
        param_AMB /**< the phase ambiguity  */
    } PARAM_TYPE;

    ///definition of the estimated param 
    // typedef struct _param_
    // {
    //     PARAM_TYPE  type;
    //     double value;
    //     double sigma;
    // }PARAM;
    

    // typedef struct _AMB_STATE_
    // {
    //     sysGNSS satsys;
    //     int prn;
    //     double amb_float[MAXNUM_FREQ]; // ambiguity in cycles
    //     double amb_sigma[MAXNUM_FREQ]; // ambiguity sigma[cycle]
    //     double amb_fixed[MAXNUM_FREQ]; // the fixed ambiguity

    // } AMB_STATE;

    // typedef enum _enum_state_cycle_slip
    // {
    //     NO_CYCLE_SLIP,
    //     CYCLE_SLIP,
    //     UKN_CLCYLE_SLIP
    // } ENUM_STATE_CYCLE_SLIP;
    
    typedef struct _sat_state_
    {
        sysGNSS satsys;
        int prn;
        // double azel[2]; // az el in rad
        // double resp[MAXNUM_FREQ]; /* residuals of pseudorange (m) */
        // double resc[MAXNUM_FREQ]; /* residuals of carrier-phase (m) */
        // uint8_t vsat[NFREQ]; /* valid satellite flag */
        // uint16_t snr[NFREQ]; /* signal strength (*SNR_UNIT dBHz) */
        int flag_fix[MAXNUM_FREQ];  /* ambiguity fix flag (1:fix,2:float,3:hold) */
        int flag_slip[MAXNUM_FREQ]; /* cycle-slip flag, 0=no cycle slip, 1=cycle slip, 2 means initialization with cycle slips*/
        int flag_half[MAXNUM_FREQ]; /* half-cycle valid flag */

        double amb_float[MAXNUM_FREQ]; // ambiguity in cycles
        double amb_sigma[MAXNUM_FREQ]; // ambiguity sigma[cycle]
        double amb_fixed[MAXNUM_FREQ]; // the fixed ambiguity
        
        int lock[MAXNUM_FREQ];            /* lock counter of phase */
        int outc[MAXNUM_FREQ];       /* obs outage counter of phase */
        int slipc[MAXNUM_FREQ];      /* cycle-slip counter */
        // int rejc[MAXNUM_FREQ];       /* reject counter */
        double gf_phase[MAXNUM_FREQ ]; /* geometry-free phase (m) */
        double gf_range[MAXNUM_FREQ ]; /* geometry-free phase (m) */
        double if_phase[MAXNUM_FREQ ];
        double if_code[MAXNUM_FREQ ];
        double mw[MAXNUM_FREQ ]; /* MW-LC (m) */
        double mpc[MAXNUM_FREQ];    // the multi-path combination
        double phw;                 /* phase windup (cycle), noting to do with frequency */
        
        //GTime pt[2][MAXNUM_FREQ];  /* previous carrier-phase time */
        //double ph[2][MAXNUM_FREQ]; /* previous carrier-phase observable (cycle) */
        ANT_INFO*  ant_info;  /**< the antenna information at current time*/
    } SAT_STATE;

    /**< This is a function pointer to get the parameter index, param_type, satsys, prn, index of the frequecy from gnssconfig, the satindex list in current epoch*/
    typedef int (*index_param_func) (PARAM_TYPE , sysGNSS, int, int, int* );
    
    // clear values in sat_state
    void clear_sat_state(SAT_STATE *sat_state);

    STATUS_SOL pseudorange_point_positioning(OBSDATA_EPOCH *obs_epoch, ATT_QUATERNION* qua_rcv_platform, ANT_INFO* antenna_rcv, SOLUTION *solt);
    
    //point positioning using doppler observations, not implemented now
    // STATUS_SOL doppler_point_positioning(OBSDATA_EPOCH *obs_epoch, SOLUTION *solt);
    
    void get_amb_from_pseudorange(OBSDATA_EPOCH *obsepoch, SAT_STATE *sat_state_all);
    void get_gf_mw(OBSDATA_EPOCH *obs_epoch, GNSSCONFIG *gnsscfg, SAT_STATE *runtime_state);

    // get the carrier smoothed code using hatch filter
    // void get_carrier_smoothed_code();
    
    /*
    ref: https://groops-devs.github.io/groops/html/fundamentals.robustLeastSquares.html
    */
    void compute_obs_redundancy(int nobs, int nx, double *A, double *W, double *R);

    double propagation_time_iterate(GTime obstime, int satindex, double rcv_clock, double *station_pos, double *satposvel, double *satclk, double *tgd);
    double propagation_time_use_range(GTime obstime, int satindex, double range, double *station_pos, double *satposvel, double *satclk, double *tgd);

    int residual_control(int nsat, int *satidx_in_obs_epoch, int *satlist_satindex, OBSDATA_EPOCH *obs_epoch, int *satindex_exclude, bool *satindex_list_indicator);
    
    // compute the observation minues computation
    // V = L - H.X
    int compute_omc_zd_gnss(int nsat, int *satidx_in_obs_epoch, int *satlist_satindex, OBSDATA_EPOCH *obs_epoch,
                            double *sunposvel_ecf, ATT_QUATERNION *att_rcv, ANT_INFO *ant_rcv,
                            int nx, PARAM_TYPE *param_types, int *index_paramX,
                            index_param_func idxfunc,
                            double *X, SAT_STATE *sat_state,
                            int nobs_max, int nx_max, double *h, double *v, double *r);

    void standard_pres_temp(double *blh, double *pres, double *temp);
    double MappingFunction_simple(double el, double *map_dry_wet);
    double saastamonien(double *blh, double temp, double pres, double humi, double *zhd_zwd);
    double NMF(double doy, double *pos_lat_lon_hgt, double *azel, double *map_dry_wet);
    double trop_delay(double mjd_gpst, double *blh, double *azel, double *dry, double *wet, double humi);

    double iono_map_FK_LEO(double *blh, double *azel);
    double iono_map_single_layer(double *blh, double *azel);
    double iono_pierce_point(double *blh, double *azel, double RE, double hion, double *pblh);
    double Klobuchar_BDS(double a[4], double b[4], double tE, double *user_geo, double *azel);
    double Klobuchar_BDS_stec(double a[4], double b[4], double tE, double *user_geo, double *azel);
    double Klobuchar_GPS(double a[4], double b[4], double tE, double *user_geo, double *azel);
    double Klobuchar_GPS_stec(double a[4], double b[4], double tE, double *user_geo, double *azel);

    double iono_map_BDGIM(double *azel);
    
    double Nequick_GAL(double a[3], double *user_geo, double *sat_geo, double UT, int mth);

    void model_phase_windup(double *exs, double *eys, double *ek, double *exr, double *eyr, double *phw);

#ifndef EMBED
    double ambfix_nearest_integer(double amb_float, double sigma_amb);
#endif


#ifdef __cplusplus
}
#endif

#endif
