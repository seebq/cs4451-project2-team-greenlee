#include <GL/glut.h>

#include "shape.h"

Shape shape;

void renderScene(void) {
	int i;

	glClear(GL_COLOR_BUFFER_BIT);
	
	for(i=0;i<numTriangles(shape);i++)
	{	
		glBegin(GL_TRIANGLES);
			glVertex3f(shape->vTable[shape->tTable[i*3]],
 				   shape->vTable[shape->tTable[i*3]+1],
				   shape->vTable[shape->tTable[i*3]+2]);
			glVertex3f(shape->vTable[shape->tTable[i*3+1]],
 				   shape->vTable[shape->tTable[i*3+1]+1],
				   shape->vTable[shape->tTable[i*3+1]+2]);
			glVertex3f(shape->vTable[shape->tTable[i*3+2]],
 				   shape->vTable[shape->tTable[i*3+2]+1],
				   shape->vTable[shape->tTable[i*3+2]+2]);
		glEnd();
	}

	glBegin(GL_TRIANGLES);
		glVertex3f(2.0,4.0,0.0);
		glVertex3f(2.1,4.1,0.1);
		glVertex3f(2.1,4.0,0.0);
	glEnd();
	glFlush();
}
	
int main(int argc,char** argv)
{
	int window;

	if(argc != 2)
		return 1;

	shape = readShapeFromFile(argv[1]);

	glutInit(&argc,argv);
	glutInitWindowPosition(-1,-1);
	glutInitWindowSize(500,500);
	glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE );

	window = glutCreateWindow("BrandonSoft Window!");
	
	glutDisplayFunc(renderScene);

	glViewport(0,0,500,500);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glShadeModel(GL_FLAT);
/*	gluLookAt(0.0,0.0,10.0,
		  0.0,0.0,0.0,
		  0.0,1.0,0.0);*/
	glutMainLoop();
}
