#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
//#include "btree.h"
//#inlcude "IOFile.h"
//#include "floorplan.h"

using namespace std;
#define NUM_BLOCKS 30

int main() {
    char hotspot[] = "..\\HotSpot-5.02\\hotspot.exe -f ..\\HotSpot-5.02\\ev6.flp -p ..\\HotSpot-5.02\\gcc.ptrace";
    system(hotspot);
	return 0;
}
