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

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//glMatrixMode(GL_PROJECTION); 
	//glOrthof(0, devSize.width, devSize.height, 0, 0, 100);
	//glMatrixMode(GL_MODELVIEW); // 카메라
	//
	//glLoadIdentity();
	
}
#include "iStd.h"
extern int monitorSizeW, monitorSizeH;

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



}
