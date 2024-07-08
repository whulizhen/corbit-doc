

/**
 * @file gnss_simulator.h
 * @brief Declarations of all the gnss simulations
 * @details
 * @author Dr. Zhen Li
 * @version 0.0.1
 * @date 2023.12.25
 * @note this file includes all the functions for gnss simulation
 * @since 2016.01.15
 * @copyright
 * @par History:
 *
 */

#ifndef EMBED

#ifndef _GNSS_SIMULATOR_H_
#define _GNSS_SIMULATOR_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "gnss_config.h"
#include "gnss_observation.h"

    void simulate();

    //deal with the waypoint files
    void read_waypoints();
    void write_waypoints();

#ifdef __cplusplus
}
#endif

#endif

#endif
