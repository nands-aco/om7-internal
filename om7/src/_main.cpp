#include "om7-application.hpp"

#include <cstdint>
#include <memory>

std::unique_ptr<om7::Om7Application> application = std::make_unique<om7::Om7Application>("OverMode7", 240, 240);

int main(int argc, char* argv[])
{
	// (0, 0) 指定でフルスクリーン起動
	application->Run();
	return 0;
}

#if defined(_WIN32)
#include <windows.h>
#include <corecrt_startup.h>
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	int argc = *__p___argc();
	char** argv = *__p___argv();
	return  main(__argc, __argv);
}
#endif
