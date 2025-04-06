// Conversions for float

void convert_float_to_int(float f, out int i) {
    i = int(f);
}

void convert_int_to_float(int i, out float f) {
    f = float(i);
}

void convert_float_to_float2(float f, out float2 v) {
    v = float2(f, f);
}

void convert_float_to_float3(float f, out float3 v) {
    v = float3(f, f, f);
}

void convert_float_to_float4(float f, out float4 v) {
    v = float4(f, f, f, f);
}

// Conversions for vector types

void convert_float2_to_float3(float2 v, out float3 r) {
    r = float3(v, 0.0);
}

void convert_float3_to_float2(float3 v, out float2 r) {
    r = v.xy;
}

void convert_float2_to_float4(float2 v, out float4 r) {
    r = float4(v, 0.0, 0.0);
}

void convert_float4_to_float2(float4 v, out float2 r) {
    r = v.xy;
}

void convert_float3_to_float4(float3 v, out float4 r) {
    r = float4(v, 0.0);
}

void convert_float4_to_float3(float4 v, out float3 r) {
    r = v.xyz;
}