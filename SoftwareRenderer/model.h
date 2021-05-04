#ifndef __MODEL_H__
#define __MODEL_H__

#include <vector>
#include "Geometry.h"

class Model 
{
private:
	std::vector<Vec3f> verts_;
	std::vector<Vec3f> uvs_;
	std::vector<Vec3f> norms_;
	std::vector<std::vector<Vec3f> > faces_;
public:
	Model(const char *filename);
	~Model();
	int nverts();
	int nfaces();
	Vec3f vert(int i);
	Vec3f uv(int i);
	Vec3f norm(int i);
	std::vector<Vec3f> face(int idx);
};

#endif //__MODEL_H__
