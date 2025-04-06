#version 330
in vec2 fragTexCoord;
out vec4 finalColor;
uniform sampler2D mainTex;
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
void Remap(in float In, in float Min, in float Max, in float NewMin, in float NewMax, out float Result) {
 Result = ((In - Min) / (Max - Min)) * (NewMax - NewMin) + NewMin; 
}

void Vec2(in float X, in float Y, out vec2 Result) {
 Result = vec2(X, Y); 
}

void Sin(in float A, out float Result) {
 Result = sin(A); 
}

void Tan(in float A, out float Result) {
 Result = tan(A); 
}

void LerpV2(in vec2 A, in vec2 B, in float In, out vec2 Result) {
 Result = mix(A, B, In); 
}

void DivideV2(in vec2 A, in vec2 B, out vec2 Result) {
 Result = vec2(A.x / B.x, A.y / B.y); 
}

void Remap(in float In, in float Min, in float Max, out float Result) {
 Result = clamp(In, Min, Max); 
}

void SubtractV3(in vec3 A, in vec3 B, out vec3 Result) {
 Result = A - B; 
}

void SubtractV2(in vec2 A, in vec2 B, out vec2 Result) {
 Result = A - B; 
}

void Normalize(in vec3 A, out vec3 Result) {
 Result = normalize(A); 
}

void DivideV3(in vec3 A, in vec3 B, out vec3 Result) {
 Result = vec3(A.x / B.x, A.y / B.y, A.z / B.z); 
}

void LerpV3(in vec3 A, in vec3 B, in float In, out vec3 Result) {
 Result = mix(A, B, In); 
}

void Vec3(in float X, in float Y, in float Z, out vec3 Result) {
 Result = vec3(X, Y, Z); 
}

void Smoothstep(in float Edge0, in float Edge1, in float In, out float Result) {
 Result = smoothstep(Edge0, Edge1, In); 
}

void MultiplyV2(in vec2 A, in vec2 B, out vec2 Result) {
 Result = vec2(A.x * B.x, A.y * B.y); 
}

void SmoothstepV3(in vec3 Edge0, in vec3 Edge1, in float In, out vec3 Result) {
 Result = vec3(smoothstep(Edge0.x, Edge1.x, In), smoothstep(Edge0.y, Edge1.y, In), smoothstep(Edge0.z, Edge1.z, In)); 
}

void Divide(in float A, in float B, out float Result) {
 Result = A / B; 
}

void AddV3(in vec3 A, in vec3 B, out vec3 Result) {
 Result = A + B; 
}

void Abs(in float A, out float Result) {
 Result = abs(A); 
}

void Dot(in vec4 A, in vec4 B, out float Result) {
 Result = dot(A, B); 
}

void Subtract(in float A, in float B, out float Result) {
 Result = A - B; 
}

void AddV2(in vec2 A, in vec2 B, out vec2 Result) {
 Result = A + B; 
}

void SmoothstepV2(in vec2 Edge0, in vec2 Edge1, in float In, out vec2 Result) {
 Result = vec2(smoothstep(Edge0.x, Edge1.x, In), smoothstep(Edge0.y, Edge1.y, In)); 
}

void Lerp(in float A, in float B, in float In, out float Result) {
 Result = mix(A, B, In); 
}

void MultiplyV3(in vec3 A, in vec3 B, out vec3 Result) {
 Result = vec3(A.x * B.x, A.y * B.y, A.z * B.z); 
}

void SampleMainTex(in vec2 uv, out float color) {
 color = texture(mainTex, uv); 
}

void Length(in vec3 V, out float Length) {
 Length = length(V); 
}

void Pow(in float A, in float B, out float Result) {
 Result = pow(A, B); 
}

void Add(in float A, in float B, out float Result) {
 Result = A + B; 
}

void UV(out vec2 uv) {
 uv = fragTexCoord; 
}

void Split(in vec4 V, out float X, out float Y, out float Z, out float W) {
 X = V.x; Y = V.y; Z = V.z; W = V.w; 
}

void Cross(in vec3 A, in vec3 B, out vec3 Result) {
 Result = cross(A, B); 
}

void Cos(in float A, out float Result) {
 Result = cos(A); 
}

void PI(out float HalfPI, out float PI, out float TwoPI) {
 HalfPI = 1.5707963267948966; PI = 3.141592653589793; TwoPI = 6.283185307179586; 
}

void ArcTan(in float Y, in float X, out float Result) {
 Result = atan(Y, X); 
}

void Multiply(in float A, in float B, out float Result) {
 Result = A * B; 
}

void Final(in vec3 color, in float alpha) {
finalColor = vec4(color, alpha);
}

void main(){
	float input_725416546_X;
	input_725416546_X = 0.6000000238;
	float input_725416546_Y;
	input_725416546_Y = 0.3000000119;
	float input_725416546_Z;
	input_725416546_Z = 0.3000000119;
	vec3 output_725416546_Result;
	Vec3(input_725416546_X, input_725416546_Y, input_725416546_Z, output_725416546_Result);
	float input_725356669_X;
	input_725356669_X = 0.5;
	float input_725356669_Y;
	input_725356669_Y = 0.6999999881;
	float input_725356669_Z;
	input_725356669_Z = 1;
	vec3 output_725356669_Result;
	Vec3(input_725356669_X, input_725356669_Y, input_725356669_Z, output_725356669_Result);
	float input_742186553_X;
	input_742186553_X = 0.5;
	float input_742186553_Y;
	input_742186553_Y = 0.5;
	vec2 output_742186553_Result;
	Vec2(input_742186553_X, input_742186553_Y, output_742186553_Result);
	vec2 output_1760778010_uv;
	UV(output_1760778010_uv);
	vec2 input_2633002973_A;
	input_2633002973_A = output_1760778010_uv;
	vec2 input_2633002973_B;
	input_2633002973_B = output_742186553_Result;
	vec2 output_2633002973_Result;
	SubtractV2(input_2633002973_A, input_2633002973_B, output_2633002973_Result);
	vec4 input_379690931_V;
	convert_vec2_to_vec4(output_2633002973_Result,input_379690931_V);
	float output_379690931_X;
	float output_379690931_Y;
	float output_379690931_Z;
	float output_379690931_W;
	Split(input_379690931_V, output_379690931_X, output_379690931_Y, output_379690931_Z, output_379690931_W);
	float input_2742869785_Y;
	input_2742869785_Y = output_379690931_Y;
	float input_2742869785_X;
	input_2742869785_X = output_379690931_X;
	float output_2742869785_Result;
	ArcTan(input_2742869785_Y, input_2742869785_X, output_2742869785_Result);
	float input_2805251754_A;
	input_2805251754_A = output_2742869785_Result;
	float input_2805251754_B;
	input_2805251754_B = 8;
	float output_2805251754_Result;
	Multiply(input_2805251754_A, input_2805251754_B, output_2805251754_Result);
	float input_1578696448_A;
	input_1578696448_A = output_2805251754_Result;
	float output_1578696448_Result;
	Cos(input_1578696448_A, output_1578696448_Result);
	float input_2805256430_A;
	input_2805256430_A = output_1578696448_Result;
	float input_2805256430_B;
	input_2805256430_B = 0.0700000003;
	float output_2805256430_Result;
	Multiply(input_2805256430_A, input_2805256430_B, output_2805256430_Result);
	vec3 input_3428714217_V;
	convert_vec2_to_vec3(output_2633002973_Result,input_3428714217_V);
	float output_3428714217_Length;
	Length(input_3428714217_V, output_3428714217_Length);
	float input_2646859247_A;
	input_2646859247_A = output_3428714217_Length;
	float input_2646859247_B;
	input_2646859247_B = output_2805256430_Result;
	float output_2646859247_Result;
	Add(input_2646859247_A, input_2646859247_B, output_2646859247_Result);
	float input_3632677935_Edge0;
	input_3632677935_Edge0 = 0.400000006;
	float input_3632677935_Edge1;
	input_3632677935_Edge1 = 0.200000003;
	float input_3632677935_In;
	input_3632677935_In = output_2646859247_Result;
	float output_3632677935_Result;
	Smoothstep(input_3632677935_Edge0, input_3632677935_Edge1, input_3632677935_In, output_3632677935_Result);
	vec3 input_3224017289_A;
	input_3224017289_A = output_725416546_Result;
	vec3 input_3224017289_B;
	input_3224017289_B = output_725356669_Result;
	float input_3224017289_In;
	input_3224017289_In = output_3632677935_Result;
	vec3 output_3224017289_Result;
	LerpV3(input_3224017289_A, input_3224017289_B, input_3224017289_In, output_3224017289_Result);
	vec3 input_879279141_color;
	input_879279141_color = output_3224017289_Result;
	float input_879279141_alpha;
	input_879279141_alpha = 1;
	Final(input_879279141_color, input_879279141_alpha);
}
