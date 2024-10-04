#pragma once
//#include<gl/gl.h>
class Application
{
private:
	Application(const Application&);
	Application& operator=(const Application&);

public:
	inline Application(){}
	inline virtual ~Application(){}
	inline virtual void Initialize(){}
	inline virtual void Update(float DeltaTime){}
	inline virtual void Render(float AspectRatio){}
	inline virtual void Shutdown(){}
};