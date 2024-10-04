#pragma once

#include "WinHeader.h"
#include "CustomException.h"
#include "Keyboard.h"
#include "Mouse.h"
#include <optional>


class Window
{
public:
	class Exception : public CustomException
	{
	public:
		Exception(int line, const char* file, HRESULT hr) noexcept;
		const char* what() const noexcept override;
		virtual const char* GetType() const noexcept override;
		static std::string TranslateErrorCode(HRESULT hr) noexcept;
		HRESULT GetErrorCode() const noexcept;
		std::string GetErrorString() const noexcept;
	private:
		HRESULT hr;
	};
private:
	//Singleton class
	class WindowClass
	{
	public:
		static const char* GetName() noexcept;
		static HINSTANCE GetInstance() noexcept;
	private:
		WindowClass() noexcept;
		~WindowClass();
		WindowClass(const WindowClass&) = delete;
		WindowClass& operator= (const WindowClass&) = delete;
		static constexpr const char* wndClassName = "Window";
		static WindowClass wndClass;
		HINSTANCE hInst;
	};
public:
	Window(int width, int height, const char* name);
	~Window();
	Window(const Window&) = delete;
	Window& operator= (const Window&) = delete;
	void SetTitle(const std::string title);
	static std::optional <int> ProcessMessages();
	
	//three functions, because the last one cannot be called directly as it takes an
	//additional hidden parameter, the this pointer
private:
	static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;

public:
	Keyboard keyboard;
	Mouse mouse;

private:
	int width;
	int height;
	HWND hWnd;
};

#define CUWND_EXCEPT(hr) Window::Exception(__LINE__, __FILE__, hr)
#define CUWND_LAST_EXCEPT() Window::Exception(__LINE__, __FILE__, GetLastError())