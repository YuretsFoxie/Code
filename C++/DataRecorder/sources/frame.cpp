#include "frame.h"

RECT Frame::getFrame() const
{
	return frame;
}

FirstQuadrant::FirstQuadrant(const RECT& window)
{
	frame = {0, 0, window.right / 2, window.bottom / 2};
}

SecondQuadrant::SecondQuadrant(const RECT& window)
{
	frame = {window.right / 2, 0, window.right, window.bottom / 2};
}

ThirdQuadrant::ThirdQuadrant(const RECT& window)
{
	frame = {0, window.bottom / 2, window.right / 2, window.bottom};
}

FourthQuadrant::FourthQuadrant(const RECT& window)
{
	frame = {window.right / 2, window.bottom / 2, window.right, window.bottom};
}

FirstAndSecondQuadrant::FirstAndSecondQuadrant(const RECT& window)
{
	frame = {0, 0, window.right, window.bottom / 2};
}

ThirdAndFourthQuadrant::ThirdAndFourthQuadrant(const RECT& window)
{
	frame = {0, window.bottom / 2, window.right, window.bottom};
}

FirstAndThirdQuadrant::FirstAndThirdQuadrant(const RECT& window)
{
	frame = {0, 0, window.right / 2, window.bottom};
}

SecondAndFourthQuadrant::SecondAndFourthQuadrant(const RECT& window)
{
	frame = {window.right / 2, 0, window.right, window.bottom};
}

AllQuadrants::AllQuadrants(const RECT& window)
{
	frame = {0, 0, window.right, window.bottom};
}
