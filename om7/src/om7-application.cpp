//
// GLFWに依存する処理は、このソースコードに限定する方針です。
// GLFWでハンドリングしたイベントをOm7Applicationに通知するため、
// Om7Applicationのインスタンスを引数として受け取るrun関数を定義します。
//
#include "om7-hgl.hpp"
#include "om7-application.hpp"

#include <cstdint>
#include <cstdlib>
#include <chrono>
#include <thread>
#include <algorithm>
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace
{
	void OnError(int errorCode, const char *description)
	{
		std::cerr << "[GLFW Error] code=" << errorCode  << " description=" << (description ? description : "(null)")  << std::endl;
	}
	void OnWindowPos(GLFWwindow * /*window*/, int /*xpos*/, int /*ypos*/) {}
	void OnWindowSize(GLFWwindow * /*window*/, int /*width*/, int /*height*/) {}
	void OnWindowClose(GLFWwindow * /*window*/) {}
	void OnWindowRefresh(GLFWwindow * /*window*/) {}
	void OnWindowFocus(GLFWwindow * /*window*/, int /*focused*/) {}
	void OnWindowIconify(GLFWwindow * /*window*/, int /*iconified*/) {}
	void OnWindowMaximize(GLFWwindow * /*window*/, int /*maximized*/) {}
	void OnFramebufferSize(GLFWwindow * /*window*/, int width, int height)
	{
		glViewport(0, 0, width, height);
	}
	void OnWindowContentScale(GLFWwindow * /*window*/, float /*xscale*/, float /*yscale*/) {}
	void OnMouseButton(GLFWwindow * /*window*/, int /*button*/, int /*action*/, int /*mods*/) {}
	void OnCursorPos(GLFWwindow * /*window*/, double /*xpos*/, double /*ypos*/) {}
	void OnCursorEnter(GLFWwindow * /*window*/, int /*entered*/) {}
	void OnScroll(GLFWwindow * /*window*/, double /*xoffset*/, double /*yoffset*/) {}
	void OnKey(GLFWwindow *window, int key, int /*scancode*/, int action, int /*mods*/)
	{
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(window, GLFW_TRUE);
		}
	}
	void OnChar(GLFWwindow * /*window*/, unsigned int /*codepoint*/) {}
	void OnCharMods(GLFWwindow * /*window*/, unsigned int /*codepoint*/, int /*mods*/) {}
	void OnDrop(GLFWwindow * /*window*/, int /*pathCount*/, const char *[] /*paths*/) {}

	void RegisterEventStubs(GLFWwindow *window)
	{
		glfwSetWindowPosCallback(window, OnWindowPos);
		glfwSetWindowSizeCallback(window, OnWindowSize);
		glfwSetWindowCloseCallback(window, OnWindowClose);
		glfwSetWindowRefreshCallback(window, OnWindowRefresh);
		glfwSetWindowFocusCallback(window, OnWindowFocus);
		glfwSetWindowIconifyCallback(window, OnWindowIconify);
		glfwSetWindowMaximizeCallback(window, OnWindowMaximize);
		glfwSetFramebufferSizeCallback(window, OnFramebufferSize);
		glfwSetWindowContentScaleCallback(window, OnWindowContentScale);

		glfwSetMouseButtonCallback(window, OnMouseButton);
		glfwSetCursorPosCallback(window, OnCursorPos);
		glfwSetCursorEnterCallback(window, OnCursorEnter);
		glfwSetScrollCallback(window, OnScroll);

		glfwSetKeyCallback(window, OnKey);
		glfwSetCharCallback(window, OnChar);
#if GLFW_VERSION_MAJOR >= 3 && GLFW_VERSION_MINOR >= 3
		glfwSetCharModsCallback(window, OnCharMods);
#endif
		glfwSetDropCallback(window, OnDrop);
	}

	void UpdateFixedStep()
	{
		// TODO: 60FPS固定で処理したいゲームロジックをここに実装
	}

	void RenderFrame()
	{
		// TODO: OpenGL 3.3 Core の描画処理をここに実装
		glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
	}

} // namespace

//
// ウィンドウに関係するもの
//
namespace
{

}

namespace om7
{
	Om7Application::Om7Application(const char *title, std::int32_t width, std::int32_t height)
	{
		glfwSetErrorCallback(OnError);
		if (glfwInit() != GLFW_TRUE) throw Om7RuntimeException("glfwInit failed.");

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	#if defined(__APPLE__)
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
	#endif

		if (width == 0 && height == 0)
		{
			GLFWmonitor *monitor = glfwGetPrimaryMonitor();
			if (!monitor) throw Om7RuntimeException("Primary monitor is not available.");
			const GLFWvidmode *mode = glfwGetVideoMode(monitor);
			if (!mode) throw Om7RuntimeException("Failed to get video mode.");
			Window = glfwCreateWindow(mode->width, mode->height, title, monitor, nullptr);
		}
		else
		{
			Window = glfwCreateWindow(width, height, title, nullptr, nullptr);
		}
		if (!Window) throw Om7RuntimeException("glfwCreateWindow failed.");
		glfwMakeContextCurrent(Window);
		glfwSwapInterval(0); // 自前60FPS制御のためVSync無効
		if (gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)) == 0) throw Om7RuntimeException("Failed to initialize GLAD.");
		RegisterEventStubs(Window);
	}

	Om7Application::~Om7Application()
	{
		glfwDestroyWindow(Window);
		glfwTerminate();
	}

	void Om7Application::OnInit()
	{
		GlInit();
	}

	void Om7Application::OnTerm()
	{
		GlTerm();
	}

	int Om7Application::Run()
	{
		OnInit();
		double delTime = glfwGetTime() + FrameInterval;
		int dropCount = 0;
		bool dropFlag = false;
		while (!glfwWindowShouldClose(Window))
		{
			OnUpdate();
			dropCount = delTime <= glfwGetTime() ? dropCount + 1 : 0;
			while (delTime >= glfwGetTime()) glfwPollEvents();
			delTime = glfwGetTime() + FrameInterval;
			if (dropCount == 0 || dropCount >= 4)
			{
				OnGraphRender(ScreenWidth, ScreenHeight, ScreenBuffers[ScreenBufferIndex]);
				if (dropFlag)
				{
					// TODO: ドロップ特有処理
					dropFlag = false;
				}
				GlRender();
				glfwSwapBuffers(Window);
			}
			else
			{
				// フレームのスキップ
				dropFlag = true;
			}
			glfwPollEvents();
		}
		OnTerm();
		return 0;
	}
} // namespace om7
