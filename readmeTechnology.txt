Для Linux:
Установить CMake GUI
!Установить glfw-x11 (не устанавливать glfw-wayland)

//Сборка и запуск
cmake -G Ninja ..
ninja -j4
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/run/media/alexey/LinuxData/Cascade/occt_build780/lin64/gcc/libd/
./glfwocct





