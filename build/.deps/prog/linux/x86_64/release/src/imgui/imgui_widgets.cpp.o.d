{
    files = {
        "src/imgui/imgui_widgets.cpp"
    },
    depfiles = "imgui_widgets.o: src/imgui/imgui_widgets.cpp include/imgui/imgui.h  include/imgui/imconfig.h include/imgui/imgui_internal.h  include/imgui/imstb_textedit.h\
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