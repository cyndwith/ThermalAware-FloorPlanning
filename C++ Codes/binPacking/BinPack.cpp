#include "MaxRectsBinPack.h"
#include <cstdio>
#include <fstream>
#include <iostream>

using namespace std;

int noBlocks;
float Gamma;
float *height,*width;
float x_pos[30],y_pos[30];



char inputFile[] = "bin/Debug/input.txt";
char outputFile[] = "bin/Debug/output.txt";
float *readHeight(char fileName[],float *height,float *width)
{
  int i=0;double w,h,p;
  ifstream infile(fileName);
  if(!infile)
        cout<<"cannot open file!"<<endl;
  infile>>noBlocks>>Gamma;
  cout<<"noBlocks:"<<noBlocks<<endl;
  cout<<"Gamma:"<<Gamma<<endl;
  height =(float *)malloc(noBlocks*sizeof(float));
  width =(float *)malloc(noBlocks*sizeof(float));
  while(i<noBlocks)   /* Display the block parameters */
  {
      infile>>w>>h>>p;
      /* Assign data values to blocks */
      height[i] = h;
      width[i] = w;
      //cout<<"Block:"<<(i+1)<<"\t Width:"<<w<<"\t Height:"<<h<<"\t Power:"<<p<<endl;
      i++;
  }
  infile.close();
  cout<<"Read File called !!"<<endl;
  return width;
}

float *readWidth(char fileName[],float *height,float *width)
{
  int i=0;double w,h,p;
  ifstream infile(fileName);
  if(!infile)
        cout<<"cannot open file!"<<endl;
  infile>>noBlocks>>Gamma;
  cout<<"noBlocks:"<<noBlocks<<endl;
  cout<<"Gamma:"<<Gamma<<endl;
  height =(float *)malloc(noBlocks*sizeof(float));
  width =(float *)malloc(noBlocks*sizeof(float));
  while(i<noBlocks)   /* Display the block parameters */
  {
      infile>>w>>h>>p;
      /* Assign data values to blocks */
      height[i] = h;
      width[i] = w;
      //cout<<"Block:"<<(i+1)<<"\t Width:"<<w<<"\t Height:"<<h<<"\t Power:"<<p<<endl;
      i++;
  }
  infile.close();
  cout<<"Read File called !!"<<endl;
  return height;
}

void writeFLP(char fileName[])
{
  ofstream myfile (fileName);
  if (myfile.is_open())
  {
      for(int i=0;i<noBlocks;i++){
            myfile <<"Block-"<<i;//flpBlockData[i].blockNo;
            myfile <<"\t"<<width[i];//flpBlockData[i].width;
            myfile <<"\t"<<height[i];//flpBlockData[i].height;
            myfile <<"\t"<<x_pos[i];//flpBlockData[i].x;
            myfile <<"\t"<<y_pos[i];//flpBlockData[i].y;
            myfile <<"\n";
      }
    myfile.close();
  }
  else cout << "Unable to open file";
}




int main(int argc, char **argv)
{
    /*
	if (argc < 5 || argc % 2 != 1)
	{
		printf("Usage: MaxRectsBinPackTest binWidth binHeight w_0 h_0 w_1 h_1 w_2 h_2 ... w_n h_n\n");
		printf("       where binWidth and binHeight define the size of the bin.\n");
		printf("       w_i is the width of the i'th rectangle to pack, and h_i the height.\n");
		printf("Example: MaxRectsBinPackTest 256 256 30 20 50 20 10 80 90 20\n");
		return 0;
	}
    */
	using namespace rbp;
    //float *height,*width;
    height = readHeight(inputFile,height,width);
    width = readWidth(inputFile,height,width);
	// Create a bin to pack to, use the bin size from command line.
	MaxRectsBinPack bin;
	float binWidth=0; // = atoi(argv[1]);
	float binHeight=0;// = atoi(argv[2]);

	for(int i=0;i<noBlocks;i++){
        binWidth += width[i]*1E9;
        binHeight += height[i]*1E9;
	}
    binWidth = binWidth + 0.2*binWidth;
    binHeight = binHeight + 0.2*binHeight;

    for(int i=0;i<noBlocks;i++){
        width[i]*=1E6;
        height[i]*=1E6;
        //cout<<" "<<width[i]*1E9;
        //cout<<" "<<height[i]*1E9<<endl;
	}
	cout<<endl;
	cout<<"Initial bin size : "<<binWidth<<","<<binHeight<<endl;
	//printf("Initializing bin to size %fx%f.\n", binWidth, binHeight);
	bin.Init(binWidth, binHeight);

	// Pack each rectangle (w_i, h_i) the user inputted on the command line.
	int rectWidth; //height[0]; //atoi(argv[i]);
    int rectHeight; // = width[0]; //atoi(argv[i+1]);
    //int x_pos[30],y_pos[30];
	for(int i = 0; i < noBlocks; i ++)
	{
		// Read next rectangle to pack.
		rectWidth = width[i];
		rectHeight = height[i];
		//int rectHeight = atoi(argv[i+1]);
		//printf("Packing rectangle of size %dx%d: ", rectWidth, rectHeight);

		// Perform the packing.
		//MaxRectsBinPack::FreeRectChoiceHeuristic heuristic = MaxRectsBinPack::RectBestShortSideFit; // This can be changed individually even for each rectangle packed.
		MaxRectsBinPack::FreeRectChoiceHeuristic heuristic = MaxRectsBinPack::RectContactPointRule;
		//Rect packedRect = bin.Insert(rectWidth, rectHeight, heuristic);
		Rect packedRect = bin.Insert(rectWidth, rectHeight, heuristic);

        x_pos[i] = packedRect.x;
        y_pos[i] = packedRect.y;
		// Test success or failure.
		if (packedRect.height > 0)
			printf("Packed to (x,y)=(%d,%d), (w,h)=(%d,%d). Free space left: %.2f%%\n", packedRect.x, packedRect.y, packedRect.width, packedRect.height, 100.f - bin.Occupancy()*100.f);
		else
			printf("Failed! Could not find a proper position to pack this rectangle into. Skipping this one.\n");
	}
	writeFLP(outputFile);
	printf("Done. All rectangles packed.\n");
}
