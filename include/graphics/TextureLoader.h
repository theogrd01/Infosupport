#pragma once

#include <d3d11.h>
#include <string>

struct Texture
{
    void* id = nullptr;
    int width = 0;
    int height = 0;
};

class TextureLoader
{
public:

    static void Initialize(ID3D11Device* device);

    static Texture LoadLogo(
        const std::string& path);
};