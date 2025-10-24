{
    files = {
        "src/glad/glad.c"
    },
    depfiles = "glad.o: src/glad/glad.c include/glad/glad.h include/KHR/khrplatform.h\
",
    values = {
        "/usr/bin/gcc",
        {
            "-m64",
            "-fvisibility=hidden",
            "-O3",
            "-Iinclude",
            "-flto",
            "-DNDEBUG"
        }
    },
    depfiles_format = "gcc"
}