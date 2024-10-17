project "Demo"
    kind "WindowedApp"
    language "C++"
    cppdialect "c++17"
    
    files { "src/**.hpp", "src/**.cpp" }
    links { "Renderer", "Flecs", "Stb", "Core", "glfw", "GL", "GLEW", "pthread", "dl" }

    build_target_dir()
    build_object_dir()
    build_lib_dirs()
    build_include_dirs()

    build_target_dir()

    platform_config()
    
    filter "configurations:debug"
        defines { "NDEBUG" }
        symbols "On"
    
    filter "configurations:release"
        defines { "NDEBUG" }
        optimize "On"