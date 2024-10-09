project "Renderer"
    kind "StaticLib"
    language "C++"
    cppdialect "c++17"
    toolset "gcc"
    
    files { "src/**.hpp", "src/**.cpp" }
    -- links { "glfw", "GL", "GLEW", "glm", "pthread", "dl" }

    build_target_dir()
    build_object_dir()
    build_lib_dirs()
    build_include_dirs()

    build_target_dir()

    
    filter "configurations:debug"
        defines { "NDEBUG" }
        symbols "On"
    
    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"