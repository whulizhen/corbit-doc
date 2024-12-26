
/**
 * @file flux_grid.h
 * @brief Declarations of all the functions related to the grid file processing in srp, erp and earth flux
 * @details
 * @author Dr. Zhen Li
 * @version 0.0.1
 * @date 2023.12.25
 * @note this file includes Declarations of all the functions related to the grid file processing in srp, erp and earth flux
 * This srp or erp grid files are mainly used for high-precision post-processing
 * @since 2016.01.15
 * @copyright
 * @par History:
 *
 */

#ifndef EMBED

#ifndef _GRID_DATA_H_
#define _GRID_DATA_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "gtime.h"
    
typedef struct _grid_meta_
{
    char name_grid[100]; // the name of the grid file
    char type_grid[50];  // could be "srp_acc", "erp_acc", "earth_flux_longwave", "earth_flux_shortwave"
    double interval_row; // the interval in row
    double interval_col; // the interval in col
    double grid_range_row[2]; //the start and end in row for the grid
    double grid_range_col[2]; //the start and end in col for the grid
} GRID_META;

typedef struct _grid_data_
{
    GRID_META meta; // the meta data for this grid
    int nrow; // number of rows
    int ncol; // number of cols
    double *data; // to store the data
} GRID_DATA;

//this struct is used to store all the grid data
typedef struct _dataset_grid_
{
    int num;
    GRID_DATA *dataset;
} DATASETS_GRID;

void read_grid_ncf(char *filename_ncf);

extern DATASETS_GRID g_grid_storage;

#ifdef __cplusplus
}
#endif

#endif

#endif

