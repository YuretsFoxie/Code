#include <iostream>
#include <iomanip>

#include "gplus.h"
#include "image.h"
#include "network.h"
#include "profiler.h"

using namespace std;

void printDigit(const int n)
{
	Image image = Image("images/" + to_string(n) + ".png");
	vector<int> data = image.getData();
	
	int k = 0;
	for (int i = 0; i < image.getHeight(); i++)
	{
		for (int j = 0; j < image.getWidth(); j++)
			cout << setw(3) << data[k++] << " ";
		
		cout << endl;
	}
	
	cout << endl << endl;
}

int main()
{
	GDIPlus gdiPlus;
	gdiPlus.start();
	Profiler profiler;
	
	for (int i = 0; i <= 9; i++)
		printDigit(i);
	
	gdiPlus.stop();
	cin.get();
	return 0;
}
