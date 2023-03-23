#include "Vec2.h"
#include <math.h>

Vec2::Vec2()
{
}

Vec2::Vec2(float xin, float yin) : x(xin), y(yin)
{
}

bool Vec2::operator==(const Vec2& rhs) const
{

	return (x == rhs.x && y == rhs.y);
}

bool Vec2::operator!=(const Vec2& rhs) const
{
	return (x != rhs.x && y != rhs.y);
}

Vec2 Vec2::operator+(const Vec2& rhs) const
{
	return Vec2(x + rhs.x, y + rhs.y);
}

Vec2 Vec2::operator-(const Vec2& rhs) const
{
	return Vec2(x - rhs.x, y - rhs.y);
}

Vec2 Vec2::operator/(const float val) const
{
	return Vec2(x/val, y/val);
}

Vec2 Vec2::operator*(const float val) const
{
	return Vec2(x*val, y*val);
}

void Vec2::operator+=(const Vec2& rhs)
{
	x += rhs.x;
	y += rhs.y;
}

void Vec2::operator-=(const Vec2& rhs)
{
	x -= rhs.x;
	y -= rhs.y;
}

void Vec2::operator*=(const float val)
{
	x *= val;
	y *= val;
}

void Vec2::operator/=(const float val)
{
	x /= val;
	y /= val;
}

std::ostream& operator<<(std::ostream& stream, const Vec2& vec)
{
	stream << "(" << vec.x << "," << vec.y << ")";
	return stream;
}

Vec2 Vec2::Zero() const
{
	return Vec2(0.0f, 0.0f);
}

float Vec2::Dist(const Vec2& rhs) const
{
	Vec2 D = Vec2(rhs.x - x, rhs.y - y);
	return sqrtf(D.x * D.x + D.y * D.y);
}

float Vec2::Magnitude() const
{
	return sqrt(x * x + y * y);
}

float Vec2::MagnitudeSqr() const
{
	return x * x + y * y;
}

Vec2 Vec2::Normalize() const
{
	float mag = Magnitude();

	return Vec2(x / mag, y / mag);
}

Vec2 Vec2::RotateVector(const Vec2& vec, float angle) const
{
	float radAngle = angle * 3.14159265f / 180.0f;
	return Vec2((vec.x * cosf(radAngle) - vec.y * sinf(radAngle)), (vec.x * sinf(radAngle) + vec.y * cosf(radAngle)));
}

float Vec2::Dot(const Vec2& vec1, const Vec2& vec2) const
{
	return vec1.x * vec2.x + vec1.y * vec2.y;
}

float Vec2::Clamp(const float& value, const float& min, const float& max) const
{
	if (value > max)
	{
		return max;
	}
	if (value < min)
	{
		return min;
	}
	return value;
}

Vec2 Vec2::Lerp(Vec2& start, Vec2& end, float time) const
{
	if (time <= 0.0f)
	{
		return start;
	}

	if (time >= 1.0f)
	{
		return end;
	}

	Vec2 dir = (end - start).Normalize();
	float mag = (end - start).Magnitude();

	return start + dir * mag * time;
}