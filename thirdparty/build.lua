libraries = {
    "flecs", "stb", "tinygltf"
}

for i, lib in ipairs(libraries) do
    include(lib .. "/build.lua")
end