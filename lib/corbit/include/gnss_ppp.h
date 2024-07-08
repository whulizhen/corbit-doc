
/**
 * @file gnss_ppp.h
 * @brief Declarations for gnss ppp
 * @details
 * @author Dr. Zhen Li
 * @version 0.0.1
 * @date 2023.12.25
 * @note this file includes all the functions for gnss ppp
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

    // precie point positioning
    STATUS_SOL precise_point_positioning(OBSDATA_EPOCH *obsepoch, SOLUTION *solt);

#ifdef __cplusplus
}
#endif

#endif

#endif