libraries = {
    "flecs", "stb", "rgfw", "glad"
}

for i, lib in ipairs(libraries) do
    include(lib .. "/build.lua")
end