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

	float Vector2Cross(Vector2 a, Vector2 b);

	float Vector3XZAngle(Vector3 pos);

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
	// 力
	Vector3 force_;
	// 質量
	float mass_;
};