#ifndef FRAME_H_INCLUDED
#define FRAME_H_INCLUDED

#include <windows.h>

class Frame
{
public:
	RECT getFrame() const;

protected:
	RECT frame;
};

class FirstQuadrant: public Frame
{
	public: FirstQuadrant(const RECT& window);
};

class SecondQuadrant: public Frame
{
	public: SecondQuadrant(const RECT& window);
};

class ThirdQuadrant: public Frame
{
	public: ThirdQuadrant(const RECT& window);
};

class FourthQuadrant: public Frame
{
	public: FourthQuadrant(const RECT& window);
};

class FirstAndSecondQuadrant: public Frame
{
	public: FirstAndSecondQuadrant(const RECT& window);
};

class ThirdAndFourthQuadrant: public Frame
{
	public: ThirdAndFourthQuadrant(const RECT& window);
};

class FirstAndThirdQuadrant: public Frame
{
	public: FirstAndThirdQuadrant(const RECT& window);
};

class SecondAndFourthQuadrant: public Frame
{
	public: SecondAndFourthQuadrant(const RECT& window);
};

class AllQuadrants: public Frame
{ 
	public: AllQuadrants(const RECT& window);
};

#endif // FRAME_H_INCLUDED
