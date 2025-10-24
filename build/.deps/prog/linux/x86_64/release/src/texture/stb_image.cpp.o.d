{
    files = {
        "src/texture/stb_image.cpp"
    },
    depfiles = "stb_image.o: src/texture/stb_image.cpp include/texture/stb_image.h\
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