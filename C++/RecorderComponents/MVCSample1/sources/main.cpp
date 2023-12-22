#include <iostream>
#include <string>

using namespace std;



class Model
{
public:
	Model() { cout << "Model constructed" << endl; }
	~Model() { cout << "Model destructed" << endl; }
	
	void save(int n) { number = n; }
	int load() { return number; }
	
private:
	int number;
};



class View
{
public:
	View() { cout << "View constructed" << endl; }	
	~View() { cout << "View destructed" << endl; }
	
	void print(const int number) { cout << number << endl; }
	void print(const string& text) { cout << text << endl; }
	
	int readNumber()
	{
		int number;
		cin >> number;
		return number;
	}
};



class Controller
{
public:
	Controller()
	{
		cout << "Controller constructed" << endl << endl;
		performActions();
	}
	
	~Controller() { cout << "Controller destructed" << endl; }
	
private:
	void performActions()
	{
		view.print("enter the number:");
		model.save( view.readNumber() );
		
		view.print("the square is:");
		int number = model.load();
		view.print(number * number);
		view.print("");
	}
	
	Model model = Model();
	View view = View();
};



int main()
{
	Controller();
	
	cin.get();
	cin.get();
	return 0;
}
