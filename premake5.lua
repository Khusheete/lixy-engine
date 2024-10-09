require "ecc/ecc"

workspace "opengl-toy-renderer"
    configurations { "debug", "release" }
    platforms { "linux", "windows" }


function build_target_dir()
    targetdir "%{wks.location}/bin/%{cfg.platform}/%{cfg.buildcfg}"
end

function build_object_dir()
    objdir "%{wks.location}/obj/%{cfg.platform}/%{cfg.buildcfg}/%{prj.name}"
end

function build_lib_dirs()
    libdirs { "%{wks.location}/bin/%{cfg.platform}/%{cfg.buildcfg}", "%{wks.location}/lib" }
end

function build_include_dirs()
    includedirs { "%{wks.location}" }
end


include "thirdparty/build.lua"
include "renderer/build.lua"
include "demo/build.lua"