project "Stb"
    kind "StaticLib"
    language "C"
    
    files { "implementation.c", "**.h" }
    includedirs { "." }

    build_target_dir()
    build_object_dir()
    build_lib_dirs()
    build_include_dirs()

    build_target_dir()

    build_config()