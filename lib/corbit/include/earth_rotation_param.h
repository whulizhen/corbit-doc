
/**
 * @file earth_rotation_param.h
 * @brief Declarations of earth rotation parameters computation
 * @details
 * @author Dr. Zhen Li
 * @version 0.0.1
 * @date 2023.12.25
 * @note this file includes all the functions for earth rotation parameters computation
 * @since 2016.01.15
 * @copyright
 * @par History:
 *
 */

#ifndef _EARTH_ROTATION_PARAM_H_
#define _EARTH_ROTATION_PARAM_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdio.h>
#include <stdlib.h>

#include <math.h>
#include <string.h>
#include <stdbool.h>
#include "iau_function.h"
#include "iers_function.h"
#include "mathlib.h"
#include "common_function.h"

#ifdef EMBED
#define MAXSIZE_EOP 3
#else
#define MAXSIZE_EOP 30000
#endif

//this is only for the eop interpolation
#define NUM_EOP_INTERP 3

	typedef struct _EOPREC_
	{
		char type;	   /**<  "final" of "prediction", 'F' for final, 'P' for prediction, 'C' for interpolated */
		bool ok;	   /**<  to indicate eop rec is ok or not */
		double mjd_utc; /**< mjd days in UTC */
		double pmX;		/**< polar motion x, Unit: arcsec */
		double pmY;		/**< polar motion y, Unit: arcsec */
		double UT1mUTC; /**< UT1 minus UTC, Unit: second */
		double LOD;		/**< the length of day,only in BulletinA ,Unit: second */
		double dX;		/**< dX Unit: arcsec */
		double dY;		/**< dY Unit: arcsec */
		double dPsi;    /**< dPsi */
		double dEpsilon; /**< dEpsilon */
		double dOMEGA; /**<  the correction to the earth rotation speed, rad/second */
	} EOPREC;

	void initialize_global_earth_rotation_param();
	
	int search_eop_index(double mjd_utc);

	bool compute_eop(double mjd_utc, EOPREC *eop);

	void mean_polar_motion(double mjd_UTC, double pm_xy[2]);

	bool compute_eci2ecf_matrix(double mjd_utc, double leap_sec, double *pos_eci2ecf, double *vel_eci2ecf);

	void compute_eci2ecf_rotation_precise(double mjd_utc, double leapsec, double pmx, double pmy, double ut1mutc, double lod, double dOMEGA, double *pos_eci2ecf, double *vel_eci2ecf);

	void correction_PoleTide_gravity(double mjd_utc, double eop_pmx, double eop_pmy, double *dC21, double *dS21);
	void correction_PoleTide_position(double mjd_utc, double eop_pmxy[2], double *xyz_ecf, double *disp_ecf);

#ifndef EMBED
	bool load_eop_storage(char *eopfilename);
	bool load_eop_storage_rapid(char *eopfilename);
	bool load_eop_storage_celestrack(char *eopall_filename);
#endif
	
	void eci2ecf(double mjd_utc, double leap_sec, double *pos_eci, double *vel_eci, double *pos_ecf, double *vel_ecf);
	void ecf2eci(double mjd_utc, double leap_sec, double *pos_ecf, double *vel_ecf, double *pos_eci, double *vel_eci);
	void load_eops_default(EOPREC * ptr_eop_store, int nsize);
	
	// declare for external use
	extern EOPREC *g_eop_storage;
	extern long g_eop_storage_size;

#ifdef __cplusplus
}
#endif

#endif