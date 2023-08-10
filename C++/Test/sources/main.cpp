#include <iostream>
#include <iomanip>
#include <chrono>

using namespace std;

double halfHeight = 506.5;
double dy = 3.87891;

int convertDataToScreenCoordinates(int number)
{
	return halfHeight - number * dy;
}

int convertScreenCoordinatesToData(int number)
{
	int result = (halfHeight - number) / dy;
	return result < 0 ? result - 1 : result;
}



int main()
{
	/*
	using milli = chrono::milliseconds;
	auto start = chrono::high_resolution_clock::now();
	
	// ...
	
	auto finish = chrono::high_resolution_clock::now();
	cout << chrono::duration_cast<milli>(finish - start).count() << " ms" << endl;
	*/
	
	for (int i = 127; i >= -128; i--)
	{
		int screenData = convertDataToScreenCoordinates(i);
		int backwardData = convertScreenCoordinatesToData(screenData);
		
		if (i != backwardData)
			cout << "Error!" << endl;
		
		cout << setw(4) << i << setw(5) << screenData << setw(5) << backwardData << endl;
	}
	
    cin.get();
    return 0;
}
