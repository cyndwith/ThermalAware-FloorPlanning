#include <iostream>
#include <fstream>
#include "btree.h"
#inlcude "IOFile.h"
#include "floorplan.h"

using namespace std;
#define NUM_BLOCKS 30

int main() {
	// read input file
	int noBlocks;
	double gamma,blockPara[30][3];
	cin>>noBlocks;
	cin>>gamma;
	cout<<"noBlock:"<<noBlocks<<endl;
	cout<<"Area Slack:"<<gamma<<endl;
	// Read block parameters
	for(int i=0;i<noBlocks;i++)
	{
	    cin>>blockPara[i][1];
	    cin>>blockPara[i][2];
	    cin>>blockPara[i][3];
	}
	// Display the file
	for(int i=0;i<noBlocks;i++)
	{
	    cout<<"BlockNo:"<<i<<" Width:"<<blockPara[i][1];
	    cout<<" Height:"<<blockPara[i][2];
	    cout<<" Power:"<<blockPara[i][3]<<endl;
	}

	return 0;
}
