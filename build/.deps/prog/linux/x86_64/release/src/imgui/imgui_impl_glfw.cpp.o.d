{
    files = {
        "src/imgui/imgui_impl_glfw.cpp"
    },
    depfiles = "imgui_impl_glfw.o: src/imgui/imgui_impl_glfw.cpp include/imgui/imgui.h  include/imgui/imconfig.h include/imgui/imgui_impl_glfw.h  include/GLFW/glfw3.h include/GLFW/glfw3native.h\
",
    values = {
        "/usr/bin/g++",
        {
            "-m64",
            "-fvisibility=hidden",
            "-fvisibility-inlines-hidden",
            "-O3",
            "-std=c++17",
            "-Iinclude",
            "-flto",
            "-DNDEBUG"
        }
    },
    depfiles_format = "gcc"
}