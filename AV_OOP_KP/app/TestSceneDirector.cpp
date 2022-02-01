#include "TestSceneDirector.hpp"
#include "../scene/Scene.hpp"
#include "../scene/SceneEntityBuilder.hpp"
#include "PrimitiveGenerator.hpp"

namespace av
{
	TestSceneDirector::TestSceneDirector(PrimitiveGenerator& g)
	{
		primitives = &g;
	}

	void TestSceneDirector::BuildTestScene(Scene& scene, Material& material)
	{
		SceneEntity* entity = nullptr;

		entity = primitives->RequestFactory("Tetrahedron")->Build()
			.Position(Vector3f(0.0f, 0.0f, 0.0f))
			.Rotate(Vector3f(0.0f, 0.0f, 0.0f))
			.Scale(0.4f)
			.WithColor(Vector4f(0.9f, 0.3f, 0.0f, 1.0f))
			.WithMaterial(material)
			.OnScene(&scene)
			.Finish();

		entity = primitives->RequestFactory("Triangle")->Build()
			.Position(Vector3f(0.5f, 0.0f, 0.0f))
			.Rotate(Vector3f(0.0f, 0.0f, 0.0f))
			.Scale(0.5f)
			.WithColor(Vector4f(0.7f, 0.8f, 0.4f, 1.0f))
			.WithMaterial(material)
			.OnScene(&scene)
			.Finish();

		entity = primitives->RequestFactory("Diamond")->Build()
			.Position(Vector3f(0.0f, 0.0f, 0.5f))
			.Rotate(Vector3f(0.0f, 0.0f, 0.0f))
			.Scale(0.5f)
			.WithColor(Vector4f(0.0f, 0.8f, 0.8f, 1.0f))
			.WithMaterial(material)
			.OnScene(&scene)
			.Finish();

		entity = primitives->RequestFactory("Cube")->Build()
			.Position(Vector3f(-0.5f, 0.0f, -0.5f))
			.Rotate(Vector3f(0.0f, 0.0f, 0.0f))
			.Scale(0.25f)
			.WithColor(Vector4f(0.2f, 0.8f, 0.4f, 1.0f))
			.WithMaterial(material)
			.OnScene(&scene)
			.Finish();

		entity = primitives->RequestFactory("Rectangle")->Build()
			.Position(Vector3f(1.0f, 0.3f, -1.0f))
			.Rotate(Vector3f(0.0f, 0.5f, 0.0f))
			.Scale(0.25f)
			.WithColor(Vector4f(0.2f, 0.8f, 0.4f, 1.0f))
			.WithMaterial(material)
			.OnScene(&scene)
			.Finish();

		entity = primitives->RequestFactory("Sphere")->Build()
			.Position(Vector3f(-1.0f, 0.0f, 1.0f))
			.Rotate(Vector3f(0.0f, 0.5f, 0.0f))
			.Scale(0.5f)
			.WithColor(Vector4f(0.2f, 0.8f, 0.4f, 1.0f))
			.WithMaterial(material)
			.OnScene(&scene)
			.Finish();
	}
}