shader_type spatial;

const float horiz_offset = 0.0;
const float vert_offset = 0.0;
const float size = 20.0;
const float thick = 10.5;
const int crosshair_color_r = 100;
const int crosshair_color_g = 142;
const int crosshair_color_b = 100;

float F_ReShade_GetAspectRatio()
{
	return 1.77777779e+00;
}
vec2 F_ReShade_GetPixelSize()
{
	return vec2(8.68055562e-04, 1.54320989e-03);
}
vec2 F_ReShade_GetScreenSize()
{
	return vec2(1.15200000e+03, 6.48000000e+02);
}

uniform sampler2D V_ReShade_BackBuffer  : hint_screen_texture, filter_nearest;
uniform sampler2D V_ReShade_DepthBuffer : hint_depth_texture, filter_nearest;

float F_ReShade_GetLinearizedDepth(
	in vec2 texcoord)
{
	float _19 = texcoord.x / 1.00000000e+00;
	texcoord[0] = _19;
	float _22 = texcoord.y / 1.00000000e+00;
	texcoord[1] = _22;
	float _25 = texcoord.x - 0.00000000e+00;
	texcoord[0] = _25;
	float _28 = texcoord.y + 0.00000000e+00;
	texcoord[1] = _28;
	vec4 _33 = vec4(texcoord.x, texcoord.y, 0.00000000e+00, 0.00000000e+00);
	vec4 _34 = textureLod(V_ReShade_DepthBuffer, _33.xy, _33.w);
	float _37 = _34.x * 1.00000000e+00;
	float depth = _37;
	float _40 = depth * 9.99000000e+02;
	float _42 = 1.00000000e+03 - _40;
	float _43 = depth / _42;
	depth = _43;
	return depth;
}
void F_PostProcessVS(
	in uint id,
	out vec4 position,
	out vec2 texcoord)
{
	bool _53 = id == 2u;
	if (_53)
	{
		texcoord[0] = 2.00000000e+00;
	}
	else
	{
		texcoord[0] = 0.00000000e+00;
	}
	bool _60 = id == 1u;
	if (_60)
	{
		texcoord[1] = 2.00000000e+00;
	}
	else
	{
		texcoord[1] = 0.00000000e+00;
	}
	vec2 _64 = texcoord * vec2(2.00000000e+00, -2.00000000e+00);
	vec2 _66 = _64 + vec2(-1.00000000e+00, 1.00000000e+00);
	vec4 _71 = vec4(_66.x, _66.y, 0.00000000e+00, 1.00000000e+00);
	position = _71;
	return;
}
vec3 F_PS_crosshairRing(
	in vec4 pos,
	in vec2 texcoord)
{
	vec4 _83 = texture(V_ReShade_BackBuffer, texcoord);
	vec3 color = _83.xyz;
	float _87 = 5.76000000e+02 + horiz_offset;
	float _89 = 3.24000000e+02 + vert_offset;
	vec2 _90 = vec2(_87, _89);
	vec2 center = _90;
	vec3 _93 = vec3(1.00000000e+00, 1.00000000e+00, 1.00000000e+00) - color;
	vec3 invertedColor = _93;
	vec3 _104 = vec3(float(crosshair_color_r) / float(255), float(crosshair_color_g) / float(255), float(crosshair_color_b) / float(255));
	vec3 _105 = _104 * invertedColor;
	vec3 crosshairDisplayColor = _105;
	float _108 = distance(center, pos.xy);
	float dist_fromCenter = _108;
	float _110 = dist_fromCenter - size;
	float _111 = abs(_110);
	float dist_fromIdeal = _111;
	float _113 = thick - dist_fromIdeal;
	float _115 = max(_113, 0.00000000e+00);
	float _116 = _115 / thick;
	float _mix = _116;
	vec3 _119 = mix(color, crosshairDisplayColor, vec3(_mix,_mix,_mix));
	color = _119;
	return color;
}
//out vec2 _out_param2;
varying vec4 pos;
void vertex()
{
	uint _param0 = uint(VERTEX_ID);
	vec4 _param1;
	vec2 _param2;
	F_PostProcessVS(_param0, _param1, _param2);
	POSITION = _param1;
	pos = _param1;
	//_out_param2 = _param2;
	UV = _param2;
}
//out vec3 COLOR;
//in vec2 _in_param1;
void fragment()
{
	vec4 _param0 = pos;
	//vec2 _param1 = _in_param1;
	vec2 _param1 = UV;
	//ALBEDO = F_PS_crosshairRing(_param0, _param1);
	
	// well at least it compiles and gets something on-screen
	ALBEDO = pos.rgb;
	//ALBEDO = texture(V_ReShade_BackBuffer, SCREEN_UV).rgb;
}
