#include "Vec2.h"
#include <SDL.h>

Vec2::Vec2(float _x, float _y) : x(_x), y(_y) {}
Vec2::Vec2(const Vec2& v) : x(v.x), y(v.y) {}

Vec2 Vec2::operator+(Vec2 b) {
	return { x + b.x, y + b.y };
}
Vec2& Vec2::operator+=(Vec2 b) {
	x += b.x;
	y += b.y;
	return *this;
}

Vec2 Vec2::operator-(Vec2 b) {
	return { x - b.x, y - b.y };
}
Vec2& Vec2::operator-= (Vec2 b) {
	x -= b.x;
	y -= b.y;
	return *this;
}

Vec2 Vec2::operator*(float b) {
	return { x * b, y * b };
}
Vec2& Vec2::operator*= (float b) {
	x *= b;
	y *= b;
	return *this;
}

Vec2 Vec2::operator/(float b) {
	return { x / b, y / b };
}
Vec2& Vec2::operator/= (float b) {
	x /= b;
	y /= b;
	return *this;
}

float Vec2::SqrMagnitude() {
	return x * x + y * y;
}
float Vec2::Magnitude() {
	return sqrt(x * x + y * y);
}

Vec2 Vec2::normalised() {
	float mag = Magnitude();
	if (mag == 0) return *this;
	return *this / mag;
}
Vec2& Vec2::Normalise() {
	float mag = Magnitude();
	if (mag == 0) return *this;
	return *this /= mag;
}

Vec2 Vec2::Zero() { return { 0,0 }; }
Vec2 Vec2::One() { return { 1,1 }; }

Vec2 Vec2::Left() { return { -1,0 }; }
Vec2 Vec2::Right() { return { 1,0 }; }
Vec2 Vec2::Down() { return { 0,-1 }; }
Vec2 Vec2::Up() { return { 0,1 }; }

float Vec2::GetAngleRad() {
	return atan2(y, x);
}
float Vec2::GetAngleNormal() {
	return atan2(y, x) / M_PI;
}
float Vec2::GetAngleDeg() {
	return atan2(y, x) / M_PI * 180.0;
}