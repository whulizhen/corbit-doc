
/**
 * @file gnss_config.h
 * @brief Declarations for global variable gnssconfig
 * @details
 * @author Dr. Zhen Li
 * @version 0.0.1
 * @date 2023.12.25
 * @note this file includes all the functions for gnssconfig
 * @since 2016.01.15
 * @copyright
 * @par History:
 *
 */

#ifndef _GNSS_CONFIG_H_
#define _GNSS_CONFIG_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdio.h>
#include <stdlib.h>

#include <stdbool.h>
#include <string.h>
#include <math.h>
#include "corbit_const.h"
#include "common_function.h"
#include "gtime.h"


#ifdef EMBED

/// enable GPS
#define ENAGPS
// /// enable BeiDou
// #define ENABDS

#else

#define NameAsStr(x) (#x)                                   /**< convert variable to its name*/
#define ENUM_STR(x, str) (sprintf(str, "%s", NameAsStr(x))) /**< convert enum varialbe to its name*/

/// enable GPS
#define ENAGPS

// /// enable BeiDou
#define ENABDS

/// enable LEO
#define ENALEO

/// enable GAL
// #define ENAGAL

/// enable GLO
// #define ENAGLO

///enable STATION
#define ENASTA

#endif



#ifdef ENAGPS
#define MINPRNGPS 1                         /**< min satellite PRN number of GPS */
#define MAXPRNGPS 32                        /**< max satellite PRN number of GPS */
#define NSATGPS (MAXPRNGPS - MINPRNGPS + 1) /**< number of GPS satellites */
#define NSYSGPS 1                           /**< NSYSGPS for gps */
#else
#define MINPRNGPS 0 /**< min satellite PRN number of GPS */
#define MAXPRNGPS 0 /**< max satellite PRN number of GPS */
#define NSATGPS 0   /**< number of GPS satellites */
#define NSYSGPS 0   /**< NSYSGPS for GPS */
#endif

#ifdef ENABDS
#define MINPRNBDS 1                         /**< min satellite PRN number of BDS */
#define MAXPRNBDS 63                        /**< max satellite PRN number of BDS */
#define NSATBDS (MAXPRNBDS - MINPRNBDS + 1) /**< number of BDS satellites */
#define NSYSBDS 1                           /**< NSYSBDS for bds */
#else
#define MINPRNBDS 0 /**< min satellite PRN number of BDS */
#define MAXPRNBDS 0 /**< max satellite PRN number of BDS */
#define NSATBDS 0   /**< number of GPS satellites */
#define NSYSBDS 0   /**< NSYSBDS for bds */
#endif

#ifdef ENAGAL
#define MINPRNGAL 1                         /**< min satellite PRN number of GAL */
#define MAXPRNGAL 36                        /**< max satellite PRN number of GAL */
#define NSATGAL (MAXPRNGAL - MINPRNGAL + 1) /**< number of GAL satellites */
#define NSYSGAL 1                           /**< NSYSGAL for gal */
#else
#define MINPRNGAL 0 /**< min satellite PRN number of GAL */
#define MAXPRNGAL 0 /**< max satellite PRN number of GAL */
#define NSATGAL 0   /**< number of GAL satellites */
#define NSYSGAL 0   /**< NSYSGAL for bds */
#endif

#ifdef ENAGLO
#define MINPRNGLO 1                         /* min satellite sat number of GLO */
#define MAXPRNGLO 30                        /* max satellite sat number of GLO */
#define NSATGLO (MAXPRNGLO - MINPRNGLO + 1) /* number of GLO satellites */
#define NSYSGLO 1                           /**< NSYSGLO for glo */
#else
#define MINPRNGLO 0
#define MAXPRNGLO 0
#define NSATGLO 0
#define NSYSGLO 0
#endif

#ifdef ENALEO
#define MINPRNLEO 1                         /* min satellite sat number of LEO */
#define MAXPRNLEO 2                         /* max satellite sat number of LEO */
#define NSATLEO (MAXPRNLEO - MINPRNLEO + 1) /* number of LEO satellites */
#define NSYSLEO 1                           /**< NSYSLEO for leo */
#else
#define MINPRNLEO 0
#define MAXPRNLEO 0
#define NSATLEO 0
#define NSYSLEO 0
#endif

#ifdef ENAGEO
#define MINPRNGEO 1                         /* min satellite sat number of GEO */
#define MAXPRNGEO 10                        /* max satellite sat number of GEO */
#define NSATGEO (MAXPRNGEO - MINPRNGEO + 1) /* number of GEO satellites */
#define NSYSGEO 1                           /**< NSYSGEO for GEO satellite */
#else
#define MINPRNGEO 0
#define MAXPRNGEO 0
#define NSATGEO 0
#define NSYSGEO 0
#endif

// this is for ground station
#ifdef ENASTA
#define MINNUMSTA 1                      /* min index of ground station */
#define MAXNUMSTA 10                     /* max index of ground station */
#define NSTA (MAXNUMSTA - MINNUMSTA + 1) /* number of ground stations */
#define NSYSSTA 1
#else
#define MINNUMSTA 0
#define MAXNUMSTA 0
#define NSTA 0
#define NSYSSTA 0
#endif

#define NSYS (NSYSGPS + NSYSGLO + NSYSGAL + NSYSBDS + NSYSLEO + NSYSGEO)                    /**< number of gnss systems */
#define MAXSAT_GNSS (MAXPRNGPS + MAXPRNBDS + MAXPRNGAL + MAXPRNGLO + MAXPRNGEO + MAXPRNLEO) /**< number of total gnss satellites */

// the following variable definition is related to the stack size, mgith cause stack dump when they are large
#define MAXNUM_STATION (NSTA) /**< maximum number of gnss ground stations */

#define MAXSIZE_OBS_EPOCH (86400*2) /**< max number of observations in the storage */

#define SIGMA_RANGE 0.3   /**< the standard deviation of range observations*/
#define SIGMA_PHASE 0.002 /**< the standard deviation of phase observations*/

#ifdef  EMBED
#define MAXOBS_PER_SAT 4    /**< max number of observation values for one gnss satellite */
#define MAXSAT_PER_EPOCH 10 /**< max number of satellites  for one gnss epoch */
#define MAXNUM_FREQ 2       /**< max number of frequency for one gnss satellite */
#else
#define MAXOBS_PER_SAT 10   /**< max number of observation values for one gnss satellite */
#define MAXSAT_PER_EPOCH 15 /**< max number of satellites  for one gnss epoch */
#define MAXNUM_FREQ 2       /**< max number of frequency for one gnss satellite */
#endif


    /**
     * @brief definition of all the possible satellite systems
     * @note
     * 2 power GPS = 2, it is 00000010 in binary
     * 2 power BDS = 4, it is 00000100 in binary
     * 2 power GAL = 8, it is 00001000 in binary
     */
    typedef enum _sysGNSS_
    {
        UKNSYS, /**< unknown satellite system*/
        GPS,    /**< GPS*/
        BDS,    /**< BeiDou system*/
        GAL,    /**< Galileo system*/
        GLO,    /**< Glonass system*/
        QZS,    /**< QZS system*/
        GEO,    /**< GEO navigation system*/
        LEO,    /**< LEO satellite system*/
        STA     /**< ground station*/
    } sysGNSS;

    /**
     * @brief definition of all the observation types
     * @note
     */
    typedef enum _OBSTYPE_
    {
        UKNTYPE, /**< unknown observation type*/
        RANGE,   /**<  pseudorange */
        PHASE,   /**< carrier phase*/
        DOPPLER, /**< Doppler*/
        SIGNAL   /**< signal strength*/
    } OBSTYPE;

    /**
     * @brief definition of all the observation frequency
     *
     */
    typedef enum _OBSFREQ_
    {
        UKNFREQ, /**< unknown frequency*/
        L1,      /**< L1 frequency, GPS L1, GAL E1, 1575.42 ;  BDS B1*/
        L2,      /**< L2 frequency, GPS L2, 1227.60*/
        L3,      /**< L3 frequency, */
        L4,      /**< L4 frequency, */
        L5,      /**< L5 frequency, GPS L5 (1176.45), GAL E5, */
        L6,      /**< L6 frequency, GAL E6*/
        L7,      /**< L7 frequency, GAL E5a*/
        L8,      /**< L8 frequency, GAL E5b*/
        L9,      /**< L9 frequency,*/
        C1,      /**< C1 frequency, C band1*/
        C2,      /**< C2 frequency, C band2*/
        C3,      /**< C3 frequency, C band3*/
        K1,      /**< K1 frequency, K band1*/
        K2,      /**< K2 frequency, K band2*/
        K3,      /**< K3 frequency, K band3*/
        K4,      /**< K4 frequency, K band4*/
        K5       /**< K5 frequency, K band5*/
    } OBSFREQ;

    /**
     * @brief observation code referred to Rinex 3.04 rules
     *
     */
    typedef enum _OBSCODE_
    {
        UKNCODE, /**< unknown obs code*/
        aA,      /**< obs code A*/
        aB,      /**< obs code B*/
        aC,      /**< obs code C*/
        aD,      /**< obs code D*/
        aE,      /**< obs code E*/
        aI,      /**< obs code I*/
        aL,      /**< obs code L*/
        aM,      /**< obs code M*/
        aN,      /**< obs code N*/
        aP,      /**< obs code P*/
        aQ,      /**< obs code Q*/
        aS,      /**< obs code S*/
        aW,      /**< obs code W*/
        aX,      /**< obs code X*/
        aY,      /**< obs code Y*/
        aZ       /**< obs code Z*/
    } OBSCODE;

    /**
     * @brief struct definition of observation type, freq and code
     */
    typedef struct _type_freq_code_
    {
        OBSTYPE type; /**< obs type*/
        OBSFREQ freq; /**< obs freq*/
        OBSCODE code; /**< obs code*/
    } TYPE_FREQ_CODE;

    /**
     * @brief struct definition of soluiton status
     */
    typedef enum _SOLUTION_STATUS_
    {
        SOL_OK,              /**< solution ok*/
        SOL_SPP,             /**< spp solution*/
        SOL_PPP,             /**< ppp solution*/
        SOL_DYN,             /**< dynamic solution*/
        SOL_SPP_ERROR,       /**< spp solution error*/
        SOL_SPP_LACK_SAT,    /**< spp solution lack of satellite(<4)*/
        SOL_ITER_EXCEED,     /**< solution iteration exceed*/
        SOL_OBS_BAD,         /**< solution with bad observation*/
        SOL_CHI2_TEST_FAILED /**< solution chi2 test failed*/
    } STATUS_SOL;

    /**
     * @brief struct definition of GNSSCONFIG
     */
    typedef struct _gnsssconfig_
    {

        sysGNSS sys_used[NSYS]; /**< all the gnss system used */
        int num_sys;            /**< number of gnss system used */

        int num_obs_freq_code[NSYS]; /**< num of obs for each sat in this sysGNSS */
        int num_freq[NSYS];          /**< number of frequency in the observation for each satellite system */
        // int opt_IF_combination;                                       /**< options for IF combination, 0 means no combination, 1 means IF combination*/
        double ele_cutoff;         /**< cutoff elevation in radian */
        double thres_slip_gf;      /**< the threshold of GF in meters for cycle slip detection, 0.05 meters by default */
        double thres_slip_mw;      /**< the threshold of MW in meters for cycle slip detection, 10 meters by default */
        double huber_param_spp[2]; /**< huber and huber_power for spp */

        double var_pos;           /**< init variance receiver position (m^2) */
        double var_vel;           /**< init variance of receiver vel ((m/s)^2) */
        double var_acc;           /**< init variance of receiver acc ((m/ss)^2) */
        double var_clk;           /**< init variance receiver clock (m^2) */
        double var_ztd;           /**< init variance ztd (m^2) */
        double var_trop_gradient; /**< init variance troposphere gradient (m^2) */
        double var_dcb;           /**< init variance dcb (m^2) */
        double var_bias;          /**< init variance phase-bias (m^2) */
        double var_iono;          /**< init variance iono-delay */
        double noise_amb;         /**< the noise of ambiguity in m^2 */

        TYPE_FREQ_CODE obs_type_freq_code_used[NSYS][MAXOBS_PER_SAT]; /**< obs frequency used for each satellite system*/
        int obs_index_IF_code[NSYS][MAXNUM_FREQ];                     /**< index of code in obs_type_freq_code_used for IF combination */
        int obs_index_IF_phase[NSYS][MAXNUM_FREQ];                    /**< index of phase in obs_type_freq_code_used for IF combination */
        int opt_IF[NSYS];                                             /**< the IF combination for each of the satellite system, 0: no IF combination, 1: IF combinaton*/
    } GNSSCONFIG;

    /**
     * @brief struct definition of SOLUTION
     */
    typedef struct _solution_
    {
        char type;                        /**< 'C' means spp, 'P' means PPP, 'R' means RTK, 'D' means Dynamic PPP */
        STATUS_SOL status;                /**< solution status from positioning */
        GTime time_epoch;                 /**< the epoch time for current solution  */
        double pos_vel_clk[6 + 2 * NSYS]; /**< 3 pos, 3 vel, NSYS receiver clock offset, NSYS receiver clock drift */
        double sigma;                     /**< posterior sigma */
        double gdop;                      /**< GDOP */
        int nsat;                         /**< the number of satellite used */
        char msg[256];                    /**< the message returned from positioning */
    } SOLUTION;

    void sysGNSSAsString(sysGNSS sys, char *str);
    void TYPE_FREQ_CODE_AsString(TYPE_FREQ_CODE tfc, char *str);
    void obscode_conversion_v2(char *obscode, OBSTYPE *t, OBSFREQ *b, OBSCODE *c);
    void obscode_conversion_v3(char *obscode, OBSTYPE *t, OBSFREQ *b, OBSCODE *c);

    sysGNSS string2satsysprn(char *str_satsysprn, int *prn);
    void satsysprn2string(sysGNSS satsys, int prn, char *prn_str);
    int satsysprn2index(sysGNSS sys, int prn);
    bool satindex2sysprn(int index, sysGNSS *sys, int *prn);

    double get_obs_frequency(sysGNSS satsys, OBSFREQ band, int prn);

    void set_gnssconfig_system(GNSSCONFIG *config, int nsys, sysGNSS *systems);

    int get_gnssconfig_sys_index(sysGNSS sys, GNSSCONFIG *config);

    extern GNSSCONFIG gnssconfig;

#ifndef EMBED
    void set_gnssconfig_from_file(char *configfile);
#endif

#ifdef __cplusplus
}
#endif

#endif