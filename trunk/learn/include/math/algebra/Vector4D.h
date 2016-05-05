#pragma once
namespace plu{
	class Vector4D{
	public:
		Vector4D();
		Vector4D(float px, float py, float pz, float pw);
		~Vector4D();

		float x, y, z, w;
	};
}