#pragma once
#include "MathCalculation.h"
#include "ViewProjection.h"

class Physics {
public:

	void Initialize(float mass);

	Vector3 Update();

	Vector3 CheckForSwingPoints(const ViewProjection& viewProjection, const Vector3& maxDistance, uint32_t mask);

	Vector3 RubberMovement(const Vector3& start, const Vector3& end, float stiffness, float dampingCoefficient);

	void AddForce(const Vector3& force, uint32_t mode = 0);

	void SetGravity(const Vector3& gravity) { gravity_ = gravity; }

	Vector3 GetVelocity() { return velocity_; }

	Vector3 GetImpulse_();

	float Vector2Cross(const Vector2& v1, const Vector2& v2);

	float Vector3XZAngle(const Vector3& v);

	Vector2 Vector2Perpendicular();

	Vector2 Vector2Normalize(const Vector2& v);

	float Vector3Angle(const Vector3& v1, const Vector3& v2);

	void Vector3Direction(const Vector3& v, Vector3* forward, Vector3* right);

public:
	// デルタタイム
	float deltaTime_ = 1.0f / 60.0f;

private:
	// 重力
	Vector3 gravity_ = { 0.0f, -9.8f, 0.0f };
	// 速度
	Vector3 velocity_;
	// 加速度
	Vector3 acceleration_;
	// 衝撃
	Vector3 impulse_;
	// 力
	Vector3 force_;
	// 質量
	float mass_;
};