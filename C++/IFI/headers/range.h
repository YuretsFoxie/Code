#ifndef RANGE_H_INCLUDED
#define RANGE_H_INCLUDED

template <typename T>
class Range
{
public:
	Range(): min(0), max(0) {}
	Range(T min, T max): min(min), max(max) {}
	
	T range() const
	{
		return max - min;
	}
	
	T convertValueToNewRange(const T value, const Range& newRange) const
	{
		return (((value - this->min) * newRange.range()) / this->range()) + newRange.min;
	}
	
	T getMin() const
	{
		return min;
	}
	
	T getMax() const
	{
		return max;
	}
	
private:
	T min;
	T max;	
};

#endif // RANGE_H_INCLUDED
