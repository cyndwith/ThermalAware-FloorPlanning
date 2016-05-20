    /* loop over a certain number of time */
    /* It has three steps : i) Set T = -T/ln(p) ii) T -> 0 greedy selection   and  iii) Traditional Annealing */
    float P0 = 0.99,T3,T2,T,T1; T = T1 = -1 / log(P0); /* Initial probability of acceptance */ /* Initial annealing temperature */
    float Tstop = -1 / log ((1.0 - P0) / 2.0),upHillMoves=0,rejectMoves=0,downHillMoves=0,m=0;
    int stop_clk = clock(), maxRuns=100; /* calculate the stop time of clock */
    for(int i=0;i<maxRuns && T > Tstop && exeTime < 570 ;i++){ /* stop criteria check */
      	upHillMoves=0,rejectMoves=0,downHillMoves=0,m=0;int maxMoves = 5*NUM_BLOCKS; /*random moves */
        while((m<maxMoves*2) && upHillMoves < maxMoves && rejectMoves < 2*maxMoves ){ 
            /* Try total iteration or uphill moves for T */
            tempFLP = randomMove(flpBlockData,&seqPair); /* use temp floorplan which is saved if move is accepted */
            /* update sequence pair */
            tempFLP = sequencePairToFloorplan(&tempPair,tempFLP); /* convert sequence pair to floorplan  */
            writeFloorPlan(flpFile,tempFLP); /* write the floorplan to text file */
            sim_init(); /* hotspot integration code - Initializes the configuration parameters and other variables */
            sim_main(); /* Main loop that runs the simulation required number of time : 1 for our case */
            sim_exit(); /* calculates steady state temperature */
            maxTemp = maxBlockTemp(steady_temp);newArea = flpH*flpW;newGamma = (fabs(newArea - bestArea)/bestArea);
            newCost = 0.5*maxTemp + 40*newGamma;
            if ((maxTemp < newTemp || rand_fraction() < exp(-(newCost-cost)/T)){ /*&& newGamma < Gamma*/){
                 if(maxTemp<newTemp) downHillMoves++;  /* downhill always accepted & up hill boltzmann probability function */
                 else upHillMoves++;
                 if(maxTemp<bestTemp && newGamma <= Gamma){
                    bestTemp = maxTemp;bestGamma = newGamma;
                    writeFloorPlan(bestFlpFile,tempFLP);  /* write the floorplan to text file */
                    writeSeqPair(seqPairFile,&seqPair);
                    bestFLP = tempFLP;maxT=steady_temp[0];maxT2 = 0;tBlock=0;
                 }
                 newTemp = maxTemp;cost = newCost;flpBlockData = tempFLP;
            }
            else
                rejectMoves++;
        } 
		/* print statistics   and  update  (reduce ) annealing Temperature	 T  *= 0.95 */
        stop_clk = clock(); /* calculate time to stop at 570 seconds */
        exeTime = ((stop_clk - start_clk)/double(CLOCKS_PER_SEC)*1000)/1000;
    }
    cout<<"\nRun: "<<(maxRuns)<<"\tbest Temp: "<<bestTemp<<endl;
    /*Clean Up*/
    free(blockData); free(flpBlockData);
    return 0;
}
