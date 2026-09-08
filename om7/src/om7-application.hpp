#pragma once

#include <cstdint>
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
		Om7Application(const char *title, std::int32_t width, std::int32_t height);
		virtual ~Om7Application();
		int Run();
	protected:
		virtual void OnInit();
		virtual void OnTerm();
		virtual void OnUpdate() noexcept {}
		virtual void OnGraphRender(const std::int32_t width, const std::int32_t height, std::uint16_t buffer[]) noexcept
		{
			for (std::int32_t r = 0; r < height; ++r)
			{
				for (std::int32_t c = 0; c < width; ++c)
				{
					buffer[r * width + c] = 0x00FF; // RGBA4444 format: R=0, G=0, B=15, A=15
				}
			}
		}
	};
}
