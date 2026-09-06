//
// GLFWに依存する処理は、このソースコードに限定する方針です。
// GLFWでハンドリングしたイベントをOm7Applicationに通知するため、
// Om7Applicationのインスタンスを引数として受け取るrun関数を定義します。
//
#include <cstdint>
#include <cstdlib>
#include <chrono>
#include <thread>
#include <algorithm>
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "om7-application.hpp"

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

//
// OpenGLに関係するもの
//
namespace
{
	// 1秒間に対するフレーム数を表します。
	constexpr double FramesPerSecond = 60.0;
	// 1フレームの時間間隔(秒)表します。
	constexpr double FrameInterval = 1.0 / FramesPerSecond;
	// ドロップを許容するフレーム数を表します。
	constexpr std::int32_t MaxDropFrames = 3;
	// 画面の横幅を表します。
	constexpr std::int32_t ScreenWidth = 240;
	// 画面の縦幅を表します。
	constexpr std::int32_t ScreenHeight = 240;
	// 画面のデータサイズを表します。
	constexpr std::int32_t ScreenBufferSize = ScreenWidth * ScreenHeight;
	// 画面のバッファ数を表します。
	constexpr std::int32_t ScreenBufferCount = 2;
	// 画面バッファを表します。
	om7::Om7GraphPixel ScreenBuffers[ScreenBufferCount][ScreenBufferSize];
	// 使用中の画面バッファのインデックスを表します。
	std::int32_t ScreenBufferIndex = 0;

	GLuint Texture;
	GLuint VertexShader;
	GLuint FragmentShader;
	GLuint Program;
	GLuint VertexArray;
	GLuint VerticesBuffer;
	GLuint IndicesBuffer;

	const GLchar *VertexShaderSource =
		"#version 300 es\n"
		// "in highp vec2 position;\n"
		// "in mediump vec2 texCoords;\n"
		"layout(location = 0) in highp vec2 position;\n"
		"layout(location = 1) in mediump vec2 texCoords;\n"
		"out mediump vec2 TexCoords;\n"
		"void main()\n"
		"{\n"
		"\tgl_Position = vec4(position, 0.0, 1.0);\n"
		"\tTexCoords = texCoords;\n"
		"}\n";
	//
	//
	//
	const GLchar *FragmentShaderSource = 
		"#version 300 es\n"
		"in mediump vec2 TexCoords;\n"
		"out mediump vec4 color;\n"
		"uniform sampler2D ourTexture;\n"
		"void main()\n"
		"{\n"
		"\tcolor = texture(ourTexture, TexCoords);\n"
		"}\n";
	//
	//
	//
	const float Vertices[] =
	{
		/* 左上 */ -1.0f, +1.0f, +0.0f, +0.0f,
		/* 左下 */ -1.0f, -1.0f, +0.0f, +1.0f,
		/* 右下 */ +1.0f, -1.0f, +1.0f, +1.0f,
		/* 右上 */ +1.0f, +1.0f, +1.0f, +0.0f,
	};
	//
	//
	//
	const GLuint Indices[] =
	{
		/* １つめ */ 0, 1, 2,
		/* ２つめ */ 0, 2, 3,
	};
	//
	//
	//
	void GlInit()
	{
		//
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glDisable(GL_DITHER);
		//
		glGenTextures(1, &Texture);
		glBindTexture(GL_TEXTURE_2D, Texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB5_A1, ScreenWidth, ScreenHeight, 0, GL_RGBA, GL_UNSIGNED_SHORT_4_4_4_4, nullptr);
		//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB5_A1, ScreenWidth, ScreenHeight, 0, GL_RGBA, GL_UNSIGNED_SHORT_5_5_5_1, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		//
		VertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(VertexShader, 1, &VertexShaderSource, nullptr);
		glCompileShader(VertexShader);
		//
		FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(FragmentShader, 1, &FragmentShaderSource, nullptr);
		glCompileShader(FragmentShader);
		//
		Program = glCreateProgram();
		glAttachShader(Program, VertexShader);
		glAttachShader(Program, FragmentShader);
		glLinkProgram(Program);
		//
		glUseProgram(Program);
		//
		glDetachShader(Program, VertexShader);
		glDetachShader(Program, FragmentShader);
		glDeleteShader(VertexShader);
		glDeleteShader(FragmentShader);
		//
		glGenVertexArrays(1, &VertexArray);
		glGenBuffers(1, &VerticesBuffer);
		glGenBuffers(1, &IndicesBuffer);
		//
		glBindVertexArray(VertexArray);
		glBindBuffer(GL_ARRAY_BUFFER, VerticesBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndicesBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);
		//
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)(0 * sizeof(float)));
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)(2 * sizeof(float)));
		glEnableVertexAttribArray(1);
		//
		glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
	}
	//
	//
	//
	void GlTerm()
	{
		//
		glDeleteBuffers(1, &IndicesBuffer);
		glDeleteBuffers(1, &VerticesBuffer);
		glDeleteVertexArrays(1, &VertexArray);
		//
		glBindTexture(GL_TEXTURE_2D, 0);
		glDeleteTextures(1, &Texture);
		//
		glUseProgram(0);
		glDeleteProgram(Program);
	}

	void GlRender()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, ScreenWidth, ScreenHeight, GL_RGBA, GL_UNSIGNED_SHORT_4_4_4_4, ScreenBuffers[ScreenBufferIndex]);
		// glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, ScreenWidth, ScreenHeight, GL_RGBA, GL_UNSIGNED_SHORT_5_5_5_1, ScreenBuffers[ScreenBufferIndex]);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		ScreenBufferIndex = (ScreenBufferIndex + 1) & 1;
	}
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
