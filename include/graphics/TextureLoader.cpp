#include "TextureLoader.h"

#include <wincodec.h>
#include <vector>

#pragma comment(lib, "windowscodecs.lib")
#pragma comment(lib, "ole32.lib")

static ID3D11Device* g_device = nullptr;
static bool g_wicInitialized = false;

namespace
{
    bool EnsureWICInitialized()
    {
        if (g_wicInitialized)
        {
            return true;
        }

        HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
        if (FAILED(hr) && hr != RPC_E_CHANGED_MODE)
        {
            return false;
        }

        g_wicInitialized = true;
        return true;
    }
}

void TextureLoader::Initialize(ID3D11Device* device)
{
    g_device = device;
    EnsureWICInitialized();
}

Texture TextureLoader::LoadLogo(const std::string& path)
{
    Texture result;

    if (g_device == nullptr || path.empty())
    {
        return result;
    }

    if (!EnsureWICInitialized())
    {
        return result;
    }

    IWICImagingFactory* factory = nullptr;
    HRESULT hr = CoCreateInstance(
        CLSID_WICImagingFactory,
        nullptr,
        CLSCTX_INPROC_SERVER,
        IID_PPV_ARGS(&factory));

    if (FAILED(hr) || factory == nullptr)
    {
        return result;
    }

    IWICBitmapDecoder* decoder = nullptr;
    const std::wstring widePath(path.begin(), path.end());
    hr = factory->CreateDecoderFromFilename(
        widePath.c_str(),
        nullptr,
        GENERIC_READ,
        WICDecodeMetadataCacheOnLoad,
        &decoder);

    if (FAILED(hr) || decoder == nullptr)
    {
        factory->Release();
        return result;
    }

    IWICBitmapFrameDecode* frame = nullptr;
    hr = decoder->GetFrame(0, &frame);
    if (FAILED(hr) || frame == nullptr)
    {
        decoder->Release();
        factory->Release();
        return result;
    }

    IWICFormatConverter* converter = nullptr;
    hr = factory->CreateFormatConverter(&converter);
    if (SUCCEEDED(hr) && converter != nullptr)
    {
        hr = converter->Initialize(
            frame,
            GUID_WICPixelFormat32bppRGBA,
            WICBitmapDitherTypeNone,
            nullptr,
            0.0,
            WICBitmapPaletteTypeCustom);
    }

    if (FAILED(hr) || converter == nullptr)
    {
        frame->Release();
        decoder->Release();
        factory->Release();
        return result;
    }

    UINT width = 0;
    UINT height = 0;
    converter->GetSize(&width, &height);

    result.width = static_cast<int>(width);
    result.height = static_cast<int>(height);

    const UINT rowPitch = width * 4;
    const UINT bufferSize = rowPitch * height;
    std::vector<BYTE> pixels(bufferSize);

    hr = converter->CopyPixels(nullptr, rowPitch, bufferSize, pixels.data());
    if (FAILED(hr))
    {
        converter->Release();
        frame->Release();
        decoder->Release();
        factory->Release();
        return result;
    }

    D3D11_TEXTURE2D_DESC textureDesc{};
    textureDesc.Width = width;
    textureDesc.Height = height;
    textureDesc.MipLevels = 1;
    textureDesc.ArraySize = 1;
    textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    textureDesc.SampleDesc.Count = 1;
    textureDesc.Usage = D3D11_USAGE_DEFAULT;
    textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

    D3D11_SUBRESOURCE_DATA initData{};
    initData.pSysMem = pixels.data();
    initData.SysMemPitch = rowPitch;
    initData.SysMemSlicePitch = bufferSize;

    ID3D11Texture2D* texture = nullptr;
    hr = g_device->CreateTexture2D(&textureDesc, &initData, &texture);
    if (SUCCEEDED(hr) && texture != nullptr)
    {
        D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc{};
        srvDesc.Format = textureDesc.Format;
        srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        srvDesc.Texture2D.MipLevels = 1;
        srvDesc.Texture2D.MostDetailedMip = 0;

        ID3D11ShaderResourceView* shaderResourceView = nullptr;
        hr = g_device->CreateShaderResourceView(texture, &srvDesc, &shaderResourceView);
        if (SUCCEEDED(hr) && shaderResourceView != nullptr)
        {
            result.id = shaderResourceView;
        }

        texture->Release();
    }

    converter->Release();
    frame->Release();
    decoder->Release();
    factory->Release();

    return result;
}