// float

void convert_float_to_int(const float f, out int i) {
    i = int(f);
}

void convert_int_to_float(const int i, out float f) {
    f = float(i);
}

void convert_float_to_vec2(const float f, out vec2 v) {
    v = vec2(f, f);
}

void convert_float_to_vec3(const float f, out vec3 v) {
    v = vec3(f, f, f);
}

void convert_float_to_vec4(const float f, out vec4 v) {
    v = vec4(f, f, f, f);
}

// vectors

void convert_vec2_to_vec3(const vec2 v, out vec3 r) {
    r = vec3(v, 0.0);
}

void convert_vec3_to_vec2(const vec3 v, out vec2 r) {
    r = v.xy;
}

void convert_vec2_to_vec4(const vec2 v, out vec4 r) {
    r = vec4(v, 0.0, 0.0);
}

void convert_vec4_to_vec2(const vec4 v, out vec2 r) {
    r = v.xy;
}

void convert_vec3_to_vec4(const vec3 v, out vec4 r) {
    r = vec4(v, 0.0);
}

void convert_vec4_to_vec3(const vec4 v, out vec3 r) {
    r = v.xyz;
}