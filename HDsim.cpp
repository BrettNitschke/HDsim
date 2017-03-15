// Author: Brett Nitschke
// 10 March 2017
#include <iostream>
#include <math.h>
#include <cstdlib>
#include <time.h>
#include <vector>
#include <limits.h>
#include <iomanip>
using namespace std;


//Quantom Atlas III parameters

static const int xMax = 8057; //cylinders
static const double C = 9.1;  //GB
static const int N = 7200;  //rpm
static const int xStar = 1686; 
static const double t = 1.5455; //ms
static const double c = 0.3197; //ms
static const double r = 0.3868;
static const int numOfDiskAccesses = 10000;


static int cylArray[xMax];

int getRandomCylinder(){
	static int cyl = 0;

	cyl = (5*cyl + 1699) % xMax;

	return cyl;
}

 void fillCylinderArray(){

	for (int i =0; i< xMax; i++){
		cylArray[i] = getRandomCylinder();
	}
}

int getRandomArrayPosition(){
	return rand() % xMax;
}

vector<double> Sd(int Q){

	//result vector
	vector<double> seekArray;
	double totalSeekTime = 0;
	double totalSeekDistance = 0;
	int currentCylinderIndex;
	
	int seekDistance;
	double seekTime;

	//intialize the starting cylinder
	int currentCylinder = cylArray[getRandomArrayPosition()];

	int diskQueue[Q];
	//fill the Queue w/ random cylinders
	for (int i=0; i< Q; i++){
		diskQueue[i] = cylArray[getRandomArrayPosition()];
		
	}
	
	for (int j = 0; j< numOfDiskAccesses; j++){
		double x = INT_MAX;

		for (int k =0; k<Q; k++){
			
			int temp = abs(currentCylinder - diskQueue[k]);
			//if (temp == 0) temp = 1;
			if (temp < x){
				x = temp;
				currentCylinderIndex = k;
			} 
		}

		//compute seek distance from initial cylinder to closest cylinder
		seekDistance = abs(currentCylinder - diskQueue[currentCylinderIndex]);

		//compute seek time
		if (x >= 1 && x < xStar){
			seekTime = (t + c * pow((x - 1), r)); 
		} else if ( x > xStar) {
			seekTime = ( (c * r * (x - xStar)/(pow((xStar - 1.0), (1.0 - r)))) + t + c * pow((xStar - 1.0), r));
		}

		//update current cylinder and replace current cylinder in queue
		currentCylinder = diskQueue[currentCylinderIndex];
		diskQueue[currentCylinderIndex] = cylArray[getRandomArrayPosition()];

		totalSeekDistance += seekDistance;
		totalSeekTime += seekTime;
	}

	double avgSeekDistance = totalSeekDistance / numOfDiskAccesses;
	double avgSeekTime = totalSeekTime / numOfDiskAccesses;

	seekArray.push_back(avgSeekDistance);
	seekArray.push_back(avgSeekTime);
	return seekArray;
}


int main(){

	//initialize random seed
	srand (time(NULL)); 

	//fill cyl array with uniform dist random numbers
	fillCylinderArray();

	
	
	for (int i = 1; i<=20; i++){
		vector<double> results = Sd(i);
		cout << "for " <<setw(2) << i << " in queue  --  ";
		cout << "average seek distance = " << setw(7) << results[0] << "    ";
		cout << "average seek time = " << results[1] << endl;

		
		
	}
	
	
	
}