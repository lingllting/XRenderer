#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "model.h"

Model::Model(const char *filename) : verts_(), faces_() {
    std::ifstream in;
    in.open (filename, std::ifstream::in);
    if (in.fail()) return;
    std::string line;
    while (!in.eof()) {
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
		if (!line.compare(0, 3, "vt "))
		{
			iss >> trash >> trash;
			Vec3f v;
			for (int i = 0; i < 3; i++) iss >> v.raw[i];
			uvs_.push_back(v);
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

Vec3f Model::vert(int i) {
    return verts_[i];
}

Vec3f Model::uv(int i) {
	return uvs_[i];
}

Vec3f Model::norm(int i) {
	return norms_[i];
}


