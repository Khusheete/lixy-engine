libraries = {
    "flecs", "stb"
}

for i, lib in ipairs(libraries) do
    include(lib .. "/build.lua")
end