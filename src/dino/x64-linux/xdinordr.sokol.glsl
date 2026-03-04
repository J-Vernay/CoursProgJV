#pragma sokol @vs vs
layout(binding=0) uniform u {
    vec2 half_vp_size;
    vec2 tex_size;
    vec2 offset;
    float rot_cos;
    float rot_sin;
    float scale;
};

in vec2 inPos;
in uvec2 inUV;
in vec4 inColor;

out vec2 fragUV;
out vec4 fragColor;

void main()
{
    vec2 pos;
    pos.x = inPos.x * rot_cos - inPos.y * rot_sin;
    pos.y = inPos.x * rot_sin + inPos.y * rot_cos;
    pos *= scale;
    pos += offset;
    pos /= half_vp_size;
    pos -= vec2(1, 1);

    // Invert y-axis to have topleft = (0,0)
    gl_Position = vec4(pos.x, -pos.y, 0, 1);
    fragUV = inUV / tex_size;
    fragColor = inColor;
}
#pragma sokol @end

#pragma sokol @fs fs

in vec2 fragUV;
in vec4 fragColor;

out vec4 outColor;

layout(set=0, binding=0) uniform texture2D uTexture;
layout(set=0, binding=1) uniform sampler uSampler;

void main()
{
    outColor = texture(sampler2D(uTexture, uSampler), fragUV) * fragColor;
}

#pragma sokol @end

#pragma sokol @program xdinordr vs fs