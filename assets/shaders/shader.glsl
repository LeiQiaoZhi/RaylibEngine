#version 330
in vec2 fragTexCoord;
uniform sampler2D mainTex;
out vec4 finalColor;
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

void Clamp(in float In, in float Min, in float Max, out float Result) {
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

void Floor(in float A, out float Result) {
 Result = floor(A); 
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

void Mod(in float A, in float B, out float Result) {
 Result = mod(A, B); 
}

void Smoothstep(in float Edge0, in float Edge1, in float In, out float Result) {
 Result = smoothstep(Edge0, Edge1, In); 
}

void FlipY(in vec2 A, out vec2 Result) {
 Result = vec2(A.x , 1.-A.y); 
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

void Float(in float A, out float Result) {
 Result = A; 
}

void SampleMainTex(in vec2 uv, out vec4 color) {
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

void Fraction(in float A, out float Result) {
 Result = fract(A); 
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
	float input_725363518_X;
	input_725363518_X = 0;
	float input_725363518_Y;
	input_725363518_Y = 0;
	float input_725363518_Z;
	input_725363518_Z = 0.6000000238;
	vec3 output_725363518_Result;
	Vec3(input_725363518_X, input_725363518_Y, input_725363518_Z, output_725363518_Result);
	float input_725416577_X;
	input_725416577_X = 0.5;
	float input_725416577_Y;
	input_725416577_Y = 1;
	float input_725416577_Z;
	input_725416577_Z = 0.5;
	vec3 output_725416577_Result;
	Vec3(input_725416577_X, input_725416577_Y, input_725416577_Z, output_725416577_Result);
	float input_1283554549_A;
	input_1283554549_A = 4;
	float output_1283554549_Result;
	Float(input_1283554549_A, output_1283554549_Result);
	float input_725352577_X;
	input_725352577_X = 0.2126000077;
	float input_725352577_Y;
	input_725352577_Y = 0.7155999541;
	float input_725352577_Z;
	input_725352577_Z = 0.07220000029;
	vec3 output_725352577_Result;
	Vec3(input_725352577_X, input_725352577_Y, input_725352577_Z, output_725352577_Result);
	vec2 output_1760802544_uv;
	UV(output_1760802544_uv);
	vec2 input_2772404041_uv;
	input_2772404041_uv = output_1760802544_uv;
	vec4 output_2772404041_color;
	SampleMainTex(input_2772404041_uv, output_2772404041_color);
	vec4 input_946699418_A;
	convert_vec3_to_vec4(output_725352577_Result,input_946699418_A);
	vec4 input_946699418_B;
	input_946699418_B = output_2772404041_color;
	float output_946699418_Result;
	Dot(input_946699418_A, input_946699418_B, output_946699418_Result);
	float input_2805226371_A;
	input_2805226371_A = output_1283554549_Result;
	float input_2805226371_B;
	input_2805226371_B = output_946699418_Result;
	float output_2805226371_Result;
	Multiply(input_2805226371_A, input_2805226371_B, output_2805226371_Result);
	float input_1587846372_A;
	input_1587846372_A = output_2805226371_Result;
	float output_1587846372_Result;
	Floor(input_1587846372_A, output_1587846372_Result);
	float input_2103965715_A;
	input_2103965715_A = output_1587846372_Result;
	float input_2103965715_B;
	input_2103965715_B = output_1283554549_Result;
	float output_2103965715_Result;
	Divide(input_2103965715_A, input_2103965715_B, output_2103965715_Result);
	vec3 input_3224031352_A;
	input_3224031352_A = output_725363518_Result;
	vec3 input_3224031352_B;
	input_3224031352_B = output_725416577_Result;
	float input_3224031352_In;
	input_3224031352_In = output_2103965715_Result;
	vec3 output_3224031352_Result;
	LerpV3(input_3224031352_A, input_3224031352_B, input_3224031352_In, output_3224031352_Result);
	vec3 input_879261618_color;
	input_879261618_color = output_3224031352_Result;
	float input_879261618_alpha;
	input_879261618_alpha = 1;
	Final(input_879261618_color, input_879261618_alpha);
}
