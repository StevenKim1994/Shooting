#version 150

#if GL_ES

precision mediump float;

#endif

out vec4 fragColor;

// uniform == only one in
uniform vec2 resolution;
uniform sampler2D texID;


void main()
{
	vec2 uv = gl_FragCoord.xy / resolution; // world => st (uv)

	if(uv.x < 0.5)
	{	if(uv.y < 0.5)
			fragColor = vec4(1,0,0,1);
		else
			fragColor = vec4(0,1,0,1);
	}
	else
	{
		if(uv.y <0.5)
			fragColor = vec4(0,0,1,1);
		else
			fragColor = vec4(1,1,1,1);
	}
}

