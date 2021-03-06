#pragma once
#ifndef MATH_H
#define MATH_H
#ifndef TUPLES_H
#include "Tuples.h"
#endif


class Img;

struct s_rgba {
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;
};

struct s_2pt_i {
	long x0;
	long x1;
};
struct s_2pt {
	float x0;
	float x1;
};
namespace utilStruct {
	inline void zeroRGBA(s_rgba& rgba) { rgba.r = 0x00; rgba.g = 0x00; rgba.b = 0x00; rgba.a = 0x00; }
	inline void zero2pt_i(s_2pt_i& spt) { spt.x0 = 0; spt.x1 = 0; }
	inline void zero2pt(s_2pt& pt) { pt.x0 = 0.f; pt.x1 = 0.f; }
}
namespace Math {
	float StepFunc(float x);/* 1/(1+exp(-x))  takes an x value and returns a step function where - x goes to 0, 0 goes to 1/2 and +x goes to 1*/
	int loop(int i, int n);
	float power(float x, int y);
}
namespace vecMath {
	s_2pt add(const s_2pt& v1, const s_2pt& v2);
	s_2pt add(const s_2pt& v1, const s_2pt& v2, const s_2pt& v3);
	s_2pt v12(const s_2pt& v1, const s_2pt& v2);
	Tup3 v12(const Tup3& v1, const Tup3& v2);
	float len(const s_2pt& v);
	float len(const Tup3& v);
	float dist(const s_2pt& v1, const s_2pt& v2);
	float dist(const Tup3& v1, const Tup3& v2);
	float dot(s_2pt& v1, s_2pt& v2);
	s_2pt mul(float a, const s_2pt& vec);
	s_2pt perpUR(s_2pt& vec);
	s_2pt perpUL(s_2pt& vec);
	s_2pt convBasis(s_2pt& basisU0, s_2pt& basisU1, s_2pt& vecInBasis);
}
namespace imgMath {
	s_rgba convToRGBA(float r, float g, float b);
	s_2pt_i convToVint(const s_2pt& vec);
	s_2pt convToVfloat(const s_2pt_i& vi);
	void drawPoint(long i, long j, const s_rgba& col, Img* canvas);
	void drawLine(long i_start, long j_start, long i_end, long j_end, const s_rgba& col, float thickness, Img* canvas);
	void drawV(const s_2pt& v, long i, long j, const s_rgba& col, float thickness, Img* canvas);
}
#endif