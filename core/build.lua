project "Core"
    kind "StaticLib"
    language "C++"
    cppdialect "c++17"
    
    files { "src/**.hpp", "src/**.cpp" }
    -- links { "Flecs" }

    build_target_dir()
    build_object_dir()
    build_lib_dirs()
    build_include_dirs()

    build_target_dir()

    build_config()