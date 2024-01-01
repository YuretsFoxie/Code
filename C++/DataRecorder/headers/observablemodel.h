#ifndef MODEL_H_INCLUDED
#define MODEL_H_INCLUDED

#include <string>
#include <list>

using namespace std;

class ObserverInterface
{
public:
	virtual ~ObserverInterface() {};
	virtual void update(const string& message) = 0;
	virtual void update(const int value) = 0;
};



class SubjectInterface
{
public:
	virtual ~SubjectInterface() {};
	virtual void addObserver(ObserverInterface* observer) = 0;
	virtual void removeObserver(ObserverInterface* observer) = 0;
	
protected:
	virtual void notify(const string& message) = 0;
	virtual void notify(const int value) = 0;
};



class ObservableModel: public SubjectInterface
{
public:
	void addObserver(ObserverInterface* observer) override;
	void removeObserver(ObserverInterface* observer) override;
	
protected:
	void notify(const string& message) override;
	void notify(const int value) override;
	
	list<ObserverInterface *> observers;
};

#endif // MODEL_H_INCLUDED
