#pragma once

#include <exception>
#include <string>

class CustomException : public std::exception
{
public:
	CustomException(int line, const char* file) noexcept;
	const char* what() const noexcept override;
	virtual const char* GetType() const noexcept;
	int GetLine() const noexcept;
	const std::string& GetFile() const noexcept;
	std::string GetOriginString() const noexcept;
private:
	int line;
	std::string file;
protected:
	//Used because pointer in what() will point to garbage after function execution
	//mutable because what() is const
	mutable std::string whatBuffer;
};
