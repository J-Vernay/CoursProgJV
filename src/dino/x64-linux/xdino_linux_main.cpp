#include <dino/xdino.h>

#include <random>
#include <unordered_map>
#include <vector>

#pragma region Sokol impl

#define SOKOL_ASSERT(c)                                                                                                \
    do {                                                                                                               \
        if (!(c))                                                                                                      \
            DINO_CRITICAL("[Sokol] " #c);                                                                              \
    } while (0)

#define SOKOL_UNREACHABLE DINO_CRITICAL("[Sokol] Unreachable")

#define SOKOL_GLCORE
#define SOKOL_IMPL
#include <sokol/sokol_app.h>
#include <sokol/sokol_gfx.h>
#include <sokol/sokol_glue.h>
#include <sokol/sokol_log.h>
#include <sokol/sokol_time.h>

#include <dino/x64-linux/xdinordr.sokol.h>

#include <stb/stb_image.h>

#pragma endregion

#pragma region Window and events implementation

constexpr DinoVec2 XDino_INIT_SIZE = {640.0f, 480.0f};
DinoVec2 gXDino_windowSize = XDino_INIT_SIZE;
DinoVec2 gXDino_renderSize = XDino_INIT_SIZE;

uint64_t gXDino_initTick = 0;
sg_shader gXDino_gfxShader{};
sg_sampler gXDino_gfxSampler{};
sg_pipeline gXDino_gfxPipeline{};
sg_pass_action gXDino_gfxPassAction{};
std::random_device gXDino_randomDevice;
std::mt19937 gXDino_rng(gXDino_randomDevice());

struct XDino_Linux_Texture {
    uint16_t texWidth = 1;
    uint16_t texHeight = 1;
    sg_image image;
    sg_view view;
};

struct XDino_Linux_Gamepad {
    bool bValid = false;
    int fd = -1;
    DinoGamepad g{};
};
std::vector<XDino_Linux_Gamepad> gXDino_gamepads;

struct alignas(16) XDino_Linux_Uniforms {
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
std::unordered_map<std::string, XDino_Linux_Texture> gXDino_textures;

std::vector<DinoDrawCall> gXDino_drawCalls;

void XDino_Linux_Init()
{
    sg_desc desc{};
    desc.environment = sglue_environment();
    desc.logger.func = slog_func;
    sg_setup(&desc);

    gXDino_gfxShader = sg_make_shader(xdinordr_shader_desc(sg_query_backend()));

    sg_pipeline_desc pldesc{};
    pldesc.shader = gXDino_gfxShader;
    pldesc.layout.attrs[ATTR_xdinordr_inPos].format = SG_VERTEXFORMAT_FLOAT2;
    pldesc.layout.attrs[ATTR_xdinordr_inPos].offset = offsetof(DinoVertex, pos);
    pldesc.layout.attrs[ATTR_xdinordr_inUV].format = SG_VERTEXFORMAT_USHORT2;
    pldesc.layout.attrs[ATTR_xdinordr_inUV].offset = offsetof(DinoVertex, u);
    pldesc.layout.attrs[ATTR_xdinordr_inColor].format = SG_VERTEXFORMAT_UBYTE4N;
    pldesc.layout.attrs[ATTR_xdinordr_inColor].offset = offsetof(DinoVertex, color);
    pldesc.color_count = 1;
    pldesc.colors[0].blend.enabled = true;
    pldesc.colors[0].blend.src_factor_rgb = SG_BLENDFACTOR_SRC_ALPHA;
    pldesc.colors[0].blend.dst_factor_rgb = SG_BLENDFACTOR_ONE_MINUS_SRC_ALPHA;
    pldesc.colors[0].blend.op_rgb = SG_BLENDOP_ADD;
    pldesc.colors[0].blend.src_factor_alpha = SG_BLENDFACTOR_ZERO;
    pldesc.colors[0].blend.dst_factor_alpha = SG_BLENDFACTOR_ONE;
    pldesc.colors[0].blend.op_alpha = SG_BLENDOP_ADD;
    pldesc.colors[0].write_mask = SG_COLORMASK_RGB;

    sg_sampler_desc sdesc{};
    gXDino_gfxSampler = sg_make_sampler(&sdesc);

    gXDino_gfxPipeline = sg_make_pipeline(&pldesc);
    gXDino_gfxPassAction.colors[0].load_action = SG_LOADACTION_CLEAR;
    gXDino_gfxPassAction.colors[0].clear_value = {0, 0, 0, 1};

    stm_setup();
    gXDino_initTick = stm_now();

    gXDino_gamepads.resize((uint32_t)DinoGamepadIdx::Keyboard + 1);
    gXDino_gamepads[(uint32_t)DinoGamepadIdx::Keyboard].bValid = true;

    Dino_GameInit();
}

void XDino_Linux_Frame()
{
    gXDino_drawCalls.resize(0);
    Dino_GameFrame(stm_sec(stm_since(gXDino_initTick)));

    sg_pass pass{};
    pass.action = gXDino_gfxPassAction;
    pass.swapchain = sglue_swapchain();
    sg_begin_pass(&pass);
    sg_apply_pipeline(gXDino_gfxPipeline);

    for (DinoDrawCall const& drawCall : gXDino_drawCalls) {
        if (!gXDino_textures.contains(drawCall.textureName)) {
            std::string filePath = "assets/" + drawCall.textureName;
            int w, h, n;
            unsigned char WHITE_TEXTURE[] = {255, 255, 255, 255};
            unsigned char* pData = stbi_load(filePath.c_str(), &w, &h, &n, 4);
            if (pData == nullptr) {
                w = 1;
                h = 1;
                pData = WHITE_TEXTURE;
            }

            sg_image_desc idesc{};
            idesc.width = w;
            idesc.height = h;
            idesc.num_slices = 1;
            idesc.num_mipmaps = 1;
            idesc.pixel_format = SG_PIXELFORMAT_RGBA8;
            idesc.data.mip_levels[0].ptr = pData;
            idesc.data.mip_levels[0].size = 4 * w * h;
            sg_image i = sg_make_image(&idesc);
            if (pData != WHITE_TEXTURE)
                stbi_image_free(pData);

            sg_view_desc vdesc{};
            vdesc.texture.image = i;
            sg_view v = sg_make_view(&vdesc);

            XDino_Linux_Texture tex;
            tex.texWidth = w;
            tex.texHeight = h;
            tex.image = i;
            tex.view = v;
            gXDino_textures.emplace(drawCall.textureName, tex);
        }

        XDino_Linux_Texture const& tex = gXDino_textures[drawCall.textureName];

        sg_buffer_desc desc{};
        desc.data.ptr = drawCall.vertices.data();
        desc.data.size = drawCall.vertices.size() * sizeof(DinoVertex);
        desc.usage.vertex_buffer = true;
        desc.usage.immutable = true;
        sg_buffer buf = sg_make_buffer(&desc);

        sg_bindings bindings{};
        bindings.vertex_buffers[0] = buf;
        bindings.views[VIEW_uTexture] = tex.view;
        bindings.samplers[SMP_uSampler] = gXDino_gfxSampler;
        sg_apply_bindings(&bindings);

        double rotationRadians = drawCall.rotation * (std::numbers::pi / 180.0);
        XDino_Linux_Uniforms u;
        u.half_vp_size_x = gXDino_renderSize.x / 2.f;
        u.half_vp_size_y = gXDino_renderSize.y / 2.f;
        u.tex_size_x = static_cast<float>(tex.texWidth);
        u.tex_size_y = static_cast<float>(tex.texHeight);
        u.offset_x = static_cast<float>(drawCall.translation.x);
        u.offset_y = static_cast<float>(drawCall.translation.y);
        u.rot_cos = static_cast<float>(std::cos(rotationRadians));
        u.rot_sin = static_cast<float>(std::sin(rotationRadians));
        u.scale = static_cast<float>(drawCall.scale);

        sg_apply_uniforms(UB_u, {&u, sizeof(u)});
        sg_draw(0, drawCall.vertices.size(), 1);
        sg_destroy_buffer(buf);
    }
    sg_end_pass();
    sg_commit();
}

void XDino_Linux_Cleanup()
{
    Dino_GameShut();
    sg_shutdown();
}

void XDino_Linux_Event(sapp_event const* pEvent)
{
    if (pEvent->type == SAPP_EVENTTYPE_RESIZED) {
        gXDino_windowSize = {(float)pEvent->window_width, (float)pEvent->window_height};
    }
    else if (pEvent->type == SAPP_EVENTTYPE_KEY_DOWN || pEvent->type == SAPP_EVENTTYPE_KEY_UP) {
        // Keyboard
        DinoGamepad& g = gXDino_gamepads[(uint32_t)DinoGamepadIdx::Keyboard].g;
        bool bPressed = pEvent->type == SAPP_EVENTTYPE_KEY_DOWN;
        switch (pEvent->key_code) {
        case SAPP_KEYCODE_UP: g.dpad_up = bPressed; break;
        case SAPP_KEYCODE_LEFT: g.dpad_left = bPressed; break;
        case SAPP_KEYCODE_RIGHT: g.dpad_right = bPressed; break;
        case SAPP_KEYCODE_DOWN: g.dpad_down = bPressed; break;
        case SAPP_KEYCODE_Z:
        case SAPP_KEYCODE_W: g.btn_up = bPressed; break; // Z ou W
        case SAPP_KEYCODE_Q:
        case SAPP_KEYCODE_A: g.btn_left = bPressed; break; // Q ou A
        case SAPP_KEYCODE_D: g.btn_right = bPressed; break;
        case SAPP_KEYCODE_S: g.btn_down = bPressed; break;
        case SAPP_KEYCODE_SPACE:
        case SAPP_KEYCODE_ENTER: g.start = bPressed; break;
        case SAPP_KEYCODE_LEFT_SHIFT:
        case SAPP_KEYCODE_RIGHT_SHIFT: g.select = bPressed; break;
        case SAPP_KEYCODE_LEFT_CONTROL:
        case SAPP_KEYCODE_RIGHT_CONTROL: g.shoulder_left = bPressed; break;
        case SAPP_KEYCODE_LEFT_ALT:
        case SAPP_KEYCODE_RIGHT_ALT: g.shoulder_right = bPressed; break;
        default: break;
        }

        float x, y, hypot;

        x = static_cast<float>(g.dpad_right) - static_cast<float>(g.dpad_left);
        y = static_cast<float>(g.dpad_down) - static_cast<float>(g.dpad_up);
        hypot = std::hypot(x, y);
        if (hypot > 0) {
            g.stick_left_x = x / hypot;
            g.stick_left_y = y / hypot;
        }
        else {
            g.stick_left_x = 0;
            g.stick_left_y = 0;
        }

        x = static_cast<float>(g.btn_right) - static_cast<float>(g.btn_left);
        y = static_cast<float>(g.btn_down) - static_cast<float>(g.btn_up);
        hypot = std::hypot(x, y);
        if (hypot > 0) {
            g.stick_right_x = x / hypot;
            g.stick_right_y = y / hypot;
        }
        else {
            g.stick_right_x = 0;
            g.stick_right_y = 0;
        }
    }
    else if (pEvent->type == SAPP_EVENTTYPE_MOUSE_MOVE) {
        DinoGamepad& g = gXDino_gamepads[(uint32_t)DinoGamepadIdx::Keyboard].g;
        g.mouse_x = pEvent->mouse_x;
        g.mouse_y = pEvent->mouse_y;
    }
}

sapp_desc sokol_main(int argc, char** argv)
{
    sapp_desc app{};
    app.init_cb = XDino_Linux_Init;
    app.frame_cb = XDino_Linux_Frame;
    app.cleanup_cb = XDino_Linux_Cleanup;
    app.event_cb = XDino_Linux_Event;
    app.width = gXDino_windowSize.x;
    app.height = gXDino_windowSize.y;
    app.sample_count = 1;
    app.swap_interval = 1;
    app.window_title = "Dino";
    app.logger.func = slog_func;
    return app;
}

#pragma endregion

#pragma region Public API

DinoVec2 XDino_GetWindowSize()
{
    return gXDino_windowSize;
}

DinoVec2 XDino_GetRenderSize()
{
    return gXDino_renderSize;
}

void XDino_SetRenderSize(DinoVec2 renderSize)
{
    gXDino_renderSize = renderSize;
}

/// Pour indiquer la couleur d'arrière-plan.
void XDino_SetClearColor(DinoColor color)
{
    constexpr float kScale = 1.f / UINT8_MAX;

    sg_color& c = gXDino_gfxPassAction.colors[0].clear_value;
    c.r = color.r * kScale;
    c.g = color.g * kScale;
    c.b = color.b * kScale;
    c.a = color.a * kScale;
}

void XDino_Draw(DinoDrawCall drawCall)
{
    if (drawCall.vertices.empty())
        return;
    gXDino_drawCalls.emplace_back(std::move(drawCall));
}

bool XDino_GetGamepad(DinoGamepadIdx idx, DinoGamepad& outGamepad)
{
    XDino_Linux_Gamepad& g = gXDino_gamepads[(uint32_t)idx];
    if (!g.bValid)
        return false;
    outGamepad = g.g;
    return true;
}

uint32_t XDino_RandomUint32(uint32_t min, uint32_t max)
{
    std::uniform_int_distribution<uint32_t> distribution(min, max);
    return distribution(gXDino_rng);
}

int32_t XDino_RandomInt32(int32_t min, int32_t max)
{
    std::uniform_int_distribution<int32_t> distribution(min, max);
    return distribution(gXDino_rng);
}

float XDino_RandomFloat(float min, float max)
{
    std::uniform_real_distribution<float> distribution(min, max);
    return distribution(gXDino_rng);
}

DinoVec2 XDino_RandomUnitVec2()
{
    // On choisit un angle entre 0 et 2*PI radians.
    std::uniform_real_distribution<float> distribution(0, 6.28318530718f);
    float angle = distribution(gXDino_rng);
    return {cosf(angle), sinf(angle)};
}
/// Ne pas appeler directement, il faut utiliser `DINO_CRITICAL`.
void _impl_XDino_Critical(char const* pFunc, int line, char const* msg)
{
    slog_func("dino", 1, 0, msg, line, pFunc, nullptr);
}

#pragma endregion