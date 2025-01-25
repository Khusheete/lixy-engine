project "rgfw"
    kind "StaticLib"
    language "C"
    cdialect "C99"
    
    files { "RGFW.c", "RGFW.h" }
    includedirs { "." }

    build_target_dir()
    build_object_dir()
    build_lib_dirs()
    build_include_dirs()

    build_target_dir()

    build_config()