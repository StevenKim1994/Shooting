#version 150

#if GL_ES

precision mediump float;

#endif

//in != uniform
in vec4 position;
//float p[16] = { // x,x,0,1 = 0,1 is vec2 to vec4 defulat value
//0,1,0,1			1,1,0,1
//0,0,0,1			1,0,0,1
//};

void main()
{
	// bottom left, 0,0, bottom right 1, 0 , top right 1,1 top left 1,0
	gl_Position = position; 

}

