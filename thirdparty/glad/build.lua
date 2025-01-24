project "glad"
    kind "StaticLib"
    language "C++"
    cppdialect "c++11"
    
    files { "src/glad.c", "include/**.h" }
    includedirs { "." }

    build_target_dir()
    build_object_dir()
    build_lib_dirs()
    build_include_dirs()

    build_target_dir()

    build_config()