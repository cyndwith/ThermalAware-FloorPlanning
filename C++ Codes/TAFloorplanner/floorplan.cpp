/* Generic libraries */
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <algorithm>
/* external C - libraries for hotspot.c */
extern "C"{
#include "flp.h"
#include "package.h"
#include "temperature.h"
#include "temperature_block.h"
#include "temperature_grid.h"
#include "util.h"
#include "hotspot.h"
#include "sim-template.h"
}

/* C++ - libraries or header files */
#include "TAfloorplan.h"
#include "IOFile.h"
#include "floorplan.h"
#include "sequencePair.h"

using namespace std;

char inputFile[] = "input.txt";
char flpFile[] = "output.flp";
char pwrFile[] = "output.pwr";
char bestFLP[] = "best.flp";
char bestPWR[] = "best.pwr";

int main(int argc, char **argv) {
    //struct hspBlock *hotspotData;//*hotspotData2;
    block *blockData;
    flpBlock *flpBlockData,*flpBlockData2;
    double maxTemp,minTemp;
    int Run=0;
    /* Read input blocks data files */
    blockData = readBlocks(inputFile,blockData);
    /* Global variables noBlocks (int), Gamma(double) */
    cout<<"noBlocks:"<<noBlocks<<endl;
    cout<<"Gamma:"<<Gamma<<endl;
    for(int i=0;i<noBlocks;i++){
        blockData[i].print();
    }

    /* Initial floorplan : use sequence pairs */
    flpBlockData = initFloorplan(blockData);
    cout<<"Initial Solution for Floorplan !"<<endl;
    // cout<<"Floorplan:"<<endl;
    /*for(int i=0;i<noBlocks;i++){
        flpBlockData[i].print();
    }*/
    /* Try other configuration from SP file */
    sequencePair seqPair;
    /* loop over a certain number of time */
    int noRuns=150;
    //while(1)
    for(int i=0;i<noRuns;i++)
    {
        flpBlockData = flpToSequencePair(flpBlockData,&seqPair);
        /* update the sequence */
        for(int i=0;i<noBlocks;i++){
            seqPair.posSeq[i].update(flpBlockData[seqPair.posSeq[i].blockNo]);
            seqPair.negSeq[i].update(flpBlockData[seqPair.negSeq[i].blockNo]);
        }
        /* printing the update sequence */
        /*cout<<"\nPosSeq: ";
        for(int i=0;i<NUM_BLOCKS;i++)
            cout<<" "<<seqPair.posSeq[i].blockNo;
        cout<<"\nNegSeq: ";
        for(int i=0;i<NUM_BLOCKS;i++)
            cout<<" "<<seqPair.negSeq[i].blockNo;
        */
        /*cout<<"Check update on seqPair :"<<endl;
        for(int i=0;i<noBlocks;i++){
            seqPair.posSeq[i].print();
        }*/
        /* convert sequence pair to floorplan  */
        flpBlockData = sequencePairToFloorplan(&seqPair,flpBlockData);

        /* Converted floorplan from Sequence Pair */
        /*cout<<"\n Converted floorplan from Sequence:"<<endl;
        for(int i=0;i<noBlocks;i++){
            flpBlockData[i].print();
        }*/
        /* write the floorplan to text file */
        writeFloorPlan(flpFile,flpBlockData);
        writePower(pwrFile,flpBlockData);
        /* hotspot integration code */
        /* Initializes the configuration parameters and other variables */
        sim_init();
        /* Main loop that runs the simulation required number of time : 1 for our case */
        sim_main();
        /* calculates steady state temperature */
        sim_exit();

        /*for(int i=0;i<NUM_BLOCKS;i++)
              cout<<i<<"\t"<<steady_temp[i]<<endl;*/
        maxTemp = maxBlockTemp(steady_temp);
        cout<<"\nRun: "<<(Run++)<<"\tmaxTemp: "<<maxTemp<<endl;
        if(maxTemp<minTemp){
              minTemp = maxTemp;
             /* write the floorplan to text file */
             writeFloorPlan(bestFLP,flpBlockData);
             writePower(bestPWR,flpBlockData);
        }
        else if (i==0){
             minTemp = maxTemp;
             /* write the floorplan to text file */
             writeFloorPlan(bestFLP,flpBlockData);
             writePower(bestPWR,flpBlockData);
        }


        first_call = TRUE;
    }
     cout<<"\nRun: "<<(Run++)<<"\tbest Temp: "<<minTemp<<endl;
    /*Clean Up*/
    free(blockData); free(flpBlockData);
    return 0;
}
