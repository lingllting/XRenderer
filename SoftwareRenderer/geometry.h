#ifndef __GEOMETRY_H__
#define __GEOMETRY_H__

#include <cmath>
#include <iostream>

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
	inline Vec2<T> operator +(const Vec2<T> &V) const { return Vec2<T>(u+V.u, v+V.v); }
	inline Vec2<T> operator -(const Vec2<T> &V) const { return Vec2<T>(u-V.u, v-V.v); }
	inline Vec2<T> operator *(float f)          const { return Vec2<T>(u*f, v*f); }
	template <class > friend std::ostream& operator<<(std::ostream& s, Vec2<T>& v);

};

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
	inline Vec3<T> operator ^(const Vec3<T> &v) const { return Vec3<T>(y*v.z-z*v.y, z*v.x-x*v.z, x*v.y-y*v.x); }
	inline Vec3<T> operator +(const Vec3<T> &v) const { return Vec3<T>(x+v.x, y+v.y, z+v.z); }
	inline Vec3<T> operator -(const Vec3<T> &v) const { return Vec3<T>(x-v.x, y-v.y, z-v.z); }
	inline Vec3<T> operator *(float f)          const { return Vec3<T>(x*f, y*f, z*f); }
	inline T       operator *(const Vec3<T> &v) const { return x*v.x + y*v.y + z*v.z; }
    inline Vec3<T> Cross(const Vec3<T> &v)      const { return Vec3<T>(y*v.z-z*v.y, z*v.x-x*v.z, x*v.y-y*v.x); }
	float norm  () const { return std::sqrt(x*x+y*y+z*z); }
	Vec3<T>& normalize(T l=1) { *this = (*this)*(l/norm()); return *this; }
	template <class > friend std::ostream& operator<<(std::ostream& s, Vec3<T>& v);
};

typedef Vec2<float> Vec2f;
typedef Vec2<int>   Vec2i;
typedef Vec3<float> Vec3f;
typedef Vec3<int>   Vec3i;

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
    Vec2i min;
    Vec2i max;
    
    AABB(){}
};

struct Triangle
{
private:
    Vec2i _a;
    Vec2i _b;
    Vec2i _c;
public:
    Triangle() : _a(Vec2<int>()), _b(Vec2<int>()), _c(Vec2<int>()){}
    Triangle(Vec2i a, Vec2i b, Vec2i c) : _a(a), _b(b), _c(c){}
    Triangle(Vec2i* vertices) : _a(vertices[0]), _b(vertices[1]), _c(vertices[2]){}
    
    inline bool Contains(Vec2i point) const
    {
        Vec3i ab = _b - _a;
        Vec3i ap = point - _a;
        Vec3i bc = _c - _b;
        Vec3i bp = point - _b;
        Vec3i ca = _a - _c;
        Vec3i cp = point - _c;
        
        return (ab.Cross(ap).z > 0 && bc.Cross(bp).z > 0 && ca.Cross(cp).z > 0) || (ab.Cross(ap).z < 0 && bc.Cross(bp).z < 0 && ca.Cross(cp).z < 0);
    }
};

#endif //__GEOMETRY_H__
