#include <vector>
#include <cmath>
#include "model.h"
#include "tgaimage.h"
#include "Graphics.h"

Model *model = NULL;
Vec3f lightDir = Vec3f(1, -1, 1).normalize();
Vec3f eye = Vec3f(1, 1, 3);
Vec3f center(0, 0, 0);
Vec3f up(0, 1, 0);

struct GouraudShader : public IShader
{
	Vec3f varying_intensity; // written by vertex shader, read by fragment shader
	Matrix varying_uv = Matrix(2, 3);

	virtual Vec3f vertex(int iface, int nthvert)
	{
		Matrix gl_Vertex = v2m(model->vert(iface, nthvert)); // read the vertex from .obj file
		gl_Vertex = Viewport * Projection * ModelView * gl_Vertex;     // transform it to screen coordinates
		varying_intensity[nthvert] = std::max(0.f, model->norm(iface, nthvert) * lightDir); 
		Vec2f uv = model->uv(iface, nthvert);
		varying_uv.SetCol(nthvert, uv);
		// get diffuse lighting intensity
		return m2v(gl_Vertex);
	}

	virtual bool fragment(Vec3f bar, TGAColor &color) 
	{
		float intensity = varying_intensity * bar;   // interpolate intensity for the current pixel
		Matrix uvMat = varying_uv * bar;                 // interpolate uv for the current pixel
		Vec2f uv = Vec2f(uvMat.m[0][0], uvMat.m[1][0]);
		color = TGAColor(255, 255, 255) * intensity;
		return false;                              // no, we do not discard this pixel
	}
};

int main(int argc, char** argv)
{
    Graphics graphics;

	lookat(eye, center, up);
	viewport(graphics.IMAGE_WIDTH / 8, graphics.IMAGE_HEIGHT / 8, graphics.IMAGE_WIDTH * 3 / 4, graphics.IMAGE_HEIGHT * 3 / 4);
	projection(-1.f / (eye - center).norm());

	GouraudShader shader;

    // Vec2i a[3] = {Vec2i(10, 70),   Vec2i(50, 160),  Vec2i(70, 80)};
	// Vec2i b[3] = {Vec2i(180, 50),  Vec2i(150, 1),   Vec2i(70, 180)};
	// Vec2i c[3] = {Vec2i(180, 150), Vec2i(120, 160), Vec2i(130, 180)};
    
    
    // line sweeping
    // graphics.DrawTriangle(a[0], a[1], a[2]);
    // graphics.DrawTriangle(b[0], b[1], b[2]);
    // graphics.DrawTriangle(c[0], c[1], c[2]);
    
    //AABB
    // graphics.DrawTriangle(a, white);
    // graphics.DrawTriangle(b, white);
    // graphics.DrawTriangle(c, white);
    
    model = new Model("obj/african_head.obj");
	graphics.DrawModel(model, eye, lightDir, shader);
    graphics.End();
    
    delete model;
	return 0;
}

