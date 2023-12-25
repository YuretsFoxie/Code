#include "model.h"

void Model::addObserver(ObserverInterface* observer)
{
	observers.push_back(observer);
}

void Model::removeObserver(ObserverInterface* observer)
{
	observers.remove(observer);
}

void Model::toggleShape()
{
	isTriangle = !isTriangle;
	notify( {getShapeName(), getShape()} );
}

void Model::notify(const Shape& shape)
{
	list<ObserverInterface *>::iterator iterator = observers.begin();
	
	for (; iterator != observers.end(); ++iterator)
		(*iterator)->update(shape);
}

string Model::getShapeName()
{
	return isTriangle ? "Triangle" : "Square";
}

vector<POINT> Model::getShape()
{
	return isTriangle ? triangle : square;
}
