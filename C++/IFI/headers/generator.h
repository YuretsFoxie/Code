#ifndef GENERATOR_H_INCLUDED
#define GENERATOR_H_INCLUDED

class Generator
{
public:
    static Generator& shared()
    {
        static Generator instance;
        return instance;
    }
	
	void generateTestValue();
	
private:
	int testValue = 0;
};

#endif // GENERATOR_H_INCLUDED
