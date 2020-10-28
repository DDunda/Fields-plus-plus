#pragma once
class Vec2 {
public:
	float x, y;

	Vec2(float, float);
	Vec2(const Vec2&);

	Vec2 operator+(Vec2 b);
	Vec2& operator+=(Vec2 b);

	Vec2 operator-(Vec2 b);
	Vec2& operator-= (Vec2 b);

	Vec2 operator*(float b);
	Vec2& operator*= (float b);

	Vec2 operator/(float b);
	Vec2& operator/= (float b);

	// The size of this vector, squared
	float SqrMagnitude();
	// The size of this vector
	float Magnitude();

	// Normalises a copy of this vector
	Vec2 normalised();
	// Normalises this object, and then returns it
	Vec2& Normalise();

	// [0,0]
	static Vec2 Zero();
	// [1,1]
	static Vec2 One();
	// [-1,0]
	static Vec2 Left();
	// [1,0]
	static Vec2 Right();
	// [0,-1]
	static Vec2 Down();
	// [0,1]
	static Vec2 Up();

	// -pi to pi
	float GetAngleRad();
	// -1 to 1
	float GetAngleNormal();
	// -180 to 180
	float GetAngleDeg();
};