#ifndef _CMATH_H_
#define _CMATH_H_

#include <stdio.h>
#include <stdlib.h>
#include "XAssert.h"
#include "Vector2.h"
#include "Vector3.h"

#define MIN(x,y) (x <= y ? x : y)
#define MAX(x,y) (x >= y ? x : y)


//T type: Vector2 or Vector3
template<class T> class LineParent
{
public:
	LineParent()
	{
		XAssert(0, "line template param error");
	}

};

template <>
class LineParent<Vector2>{};
template <>
class LineParent<Vector3>{};


template<typename T>
class Line : public LineParent<T>
{
public:
	Line(){
		
	}
	Line(T & p1, T & p2)
	{
		pos[0] = p1;
		pos[1] = p2;

	}

	void typeOk()
	{
		static_assert((std::is_same<T, Vector2>::value)
			|| (std::is_same<T, Vector3>::value)
			, "not Vector2 or Vector3");
	}

	bool operator == (Line &line)
	{
		if ((pos[0] == line.pos[0] && pos[1] == line.pos[1])
			|| (pos[0] == line.pos[1] && pos[1] == line.pos[0]))
		{
			return true;
		}
		return false;
	}

	T pos[2];
};


class CMath
{
public:
	CMath();
	~CMath();

	//判断点在三角形内部
	static bool IsInTriangle(const Vector2 & p1, const Vector2 & p2, const Vector2 & p3, const Vector2 & testp);

	//获取三角形的面积
	static float GetTriangleSquar(const Vector2 & p1, const Vector2 & p2, const Vector2 & p3);

	//获取多边形面积
	static float GetPolygonSquar(const Vector2 * arr, int len);

	//知道两个向量求夹角
	static float GetAngle(const Vector2 & a, const Vector2 & b);


	//判断两条线段是否相交
	static bool isIntersect(Vector2 & aa, Vector2 & bb, Vector2 & cc, Vector2 & dd);
	static bool isIntersect(Line<Vector2> & l1, Line<Vector2> & l2);
	static double mult(Vector2 & a, Vector2 & b, Vector2 & c);//叉积

	//判断点在线段的左边还是右边（当P1P2P3逆时针时S为正的，当P1P2P3顺时针时S为负的）
	static int pLineLR(Vector2 & p, Line<Vector2> & line);

	//求两点之间的距离
	static float getDistance(Vector2 & a, Vector2 & b);

	//求线段中心点坐标
	static Vector2 getLineCorePoint(Line<Vector2> & line);
	static Vector3 getLineCorePoint(Line<Vector3> & line);

	//求三角形的中心点
	static Vector2 getTriangleCorePoint(Vector2 & p1, Vector2 & p2, Vector2 & p3);

	//圆和线段的焦点
	static bool LineInterCircle(Vector2 & ptStart, Vector2 & ptEnd, Vector2 & ptCenter, const float Radius, Vector2 & ptInter1, Vector2 & ptInter2);

	//计算两条直线的交点
	static Vector2 getCross(Vector2 & u1, Vector2 & u2, Vector2 & v1, Vector2 & v2);
};

#endif