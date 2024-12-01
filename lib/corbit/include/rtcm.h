
/**
 * @file rtcm.h
 * @brief Declarations for decoding and encoding RTCM messages stream
 * @details
 * @author Dr. Zhen Li
 * @version 0.0.1
 * @date 2023.12.25
 * @note this file includes all the functions for rtcm decoding and encoding
 * @since 2016.01.15
 * @copyright
 * @par History:
 *
 */

#ifndef _RTCM_H_
#define _RTCM_H_

#include "corbit_const.h"
#include "gtime.h"
#include "gnss_config.h"
#include "common_function.h"
#include "gnss_observation.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define RTCM2PREAMB 0x66        /* rtcm ver.2 frame preamble */
#define RTCM3PREAMB 0xD3        /* rtcm ver.3 frame preamble */



/**<  DGPS/GNSS correction type */
typedef struct _dgps_t
{                
    GTime t0;         /* correction time */
    double prc;         /* pseudorange correction (PRC) (m) */
    double rrc;         /* range rate correction (RRC) (m/s) */
    int iod;            /* issue of data (IOD) */
    double udre;        /* UDRE */
} RTCM_DGPS;


/**< RTCM SSR correction type for one GNSS satellite */
typedef struct _rtcm_ssr_
{        
    GTime t0[5];      /* epoch time (GPST) {eph,clk,hrclk,ura,bias} */
    double udi[5];      /* SSR update interval (s) */
    int iod[5];         /* iod ssr {eph,clk,hrclk,ura,bias} */
    int iode;           /* issue of data */
    int ura;            /* URA indicator */
    int refd;           /* sat ref datum (0:ITRF,1:regional) */
    double deph [3];    /* delta orbit {radial,along,cross} (m) */
    double ddeph[3];    /* dot delta orbit {radial,along,cross} (m/s) */
    double dclk [3];    /* delta clock {c0,c1,c2} (m,m/s,m/s^2) */
    double hrclk;       /* high-rate clock corection (m) */
    float cbias[10]; /* code biases (m) MAXCODE=10 */
    int update; /* update flag (0:no update,1:update) */
} RTCM_SSR;


 /**<  RTCM control struct type */
typedef struct _rtcm_
{       
    int staid;          /* station id */
    int stah;           /* station health */
    int seqno;          /* sequence number for rtcm 2 or iods msm */
    int outtype;        /* output message type */
    GTime time;       /* message time */
    GTime time_s;     /* message start time */
    // obs_t obs;          /* observation data (uncorrected) */
    // nav_t nav;          /* satellite ephemerides */
    // sta_t sta;          /* station parameters */
    RTCM_DGPS *dgps;       /* output of dgps corrections */
    RTCM_SSR ssr[MAXSAT_GNSS];  /* output of ssr corrections */
    
    
    char msg[128];      /* special message */
    char msgtype[256];  /* last message type */
    char msmtype[6][128]; /* msm signal types */
    int obsflag;        /* obs data complete flag (1:ok,0:not complete) */
    int ephsat;         /* update satellite of ephemeris */
    double cp[MAXSAT_GNSS][MAXNUM_FREQ]; /* carrier-phase measurement */
    unsigned char lock[MAXSAT_GNSS][MAXNUM_FREQ]; /* lock time */
    unsigned char loss[MAXSAT_GNSS][MAXNUM_FREQ]; /* loss of lock count */
    GTime lltime[MAXSAT_GNSS][MAXNUM_FREQ]; /* last lock time */
    int nbyte;          /* number of bytes in message buffer */ 
    int nbit;           /* number of bits in word buffer */ 
    int len;            /* message length (bytes) */
    unsigned char buff[1200]; /* message buffer */
    unsigned int word;  /* word buffer for rtcm 2 */
    unsigned int nmsg2[100]; /* message count of RTCM 2 (1-99:1-99,0:other) */
    unsigned int nmsg3[300]; /* message count of RTCM 3 (1-299:1001-1299,0:ohter) */
    char opt[256];      /* RTCM dependent options */
} RTCM;


int init_RTCM(RTCM *rtcm);
int decode_rtcm3(RTCM *rtcm);
int input_rtcm3(RTCM *rtcm, unsigned char data);
int run_decode_rtcm(GTime obstime, RTCM *rtcm, unsigned char* byte_steam, int size_byte_steam);

#ifndef EMBED
int input_rtcm3f(RTCM *rtcm, FILE *fp);
#endif


extern RTCM *g_rtcm_storage; // RTCM storage

#ifdef __cplusplus
}
#endif

#endif