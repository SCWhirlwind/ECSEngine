#pragma once
#include <ostream>
class Vec2
{
public:
	float x = 0.0f;
	float y = 0.0f;

	Vec2();
	Vec2(float xin, float yin);

	bool operator == (const Vec2& rhs) const;
	bool operator != (const Vec2& rhs) const;

	Vec2 operator + (const Vec2& rhs) const;
	Vec2 operator - (const Vec2& rhs) const;
	Vec2 operator / (const float val) const;
	Vec2 operator * (const float val) const;

	void operator += (const Vec2& rhs);
	void operator -= (const Vec2& rhs);
	void operator *= (const float val);
	void operator /= (const float val);

	friend std::ostream& operator<<(std::ostream& stream, const Vec2& vec);

	Vec2 Zero() const;

	float Dist(const Vec2& rhs) const;

	float Magnitude() const;
	float MagnitudeSqr() const;

	Vec2 Normalize() const;

	Vec2 RotateVector(const Vec2& vec, float angle) const;

	float Dot(const Vec2& vec1, const Vec2& vec2) const;

	float Clamp(const float& value, const float& min, const float& max) const;

	Vec2 Lerp(Vec2& start, Vec2& end, float time) const;


};

