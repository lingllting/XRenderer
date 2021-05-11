#ifndef __MODEL_H__
#define __MODEL_H__

#include <vector>
#include "Geometry.h"
#include "tgaimage.h"

class Model 
{
private:
	std::vector<Vec3f> verts_;
	std::vector<Vec3f> uvs_;
	std::vector<Vec3f> norms_;
	std::vector<std::vector<Vec3f> > faces_;
	TGAImage diffusemap_;
	TGAImage normalmap_;
	TGAImage specularmap_;
public:
	Model(const char *filename);
	~Model();
	int nverts();
	int nfaces();
	Vec3f vert(int i, int ivert);
	Vec2f uv(int i, int ivert);
	Vec3f norm(int i, int ivert);
	std::vector<Vec3f> face(int idx);

	TGAColor diffuse(Vec2f uvf);
    Vec3f normal(Vec2f uv);
    
	void load_texture(std::string filename, const char *suffix, TGAImage &img);
	TGAImage& GetDiffuseMap() { return diffusemap_; }
};

#endif //__MODEL_H__
