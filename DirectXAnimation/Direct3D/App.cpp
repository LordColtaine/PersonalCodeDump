#include "App.h"
#include <sstream>
#include <iomanip>
#include "Box.h"
#include "Surface.h"
#include "GDIPlusManager.h"
#include "imgui/imgui.h"
#include "Vertex.h"

GDIPlusManager gdipm;

App::App()
	:
	window(1280, 720, "Direct3D"),
	light(window.graphics())
{
	
	DirectX::XMFLOAT3 mat = { 1.0f, 0.0f, 1.0f };
	//cube = std::make_unique<DrawCube>(window.graphics(), mat);
	
	//Pose p = nano->GetPose();
	window.graphics().SetProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5f, 40.0f));
}

int App::Go()
{
	while (true)
	{
		if (const auto ecode = Window::ProcessMessages())
		{
			return *ecode;
		}
		DoFrame();
	}
}
void App::DoFrame()
{
	namespace dx = DirectX;
	auto dt = timer.Mark() * speed;
	
	window.graphics().BeginFrame(0.07f, 0.0f, 0.12f);
	window.graphics().SetCamera(cam.GetMatrix());
	light.Bind(window.graphics(), cam.GetMatrix());
	static char fileNameBuffer[1024];
	static bool animate = false;
	static bool ik = false;
	if (ImGui::Begin("Load"))
	{
		if (nullptr != nano)
		{
			ImGui::Checkbox("Animate", &animate);
			if (nano->IKChainPresent())
			{
				ImGui::Checkbox("IK for hand", &ik);
			}
			if (ImGui::Button("Unload file"))
			{
				delete nano;
				nano = nullptr;
			}
		}
		else
		{
			ImGui::InputText("FilePath", fileNameBuffer, sizeof(fileNameBuffer));
			if (ImGui::Button("Load File"))
			{
				nano = new Model(window.graphics(), fileNameBuffer);
				fileNameBuffer[0] = '\0';
			}
		}
	}
	ImGui::End();
	if (nullptr != nano)
	{
		nano->Draw(window.graphics(), dt, light.GetPos(), animate, ik);
	}
	
	light.Draw(window.graphics());
	

	if (ImGui::Begin("Simulation Speed"))
	{
		ImGui::SliderFloat("Speed Factor", &speed, 0.0f, 4.0f);
		ImGui::Text("app avg %.3f ms/frame (%.1f fps)", 1000/ImGui::GetIO().Framerate,ImGui::GetIO().Framerate );
	}
	ImGui::End();
	
	cam.SpawnControlWindow();
	if (nullptr != nano)
	{
		nano->ShowWindow(nullptr);
	}
	light.SpawnControlWindow();
	window.graphics().EndFrame();
}

