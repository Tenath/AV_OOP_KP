#pragma once

namespace av
{
	class Scene;
	class Material;
	class PrimitiveGenerator;

	class SceneDirector
	{
	public:
		virtual void BuildScene(Scene& scene, Material& m) = 0;
	};

	class TestSceneDirector : public SceneDirector
	{
		PrimitiveGenerator* primitives;
	public:
		TestSceneDirector(PrimitiveGenerator& g);
		void BuildScene(Scene& scene, Material& m) override;
	};
}