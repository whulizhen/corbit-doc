
/**
 * @file attitude.h
 * @brief Declarations of attitude operations
 * @details
 * @author Dr. Zhen Li
 * @version 0.0.1
 * @date 2023.12.25
 * @note this file includes all the functions for attitude operations.
 * @since 2016.01.15
 * @copyright
 * @par History:
 *
 */

#ifndef _ATTITUDE_H_
#define _ATTITUDE_H_

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef EMBED
#include <stdio.h>
#include <stdlib.h>
#endif

#include "mathlib.h"
#include "gnss_config.h"
#include "gtime.h"

#ifndef EMBED

#define MAXSIZE_ATT_OBJ 3	  /**< the maximum number of objects in the ATT_QUATERION_STORE */
#define MAXSIZE_ATT_QUA (86401*2) /**< max number of quaternions in the storage*/

#else

#define MAXSIZE_ATT_OBJ 2	  /**< the maximum number of objects in the ATT_QUATERION_STORE */
#define MAXSIZE_ATT_QUA 10 /**< max number of quaternions in the storage*/

#endif


	/**
	 * @brief attitude quaternion, it is a time tagged quaternion object
	 */
	typedef struct _att_quaternion_
	{
		GTime time;		/**<  time epoch in GPST */
		QUATERNION qua; /**< quaternion values*/
		bool ok;		/**< ok or not */
	} ATT_QUATERNION;

	// struct for quaternion attitude
	typedef struct _att_quaternion_store_
	{
		bool updated;
		ATT_QUATERNION *qua_all[MAXSIZE_ATT_OBJ]; /**< an array of pointer to store all the ATT_QUATERNION for all the objects */
		int num_object;							  /** <the total number of objects in the ATT_QUATERION_STORE */
		int idx_object[MAXSIZE_ATT_OBJ];		  /**< the object index for all the objects*/
		int qua_num[MAXSIZE_ATT_OBJ];			  /**< the number of quaternions for each object */
	} ATT_QUATERION_STORE;

#ifndef EMBED
	int read_att_quaternion_gfo(char *quaternion_att_filename, ATT_QUATERNION *qua_store, int record_start);
	
	int read_att_quaternion_corbit(char *quaternion_att_filename, ATT_QUATERNION *qua_store, int record_start);
	bool write_att_quaternion_coribt(char *quaternion_att_filename, ATT_QUATERNION *qua_store, int num_qua);

	bool load_attitude_quaternion(int satindex, char *quaternion_att_filename);
	bool load_sca_grace(int satindex, char *quaternion_att_filename);
	void free_att_quaternion_store();
#endif

	int attitude_platform(int index_object, GTime time_utc, double *position_platform_ecf, double *velocity_platform_ecf, double *position_platform_eci, double *velocity_platform_eci, ATT_QUATERNION *att_qua, double *ex, double *ey, double *ez, double *ep);
	bool get_att_quaternion(int idx_sat, GTime time_target, ATT_QUATERNION *qua_att);

	void gnss_sat_attitude(int satindex, double *satpos, double *satvel, double *sunpos, double *ex, double *ey, double *ez, double *ep);
	void attitude_yaw_steering(double *satpos, double *satvel, double *sunpos, double *ex, double *ey, double *ez, double *ep);

	extern ATT_QUATERION_STORE g_att_qua_storage; /**< global: the attutde quaternion storage*/

#ifdef __cplusplus
}
#endif

#endif
