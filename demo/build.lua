project "Demo"
    kind "WindowedApp"
    language "C++"
    cppdialect "c++17"
    toolset "gcc"
    
    files { "src/**.hpp", "src/**.cpp" }
    links { "glfw", "GL", "GLEW", "pthread", "dl", "Flecs", "Core", "Renderer", "Stb" }

    build_target_dir()
    build_object_dir()
    build_lib_dirs()
    build_include_dirs()

    build_target_dir()

    
    filter "configurations:debug"
        defines { "NDEBUG" }
        symbols "On"
    
    filter "configurations:release"
        defines { "NDEBUG" }
        optimize "On"