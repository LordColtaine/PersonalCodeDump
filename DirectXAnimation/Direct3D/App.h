#pragma once
#include "Window.h"
#include "Timer.h"
#include "ImguiManager.h"
#include "Camera.h"
#include "PointLight.h"
#include "Mesh.h"

class App
{
public:
	App();
	//message loop / master frame
	int Go();

private:
	void DoFrame();
	
private:
	
	ImguiManager imgui;
	Window window;
	Timer timer;
	float speed = 1.0f;
	//std::vector<std::unique_ptr<class Box>> boxes;
	Camera cam;
	PointLight light;
	Model* nano = nullptr;// = new Model(window.graphics(), "C:\\Users\\K Raviraju Prasanna\\Documents\\OBJFiles\\Character Running.dae");
	//Model nano{ window.graphics(),"C:\\Users\\K Raviraju Prasanna\\Documents\\OBJFiles\\BrainStem.dae" };
	//Model nano{ window.graphics(),"C:\\Users\\K Raviraju Prasanna\\Documents\\OBJFiles\\Woman.dae" };
};