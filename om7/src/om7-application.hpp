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

	// １ピクセルのフォーマットを表す構造体です。
	// ※プラットフォームごとに定義を調整する可能性があります。
	struct Om7GraphPixel
	{
		Om7GraphPixel() : R(0), G(0), B(0), A(15) {}
		Om7GraphPixel(std::uint16_t r, std::uint16_t g, std::uint16_t b) : R(r), G(g), B(b), A(15) {}
		Om7GraphPixel(std::uint16_t r, std::uint16_t g, std::uint16_t b, std::uint16_t a) : R(r), G(g), B(b), A(a) {}
		std::uint16_t A : 4;
		std::uint16_t B : 4;
		std::uint16_t G : 4;
		std::uint16_t R : 4;
	};
	static_assert(sizeof(Om7GraphPixel) == 2);

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
		virtual void OnGraphRender(const std::int32_t width, const std::int32_t height, Om7GraphPixel buffer[]) noexcept
		{
			for (std::int32_t r = 0; r < height; ++r)
			{
				for (std::int32_t c = 0; c < width; ++c)
				{
					buffer[r * width + c] = Om7GraphPixel(0, 0, 15, 15);
				}
			}
		}
	};
}
