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
void Graphics::DrawTriangleSweepingLine(Vec3f* vertices, Vec3f* uvs, Vec3f* normals, float* zBuffer, TGAImage* texture, Vec3f light, float intensity)
{
	vertices[0] = Vec3i(vertices[0]);
	vertices[1] = Vec3i(vertices[1]);
	vertices[2] = Vec3i(vertices[2]);

	Vec3f t0 = vertices[0];
	Vec3f t1 = vertices[1];
	Vec3f t2 = vertices[2];
	Vec2i uv0 = Vec2i(uvs[0].x * texture->get_width(), uvs[0].y * texture->get_height());
	Vec2i uv1 = Vec2i(uvs[1].x * texture->get_width(), uvs[1].y * texture->get_height());
	Vec2i uv2 = Vec2i(uvs[2].x * texture->get_width(), uvs[2].y * texture->get_height());
	float norm0 = normals[0] * light;
	float norm1 = normals[1] * light;
	float norm2 = normals[2] * light;
	// sort the vertices bottom-to-top in ascending order by their y-coordinate.
	if (t0.y > t1.y) { std::swap(t0, t1); std::swap(uv0, uv1); std::swap(norm0, norm1); };
	if (t0.y > t2.y) { std::swap(t0, t2); std::swap(uv0, uv2); std::swap(norm0, norm2); };
	if (t1.y > t2.y) { std::swap(t1, t2); std::swap(uv1, uv2); std::swap(norm1, norm2); };
	int total_height = t2.y - t0.y;
	for (int i = 0; i < total_height; i++) 
	{
		bool second_half = i > t1.y - t0.y || t1.y == t0.y;
		int segment_height = second_half ? t2.y - t1.y : t1.y - t0.y;
		float alpha = (float)i / total_height;
		float beta = (float)(i - (second_half ? t1.y - t0.y : 0)) / segment_height; // be careful: with above conditions no division by zero here
		Vec3f A = t0 + Vec3f(t2 - t0) * alpha;
		Vec3f B = second_half ? t1 + Vec3f(t2 - t1)*beta : t0 + Vec3f(t1 - t0) * beta;
		Vec2i uvA = uv0 + (uv2 - uv0) * alpha;
		Vec2i uvB = second_half ? uv1 + (uv2 - uv1)*beta : uv0 + (uv1 - uv0) * beta;
		float normA = norm0 + (norm2 - norm0) * alpha;
		float normB = second_half ? norm1 + (norm2 - norm1)*beta : norm0 + (norm1 - norm0) * beta;
		if (A.x > B.x)
		{ 
			std::swap(A, B); 
			std::swap(uvA, uvB); 
			std::swap(normA, normB);
		}
		for (int j = A.x; j <= B.x; j++)
		{
			float phi = B.x == A.x ? 1. : (float)(j - A.x) / (float)(B.x - A.x);
			Vec3f P = A + (B - A) * phi;
			Vec2i uvP = uvA + (uvB - uvA) * phi;
			float norm = normA + (normB - normA) * phi;
			int idx = P.x + P.y * IMAGE_WIDTH;
			if (zBuffer[idx] < P.z) 
			{
				zBuffer[idx] = P.z;
				TGAColor color = texture->get(uvP.x, uvP.y);
				color = color * norm;
				_image->set(P.x, P.y, color);

				//_image->set(P.x, P.y, white * norm);
			}
		}
	}
}

// Rasterize triangle based on AABB.
void Graphics::DrawTriangleAABB(Vec3f* vertices, Vec3f* uvs, Vec3f* normals, float* zBuffer, TGAImage* texture, Vec3f light, float intensity)
{
	vertices[0] = Vec3i(vertices[0]);
	vertices[1] = Vec3i(vertices[1]);
	vertices[2] = Vec3i(vertices[2]);
    // create AABB of the triangle
    AABB aabb;
    aabb.min.x = std::max(0.0f, std::min(vertices[0].x, std::min(vertices[1].x, vertices[2].x)));
    aabb.min.y = std::max(0.0f, std::min(vertices[0].y, std::min(vertices[1].y, vertices[2].y)));
    aabb.max.x = std::min((float)(_image->get_width() - 1), std::max(vertices[0].x, std::max(vertices[1].x, vertices[2].x)));
    aabb.max.y = std::min((float)(_image->get_height() - 1), std::max(vertices[0].y, std::max(vertices[1].y, vertices[2].y)));

	Vec3f t0 = vertices[0];
	Vec3f t1 = vertices[1];
	Vec3f t2 = vertices[2];
	Vec2i uv0 = Vec2i(uvs[0].x * texture->get_width(), uvs[0].y * texture->get_height());
	Vec2i uv1 = Vec2i(uvs[1].x * texture->get_width(), uvs[1].y * texture->get_height());
	Vec2i uv2 = Vec2i(uvs[2].x * texture->get_width(), uvs[2].y * texture->get_height());
	float intensity0 = normals[0] * light;
	float intensity1 = normals[1] * light;
	float intensity2 = normals[2] * light;
	// sort the vertices bottom-to-top in ascending order by their y-coordinate.
	if (t0.y > t1.y) { std::swap(t0, t1); std::swap(uv0, uv1); std::swap(intensity0, intensity1);};
	if (t0.y > t2.y) { std::swap(t0, t2); std::swap(uv0, uv2); std::swap(intensity0, intensity2);};
	if (t1.y > t2.y) { std::swap(t1, t2); std::swap(uv1, uv2); std::swap(intensity1, intensity2);};
    
    Triangle triangle(vertices);
    Vec3f point;
    for (point.x = aabb.min.x; point.x <= aabb.max.x; point.x++)
    {
        for (point.y = aabb.min.y; point.y <= aabb.max.y; point.y++)
        {
			Vec3f p = triangle.BaryCentric(point);
            if (p.x >= 0 && p.y >= 0 && p.z >= 0)
			// if (triangle.Contains(point))
            {
				// barycentric interpolation
				point.z = vertices[0].z * p.x + vertices[1].z * p.y + vertices[2].z * p.z;
				
				if (zBuffer[int(point.x + point.y * IMAGE_WIDTH)] < point.z)
				{
					zBuffer[int(point.x + point.y * IMAGE_WIDTH)] = point.z;
					// sample texture
					Vec3f uv = uvs[0] * p.x + uvs[1] * p.z + uvs[2] * p.y;
					Vec3f norm = normals[0] * p.x + normals[1] * p.z + normals[2] * p.y;

					float intensity = light * norm;
					TGAColor color = texture->get(uv.x * texture->get_width(),  uv.y * texture->get_height());
// 					if (intensity > 0)
// 					{
// 						color = TGAColor(color.r * intensity, color.g * intensity, color.b * intensity, color.a);
// 					}
					color = color * intensity;
					_image->set(point.x, point.y, color);
				}

				// bilinear interpolation
// 				int height = point.y - t0.y;
// 				int total_height = t2.y - t0.y;
// 				bool second_half = height > t1.y - t0.y || t1.y == t0.y;
// 				int segment_height = second_half ? t2.y - t1.y : t1.y - t0.y;
// 				float alpha = (float)height / total_height;
// 				float beta = (float)(height - (second_half ? t1.y - t0.y : 0)) / segment_height;
// 				Vec3f A = t0 + Vec3f(t2 - t0) * alpha;
// 				Vec3f B = second_half ? t1 + Vec3f(t2 - t1)*beta : t0 + Vec3f(t1 - t0) * beta;
// 				Vec2i uvA = uv0 + (uv2 - uv0) * alpha;
// 				Vec2i uvB = second_half ? uv1 + (uv2 - uv1)*beta : uv0 + (uv1 - uv0) * beta;
// 				float intensityA = intensity0 + (intensity2 - intensity0) * alpha;
// 				float intensityB = second_half ? intensity1 + (intensity2 - intensity1)*beta : intensity0 + (intensity1 - intensity0) * beta;
// 
// 				float phi = B.x == A.x ? 1. : (float)(point.x - A.x) / (float)(B.x - A.x);
// 				Vec3f P = A + (B - A) * phi;
// 				Vec2i uvP = uvA + (uvB - uvA) * phi;
// 				float intensityP = intensityA + (intensityB - intensityA) * phi;
// 				int idx = P.x + P.y * IMAGE_WIDTH;
// 				if (zBuffer[idx] < P.z)
// 				{
// 					zBuffer[idx] = P.z;
// 					TGAColor color = texture->get(uvP.x, uvP.y);
// 					color = color * intensityP;
// 					_image->set(P.x, P.y, color);
// 					// _image->set(P.x, P.y, white * norm);
// 				}
             }
        }
    }
}

void Graphics::DrawModel(Model* model, Vec3f eye, Vec3f lightDir)
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
	texture->flip_vertically();

	Matrix projection = Matrix::identity(4);
	projection[3][2] = -1.0f / 3.0f;

	Matrix viewPort = viewport(IMAGE_WIDTH / 8, IMAGE_HEIGHT / 8, IMAGE_WIDTH * 3 / 4, IMAGE_HEIGHT * 3 / 4);
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
			Vec3f norm = model->norm(face[2][j]);
			// screen_coords[j] = WorldToScreenPoint(worldPosition);
			screen_coords[j] = m2v(viewPort * projection  * lookat(eye, Vec3f(0, 0, 0), Vec3f(0, 1, 0)) * v2m(worldPosition));
            world_coords[j] = worldPosition;
			uvs[j] = uv;
			norms[j] = norm;
        }

		Vec3f normal = (world_coords[2] - world_coords[0]).Cross(world_coords[1] - world_coords[0]).normalize();
		float intensity = lightDir * normal;
		bool isBackface = eye * normal <= 0;
		if (isBackface > 0)
		{
			DrawTriangleAABB(screen_coords, uvs, norms, zBuffer, texture, lightDir, intensity);
		}
    }

	delete texture;
}

void Graphics::End()
{
    _image->flip_vertically();
    _image->write_tga_file("output.tga");
}