{
    files = {
        "src/imgui/imgui_impl_opengl3.cpp"
    },
    depfiles = "imgui_impl_opengl3.o: src/imgui/imgui_impl_opengl3.cpp  include/imgui/imgui.h include/imgui/imconfig.h  include/imgui/imgui_impl_opengl3.h  include/imgui/imgui_impl_opengl3_loader.h\
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