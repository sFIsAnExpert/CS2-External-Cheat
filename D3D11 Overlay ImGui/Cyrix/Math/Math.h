#pragma once
#include "../../pch.h"

#define DegreesToRadians(a) ((a)*((float)M_PI/180.0f))
#define RadiansToDegrees(a) ((a)*(180.0f/(float)M_PI))
#define AngleToShort(a) ((int)((a)*(65536/360.0f))&65535)
#define ShortToAngle(a) ((float)((a)*(360.0f/65536)))
#define M_PI           3.14159265358979323846

struct ViewMatrix {
	float matrix[4][4];
};

struct Vector4 {
	float x, y, z, w;
};

struct Vector2 {
	float x, y;

	inline void operator-=(const Vector2& b) {
		this->x -= b.x; this->y -= b.y;
	}
};

struct Vector3 {
	float x, y, z;
	inline void operator-=(const Vector3& b) {
		this->x -= b.x; this->y -= b.y; this->z -= b.z;
	}
	inline void operator-=(const Vector2& b) {
		this->x -= b.x; this->y -= b.y;
	}
	inline void operator*=(const Vector3& b) {
		this->x *= b.x; this->y *= b.y; this->z *= b.z;
	}
	inline void operator*=(const float& b) {
		this->x *= b; this->y *= b; this->z *= b;
	}
	inline void operator+=(const Vector3& b) {
		this->x += b.x; this->y += b.y; this->z += b.z;
	}
	inline Vector3 operator-(const Vector3& b) {
		Vector3 ret;
		ret.x = this->x -= b.x; ret.y = this->y -= b.y; ret.z = this->z -= b.z;
		return ret;
	}
};

struct Resolution {
	int width, height;
};

struct XMath {
	float Dist2D(Vector2 from, Vector2 to);
	float Dist3D(Vector3 from, Vector3 to);
	float DotProduct3D(Vector3 left, Vector3 right);
	float DotProduct2D(Vector2 left, Vector2 right);
	float VectorLength3D(Vector3 left, Vector3 right);
	float VectorLength2D(Vector2 left, Vector2 right);
	void ClampAngles(Vector3& angles);
	void VectorNormalize(Vector3& direction);
	void VectorAngles(Vector3 direction, Vector3& angles);
	void CalculateAimAngles(Vector3* vTargetPos, Vector3* vCameraPos, Vector3* vAngles);
};

extern XMath math;

bool WorldToScreen(Vector3 pos, Vector2& screen, ViewMatrix vm, Resolution res);

bool WorldToScreen(Vector3 pos, ImVec2& screen, ViewMatrix vm, Resolution res);

void Draw3DBox(Vector3 HeadPos, Vector3 Pos, ViewMatrix vm, Resolution res, ImColor color);

void DrawNormalBox(float x, float y, float w, float h, float thickness, ImVec4 borderColor);

void DrawFilledBox(float x, float y, float w, float h, ImVec4 color);