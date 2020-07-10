#include "iShaderToy.h"
#include "iStd.h"

iShaderToy::iShaderToy(STInput* si)
{
	int i;

	programID = (GLuint*)calloc(sizeof(GLuint), 5);
	texBuf = (Texture**)calloc(sizeof(Texture*), 4);
	texBufChannel = (Texture***)calloc(sizeof(Texture**), 5);
	settingBufChannel = (GLenum***)malloc(sizeof(GLenum**) * 5);

	int lenCommon;
	char* strCommon = loadFile(si->strCommonPath, lenCommon);
	
	const char* strEnd[2] = {
		"\
	void main()									\
	{										 \
		mainImage(fragColor, gl_FragCoord.xy); \
					 \
	}",
		"\
	void main()									\
	{										 \
		mainImage(fragColor, gl_FragCoord.xy); \
		fragColor.a = 1.0;						 \
	}"

	};


	int lenEnd = strlen(strEnd[1]);

	for (i = 0; i < 5; i++)
	{
		STBuf* input = &si->sb[i];
		if (input->strVertPath == NULL ||
			input->strFragPath == NULL) continue;

		int length;
		char* str = loadFile(input->strVertPath, length);
		GLuint vertID = createShader(str, GL_VERTEX_SHADER);
		free(str);
		str = loadFile(input->strFragPath, length);
		char* s = (char*)calloc(sizeof(char), lenCommon + length + lenEnd + 3);
	
		sprintf(s, "%s\n%s\n%s", strCommon, str, strEnd[i<4 ? 0 : 1]);
		
		GLuint fragID = createShader(s, GL_FRAGMENT_SHADER);
		free(str);
		free(s);

		programID[i] = createProgramID(vertID, fragID);

		destroyShader(vertID);
		destroyShader(fragID);

		if (i < 4)
		{
			texBuf[i] = createTexture(devSize.width, devSize.height);
			// supply runtime.....
			//setTexture(input->channel[i].wrap, input->channel[i].filter);
			//input->channel[i].vFlip;
		}
	}

	for (i = 0; i < 5; i++)
	{
		STBuf* input = &si->sb[i];
		if (input->strVertPath == NULL ||
			input->strFragPath == NULL) continue;

		texBufChannel[i] = (Texture**)calloc(sizeof(Texture*), 4);
		settingBufChannel[i] = (GLenum**)malloc(sizeof(GLenum*) * 4);
		for (int j = 0; j < 4; j++)
		{
			int buf = input->channel[j].buf;
			if (buf != -1)
			{
				texBufChannel[i][j] = texBuf[buf];
				texBuf[buf]->retainCount++;
			}
			else if (input->channel[j].strPath)
			{
				texBufChannel[i][j] = createImage(input->channel[j].strPath);
			}

			settingBufChannel[i][j] = (GLenum*)malloc(sizeof(GLenum) * 2);
			settingBufChannel[i][j][0] = input->channel[i].wrap;
			settingBufChannel[i][j][1] = input->channel[i].filter;
			//setTexture(input->channel[i].wrap, input->channel[i].filter);
			//input->channel[i].vFlip;
		}
	}




	takeTime = 0.0f;
	frame = 0;
	memset(mouse, 0x00, sizeof(float) * 4);
}

iShaderToy::~iShaderToy()
{
	int i, j;

	for (i = 0; i < 5; i++)
	{
		if (programID[i])
			destroyProgram(programID[i]);
		if (i < 4)
		{
			if (texBuf[i])
				freeImage(texBuf[i]);
		}

		for (j = 0; j < 4; j++)
		{
			if (texBufChannel[i][j])
				freeImage(texBufChannel[i][j]);
		}
		free(texBufChannel[i]);
	}
	free(programID);
	free(texBuf);
	free(texBufChannel);
}

void iShaderToy::paint(float dt)
{
	int i, j;
	char s[64];

	for (i = 0; i < 5; i++)
	{
		GLuint pid = programID[i];
		if (pid == 0) continue;

		if (i < 4)
			fbo->bind(texBuf[i]);

		glViewport(0, 0, devSize.width, devSize.height);
		glClearColor(1, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(pid);

		float p[16] = {
			-1, 1, 0, 1,		1, 1, 0, 1,
			-1, -1, 0, 1,		1, -1, 0, 1
		};
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * 16, p);

		GLuint positionAttr = glGetAttribLocation(pid, "position");
		glEnableVertexAttribArray(positionAttr);
		glVertexAttribPointer(positionAttr, 4, GL_FLOAT, GL_FALSE, 0, (const void*)0);
		// glGetUniformLocation != -1, 실행
		glUniform3f(glGetUniformLocation(pid, "iResolution"), devSize.width, devSize.height, 0);
		glUniform1f(glGetUniformLocation(pid, "iTime"), takeTime);
		takeTime += dt;
		glUniform1f(glGetUniformLocation(pid, "iTimeDelta"), dt);
		glUniform1i(glGetUniformLocation(pid, "iFrame"), frame);
		frame++;
		glUniform4fv(glGetUniformLocation(pid, "iMouse"), 1, mouse);
		for (j = 0; j < 4; j++)
		{
			Texture* tex = texBufChannel[i][j];
			float channelResolution[3] = { 0,0,0 };
			if (tex)
			{
				sprintf(s, "iChannel%d", j);
				glUniform1i(glGetUniformLocation(pid, s), j);
				glActiveTexture(GL_TEXTURE0 + j);
				glBindTexture(GL_TEXTURE_2D, tex->texID);
				setTexture((TextureWrap)settingBufChannel[i][j][0],
				(TextureFilter)settingBufChannel[i][j][0]);
				channelResolution[0] = tex->width;
				channelResolution[1] = tex->height;
			}
			char s[32];
			sprintf(s, "iChannelResolution[%d]", j);
			glUniform3fv(glGetUniformLocation(pid, s), 1, channelResolution);
		}

		//glUniform1iv(glGetUniformLocation(pid, "ikeyboard"), 5, keyboard) //ATI X NVIDIA O
		for (j = 0; j < 5; j++)
		{
			char s[32];
			sprintf(s, "ikeyboard[%d]", j);
			glUniform1i(glGetUniformLocation(pid, s), keyboard[j]);
		}

		uint8 indices[6] = { 0, 1, 2,  1, 2, 3 };
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, indices);

		if (i < 4)
			fbo->unbind();

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDisableVertexAttribArray(positionAttr);

		for (j = 0; j < 4; j++)
		{
			Texture* tex = texBufChannel[i][j];
			if (tex)
			{
				glActiveTexture(GL_TEXTURE0 + j);
				glBindTexture(GL_TEXTURE_2D, 0);
			}
		}
	}
	// key
	//getkeyStat

	uint32 key = getKeyStat();
	
	if (key & keyboard_up)
		keyboard[0] = 1;
	if (key & keyboard_left)
		keyboard[1] = 1;
	if (key & keyboard_right)
		keyboard[2] = 1;
	if (key & keyboard_down)
		keyboard[3] = 1;
	if (key & keyboard_space)
		keyboard[4] = 1;

}

void iShaderToy::key(iKeyState stat, iPoint point)
{
	if (stat == iKeyStateBegan)
	{
		mouse[0] =
			mouse[2] = point.x;
		mouse[1] =
			mouse[3] = devSize.height - point.y;
	}
	else if (stat == iKeyStateMoved)
	{
		mouse[0] = point.x;
		mouse[1] = devSize.height - point.y;
	}
	else if (stat == iKeyStateEnded)
	{
		mouse[2] = -mouse[2];
		mouse[3] = -mouse[3];
	}
}

