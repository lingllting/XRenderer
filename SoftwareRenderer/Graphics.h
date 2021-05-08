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
    
    const int IMAGE_WIDTH = 800;
    const int IMAGE_HEIGHT = 800;
    const TGAColor white = TGAColor(255, 255, 255, 255);
    const TGAColor red   = TGAColor(255, 0,   0,   255);
    const TGAColor yellow   = TGAColor(255, 255,   0,   255);
	const TGAColor black = TGAColor(0, 0, 0, 255);

	void DrawLine(Vec2i a, Vec2i b, TGAColor color);
	void DrawTriangleSweepingLine(Vec3f* vertices, Vec3f* uvs, Vec3f* normals, float* zBuffer, TGAImage* texture, Vec3f light, float intensity);
    void DrawTriangleAABB(Vec3f* vetices, Vec3f* uvs, Vec3f* normals, float* zBuffer, TGAImage* texture, Vec3f light, float intensity);
    void DrawModel(Model* model, Vec3f eye, Vec3f lightDir);
    void End();

	inline Vec3f WorldToScreenPoint(Vec3f point)
	{
		return Vec3f(int((point.x + 1.) * IMAGE_WIDTH / 2. + .5), int((point.y + 1.) * IMAGE_HEIGHT / 2. + .5), point.z);
	}

	inline Vec3f m2v(Matrix m)
	{
		return Vec3f(m[0][0] / m[3][0], m[1][0] / m[3][0], m[2][0] / m[3][0]);
	}

	inline Matrix v2m(Vec3f v)
	{
		Matrix m(4, 1);
		m[0][0] = v.x;
		m[1][0] = v.y;
		m[2][0] = v.z;
		m[3][0] = 1.f;
		return m;
	}

	Matrix viewport(int x, int y, int w, int h) 
	{
		Matrix m = Matrix::identity(4);
		m[0][3] = x + w / 2.f;
		m[1][3] = y + h / 2.f;
		m[2][3] = 255 / 2.f;

		m[0][0] = w / 2.f;
		m[1][1] = h / 2.f;
		m[2][2] = 255 / 2.f;
		return m;
	}

	Matrix lookat(Vec3f eye, Vec3f center, Vec3f up)
	{
		Vec3f z = (eye - center).normalize();
		Vec3f x = (up^z).normalize();
		Vec3f y = (z^x).normalize();
		Matrix res = Matrix::identity(4);
		for (int i = 0; i < 3; i++) 
		{
			res[0][i] = x[i];
			res[1][i] = y[i];
			res[2][i] = z[i];
			res[i][3] = -center[i];
		}
		return res;
	}
};



#endif
