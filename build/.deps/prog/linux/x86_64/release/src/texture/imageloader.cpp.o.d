{
    files = {
        "src/texture/imageloader.cpp"
    },
    depfiles = "imageloader.o: src/texture/imageloader.cpp include/texture/imageloader.h  include/glad/glad.h include/KHR/khrplatform.h  include/texture/stb_image.h\
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