////////////////////////////////////////////////////////////////////////////////
// OpenGL first shader program
// Special for course "Algorithmic basics of multimedia"
//
// Copyright (C) 2011 by icosaeder
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <string.h>
#include <GL/glut.h>

#define TIME_PRECISION_FACTOR 0

// Maximal size of shader source in bytes
#define MAX_SHADER_SIZE 10240 // 10 Kb

GLuint g_shaderProgram;
int g_useShaders = 0;

#ifdef _WIN32
// In Windows there is no native support of OpenGL 2+ functions, so we have to
// import them manually

PFNGLACTIVETEXTUREARBPROC glActiveTextureARB;

// FrameBuffer (FBO) gen, bin and texturebind
PFNGLGENFRAMEBUFFERSEXTPROC glGenFramebuffersEXT ;
PFNGLBINDFRAMEBUFFEREXTPROC glBindFramebufferEXT ;
PFNGLFRAMEBUFFERTEXTURE2DEXTPROC glFramebufferTexture2DEXT ;
PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC glCheckFramebufferStatusEXT ;


// Shader functions
PFNGLCREATEPROGRAMOBJECTARBPROC  glCreateProgramObjectARB ;
PFNGLUSEPROGRAMOBJECTARBPROC     glUseProgramObjectARB    ;
PFNGLCREATESHADEROBJECTARBPROC   glCreateShaderObjectARB  ;
PFNGLSHADERSOURCEARBPROC         glShaderSourceARB        ;
PFNGLCOMPILESHADERARBPROC        glCompileShaderARB       ;
PFNGLGETOBJECTPARAMETERIVARBPROC glGetObjectParameterivARB;
PFNGLATTACHOBJECTARBPROC         glAttachObjectARB        ;
PFNGLLINKPROGRAMARBPROC          glLinkProgramARB         ;
PFNGLGETUNIFORMLOCATIONARBPROC   glGetUniformLocationARB  ;
PFNGLUNIFORM1IARBPROC            glUniform1iARB           ;
PFNGLACTIVETEXTUREARBPROC        glActiveTextureARB       ;
PFNGLGETINFOLOGARBPROC           glGetInfoLogARB          ;

void getOpenGLFunctionPointers(void)
{
	// FBO
	glActiveTextureARB = (PFNGLACTIVETEXTUREARBPROC)wglGetProcAddress("glActiveTextureARB");
	glGenFramebuffersEXT		= (PFNGLGENFRAMEBUFFERSEXTPROC)		wglGetProcAddress("glGenFramebuffersEXT");
	glBindFramebufferEXT		= (PFNGLBINDFRAMEBUFFEREXTPROC)		wglGetProcAddress("glBindFramebufferEXT");
	glFramebufferTexture2DEXT	= (PFNGLFRAMEBUFFERTEXTURE2DEXTPROC)wglGetProcAddress("glFramebufferTexture2DEXT");
	glCheckFramebufferStatusEXT	= (PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC)wglGetProcAddress("glCheckFramebufferStatusEXT");
	
	// Shaders
	glCreateProgramObjectARB = (PFNGLCREATEPROGRAMOBJECTARBPROC)wglGetProcAddress("glCreateProgramObjectARB");
	glUseProgramObjectARB = (PFNGLUSEPROGRAMOBJECTARBPROC)wglGetProcAddress("glUseProgramObjectARB");
	glCreateShaderObjectARB = (PFNGLCREATESHADEROBJECTARBPROC)wglGetProcAddress("glCreateShaderObjectARB");
	glShaderSourceARB = (PFNGLSHADERSOURCEARBPROC)wglGetProcAddress("glShaderSourceARB"); 
	glCompileShaderARB = (PFNGLCOMPILESHADERARBPROC)wglGetProcAddress("glCompileShaderARB"); 
	glGetObjectParameterivARB = (PFNGLGETOBJECTPARAMETERIVARBPROC)wglGetProcAddress("glGetObjectParameterivARB"); 
	glAttachObjectARB = (PFNGLATTACHOBJECTARBPROC)wglGetProcAddress("glAttachObjectARB"); 
	glGetInfoLogARB = (PFNGLGETINFOLOGARBPROC)wglGetProcAddress("glGetInfoLogARB");
	glLinkProgramARB = (PFNGLLINKPROGRAMARBPROC)wglGetProcAddress("glLinkProgramARB");
	glGetUniformLocationARB = (PFNGLGETUNIFORMLOCATIONARBPROC)wglGetProcAddress("glGetUniformLocationARB");
	glUniform1iARB = (PFNGLUNIFORM1IARBPROC)wglGetProcAddress("glUniform1iARB");
}
#endif // _WIN32

void draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Don't use shaders -- use fixed pipeline instead
    glUseProgramObjectARB(0);
    
    glDisable(GL_LIGHTING);
    
    glBegin(GL_POINTS);
        glVertex3d(0, 0, -1);
    glEnd();
    
    glEnable(GL_LIGHTING);
    
    // Use shader program if needed
    if (g_useShaders)
        glUseProgramObjectARB(g_shaderProgram);
    
    float pos[] = {0, 0, -1, 1};
    glLightfv(GL_LIGHT0, GL_POSITION, pos);
    
    glPushMatrix();
    glLoadIdentity();
    glTranslated(0, 0, -3);
    glRotated(45, 1, 1, 0);
    
    glutSolidTeapot(0.7);
        
    glPopMatrix();
    
    glutSwapBuffers();
}

void reshape(int width, int height)
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, 1, 1, 10);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslated(0, 0, -3);
}

void animate()
{
    static int startTime = 0;
    static int loops = 0;
    static int animationEnabled = 0;
    static float delta = 1;
    int currentTime;
    
    if (!startTime)
        startTime = glutGet(GLUT_ELAPSED_TIME);
    
    if (loops++ > TIME_PRECISION_FACTOR)
    {
        animationEnabled = 1;
        currentTime = glutGet(GLUT_ELAPSED_TIME);
        delta = (currentTime - startTime) / (loops * 1000.0);
        startTime = currentTime;
        loops = 0;
    }
    
    if (animationEnabled)
        glRotated(90.0 * delta, 1, 1, 0);
    
    glutPostRedisplay();
}

void keyPressed(unsigned char key, int x, int y)
{
    if (key == ' ')
    {
        // Toggle shaders
        g_useShaders = !g_useShaders;
    }
}

void setUp()
{
    glEnable(GL_DEPTH_TEST);
    
    glClearColor(0, 0, 0, 1);
    
    glColor3ub(255, 0, 0);
	glPointSize(20);
    
    float ambLit[] = {0.8, 0, 0, 1};
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambLit);
	
	glShadeModel(GL_SMOOTH);
    
    float amb[] = {0, 0, 0, 1};
    float dif[] = {1, 1, 1, 1};
    float spec[] = {1, 1, 1, 1};
    float pos[] = {1, 1, 1, 1};
    glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, dif);
	glLightfv(GL_LIGHT0, GL_SPECULAR, spec);
	glLightfv(GL_LIGHT0, GL_POSITION, pos);
    
    glEnable(GL_LIGHT0);
    
    float ambM[] = {0.247, 0.199, 0.07, 1};
    float difM[] = {0.75, 0.606, 0.226, 1};
    float specM[] = {0.628, 0.555, 0.366, 1};
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambM);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, difM);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specM);
}

GLhandleARB loadShader(char* fileName, unsigned int type)
{
    FILE *pFile;
	GLhandleARB handle;
	const GLcharARB *files[1];
	
	// Variables needed for shader compilation
	GLint result;
	GLint errorLoglength;
	char* errorLogText;
	GLsizei actualErrorLogLength;
	
	char buffer[MAX_SHADER_SIZE];
	memset(buffer, 0, MAX_SHADER_SIZE);
	
	pFile = fopen(fileName, "rb");
	if (!pFile)
	{
	    printf("Cannot open file <%s>\n", fileName);
	    exit(-1);
	}
	
	fread(buffer, 1, MAX_SHADER_SIZE, pFile);
	
	fclose(pFile);
	
	// Create shader object of given type (vertex or fragment)
	handle = glCreateShaderObjectARB(type);
	if (!handle)
	{
		printf("Failed creating vertex shader object from file <%s>\n", 
		       fileName);
		exit(-1);
	}
	
	files[0] = (const GLcharARB*)buffer;
	
	glShaderSourceARB(handle, // The handle of shader
					  1,      // The number of files to proceed
					  files,  // The array of const char * data, 
					          // which represents the source code of the shaders
					  NULL    // Lengths of files, 
					          // if NULL -- strings supposed to be \0 terminated
					 );
					 
    // Compile shader
    glCompileShaderARB(handle);
	
	// Obtain compilation result
	glGetObjectParameterivARB(handle, GL_OBJECT_COMPILE_STATUS_ARB, &result);
	
	// If any errors
	if (!result)
	{
		// Compilation failed
		printf("Shader <%s> failed compilation\n", fileName);
		
		// Get the length of error log
		glGetObjectParameterivARB(handle, GL_OBJECT_INFO_LOG_LENGTH_ARB, 
		                          &errorLoglength);
		
		// Create a buffer to read compilation error message
		errorLogText = malloc(errorLoglength);
		
		// Used to get the final length of the log
		glGetInfoLogARB(handle, errorLoglength, &actualErrorLogLength, 
		                errorLogText);
		
		// Display errors
		printf("%s\n", errorLogText);
		
		// Free the buffer
		free(errorLogText);
	}
	
	return handle;
}

void loadShaders()
{
    // Load shaders from files
    GLhandleARB vertexShaderHandle = loadShader("vertexShader.glsl",
                                                GL_VERTEX_SHADER);
	GLhandleARB fragmentShaderHandle = loadShader("fragmentShader.glsl",
	                                              GL_FRAGMENT_SHADER);
	
	// Create shader program object
	g_shaderProgram = glCreateProgramObjectARB();
	
	// Attach shaders to program
	glAttachObjectARB(g_shaderProgram, vertexShaderHandle);
	glAttachObjectARB(g_shaderProgram, fragmentShaderHandle);
	
	// Link program to use shaders together
	glLinkProgramARB(g_shaderProgram);
}

int main(int argc, char **argv)
{
    glutInit(&argc,argv);
    glutInitWindowSize(480, 480);
    glutInitWindowPosition(100, 100);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow("Test of shaders");
    glutDisplayFunc(draw);
    glutReshapeFunc(reshape);
    glutIdleFunc(animate);
    glutKeyboardFunc(keyPressed);
    
#ifdef _WIN32
	getOpenGLFunctionPointers();
#endif // _WIN32
    
    setUp();
    loadShaders();
    
    glutMainLoop();
    
    return 0;
}

