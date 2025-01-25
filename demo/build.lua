project "Demo"
    kind "WindowedApp"
    language "C++"
    cppdialect "c++17"
    
    files { "src/**.hpp", "src/**.cpp" }
    links { "Renderer", "Windowing", "Flecs", "Stb", "Core", "WavefrontLoader", "rgfw", "glad" }

    build_target_dir()
    build_object_dir()
    build_lib_dirs()
    build_include_dirs()

    build_target_dir()

    build_config()

    filter "platforms:linux"
        links { "GL", "X11", "Xrandr", "pthread", "dl" }
    filter "platforms:windows"
        links { "opengl32", "gdi32", "ws2_32" }
    filter ""