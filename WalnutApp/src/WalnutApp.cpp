#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"

#include "Walnut/Image.h"
#include "Walnut/Random.h"
#include "Walnut/Timer.h"
#include "Walnut/Input/Input.h"

#include "Camera.h"
#include "Renderer.h"
#include "Scene.h"
#include "Sphere.h"

#include <glm/gtc/type_ptr.hpp>

using namespace Walnut;

Walnut::Application* mainApp = nullptr;

class ExampleLayer : public Walnut::Layer
{
public:
	ExampleLayer()
	: camera(45.0f, 0.1f, 100.0f)
	{
	}

	virtual void OnUpdate(float ts) override
	{
		if (camera.OnUpdate(ts))
		{
			renderer.resetFrameIndex();
		}

		if (Input::IsKeyDown(KeyCode::Escape))
		{
			mainApp->Close();
		}
	}

	virtual void OnUIRender() override
	{
		ImGui::Begin("Settings");
		ImGui::Text("Last render time: %.3fms", lastRenderTime);

		if (ImGui::Button("Render"))
		{
			Render();
		}

		ImGui::Checkbox("Accumulate", &renderer.getSettings().accumulate);

		ImGui::LabelText("", "Frame Count:%d", renderer.getFrameIndex());

		if (ImGui::Button("Reset"))
		{
			renderer.resetFrameIndex();
		}

		ImGui::End();

		ImGui::Begin("Scene");

		for (auto i = 0; i < scene.getSpheres().size(); i++)
		{
			// Give this control block a unique id (otherwise ImGui don't know how to distinguish multiple controls with the same name)
			ImGui::PushID(i);
			
			auto& sphere = scene.getSpheres()[i];

			ImGui::LabelText("", "Sphere%d", i);

			ImGui::DragFloat3("Sphere Position", glm::value_ptr(sphere->center), 0.1f);
			ImGui::DragFloat("Sphere Radius", &sphere->radius, 0.01f, 0.1f, 1.0f);
			ImGui::DragInt("Material Index", &sphere->materialIndex, 0.1f, 0, scene.getMaterials().size() - 1);

			ImGui::Separator();

			ImGui::PopID();
		}

		ImGui::LabelText("", "Materials");

		for (auto i = 0; i < scene.getMaterials().size(); i++)
		{
			ImGui::PushID(i);

			auto& material = scene.getMaterials()[i];
			ImGui::ColorEdit3("Sphere Color", glm::value_ptr(material.albedo));
			ImGui::DragFloat("Roughness", &material.roughness, 0.01f, 0.0f, 1.0f);
			ImGui::DragFloat("Metallic", &material.metallic, 0.01f, 0.0f, 1.0f);

			ImGui::Separator();

			ImGui::PopID();
		}

		ImGui::SliderFloat3("Light Direction", &renderer.lightDirection[0], -1.0f, 1.0f);
		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("Viewport");

		viewportWidth = static_cast<uint32_t>(ImGui::GetContentRegionAvail().x);
		viewportHeight = static_cast<uint32_t>(ImGui::GetContentRegionAvail().y);

		auto image = renderer.GetFinadlImage();

		if (image)
		{
			ImGui::Image(image->GetDescriptorSet(),
				{ static_cast<float>(image->GetWidth()),
					   static_cast<float>(image->GetHeight()) }, { 0, 1 }, {1, 0} );
		}

		ImGui::End();
		ImGui::PopStyleVar();

		Render();
	}

	void Render()
	{
		Timer timer;

		renderer.OnResize(viewportWidth, viewportHeight);
		camera.OnResize(viewportWidth, viewportHeight);
		renderer.Render(scene, camera);

		lastRenderTime = timer.ElapsedMillis();
	}

private:
	Scene scene;
	Camera camera;
	Renderer renderer;
	uint32_t viewportWidth = 0;
	uint32_t viewportHeight = 0;
	float lastRenderTime = 0.0f;
};

Walnut::Application* Walnut::CreateApplication(int argc, char** argv)
{
	Walnut::ApplicationSpecification spec;
	spec.Name = "Walnut Example";

	Walnut::Application* app = new Walnut::Application(spec);
	mainApp = app;
	app->PushLayer<ExampleLayer>();
	app->SetMenubarCallback([app]()
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Exit"))
			{
				app->Close();
			}
			ImGui::EndMenu();
		}
	});
	return app;
}