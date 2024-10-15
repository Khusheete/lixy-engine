project "Flecs"
    kind "StaticLib"
    language "C++"
    cppdialect "c++11"
    
    files { "flecs.c", "flecs.h" }
    includedirs { "." }

    build_target_dir()
    build_object_dir()
    build_lib_dirs()
    build_include_dirs()

    build_target_dir()

    platform_config()

    filter "platforms:windows"
        defines { "ECS_TARGET_WINDOWS" }
    
    filter "platforms:linux"
        defines { "ECS_TARGET_LINUX" }
    
    filter "configurations:debug"
        defines { "NDEBUG" }
        symbols "On"
    
    filter "configurations:release"
        defines { "NDEBUG" }
        optimize "On"