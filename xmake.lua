-- Project Configuration

set_project("curloz-engine")
set_version("0.1.0")
set_languages("c++17")

add_rules("mode.debug", "mode.release")
set_defaultmode("debug")
add_rules("plugin.compile_commands.autoupdate", { outputdir = "." })


-- Dependencies




target("curloz engine")
    set_kind("binary")
    set_rundir("$(projectdir)")
    set_toolchains("gcc", "clang")

    -- source files
    add_files("src/**/*.cpp")
    add_files("src/*.cpp")
    add_files("external/**/*.cpp", "external/**/*.c")

    -- include dirs
    add_includedirs("include", { public = true })
    add_includedirs("external", { public = true })

    -- Project Definitions
    add_defines("GLFW_INCLUDE_NONE")

    -- external libraries folder
    local lib_folder = ""
    if is_plat("linux") then
        add_linkdirs("lib/linux")
    elseif is_plat("windows") then
        add_linkdirs("lib/windows")
    end

    -- Manual libraries
    add_links("glfw3", "reactphysics3d")

    -- system libraries
    add_links("vulkan", "fmt")

    if is_plat("linux") then
        add_syslinks("pthread", "dl")
    end
