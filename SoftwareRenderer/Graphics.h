#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__

#include "tgaimage.h"
#include "Geometry.h"
#include "model.h"
#include <algorithm>

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
    
    const int IMAGE_WIDTH = 1024;
    const int IMAGE_HEIGHT = 1024;
    const TGAColor white = TGAColor(255, 255, 255, 255);
    const TGAColor red   = TGAColor(255, 0,   0,   255);
    const TGAColor yellow   = TGAColor(255, 255,   0,   255);
	const TGAColor black = TGAColor(0, 0, 0, 255);

	void DrawLine(Vec2i a, Vec2i b, TGAColor color);
	void DrawTriangle(Vec2i a, Vec2i b, Vec2i c);
    void DrawTriangle(Vec3f* vetices, Vec3f* uvs, Vec3f* normals, float* zBuffer, TGAImage* texture, Vec3f light);
    void DrawModel(Model* model);
    void End();

	inline Vec3f WorldToScreenPoint(Vec3f point)
	{
		return Vec3f(int((point.x + 1.) * IMAGE_WIDTH / 2. + .5), int((point.y + 1.) * IMAGE_HEIGHT / 2. + .5), point.z);
	}
};



#endif
