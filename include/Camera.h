#ifndef CAMERA_H
#define CAMERA_H
#include "Vector3.h"
#define PI 3.1415265359
#define PIdiv180 3.1415265359 / 180.0
namespace DemoSystem
{
	class Camera
	{
	public:
		Camera();
		~Camera(void);

		void update();
		void move(Vector3 dir);
		void rotateX(double angle);
		void rotateY(double angle);
		void rotateZ(double angle);
		void moveForward(double speed);
		void moveBackward(double speed);
		void zeroVelocity();
		Vector3 getViewDir();
		Vector3 getPosition();

	private:
		Vector3 position;
		Vector3 viewDir;
		Vector3 velocity;
		Vector3 rotation;
	};
}
#endif