/* Generic libraries */
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <algorithm>
#include <tgmath.h>
#include <time.h>
#include <ctime>
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
char flpFile[] = "Results/output.flp";
char pwrFile[] = "Results/output.pwr";
char seqPairFile[] = "Results/best.seq";
char bestFlpFile[] = "Results/best.flp";
char bestPwrFile[] = "Results/best.pwr";

int main(int argc, char **argv) {
    if(argc != 2 )
        cout<<argc<<"Usage : TAFloorplanner Results/input.txt"<<endl;
    //char *inputFile = argv[1];
    //flpFile[] = argv[1];

    int start_clk = clock();
    srand(time(0));
    block *blockData;
    flpBlock *flpBlockData,*tempFLP,*bestFLP;
    double maxTemp,bestTemp,newTemp;
    float cost,newCost,maxCost,bestCost,lamdaA,lamdaT;
    float area,newArea,bestArea,newGamma,bestGamma;
    int Run=0,tBlock=0,bestBlock=0,b2Block=0;
    double maxT,maxT2;//maxBlockTempValue=0;
    /* Read input blocks data files */
    blockData = readBlocks(inputFile,blockData);
    /* Global variables noBlocks (int), Gamma(double) */
    cout<<"\tnoBlocks:"<<noBlocks<<endl;
    cout<<"\tGamma:"<<Gamma<<endl;
    for(int i=0;i<noBlocks;i++){
        blockData[i].print();
        //cout<<" "<<i;
    }
    /* Initial floor-plan : use sequence pairs */
    /* Try other configuration from SP file */
    /* initial random solution and its sequence pair */
    sequencePair seqPair,tempPair;
    //sequencePair *seqPair=(sequencePair *)malloc(noBlocks*sizeof(sequencePair *));
    flpBlockData = initFloorplan(blockData,&seqPair);
    cout<<"\nDone!"<<endl;
    //readSeqPair(seqPairFile,flpBlockData,&seqPair);
    cout<<"Initial Solution for Floorplan !"<<endl;
    for(int i=0;i<NUM_BLOCKS;i++){
            seqPair.update(flpBlockData);
            seqPair.posSeq[i].update(flpBlockData[seqPair.posSeq[i].blockNo]);
            seqPair.negSeq[i].update(flpBlockData[seqPair.negSeq[i].blockNo]);
    }
    cout<<"Initial Solution for Floorplan !"<<endl;
    //flpBlockData = flpToSequencePair(flpBlockData,&seqPair);
    flpBlockData = sequencePairToFloorplan(&seqPair,flpBlockData);
    cout<<"\tnoBlocks:"<<noBlocks<<endl;
    for(int i=0;i<noBlocks;i++){
            seqPair.update(flpBlockData);
            seqPair.posSeq[i].update(flpBlockData[seqPair.posSeq[i].blockNo]);
            seqPair.negSeq[i].update(flpBlockData[seqPair.negSeq[i].blockNo]);
    }
    for(int i=0;i<noBlocks;i++){
            flpBlockData[i].print();
    }
    seqPair = shelfAlgo(seqPair);
    cout<<"\nPosSeq: ";
    for(int i=0;i<NUM_BLOCKS;i++)
         cout<<" "<<seqPair.posSeq[i].blockNo;
    cout<<"\nNegSeq: ";
    for(int i=0;i<NUM_BLOCKS;i++)
         cout<<" "<<seqPair.negSeq[i].blockNo;
    /* convert sequence pair to floorplan  */
    flpBlockData = sequencePairToFloorplan(&seqPair,flpBlockData);
    /* Converted floorplan from Sequence Pair */
    /* write the floorplan to text file */
    writeFloorPlan(flpFile,flpBlockData);
    writeSeqPair(seqPairFile,&seqPair);
    /* hotspot integration code */
    /* Initializes the configuration parameters and other variables */
    sim_init();
    /* Main loop that runs the simulation required number of time : 1 for our case */
    sim_main();
    /* calculates steady state temperature */
    sim_exit();
    newTemp = maxBlockTemp(steady_temp);
    /* write the floorplan to text file */
    /*writeFloorPlan(bestFlpFile,tempFLP);*/
    maxT = steady_temp[0];
    maxT2 = 0;
    tBlock = 0;
    //cout<<"\n\tBest Temps : ";
    for(int i=0;i<NUM_BLOCKS;i++)
    {
        //cout<<steady_temp[i]<<" ";
        if(maxT < steady_temp[i])
        {
             maxT = steady_temp[i];
             bestBlock = i;
        }else if (maxT2 < steady_temp[i] && steady_temp[i] < maxT)
        {
                     maxT2 = steady_temp[i];
                     b2Block = i;
        }
    }
    cout<<endl;
    bestTemp = newTemp;
    bestArea = area = 0.000256;
    newArea = flpH*flpW;
    bestGamma = newGamma = (fabs(newArea - area)/area);
    writeFloorPlan(bestFlpFile,flpBlockData);
    bestCost = cost = bestTemp + 20*bestGamma;
    cout<<"\n\t Initial Solution :"<<0<<"\tMoves: "<<(1)<<"\tTemp: "<<bestTemp;
    cout<<"\tdownHill: "<<0<<"\tupHill :"<<0<<"\tbestTemp: "<< bestTemp<<"("<<bestBlock<<")/"<<maxT2<<"("<<b2Block<<")";
    //cout<<"\tbestGamma: "<<bestGamma<<"\tArea : "<<flpH*flpW;
    cout<<"\t Gamma : "<<newGamma<<endl;
    //cout<<"\tProgress-bar : ";
    int yes;
    cin>>yes;
    /* loop over a certain number of time *//* start Fast- simulate annealing */
    /* It has three steps : i) Set T = -T/ln(p) ii) T -> 0 greedy selection   & iii) Annealing */
    float P0 = 0.99; /* Initial probability of acceptance */
	float T3,T2,T,T1; T = T1 = -1 / log(P0); /* Initial annealing temperature */
    float Tstop = -1 / log ((1.0 - P0) / 2.0);
	int maxRuns=100; /* Total number of random moves */
    float upHillMoves=0,rejectMoves=0,downHillMoves=0,m=0;
    int stop_clk = clock(); /* calculate the stop time of clock */
    float exeTime = ((stop_clk - start_clk)/double(CLOCKS_PER_SEC)*1000)/1000;
    cout<<"\nExecution Time (s) : "<<exeTime<<endl;
    for(int i=0;i<maxRuns && T > Tstop && exeTime < 570 ;i++)
    {   /*random moves */
        flpBlockData = sequencePairToFloorplan(&seqPair,flpBlockData);
        for(int i=0;i<noBlocks;i++){
            seqPair.update(flpBlockData);
            seqPair.posSeq[i].update(flpBlockData[seqPair.posSeq[i].blockNo]);
            seqPair.negSeq[i].update(flpBlockData[seqPair.negSeq[i].blockNo]);
        }
        upHillMoves=0,rejectMoves=0,downHillMoves=0,m=0;
        int maxMoves = 5*NUM_BLOCKS;
        /* Try total iteration or uphill moves for T */
        //for(int m=0;(m<maxMoves*2) && ((m/2) < upHillMoves);m++)
        while((m<maxMoves*2) && upHillMoves < maxMoves && rejectMoves < 2*maxMoves )
        {
            //flpBlockData = randomMove(flpBlockData,&seqPair);
            tempFLP = randomMove(flpBlockData,&seqPair);
            /* update the sequence */
            /*for(int i=0;i<noBlocks;i++){
                seqPair.update(flpBlockData);
                seqPair.posSeq[i].update(flpBlockData[seqPair.posSeq[i].blockNo]);
                seqPair.negSeq[i].update(flpBlockData[seqPair.negSeq[i].blockNo]);
            }*/
            for(int i=0;i<noBlocks;i++){
                tempPair.update(tempFLP);
                tempPair.posSeq[i].update(flpBlockData[tempPair.posSeq[i].blockNo]);
                tempPair.negSeq[i].update(flpBlockData[tempPair.negSeq[i].blockNo]);
            }
            /* printing the update sequence */
            /*cout<<"\nPosSeq: ";
            for(int i=0;i<NUM_BLOCKS;i++)
                cout<<" "<<seqPair.posSeq[i].blockNo;
            cout<<"\nNegSeq: ";
            for(int i=0;i<NUM_BLOCKS;i++)
                cout<<" "<<seqPair.negSeq[i].blockNo;*/
            /*cout<<"Check update on seqPair :"<<endl;
            for(int i=0;i<noBlocks;i++){
                seqPair.posSeq[i].print();
            }*/
            /* convert sequence pair to floorplan  */
            //flpBlockData = sequencePairToFloorplan(&seqPair,flpBlockData);
            tempFLP = sequencePairToFloorplan(&tempPair,tempFLP);
            /* Converted floorplan from Sequence Pair */
            /*cout<<"\n Converted floor-plan from Sequence:"<<endl;
            for(int i=0;i<noBlocks;i++){
                flpBlockData[i].print();
            }*/
            /* write the floorplan to text file */
            //writeFloorPlan(flpFile,flpBlockData);
            writeFloorPlan(flpFile,tempFLP);
            //writePower(pwrFile,flpBlockData);
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
            newArea = flpH*flpW;
            newGamma = (fabs(newArea - bestArea)/bestArea);
            newCost = 0.5*maxTemp + 100*newGamma;
            //if ((maxTemp < newTemp || rand_fraction() < exp(-(maxTemp-newTemp)/T)) && newGamma < Gamma)
            if ((maxTemp < newTemp || rand_fraction() < exp(-(newCost-cost)/T)) /*&& newGamma < Gamma*/)
            {
                 /* downhill always accepted & up hill boltzmann probability function */
                 //writePower(bestPWR,flpBlockData);
                 if(maxTemp<newTemp){
                        downHillMoves++;
                 }
                 else upHillMoves++;

                 if(maxTemp<bestTemp && newGamma <= Gamma){
                    bestTemp = maxTemp;
                    bestGamma = newGamma;
                    /* write the floorplan to text file */
                    writeFloorPlan(bestFlpFile,tempFLP);
                    writeSeqPair(seqPairFile,&seqPair);
                    bestFLP = tempFLP;
                    maxT=steady_temp[0];maxT2 = 0;tBlock=0;
                    //cout<<"\n\tBest Temps : ";
                    for(int i=0;i<NUM_BLOCKS;i++)
                    {
                        //cout<<steady_temp[i]<<" ";
                        if(maxT < steady_temp[i])
                        {
                                maxT = steady_temp[i];
                                bestBlock = i;
                        }else if (maxT2 < steady_temp[i] && steady_temp[i] < maxT)
                        {
                                maxT2 = steady_temp[i];
                                b2Block = i;
                        }
                    }
                     /* print statistics */
                    cout<<"\n\tRun: "<<i<<"\tMoves: "<<(++m);/*<<"\tTemp: "<<maxTemp<<"("<<tBlock<<")"<<"\t Gamma : "<<newGamma;*/
                    cout<<"\tdownHill: "<<downHillMoves<<"\tupHill :"<<upHillMoves<<"\tbestTemp: "<< bestTemp<<"("<<bestBlock<<")";//"<<maxT2<<"("<<b2Block<<")";
                    cout<<"\tbestGamma: "<<bestGamma;//<<"\tArea : "<<flpH*flpW*100000000;
                    cout<<endl;
                 }
                 /* FIND THE BLOCK WITH MAX TEMP */
                 maxT=steady_temp[0];tBlock=0;
                 for(int i=0;i<NUM_BLOCKS;i++){
                    if(maxT < steady_temp[i]){
                      maxT = steady_temp[i];
                      tBlock = i;
                    }
                 }
                 newTemp = maxTemp;cost = newCost;
                 flpBlockData = tempFLP;
            }
            else{
                rejectMoves++;
            }
            /* print statistics */
            first_call = TRUE;
            m++;
            //cout<<"*";
        }
        /* update  (reduce ) annealing Temperature	*/
		T1 *= 0.95;//T2 = T1/10;
		/*if(1<i && i< 5)
            T = T1/10;
        else
            T = T1/10;*/
        /* print statistics */
        cout<<"\n\tRun: "<<i<<"\tMoves: "<<(++m);/*<<"\tTemp: "<<maxTemp<<"("<<tBlock<<")"<<"\t Gamma : "<<newGamma;*/
        cout<<"\tdownHill: "<<downHillMoves<<"\tupHill :"<<upHillMoves<<"\tbestTemp: "<< bestTemp<<"("<<bestBlock<<")";//"<<maxT2<<"("<<b2Block<<")";
        cout<<"\tbestGamma: "<<bestGamma;//<<"\tArea : "<<flpH*flpW*100000000;
        //cout<<endl;
        stop_clk = clock();
        exeTime = ((stop_clk - start_clk)/double(CLOCKS_PER_SEC)*1000)/1000;
        cout<<"\tTime (s) : "<<exeTime<<endl;
    }
    cout<<"\nRun: "<<(maxRuns)<<"\tbest Temp: "<<bestTemp<<endl;
    /*Clean Up*/
    free(blockData); free(flpBlockData);
    return 0;
}
