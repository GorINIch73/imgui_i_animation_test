# imgui_icon_animations
для имгуи под винду мсис2

Откройте MSYS2 MinGW x64 и выполните:
pacman -Syu                          # Обновление системы
pacman -S --needed base-devel        # Базовые инструменты разработки
pacman -S mingw-w64-x86_64-toolchain # Компилятор MinGW-w64
pacman -S mingw-w64-x86_64-cmake     # CMake для MinGW
pacman -S mingw-w64-x86_64-glfw      # GLFW (оконная система для ImGui)


Сборка проекта: (докачает imgui)
mkdir build && cd build
cmake -G "MinGW Makefiles" ..
cmake --build . 


некоторык dll нужно положить руками

для релиза
cmake -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release ..


для автономной работы надо докинуть из каталога msys64\mingw64\bin\ библиотеки:
libwinpthread-1.dll
libgcc_s_seh-1.dll
libstdc++-6.dll


