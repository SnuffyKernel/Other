#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <glut.h>

const unsigned int L = 320; // constants for the equation
const unsigned int R = 35;
const float  PI = 3.141592653589293238;
const float  mu = 0.003;

const unsigned int delay = 20;
clock_t t = 0;

const unsigned int Y0 = 360; // coordinates are the beginning of a straight line
const int X0_1 = -640, X0_2 = 0, X0_3 = 540;

const int XG_begin1 = -950, XG_begin2 = -280, XG_begin3 = 340; // coordinates of the beginning and end of the graph
const int XG_end1 = -350, XG_end2 = 320, XG_end3 = 940;

const unsigned int F1 = 90, F2 = 60, F3 = 30; // angle in degrees
float f1 = 0, f2 = 0, f3 = 0;

float W1 = 0, W2 = 0, W3 = 0; // angular acceleration
float w1 = 0, w2 = 0, w3 = 0;

int X_current1 = 0, X_current2 = 0, X_current3 = 0;// current coordinates
int Y_current1 = 0, Y_current2 = 0, Y_current3 = 0;

int X_last1 = 0, X_last2 = 0, X_last3 = 0;// past coordinates
int Y_last1 = 0, Y_last2 = 0, Y_last3 = 0;

int XG_offset1 = -950, XG_offset2 = -280, XG_offset3 = 340; // coordinates of the graph offset

void Circle(int x, int y, int r, float n)
{
	glBegin(GL_POLYGON);
	for (float i = 0; i < n; i++)
	{
		float f = 2 * PI * i / n;
		float X = r * cos(f);
		float Y = r * sin(f);
		glVertex2f(x + X, y + Y);
	}
	glEnd();
}

void construction() {

	for (int x = -250, y = 360; x <= 2000; x += 250) {
		glColor3ub(250, 250, 250);
		glBegin(GL_LINES);
		glVertex2d(x, y);
		glVertex2d(x + 1300, 2 * y);
		glEnd();
	}

	Circle(-640, 360, 5, 2000);

	glColor3ub(250, 250, 250);
	glBegin(GL_LINES);
	glVertex2d(-250, 360);
	glVertex2d(5000, 360);
	glEnd();

	glColor3ub(250, 250, 250);
	glBegin(GL_LINES);
	glVertex2d(-1950, -460);
	glVertex2d(+1950, -460);
	glEnd();

	glColor3ub(250, 250, 250);
	glBegin(GL_LINES);
	glVertex2d(-950, -500);
	glVertex2d(-950, -200);
	glEnd();

	glColor3ub(250, 250, 250);
	glBegin(GL_LINES);
	glVertex2d(340, -500);
	glVertex2d(340, -300);
	glEnd();

	glColor3ub(250, 250, 250);
	glBegin(GL_LINES);
	glVertex2d(-280, -500);
	glVertex2d(-280, -250);
	glEnd();

}

void pendulum(const int X0, int* X_last, int* Y_last, int X_current, int Y_current, int * XG_offset, int XG_begin, int XG_end, int r, int g, int b) {

	glColor3ub(r, g, b);
	glBegin(GL_LINE_LOOP);
	glVertex2f(*XG_offset, *Y_last - 500);
	glVertex2f(*XG_offset - 1, Y_current - 500);
	glEnd();
	(*XG_offset)++;
	if (*XG_offset == XG_end) {
		*XG_offset = XG_begin;
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	glColor3ub(0, 0, 0);
	glBegin(GL_LINE_LOOP);
	glVertex2f(X0, Y0);
	glVertex2f(*X_last, *Y_last);
	glEnd();

	glColor3ub(r, g, b);
	glBegin(GL_LINE_LOOP);
	glVertex2f(X0, Y0);
	glVertex2f(X_current, Y_current);
	glEnd();

	glColor3ub(0, 0, 0);
	Circle(*X_last, *Y_last, R, 50);

	glColor3ub(r, g, b);
	Circle(X_current, Y_current, R, 50);

	*X_last = X_current; *Y_last = Y_current;

}

void render_image()
{
	construction();

	pendulum(X0_1, &X_last1, &Y_last1, X_current1, Y_current1, &XG_offset1, XG_begin1, XG_end1, 0, 0, 255);
	pendulum(X0_2, &X_last2, &Y_last2, X_current2, Y_current2, &XG_offset2, XG_begin2, XG_end2, 0, 128, 0);
	pendulum(X0_3, &X_last3, &Y_last3, X_current3, Y_current3, &XG_offset3, XG_begin3, XG_end3, 255, 0, 0);

	glFlush();
	glutSwapBuffers();
}

void changeSize()
{
	int w = 1920, h = 1024;

	if (h == 0) h = 1;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(-w / 2, w / 2, -h / 2, h / 2, -1, 1);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void Keyboard(unsigned char key, int x, int y)
{
#define ESCAPE '\033'
	if (key == ESCAPE)
		exit(0);
}



void coordinates(int* x, int* y, int X0, float* f, float* w, float* W) {
	*W = -mu * (sin(*f) + *w);
	*f += *w;
	*w += *W;
	*x = X0 + L * sin(*f);
	*y = Y0 - L * cos(*f);
}

void expectation()
{
	if (clock() - t < delay) return;
	t = clock();
	coordinates(&X_current1, &Y_current1, X0_1, &f1, &w1, &W1);
	coordinates(&X_current2, &Y_current2, X0_2, &f2, &w2, &W2);
	coordinates(&X_current3, &Y_current3, X0_3, &f3, &w3, &W3);
	glutPostRedisplay();
}

void main(int argc, char* argv[])
{

	f1 = F1 * PI / 180;
	f2 = F2 * PI / 180;
	f3 = F3 * PI / 180;


	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB);
	glutInitWindowSize(1920, 1024);
	glutCreateWindow("Mathematical Pendulum");
	glutFullScreen();


	glutDisplayFunc(render_image);
	glutReshapeFunc(changeSize);
	glutKeyboardFunc(Keyboard);
	glutIdleFunc(expectation);

	glutMainLoop();
}