
/**
 * @file gnss_satposition.h
 * @brief Declarations for gnss satellite position computation
 * @details
 * @author Dr. Zhen Li
 * @version 0.0.1
 * @date 2023.12.25
 * @note this file includes all the functions for gnss satellite position computation
 * @since 2016.01.15
 * @copyright
 * @par History:
 *
 */

#ifndef _GNSS_SATPOSITION_
#define _GNSS_SATPOSITION_

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef EMBED
#include <stdio.h>
#define MAXEPOCH_EPH_PRE 86401
#define MAXEPOCH_CLK_PRE 86401
#define MAXEPOCH_EPH_BRDC 200

#else
#define MAXEPOCH_EPH_PRE 10
#define MAXEPOCH_CLK_PRE 30
#define MAXEPOCH_EPH_BRDC 2

#endif

#include "gtime.h"
#include "gnss_config.h"
#include "common_function.h"
#include "rtcm.h"

    /// the ionosphere model parameters in broadcast ephemeris
    extern double iono_brdc_GPS_A[4];
    extern double iono_brdc_GPS_B[4];
    extern double iono_brdc_BDS_A[4];
    extern double iono_brdc_BDS_B[4];
    extern double iono_brdc_GAL_I[3];
    extern double iono_brdc_QZS_A[4];
    extern double iono_brdc_QZS_B[4];
    extern double iono_brdc_BDGIM[9];  /**< 9 param BDGIM ionosphere model */ 
    
    /** @brief EPHOPT definitions */
    typedef enum _EPHOPT_
    {
        EPHOPT_BRDC, /**< broadcast ephemeris option */
        EPHOPT_PREC, /**< precise ephemeris option */
        EPHOPT_RTCM /**< broadcast ephemeris with RTCM SSR option */
    } EPHOPT;
    
    /// @cond IGNORE
    /** @brief  tag the status of the current ephemeris status */ 
    typedef enum _EPHSTATE_
    {
        EPHSTATE_CONTINUOUS  = 0x0,  /**< continuous ephemeris for both brdc and rtcm ssr */
        EPHSTATE_BRDC_CHANGE = 0x1, /**< brdc ephemeris changes at 0, 2 4, hours */
        EPHSTATE_SSR_MISSING = 0x2      /**<  RTCM SSR Discontinuous */
    } EPHSTATE;
    /// @endcond  
    
    /**
     * @brief GPS broadcast ephemeris record , sat position at the phase center 
     */
    typedef struct _EPHREC_GPS_
    {
        int prn;                    /**<  satellite prn num_type_of_observ  */
        TimeYMD TOC;                /**< Time of Clock in GPS time */
        double sv_clock_bias;        /**< clock bias in seconds */
        double sv_clock_drift;        /**< clock drift in second/sec */
        double sv_clock_drift_rate; /**< clock drift rate sec/sec^2 */

        double IODE;    /**< Issue of Data, Ephemeris */
        double Crs;        /**< in meters */
        double Delta_n; /**< in radians/sec */
        double M0;        /**< in radians */

        double Cuc;       /**< in radians */
        double e;       /**< orbit eccentricity, no unit */
        double Cus;       /**< radians */
        double sqrt_A;  /**< sqrt of semi-major axis in sqrt(m) */

        double TOE;       /**< Time of Ephemeris, sec of GPS week */
        double Cic;       /**< in radians */
        double OMEGA0; /**< in radians */
        double Cis;       /**< in radians */

        double i0;          /**< orbit inclination in radians */
        double Crc;          /**< in meters */
        double omega;      /**< in radians */
        double OMEGA_dot; /**< in radians/sec */

        double i_dot; /**< rate of orbital inclination in radians/sec */
        bool L2_code; /**< Codes on L2 channel */
        int Week;      /**< GPS week number, to go with TOE, continuous number, not mod(1024)! */
        bool L2_P;      /**< L2 P data flag */

        double sv_accuracy; /**< satellite accuracy in meters */
        int sv_health;        /**< satellite health */
        double TGD;            /**< L1-L2 in seconds, Time of Group Delay, see ICD */
        double IODC;        /**< Issue of Data, Clock */

        double transmission_msg_time; /**< transmission time of message, in sec of GPS week, Adjust the Transmission time of message by -604800 to refer to the reported week, if necessary */
        double Fit_interval;          /**< in hours, brdc is fitted from precise orbit */
        bool ok;
    } EPHREC_GPS;

    /// BDS broadcast ephemeris record, sat position at the phase center
    typedef struct _EPHREC_BDS_
    {
        int prn;            /**< satellite prn  */
        int sat_type;       /**<  00: default, 01: GEO, 02:IGSO, 03: MEO */
        TimeYMD TOC;                /**< Time of Clock in BDS time */
        double sv_clock_bias;        /**< clock bias in seconds */
        double sv_clock_drift;        /**< clock drift in second/sec */
        double sv_clock_drift_rate; /**< clock drift rate sec/sec^2 */

        double AODE;    /**< Age of Data, Ephemeris, see ICD */
        double Crs;        /**< in meters */
        double Delta_n; /**< in radians/sec */
        double Delta_n_dot; /**< in unit of pi/s^2 */
        double M0; /**< in radians */
        
        double Cuc;       /**< in radians */
        double e;       /**< orbit eccentricity, no unit */
        double Cus;       /**< radians */
        double sqrt_A; /**< sqrt of semi-major axis in sqrt(m), this is not used for BDS3 */
        double delta_A;  /**< ICD table 7-9, delta A to the reference sma, Aref=27906100 for MEO, Aref= 42162200 for IGSO/GEO */
        double A_dot;  /**< ICD table 7-9 */

        double TOE;       /**< Time of Ephemeris, sec of BDS week */
        double Cic;       /**< in radians */
        double OMEGA0; /**< in radians */
        double Cis;       /**< in radians */

        double i0;          /**< orbit inclination in radians  */
        double Crc;          /**< in meters  */
        double omega;      /**< in radians  */
        double OMEGA_dot; /**< in radians/sec */

        double i_dot; /**< rate of orbital inclination in radians/sec */
        // double spare;
        int Week; /**< BDS week number,It started at zero at 1-Jan-2006, Hence BDT week = BDT week_BRD + (n*8192) where (n: number of BDT roll-overs).  */
        // bool spare; /**< spared data bit  */

        double sv_accuracy; /**< (meters)  */
        int sv_health;        /**< satellite health  */
        double TGD_B1_B3;    /**< in Seconds referenced to B3I, For BDS3, TGD_B1Cp, pilot channel  */
        double TGD_B2_B3;    /**< in seconds referenced to B3I, For BDS3, TGD_B2ap, pilot channel  */
        double ISC_B1Cd;    /**< BDS3, B1C data channel, inter-frequency-difference in TGD  */

        double transmission_msg_time; /**< transmission time of message, in sec of BDS week, Adjust the Transmission time of message by + or -604800 to refer to the reported week in BROADCAST ORBIT 5, if necessary. Set value to 0.9999E9 if not known.  */
        double AODC;                  /**< Age of Data, Clock, see ICD  */
        bool ok;  /**< ok or not */
    } EPHREC_BDS;

    /// GAL broadcast ephemeris record, sat position at the phase center
    typedef struct _EPHREC_GAL_
    {
        int prn;            /// satellite prn
        TimeYMD TOC;                /// Time of Clock in GAL time
        double sv_clock_bias;        /// clock bias in seconds
        double sv_clock_drift;        /// clock drift in second/sec
        double sv_clock_drift_rate; /// clock drift rate sec/sec^2

        double IODnav;    /// Issue of Data of the nav batch, IODE
        double Crs;        /// in meters
        double Delta_n; /// in radians/sec
        double M0;        /// in radians

        double Cuc;       /// in radians
        double e;       /// orbit eccentricity, no unit
        double Cus;       /// radians
        double sqrt_A; // sqrt of semi-major axis in sqrt(m)

        double TOE;       /// Time of Ephemeris, sec of GAL week
        double Cic;       /// in radians
        double OMEGA0; /// in radians
        double Cis;       /// in radians

        double i0;          /// orbit inclination in radians
        double Crc;          /// in meters
        double omega;      /// in radians
        double OMEGA_dot; /// in radians/sec

        double i_dot;      /// rate of orbital inclination in radians/sec
        int Data_sources; /// FLOAT-->INTEGER, bit 0,I/NAV E1-B; bit 1, F/NAV E5a-I; bit 2, I/NAV E5b-I
        int Week;          /// GAL week number, to go with TOE, continuous number, not mod(1024)! GAL week = GST week + 1024 + n*4096 (n: number of GST roll-overs).
        // bool spare; /// spared data bit

        double SISA;       /// Signal in Space Accuracy (meters)
        int sv_health;       /// satellite health
        double BGD_E5a_E1; /// Broadcast Group Delay in Seconds based on E5a and E1 Ionosphere Free combination, E1 -  IF(E5a_E1)
        double BGD_E5b_E1; /// Broadcast Group Delay in Seconds based on E5b and E1 Ionosphere Free combination, E1 -  IF(E5b_E1)

        double transmission_msg_time; /// data frame transmission time of message, in sec of GAL week, Adjust the Transmission time of message by + or -604800 to refer to the reported week in BROADCAST ORBIT 5, if necessary. Set value to 0.9999E9 if not known.
        bool ok;
    } EPHREC_GAL;

    /// GAL broadcast ephemeris record, sat position at the phase center
    typedef struct _EPHREC_GLO_
    {
        int SlotNum;      /**<  Slot number in sat. constellation */
        TimeGNSS RefTime; /**<  Epoch of ephemerides (UTC) */
        double ClockBias; /**<  SV clock bias (sec) (-TauN)  */
        double FreqBias;  /**< SV relative frequency bias (+GammaN) */
        double Tk;          /**< message frame time (0 <= Tk < 86400 sec of day UTC)*/
        double Pos[3];      /**<  Satellite position [m]  */
        double Vel[3];      /**<  Satellite  velocity[m/s] */
        double Acc[3];      /**<  Satellite  velocity[m/s^2] */
        int Health;          /**<  health (0=OK) */
        double FreqNum;      /**<  frequency number (1-24) */
        double InforAge;  /**<  Age of oper. information (days) */
    } EPHREC_GLO;

    /// the precise ephemeris for GNSS, mainly sp3, sat positions at the center of mass
    typedef struct _EPHREC_PRE_
    {
        GTime time;  /**< epoch time  */
        sysGNSS satsys; /**< sat system */
        int prn;  /**< satellite prn */
        double pos_ecf[3]; /**< sat position in ecef, in meter */
        double vel_ecf[3]; /**< sat velocity in ecef, in m/s */
        double sigma[6]; /**< sigma of position and velocity */
        bool ok; /** record ok or not */
        // int tle_idx; /**< index of the tle in the tle database */
    } EPHREC_PRE;

    /// the precise clock offset for GNSS, mainly sp3
    typedef struct _CLKREC_PRE_
    {
        GTime time; /**< epoch time  */
        sysGNSS satsys; /**< sat system */
        int prn; /**< satellite prn */
        double clk_offset; /**< clock offset in unit of sec */
        double clk_drift;  /**< clock offset in unit of sec/sec */
        bool ok; /**< record ok or not */
    } CLKREC_PRE;

    // /// RTCM orbit correction
    // typedef struct _RTCM_ORBIT_
    // {
    //     GTime time_epoch;        /**<  the reference time of the current RTCM msg */
    //     sysGNSS satsys; /**< satellite system */
    //     int prn; /**< satellite prn */
    //     double IODE;            /**<  the IODE of this RTCM msg, has to be be matchched with the brdc */
    //     double pos_RTN[3];        /**<  orbit correction based on brdc in the RTN axes */
    //     double pos_rate_RTN[3]; /**<  the rate of the orbit correction based on brdc in the RTN axes */
    // } RTCM_ORBIT_REC;

    // /// RTCM clock correction
    // typedef struct _RTCM_CLOCK_
    // {
    //     GTime time_epoch;  // the reference time of the current RTCM msg
    //     sysGNSS satsys;
    //     int prn;
    //     double IODC;       // the IODE of this RTCM msg, has to be be matchched with the brdc
    //     double clk_offset; // orbit correction based on brdc in the RTN axes
    //     double clk_drift;  // the rate of the orbit correction based on brdc in the RTN axes
    // } RTCM_CLOCK_REC;

    /// PRE_EPH are the position of center of gravity
    typedef struct _EPHPREC_STORE_
    {
        bool update;
        EPHREC_PRE eph_pre_all[MAXSAT_GNSS+1][MAXEPOCH_EPH_PRE];
        CLKREC_PRE clk_pre_all[MAXSAT_GNSS+1][MAXEPOCH_CLK_PRE];
        int idx_eph[MAXSAT_GNSS+1];
        int idx_clk[MAXSAT_GNSS+1];
    } EPHPREC_STORE;

    /**
     * @brief EPHBRDC_STORE is a struct to store all the brdc ephemersi records. Usually, this is related to the rinex navigation file.
     */
    typedef struct _EPHBRDC_STORE_
    {
        EPHREC_GPS eph_brdc_gps_all[MAXPRNGPS + 1][MAXEPOCH_EPH_BRDC]; /**< GPS brdc ephemris records */
        EPHREC_BDS eph_brdc_bds_all[MAXPRNBDS + 1][MAXEPOCH_EPH_BRDC]; /**< BDS brdc ephemris records */
        EPHREC_GAL eph_brdc_gal_all[MAXPRNGAL + 1][MAXEPOCH_EPH_BRDC]; /**< GAL brdc ephemris records */
        EPHREC_GLO eph_brdc_glo_all[MAXPRNGLO + 1][MAXEPOCH_EPH_BRDC]; /**< GLO brdc ephemris records */
        int update_index; // to indicate which of the index in MAXEPOCH_EPH_BRDC has been updated!
    } EPHBRDC_STORE;

    /**
     * @brief struct EPHBRDC_NOW is a temperay variable to store the choosed brdc pointers for current time epoch.
     * @note all the members are pointers to EPHBRDC_STORE, therefore, there is no need to allocate memory for pointers in EPHBRDC_NOW. The memory for a pointer is 8 bytes
     */
    typedef struct _EPHBRDC_NOW_
    {
        EPHREC_GPS* eph_brdc_gps_now[MAXPRNGPS + 1]; /**< pointers for broadcast ephemeris for GPS satellites */ 
        EPHREC_BDS* eph_brdc_bds_now[MAXPRNBDS + 1]; /**< pointers for broadcast ephemeris for BDS satellites */
        EPHREC_GAL* eph_brdc_gal_now[MAXPRNGAL + 1]; /**< pointers for broadcast ephemeris for GAL satellites */
        EPHREC_GLO* eph_brdc_glo_now[MAXPRNGLO + 1]; /**< pointers for broadcast ephemeris for GLO satellites */

        // those are state for the current broadcast ephemeris, use only 1 byte (8 bits) to store the states
        //  <high>xxxx-xxxx<low>, low 4 bits to store the state, the 5th bit to store if it is the first call
        char eph_brdc_gps_state_now[MAXPRNGPS + 1]; /**< state for broadcast ephemeris for GPS satellites */
        char eph_brdc_bds_state_now[MAXPRNBDS + 1]; /**< state for broadcast ephemeris for BDS satellites */
        char eph_brdc_gal_state_now[MAXPRNGAL + 1]; /**< state for broadcast ephemeris for GAL satellites */
        char eph_brdc_glo_state_now[MAXPRNGLO + 1]; /**< state for broadcast ephemeris for GLO satellites */

    } EPHBRDC_NOW;

    /**
     *@brief Antenna information for both the GNSS satellite and the receiver antenna
     *@note  for gnss satellite antenna:   ant_code is G01, E01,...  ant_type is the block type of the gnss satellite
     *       for ground receiver antenna:  ant_code are all spaces,  ant_type is the type of the ground antenna (might contain empty spaces), 
     *          like "SEPVC6150L      SCIS".
     * idx_object is the satindex for gnss satellites and idx_object=-1 for ground receivers
     * installation matrix is from antenna frame to the platform frame
     */
    typedef struct _ANT_INFO_
    {
        bool ant_ok;
        int idx_object;  /**<  the satindex or object index */
        char ant_type[21];     /**< Serial number */
        char ant_code[21];     /**< all the receiver antenna has Blank ant_code, GNSS satellites has G01... as the ant_code */
        double mjd_available[2];    /**<  available start time in mjd */
        int freq_num;  /**< number of frequency*/
        double pco[MAXNUM_FREQ][3]; /**<  phase center of pco for every frequency, xyz or ENU */
        OBSFREQ freqs[MAXNUM_FREQ]; /**<  the frequency info for the pco and pcv */
        sysGNSS satsys[MAXNUM_FREQ]; /**< satellite systems */
        // double pcv[360][90];        /**<  only include average pcv for all frequency */
        
        /* the installation matrix for this antenna, i.e. the transformation matrix from antenna frame to plantform frame
        // installation_matrix should be identical when ground station antenna is aligned to ENU
        // for LEO satellite, should get installation matrix from manufacturer, typically, in body fixed frame, antena frame has negaive z axis w.r.t bfs.
        
        // i.e.  x_platform = installation_matrix * x_antenna
           ex[0], ex[1], ex[2]
           ey[0], ey[1], ey[2]
           ez[0], ez[1], ez[2]
        */
        double installation_matrix[9];  /**<  could be from antenna frame to ENU, or antenna frame to spacecraft body fixed frame */

    } ANT_INFO;


    /// struct ant_info_store
    typedef struct _ANT_INFO_STORE_
    {
        ANT_INFO* antenna_info_storage; /**<  the array of antenna info */
        int num_antenna_info; /**< the current size of antenna info array */
        int num_max_memory;  /**< the size of the antenna_info_storage memory */
    } ANT_INFO_STORE;

    void initialize_global_gnss_satposition();
    void reset_eph_brdc_now_state();
    void antenna_GNSS_default();
    bool add_antenna_into_storage(ANT_INFO *ant_info, ANT_INFO_STORE *ptr_antenna_info_store);
    ANT_INFO* search_ant_info(int sat_rcv_index, char *ant_type_code, double mjd_now);
    
    bool compute_satposvel(GTime transmission_time, int satindex, double *satposvel, double *satclk, double *tgd);
    double get_tgd(sysGNSS satsys, int prn, OBSFREQ band, OBSCODE code);
    
    int find_closest_eph_prec(EPHREC_PRE *eph_all, int nsize, GTime time_target);

    bool choose_eph_gps_brdc(GTime transmission_time, int satindex);
    bool choose_eph_bds_brdc(GTime transmission_time, int satindex);
    // int  get_state_brdc_eph_now(int satindex);
    // int  set_state_brdc_eph_now(int satindex);

    double compute_satposvelclk_gps_brdc(int prn, GTime transmission_time, double satposvel[], double satclk[]);
    double compute_satposvelclk_bds_brdc(int prn, GTime transmission_time, double satposvel[], double satclk[]);
    double compute_satposvelclk_bds_brdc_v3(int prn, GTime transmission_time, double satposvel[], double satclk[]);
    double compute_satposvelclk_gal_brdc(int prn, GTime transmission_time, double satposvel[], double satclk[]);

    bool compute_satposvelclk_ssr(GTime transmission_time, int satindex, RTCM_SSR *rtcm_ssr, double *deph, double *dclk);
    bool compute_satposvel_prec(GTime transmission_time, int satindex, double satposvel[]);
    bool compute_satclk_prec(GTime transmission_time, int satindex, double satclk[]);
    double get_ant_pco_correction(ANT_INFO *antinfo, double mjd_now, OBSFREQ freq, double *ex_platform, double *ey_platform, double *ez_platform, double *signal_los, double *offset_in_platform);
    
#ifndef EMBED
    bool load_ephemeris_brdc(char *brdcfilename);
    bool load_ephemris_prec(char *precfilename);
    bool load_clock_prec(char *clkfilename, EPHPREC_STORE *eph_prec);

    int cmp_ephrec_pre(const void *a, const void *b);
    int cmp_clkrec_pre(const void *a, const void *b);
    void sort_ephprec_storage(EPHPREC_STORE *ephprec_store);
    void remove_duplicate_ephprec_storage(EPHPREC_STORE *ephprec_store);

    bool load_antenna_storage(char *antex_file, ANT_INFO_STORE *ant_info_store);
    bool read_atx(char *atx_filename, ANT_INFO_STORE *ptr_antenna_info_store);

#endif

    extern EPHOPT g_eph_selection; // eph_selection: 0 for brdc, 1 for precise sp3
    extern EPHPREC_STORE *g_prec_storage;
    extern EPHBRDC_STORE *g_brdc_storage;
    extern EPHBRDC_NOW g_eph_brdc_now;
    extern ANT_INFO_STORE* g_antenna_info_store;
    
#ifdef __cplusplus
}
#endif

#endif