#pragma once
#include "Vec3.h"
#include "Vec4.h"
#include <cstring>
#include <cmath>

// Column-major 4x4 matrix (matches OpenGL convention)
struct Mat4 {
    float m[4][4] = {};

    Mat4() { setIdentity(); }

    void setIdentity() {
        for (int r=0;r<4;r++) for (int c=0;c<4;c++) m[r][c]=(r==c)?1.f:0.f;
    }

    float* data() { return &m[0][0]; }
    const float* data() const { return &m[0][0]; }

    // Matrix multiply
    Mat4 operator*(const Mat4& o) const {
        Mat4 res; res.setIdentity();
        for (int r=0;r<4;r++)
            for (int c=0;c<4;c++) {
                res.m[r][c]=0.f;
                for (int k=0;k<4;k++) res.m[r][c]+=m[r][k]*o.m[k][c];
            }
        return res;
    }

    Vec4 operator*(const Vec4& v) const {
        return {
            m[0][0]*v.x+m[0][1]*v.y+m[0][2]*v.z+m[0][3]*v.w,
            m[1][0]*v.x+m[1][1]*v.y+m[1][2]*v.z+m[1][3]*v.w,
            m[2][0]*v.x+m[2][1]*v.y+m[2][2]*v.z+m[2][3]*v.w,
            m[3][0]*v.x+m[3][1]*v.y+m[3][2]*v.z+m[3][3]*v.w
        };
    }

    // ── Factories ─────────────────────────────────────────────────────────────
    static Mat4 identity() { return Mat4{}; }

    static Mat4 translate(const Vec3& t) {
        Mat4 r;
        r.m[0][3]=t.x; r.m[1][3]=t.y; r.m[2][3]=t.z;
        return r;
    }

    static Mat4 scale(const Vec3& s) {
        Mat4 r;
        r.m[0][0]=s.x; r.m[1][1]=s.y; r.m[2][2]=s.z;
        return r;
    }

    static Mat4 scale(float s) { return scale({s,s,s}); }

    static Mat4 rotateX(float a) {
        Mat4 r;
        r.m[1][1]= std::cos(a); r.m[1][2]=-std::sin(a);
        r.m[2][1]= std::sin(a); r.m[2][2]= std::cos(a);
        return r;
    }
    static Mat4 rotateY(float a) {
        Mat4 r;
        r.m[0][0]= std::cos(a); r.m[0][2]= std::sin(a);
        r.m[2][0]=-std::sin(a); r.m[2][2]= std::cos(a);
        return r;
    }
    static Mat4 rotateZ(float a) {
        Mat4 r;
        r.m[0][0]= std::cos(a); r.m[0][1]=-std::sin(a);
        r.m[1][0]= std::sin(a); r.m[1][1]= std::cos(a);
        return r;
    }

    // Perspective projection (fovY in radians, aspect = w/h)
    // Parameters named zNear/zFar to avoid MSVC 'near'/'far' reserved-word conflict
    static Mat4 perspective(float fovY, float aspect, float zNear, float zFar) {
        float tanHalf = std::tan(fovY * 0.5f);
        Mat4 r; r.m[0][0]=r.m[1][1]=r.m[2][2]=r.m[2][3]=r.m[3][2]=0.f; r.m[3][3]=0.f;
        r.m[0][0] =  1.f / (aspect * tanHalf);
        r.m[1][1] =  1.f / tanHalf;
        r.m[2][2] = -(zFar + zNear) / (zFar - zNear);
        r.m[2][3] = -(2.f * zFar * zNear) / (zFar - zNear);
        r.m[3][2] = -1.f;
        return r;
    }

    // Orthographic projection
    static Mat4 ortho(float l, float r2, float b, float t, float n, float f) {
        Mat4 res; res.m[3][3]=1.f;
        res.m[0][0]=2.f/(r2-l);  res.m[0][3]=-(r2+l)/(r2-l);
        res.m[1][1]=2.f/(t-b);   res.m[1][3]=-(t+b)/(t-b);
        res.m[2][2]=-2.f/(f-n);  res.m[2][3]=-(f+n)/(f-n);
        return res;
    }

    // LookAt view matrix
    static Mat4 lookAt(Vec3 eye, Vec3 center, Vec3 up2) {
        Vec3 f = (center-eye).normalized();
        Vec3 s = f.cross(up2).normalized();
        Vec3 u = s.cross(f);
        Mat4 r;
        r.m[0][0]=s.x; r.m[0][1]=s.y; r.m[0][2]=s.z; r.m[0][3]=-s.dot(eye);
        r.m[1][0]=u.x; r.m[1][1]=u.y; r.m[1][2]=u.z; r.m[1][3]=-u.dot(eye);
        r.m[2][0]=-f.x;r.m[2][1]=-f.y;r.m[2][2]=-f.z;r.m[2][3]= f.dot(eye);
        r.m[3][0]=0.f; r.m[3][1]=0.f; r.m[3][2]=0.f; r.m[3][3]=1.f;
        return r;
    }
};
