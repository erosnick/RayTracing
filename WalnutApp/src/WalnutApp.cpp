#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"

#include "Walnut/Image.h"
#include "Walnut/Random.h"
#include "Walnut/Timer.h"
#include "Renderer.h"
#include "Camera.h"

using namespace Walnut;

class ExampleLayer : public Walnut::Layer
{
public:
	ExampleLayer()
	: camera(45.0f, 0.1f, 100.0f)
	{
	}

	virtual void OnUpdate(float ts) override
	{
		camera.OnUpdate(ts);
	}

	virtual void OnUIRender() override
	{
		ImGui::Begin("Settings");
		ImGui::Text("Last render time: %.3fms", lastRenderTime);
		if (ImGui::Button("Render"))
		{
			Render();
		}
		ImGui::ColorEdit3("Object Color", &renderer.objectColor[0]);
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
		renderer.Render(camera);

		lastRenderTime = timer.ElapsedMillis();
	}

private:
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