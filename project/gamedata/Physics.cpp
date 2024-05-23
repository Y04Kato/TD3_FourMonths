#include "Physics.h"

#include <cmath>

void Physics::Initialize(float mass)
{
	mass_ = mass;
	velocity_ = { 0.0f, 0.0f, 0.0f };
	acceleration_ = { 0.0f, 0.0f, 0.0f };
	force_ = { 0.0f, 0.0f, 0.0f};
}

Vector3 Physics::Update()
{
	acceleration_ = gravity_;
	acceleration_ += Multiply(1.0f / mass_, force_);
	/*acceleration_ = Multiply(deltaTime_, gravity_);
	acceleration_ += Multiply(deltaTime_, force_ / mass_);*/
	velocity_ += acceleration_ * deltaTime_;
	return velocity_;
}

Vector3 Physics::CheckForSwingPoints(const ViewProjection& viewProjection, const Vector3& maxDistance, uint32_t mask)
{
	return Vector3();
}

void Physics::AddForce(const Vector3& force, uint32_t mode)
{
	if (mode == 0) {
		force_ += force;
	}
	else {
		Vector3 acceleration = Multiply(deltaTime_, force_ / mass_);
		velocity_ += acceleration_ * deltaTime_;
	}

}

float Physics::Vector2Cross(Vector2 a, Vector2 b)
{
	return a.num[0] * b.num[1] - a.num[1] * b.num[0];
}

float Physics::Vector3XZAngle(Vector3 pos)
{
	return std::atan2(pos.num[2], pos.num[0]) * (180.0f / float(M_PI));
}

Vector3 Physics::RubberMovement(const Vector3& start, const Vector3& end, float stiffness, float dampingCoefficient)
{
	Vector3 diff = start - end;
	float length = Length(diff);
	//length = 100f;
	if (length != 0.0f)
	{
		if (length > 5.0f)
		{
			length = 5.0f;
		}
		/*else if (length < 50.0f)
		{
			length = 50.0f;
		}*/
		Vector3 direction = Normalize(diff);
		Vector3 displacement = Multiply(length, (start - end));
		Vector3 restoringForce = Multiply(-stiffness, displacement);
		Vector3 dampingForce = Multiply(-dampingCoefficient, velocity_);
		Vector3 force = restoringForce + dampingForce;
		return force;
	}
	return Vector3();
}

