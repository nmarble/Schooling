#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <algorithm>
#include <vector>
#include <chrono>

using namespace std;

//Function to create a random array with given size
vector<int> getRandomArray(int size) {

	vector<int> thisArray(size); //Holds all the values
	
	for (int i = 0; i < size; i++) {
		thisArray[i] = rand() % 100000 + 1; //Gets a random number between 1 and 100000
	}
	
	sort(thisArray.begin(), thisArray.end()); //Sort the array
	
	return thisArray;
}

//Recursive function to search through the array using the Ternary method
bool ternarySearch (vector<int> thisArray, int start, int end, int findMe) {

	int firstBreak = ((end - start) / 3) + (start); //Holds the position of the 1/3 way point
	int secondBreak = (((end - start) / 3) * 2) + (start);//Holds the position of the 2/3 way point
	int firstValue = thisArray.at(firstBreak); //Holds the value of the 1/3 way point
	int secondValue = thisArray.at(secondBreak); //Holds the value of the 2/3 way point
	
	//Check for values for a match
	if (findMe == firstValue) return true;
	if (findMe == secondValue) return true;
	
	//Checks if it is at the end
	if (firstValue == secondValue) return false;
	
	//Finds the proper location to search next
	if (findMe < firstValue) {
		ternarySearch(thisArray, start, firstBreak, findMe);
	}else if (findMe > firstValue && findMe < secondValue){
		ternarySearch(thisArray, (firstBreak), (secondBreak), findMe);
	}else if (findMe > secondValue){
		ternarySearch(thisArray, (secondBreak), (end), findMe);
	}
	return false;
}

//Main function for all the tests
int main() {
	srand (time(NULL)); //initialize random seed
	
	//Loop from 5000 to 100000 in 500 increments
	for (int currentLength = 5000; currentLength <= 100000; currentLength += 5000){
		chrono::duration<double> average; //Holds the current iterations average
		
		//runs test 100 times
		for (int index = 0; index < 100; index++){
			int toFind = rand() % 100000 + 1; //Holds the random value to search for
			vector<int> testArray(currentLength); //holds all the values of the testing array
			testArray = getRandomArray(currentLength); //Fills the random array
			
			//Set the start time
			chrono::high_resolution_clock::time_point t1 = chrono::high_resolution_clock::now();
			
			//THIS IS THE RECURSIVE TEST
			ternarySearch(testArray, 0, testArray.size(), toFind);
			
			//Set end time
			chrono::high_resolution_clock::time_point t2 = chrono::high_resolution_clock::now();
			
			//Gets difference and adds to the average
			chrono::duration<double> time_span = chrono::duration_cast<chrono::duration<double>>(t2-t1);
			average = average + time_span;
		}
		average = average / 100; //Complete the averaging calculations
		cout << "average for time: " << currentLength << " = "<< average.count() << endl;
	}
	return 0;
}
