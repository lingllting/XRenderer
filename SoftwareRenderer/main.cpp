#include <vector>
#include <cmath>
#include "model.h"
#include "tgaimage.h"
#include "Graphics.h"

Model *model = NULL;
Vec3f lightDir = Vec3f(1, 1, 1).normalize();
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

	virtual bool fragment(Vec3f bar, TGAColor& color)
	{
		float intensity = varying_intensity * bar;   // interpolate intensity for the current pixel
		Matrix uvMat = varying_uv * bar;                 // interpolate uv for the current pixel
//		Vec2f uv = Vec2f(uvMat.m[0][0], uvMat.m[1][0]);
//      color = model->diffuse(uv) * intensity;
		color = TGAColor(255, 255, 255) * intensity;
		return false;                              // no, we do not discard this pixel
	}
};

struct Shader : public IShader
{
    Matrix varying_uv = Matrix(2, 3);
    Matrix uniform_MVP = Matrix(4, 4);
    Matrix uniform_MVP_IT = Matrix(4, 4);

    virtual Vec3f vertex(int iface, int nthvert)
    {
        Matrix gl_Vertex = v2m(model->vert(iface, nthvert));
        gl_Vertex = Viewport * Projection * ModelView * gl_Vertex;
        
        Vec2f uv = model->uv(iface, nthvert);
        varying_uv.SetCol(nthvert, uv);
        
        return m2v(gl_Vertex);
    }

    virtual bool fragment(Vec3f bar, TGAColor& color)
    {
        Matrix uvMat = varying_uv * bar;
        Vec2f uv = Vec2f(uvMat.m[0][0], uvMat.m[1][0]);
        
        Matrix lightMat = uniform_MVP * v2m(lightDir);
        Vec3f light = Vec3f(lightMat[0][0], lightMat[1][0], lightMat[2][0]).normalize();
        
        Matrix normalMat = uniform_MVP_IT * v2m(model->normal(uv));
        Vec3f normal = Vec3f(normalMat[0][0], normalMat[1][0], normalMat[2][0]).normalize();
        float intensity = std::max(0.f, light * normal);
        color = model->diffuse(uv) * intensity;
        return false;                           
    }
};

struct PhongShader : public IShader
{
    Matrix varying_uv = Matrix(2, 3);
    Matrix uniform_MVP = Matrix(4, 4);
    Matrix uniform_MVP_IT = Matrix(4, 4);
    
    virtual Vec3f vertex(int iface, int nthvert)
    {
        Matrix gl_Vertex = v2m(model->vert(iface, nthvert));
        gl_Vertex = Viewport * Projection * ModelView * gl_Vertex;
        
        Vec2f uv = model->uv(iface, nthvert);
        varying_uv.SetCol(nthvert, uv);
        
        return m2v(gl_Vertex);
    }

    virtual bool fragment(Vec3f bar, TGAColor& color)
    {
        Matrix uvMat = varying_uv * bar;
        Vec2f uv = Vec2f(uvMat.m[0][0], uvMat.m[1][0]);
        
        Matrix lightMat = uniform_MVP * v2m(lightDir);
        Vec3f light = Vec3f(lightMat[0][0], lightMat[1][0], lightMat[2][0]).normalize();
        
        Matrix normalMat = uniform_MVP_IT * v2m(model->normal(uv));
        Vec3f normal = Vec3f(normalMat[0][0], normalMat[1][0], normalMat[2][0]).normalize();
        float diffuse = std::max(0.f, normal * light);
        Vec3f reflect = (normal * ( normal * light * 2.f) - light).normalize();
        // float specular = pow(std::max(reflect * eye.normalize(), 0.0f), model->specular(uv));
        float specular = pow(std::max(reflect.z, 0.0f), model->specular(uv));
        float ambient = 5.;
        color = model->diffuse(uv);
        
        color.r = std::min<float>((ambient + color.r * (diffuse + specular)), 255);
        color.g = std::min<float>((ambient + color.g * (diffuse + specular)), 255);
        color.b = std::min<float>((ambient + color.b * (diffuse + specular)), 255);
        return false;
    }
};

int main(int argc, char** argv)
{
    Graphics graphics;

	lookat(eye, center, up);
	viewport(graphics.IMAGE_WIDTH / 8, graphics.IMAGE_HEIGHT / 8, graphics.IMAGE_WIDTH * 3 / 4, graphics.IMAGE_HEIGHT * 3 / 4);
	projection(-1.f / (eye - center).norm());

	PhongShader shader;
    shader.uniform_MVP = Projection * ModelView;
    shader.uniform_MVP_IT = (Projection * ModelView).inverse().transpose();

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

