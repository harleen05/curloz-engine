{
    files = {
        "src/imgui/imgui_draw.cpp"
    },
    depfiles = "imgui_draw.o: src/imgui/imgui_draw.cpp include/imgui/imgui.h  include/imgui/imconfig.h include/imgui/imgui_internal.h  include/imgui/imstb_rectpack.h include/imgui/imstb_truetype.h\
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