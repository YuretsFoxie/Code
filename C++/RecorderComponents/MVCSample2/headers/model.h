#ifndef MODEL_H_INCLUDED
#define MODEL_H_INCLUDED

#include <windows.h>
#include <string>
#include <vector>
#include <list>

using namespace std;

struct Shape
{
	string name;
	vector<POINT> points;
};



class ObserverInterface
{
public:
	virtual ~ObserverInterface() {};
	virtual void update(const Shape& shape) = 0;
};



class SubjectInterface
{
public:
	virtual ~SubjectInterface() {};
	virtual void addObserver(ObserverInterface* observer) = 0;
	virtual void removeObserver(ObserverInterface* observer) = 0;
	virtual void notify(const Shape& shape) = 0;
};



class Model: public SubjectInterface
{
public:
	virtual ~Model() {}
	
	void addObserver(ObserverInterface* observer) override;
	void removeObserver(ObserverInterface* observer) override;
	void notify(const Shape& shape) override;
	
	void toggleShape();
	string getShapeName();
	vector<POINT> getShape();
	
private:
	const vector<POINT> triangle = {{100, 300}, {300, 300}, {200, 100}, {100, 300}};
	const vector<POINT> square = {{50, 50}, {250, 50}, {250, 250}, {50, 250}, {50, 50}};
	
	list<ObserverInterface *> observers;
	bool isTriangle = true;
};

#endif // MODEL_H_INCLUDED
