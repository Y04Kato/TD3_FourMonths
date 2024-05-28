#include "Physics.h"

#include <cmath>

void Physics::Initialize(float mass)
{
	mass_ = mass;
	velocity_ = { 0.0f, 0.0f, 0.0f };
	acceleration_ = { 0.0f, 0.0f, 0.0f };
	force_ = { 0.0f, 0.0f, 0.0f };
}

Vector3 Physics::Update()
{
	acceleration_ = gravity_;
	acceleration_ += Multiply(1.0f / mass_, force_);
	force_ = { 0.0f, 0.0f, 0.0f };
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
		impulse_ += /* force / mass_;*/Multiply(deltaTime_, force / mass_);
		//velocity_ += acceleration * deltaTime_;
	}

}

Vector3 Physics::GetImpulse_()
{
	Vector3 result = impulse_;
	for (uint32_t i = 0; i < 3; i++) {
		impulse_.num[i] = 0.0f;
	}

	return result;
}

float Physics::Vector2Cross(const Vector2& v1, const Vector2& v2)
{
	return v1.num[0] * v2.num[1] - v1.num[1] * v2.num[0];
}

float Physics::Vector3XZAngle(const Vector3& v)
{
	return std::atan2(v.num[2], v.num[0]) * (180.0f / float(M_PI));
}

Vector2 Physics::Vector2Perpendicular()
{
	return { -velocity_.num[2], velocity_.num[0] };
}

Vector2 Physics::Vector2Normalize(const Vector2& v)
{
	float len = std::sqrtf(v.num[0] * v.num[0] + v.num[1] * v.num[1]);
	if (len == 0) {
		return { 0.0f, 0.0f };
	}
	return { v.num[0] / len, v.num[1] / len };
}

float Physics::Vector3Angle(const Vector3& v1, const Vector3& v2)
{
	float dot = Dot(v1, v2);
	float lenght = Length(v1) * Length(v2);

	if (lenght == 0) {
		return 0.0f;
	}

	float cosTheta = dot / lenght;
	cosTheta = std::fmax(-1.0f, std::fmin(1.0f, cosTheta));

	float angleRadians = std::acos(cosTheta);
	return angleRadians * float(180.0f / M_PI);
}

void Physics::Vector3Direction(const Vector3& v, Vector3* forward, Vector3* right)
{
	Vector3 result = v;
	result.num[1] = 0.0f;
	*forward = Normalize(result);

	result = { -result.num[2], 0.0f, result.num[0] };
	result = Multiply(-1.0f, result);
	*right = Normalize(result);
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

