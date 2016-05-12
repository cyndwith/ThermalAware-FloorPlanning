#ifndef FLOORPLAN_H_INCLUDED
#define FLOORPLAN_H_INCLUDED

#include "sequencePair.h"
#include <iostream>
using namespace std;

int noBlocks;
double Gamma;

class block{
public:
    int blockNo;
    double width,height,power;

    void print(){
        //cout<<"Block Parameter:"<<endl;
        cout<<"block No :"<<blockNo;
        cout<<"\twidth :"<<width;
        cout<<"\theight :"<<height;
        cout<<"\tpower :"<<power<<endl;
    }
};

class flpBlock: public block
{
public:
    int posSeq_Location,negSeq_Location;
    double x,y;
    void print(){
        //cout<<"Block Parameter: ";
        cout<<"block No :"<<blockNo;
        //cout<<"\twidth :"<<width;
        //cout<<"\theight :"<<height;
        cout<<"\tpower :"<<power;
        cout<<"\tx :"<<x;
        cout<<"\ty :"<<y;
        cout<<"\tposSeqInd :"<<posSeq_Location;
        cout<<"\tnegSeqInd :"<<negSeq_Location<<endl;
    }
    void update(flpBlock B){
        blockNo = B.blockNo;
        width = B.width;
        height = B.height;
        power = B.power;
        x = B.x;
        y = B.y;
        posSeq_Location = B.posSeq_Location;
        negSeq_Location = B.negSeq_Location;
    }
};

class sequencePair{
public:
    flpBlock posSeq[NUM_BLOCKS];
    flpBlock negSeq[NUM_BLOCKS];
};


int myrandom (int i) { return std::rand()%i;}



flpBlock *initFloorplan(block *blockData)
{
    flpBlock *flpBlockData=(flpBlock *)malloc(noBlocks*sizeof(flpBlock));
    double xAxis=0,yAxis=0;
    for(int i=0;i<noBlocks;i++){
        flpBlockData[i].blockNo = blockData[i].blockNo;
        flpBlockData[i].width = blockData[i].width;
        flpBlockData[i].height = blockData[i].height;
        flpBlockData[i].power = blockData[i].power;
        flpBlockData[i].x = xAxis;
        flpBlockData[i].y = yAxis;
        flpBlockData[i].posSeq_Location = i;
        flpBlockData[i].negSeq_Location = i;
        xAxis = xAxis + blockData[i].width;
        //yAxis = yAxis + blockData[i].height;
    }
    return flpBlockData;
}

double maxBlockTemp(double *steady_temp)
{
    double maxBlockTemp=0;
    for(int i=0;i<NUM_BLOCKS;i++)
        if(steady_temp[i]>maxBlockTemp)maxBlockTemp=steady_temp[i];
    return maxBlockTemp;
}


/*function definitions */
void wLCS(flpBlock *S1,flpBlock *S2,double *weights,double *pos)
{
    double lengths[NUM_BLOCKS],t_span;
    flpBlock tempBlock;
    int index;
    for(int i=0;i<NUM_BLOCKS;i++)
        lengths[i]=0;
    for(int i=0;i<NUM_BLOCKS;i++)
    {
        tempBlock = S1[i];//tempBlock.print();
        index = tempBlock.negSeq_Location; //S2 Index for same block
        pos[tempBlock.blockNo] = lengths[index];
        t_span = pos[tempBlock.blockNo] + weights[tempBlock.blockNo];
        for(int j=index;j<NUM_BLOCKS;j++)
        {
            if(t_span > lengths[j]){
                lengths[j]=t_span;
            }
            else{
                break;
            }
        }
    }
    //return seqPair;
}

//sequencePair *flpToSequencePair(flpBlock *flpBlockData,sequencePair *seqPair){
//void flpToSequencePair(flpBlock *flpBlockData,sequencePair *seqPair)
flpBlock *flpToSequencePair(flpBlock *flpBlockData,sequencePair *seqPair)
{
    //sequencePair seqPair;
    int a[NUM_BLOCKS],b[NUM_BLOCKS];
    //cout<<"\n Random Sequence: "<<endl;
    for(int i=0;i<NUM_BLOCKS;i++){
        a[i]=i;
        b[i]=i;
    }
    random_shuffle(&a[0], &a[NUM_BLOCKS]);
    random_shuffle(&b[0], &b[NUM_BLOCKS]);
    /*cout<<"\n Random Sequence: "<<endl;
    for(int i=0;i<NUM_BLOCKS;i++)
        cout<<" "<<a[i];
    cout<<endl;
    for(int i=0;i<NUM_BLOCKS;i++)
        cout<<" "<<b[i];
    cout<<endl;*/
    for(int i=0;i<NUM_BLOCKS;i++){
        /*
        flpBlockData[i].posSeq_Location = i;
        seqPair->posSeq[i] = flpBlockData[i];
        seqPair->posSeq[i].posSeq_Location = i;

        flpBlockData[NUM_BLOCKS-1-i].negSeq_Location = i;
        seqPair->negSeq[i] = flpBlockData[NUM_BLOCKS-1-i];
        seqPair->negSeq[i].negSeq_Location = i;
        */
        flpBlockData[a[i]].posSeq_Location = i;
        seqPair->posSeq[i] = flpBlockData[a[i]];
        seqPair->posSeq[i].posSeq_Location = i;

        flpBlockData[b[i]].negSeq_Location = i;
        seqPair->negSeq[i] = flpBlockData[b[i]];
        seqPair->negSeq[i].negSeq_Location = i;
    }
    return flpBlockData;
}


flpBlock *randomMove(flpBlock *flpBlockData,sequencePair *seqPair)
{
    int a[NUM_BLOCKS],b[NUM_BLOCKS],c[3];
    for(int i=0;i<NUM_BLOCKS;i++){
        a[i]=i;
        b[i]=i;
    }
    for(int i=0;i<3;i++)c[i]=i;
    random_shuffle(&a[0], &a[NUM_BLOCKS]);
    random_shuffle(&b[0], &b[NUM_BLOCKS]);
    random_shuffle(&c[0],&c[3])
    int x = a[0];
    int y = b[0];
    for(int i=0;i<NUM_BLOCKS;i++){
        /*
        flpBlockData[i].posSeq_Location = i;
        seqPair->posSeq[i] = flpBlockData[i];
        seqPair->posSeq[i].posSeq_Location = i;

        flpBlockData[NUM_BLOCKS-1-i].negSeq_Location = i;
        seqPair->negSeq[i] = flpBlockData[NUM_BLOCKS-1-i];
        seqPair->negSeq[i].negSeq_Location = i;
        */
        flpBlockData[a[i]].posSeq_Location = i;
        seqPair->posSeq[i] = flpBlockData[a[i]];
        seqPair->posSeq[i].posSeq_Location = i;

        flpBlockData[b[i]].negSeq_Location = i;
        seqPair->negSeq[i] = flpBlockData[b[i]];
        seqPair->negSeq[i].negSeq_Location = i;
    }
    return flpBlockData;
}


flpBlock *sequencePairToFloorplan(sequencePair *seqPair,flpBlock *flpBlockData){
    //for(int i=0;i<NUM_BLOCKS;i++)
    double x_pos[NUM_BLOCKS],y_pos[NUM_BLOCKS],weights[NUM_BLOCKS];
    double W=0,H=0;
    flpBlock revPosSeq[NUM_BLOCKS];
    //
    for(int i=0;i<NUM_BLOCKS;i++)
        weights[i] = flpBlockData[i].width;

    wLCS(seqPair->posSeq,seqPair->negSeq,weights,x_pos); /* use 1 for x - co-ordinate */
    for(int i=0;i<NUM_BLOCKS;i++)
        weights[i] = flpBlockData[i].height;

    for(int i=0;i<NUM_BLOCKS;i++)
    {
         revPosSeq[NUM_BLOCKS-1-i]=seqPair->posSeq[i];
    }

    wLCS(revPosSeq,seqPair->negSeq,weights,y_pos); /* use for y co-ordinate */

    for(int i=0;i<NUM_BLOCKS;i++)
    {
        W += x_pos[i];
        H += y_pos[i];
    }

    int index_X,index_Y;
    for(int i=0;i<NUM_BLOCKS;i++)
    {
        flpBlockData[i].x = x_pos[i];
        flpBlockData[i].y = y_pos[i];
    }
    return flpBlockData;
}




/*
horGraph *horGraphCons(sequencePair seqPair){
    spBlock pBlock;
    spBlock nBlock;
    int j=0;
    for(int i=0;i<NUM_BLOCKS;i++)
    {
        pBlock = seqPair.posSeq[i];
        nBlock = seqPair.negSeq[i];
        while(pBlock.blockNo != nBlock.blockNo){
            if(nBlock.posSeq_Location < pBlock.posSeq_Location){
                // Add to horizontal graph (nBlock);
            }
            j++;
            nBlock = seqPair.negSeq[j];
        }
    }

}



verGraph *verGraphCons(){
    spBlock pBlock;
    spBlock nBlock;
    int j=0;
    for(int i=0;i<NUM_BLOCKS;i++)
    {
        pBlock = seqPair.posSeq[i];
        nBlock = seqPair.negSeq[i];
        while(pBlock.posSeq_Location != nBlock.posSeq_Location){
            if(nBlock.posSeq_Location > pBlock.posSeq_Location){
                // Add to horizontal graph (nBlock);
            }
            j++;
            nBlock = seqPair.negSeq[j];
        }
    }

}*/

#endif // FLOORPLAN_H_INCLUDED
