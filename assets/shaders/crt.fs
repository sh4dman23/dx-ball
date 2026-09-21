#version 330

// Input vertex attributes (from vertex shader)
in vec2 fragTexCoord;
in vec4 fragColor;

// Input uniform values
uniform sampler2D texture0;
uniform vec4 colDiffuse;
uniform float time;

// Output fragment color
out vec4 finalColor;

void main()
{
    vec2 uv = fragTexCoord;

    // Sample the texture
    vec4 color = texture(texture0, uv);

    // CRT scanlines
    float scanline = sin(uv.y * 800.0) * 0.04;
    color.rgb -= scanline;

    // CRT curve (slight barrel distortion)
    vec2 cc = uv - 0.5;
    float dist = dot(cc, cc) * 0.01;
    uv = uv + cc * (1.0 + dist) * dist;

    // Vignette effect
    float vignette = smoothstep(0.7, 0.4, length(cc) / 2);

    // RGB shift for chromatic aberration
    float shift = 0.00025;
    float r = texture(texture0, uv + vec2(shift, 0.0)).r;
    float g = texture(texture0, uv).g;
    float b = texture(texture0, uv - vec2(shift, 0.0)).b;

    // Combine effects
    color = vec4(r, g, b, 1.0);
    color.rgb *= vignette;
    color.rgb -= scanline;

    // Subtle flicker
    color.rgb *= 0.97 + 0.03 * sin(time * 10.0);

    // Add slight noise
    float noise = fract(sin(dot(uv, vec2(12.9898, 78.233)) + time) * 43758.5453);
    color.rgb += noise * 0.02;

    finalColor = color * colDiffuse * fragColor;
}
