// OpenGL GLUT, Pendulum Waves Visualized
// Inspired by: https://www.youtube.com/watch?v=yVkdfJ9PkRQ&ab_channel=HarvardNaturalSciencesLectureDemonstrations
// Coded by: Rita DOUMET, Christian RECHDAN for Computer Graphics project
// Coded on: 30/06/2022
// 
// Note: the physics behind the pendulum is accurate and backed up by Oscillation of a Simple Pendulum formulas,
// as well as formulas for the Law of Conservation of Momentum.
// 
// Feel free to edit some of the constants defined below (such as: L, thetaMax, R, N, x0, speed, etc.) to see what happens.


#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include<iostream>
#include <math.h>

#define PI 3.14159265359
#define screenWidth 800.0
#define screenHeight 700.0
#define thetaMax 30.0*PI/180.0 // Maximum angle (radians)
#define R 15.0 // Radius of circles
#define N 15 // Number of pendulums
#define x0 screenWidth/2 // X dimension of the center of the first circle (from left to right)
#define speed 3.0 //speed of the animation

using namespace std;

struct Pendulum {
	GLint centerX, centerY, L;//center of the circle
	GLfloat theta, omega;
};

Pendulum pendulums[N]; //contains all the pendulums to be displayed

GLfloat t = 0.0;

void initializePendulums() {
	
	for (int i = 0; i < N; i++) {
		/*The WHOLE TRICK is HERE!!
		According to the source of the video mentioned above:

		" The length of the longest pendulum has been adjusted so that it executes 51 oscillations in 60 seconds, 
		and the length of each successive shorter pendulum is carefully adjusted so that it executes one additional oscillation in this period.
		Thus, the 15th pendulum (shortest) undergoes 65 oscillations in 60 seconds. "
		
		Below, we calculated the length of each pendulum so that it has exactly one more oscillation than its previous one.
		L = g * ( T / (2 * nbOsc * PI))^2
		We multiplied the end result by a factor of 1500 to make it appear on screen
		*/
		pendulums[i].L = 9.8 * pow(60 / (2 * (51 + i) * PI), 2) * 1500;

		pendulums[i].omega = sqrt(static_cast<GLfloat>(9.8) / pendulums[i].L);
		pendulums[i].theta = thetaMax;
		pendulums[i].centerX = x0;
		pendulums[i].centerY = screenHeight - pendulums[i].L;
	}
}
void init(void) {
	glClearColor(0.1, 0.1, 0.1, 1.0); //set display-window color to white
	glMatrixMode(GL_PROJECTION); //set projection parameters
	gluOrtho2D(0.0, screenWidth, 0.0, screenHeight);
	initializePendulums(); //initialize the coordinates of the centers of the pendulums
}

void displayText() {
	string text = "Coded by Rita DOUMET & Christian RECHDAN, 2022.";
	glRasterPos2i(50, 50);
	for (int i = 0; i < text.size(); i++)
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, text[i]);
}

void display(void) {
	glClear(GL_COLOR_BUFFER_BIT); //clear display window
	displayText();
	glBegin(GL_LINES);
	for (int i = 0; i < N; i++) {
		glColor3f(0.3, 0.3, 0.3);
		glVertex2i(pendulums[i].centerX, pendulums[i].centerY);
		glColor3f(1.0, 1.0, 1.0); //set line segment color to white
		glVertex2i(x0, screenHeight );
	}
	glEnd();

	for (int i = 0; i < N; i++) {
		GLfloat t = 0.0; //t is the current angle we are drawing, in radians
		GLfloat c = 0.0; //color
		GLint min = pendulums[i].theta * 180 / PI +90;  // effect: gradient
		GLint max = min + 360;
		//min and max are calculated for the sake of an accurate gradient color on the moving object

		glBegin(GL_POLYGON);
		for (int j = min; j < max; j++) {
			t = j * PI / 180;
			c = 1.0 * j / 360;
			glColor3f(c, c, c);
			glVertex2i(cos(t) * R + pendulums[i].centerX, sin(t) * R + pendulums[i].centerY);
			//formula: calculating the x and y coordinates of the next vertex to add to the polygon
			//and translating the origin to the center of the circle
		}
		glEnd();
	}

	glutSwapBuffers(); //swap from front to back buffer
}
void updateSpecials() {
	for (int i = 0; i < N; i++) {
		pendulums[i].centerX = x0 + pendulums[i].L * sin(pendulums[i].theta);
		pendulums[i].centerY = screenHeight - pendulums[i].L * cos(pendulums[i].theta);
	}

}
void changeTheta() {
	for (int i = 0; i < N; i++) {
		pendulums[i].theta  = thetaMax * cos(pendulums[i].omega * t * speed);
	}
	updateSpecials();
}
void timer(int) {
	glutPostRedisplay();
	changeTheta();
	glutTimerFunc(1000 / 60, timer, 0); //60fps
	t += (static_cast<float>(60) / 1000); //updating the time passed, to use it later in the theta formula
}
void reshape(int w, int h) {
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, screenWidth, 0, screenHeight);
	glMatrixMode(GL_MODELVIEW);
}
void main(int argc, char** argv) {
	glutInit(&argc, argv); //initialize GLUT
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB); //set display mode; DOUBLE for animation
	glutInitWindowPosition(150, 50); // set top-left display-window position
	glutInitWindowSize(screenWidth, screenHeight); //set display-window width and height
	glutCreateWindow("Pendulum Waves"); //create display window

	init(); //execute initialization procedure

	glutDisplayFunc(display); //send graphics to display window
	glutReshapeFunc(reshape);
	glutTimerFunc(0, timer, 0);
	glutMainLoop(); //display everything and wait
}