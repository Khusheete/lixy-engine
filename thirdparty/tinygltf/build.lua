project "TinyGLTF"
    kind "StaticLib"
    language "C"
    
    files { "inc.hpp", "implementation.cpp" }
    includedirs { "." }

    build_target_dir()
    build_object_dir()
    build_lib_dirs()
    build_include_dirs()

    build_target_dir()

    build_config()