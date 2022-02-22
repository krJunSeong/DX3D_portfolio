#pragma once

class Vector2 : public Float2
{
public:
	Vector2();
	Vector2(float x, float y);
	Vector2(Float2 value);

	Vector2 operator+ (const Vector2& value) const;//맴버변수를 수정하지 않겠다
	Vector2 operator- (const Vector2& value) const;	
	Vector2 operator* (const Vector2& value) const;
	Vector2 operator/ (const Vector2& value) const;

	Vector2 operator* (const float& value) const;
	Vector2 operator/ (const float& value) const;

	void operator+= (const Vector2& value);
	void operator-= (const Vector2& value);

	void operator*= (const float& value);
	void operator/= (const float& value);

	float Length() const;

	void Normalize();
	Vector2 Normalized() const;

	float Angle() const;

	static float Dot(const Vector2& v1, const Vector2& v2);

	static Vector2 TransformCoord(const Vector2& vec, const Matrix& mat);

	static float Distance(const Vector2& v1, const Vector2& v2);
};