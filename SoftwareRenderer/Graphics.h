#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__

#include "tgaimage.h"
#include "Geometry.h"
#include "model.h"

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
    
    const int IMAGE_WIDTH = 800;
    const int IMAGE_HEIGHT = 800;
    const TGAColor white = TGAColor(255, 255, 255, 255);
    const TGAColor red   = TGAColor(255, 0,   0,   255);
    const TGAColor yellow   = TGAColor(255, 255,   0,   255);

	void DrawLine(Vec2i a, Vec2i b, TGAColor color);
	void DrawTriangle(Vec2i a, Vec2i b, Vec2i c);
    void DrawTriangle(Vec2i* vetices, TGAColor color);
    void DrawModel(Model* model);
    void End();
};



#endif
