#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__

#include "tgaimage.h"
#include "Geometry.h"
#include "model.h"
#include <algorithm>

extern Matrix ModelView;
extern Matrix Viewport;
extern Matrix Projection;

Vec3f m2v(Matrix m);
Matrix v2m(Vec3f v);
void viewport(int x, int y, int w, int h);
void lookat(Vec3f eye, Vec3f center, Vec3f up);
void projection(float coeff = 0.f); // coeff = -1/c

struct Color
{

};

struct IShader
{
	virtual ~IShader();
	virtual Vec3f vertex(int iface, int nthvert) = 0;
	virtual bool fragment(Vec3f bar, TGAColor &color) = 0;
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
	const TGAColor black = TGAColor(0, 0, 0, 255);

	void DrawLine(Vec2i a, Vec2i b, TGAColor color);
	void DrawTriangleSweepingLine(Vec3f* vertices, Vec3f* uvs, Vec3f* normals, float* zBuffer, TGAImage* texture, Vec3f light, float intensity);
    void DrawTriangleAABB(Vec3f* vetices, Vec3f* uvs, Vec3f* normals, float* zBuffer, Model* texture, Vec3f light, float intensity, IShader& shader);
    void DrawModel(Model* model, Vec3f eye, Vec3f lightDir, IShader& shader);
    void End();

	inline Vec3f WorldToScreenPoint(Vec3f point)
	{
		return Vec3f(int((point.x + 1.) * IMAGE_WIDTH / 2. + .5), int((point.y + 1.) * IMAGE_HEIGHT / 2. + .5), point.z);
	}
};



#endif
