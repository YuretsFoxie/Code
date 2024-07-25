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
	
private:

};

#endif // GENERATOR_H_INCLUDED
