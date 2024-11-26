#ifndef SUBWINDOW_H_INCLUDED
#define SUBWINDOW_H_INCLUDED

struct Origin
{
	float x;
	float y;
};

struct Size
{
	float width;
	float height;
};



class Subwindow
{
public:
	Subwindow(const Origin& origin, const Size& size): origin(origin), size(size) {}
	virtual ~Subwindow() = default;
	virtual void render() = 0;
	
protected:
	Origin origin;
	Size size;
};

#endif // SUBWINDOW_H_INCLUDED
