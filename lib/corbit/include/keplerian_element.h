
/**
 * @file keplerian_element.h
 * @brief Declarations for Keplerian elements computation
 * @details These functions are for Keplerian elements computation
 * @author Dr. Zhen Li
 * @version 0.0.1
 * @date 2023.12.25
 * @note this file includes all the functions for Keplerian elements computation
 * @since 2016.01.15
 * @copyright
 * @par History:
 *
 */

#ifndef EMBED

#ifndef _KEPLERIAN_ELEMENT_H_
#define _KEPLERIAN_ELEMENT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdio.h>
#include <math.h>
#include <string.h>
#include "corbit_const.h"

    /**
     * @brief definition of struct Keplerian_element
     *
     */
    typedef struct _keplerian_ele_
    {
        double m_sma;    /**< semi-major axis (km) */
        double m_ecc;    /**< eccentricity of ellipse (ratio-->no units) */
        double m_inc;    /**< inclination of orbital plane (rad) */
        double m_argp; /**< argument of perigee (rad) */
        double m_raan; /**< right ascension of ascending node (rad) */
        double m_tran; /**< true anomaly (rad) */

        double m_ecan; /**< eccentric anomaly (rad) */
        double m_mean; /**< mean anomaly (rad) */

    } keplerian_elements;

    void update_kpe(keplerian_elements *kpe, double sec_since);
    void kpe2xyz(keplerian_elements *kpe, double *pos, double *vel);
    void xyz2kpe(double *pos, double *vel, keplerian_elements *kpe);

    double get_ecan_from_mean(double mean, double ecc);
    double get_mean_from_ecan(double ecan, double ecc);
    double get_tran_from_ecan(double ecan, double ecc);
    double get_ecan_from_tran(double ecc, double tran);

#ifdef __cplusplus
}
#endif

#endif

#endif
