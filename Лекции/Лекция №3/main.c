////////////////////////////////////////////////////////////////////////////////
// OpenGL first lighting program
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

#include "GL/glut.h"

#define TIME_PRECISION_FACTOR 0

void draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Disable lighting to draw non-lit geometry
    glDisable(GL_LIGHTING);
    
    // Draw point indicating light source
    glBegin(GL_POINTS);
        glVertex3d(0, 0, -1);
    glEnd();
    
    // Enable lighting to draw lit geometry
    glEnable(GL_LIGHTING);
    
    // Set light position -- light is placed in space as it was a point
    // It's position is transformed by the MODELVIEW matrix in the same way as 
    // point's position
    float pos[] = {0, 0, -1, 1};
    glLightfv(GL_LIGHT0, GL_POSITION, pos);
    
    // The object uses another coordinate system -- it's not animated and has 
    // constant rotation, so we push MODELVIEW matrix and change it than
    glPushMatrix();
    glLoadIdentity();
    glTranslated(0, 0, -3);
    glRotated(45, 1, 1, 0);
    
    // Draw object (teapot of size 0.7)
    glutSolidTeapot(0.7);
        
    // Pop matrix -- we don't need object's coordinate system anymore
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
    static int smooth = 0;
    if (key == ' ')
    {
        smooth = !smooth;
        // Change shade model
        if (smooth)
            glShadeModel(GL_SMOOTH);
        else
            glShadeModel(GL_FLAT);
    }
}

void setUp()
{
    glEnable(GL_DEPTH_TEST);
    
    glClearColor(0, 0, 0, 1);
    
    // Setup appearance of non-lit points, drawn with glBegin(GL_POINTS)
    glColor3ub(255, 0, 0);
	glPointSize(20);
    
    // Setup lighting model
    float ambLit[] = {0.8, 0, 0, 1};
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambLit);
	
	// Setup initial shade model
	glShadeModel(GL_FLAT);
    
    // Setup light source #0 by defining ambient, diffuse and specular color
    float amb[] = {0, 0, 0, 1};
    float dif[] = {1, 1, 1, 1};
    float spec[] = {1, 1, 1, 1};
    float pos[] = {1, 1, 1, 1};
    glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, dif);
	glLightfv(GL_LIGHT0, GL_SPECULAR, spec);
	glLightfv(GL_LIGHT0, GL_POSITION, pos);
    
    // Activate light source #0
    glEnable(GL_LIGHT0);
    
    // Setup material by defining ambient, diffuse and specular color
    float ambM[] = {0.247, 0.199, 0.07, 1};
    float difM[] = {0.75, 0.606, 0.226, 1};
    float specM[] = {0.628, 0.555, 0.366, 1};
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambM);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, difM);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specM);
}

int main(int argc, char **argv)
{
    glutInit(&argc,argv);
    glutInitWindowSize(480, 480);
    glutInitWindowPosition(100, 100);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow("Test of lighting");
    glutDisplayFunc(draw);
    glutReshapeFunc(reshape);
    glutIdleFunc(animate);
    glutKeyboardFunc(keyPressed);
    
    setUp();
    
    glutMainLoop();
    
    return 0;
}

