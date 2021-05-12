#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "model.h"

Model::Model(const char *filename) : verts_(), faces_() 
{
    std::ifstream in;
    in.open (filename, std::ifstream::in);
    if (in.fail()) return;
    std::string line;
    while (!in.eof()) 
	{
        std::getline(in, line);
        std::istringstream iss(line.c_str());
        char trash;
        if (!line.compare(0, 2, "v ")) 
		{
            iss >> trash;
            Vec3f v;
            for (int i=0;i<3;i++) iss >> v.raw[i];
            verts_.push_back(v);
        }
		else if (!line.compare(0, 3, "vt "))
		{
			iss >> trash >> trash;
			Vec3f v;
			for (int i = 0; i < 3; i++) iss >> v.raw[i];
			uvs_.push_back(v);
		}
		else if (!line.compare(0, 3, "vn "))
		{
			iss >> trash >> trash;
			Vec3f v;
			for (int i = 0; i < 3; i++) iss >> v.raw[i];
			norms_.push_back(v);
		}
		else if (!line.compare(0, 2, "f "))
		{
            std::vector<Vec3f> f;
			Vec3f iVert, iUV, iNorm;
            iss >> trash;
			for (int i = 0; i < 3; i++)
			{
				iss >> iVert[i] >> trash >> iUV[i] >> trash >> iNorm[i];
				iVert[i]--;
				iUV[i]--;
				iNorm[i]--;
			}
			f.push_back(iVert);
			f.push_back(iUV);
			f.push_back(iNorm);
			faces_.push_back(f);
        }
    }
    std::cerr << "# v# " << verts_.size() << " f# "  << faces_.size() << std::endl;

	load_texture(filename, "_diffuse.tga", diffusemap_);
    load_texture(filename, "_nm.tga", normalmap_);
    load_texture(filename, "_spec.tga", specularmap_);
}

Model::~Model() {
}

int Model::nverts() {
    return (int)verts_.size();
}

int Model::nfaces() {
    return (int)faces_.size();
}

std::vector<Vec3f> Model::face(int idx) {
    return faces_[idx];
}

Vec3f Model::vert(int i, int ivert) 
{
    return verts_[face(i)[0][ivert]];
}

Vec2f Model::uv(int i, int ivert) 
{
	return uvs_[face(i)[1][ivert]];
}

Vec3f Model::norm(int i, int ivert) 
{
	return norms_[face(i)[2][ivert]].normalize();
}

TGAColor Model::diffuse(Vec2f uvf) 
{
	Vec2i uv(uvf[0] * diffusemap_.get_width(), uvf[1] * diffusemap_.get_height());
	return diffusemap_.get(uv[0], uv[1]);
}

Vec3f Model::normal(Vec2f uvf)
{
    Vec2i uv(uvf[0] * normalmap_.get_width(), uvf[1] * normalmap_.get_height());
    return normalmap_.get(uv[0], uv[1]);
}

float Model::specular(Vec2f uvf)
{
    return specularmap_.get(uvf[0] * specularmap_.get_width(), uvf[1] * specularmap_.get_height()).raw[0];
}

void Model::load_texture(std::string filename, const char *suffix, TGAImage &img) 
{
	std::string texfile(filename);
	size_t dot = texfile.find_last_of(".");
	if (dot != std::string::npos) 
	{
		texfile = texfile.substr(0, dot) + std::string(suffix);
		std::cerr << "texture file " << texfile << " loading " << (img.read_tga_file(texfile.c_str()) ? "ok" : "failed") << std::endl;
		img.flip_vertically();
	}
}


