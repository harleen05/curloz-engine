{
    files = {
        "src/lighting/lightingPoint.cpp"
    },
    depfiles = "lightingPoint.o: src/lighting/lightingPoint.cpp  include/lighting/lightingPoint.h include/glm/vec3.hpp  include/glm/./ext/vector_bool3.hpp  include/glm/./ext/../detail/type_vec3.hpp  include/glm/./ext/../detail/qualifier.hpp  include/glm/./ext/../detail/setup.hpp  include/glm/./ext/../detail/../simd/platform.h  include/glm/./ext/../detail/type_vec3.inl  include/glm/./ext/../detail/compute_vector_relational.hpp  include/glm/./ext/../detail/compute_vector_decl.hpp  include/glm/./ext/../detail/_vectorize.hpp  include/glm/./ext/vector_bool3_precision.hpp  include/glm/./ext/vector_float3.hpp  include/glm/./ext/vector_float3_precision.hpp  include/glm/./ext/vector_double3.hpp  include/glm/./ext/vector_double3_precision.hpp  include/glm/./ext/vector_int3.hpp  include/glm/./ext/vector_int3_sized.hpp  include/glm/./ext/../ext/scalar_int_sized.hpp  include/glm/./ext/../ext/../detail/setup.hpp  include/glm/./ext/vector_uint3.hpp  include/glm/./ext/vector_uint3_sized.hpp  include/glm/./ext/../ext/scalar_uint_sized.hpp include/imgui/imgui.h  include/imgui/imconfig.h\
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