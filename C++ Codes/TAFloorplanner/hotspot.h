#ifndef __HOTSPOT_H_
#define __HOTSPOT_H_
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#include "flp.h"
#include "package.h"
#include "temperature.h"
#include "temperature_block.h"
#include "temperature_grid.h"
#include "util.h"
#include "hotspot.h"
#include "util.h"
#include "TAfloorplan.h"
#define NUM_BLOCKS 30

struct hspBlock{
    char name[100];
    double steady_temp;
};

/* global configuration parameters for HotSpot	*/
typedef struct global_config_t_st
{
	/* floorplan input file */
	char flp_file[STR_SIZE];
	/* input power trace file */
	char p_infile[STR_SIZE];
	/* output file for the temperature trace */
	char t_outfile[STR_SIZE];
	/* input configuration parameters from file	*/
	char config[STR_SIZE];
	/* output configuration parameters to file	*/
	char dump_config[STR_SIZE];
}global_config_t;

/*
 * parse a table of name-value string pairs and add the configuration
 * parameters to 'config'
 */
void global_config_from_strs(global_config_t *config, str_pair *table, int size);
/*
 * convert config into a table of name-value pairs. returns the no.
 * of parameters converted
 */
int global_config_to_strs(global_config_t *config, str_pair *table, int max_entries);

//double  *hotspot(int argc, char **argv,double *steady_temp);
//struct block  *hotspot(int argc, char **argv,struct block *hotspotData);
//void hotspot(int argc, char **argv,struct hspBlock *hotspotData);
struct hspBlock *hotspot(int argc, char **argv);
#endif
