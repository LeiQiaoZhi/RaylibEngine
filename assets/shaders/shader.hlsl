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
void Remap(in float2 fragTexCoord, in Texture2D mainTex, in SamplerState mainTexSampler, inout float4 finalColor, in float In, in float Min, in float Max, in float NewMin, in float NewMax, out float Result) {
 Result = ((In - Min) / (Max - Min)) * (NewMax - NewMin) + NewMin; 
}

void Vec2(in float2 fragTexCoord, in Texture2D mainTex, in SamplerState mainTexSampler, inout float4 finalColor, in float X, in float Y, out float2 Result) {
 Result = float2(X, Y); 
}

void Sin(in float2 fragTexCoord, in Texture2D mainTex, in SamplerState mainTexSampler, inout float4 finalColor, in float A, out float Result) {
 Result = sin(A); 
}

void Tan(in float2 fragTexCoord, in Texture2D mainTex, in SamplerState mainTexSampler, inout float4 finalColor, in float A, out float Result) {
 Result = tan(A); 
}

void LerpV2(in float2 fragTexCoord, in Texture2D mainTex, in SamplerState mainTexSampler, inout float4 finalColor, in float2 A, in float2 B, in float In, out float2 Result) {
 Result = lerp(A, B, In); 
}

void DivideV2(in float2 fragTexCoord, in Texture2D mainTex, in SamplerState mainTexSampler, inout float4 finalColor, in float2 A, in float2 B, out float2 Result) {
 Result = float2(A.x / B.x, A.y / B.y); 
}

void Remap(in float2 fragTexCoord, in Texture2D mainTex, in SamplerState mainTexSampler, inout float4 finalColor, in float In, in float Min, in float Max, out float Result) {
 Result = clamp(In, Min, Max); 
}

void SubtractV3(in float2 fragTexCoord, in Texture2D mainTex, in SamplerState mainTexSampler, inout float4 finalColor, in float3 A, in float3 B, out float3 Result) {
 Result = A - B; 
}

void SubtractV2(in float2 fragTexCoord, in Texture2D mainTex, in SamplerState mainTexSampler, inout float4 finalColor, in float2 A, in float2 B, out float2 Result) {
 Result = A - B; 
}

void Normalize(in float2 fragTexCoord, in Texture2D mainTex, in SamplerState mainTexSampler, inout float4 finalColor, in float3 A, out float3 Result) {
 Result = normalize(A); 
}

void DivideV3(in float2 fragTexCoord, in Texture2D mainTex, in SamplerState mainTexSampler, inout float4 finalColor, in float3 A, in float3 B, out float3 Result) {
 Result = float3(A.x / B.x, A.y / B.y, A.z / B.z); 
}

void LerpV3(in float2 fragTexCoord, in Texture2D mainTex, in SamplerState mainTexSampler, inout float4 finalColor, in float3 A, in float3 B, in float In, out float3 Result) {
 Result = lerp(A, B, In); 
}

void Vec3(in float2 fragTexCoord, in Texture2D mainTex, in SamplerState mainTexSampler, inout float4 finalColor, in float X, in float Y, in float Z, out float3 Result) {
 Result = float3(X, Y, Z); 
}

void Smoothstep(in float2 fragTexCoord, in Texture2D mainTex, in SamplerState mainTexSampler, inout float4 finalColor, in float Edge0, in float Edge1, in float In, out float Result) {
 Result = smoothstep(Edge0, Edge1, In); 
}

void MultiplyV2(in float2 fragTexCoord, in Texture2D mainTex, in SamplerState mainTexSampler, inout float4 finalColor, in float2 A, in float2 B, out float2 Result) {
 Result = float2(A.x * B.x, A.y * B.y); 
}

void SmoothstepV3(in float2 fragTexCoord, in Texture2D mainTex, in SamplerState mainTexSampler, inout float4 finalColor, in float3 Edge0, in float3 Edge1, in float In, out float3 Result) {
 Result = float3(smoothstep(Edge0.x, Edge1.x, In), smoothstep(Edge0.y, Edge1.y, In), smoothstep(Edge0.z, Edge1.z, In)); 
}

void Divide(in float2 fragTexCoord, in Texture2D mainTex, in SamplerState mainTexSampler, inout float4 finalColor, in float A, in float B, out float Result) {
 Result = A / B; 
}

void AddV3(in float2 fragTexCoord, in Texture2D mainTex, in SamplerState mainTexSampler, inout float4 finalColor, in float3 A, in float3 B, out float3 Result) {
 Result = A + B; 
}

void Abs(in float2 fragTexCoord, in Texture2D mainTex, in SamplerState mainTexSampler, inout float4 finalColor, in float A, out float Result) {
 Result = abs(A); 
}

void Dot(in float2 fragTexCoord, in Texture2D mainTex, in SamplerState mainTexSampler, inout float4 finalColor, in float4 A, in float4 B, out float Result) {
 Result = dot(A, B); 
}

void Subtract(in float2 fragTexCoord, in Texture2D mainTex, in SamplerState mainTexSampler, inout float4 finalColor, in float A, in float B, out float Result) {
 Result = A - B; 
}

void AddV2(in float2 fragTexCoord, in Texture2D mainTex, in SamplerState mainTexSampler, inout float4 finalColor, in float2 A, in float2 B, out float2 Result) {
 Result = A + B; 
}

void SmoothstepV2(in float2 fragTexCoord, in Texture2D mainTex, in SamplerState mainTexSampler, inout float4 finalColor, in float2 Edge0, in float2 Edge1, in float In, out float2 Result) {
 Result = float2(smoothstep(Edge0.x, Edge1.x, In), smoothstep(Edge0.y, Edge1.y, In)); 
}

void Lerp(in float2 fragTexCoord, in Texture2D mainTex, in SamplerState mainTexSampler, inout float4 finalColor, in float A, in float B, in float In, out float Result) {
 Result = lerp(A, B, In); 
}

void MultiplyV3(in float2 fragTexCoord, in Texture2D mainTex, in SamplerState mainTexSampler, inout float4 finalColor, in float3 A, in float3 B, out float3 Result) {
 Result = float3(A.x * B.x, A.y * B.y, A.z * B.z); 
}

void SampleMainTex(in float2 fragTexCoord, in Texture2D mainTex, in SamplerState mainTexSampler, inout float4 finalColor, in float2 uv, out float4 color) {
 color = mainTex.Sample(mainTexSampler, uv); 
}

void Length(in float2 fragTexCoord, in Texture2D mainTex, in SamplerState mainTexSampler, inout float4 finalColor, in float3 V, out float Length) {
 Length = length(V); 
}

void Pow(in float2 fragTexCoord, in Texture2D mainTex, in SamplerState mainTexSampler, inout float4 finalColor, in float A, in float B, out float Result) {
 Result = pow(A, B); 
}

void Add(in float2 fragTexCoord, in Texture2D mainTex, in SamplerState mainTexSampler, inout float4 finalColor, in float A, in float B, out float Result) {
 Result = A + B; 
}

void UV(in float2 fragTexCoord, in Texture2D mainTex, in SamplerState mainTexSampler, inout float4 finalColor, out float2 uv) {
 uv = fragTexCoord; 
}

void Split(in float2 fragTexCoord, in Texture2D mainTex, in SamplerState mainTexSampler, inout float4 finalColor, in float4 V, out float X, out float Y, out float Z, out float W) {
 X = V.x; Y = V.y; Z = V.z; W = V.w; 
}

void Cross(in float2 fragTexCoord, in Texture2D mainTex, in SamplerState mainTexSampler, inout float4 finalColor, in float3 A, in float3 B, out float3 Result) {
 Result = cross(A, B); 
}

void Cos(in float2 fragTexCoord, in Texture2D mainTex, in SamplerState mainTexSampler, inout float4 finalColor, in float A, out float Result) {
 Result = cos(A); 
}

void PI(in float2 fragTexCoord, in Texture2D mainTex, in SamplerState mainTexSampler, inout float4 finalColor, out float HalfPI, out float PI, out float TwoPI) {
 HalfPI = 1.5707963267948966; PI = 3.141592653589793; TwoPI = 6.283185307179586; 
}

void ArcTan(in float2 fragTexCoord, in Texture2D mainTex, in SamplerState mainTexSampler, inout float4 finalColor, in float Y, in float X, out float Result) {
 Result = atan2(Y, X); 
}

void Multiply(in float2 fragTexCoord, in Texture2D mainTex, in SamplerState mainTexSampler, inout float4 finalColor, in float A, in float B, out float Result) {
 Result = A * B; 
}

void Final(in float2 fragTexCoord, in Texture2D mainTex, in SamplerState mainTexSampler, inout float4 finalColor, in float3 color, in float alpha) {
finalColor = float4(color, alpha);
}

void main(in float2 fragTexCoord, in Texture2D mainTex, in SamplerState mainTexSampler, inout float4 finalColor){
	float input_725352577_X;
	input_725352577_X = 1;
	float input_725352577_Y;
	input_725352577_Y = 1;
	float input_725352577_Z;
	input_725352577_Z = 1;
	float3 output_725352577_Result;
	Vec3(fragTexCoord, mainTex, mainTexSampler, finalColor, input_725352577_X, input_725352577_Y, input_725352577_Z, output_725352577_Result);
	float2 output_1760802544_uv;
	UV(fragTexCoord, mainTex, mainTexSampler, finalColor, output_1760802544_uv);
	float2 input_2772404041_uv;
	input_2772404041_uv = output_1760802544_uv;
	float4 output_2772404041_color;
	SampleMainTex(fragTexCoord, mainTex, mainTexSampler, finalColor, input_2772404041_uv, output_2772404041_color);
	float3 input_2616251015_A;
	input_2616251015_A = output_725352577_Result;
	float3 input_2616251015_B;
	convert_float4_to_float3(output_2772404041_color,input_2616251015_B);
	float3 output_2616251015_Result;
	float3 input_879261618_color;
	input_879261618_color = output_2616251015_Result;
	float input_879261618_alpha;
	input_879261618_alpha = 1;
	input_2616251015_B = 1-input_2616251015_B;
	Final(fragTexCoord, mainTex, mainTexSampler, finalColor, input_2616251015_B, input_879261618_alpha);
}
