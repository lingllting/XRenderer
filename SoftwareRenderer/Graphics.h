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
	TGAImage* _image;
public:
	Graphics();
	~Graphics();
    
    const TGAColor white = TGAColor(255, 255, 255, 255);
    const TGAColor red   = TGAColor(255, 0,   0,   255);
    const TGAColor yellow   = TGAColor(255, 255,   0,   255);

	void DrawLine(Vec2i a, Vec2i b, TGAColor color);
	void DrawTriangle(Vec2i a, Vec2i b, Vec2i c);
    void End();
};



#endif
