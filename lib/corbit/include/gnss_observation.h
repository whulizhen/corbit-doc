
/**
 * @file gnss_observation.h
 * @brief Declarations for gnss observation operations
 * @details
 * @author Dr. Zhen Li
 * @version 0.0.1
 * @date 2023.12.25
 * @note this file includes all the functions for gnss observation operations
 * @since 2016.01.15
 * @copyright
 * @par History:
 *
 */

#ifndef _GNSS_OBSERVATION_H_
#define _GNSS_OBSERVATION_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdio.h>
#include <stdlib.h>

#include <math.h>
#include <stdbool.h>
#include "corbit_const.h"
#include "gtime.h"
#include "gnss_config.h"
#include "common_function.h"
#include "attitude.h"

    /// define the exceptions in observation
    typedef enum _OBS_EXCEPTIONS_
    {
        OBS_OK,  /**< the observation is OK */
        LOW_ELE, /**< low elevation for this observation */
        SAT_UKN, /**< unknown satellite */
        SAT_NOT_USED, /**< this satellite is not used */
        SATPOS_ERROR, /**< the satellite position is wrong */
        // CYCLE_SLIP, /**< cycle slip found */
        CLOCK_JUMP, /**< a clock jump found */
        GROSS_ERROR, /**< gross error for this obs */
        OBS_UKN /**< unknown observation */
    } OBS_EXCEPTION;

    /// record of GNSS obs, memory: 48 Bytes
    typedef struct _OBSREC_
    {
        int flag_obs;     /**< record good or bad, 0 means good, 1,2,3... means different types of observation exceptions */ 
        OBSFREQ freq;     /**< frequency indicator, 4 bytes */ 
        OBSTYPE type;     /**< observation type, range, phase, doppler, */ 
        OBSCODE code;     /**< obs code, C, P, W, I, Q et.al */ 
        int LLI;         /**< LLI: signal lost lock indicator, cycle slip detection */ 
        double value;     /**< obs value */ 
        double sigma;     /**< the sigma for this observation value */ 
        double residual; /**< the residual for this observation */ 
    } OBSREC;

    /// observation of one satellite, memory: 632 byes
    typedef struct _OBSDATA_SAT_
    {
        int prn;        /**< satellite prn, 4 bytes */
        sysGNSS satsys; /**< satellite system 4 byes */
        int num_obs;    /**< total num of observation from rinex file or gnss chip, 4 byes */
        int flag_sat;    /**< flag of current sat , 4 byes */

        int obs_index_used[MAXOBS_PER_SAT]; /**< the index in obsdata that is ued in data processing, this is determined based on gnssconfig, -1 means not available, 40 byes */
        int num_obs_used;                    /**< the number of obs used in data processing, based on definition in gnssconfig */
        double satpos_ecf[3];                /**< sat position [the center of graivty] in ecf, without pco, as pco is related to signal frequency  */
        double satvel_ecf[3];                /**< sat velocity in ecf  */
        double satclk_offset;                /**< sat clock offset in sec */
        double satclk_drift;                /**< sat clock drift in s/s */
        double satclk_drift_rate;            /**<  sat clock drift rate in s/s^2 */
        double sat_azel[2];                    /**< sat azimuth and elevation for this epoch observation, these angles are in the antenna frame */
        OBSREC obsdata[MAXOBS_PER_SAT];        /**< memory: 480 bytes */
    } OBSDATA_SAT;

    /// observation of one epoch with many satellites
    typedef struct _OBSDATA_EPOCH_
    {
        GTime epoch_time;  /**< epoch time of the observation epoch */
        int rcv;          /**<  the rcv indicator */
        int flag_epoch; /**< 0 means ok, 1,2,3,4, means different types of exception */
        int sat_num; /**< the total number of satellite in this epoch */
        OBSDATA_SAT satobs[MAXSAT_PER_EPOCH]; /**< all the observations for each of the satellite */
    } OBSDATA_EPOCH;

    void initialize_obsdata_epoch(OBSDATA_EPOCH *obsdata_eopch);

    bool get_obs_record(int sat_idx_epoch, int obs_idx_obsdata, OBSDATA_EPOCH *obsepoch, OBSREC *obsrec);

    void choose_observation(OBSDATA_EPOCH *obsepoch);
    int choose_satellite(OBSDATA_EPOCH *obsepoch, int *satlist_in_obsepoch, int *satlist_satindex);

    // MP combination
    void get_multipath_combination(double phase[2], double range[2], double freq[2], double MP[2]);
    void get_ionosphere_free_combination(double *obs_phase, double *obs_range, double *freq_value, double *IF_combination, double *factor);

#ifndef EMBED
    int resolve_rnx_header_v20(FILE *pf, OBSFREQ *obs_freq_header, OBSTYPE *obs_type_header, OBSCODE *obs_code_header);
    int resolve_rnx_record_v20(FILE *pf, long epoch_index, int rcvno, int num_obs_code, OBSFREQ *obs_freq_header, OBSTYPE *obs_type_header, OBSCODE *obs_code_header, double sample_rate);

    int resolve_rnx_header_v30(FILE *pf, sysGNSS sysall[NSYS], int numobs[NSYS], TYPE_FREQ_CODE obs_type_freq_code[NSYS][MAXOBS_PER_SAT]);
    int resolve_rnx_record_v30(FILE *pf, long epoch_index, int rcvno, int nsys, sysGNSS sysall[NSYS], int nobs[NSYS], TYPE_FREQ_CODE obs_type_freq_code[NSYS][MAXOBS_PER_SAT], double sample_rate);
    bool get_rnx_line_v30(char *line_str, int ksys, OBSDATA_SAT *satobs, int numobs[NSYS], TYPE_FREQ_CODE obs_type_freq_code[NSYS][MAXOBS_PER_SAT]);
    
    bool load_observation_rinex(char *filename_rinex, int rcvno, double sample_rate);

    void generate_rinex_header(double version, int rcvno, int num_obs_storage, OBSDATA_EPOCH *obs_storage, char *header_txt);
    bool write_rinex_observation(char *filename_rinex, int rcvno, double version, int num_obs_storage, OBSDATA_EPOCH *obs_storage);

    int cmp_sort_obsdata_epoch(const void *a, const void *b);
    void sort_observation_storage(OBSDATA_EPOCH *obs_list, size_t nsize);
    int remove_duplicate_gnss_observation(int num_epoch, OBSDATA_EPOCH *obs_storage);
    int match_observation_storage(OBSDATA_EPOCH *obs_list, size_t nsize, GTime time_target, double threshold_sec);
#endif

    OBSDATA_EPOCH *get_obs_epoch(int index, int rcvno);

    // int generateDopplerObs(OBSDATA_EPOCH obsEpoch[3]);

    // global variable
    extern OBSDATA_EPOCH *g_obs_storage;
    extern int g_num_obs_storage;

#ifdef __cplusplus
}
#endif

#endif