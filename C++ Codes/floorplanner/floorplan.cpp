#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <vector>

#include "flp.h"
#include "package.h"
#include "temperature.h"
#include "temperature_block.h"
#include "temperature_grid.h"
#include "util.h"
#include "hotspot.h"

using namespace std;
#define NUM_BLOCKS 30

int main(int argc, char **argv) {
    double steady_temp[30];
    //int X = hotspot(argc,argv);
    char hotspotCall[] = "..\\HotSpot-5.02\\hotspot.exe -f ..\\HotSpot-5.02\\ev6.flp -p ..\\HotSpot-5.02\\gcc.ptrace";
    system(hotspotCall);
    //cin>>X;
//    cout<<"X is:"<<X<<endl;
	return 0;
}
