#pragma once


#include <cstdint>
#include <string>


namespace lixy::opengl {

    enum class TextureFormat {
        R8, RG8, RGB8, RGBA8
    };

    
    class BaseTexture {
    public:
        virtual void bind(uint32_t p_location = 0) const = 0;
        virtual void unbind() const = 0;
        virtual bool is_valid() const = 0;
    };


    class Texture2D : public BaseTexture {
    public:
        virtual void bind(uint32_t p_location = 0) const override;
        virtual void unbind() const override;

        bool is_valid() const override;

        static Texture2D load(const std::string &p_path);

        Texture2D() = default;
        Texture2D(const void *p_data, int p_width, int p_height, TextureFormat p_format);
        Texture2D(Texture2D &&p_other);
        Texture2D &operator=(Texture2D &&p_other);
        virtual ~Texture2D();

    private:
        uint32_t texture_id;
        bool valid = false;
    };
}