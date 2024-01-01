#include "observablemodel.h"

void ObservableModel::addObserver(ObserverInterface* observer)
{
	observers.push_back(observer);
}

void ObservableModel::removeObserver(ObserverInterface* observer)
{
	observers.remove(observer);
}

void ObservableModel::notify(const string& message)
{
	list<ObserverInterface *>::iterator iterator = observers.begin();
	
	for (; iterator != observers.end(); ++iterator)
		(*iterator)->update(message);
}

void ObservableModel::notify(const int value)
{
	list<ObserverInterface *>::iterator iterator = observers.begin();
	
	for (; iterator != observers.end(); ++iterator)
		(*iterator)->update(value);
}
