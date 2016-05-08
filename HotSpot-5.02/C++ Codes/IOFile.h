#ifndef IOFILE_H_INCLUDED
#define IOFILE_H_INCLUDED

void readBlocks(char fileName[]);
void readFloorPlan(char fileName[]);


void readBlocks(char fileName[])
{
  int noBlocks,i=0;
  double w,h,p,ratioArea;
  ifstream infile(fileName);
  if(!infile)
        cout<<"cannot open file!"<<endl;
  infile>>noBlocks;
  infile>>ratioArea;
  cout<<"Block:"<<noBlocks<<endl;
  cout<<"Gamma:"<<ratioArea<<endl;
  while(i<noBlocks)
  {
      infile>>w>>h>>p;
      cout<<"Block:"<<(i+1)<<"\t Width:"<<w<<"\t Height:"<<h<<"\t Power:"<<p<<endl;
      i++;
  }
}


void readFloorPlan(char fileName[])
{
  int noBlocks,i=0;
  double w,h,p,ratioArea;
  ifstream infile(fileName);
  if(!infile)
        cout<<"cannot open file!"<<endl;
  infile>>noBlocks;
  infile>>ratioArea;
  cout<<"Block:"<<noBlocks<<endl;
  cout<<"Gamma:"<<ratioArea<<endl;
  while(i<noBlocks)
  {
      infile>>w>>h>>p;
      cout<<"Block:"<<(i+1)<<"\t Width:"<<w<<"\t Height:"<<h<<"\t Power:"<<p<<endl;
      i++;
  }
}


#endif // IOFILE_H_INCLUDED
