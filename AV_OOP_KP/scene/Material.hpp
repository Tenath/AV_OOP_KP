#pragma once
#include <string>
#include "../math/av_vector_t.hpp"

namespace av
{
	class Program;

	class Material
	{
	public:
		std::string Name;
		Vector3f Ambient;
		Vector3f Diffuse;
		Vector3f Specular;
		float Shininess;
		// TODO: Texture

		Program* program = nullptr;

		void UpdateUniforms();

		Program* GetProgram();
		void SetProgram(Program* prog);
	};
}