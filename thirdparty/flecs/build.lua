project "Flecs"
    kind "StaticLib"
    language "C++"
    cppdialect "c++11"
    toolset "gcc"
    
    files { "**.hpp", "**.cpp" }
    includedirs { "." }

    build_target_dir()
    build_object_dir()
    build_lib_dirs()
    build_include_dirs()

    build_target_dir()

    defines { "FLECS_CPP" }
    
    filter "configurations:debug"
        defines { "NDEBUG" }
        symbols "On"
    
    filter "configurations:release"
        defines { "NDEBUG" }
        optimize "On"