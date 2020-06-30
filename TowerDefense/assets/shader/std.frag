#version 150

#if GL_ES

precision mediump float;

#endif


in vec2 texCoordV;
in vec4 colorV;


// uniform == only one in
uniform vec2 resolution;
uniform sampler2D texID;

out vec4 fragColor;

void main()
{
	vec4 c = texture(texID, texCoordV) * colorV;
	//float grey = 0.299*c.r + 0.578 * c.g + 0.114 * c.b;
	//fragColor = vec4(grey, grey, grey, c.a);
	fragColor = c.yzzw;

	fragColor = vec4(1.);
}

