#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__

#include "tgaimage.h"
#include "Geometry.h"

struct Color
{

};

class Graphics
{
private:
	TGAImage _image;
public:
	Graphics();
	~Graphics();

	void DrawLine(Vec2i a, Vec2i b, TGAColor color);
	void DrawTriangle(Vec2i a, Vec2i b, Vec2i c, TGAColor color);
};



#endif
