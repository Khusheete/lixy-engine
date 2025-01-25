require "ecc/ecc"

local syshost = "unknown"
local ver = os.getversion()
local hosttype = ver.description

if hosttype:sub(1,5) == "Linux" then
    syshost = "Linux"
elseif hosttype:sub(1,8) == "Mac OS X" then
    syshost = "MacOSX"
elseif hosttype:sub(1,7) == "Windows" then
    syshost = "Nt"
else
    syshost = hosttype
end

workspace "opengl-toy-renderer"
    configurations { "debug", "release-debug", "release" }
    platforms { "linux", "windows" }


function build_config()
    filter "platforms:linux"
        system "linux"
        toolset "gcc"
        defines { "LIXY_LINUX" }
    
    filter "platforms:windows"
        system "windows"
        if syshost == "Linux" then
            gccprefix "x86_64-w64-mingw32-"
            toolset "gcc"
        elseif syshost == "Nt" then
            toolset "msc"
        end
        buildoptions { "-static", "-static-libgcc", "-static-libstdc++" }
        defines { "LIXY_WINDOWS" }
    
    filter "configurations:debug"
        symbols "Full"
    
    filter "configurations:release-debug or release"
        defines { "NDEBUG" }
        optimize "On"
    
    filter "configurations:release-debug"
        symbols "Full"
    
    filter ""
end

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


include "core/build.lua"
include "thirdparty/build.lua"
include "wavefront_loader/build.lua"
include "renderer/build.lua"
include "windowing/build.lua"
include "demo/build.lua"