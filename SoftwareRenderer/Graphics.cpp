#include "Graphics.h"
#include "Geometry.h"

Graphics::Graphics()
{
    _image = new TGAImage(IMAGE_WIDTH, IMAGE_HEIGHT, TGAImage::RGB);
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

// Rasterize triangle based on line sweeping.
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

// Rasterize triangle based on AABB.
void Graphics::DrawTriangle(Vec3f* vertices, Vec3f* uvs, Vec3f* normals, float* zBuffer, TGAImage* texture, Vec3f light)
{
    // create AABB of the triangle
    AABB aabb;
    aabb.min.x = std::max(0.0f, std::min(vertices[0].x, std::min(vertices[1].x, vertices[2].x)));
    aabb.min.y = std::max(0.0f, std::min(vertices[0].y, std::min(vertices[1].y, vertices[2].y)));
    aabb.max.x = std::min((float)(_image->get_width() - 1), std::max(vertices[0].x, std::max(vertices[1].x, vertices[2].x)));
    aabb.max.y = std::min((float)(_image->get_height() - 1), std::max(vertices[0].y, std::max(vertices[1].y, vertices[2].y)));
    
    Triangle triangle(vertices);
    Vec3f point;
    for (point.x = aabb.min.x; point.x <= aabb.max.x; point.x++)
    {
        for (point.y = aabb.min.y; point.y <= aabb.max.y; point.y++)
        {
			Vec3f p = triangle.BaryCentric(point);
            if (p.x >= 0 && p.y >= 0 && p.z >= 0)
            {
				point.z = vertices[0].z * p.x + vertices[1].z * p.y + vertices[2].z * p.z;
				
				if (zBuffer[int(point.x + point.y * IMAGE_WIDTH)] < point.z)
				{
					zBuffer[int(point.x + point.y * IMAGE_WIDTH)] = point.z;
					// sample texture
					Vec3f uv = uvs[0] * p.x + uvs[1] * p.y + uvs[2] * p.z;
					Vec3f norm = normals[0] * p.x + normals[1] * p.y + normals[2] * p.z;

					float intensity = light * norm;
					TGAColor color = texture->get(uv.x * texture->get_width(),  (1 - uv.y) * texture->get_height());
					if (intensity > 0)
					{
						color = TGAColor(color.r * intensity, color.g * intensity, color.b * intensity, color.a);
					}
					_image->set(point.x, point.y, color); 
				}
            }
        }
    }
}

void Graphics::DrawModel(Model* model)
{
    // wireframe render
//    for (int i = 0; i < model->nfaces(); i++)
//    {
//        std::vector<int> face = model->face(i);
//        for (int j = 0; j < 3; j++)
//        {
//            Vec3f v0 = model->vert(face[j]);
//            Vec3f v1 = model->vert(face[(j + 1) % 3]);
//            int x0 = (v0.x + 1.0) * IMAGE_WIDTH / 2.0;
//            int y0 = (v0.y + 1.0) * IMAGE_HEIGHT / 2.0;
//            int x1 = (v1.x + 1.0) * IMAGE_WIDTH / 2.0;
//            int y1 = (v1.y + 1.0) * IMAGE_HEIGHT / 2.0;
//            Vec2i x(x0, y0);
//            Vec2i y(x1, y1);
//            DrawLine(x, y, white);
//        }
//    }

	// Initialize z buffer
	float *zBuffer = new float[IMAGE_WIDTH * IMAGE_HEIGHT];
	for (int i = IMAGE_WIDTH * IMAGE_HEIGHT; i--; zBuffer[i] = -std::numeric_limits<float>::max());

	// Load texture
	TGAImage* texture = new TGAImage();
	texture->read_tga_file("obj/african_head_diffuse.tga");

    // flat shading render
    for (int i = 0; i < model->nfaces(); i++)
    {
        std::vector<Vec3f> face = model->face(i);
        Vec3f screen_coords[3];
        Vec3f world_coords[3];
		Vec3f uvs[3];
		Vec3f norms[3];
        for (int j = 0; j < 3; j++)
        {
            Vec3f worldPosition = model->vert(face[0][j]);
			Vec3f uv = model->uv(face[1][j]);
			Vec3f norm = model->uv(face[2][j]);
			screen_coords[j] = WorldToScreenPoint(worldPosition);
            world_coords[j] = worldPosition;
			uvs[j] = uv;
			norms[j] = norm;
        }

        Vec3f lightDir = Vec3f(0, 0, -1); 
		Vec3f normal = (world_coords[2] - world_coords[0]).Cross(world_coords[1] - world_coords[0]).normalize();
		for (int j = 0; j < 3; j++)
		{
			norms[j] = normal;
		}
		DrawTriangle(screen_coords, uvs, norms, zBuffer, texture, lightDir);
    }

	delete texture;
}

void Graphics::End()
{
    _image->flip_vertically();
    _image->write_tga_file("output.tga");
}
