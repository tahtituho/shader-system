#include "Camera.h"

DemoSystem::Camera::Camera()
{
	this->position = Vector3(0, 0, 0);
	this->viewDir = Vector3(0, 0, -1);
	this->rotation = Vector3(0, 0, 0);
	this->velocity = Vector3(0, 0, 0);
}

DemoSystem::Camera::~Camera(void)
{
}

void DemoSystem::Camera::update()
{
	Vector3 moveVector;
	this->viewDir = this->getViewDir();
	moveVector.x = this->viewDir.x * -this->velocity.x;
	moveVector.y = this->viewDir.y * -this->velocity.y;
	moveVector.z = this->viewDir.z * -this->velocity.z;
	this->position += moveVector;
}

void DemoSystem::Camera::move(Vector3 dir)
{
	this->position += dir;
}

void DemoSystem::Camera::rotateX(double angle)
{
	this->rotation.x += angle;
}

void DemoSystem::Camera::rotateY(double angle)
{
	this->rotation.y += angle;
}

void DemoSystem::Camera::rotateZ(double angle)
{
	this->rotation.z += angle;
}

void DemoSystem::Camera::moveForward(double speed)
{
	this->velocity.x = -speed;
	this->velocity.y = -speed;
	this->velocity.z = -speed;
}

void DemoSystem::Camera::moveBackward(double speed)
{
	this->velocity.x = speed;
	this->velocity.y = speed;
	this->velocity.z = speed;
}

void DemoSystem::Camera::zeroVelocity()
{
	this->velocity.x = 0;
	this->velocity.y = 0;
	this->velocity.z = 0;
}

DemoSystem::Vector3 DemoSystem::Camera::getViewDir()
{
	Vector3 step1, step2;
	step1.x = cos((this->rotation.y + 90.0) * PIdiv180);
	step1.z = -sin((this->rotation.y + 90.0) * PIdiv180);
	double cosX = cos(this->rotation.x * PIdiv180);
	step2.x = step1.x * cosX;
	step2.z = step1.z * cosX;
	step2.y = sin(this->rotation.x * PIdiv180);
	return step2;
}

DemoSystem::Vector3 DemoSystem::Camera::getPosition()
{
	return this->position;
}