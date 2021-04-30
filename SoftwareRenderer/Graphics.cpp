#include "Graphics.h"

Graphics::Graphics()
{
    _image = new TGAImage(500, 500, TGAImage::RGB);
}
Graphics::~Graphics()
{
    
}

void Graphics::DrawLine(Vec2i a, Vec2i b, TGAColor color)
{
	bool steep = false;
	// transpose line if it's steep
	if (std::abs(a.x - b.x) < std::abs(a.y - b.y))
	{
		std::swap(a.x, a.y);
		std::swap(b.x, b.y);
		steep = true;
	}
	// make it always be left-to-right
	if (a.x > b.x)
	{
		std::swap(a.x, b.x);
		std::swap(a.y, b.y);
	}
	int dx = b.x - a.x;
	int dy = b.y - a.y;
	// error of each x step
	int derror = std::abs(dy) * 2;
	// accumulated error
	int error = 0;
	int y = a.y;
	for (int x = a.x; x <= b.x; x++)
	{
		if (steep)
		{
			_image->set(y, x, color);
		}
		else
		{
			_image->set(x, y, color);
		}

		error += derror;
		// y steps further when error is greater than the limitation which is dx
		if (error > dx)
		{
			y += (b.y > a.y ? 1 : -1);
			error -= dx * 2;
		}
	}
}

void Graphics::DrawTriangle(Vec2i a, Vec2i b, Vec2i c)
{
    // sort the vertices bottom-to-top in ascending order by their y-coordinate.
    if (a.y > b.y) std::swap(a, b);
    if (a.y > c.y) std::swap(a, c);
    if (b.y > c.y) std::swap(b, c);
    
    // it couldn't be zero here, otherwise it's a line rather than a triangle.
    // height from a to c.
    int acHeight = c.y - a.y;
    // width from a to c
    int acWidth = c.x - a.x;
    // plus 1 in case it could be zero divisor.
    // height from a to b.
    int abHeight = b.y - a.y + 1;
    // width from a to b.
    int abWidth = b.x - a.x;
    
    // draw lower part
    for (int y = a.y; y <= b.y; y++)
    {
        // similar triangle theory.
        float alpha = (y - a.y) / (float)acHeight;
        float beta = (y - a.y) / (float)abHeight;
        int A = a.x + acWidth * alpha;
        int B = a.x + abWidth * beta;

        if (A > B) std::swap(A, B);
        for (int x = A; x <= B; x++)
        {
            _image->set(x, y, white);
        }
    }
    
    int bcHeight = c.y - b.y;
    int bcWidth = c.x - b.x;
    // draw upper part.
    for (int y = b.y; y <= c.y; y++)
    {
        float alpha = (y - a.y) / (float)acHeight;
        float beta = (y - b.y) / (float) bcHeight;
        int A = a.x + acWidth * alpha;
        int B = b.x + bcWidth * beta;

        if (A > B) std::swap(A, B);
        for (int x = A; x <= B; x++)
        {
            _image->set(x, y, white);
        }
    }
}

void Graphics::End()
{
    _image->flip_vertically();
    _image->write_tga_file("output.tga");
}
