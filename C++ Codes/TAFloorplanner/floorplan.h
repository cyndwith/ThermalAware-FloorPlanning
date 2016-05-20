#ifndef FLOORPLAN_H_INCLUDED
#define FLOORPLAN_H_INCLUDED

#include "sequencePair.h"
#include <iostream>
using namespace std;

int noBlocks;
double Gamma,flpH,flpW;
double maxBlockTempValue=0;


class block{
public:
    int blockNo;
    double width,height,power;

    void print(){
        //cout<<"Block Parameter:"<<endl;
        cout<<"block No :"<<blockNo;
        cout<<"\t\twidth :"<<width;
        cout<<"\t\theight :"<<height;
        cout<<"\t\tpower :"<<power<<endl;
    }
};

class flpBlock: public block
{
public:
    int posSeq_Location,negSeq_Location;
    double x,y;
    void print(){
        //cout<<"Block Parameter: ";
        cout<<"Block# :"<<blockNo;
        //cout<<"\twidth :"<<width;
        //cout<<"\theight :"<<height;
        //cout<<"\t\tpower :"<<power;
        cout<<"\t\tx :"<<x;
        cout<<"\t\ty :"<<y;
        cout<<"\t\tposSeqInd :"<<posSeq_Location;
        cout<<"\t\tnegSeqInd :"<<negSeq_Location<<endl;
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
    void update(flpBlock *flpBlockData)
    {
        for(int i=0;i<NUM_BLOCKS;i++)
        {
            posSeq[flpBlockData[i].posSeq_Location] = flpBlockData[i];
            negSeq[flpBlockData[i].negSeq_Location] = flpBlockData[i];
        }
    }
};


int myrandom (int i) { return std::rand()%i;}


flpBlock *initFloorplan(block *blockData,sequencePair *seqPair)
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
    //sequencePair seqPair;
    //int a[NUM_BLOCKS],b[NUM_BLOCKS];
    /* original sequence pair */
    int a[] = {0,2,3,4,5,6,7,8,9,10,26,21,28,29,27,22,13,14,19,17,11,15,16,20,18,12,25,23,24,1};
    int b[] = {1,0,2,21,26,22,27,28,29,11,17,19,13,14,12,18,20,15,16,23,24,25,5,6,7,8,9,10,3,4};
    /* Modified sequence pair */
    //int a[] = {15,16,20,24,9,0,19,13,14,23,10,2,3,4,5,6,7,8,26,21,28,29,27,22,17,11,18,12,25,1};
    //int b[] = {1,0,2,21,26,22,27,28,29,11,17,12,18,25,5,6,7,8,3,4,20,24,9,15,16,19,23,13,10,14};
    //int a[] = {23,24,0,2,3,4,5,6,7,8,9,10,26,21,28,29,27,22,13,14,19,17,11,15,16,20,18,12,25,1};
    //int b[] = {1,0,2,21,26,22,27,28,29,11,17,19,13,14,12,18,20,15,16,25,5,6,7,8,9,10,3,4,23,24};
    //cout<<"\n Random Sequence: "<<endl;
    /*for(int i=0;i<NUM_BLOCKS;i++){
        a[i]=i;
        b[i]=i;
    }*/

    //random_shuffle(&a[0], &a[NUM_BLOCKS]);
    //random_shuffle(&b[0], &b[NUM_BLOCKS]);
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
    //return flpBlockData;
    return flpBlockData;
}

double maxBlockTemp(double *steady_temp)
{
    maxBlockTempValue=0;
    for(int i=0;i<NUM_BLOCKS;i++)
        if(steady_temp[i]>maxBlockTempValue)maxBlockTempValue=steady_temp[i];
    return maxBlockTempValue;
}


/*function definitions */
double wLCS(flpBlock *S1,flpBlock *S2,double *weights,double *pos)
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
    flpW = lengths[NUM_BLOCKS-1];
    return flpW;
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
    int a[NUM_BLOCKS],b[NUM_BLOCKS],c,temp1,temp2;
    for(int i=0;i<NUM_BLOCKS;i++){
        a[i]=i;
        b[i]=i;
    }
    //for(int i=0;i<2;i++)c[i]=i;
    random_shuffle(&a[0], &a[NUM_BLOCKS]);
    random_shuffle(&b[0], &b[NUM_BLOCKS]);
    //random_shuffle(&c[0],&c[1]);
    int x,maxT=0;
    for(int i=0;i<NUM_BLOCKS;i++){
        if(maxT < steady_temp[i]){
            x = i;
            maxT = steady_temp[i];
            //cout<<"i: "<<x<<" : "<<steady_temp[i];
        }

    }
    //cout<<endl;
    //x = rand()%30;//a[0]; /* random number between 0-29 */
    int y = rand()%30;//b[0]; /* random number between 0-29 */
    c = rand()%3;
    /* based on case decide the move */
    //cout<<"Switch case: "<<a[0]<<" "<<b[0]<<" "<<c[0]<<endl;
    switch(c){
    case 0: {    /* swap positive sequence */
                temp1 = flpBlockData[x].posSeq_Location; //temp2 =  (temp1+1)%30;//flpBlockData[y].posSeq_Location;
                temp2 =  flpBlockData[y].posSeq_Location;
                flpBlockData[x].posSeq_Location = temp2;
                flpBlockData[seqPair->posSeq[temp2].blockNo].posSeq_Location = temp1;
                seqPair->posSeq[temp1] = flpBlockData[seqPair->posSeq[temp2].blockNo];
                seqPair->posSeq[temp2] = flpBlockData[x];
            }
            break;
    case 1: {
                // swap negative  sequence location
                temp1 = flpBlockData[x].negSeq_Location;//temp2 = (temp1+1)%30;//flpBlockData[y].posSeq_Location;
                temp2 = flpBlockData[y].posSeq_Location;
                flpBlockData[x].negSeq_Location = temp2;
                flpBlockData[seqPair->negSeq[temp2].blockNo].negSeq_Location = temp1;
                seqPair->negSeq[temp1] = flpBlockData[seqPair->negSeq[temp2].blockNo];
                seqPair->negSeq[temp2] = flpBlockData[x];
            }
            break;
    case 2: {   /* swap both positive and negative location */
                temp1 = flpBlockData[x].posSeq_Location;
                temp2 = flpBlockData[y].posSeq_Location;
                flpBlockData[x].posSeq_Location = temp2;
                flpBlockData[y].posSeq_Location = temp1;

                seqPair->posSeq[temp1] = flpBlockData[y];
                seqPair->posSeq[temp2] = flpBlockData[x];

                temp1 = flpBlockData[x].negSeq_Location;
                temp2 = flpBlockData[y].negSeq_Location;
                flpBlockData[x].negSeq_Location = temp2;
                flpBlockData[y].negSeq_Location = temp1;

                seqPair->negSeq[temp1] = flpBlockData[y];
                seqPair->negSeq[temp2] = flpBlockData[x];
            }
            break;
    default :   /* do nothing */
                break;
    }
    return flpBlockData;
}


flpBlock *sequencePairToFloorplan(sequencePair *seqPair,flpBlock *flpBlockData){
    //for(int i=0;i<NUM_BLOCKS;i++)
    cout<<"Initial Solution for Floorplan !"<<endl;
    double x_pos[NUM_BLOCKS],y_pos[NUM_BLOCKS],weights[NUM_BLOCKS];
    double W=0,H=0;
    flpBlock revPosSeq[NUM_BLOCKS];
    //
    for(int i=0;i<NUM_BLOCKS;i++)
        weights[i] = flpBlockData[i].width;

    flpW = wLCS(seqPair->posSeq,seqPair->negSeq,weights,x_pos); /* use 1 for x - co-ordinate */
    for(int i=0;i<NUM_BLOCKS;i++)
        weights[i] = flpBlockData[i].height;

    for(int i=0;i<NUM_BLOCKS;i++)
    {
         revPosSeq[NUM_BLOCKS-1-i]=seqPair->posSeq[i];
    }

    flpH = wLCS(revPosSeq,seqPair->negSeq,weights,y_pos); /* use for y co-ordinate */

    int index_X,index_Y;
    for(int i=0;i<NUM_BLOCKS;i++)
    {
        flpBlockData[i].x = x_pos[i];
        flpBlockData[i].y = y_pos[i];
    }
     cout<<"Initial Solution for Floorplan !"<<endl;
    return flpBlockData;
}


sequencePair *shelfAlgo(sequencePair *seqPair,float area){

    /* sort blocks by width */
    double maxWidth = sqrt(double(area);
    flpBlock tempBlock;
    float accWidth=0;
    for(int i=0;i<noBlocks;i++){
        accWidth+=seqPair->posSeq[i].width;
        if(accWidth > maxWidth ){
            accWidth = 0;
            /* (noBlocks - i) chunk to the left most */
            for(int k=i;k<noBlocks;k++){
                tempBlock = seqPair->posSeq[k];
                for(int j=k;j>1;j--){
                    seqPair->posSeq[j]=seqPair[j-1];
                }
                seqPair->posSeq[0] = tempBlock;
            }
        }
    }
    return seqPair;
}

#endif // FLOORPLAN_H_INCLUDED
