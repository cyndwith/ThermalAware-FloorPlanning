#ifndef IOFILE_H_INCLUDED
#define IOFILE_H_INCLUDED

#include <fstream>
#include <iostream>

/* C++ - libraries */
#include "floorplan.h"
using namespace std;

block *readBlocks(char fileName[],block *blockData);
void readFloorPlan(char fileName[]);


block  *readBlocks(char fileName[],block *blockData)
{
  int i=0;
  double w,h,p;
  ifstream infile(fileName);
  if(!infile)
        cout<<"cannot open file!"<<endl;
  infile>>noBlocks;
  infile>>Gamma;
  blockData=(block *)malloc(noBlocks*sizeof(block));
  //cout<<"Block:"<<noBlocks<<endl;//cout<<"Gamma:"<<Gamma<<endl;
  /* Display the block parameters */
  while(i<noBlocks)
  {
      infile>>w>>h>>p;
      /* Assign data values to blocks */
      blockData[i].blockNo = i;
      blockData[i].width = w;
      blockData[i].height = h;
      blockData[i].power = p;
      //cout<<"Block:"<<(i+1)<<"\t Width:"<<w<<"\t Height:"<<h<<"\t Power:"<<p<<endl;
      i++;
  }
  while(i<noBlocks)
  {
      //infile>>w>>h>>p;
      /* Assign data values to blocks */
      cout<<blockData[i].blockNo<<endl;
      cout<<blockData[i].width<<endl;
      cout<<blockData[i].height<<endl;
      cout<<blockData[i].power<<endl;
      //cout<<"Block:"<<(i+1)<<"\t Width:"<<w<<"\t Height:"<<h<<"\t Power:"<<p<<endl;
      i++;
  }
  infile.close();
  return blockData;
}




void writeFloorPlan(char fileName[],flpBlock *flpBlockData)
{
  ofstream myfile (fileName);
  if (myfile.is_open())
  {
      for(int i=0;i<noBlocks;i++){
            myfile <<"Block-"<<flpBlockData[i].blockNo;
            myfile <<"\t"<<flpBlockData[i].width;
            myfile <<"\t"<<flpBlockData[i].height;
            myfile <<"\t"<<flpBlockData[i].x;
            myfile <<"\t"<<flpBlockData[i].y;
            myfile <<"\n";
      }
    myfile.close();
  }
  else cout << "Unable to open file";
}

void writePower(char fileName[],flpBlock *flpBlockData)
{
  ofstream myfile (fileName);
  if (myfile.is_open())
  {
      for(int i=0;i<noBlocks;i++){
            myfile <<"Block-"<<flpBlockData[i].blockNo;
            myfile <<"\t"<<flpBlockData[i].power;
            myfile <<"\n";
      }
    myfile.close();
  }
  else cout << "Unable to open file";
}

void writeSeqPair(char fileName[],sequencePair *seqPair)
{
  ofstream myfile (fileName);
  if (myfile.is_open())
  {
      for(int i=0;i<noBlocks;i++){
            myfile<<"\t"<<seqPair->posSeq[i].blockNo;
            myfile<<"\t"<<seqPair->negSeq[i].blockNo;
            myfile <<"\n";
      }
    myfile.close();
  }
  else cout << "Unable to open file";
}

void readSeqPair(char fileName[],flpBlock *flpBlockData,sequencePair *seqPair)
{
  ifstream infile(fileName);
  if(!infile)
        cout<<"cannot open file!"<<endl;
  int pos,neg,i;
  cout<<"\nRead results pos :"<<endl;
  if (infile.is_open())
  {
      i=0;
      for(int i=0;i<NUM_BLOCKS;i++)
      {
          infile>>pos>>neg;
          seqPair->posSeq[i] = flpBlockData[pos];
          flpBlockData[pos].posSeq_Location = i;
          seqPair->negSeq[i] = flpBlockData[neg];
          flpBlockData[neg].negSeq_Location = i;
          cout<<" "<<pos;
      }
      infile.close();
  }
}

#endif // IOFILE_H_INCLUDED
