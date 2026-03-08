/// @file xdino_win64_rdr.cpp
/// @brief Implémentation du rendu graphique sur Windows.

// Expose le reste de notre programme.
#include <dino/x64-windows/xdino_win64_rdr.h>
#include <dino/xdino.h>

#define XDINO_IMPL
#include <dino/xdino_impl.h>

// Expose la bibliothèque standard.
#include <cmath>
#include <format>
#include <map>
#include <numbers>
#include <string>
#include <unordered_map>

// Macros prédéfinies qui configurent la lecture du reste du fichier.
#define COM_NO_WINDOWS_H

// Expose l'API du rendu graphique du système d'exploitation.
#include <d3d11.h>
#include <d3dcommon.h>
#include <d3dcompiler.h>
#include <dxgi.h>

// Expose une bibliothèque externe pour lire les fichiers image.
#include <stb/stb_image.h>

// Déclaration des constantes.
constexpr size_t XDino_VERTEX_BUFFER_COUNT = 60000;

#pragma region "Déclaration des variables globales"

// En général, il faut éviter les variables globales car elles peuvent être
// modifiées de n'importe où, rendant difficile de prévoir et déboguer le programme.
// Ici, les variables globales ne sont utilisées que dans ce fichier, ce qui
// limite le problème de modification inattendue depuis l'autre bout du programme.

float gXDino_width = 1;
float gXDino_height = 1;
float gXDino_rdrWidth = 1;
float gXDino_rdrHeight = 1;

ID3D11Device* gXDino_device = nullptr;
ID3D11DeviceContext* gXDino_context = nullptr;
IDXGISwapChain* gXDino_swapchain = nullptr;
ID3D11RenderTargetView* gXDino_rtv = nullptr;
ID3D11VertexShader* gXDino_vertexShader = nullptr;
ID3D11PixelShader* gXDino_pixelShader = nullptr;
ID3D11InputLayout* gXDino_inputLayout = nullptr;
ID3D11Buffer* gXDino_constantBuffer = nullptr;
ID3D11RasterizerState* gXDino_rasterState = nullptr;
ID3D11BlendState* gXDino_blendState = nullptr;
DinoColor gXDino_clearColor = DinoColor_BLACK;

struct XDino_Win64_Texture {
    std::string name;
    uint16_t width = 1;
    uint16_t height = 1;
    ID3D11Texture2D* pTexture = nullptr;
    ID3D11ShaderResourceView* pTextureView = nullptr;
    ID3D11SamplerState* pTextureSampler = nullptr;
    bool bDestroy = false;
};
std::map<uint64_t, XDino_Win64_Texture> gXDino_textures;
uint64_t gXDino_textureCounter = 0;

struct XDino_Win64_VertexBuffer {
    std::string name;
    size_t count = 0;
    ID3D11Buffer* buffer = nullptr;
    bool bDestroy = false;
};
std::map<uint64_t, XDino_Win64_VertexBuffer> gXDino_vertexBuffers;
uint64_t gXDino_vertexBufferCounter = 1;

struct XDino_Win64_Alloc {
    size_t size;
    std::string name;
    bool bDestroy = false;
};
std::map<void*, XDino_Win64_Alloc> gXDino_allocs;

bool gXDino_bDrawStats = false;
int gXDino_statScroll = 0;
int gXDino_statScrollUser = 0;

std::vector<std::string> XDino_Win64_CollectRessources(bool bIncludeTemp = false)
{
    std::vector<std::string> lines;
    lines.emplace_back(std::format("[{} Textures]", gXDino_textures.size()));
    for (auto& [id, tex] : gXDino_textures) {
        if (bIncludeTemp || !tex.bDestroy)
            lines.push_back(
                std::format("{}. {} ({}x{}, {} bytes)", id, tex.name, tex.width, tex.height, tex.width * tex.height * 4)
            );
    }
    lines.emplace_back();
    lines.emplace_back(std::format("[{} VertexBuffers]", gXDino_vertexBuffers.size()));
    for (auto& [id, vbuf] : gXDino_vertexBuffers) {
        if (bIncludeTemp || !vbuf.bDestroy)
            lines.push_back(std::format("{}. {} ({} vertices)", id, vbuf.name, vbuf.count));
    }
    lines.emplace_back();
    lines.emplace_back(std::format("[{} MemAllocs]", gXDino_allocs.size()));
    for (auto& [id, alloc] : gXDino_allocs) {
        if (bIncludeTemp || !alloc.bDestroy)
            lines.push_back(std::format("{}. {} ({} bytes)", id, alloc.name, alloc.size));
    }
    return lines;
}

#pragma endregion

#pragma region Code source des shaders

struct alignas(16) XDino_Win64_CBuffer {
    float half_vp_size_x;
    float half_vp_size_y;
    float tex_size_x;
    float tex_size_y;
    float offset_x;
    float offset_y;
    float rot_cos;
    float rot_sin;
    float scale;
};

constexpr char XDino_VERTEX_SHADER[] = R"(
cbuffer cb
{
    float2 half_vp_size;
    float2 tex_size;
    float2 offset;
    float rot_cos;
    float rot_sin;
    float scale;
}

struct VSInput
{
    float2 pos: POSITION0;
    uint2 uv: TEXCOORD0;
    float4 color: COLOR0;
};

struct VSOutput
{
    float4 pos: SV_Position;
    float2 uv: TEXCOORD0;
    float4 color: COLOR0;
};

VSOutput main(VSInput input)
{
    VSOutput output = (VSOutput)0;

    float2 pos;
    pos.x = input.pos.x * rot_cos - input.pos.y * rot_sin;
    pos.y = input.pos.x * rot_sin + input.pos.y * rot_cos;
    pos *= scale;
    pos += offset;
    pos /= half_vp_size;
    pos -= float2(1, 1);

    // Invert y-axis to have topleft = (0,0)
    output.pos = float4(pos.x, -pos.y, 0, 1);
    output.uv = input.uv / tex_size;
    output.color = input.color;
    return output;
}
)";

constexpr char XDino_PIXEL_SHADER[] = R"(
sampler Sampler : register(s0);
Texture2D Texture : register(t0);

struct PSInput
{
    float4 pos: SV_Position;
    float2 uv: TEXCOORD0;
    float4 color: COLOR0;
};

struct PSOutput
{
    float4 color: SV_Target0;
};

PSOutput main(PSInput input)
{
    PSOutput output = (PSOutput)0;
    output.color = Texture.Sample(Sampler, input.uv);
    output.color *= input.color;
    return output;
}
)";

#pragma endregion

#pragma region "Implémentation des fonctions spécifiques à la plateforme"

void XDino_Win64_CreateRenderer(HWND hWindow, int32_t width, int32_t height)
{
    HRESULT hr = 0;

    gXDino_width = static_cast<float>(width);
    gXDino_height = static_cast<float>(height);

    DXGI_SWAP_CHAIN_DESC swapchainDesc{};
    swapchainDesc.BufferDesc.Width = width;
    swapchainDesc.BufferDesc.Height = height;
    swapchainDesc.BufferDesc.RefreshRate.Numerator = 1;
    swapchainDesc.BufferDesc.RefreshRate.Denominator = 60; // 60 fps
    swapchainDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    swapchainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    swapchainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_STRETCHED;
    swapchainDesc.SampleDesc.Count = 1;
    swapchainDesc.SampleDesc.Quality = 0;
    swapchainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapchainDesc.BufferCount = 2;
    swapchainDesc.OutputWindow = hWindow;
    swapchainDesc.Windowed = TRUE;
    swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    swapchainDesc.Flags = 0;

    D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;
    D3D_FEATURE_LEVEL outFeatureLevel = D3D_FEATURE_LEVEL_11_0;
    hr = D3D11CreateDeviceAndSwapChain(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        D3D11_CREATE_DEVICE_DEBUG,
        &featureLevel,
        1,
        D3D11_SDK_VERSION,
        &swapchainDesc,
        &gXDino_swapchain,
        &gXDino_device,
        &outFeatureLevel,
        &gXDino_context
    );
    if (FAILED(hr))
        DINO_CRITICAL("D3D11CreateDeviceAndSwapChain failed");

    ID3D11Texture2D* pBackbuffer = nullptr;
    hr = gXDino_swapchain->GetBuffer(0, IID_ID3D11Texture2D, (void**)&pBackbuffer);
    if (FAILED(hr))
        DINO_CRITICAL("gXDino_swapchain->GetBuffer failed");

    hr = gXDino_device->CreateRenderTargetView(pBackbuffer, nullptr, &gXDino_rtv);
    if (FAILED(hr))
        DINO_CRITICAL("gXDino_device->CreateRenderTargetView failed");
    pBackbuffer->Release();

    ID3DBlob* pVertexShaderBlob = nullptr;
    ID3DBlob* pPixelShaderBlob = nullptr;
    ID3DBlob* pErrorMsg = nullptr;

    hr = D3DCompile(
        XDino_VERTEX_SHADER,
        strlen(XDino_VERTEX_SHADER),
        nullptr,
        nullptr,
        nullptr,
        "main",
        "vs_5_0",
        0,
        0,
        &pVertexShaderBlob,
        &pErrorMsg
    );
    if (FAILED(hr))
        DINO_CRITICAL("D3DCompile vertex failed");

    hr = D3DCompile(
        XDino_PIXEL_SHADER,
        strlen(XDino_PIXEL_SHADER),
        nullptr,
        nullptr,
        nullptr,
        "main",
        "ps_5_0",
        0,
        0,
        &pPixelShaderBlob,
        &pErrorMsg
    );
    if (FAILED(hr))
        DINO_CRITICAL("D3DCompile pixel failed");

    hr = gXDino_device->CreateVertexShader(
        pVertexShaderBlob->GetBufferPointer(), pVertexShaderBlob->GetBufferSize(), nullptr, &gXDino_vertexShader
    );
    if (FAILED(hr))
        DINO_CRITICAL("CreateVertexShader failed");

    hr = gXDino_device->CreatePixelShader(
        pPixelShaderBlob->GetBufferPointer(), pPixelShaderBlob->GetBufferSize(), nullptr, &gXDino_pixelShader
    );
    if (FAILED(hr))
        DINO_CRITICAL("CreatePixelShader failed");

    D3D11_INPUT_ELEMENT_DESC inputs[3]{};
    inputs[0].SemanticName = "POSITION";
    inputs[0].SemanticIndex = 0;
    inputs[0].Format = DXGI_FORMAT_R32G32_FLOAT;
    inputs[0].InputSlot = 0;
    inputs[0].AlignedByteOffset = offsetof(DinoVertex, pos);
    inputs[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    inputs[1].SemanticName = "TEXCOORD";
    inputs[1].SemanticIndex = 0;
    inputs[1].Format = DXGI_FORMAT_R16G16_UINT;
    inputs[1].InputSlot = 0;
    inputs[1].AlignedByteOffset = offsetof(DinoVertex, u);
    inputs[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    inputs[2].SemanticName = "COLOR";
    inputs[2].SemanticIndex = 0;
    inputs[2].Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    inputs[2].InputSlot = 0;
    inputs[2].AlignedByteOffset = offsetof(DinoVertex, color);
    inputs[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;

    hr = gXDino_device->CreateInputLayout(
        inputs, 3, pVertexShaderBlob->GetBufferPointer(), pVertexShaderBlob->GetBufferSize(), &gXDino_inputLayout
    );
    if (FAILED(hr))
        DINO_CRITICAL("CreateInputLayout failed");

    pVertexShaderBlob->Release();
    pPixelShaderBlob->Release();

    D3D11_BUFFER_DESC cbufferDesc = {};
    cbufferDesc.ByteWidth = sizeof(XDino_Win64_CBuffer);
    cbufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    cbufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    cbufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    hr = gXDino_device->CreateBuffer(&cbufferDesc, nullptr, &gXDino_constantBuffer);
    if (FAILED(hr))
        DINO_CRITICAL("CreateBuffer constant failed");

    D3D11_RASTERIZER_DESC rasterDesc = {};
    rasterDesc.FillMode = D3D11_FILL_SOLID;
    rasterDesc.CullMode = D3D11_CULL_NONE;

    hr = gXDino_device->CreateRasterizerState(&rasterDesc, &gXDino_rasterState);
    if (FAILED(hr))
        DINO_CRITICAL("CreateRasterizerState failed");

    D3D11_BLEND_DESC blendDesc = {0};
    blendDesc.RenderTarget[0].BlendEnable = TRUE;
    blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
    blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
    blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    blendDesc.RenderTarget[0].RenderTargetWriteMask =
        D3D11_COLOR_WRITE_ENABLE_RED | D3D11_COLOR_WRITE_ENABLE_GREEN | D3D11_COLOR_WRITE_ENABLE_BLUE;
    hr = gXDino_device->CreateBlendState(&blendDesc, &gXDino_blendState);
    if (FAILED(hr))
        DINO_CRITICAL("CreateBlendState failed");

    unsigned char WHITE_TEXTURE[] = {255, 255, 255, 255};
    uint64_t texID = XDino_Win64_CreateTexture("WHITE", 1, 1, WHITE_TEXTURE);
    if (texID != XDino_TEXID_WHITE)
        DINO_CRITICAL("TEXID_WHITE illogique.");
    texID = XDino_CreateGpuTexture("monogram-bitmap.png");
    if (texID != XDino_TEXID_FONT)
        DINO_CRITICAL("TEXID_FONT illogique.");

    gXDino_vertexBuffers.emplace(0, "EMPTY");
}

void XDino_Win64_ResizeRenderer(int32_t width, int32_t height)
{
    gXDino_context->Flush();
    gXDino_rtv->Release();
    gXDino_rtv = nullptr;

    HRESULT hr = gXDino_swapchain->ResizeBuffers(0, width, height, DXGI_FORMAT_B8G8R8A8_UNORM, 0);
    if (FAILED(hr))
        DINO_CRITICAL("gXDino_swapchain->ResizeBuffers failed");

    ID3D11Texture2D* pBackbuffer = nullptr;
    hr = gXDino_swapchain->GetBuffer(0, IID_ID3D11Texture2D, (void**)&pBackbuffer);
    if (FAILED(hr))
        DINO_CRITICAL("gXDino_swapchain->GetBuffer failed");

    hr = gXDino_device->CreateRenderTargetView(pBackbuffer, nullptr, &gXDino_rtv);
    if (FAILED(hr))
        DINO_CRITICAL("gXDino_device->CreateRenderTargetView failed");
    pBackbuffer->Release();

    gXDino_width = static_cast<float>(width);
    gXDino_height = static_cast<float>(height);
}

uint64_t XDino_Win64_CreateTexture(char const* pName, int w, int h, void const* pData)
{
    HRESULT hr;

    XDino_Win64_Texture texture{};

    D3D11_TEXTURE2D_DESC texDesc = {};
    texDesc.Width = w;
    texDesc.Height = h;
    texDesc.MipLevels = 1;
    texDesc.ArraySize = 1;
    texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    texDesc.SampleDesc.Count = 1;
    texDesc.SampleDesc.Quality = 0;
    texDesc.Usage = D3D11_USAGE_IMMUTABLE;
    texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

    D3D11_SUBRESOURCE_DATA initData = {};
    initData.pSysMem = pData;
    initData.SysMemPitch = 4 * texDesc.Width;
    initData.SysMemSlicePitch = 4 * texDesc.Width * texDesc.Height;
    hr = gXDino_device->CreateTexture2D(&texDesc, &initData, &texture.pTexture);
    if (FAILED(hr))
        DINO_CRITICAL("CreateTexture2D failed");

    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
    srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MostDetailedMip = 0;
    srvDesc.Texture2D.MipLevels = 1;
    hr = gXDino_device->CreateShaderResourceView(texture.pTexture, &srvDesc, &texture.pTextureView);
    if (FAILED(hr))
        DINO_CRITICAL("CreateShaderResourceView failed");

    D3D11_SAMPLER_DESC samplerDesc = {};
    samplerDesc.Filter = D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR;
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
    hr = gXDino_device->CreateSamplerState(&samplerDesc, &texture.pTextureSampler);
    if (FAILED(hr))
        DINO_CRITICAL("CreateSamplerState failed");

    uint64_t texID = gXDino_textureCounter++;
    texture.name = pName;
    texture.width = static_cast<uint16_t>(w);
    texture.height = static_cast<uint16_t>(h);
    gXDino_textures.emplace(texID, texture);
    return texID;
}

void XDino_Win64_BeginDraw()
{
    DinoColor color = gXDino_clearColor;
    float clearColor[4] = {color.r / 255.f, color.g / 255.f, color.b / 255.f, color.a / 255.f};
    gXDino_context->ClearRenderTargetView(gXDino_rtv, clearColor);

    gXDino_context->IASetInputLayout(gXDino_inputLayout);
    gXDino_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    gXDino_context->VSSetShader(gXDino_vertexShader, nullptr, 0);
    gXDino_context->VSSetConstantBuffers(0, 1, &gXDino_constantBuffer);

    float width = gXDino_width;
    float height = gXDino_rdrHeight * gXDino_width / gXDino_rdrWidth;
    if (height > gXDino_height) {
        width = gXDino_rdrWidth * gXDino_height / gXDino_rdrHeight;
        height = gXDino_height;
    }

    D3D11_VIEWPORT viewport = {};
    viewport.TopLeftX = (gXDino_width - width) / 2;
    viewport.TopLeftY = (gXDino_height - height) / 2;
    viewport.Width = width;
    viewport.Height = height;
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    gXDino_context->RSSetViewports(1, &viewport);
    gXDino_context->RSSetState(gXDino_rasterState);

    gXDino_context->PSSetShader(gXDino_pixelShader, nullptr, 0);

    gXDino_context->OMSetRenderTargets(1, &gXDino_rtv, nullptr);

    gXDino_context->OMSetBlendState(gXDino_blendState, nullptr, 0xFFFFFFFF);
}

void XDino_Win64_EndDraw()
{
    if (gXDino_bDrawStats) {
        std::vector<std::string> lines = XDino_Win64_CollectRessources(true);

        gXDino_statScroll += gXDino_statScrollUser;
        gXDino_statScroll = XDinoImpl_DrawStats(gXDino_statScroll, gXDino_rdrHeight, 1, std::move(lines));

        gXDino_bDrawStats = false;
    }

    for (auto it = gXDino_textures.begin(); it != gXDino_textures.end();) {
        if (it->second.bDestroy)
            it = gXDino_textures.erase(it);
        else
            ++it;
    }
    for (auto it = gXDino_vertexBuffers.begin(); it != gXDino_vertexBuffers.end();) {
        if (it->second.bDestroy)
            it = gXDino_vertexBuffers.erase(it);
        else
            ++it;
    }
    for (auto it = gXDino_allocs.begin(); it != gXDino_allocs.end();) {
        if (it->second.bDestroy)
            it = gXDino_allocs.erase(it);
        else
            ++it;
    }

    XDino_ProfileBegin({0x44, 0x44, 0x44, 0xFF}, "SwapchainPresent");
    gXDino_swapchain->Present(1, 0);
    XDino_ProfileEnd();
}

void XDino_Win64_DestroyRenderer()
{
    OutputDebugStringA("--- RESOURCES ALIVE BEGIN ---");
    for (std::string const& s : XDino_Win64_CollectRessources())
        OutputDebugStringA((s + "\n").c_str());
    OutputDebugStringA("--- RESOURCES ALIVE END ---");
    std::puts("--- RESOURCES ALIVE BEGIN ---");
    for (std::string s : XDino_Win64_CollectRessources())
        std::puts(s.c_str());
    std::puts("--- RESOURCES ALIVE END ---");

    for (auto& [name, texture] : gXDino_textures) {
        texture.pTextureSampler->Release();
        texture.pTextureView->Release();
        texture.pTexture->Release();
    }
    gXDino_textures.clear();

    gXDino_blendState->Release();
    gXDino_blendState = nullptr;
    gXDino_rasterState->Release();
    gXDino_rasterState = nullptr;
    gXDino_constantBuffer->Release();
    gXDino_constantBuffer = nullptr;
    gXDino_inputLayout->Release();
    gXDino_inputLayout = nullptr;
    gXDino_pixelShader->Release();
    gXDino_pixelShader = nullptr;
    gXDino_vertexShader->Release();
    gXDino_vertexShader = nullptr;
    gXDino_rtv->Release();
    gXDino_rtv = nullptr;
    gXDino_context->Release();
    gXDino_context = nullptr;
    gXDino_swapchain->Release();
    gXDino_swapchain = nullptr;
    gXDino_device->Release();
    gXDino_device = nullptr;
}

#pragma endregion

#pragma region Public API

DinoVec2 XDino_GetWindowSize()
{
    return {gXDino_width, gXDino_height};
}

void XDino_SetClearColor(DinoColor color)
{
    gXDino_clearColor = color;
}

DinoVec2 XDino_GetRenderSize()
{
    return {gXDino_rdrWidth, gXDino_rdrHeight};
}

void XDino_SetRenderSize(DinoVec2 renderSize)
{
    if (renderSize.x < 1)
        renderSize.x = 1;
    if (renderSize.y < 1)
        renderSize.y = 1;
    gXDino_rdrWidth = renderSize.x;
    gXDino_rdrHeight = renderSize.y;
}

uint64_t XDino_CreateGpuTexture(char const* pName)
{
    std::string filePath = std::format("assets/{}", pName);
    int w, h, n;
    unsigned char* pData = stbi_load(filePath.c_str(), &w, &h, &n, 4);
    if (pData == nullptr)
        DINO_CRITICAL("Impossible de créer la texture.");

    uint64_t texID = XDino_Win64_CreateTexture(pName, w, h, pData);
    stbi_image_free(pData);
    return texID;
}

DinoVec2 XDino_GetGpuTextureSize(uint64_t texID)
{
    if (texID >= gXDino_textureCounter)
        DINO_CRITICAL("Accès à une texture qui n'a jamais existée.");
    auto it = gXDino_textures.find(texID);
    if (it == gXDino_textures.end() || it->second.bDestroy)
        DINO_CRITICAL("Accès à une texture déjà détruite.");
    return DinoVec2(it->second.width, it->second.height);
}

void XDino_DestroyGpuTexture(uint64_t texID)
{
    if (texID >= gXDino_textureCounter)
        DINO_CRITICAL("Destruction d'une texture qui n'a jamais existée.");
    auto it = gXDino_textures.find(texID);
    if (it == gXDino_textures.end() || it->second.bDestroy)
        DINO_CRITICAL("Destruction d'une texture déjà détruite.");

    it->second.pTextureSampler->Release();
    it->second.pTextureView->Release();
    it->second.pTexture->Release();
    it->second.bDestroy = true;
}

uint64_t XDino_CreateVertexBuffer(std::vector<DinoVertex> const& vertices, char const* pLabel)
{
    XDino_ProfileBegin(
        {0x44, 0x44, 0x44, 0xFF}, std::format("Create vertex buffer '{}' ({})", vertexCount, pLabel).c_str()
    );
    D3D11_BUFFER_DESC bufferDesc = {};
    bufferDesc.ByteWidth = static_cast<UINT>(sizeof(DinoVertex) * vertices.size());
    bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
    bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    D3D11_SUBRESOURCE_DATA bufferData = {};
    bufferData.pSysMem = vertices.data();

    ID3D11Buffer* buf = nullptr;
    HRESULT hr = gXDino_device->CreateBuffer(&bufferDesc, &bufferData, &buf);
    if (FAILED(hr))
        DINO_CRITICAL("CreateBuffer vertex failed");

    uint64_t vbufID = gXDino_vertexBufferCounter++;
    XDino_Win64_VertexBuffer vbuf;
    vbuf.name = pLabel;
    vbuf.count = vertices.size();
    vbuf.buffer = buf;
    gXDino_vertexBuffers.emplace(vbufID, vbuf);
    XDino_ProfileEnd();
    return vbufID;
}

void XDino_DestroyVertexBuffer(uint64_t vbufID)
{
    if (vbufID == 0)
        return;

    if (vbufID >= gXDino_vertexBufferCounter)
        DINO_CRITICAL("Destruction d'un vertex buffer qui n'a jamais existée.");
    auto it = gXDino_vertexBuffers.find(vbufID);
    if (it == gXDino_vertexBuffers.end() || it->second.bDestroy)
        DINO_CRITICAL("Destruction d'un vertex buffer déjà détruite.");
    it->second.buffer->Release();
    it->second.bDestroy = true;
}

void XDino_Draw(uint64_t vbufID, uint64_t texID, DinoVec2 translation, double scale, double rotation)
{
    XDino_ProfileBegin({0x44, 0x44, 0x44, 0xFF}, std::format("Draw {} {}", texture.name, vbuf.name).c_str());
    D3D11_MAPPED_SUBRESOURCE resource;

    auto itTex = gXDino_textures.find(texID);
    if (itTex == gXDino_textures.end() || itTex->second.bDestroy)
        DINO_CRITICAL("Impossible de trouver la texture pour le drawcall");
    XDino_Win64_Texture const& texture = itTex->second;

    if (vbufID == 0)
        return;

    auto itVbuf = gXDino_vertexBuffers.find(vbufID);
    if (itVbuf == gXDino_vertexBuffers.end() || itVbuf->second.bDestroy)
        DINO_CRITICAL("Impossible de trouver la texture pour le drawcall");
    XDino_Win64_VertexBuffer const& vbuf = itVbuf->second;

    gXDino_context->PSSetShaderResources(0, 1, &texture.pTextureView);
    gXDino_context->PSSetSamplers(0, 1, &texture.pTextureSampler);

    // Mettre à jour les transformations.

    double rotationRadians = rotation * (std::numbers::pi / 180.0);
    XDino_Win64_CBuffer cbuffer{};
    cbuffer.half_vp_size_x = gXDino_rdrWidth / 2.f;
    cbuffer.half_vp_size_y = gXDino_rdrHeight / 2.f;
    cbuffer.tex_size_x = static_cast<float>(texture.width);
    cbuffer.tex_size_y = static_cast<float>(texture.height);
    cbuffer.offset_x = static_cast<float>(translation.x);
    cbuffer.offset_y = static_cast<float>(translation.y);
    cbuffer.rot_cos = static_cast<float>(std::cos(rotationRadians));
    cbuffer.rot_sin = static_cast<float>(std::sin(rotationRadians));
    cbuffer.scale = static_cast<float>(scale);

    gXDino_context->Map(gXDino_constantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
    memcpy(resource.pData, &cbuffer, sizeof(cbuffer));
    gXDino_context->Unmap(gXDino_constantBuffer, 0);

    UINT stride = sizeof(DinoVertex);
    UINT offset = 0;
    gXDino_context->IASetVertexBuffers(0, 1, &vbuf.buffer, &stride, &offset);
    gXDino_context->Draw(static_cast<UINT>(vbuf.count), 0);
    XDino_ProfileEnd();
}

void XDino_DrawStats(int diffScroll)
{
    gXDino_bDrawStats = true;
    gXDino_statScrollUser = diffScroll;
}

#pragma endregion

#pragma region Memory allocations

void* XDino_MemAlloc(size_t size, char const* pLabel)
{
    if (size == 0)
        DINO_CRITICAL("Allouer 0 octets n'a pas de sens.");
    void* p = VirtualAlloc(nullptr, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (!p)
        DINO_CRITICAL("L'OS a échoué à allouer de la mémoire.");
    XDino_Win64_Alloc& a = gXDino_allocs[p];
    a.size = size;
    a.name = pLabel;
    return p;
}

void XDino_MemFree(void* pAlloc)
{
    if (!pAlloc)
        return;
    auto it = gXDino_allocs.find(pAlloc);
    if (it == gXDino_allocs.end())
        DINO_CRITICAL("Libération de mémoire qui n'a jamais été alloué.");
    if (it->second.size == 0)
        DINO_CRITICAL("Libération de mémoire qui a déjà été allouée.");
    VirtualFree(pAlloc, 0, MEM_RELEASE);
    it->second.bDestroy = true;
}

#pragma endregion