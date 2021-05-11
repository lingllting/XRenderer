#ifndef __GEOMETRY_H__
#define __GEOMETRY_H__

#include <cmath>
#include <iostream>
#include <assert.h>
#include <vector>

template <class T> struct Vec3;
template <class T> struct Vec2 
{
	union 
	{
		struct {T u, v;};
		struct {T x, y;};
		T raw[2];
	};
	Vec2() : u(0), v(0) {}
	Vec2(T _u, T _v) : u(_u),v(_v) {}
	Vec2(Vec3<T> vec3)
	{
		x = vec3.x;
		y = vec3.y;
	}
	inline Vec2<T> operator +(const Vec2<T> &V) const { return Vec2<T>(u+V.u, v+V.v); }
	inline Vec2<T> operator -(const Vec2<T> &V) const { return Vec2<T>(u-V.u, v-V.v); }
	inline Vec2<T> operator *(float f)          const { return Vec2<T>(u*f, v*f); }

	T& operator[](const size_t i)                { assert(i < 2); return raw[i]; }
	template <class > friend std::ostream& operator<<(std::ostream& s, Vec2<T>& v);

};

struct Matrix;
template <class T> struct Vec3
{
	union
    {
		struct {T x, y, z;};
		struct { T ivert, iuv, inorm; };
		T raw[3];
	};
	Vec3() : x(0), y(0), z(0) {}
	Vec3(T _x, T _y, T _z) : x(_x),y(_y),z(_z) {}
    Vec3(Vec2<T> vec2)
    {
        x = vec2.x;
        y = vec2.y;
        z = 0;
    }
    Vec3(Matrix& matrix);
	inline Vec3<T> operator ^(const Vec3<T> &v) const { return Vec3<T>(y*v.z-z*v.y, z*v.x-x*v.z, x*v.y-y*v.x); }
	inline Vec3<T> operator +(const Vec3<T> &v) const { return Vec3<T>(x+v.x, y+v.y, z+v.z); }
	inline Vec3<T> operator -(const Vec3<T> &v) const { return Vec3<T>(x-v.x, y-v.y, z-v.z); }
	inline Vec3<T> operator *(float f)          const { return Vec3<T>(x*f, y*f, z*f); }
	inline T       operator *(const Vec3<T> &v) const { return x*v.x + y*v.y + z*v.z; }
    inline Vec3<T> Cross(const Vec3<T> &v)      const { return Vec3<T>(y*v.z-z*v.y, z*v.x-x*v.z, x*v.y-y*v.x); }
	float norm  () const { return std::sqrt(x*x+y*y+z*z); }

	T& operator[](const size_t i)                { assert(i < 3); return raw[i]; }
	Vec3<T>& normalize(T l=1) { *this = (*this)*(l/norm()); return *this; }
	template <class > friend std::ostream& operator<<(std::ostream& s, Vec3<T>& v);
	template <class u> Vec3<T>(const Vec3<u> &v);
};

typedef Vec2<float> Vec2f;
typedef Vec2<int>   Vec2i;
typedef Vec3<float> Vec3f;
typedef Vec3<int>   Vec3i;

template <> template <> Vec3<int>::Vec3(const Vec3<float> &v);
template <> template <> Vec3<float>::Vec3(const Vec3<int> &v);

template <class t> std::ostream& operator<<(std::ostream& s, Vec2<t>& v)
{
	s << "(" << v.x << ", " << v.y << ")\n";
	return s;
}

template <class t> std::ostream& operator<<(std::ostream& s, Vec3<t>& v)
{
	s << "(" << v.x << ", " << v.y << ", " << v.z << ")\n";
	return s;
}

struct AABB
{
private:
    
public:
    Vec2f min;
    Vec2f max;
    
    AABB(){}
};

struct Triangle
{
private:
    Vec3f _a;
    Vec3f _b;
    Vec3f _c;
public:
    Triangle() : _a(Vec3<float>()), _b(Vec3<float>()), _c(Vec3<float>()){}
    Triangle(Vec3f a, Vec3f b, Vec3f c) : _a(a), _b(b), _c(c){}
    Triangle(Vec3f* vertices) : _a(vertices[0]), _b(vertices[1]), _c(vertices[2]){}
    
    inline bool Contains(Vec3f point)
    {
        Vec3f ab = _b - _a;
		Vec3f ap = point - _a;
		Vec3f bc = _c - _b;
		Vec3f bp = point - _b;
		Vec3f ca = _a - _c;
		Vec3f cp = point - _c;
        
        return (ab.Cross(ap).z >= 0 && bc.Cross(bp).z >= 0 && ca.Cross(cp).z >= 0) || (ab.Cross(ap).z <= 0 && bc.Cross(bp).z <= 0 && ca.Cross(cp).z <= 0);

// 		Vec3f p = BaryCentric(point);
// 		return p.x > 0 && p.y > 0 && p.z > 0;
	}

	inline Vec3f BaryCentric(Vec3f point)
	{
		_a.z = _b.z = _c.z = point.z = 0;

		Vec3f s[2];
		for (int i = 0; i < 2; i++) 
		{
			s[i][0] = _c[i] - _a[i];
			s[i][1] = _b[i] - _a[i];
			s[i][2] = _a[i] - point[i];
		}
		Vec3f u = s[0].Cross(s[1]);
		if (std::abs(u[2]) > 1e-2)
		{
			return Vec3f(1.0 - (u.x + u.y) / u.z, u.y / u.z, u.x / u.z);
		}
		return Vec3f(-1, 1, 1);
	}
};

const int DEFAULT_ALLOC = 4;
struct Matrix
{
	std::vector<std::vector<float>> m;
	int rows, cols;
public:
	Matrix(int r = DEFAULT_ALLOC, int c = DEFAULT_ALLOC);
	inline int nrows();
	inline int ncols();

	static Matrix identity(int dimensions);
	std::vector<float>& operator[](const int i);
	Matrix operator*(const Matrix& a);
	Matrix operator*(const Vec3f& a);
	Matrix transpose();
	Matrix inverse();

	void SetCol(int col, Vec2f vec)
	{
		assert(nrows() == 2);
		m[0][col] = vec.x;
		m[1][col] = vec.y;
	}

	friend std::ostream& operator<<(std::ostream& s, Matrix& m);
};

template <> Vec3<float>::Vec3(Matrix& m);

#endif //__GEOMETRY_H__
