#pragma once

// 순서는 지켜야함!.

//glew.c 파일을 프로젝트에 추가시킴.
//GLEW_STATIC을 전처리기에 정의함.
// gl, glu를 포함하고 있으므로 glew를 먼저 등록시켜야함
#include <gl/glew.h>
//#include <GL/GL.h>
//#include <GL/GLU.h>


#pragma comment(lib,"opengl32.lib") // obj 파일을 찾기위한 lib 등록 obj 파일들이 이 lib에 모여있음. 물론 프로젝트 설정에서 포함시켜도됨.
#include <windows.h>

void setupOpenGL(bool setup, HDC hDC);
bool startGLEW();
void initOpenGL();
void reshapeOpenGL(int width, int height);

GLuint nextPot(GLuint x);

struct Texture; 
Texture* createImageWithRGBA(GLubyte* rgba, GLuint width, GLuint height);

void setAntiAliasParameters(bool anti);

#define OPENGL 1
/* 
  새로운 라이브러리 접근 방법
  
  1. compile : header file include (*.obj, *.o 생성위함)
  2. link : .lib include (archive *.obj (합친다) lib or exe 파일 생성)

 
*/