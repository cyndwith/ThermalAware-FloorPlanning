
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
//using namespace std;
#define NUM_BLOCKS 30

/*struct block{
    char name[100];
    double steady_temp;

}*hotspotData;*/


int main(int argc, char **argv) {
    int i;
    struct block *hotspotData,*hotspotData2;
    double *steady_temp;
    //steady_temp = hotspot(argc,argv,steady_temp);
    hotspotData=(struct block *)calloc(30, sizeof(struct block));
    for(i=0;i<100;i++){
        //cout<<"Run:"<<i<<endl;
        printf("Run: %d \n",i);
        //hotspotData2=(struct block *)calloc(30, sizeof(struct block));
        hotspotData = hotspot(argc,argv,hotspotData); //hotspot(argc,argv,hotspotData);
    }

    printf("stored in steady_temp:\n");
    for (i=0; i < 30; i++){
        //fprintf(stdout, "%s\t%.2f\n", hotspotData[i].name, hotspotData[i].steady_temp);
    }

    /*clean up*/
    printf("hotspotData freed!");
    free(hotspotData);
    //free(hotspotData2);
	return 0;
}
