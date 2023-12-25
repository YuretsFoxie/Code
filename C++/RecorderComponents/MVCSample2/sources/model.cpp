#include "model.h"

void Model::addObserver(ObserverInterface* observer)
{
	observers.push_back(observer);
}

void Model::removeObserver(ObserverInterface* observer)
{
	observers.remove(observer);
}

void Model::notify(const Shape& shape)
{
	list<ObserverInterface *>::iterator iterator = observers.begin();
	
	while (iterator != observers.end())
	{
		(*iterator)->update(shape);
		++iterator;
	}
}

void Model::toggleShape()
{
	isTriangle = !isTriangle;
	isTriangle ? notify( {"Triangle", triangle} ) : notify( {"Square", square} );
}

string Model::getShapeName()
{
	return isTriangle ? "Triangle" : "Square";
}

vector<POINT> Model::getShape()
{
	return isTriangle ? triangle : square;
}
