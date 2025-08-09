// Simple 2D shader

Properties
{
	unif_Texture
}

attr_00 vec2 attr_Position;
attr_01 vec2 attr_UV;

vary_vf vec2 vary_UV;

samp_00 sampler2D unif_Texture;

unif_02 mat4 unif_MVP;

outp_00 vec4 outp_Color;

void VertexMain()
{
	vary_UV = attr_UV;
	gl_Position = unif_MVP * vec4(attr_Position, 0.0, 1.0);
}

void FragmentMain()
{
	vec4 color = texture(unif_Texture, vary_UV);
	outp_Color = color;
}
