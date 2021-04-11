#ifndef VECTOR3_H
#define VECTOR3_H
#include <math.h>
namespace DemoSystem
{
	class Vector3
	{
	public:
		Vector3();
		Vector3(float x, float y, float z);
		Vector3(const Vector3 &param);
		Vector3 operator+(Vector3 vec);
		Vector3 operator-(Vector3 vec);
		Vector3 operator*(float scalar);
		Vector3 operator/(float scalar);
		Vector3 &operator+=(const Vector3 vec);
		Vector3 &operator-=(const Vector3 vec);
		float distance(Vector3 vec);
		float distance(float x, float y, float z);
		float length();
		void normalize();
		Vector3 normalized();
		void crossProduct(Vector3 second);
		static Vector3 crossProduct(Vector3 first, Vector3 second)
		{
			Vector3 returnVector = Vector3();
			returnVector.x = (first.y * second.z) - (first.z * second.y);
			returnVector.y = (first.x * second.z) - (first.z * second.x);
			returnVector.z = (first.x * second.y) - (first.y * second.x);
			return returnVector;
		};
		static Vector3 calculateFaceNormal(Vector3 pointA, Vector3 pointB, Vector3 pointC)
		{
			Vector3 fromAtoB = Vector3(pointB);
			fromAtoB -= pointA;
			Vector3 fromAtoC = Vector3(pointC);
			fromAtoC -= pointA;
			fromAtoB.crossProduct(fromAtoC);
			return fromAtoB.normalized();
		};
		~Vector3(void);

		float x;
		float y;
		float z;
	};
}
#endif