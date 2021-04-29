#include "tgaimage.h"

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);


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

void line5(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color) 
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

int main(int argc, char** argv)
{
    TGAImage image(500, 500, TGAImage::RGB);
	line5(0, 0, 10, 80, image, white);

    image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
    image.write_tga_file("output.tga");
    return 0;
}

