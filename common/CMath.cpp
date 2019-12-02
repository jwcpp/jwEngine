#include "CMath.h"
#include <math.h>


#define ABS_FLOAT_0 0.0001
#define EPS 0.00001

CMath::CMath()
{
}


CMath::~CMath()
{
}

float CMath::GetTriangleSquar(const Vector2 & p1, const Vector2 & p2, const Vector2 & p3)
{
	Vector2 AB, BC;
	AB.x = p2.x - p1.x;
	AB.y = p2.y - p1.y;
	BC.x = p3.x - p2.x;
	BC.y = p3.y - p2.y;
	return fabs((AB.x * BC.y - AB.y * BC.x)) / 2.0f;

	//任意2边向量的叉积的绝对值的1 / 2即为三角形的面积
}


bool CMath::IsInTriangle(const Vector2 & p1, const Vector2 & p2, const Vector2 & p3, const Vector2 & testp)
{
	float SABC, SADB, SBDC, SADC;
	SABC = GetTriangleSquar(p1, p2, p3);
	SADB = GetTriangleSquar(p1, testp, p2);
	SBDC = GetTriangleSquar(p2, testp, p3);
	SADC = GetTriangleSquar(p1, testp, p3);

	float SumSuqar = SADB + SBDC + SADC;

	if ((-ABS_FLOAT_0 < (SABC - SumSuqar)) && ((SABC - SumSuqar) < ABS_FLOAT_0))
	{
		return true;
	}
	else
	{
		return false;
	}
}

float CMath::GetAngle(const Vector2 & a, const Vector2 & b)
{
	if (a.x == b.x && a.y == b.y)
		return 0;

	float chu1 = (a.x * b.x + a.y * b.y);
	float chu2 = (sqrt(a.x * a.x + a.y * a.y) * sqrt(b.x * b.x + b.y * b.y));

	float tmp = chu2 == 0 ? 0 : chu1 / chu2;

	float angle = acos(tmp) * 180.0 / 3.1415926;

	return angle;
}


float CMath::GetPolygonSquar(const Vector2 * arr, int len)
{
	if (len < 3)
		return 0;

	int upIndex = 1;
	float sum = 0;

	for (int i = upIndex + 1; i < len; i++)
	{
		sum += GetTriangleSquar(arr[0], arr[upIndex], arr[i]);
		upIndex = i;
	}

	return sum;
}

double CMath::mult(Vector2 & a, Vector2 & b, Vector2 & c)
{
	return (a.x - c.x)*(b.y - c.y) - (b.x - c.x)*(a.y - c.y);
}

bool CMath::isIntersect(Vector2 & aa, Vector2 & bb, Vector2 & cc, Vector2 & dd)
{
	if (MAX(aa.x, bb.x)<MIN(cc.x, dd.x))
	{
		return false;
	}
	if (MAX(aa.y, bb.y)<MIN(cc.y, dd.y))
	{
		return false;
	}
	if (MAX(cc.x, dd.x)<MIN(aa.x, bb.x))
	{
		return false;
	}
	if (MAX(cc.y, dd.y)<MIN(aa.y, bb.y))
	{
		return false;
	}
	if (mult(cc, bb, aa)*mult(bb, dd, aa)<0)
	{
		return false;
	}
	if (mult(aa, dd, cc)*mult(dd, bb, cc)<0)
	{
		return false;
	}
	return true;
}


bool CMath::isIntersect(Line<Vector2> & l1, Line<Vector2> & l2)
{
	return isIntersect(l1.pos[0], l1.pos[1], l2.pos[0], l2.pos[1]);
}

int CMath::pLineLR(Vector2 & p, Line<Vector2> & line)
{
	Vector2 * p1 = &(line.pos[0]), *p2 = &(line.pos[1]), * p3 = &p;

	//类似于求三角形的面积(叉积)
	Vector2 AB, BC;
	AB.x = p2->x - p1->x;
	AB.y = p2->y - p1->y;
	BC.x = p3->x - p2->x;
	BC.y = p3->y - p2->y;
	return AB.x * BC.y - AB.y * BC.x;
}

float CMath::getDistance(Vector2 & a, Vector2 & b)
{
	float x = a.x - b.x;
	float y = a.y - b.y;

	
	return sqrt(x * x + y * y);
}

Vector2 CMath::getLineCorePoint(Line<Vector2> & line)
{
	return Vector2((line.pos[0].x + line.pos[1].x) / 2, (line.pos[0].y + line.pos[1].y) / 2);
}

Vector3 CMath::getLineCorePoint(Line<Vector3> & line)
{
	return Vector3((line.pos[0].x + line.pos[1].x) / 2, (line.pos[0].y + line.pos[1].y) / 2, (line.pos[0].z + line.pos[1].z) / 2);
}

Vector2 CMath::getTriangleCorePoint(Vector2 & p1, Vector2 & p2, Vector2 & p3)
{
	return Vector2((p1.x + p2.x + p3.x) / 3, (p1.y + p2.y + p3.y) / 3);
}

/**
* @brief 求线段与圆的交点
* @return 如果有交点返回true,否则返回false
* @note 与圆可能存在两个交点，如果存在两个交点在ptInter1和ptInter2都为有效值，如果有一个交点，则ptInter2的值为
*       无效值，此处为65536.0
*/
bool CMath::LineInterCircle(
	Vector2 & ptStart, // 线段起点
	Vector2 & ptEnd, // 线段终点
	Vector2 & ptCenter, // 圆心坐标
	const float Radius,
	Vector2 & ptInter1,
	Vector2 & ptInter2)
{
	ptInter1.x = ptInter2.x = 65536.0f;
	ptInter2.y = ptInter2.y = 65536.0f;

	float fDis = sqrt((ptEnd.x - ptStart.x) * (ptEnd.x - ptStart.x) + (ptEnd.y - ptStart.y) * (ptEnd.y - ptStart.y));

	Vector2 d;
	d.x = (ptEnd.x - ptStart.x) / fDis;
	d.y = (ptEnd.y - ptStart.y) / fDis;

	Vector2 E;
	E.x = ptCenter.x - ptStart.x;
	E.y = ptCenter.y - ptStart.y;

	float a = E.x * d.x + E.y * d.y;
	float a2 = a * a;

	float e2 = E.x * E.x + E.y * E.y;

	float r2 = Radius * Radius;

	if ((r2 - e2 + a2) < 0)
	{
		return false;
	}
	else
	{
		bool ret = false;

		float f = sqrt(r2 - e2 + a2);

		float t = a - f;

		if (((t - 0.0) > -EPS) && (t - fDis) < EPS)
		{
			ptInter1.x = ptStart.x + t * d.x;
			ptInter1.y = ptStart.y + t * d.y;

			ret = true;
		}

		t = a + f;

		if (((t - 0.0) > -EPS) && (t - fDis) < EPS)
		{
			ptInter2.x = ptStart.x + t * d.x;
			ptInter2.y = ptStart.y + t * d.y;

			ret = true;
		}

		return ret;
	}
}

Vector2 CMath::getCross(Vector2 & u1, Vector2 & u2, Vector2 & v1, Vector2 & v2){
	Vector2 ret = u1;
	double t = ((u1.x - v1.x)*(v1.y - v2.y) - (u1.y - v1.y)*(v1.x - v2.x))
		/ ((u1.x - u2.x)*(v1.y - v2.y) - (u1.y - u2.y)*(v1.x - v2.x));
	ret.x += (u2.x - u1.x)*t;
	ret.y += (u2.y - u1.y)*t;
	return ret;
}