
/**
 * @file gtime.h
 * @brief Declarations of time operations functions
 * @details
 * @author Dr. Zhen Li
 * @version 0.0.1
 * @date 2023.12.25
 * @note this file includes all the functions for time operations.
 * @since 2016.01.15
 * @copyright
 * @par History:
 *
 */

#ifndef _GTIME_H_
#define _GTIME_H_

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef EMBED
#include <time.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include "corbit_const.h"
// #include "common_function.h"

    /**
     * @brief enum definition for time systems
     */
    typedef enum _TIMESYS_
    {
        UKNTS,    /**< unknown time system*/
        tsGPST, /**< GPS time system*/
        tsGALT, /**< GAlileo time system*/
        tsBDST, /**< BeiDou time system*/
        tsUTC,    /**< UTC time system*/
        tsTAI,    /**< TAI time system*/
        tsTT    /**< TT time system*/
    } TIMESYS;

/**< macro for variable to string */
//#define Variable2String(val) #val


#define HOUR_PER_DAY (24.0)   /**< macro hours per day */
#define MINUTE_PER_HOUR (60.0) /**< macro minutes per hour*/
#define SEC_PER_MINUTE (60.0) /**< macro seconds per minute*/

#define SEC_PER_DAY (HOUR_PER_DAY * MINUTE_PER_HOUR * SEC_PER_MINUTE)   /**< macro seconds per day*/

#define DAY_PER_WEEK (7)  /**< macro days per week */

#define TTmTAI 32.184  /**< TT  minus TAI*/
#define TAImGPST 19.0  /**< TAI minus GPST*/
#define GPSTmBDST 14.0 /**< GPST minus BDST*/
#define GPSTmGALT 0.0  /**< GPST minus Galileo system time*/

#define GPST0 44244.0                /**<  6_Jan_1980, 1980,1,6,0,0,0 UTC */
#define BDST0 53736.0                /**< BDST0 */
#define GALT0 51411.999849537037037 /**< 1999,8,21,23,59,47, UTC */
#define J2000 51544.5                /**< J2000 epoch, 1_jan_2000_midday, UTC */
#define MJD0 2400000.5                /**< julian date of the MJD definition */
#define MJD_1_jan_1977 43144  /**< mjd of 1977 1 1 */
#define JAN11901 15385 /**< mjd of 1901 1 1  */

#define threshold_time 1.0E-10 /**< threshold  in seconds for time comparison */

    /**
     * @brief struct time format in year, month, day
     */
    typedef struct _TimeYMD_
    {
        int year;       /**< year*/
        int month;       /**< month*/
        int day;       /**< day*/
        int hour;       /**< hour*/
        int minute;       /**<minute*/
        double second; /**< second*/
    } TimeYMD;

    /**
     * @brief struct time format in year and day of year
     */
    typedef struct _TimeDOY_
    {
        int year;    /**< year*/
        double doy; /**< day of year*/
    } TimeDOY;

    /**
     * @brief struct for time format in weeks and seconds of week
     * @note used for GPST, BDST, GALT and others
     */
    typedef struct _TimeGNSS_
    {
        int week;
        double sow;
    } TimeGNSS;

    /**
     * @brief struct time format in Modified Julian Day and second of day
     */
    typedef struct _TimeMJD_
    {
        long mjd;    /**<MJD day*/
        double sod; /**< seconds of day*/
    } TimeMJD;

    /**
     * @brief struct time format in Julian day and second of day
     */
    typedef struct _TimeJD_
    {
        long jd;    /**< Julian day*/
        double sod; /**< seconds of day */
    } TimeJD;

    /**
     * @brief struct for general time computation with time system defined
     * @note this struct should be used everywhere necessary
     * memory: 24 byes
     */
    typedef struct _GTime_
    {
        TimeMJD time; /**< mjd time*/
        TIMESYS sys;  /**< time system*/
    } GTime;

    
    // time related functions
    void timeMjd_negative(TimeMJD *mjdtime);
    void TimeJD_normalise(TimeJD *jdtime);
    void TimeMjd_normalise(TimeMJD *mjdtime);
    TimeMJD mjd2TimeMJD(double mjd);
    TimeMJD timeMjdAddSec(TimeMJD mjdtime, double seconds);
    TimeMJD timeMjdAdd(TimeMJD mjdtime1, TimeMJD mjdtime2);
    TimeMJD timeMjdSub(TimeMJD mjdtime1, TimeMJD mjdtime2);

    GTime gtimeAddSec(GTime gtime, double seconds);
    GTime gtimeAdd(GTime gtime1, GTime gtime2);
    GTime gtimeSub(GTime gtime1, GTime gtime2);
    double gtimeSubSec(GTime gtime1, GTime gtime2);

    int getLeapSecond(double mjd_utc);
    double TAImUTC(GTime utct);
    GTime TAI2UTC(GTime gtime_tai);
    TimeJD YMDtoJD(TimeYMD ct);
    TimeJD GTime2JD(GTime gt);
    GTime JD2GTime(TIMESYS ts, TimeJD jdt);
    GTime mjd2GTime(TIMESYS ts, double mjd);
    TimeYMD JD2YMD(TimeJD jdt);

    TimeYMD DOY2YMD(TimeDOY doytime);
    TimeDOY YMD2DOY(TimeYMD ymdtime);
    TimeGNSS GTime2SOW(GTime gtime);
    GTime SOW2GTime(TimeGNSS timesow, TIMESYS ts);
    GTime gtimeChangeTimeSystem(GTime gtime, TIMESYS ts);
    GTime YMD2GTime(TimeYMD ymdtime, TIMESYS ts);
    TimeYMD GTime2YMD(GTime gt);
    void gtime2str(GTime gtime, char *time_str);
    double gtime2mjd(GTime gtime);
    GTime str2gtime(char *time_str, TIMESYS ts);
    GTime str2gtime_format(char *time_str, char *time_format, TIMESYS ts);
    void timeymd2str(TimeYMD ymdtime, char *time_str);
    bool gtime_greater(GTime a, GTime b);
    bool gtime_less(GTime a, GTime b);
    bool gtime_equal(GTime a, GTime b);

#ifndef EMBED
    GTime get_time_now();
#endif



#ifdef __cplusplus
}
#endif

#endif