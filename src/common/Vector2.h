#ifndef _VECTOR2_H_
#define _VECTOR2_H_

//#include <stdlib.h>
#include <string>
#include <math.h>

class Vector2
{
public:
	float x, y;
	void initData()
	{
		x = 0.0f;
		y = 0.0f;
	}

public:
	// construction
	Vector2()
	{
		initData();
	}

	Vector2(float x, float y)
	{
		this->x = x;
		this->y = y;
	}

	Vector2(float coordinate[2])
	{
		this->x = coordinate[0];
		this->y = coordinate[1];
	}

	Vector2(double coordinate[2])
	{
		this->x = (float)coordinate[0];
		this->y = (float)coordinate[1];
	}

	Vector2(const Vector2& v)
	{
		this->x = v.x;
		this->y = v.y;
	}

	bool operator<(const Vector2& v) const
	{
		return this->Length() < v.Length();
	}

	bool operator>(const Vector2& v) const
	{
		return this->Length() > v.Length();
	}

	Vector2& operator= (const Vector2& v)
	{
		this->x = v.x;
		this->y = v.y;

		return *this;
	}

	bool operator== (const Vector2& v) const
	{
		return std::abs(this->x - v.x) < 0.001f && std::abs(this->y - v.y) < 0.001f;
	}

	bool operator!= (const Vector2& v) const
	{
		return std::abs(this->x - v.x) >= 0.001f || std::abs(this->y - v.y) >= 0.001f;
	}
	//----------------------------------------------------------------------------
	// Arithmetic Operations
	Vector2 operator+ (const Vector2& v) const
	{
		Vector2 xV;

		xV.x = this->x + v.x;
		xV.y = this->y + v.y;
		return xV;
	}

	Vector2 operator- (const Vector2& v) const
	{
		Vector2 xV;

		xV.x = this->x - v.x;
		xV.y = this->y - v.y;
		return xV;
	}

	Vector2 operator- () const
	{
		return Vector2(-x, -y);
	}

	Vector2 operator* (float s) const
	{
		return Vector2(x * s, y * s);
	}

	Vector2 operator/ (float s) const
	{
		if (std::abs(s) > 0.001f)
		{
			return Vector2(x / s, y / s);
		}

		return Zero();
	}

	//----------------------------------------------------------------------------
	// Arithmetic Updates
	Vector2& operator+= (const Vector2& v)
	{
		x += v.x;
		y += v.y;
		return *this;
	}

	Vector2& operator-= (const Vector2 v)
	{
		x -= v.x;
		y -= v.y;
		return *this;
	}

	Vector2& operator*= (float s)
	{
		x *= s;
		y *= s;
		return *this;
	}

	Vector2 operator/= (float s)
	{
		//if (std::abs(s) > 0.001f)
		{
			return Vector2(x / s, y / s);
		}

		//return Zero();
	}

	float X() const
	{
		return this->x;
	}

	float Y() const
	{
		return this->y;
	}

	void SetX(float x)
	{
		this->x = x;
	}

	void SetY(float y)
	{
		this->y = y;
	}

	bool IsZero() const
	{
		return x < 0.001f && y < 0.001f;
	}

	//----------------------------------------------------------------------------
	inline float SquaredMagnitude() const
	{
		return x*x + y*y;
	}

	//----------------------------------------------------------------------------
	inline float SquaredLength() const
	{
		return SquaredMagnitude();
	}

	//----------------------------------------------------------------------------
	inline float Magnitude() const
	{
		return sqrtf(x*x + y*y);
	}

	//----------------------------------------------------------------------------
	inline float Length() const
	{
		return Magnitude();
	}

	//----------------------------------------------------------------------------
	inline Vector2 Direction() const
	{
		if (this->IsZero())
		{
			return Zero();
		}

		float lenSquared = SquaredMagnitude();
		float invSqrt = 1.0f / sqrtf(lenSquared);
		return Vector2(x * invSqrt, y * invSqrt);
	}

	//----------------------------------------------------------------------------
	inline Vector2 Normalized() const
	{
		return Direction();
	}

	//----------------------------------------------------------------------------
	float Distance(const Vector2& v) const
	{
		Vector2 vX = *this - v;
		return vX.Length();
	}


	// Special values.
	inline static const Vector2& Zero()
	{
		static Vector2 v(0, 0);
		return v;
	}

	inline static const Vector2& One()
	{
		static Vector2 v(1, 1);
		return v;
	}

	inline static const Vector2& UnitX()
	{
		static Vector2 v(1, 0);
		return v;
	}

	inline static const Vector2& UnitY()
	{
		static Vector2 v(0, 1);
		return v;
	}
};

#endif