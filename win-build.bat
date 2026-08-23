cmake -S .\ -B .\_build -DCMAKE_TOOLCHAIN_FILE=.\vcpkg\scripts\buildsystems\vcpkg.cmake || exit /b
cmake --build .\_build || exit /b
