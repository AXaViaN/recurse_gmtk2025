// Character shader

Properties
{
	unif_Color
	unif_Time
}

attr_00 vec2 attr_Position;
attr_01 vec2 attr_UV;

vary_vf vec2 vary_UV;

unif_00 vec3 unif_Color;
unif_01 float unif_Time;

unif_03 mat4 unif_MVP;

outp_00 vec4 outp_Color;

void VertexMain()
{
	vary_UV = attr_UV;
	gl_Position = unif_MVP * vec4(attr_Position, 0.0, 1.0);
}

void FragmentMain()
{
	vec2 center = vec2(0.5, 0.5);
	vec2 pos = vary_UV - center;

	float dist = length(pos);

	float pulse = sin(unif_Time * 3.0) * 0.05 + 0.1;
	float radius = 0.25 + pulse;

	float circle = smoothstep(radius, radius - 0.02, dist);

	float glow = smoothstep(0.4 + pulse, 0.2, dist);

	vec3 finalColor = unif_Color.rgb * circle + unif_Color.rgb * 1.5 * glow;
	outp_Color = vec4(finalColor, circle);
}
