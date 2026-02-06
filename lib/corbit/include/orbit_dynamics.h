

/**
 * @brief Header file for orbit dynamics computation
 * @file orbit_dynamics.h
 * @details The declaration of orbit dynamics functions
 * @author Dr. Zhen Li
 * @version 0.0.1
 * @date 2023.12.25
 * @note this file includes all the functions for orbital acceleration, orbit integrate and orbit fit
 * @since 2016.01.15
 * @par Copyright (c)
 * @par History:
 *
 */

#ifndef _ORBIT_DYNAMICS_H_
#define _ORBIT_DYNAMICS_H_

#ifdef __cplusplus
extern "C"
{
#endif


#ifndef EMBED
#include <stdio.h>
#endif

#include "mathlib.h"
#include "common_function.h"
#include "iers_function.h"
#include "iau_function.h"
#include "earth_gravity.h"
#include "eph_planet.h"
#include "attitude.h"
#include "atmospheric_density.h"

#ifndef EMBED
/// @brief maximum number of the parameters for the empirical force models (piece-wise parameters)
#define MAXNUM_FM_PARAM_EMP 100 // 864 params when  estimate 3 params in every 5 minutes
#define MAXNUM_PARAM_EMP_BASE 10
/// @brief maximum number of the parameters for the other force models (constant during the estimation)
#define MAXNUM_FM_PARAM_OHR 1

#else

#define MAXNUM_FM_PARAM_EMP  3 // 864 params when  estimate 3 params in every 5 minutes
#define MAXNUM_PARAM_EMP_BASE 3
#define MAXNUM_FM_PARAM_OHR 1  //at least 1
#endif


#define MAXLEN_PARAM_NAME 20

/// @brief maximum number of parameters for all the force models
#define MAXNUM_FM_PARAM_TOTAL (MAXNUM_FM_PARAM_EMP + MAXNUM_FM_PARAM_OHR * 2)

// @brief maximum number of states in the integrator
#define NSTATE_MAX  (6 + 36 + 6 * MAXNUM_FM_PARAM_TOTAL)

    /**
     * @brief enum definitions of force models
     * @details a enum definition for all the force model types
     * */
    typedef enum _force_model_
    {
        FM_UKN,            /**< unkonwn force model */
        FM_EarthGravity,   /**< earth gravity */
        FM_ThirdBody,      /**< third-body gravity */
        FM_Relativity,     /**< general relativity */
        FM_Antenna_Thrust, /**< Antenna thrust*/
        FM_Body_Fix_Force, /**< constant force along body fixed frame axes */
        FM_SRP,            /**< solar radiation pressure */
        FM_ERP,            /**< earth radiation pressure */
        FM_Drag,           /**< atmospheric drag */
        FM_ECOM,           /**< the ECOM empirical model for navigation satellites */
        FM_EMP,            /**< the empirical force models */
        FM_TOTAL           /**< the total number of force models */
    } ForceModel;

    typedef struct _EMP_PARAM_BASE_
    {
        ForceModel fm; // the force model that this parameter is belong to
        char name_param[MAXLEN_PARAM_NAME];
        double interval; // interval in seconds
    } EMP_PARAM_BASE;

    /**
     * @brief the struct to store force model parameters
     * @details it includes force model enum, mode, num of parameters, all the values of the parameters and their life
     *
     */
    typedef struct _param_force_model_
    {
        ForceModel fm;                                       /**< force model enum*/
        int mode;                                            /**< force model mode*/
        int num;                                             /**< number of parameters*/
        double param[MAXNUM_FM_PARAM_OHR];                   /**< values of the parameters*/
        double param_life[MAXNUM_FM_PARAM_OHR][2];           /**< the life cycle of each param in mjd, 0: life start, 1: life end. */
        int idx_global[MAXNUM_FM_PARAM_OHR];                 /**< the index of force model parameters in the global parameter list(including pos+vel)*/
        char param_name[MAXNUM_FM_PARAM_OHR][MAXLEN_PARAM_NAME]; // the names of each force model parameters
    } PARAM_FM;

    /// this is similar as PARAM_FM, but specific for the piece-wise empirical force models
    /// the reason why they should be separated is that PARAM_FM_EMP usually has much more parameters
    /// To separate them can save memory
    typedef struct _param_force_model_emp_
    {
        ForceModel fm;                                       /**< force model enum*/
        int mode;                                            /**< force model mode*/
        int num;                                             /**< number of parameters*/
        double param[MAXNUM_FM_PARAM_EMP];                   /**< values of the parameters*/
        double param_life[MAXNUM_FM_PARAM_EMP][2];           /**< the life cycle of each param in mjd, 0: life start, 1: life end. */
        int idx_global[MAXNUM_FM_PARAM_EMP];                 /**< the index of force model parameters in the global parameter list(including pos+vel)*/
        char param_name[MAXNUM_FM_PARAM_EMP][MAXLEN_PARAM_NAME]; // the names of each force model parameters
    } PARAM_FM_EMP;

    /**
     * @brief the orbit dynamics configure struct, works for both orbit integrate and orbit fit.
     * @details a struct for orbit dynamics configure
     */
    typedef struct _orbit_dynamics_config_
    {
        int satindex; /**< the index of satellite in gnssconfig */
        int ndegree_gravity; /**< earth gravity degree */ 
        int norder_gravity; /**< earth gravity order */
        bool time_vary_gravity;
        bool solid_earth_tide;
        bool ocean_tide;
        bool pole_tide;

        int opt_srp;  // option for a priori solar radiation pressure
        int opt_erp;  // option for a priori earth radiation pressure
        int opt_drag; // option for a priori atmospheric drag
        int opt_emp;  // option for empirical force

        // the following emp_params_base and emp_param_num_base includes both empirical params and Cd and Cr
        EMP_PARAM_BASE emp_params_base[MAXNUM_PARAM_EMP_BASE]; // the basic empirical params at each time interval
        int emp_param_num_base;                                // the param number of empirical force model at each time interval

        int num_planets;                  /**< number of planets used in third-body gravity */
        PLANET_JPL planets[TOTAL_PLANET]; /**< planet used */

        int num_force_models;                  /**< the number of forcemodels  */
        ForceModel force_model_used[FM_TOTAL]; /**< all the force models used */

        double step_size;       /**<integrate step size*/
        double output_interval; /**< the output interval for the orbit intergral*/
        int status_derivative;  /**< 0 means pos+vel, 1 means pos,vel and PHI are used, 2 means pos+vel, PHI and S are all used */

        char param_names_force_models[MAXNUM_FM_PARAM_TOTAL][MAXLEN_PARAM_NAME]; // maintain a force model parameter list globally
        int num_param_total;                                                 /**< total number of parameters(pos+vel+force_parameters)*/

        // moved from spacecraft_state struct
        int num_param_forcemodel;                  /**< number of force model parameters excluding the satellite position, velocity */
        PARAM_FM force_model_parameters[FM_TOTAL]; /**< the parameters for all the force models at current time */
        PARAM_FM_EMP force_model_emp_parameters;   /**< the parameters for the EMP force model*/
        int nstate;                                /**< the number of elements in state vector*/

    } ORBIT_DYN_CONFIG;

    /**
     *
     * @brief spacecraft state struct
     * @details
     */
    typedef struct _state_spacecraft_
    {
        int satindex; /**< satindex in gnssconfig */

        GTime utc_start;        /**< a record of the start of the time for orbit integration */
        double sec_since_start; /**< the current time in sec since the start */

        double mass;       /**< spacecraft mass */
        double pos_eci[3]; /**< position in eci, unit: m */
        double vel_eci[3]; /**< velocity in eci, unit: m/s */
        double acc_eci[3]; /**< acceleration in eci, unit: m/s^2 */

        double orbit_raidal[3];
        double orbit_along[3];
        double orbit_cross[3];

        double dadr[9];                         /**< dadr matrix, 3 by 3 */
        double dadv[9];                         /**< dadv matrix, 3 by 3 */
        double dadp[3 * MAXNUM_FM_PARAM_TOTAL]; /**< dadp matrix,  3 by np */

        double PHI[6 * 6];                   /**< state transition matrix PHI*/
        double S[6 * MAXNUM_FM_PARAM_TOTAL]; /**< sensitivity matrix */

        double dPHI[36];                      /**< the derivatives of state transition matrix PHI, 6 by 6 */
        double dS[6 * MAXNUM_FM_PARAM_TOTAL]; /**< the derivatives of sensitivity matrix S, 6 by np */

        double shadow_factor; /**< shadow factor of the satellite */
        double beta;          // sun elevation over orbital plane
        double eps;           // angle from orbital node to the projection of sun vector in orbit plane
        double att_ex[3];     /**< unit vector of spacecraft body fixed frame ex in eci*/
        double att_ey[3];     /**< unit vector of spacecraft body fixed frame ey in eci*/
        double att_ez[3];     /**< unit vector of spacecraft body fixed frame ez in eci*/
        double att_ep[3];     /**< unit vector of spacecraft solar panel normal in eci, assuming solar panel is always pointing to the sun*/

    } STATE_SPACECRAFT;

    /**
     * @brief the output solution of the orbit fit
     *
     */
    typedef struct _Solution_OrbitFit_
    {
        double rms_acr[3]; /**< fit RMS in orbital A, C and R */
        double *omc_eci;   /**< the fit OMC in eci xyz, with dimension nepochs by 3 */
        double *omc_ARC;   /**< fit OMC in orbital Along, Radial, and Cross*/
        int nepochs;       // the epoch number in the xyz observation, relating to omc_eci;

        // ORBIT_DYN_CONFIG* config_dyn;  /**< this is a copy of the orbit_dyn_config struct*/
        STATE_SPACECRAFT *state_all; /**< the spacecraft state for all the epochs, with demention nepochs by 1*/
        int num_output;

    } SOLUTION_ORBITFIT;

    
    /**
     * @~English
     * @brief function pointer to define the derivatives computation functions
     * @param[in] (int)  number of state
     * @param[in] (double) time of the state
     * @param[in] (double*) the state vector
     * @param[in] (double*) the derivatives vector
     * @return void
     */
    typedef void (*Derivative)(int, double, double *, double *);

    /**
     *
     * @brief the orbit derivatives function
     * @param[in] nstate  number of state vector
     * @param[in] mjd_utc time of the state vector
     * @param[in] y the state vector
     * @param[in] dydx the derivatives vector
     * @return void
     */
    void orbit_derivative(int nstate, double mjd_utc, double *y, double *dydx);

    void initialize_orbit_config_embed(ORBIT_DYN_CONFIG *ptr_orb_config);

    
    void initialize_state_spacecraft_embed(ORBIT_DYN_CONFIG *ptr_orb_config, STATE_SPACECRAFT *state_spacecraft);

    int emp_param_name_check(char *emp_param_name);

    double shadow_factor_SECM(double *pos_sun, double *pos_sat, bool modified);

    void compute_acc_all(ORBIT_DYN_CONFIG *ptr_orb_config, STATE_SPACECRAFT *state_spacecraft);

    bool compute_acc_srp(double mjd_utc, PARAM_FM *param_fm, double *satpos, double *satvel, double shadowfactor, double *sunpos, double acc[3]);
    bool compute_acc_erp(int np, double *param, int mode, double mjd_utc, double *satpos, double *satvel, double acc[3]);
    bool compute_acc_drag(int np, double *param, int mode, double total_mass_density, double *satpos, double *satvel, double *xhat, double *yhat, double *zhat, double *phat, double acc[3]);
    bool compute_acc_relativity(double *satpos, double *satvel, double *sunpos, double *sunvel, double acc[3]);
    bool compute_acc_thirdbody(double mjd_utc, int nbody, PLANET_JPL *bodies, double *satpos_eci, double *acc, double *dadr);
    // bool compute_acc_ECOM(double mjd_utc, PARAM_FM *fm_param, double *sunpos, double *satpos_eci, double *satvel_eci, double beta, double *acc, double *dadp);

    int compute_acc_Empirical(double mjd_utc, int idx_param_current[MAXNUM_FM_PARAM_EMP], PARAM_FM_EMP *fm_param, double *satpos, double *satvel, double* sunpos, double beta, double *acc, double *dadp_eci);

    double atm_density_Harris_Priester(double mjd_tt, double Pos[3]);
    // double compute_atmospheric_density();

    void single_step_RungeKuttaFehlberg78(Derivative dydxFun, int nstate, double x0, double *y0, double h, double *yerr);
    void single_step_RungeKutta4(Derivative dydxFun, int nstate, double x0, double *y0, double h, double *yerr);

    void IntegrateTo(Derivative dydxFun, int n, double x0, double *y0, double *step_sec, double xt, double *yerr);

    void produce_force_model_params(double mjd_utc_s, double mjd_utc_e, ORBIT_DYN_CONFIG *ptr_orbit_dyn_config, STATE_SPACECRAFT *state_spacecraft);
    
    bool orbit_integrate(ORBIT_DYN_CONFIG *ptr_orb_config, STATE_SPACECRAFT *state_spacecraft, double duration_sec, double step_size);

    void test_orbit_integrate_embed();

#ifndef EMBED

    void initialize_orbit_config(ORBIT_DYN_CONFIG *ptr_orb_config);
    void initialize_state_spacecraft(ORBIT_DYN_CONFIG *ptr_orb_config, STATE_SPACECRAFT *state_spacecraft);
    
    void readfile_orbitdynamics_config(char *orbitdynamic_config_filename, ORBIT_DYN_CONFIG *orbitdyn_cfg);
    
    void generate_param_empirical(double mjd_utc_s, double mjd_utc_e, double interval_sec, PARAM_FM_EMP *params, ORBIT_DYN_CONFIG *ptr_orbit_dyn_config, STATE_SPACECRAFT *state_spacecrat);
    
    bool orbit_fit(ORBIT_DYN_CONFIG *ptr_orb_config, STATE_SPACECRAFT *state_spacecraft, GTime epoch_end, int nepochs, GTime *epoch_utc, double orb_posvel_eci[][3], SOLUTION_ORBITFIT *sln_fit);

    int test_orbit_fit_data(GTime *epochs_utc, double pos_eci[][3]);
    void test_orbit_fit();
    void test_orbit_integrate();
    void test_orbit_simulation(char *orbit_dynamics_config_filename, char *output_orb_filename, char *output_orb_filename_eci);

#endif

#ifdef __cplusplus
}
#endif

#endif