	/*Simulate Annealing */
	int maxRuns=100; /* Total number of random moves */
    for(int i=0;i<maxRuns;i++)
    {   /*random moves */
      
        int upHillMoves=0,rejectMoves=0,downHillMoves=0,m=0;
        int maxMoves = 5*NUM_BLOCKS;
 
        /* Try total iteration or uphill moves for T */
        //for(int m=0;(m<maxMoves*2) && ((m/2) < upHillMoves);m++)
        while((m<maxMoves*2) && ( upHillMoves < (maxMoves)))
        {
            tempFLP = randomMove(flpBlockData,&seqPair);
            for(int i=0;i<noBlocks;i++){
                tempPair.update(tempFLP);
                tempPair.posSeq[i].update(flpBlockData[tempPair.posSeq[i].blockNo]);
                tempPair.negSeq[i].update(flpBlockData[tempPair.negSeq[i].blockNo]);
            }
            /* convert sequence pair to floorplan  */
            tempFLP = sequencePairToFloorplan(&tempPair,tempFLP);
            /* Converted floorplan from Sequence Pair */
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
            cout<<"\n\tRun: "<<i<<"\tMoves: "<<(m++)<<"\tTemp: "<<maxTemp;
            cout<<"\tdownHill: "<<downHillMoves<<"\tupHill :"<<upHillMoves<<"bestTemp: "<< bestTemp;
            if (maxTemp < nowTemp || rand_fraction() < exp(-(maxTemp-nowTemp)/nowTemp))
            {
                 cout<<"\tAccept!"<<endl;
                 /* downhill always accepted	& boltzmann probability function	*/
                 if(maxTemp<bestTemp){
                    bestTemp = maxTemp;
                    /* write the floorplan to text file */
                    writeFloorPlan(bestFLP,tempFLP);
                    flpBlockData = tempFLP;
                 }
                 //writePower(bestPWR,flpBlockData);
                 if(maxTemp<nowTemp){
                        downHillMoves++;
                 }
                 else upHillMoves++;


            }
            else
            {
                cout<<"\tReject!"<<endl;
                rejectMoves++;
            }
            if (i==0){
                 bestTemp = maxTemp;
                 /* write the floorplan to text file */
                 writeFloorPlan(bestFLP,tempFLP);
                 flpBlockData = tempFLP;
                 //writePower(bestPWR,flpBlockData);
            }
            first_call = TRUE;
            m++; /* increment moves */
        }
    }