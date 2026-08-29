#pragma once

#include <stdexcept>
#include <string>

class GLFWwindow;

namespace om7
{
	class Om7RuntimeException : public std::runtime_error
	{
	public:
		Om7RuntimeException(const char message[]) : std::runtime_error(message) {}
		Om7RuntimeException(const std::string& message) : std::runtime_error(message) {}
		const char* what() const noexcept override { return std::runtime_error::what(); }
	};

	class Om7LogicException : public std::logic_error
	{
	public:
		Om7LogicException(const char message[]) : std::logic_error(message) {}
		Om7LogicException(const std::string& message) : std::logic_error(message) {}
		const char* what() const noexcept override { return std::logic_error::what(); }
	};

	class Om7Application
	{
	private:
		GLFWwindow *Window;
	public:
		Om7Application();
		virtual ~Om7Application();
		void Init();
		void Term();
		void Run();
	protected:
		virtual void OnInit() {}
		virtual void OnTerm() {}
		virtual void OnUpdate() {}
		virtual void OnGraphicRender() {}
	};
}
