#include "Vector3.h"

DemoSystem::Vector3::Vector3()
{
	this->x = 0.0;
	this->y = 0.0;
	this->z = 0.0;
}

DemoSystem::Vector3::Vector3(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

DemoSystem::Vector3::Vector3(const DemoSystem::Vector3 &param)
{
	this->x = param.x;
	this->y = param.y;
	this->z = param.z;
}

DemoSystem::Vector3 DemoSystem::Vector3::operator+(DemoSystem::Vector3 vec)
{
	return Vector3(this->x + vec.x, this->y + vec.y, this->z + vec.z);
}

DemoSystem::Vector3 DemoSystem::Vector3::operator-(DemoSystem::Vector3 vec)
{
	return Vector3(this->x - vec.x, this->y - vec.y, this->z - vec.z);
}

DemoSystem::Vector3 DemoSystem::Vector3::operator*(float scalar)
{
	return Vector3(this->x * scalar, this->y * scalar, this->z * scalar);
}

DemoSystem::Vector3 DemoSystem::Vector3::operator/(float scalar)
{
	return Vector3(this->x / scalar, this->y / scalar, this->z / scalar);
}

DemoSystem::Vector3 &DemoSystem::Vector3::operator+=(const DemoSystem::Vector3 vec)
{
	this->x = this->x + vec.x;
	this->y = this->y + vec.y;
	this->z = this->z + vec.z;
	return *this;
}

DemoSystem::Vector3 &DemoSystem::Vector3::operator-=(const DemoSystem::Vector3 vec)
{
	this->x = this->x - vec.x;
	this->y = this->y - vec.y;
	this->z = this->z - vec.z;
	return *this;
}

float DemoSystem::Vector3::distance(DemoSystem::Vector3 vec)
{
	return sqrt(pow((this->x + vec.x), 2) + pow((this->y + vec.y), 2) + pow((this->z + vec.z), 2));
}

float DemoSystem::Vector3::distance(float x, float y, float z)
{
	return sqrt(pow((this->x + x), 2) + pow((this->y + y), 2) + pow((this->z + z), 2));
}

float DemoSystem::Vector3::length()
{
	return sqrt(pow(this->x, 2) + pow(this->y, 2) + pow(this->z, 2));
}

void DemoSystem::Vector3::normalize()
{
	this->x = this->x / this->length();
	this->y = this->y / this->length();
	this->z = this->y / this->length();
}

DemoSystem::Vector3 DemoSystem::Vector3::normalized()
{
	Vector3 returnVector(this->x, this->y, this->z);
	return returnVector / this->length();
}

void DemoSystem::Vector3::crossProduct(DemoSystem::Vector3 second)
{
	float x = (this->y * second.z) - (second.y * this->z);
	float y = (this->x * second.z) - (second.x * this->z);
	float z = (this->x * second.y) - (second.x * this->y);
	this->x = x;
	this->y = y;
	this->z = z;
}

DemoSystem::Vector3::~Vector3()
{
}
