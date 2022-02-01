#pragma once

namespace av
{
	class Scene;
	class Material;
	class PrimitiveGenerator;

	class TestSceneDirector
	{
		PrimitiveGenerator* primitives;
	public:
		TestSceneDirector(PrimitiveGenerator& g);
		void BuildTestScene(Scene& scene, Material& m);
	};
}