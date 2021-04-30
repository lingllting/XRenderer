#include <vector>
#include <cmath>
#include "model.h"
#include "tgaimage.h"
#include "Graphics.h"

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);

Model *model = NULL;
const int width = 800;
const int height = 800;

void DrawModel();


void line1(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color)
{
	// sample step based on t
    for (float t = 0; t < 1; t += 0.1)
    {
        int x = x0 * (1 - t) + x1 * t;
        int y = y0 * (1 - t) + y1 * t;
        image.set(x, y, color);
    }
}

void line2(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color) 
{
	// sample step based on x
	for (int x = x0; x <= x1; x++) 
	{
		float t = (x - x0) / (float)(x1 - x0);
		int y = y0 * (1 - t) + y1 * t;
		image.set(x, y, color);
	}
}

void line3(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color) 
{
	bool steep = false;
	if (std::abs(x0 - x1) < std::abs(y0 - y1))
	{ // if the line is steep, we transpose the image 
		std::swap(x0, y0);
		std::swap(x1, y1);
		steep = true;
	}
	if (x0 > x1) 
	{ // make it left−to−right 
		std::swap(x0, x1);
		std::swap(y0, y1);
	}
	for (int x = x0; x <= x1; x++) 
	{
		float t = (x - x0) / (float)(x1 - x0);
		int y = y0 * (1. - t) + y1 * t;

		if (steep) 
		{
			image.set(y, x, color); // if transposed, de−transpose 
		}
		else 
		{ 
			image.set(x, y, color);
		}
	}
}

void line4(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color) {
	bool steep = false;
	if (std::abs(x0 - x1) < std::abs(y0 - y1)) {
		std::swap(x0, y0);
		std::swap(x1, y1);
		steep = true;
	}
	if (x0 > x1) {
		std::swap(x0, x1);
		std::swap(y0, y1);
	}
	int dx = x1 - x0;
	int dy = y1 - y0;
	float derror = std::abs(dy / float(dx));
	float error = 0;
	int y = y0;
	for (int x = x0; x <= x1; x++) 
	{
		if (steep) 
		{
			image.set(y, x, color);
		}
		else 
		{
			image.set(x, y, color);
		}
		error += derror;
		if (error > 0.5) 
		{
			y += (y1 > y0 ? 1 : -1);
			error -= 1;
		}
	}
}

void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color) 
{
	bool steep = false;
	// transpose line if it's steep
	if (std::abs(x0 - x1) < std::abs(y0 - y1)) 
	{
		std::swap(x0, y0);
		std::swap(x1, y1);
		steep = true;
	}
	// make it always be left-to-right
	if (x0 > x1)
	{
		std::swap(x0, x1);
		std::swap(y0, y1);
	}
	int dx = x1 - x0;
	int dy = y1 - y0;
	// error of each x step
	int derror = std::abs(dy) * 2;
	// accumulated error
	int error = 0;
	int y = y0;
	for (int x = x0; x <= x1; x++) 
	{
		if (steep)
		{
			image.set(y, x, color);
		}
		else 
		{
			image.set(x, y, color);
		}
		
		error += derror;
		// y steps further when error is greater than the limitation which is dx
		if (error > dx) 
		{
			y += (y1 > y0 ? 1 : -1);
			error -= dx * 2;
		}
	}
}

void DrawModel()
{
    model = new Model("obj/african_head.obj");
    TGAImage image(width, height, TGAImage::RGB);
    for (int i = 0; i < model->nfaces(); i++)
    {
        std::vector<int> face = model->face(i);
        for (int j = 0; j < 3; j++)
        {
            Vec3f v0 = model->vert(face[j]);
            Vec3f v1 = model->vert(face[(j + 1) % 3]);
            int x0 = (v0.x + 1.)*width / 2.;
            int y0 = (v0.y + 1.)*height / 2.;
            int x1 = (v1.x + 1.)*width / 2.;
            int y1 = (v1.y + 1.)*height / 2.;
            line(x0, y0, x1, y1, image, white);
        }
    }

    image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
    image.write_tga_file("output.tga");
    delete model;
}

int main(int argc, char** argv)
{
    Graphics graphics;
    Vec2i a[3] = {Vec2i(10, 70),   Vec2i(50, 160),  Vec2i(70, 80)};
    Vec2i b[3] = {Vec2i(180, 50),  Vec2i(150, 1),   Vec2i(70, 180)};
    Vec2i c[3] = {Vec2i(180, 150), Vec2i(120, 160), Vec2i(130, 180)};
    graphics.DrawTriangle(a[0], a[1], a[2]);
    graphics.DrawTriangle(b[0], b[1], b[2]);
    graphics.DrawTriangle(c[0], c[1], c[2]);
    graphics.End();
	return 0;
}

