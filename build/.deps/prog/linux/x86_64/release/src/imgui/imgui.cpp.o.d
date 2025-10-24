{
    files = {
        "src/imgui/imgui.cpp"
    },
    depfiles = "imgui.o: src/imgui/imgui.cpp include/imgui/imgui.h  include/imgui/imconfig.h include/imgui/imgui_internal.h\
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