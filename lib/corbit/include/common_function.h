

/**
 * @file common_function.h
 * @brief Declarations of common functions
 * @details
 * @author Dr. Zhen Li
 * @version 0.0.1
 * @date 2023.12.25
 * @note this file includes all the common functions
 * @since 2016.01.15
 * @copyright
 * @par History:
 *
 */

#ifndef _COMMON_FUNCTION_H_
#define _COMMON_FUNCTION_H_

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef EMBED
    #include <ctype.h>
    #include <stdlib.h>
    #include <math.h>
#endif

#include <stdio.h>
#include <string.h>
#include "corbit_const.h"
#include "mathlib.h"
#include "gtime.h"

#define POLYCRC32   0xEDB88320u /* CRC32 polynomial */
#define POLYCRC24Q  0x1864CFBu  /* CRC24Q polynomial */


#define MAXLEN_STRING 1024



#ifndef EMBED
    void print_help(char *help_str[]);
    void hex_str_to_byte(char *in, int len, unsigned char *out);
#endif

    unsigned int getbitu(const unsigned char *buff, int pos, int len);
    int getbits(const unsigned char *buff, int pos, int len);
    void setbitu(unsigned char *buff, int pos, int len, unsigned int data);
    void setbits(unsigned char *buff, int pos, int len, int data);
    unsigned int crc32(const unsigned char *buff, int len);
    unsigned int crc24q(const unsigned char *buff, int len);
    unsigned short crc16(const unsigned char *buff, int len);

    int string_split(const char *string_raw, char *seps, char outstr[][MAXLEN_STRING]);
    void string_trim(char *s);
    
    
    void my_console_logger(ulog_level_t severity, char *msg);
    void output_port(int level, char *msg);

    double search_max_array(double *arr, int left, int right);
    int binsearch_array(double *A, int s, int e, double value);

    // detect outlier
    void detect_outlier(const int num, double *dataset, int *states, double threshold_max);

    void get_azel_signal(double *installation_matrix, double *signal_los_platform, double *azel);
    void azel(double *blh, double *dxyz, double *azel);
    void enu2xyz(double *blh, double *enu, double *dxyz);
    void xyz2enu(double *blh, double *dxyz, double *enu);
    void xyz2enu_matrix(double *blh, double *E);
    void eci2rtn(double *satpos, double *satvel, double *R, double *T, double *N);

    void blh2ecef(double *blh, double *ecef, double re_wgs84, double fe_wgs84);
    void ecef2blh(double *ecef, double *blh, double re_wgs84, double fe_wgs84);
    
#ifdef __cplusplus
}
#endif

#endif // header guard
