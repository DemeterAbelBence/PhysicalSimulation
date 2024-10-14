#version 410 core

precision highp float;

layout(quads, fractional_odd_spacing, ccw) in;

// uniform variables
uniform mat4 M;
uniform mat4 MI;
uniform mat4 V;
uniform mat4 P;

uniform float MIN_DIST;
uniform float MAX_DIST;

uniform vec3 eye;
uniform vec3 light;

uniform float amplitude_factor;
uniform float phase_factor;
uniform int iterations;

//const variables
const float rand_max = 10000;
const float pi = 3.14159;

// out variables
out vec3 n_vec;
out vec3 l_vec;
out vec3 v_vec;
out vec2 tex_coord;
out float height;
out float bottom;

float rand1(float x, float z) {
    return rand_max *
        abs(cos(sin(sqrt(dot(vec2(x, z), vec2(12.988, 78.233))) * 4765.134)));
}

float rand2(float x, float z) {
    return rand_max *
        abs(sin(fract(sqrt(dot(vec2(z, x), vec2(56.988, 632.233))) * 1542.789)));
}

float calculate_amplitude(float f1, float f2, float A0) {
    float A;
    float s = sqrt(f1 * f1 + f2 * f2);

    if (s > 0) A = A0 / s;
    else A = A0;
    return amplitude_factor * A;
}

float calculate_phase(float x, float z, float r) {
    return phase_factor * pi * r;
}

void main() {
    // vertex patch position and texture coordinate calculation
    float u = gl_TessCoord.x;
    float v = gl_TessCoord.y;

    vec4 p00 = gl_in[0].gl_Position;
    vec4 p01 = gl_in[1].gl_Position;
    vec4 p10 = gl_in[2].gl_Position;
    vec4 p11 = gl_in[3].gl_Position;

    vec4 p0 = (p01 - p00) * u + p00;
    vec4 p1 = (p11 - p10) * u + p10;
    vec4 p = (p1 - p0) * v + p0;

    float p_len = abs(p01.x - p00.x);
    tex_coord.x = u / p_len;
    tex_coord.y = v / p_len;

    //_____________________//

    // vertex height position and normal calculation
    int k = iterations;
    float nx = 0;
    float nz = 0;
    float height1 = 0;
    float height2 = 0;
    for (int f1 = 0; f1 < k; f1++) {
        for (int f2 = 0; f2 < k; f2++) {
            float amp1 = calculate_amplitude(f1, f2, rand1(f1, f2) / rand_max);
            float pha1 = calculate_phase(f1, f2, rand1(f1, f2));
            height1 += amp1 * cos(f1 * pi * p.x + f2 * pi * p.z + pha1);
            
            float amp2 = calculate_amplitude(f1, f2, rand2(f1, f2) / rand_max);
            float pha2 = calculate_phase(f1, f2, rand2(f1, f2));
            height2 += amp2 * cos(f1 * pi * p.x + f2 * pi * p.z + pha2);

            nx += -amp1 * f1 * sin(f1 * p.x + f2 * p.z + pha1);
            nz += -amp1 * f2 * sin(f1 * p.x + f2 * p.z + pha1);
        }
    }

    p.y += (0.5 * height1 + 0.5 * height2);

    if (p.y < p00.y) {
        //float d = abs(p00.y - p.y);
        //p.y = p00.y - d / 5;
        p.y = p00.y;
    }
    //_____________________//

    // final calculations and out variables  
    vec3 normal = vec3(nx, nz, 1);
    vec4 world_position = p * M;
    gl_Position = world_position * V * P;

    n_vec = normalize(normal);
    l_vec = normalize(light.xyz - world_position.xyz);
    v_vec = normalize(eye - world_position.xyz);
    height = p.y;
    bottom = p00.y;
    //_____________________//
}