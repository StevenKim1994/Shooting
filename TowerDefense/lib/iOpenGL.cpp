#include "iOpenGL.h"

HGLRC hRC;
void setupOpenGL(bool setup, HDC hDC) // 이부분은 플랫폼마다 다름 현재는 Windows 기준임.
{
	if(setup)
	{
		 // initiailize...
		PIXELFORMATDESCRIPTOR pfd;
		memset(&pfd, 0x00, sizeof(PIXELFORMATDESCRIPTOR));

		pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
		pfd.nVersion = 1;
		pfd.iPixelType = PFD_TYPE_RGBA;
		pfd.cColorBits = 32;
		pfd.cDepthBits = 32;
		pfd.iLayerType = PFD_MAIN_PLANE;
		pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
		
		 
		int pixelFormat = ChoosePixelFormat(hDC, &pfd);
		SetPixelFormat(hDC, pixelFormat, &pfd);
		
		hRC = wglCreateContext(hDC);
		wglMakeCurrent(hDC, hRC);

	}

	else  
	{
		 // 프로그램이 종료되었을때 destroy...

		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(hRC);
	}

	
}

bool startGLEW()
{
	glewExperimental = true;
	
	GLenum error = glewInit();

	if (error != GLEW_OK)
		return false;


 
	return true;
}

void initOpenGL()
{
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	//glTexEnvi();
	
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

#if 0
	glDisable(GL_DEPTH_TEST);
#else
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_ALWAYS);
	
	glClearDepth(1.0f);
#endif
	
	//base rgb + img rgb
	//base- -rgb * (1-a) +img-rgb * a
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	//base-rg +_ img-rgb * a
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glEnable(GL_SMOOTH);
	
}
#include "iStd.h"
extern int monitorSizeW, monitorSizeH;

void checkOpenGL()
{

	//GL, GLEW, SHADER 버전 확인...
	const GLubyte* strGL = glGetString(GL_VERSION);
	const GLubyte* strGLEW = glewGetString(GLEW_VERSION);
	const GLubyte* strGLSL = glGetString(GL_SHADING_LANGUAGE_VERSION);
	printf("strGL: %s strGLEW: %s strGLSL: %s\n", strGL, strGLEW, strGLSL);



	// matrix 확인 방법.. OpenGL 1.x에서만...
	GLfloat matrix[2][16];
	glGetFloatv(GL_PROJECTION_MATRIX, matrix[0]);
	glGetFloatv(GL_MODELVIEW_MATRIX, matrix[1]);

	for(int i = 0; i < 2; i++)
	{
		for(int j = 0; j<4; j++)
		{
			GLfloat* m = &matrix[i][4*j];
			for (int k = 0; k < 4; k++)
				printf("%f\t", m[k]);

			printf("\n");
		}

		
	}

	//현재 디바이스가 지원하는 Texture의 최대 크기
	GLint maxTextureSize;
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTextureSize);
	printf("maxTextureSize : %d\n", maxTextureSize);


	// OpenGL RunTime중 에러 확인방법 glew 사용시...
	GLenum error = glGetError();
	const GLubyte* strerror = glewGetErrorString(error);
	printf("%s\n", strerror);


	// 이미지 텍스처 불러온것 정보 확인 확인하기전에 바인딩된 텍스처가 있어야됨(OpenGL에서 텍스처를 사용하기 위해서는 바인딩을 해야 사용할 수있음)
	static Texture* tex = createImage("assets/ex.png");
	glBindTexture(GL_TEXTURE_2D, tex->texID);
	GLint w, h, format;
	//level : 블로어 비슷하게 쓸수 있는 개념이라고만 생각하면 될듯?
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &w); // 가로크기 확인
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &h); // 세로크기 확인
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_INTERNAL_FORMAT, &format); // GL_RGBA인지 어떤 형식인지...
	GLint texID;
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &texID);
	printf("%d\n", texID);
	glBindTexture(GL_TEXTURE_2D, 0);

	// 현재사용하는 fbo 확인 ... 우리는 일단 fbo를 사용하지 않았으므로 main fbo만 나옴.
	GLint fbo;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &fbo);
	printf("%d\n", fbo);

}

void reshapeOpenGL(int width, int height)
{

	
	float r0 = devSize.width / devSize.height;

	monitorSizeW = width;
	monitorSizeH = height;
	float r1 = 1.0f * width / height;

	if (r0 < r1)// 모니터 가로가 큰 경우
	{
		viewport.origin.y = 0; viewport.size.height = height;
		viewport.size.width = devSize.width * height / devSize.height;
		viewport.origin.x = (width - viewport.size.width) / 2;
	}
	else if (r0 > r1)// 모니터 가로가 작은 경우(세로 긴 경우)
	{
		viewport.origin.x = 0; viewport.size.width = width;
		viewport.size.height = devSize.height * width / devSize.width;
		viewport.origin.y = (height - viewport.size.height) / 2;
	}
	else// 정비율
	{
		viewport = iRectMake(0, 0, width, height);
	}
	glViewport(viewport.origin.x, viewport.origin.y, viewport.size.width, viewport.size.height);

	printf("--------------\n");
	checkOpenGL();

	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, devSize.width, devSize.height, 0, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	
	printf("==============\n");
	checkOpenGL();
	
}


GLuint nextPot(GLuint x)
{
	x = x - 1;
	x = x | (x >> 1);
	x = x | (x >> 2);
	x = x | (x >> 4);
	x = x | (x >> 8);
	x = x | (x >> 16);

	return x + 1;
}

struct xTexParam
{
	GLuint minFilter;
	GLuint magFilter;
	GLuint wrapS;
	GLuint wrapT;
};

static xTexParam texParam = { GL_NEAREST,GL_NEAREST,GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE };

void applyTexParameters()
{
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, texParam.minFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,	texParam.magFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, texParam.wrapS);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, texParam.wrapT);
}

void setAntiAliasParameters(bool anti)
{
	if(anti == false)
	{
		texParam.minFilter = GL_NEAREST;
		texParam.magFilter = GL_NEAREST;
	}
	else
	{
		texParam.minFilter = GL_LINEAR;
		texParam.magFilter = GL_LINEAR;
	}
}

Texture* createImageWithRGBA(GLubyte* rgba, GLuint width, GLuint height)
{
	GLuint texID;
	glGenTextures(1, &texID); // generate(create) Texture;
	//glActiveTexture(GL_TEXTURE0); // 첫번째 텍스처에 texID를 쓰겠다라는 의미. 이걸 사용하지 않아도 첫번째 텍스처는 0번임.

	glBindTexture(GL_TEXTURE_2D, texID);

	applyTexParameters();
	
	int potWidth = nextPot(width), potHeight = nextPot(height);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, potWidth, potHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, rgba);

	Texture* tex = (Texture*)malloc(sizeof(Texture));
	tex->texID = texID;
	tex->width = width;
	tex->height = height;
	tex->potWidth = potWidth;
	tex->potHeight = potHeight;
	tex->retainCount = 1;
	return tex;
}
