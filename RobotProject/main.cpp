/*
Control: 
Move: Arrow Button (Up, down, Left, Right)
			  +----------+		
Fly On/Off	: | Spacebar |
			  +----------++
Reset		: | Backspace |
			  +---+---+---+	
Turn Body	: | g | h |
			  +---+-+-+		
Quit program: | ESC |
			  +-----+

		Left Arm							  Right Arm
		   +---+---+---+---+				   +---+---+---+---+
Upper Arm: | e | d | s | f |		Upper Arm: | i | k | j | l |
		   +---+---+---+---+		 		   +---+---+---+---+ 
Lower Arm: | r | w | a | q |		Lower Arm: | u | o | ; | p |
		   +---+---+---+---+				   +---+---+---+---+	 
Grab	 : | v | 					Grab	 : | n |
		   +---+							   +---+
*/
#include <GL/glut.h>

GLfloat coordinateX = 0.0, coordinateY = -2.5, coordinateZ = -1.0, turn = 0.0, flyPosition = 0.0, kepitL = 45.0, kepitR = 45.0;
static int on = 0, floating = 1, shoulderR = 0, shoulderL = 0, shoulderRy = 0, shoulderLy = 0, elbowR = 0, elbowL = 0, elbowRy = 0, elbowLy = 0;
GLint fan = 0;
GLUquadricObj *quadratic;
GLUquadricObj *disk;

void init(void)
{
	glEnable (GL_DEPTH_TEST);
    glEnable (GL_LIGHTING);
    glEnable (GL_LIGHT0);
    glEnable (GL_LIGHT1);
    glEnable (GL_LIGHT2);
    glEnable (GL_LIGHT3);
	glEnable(GL_COLOR_MATERIAL);
	glShadeModel (GL_SMOOTH);
	glClearColor(0.0, 0.0, 0.0, 0.0); //background colour

	quadratic = gluNewQuadric();
    disk = gluNewQuadric();
}

void fly()
{
	if (coordinateY < flyPosition + 0.3 && floating == 1)
		coordinateY = coordinateY + 0.001;
	else floating = 0;

	if (coordinateY > flyPosition - 0.3 && floating == 0)
		coordinateY = coordinateY - 0.001;
	else floating = 1;
}

void fall(){
	if (coordinateY > -2.5){
		coordinateY = coordinateY - 0.005;
		floating = 1;
	}
	flyPosition = 0.0; //reset flyPosition value
}

// -- robot's part --
void cuboid(GLfloat length, GLfloat height, GLfloat width, GLfloat red, GLfloat green, GLfloat blue){
	glColor3f(red, green, blue); //Color
	glScalef (length, height, width); //change cube -> cuboid
	glutSolidCube(1.0);
	
	//transform back to original
	glScalef (1/length, 1/height, 1/width);
}
void sphere(GLdouble radius, GLfloat red, GLfloat green, GLfloat blue){
	glColor3f(red, green, blue); //Color
	glutSolidSphere(radius, 20.0, 20.0);
}
void cylinder(GLdouble radius, GLdouble length){
	GLint slices = 20, stacks = 20;

	glColor3f(0.8, 0.8, 0.8); //color
	glTranslatef (0.0, 0.0, -length/2);
	gluCylinder(quadratic, radius, radius, length, slices, stacks); 
	glTranslatef (0.0, 0.0, length);
	gluDisk(disk, 0, radius, slices, stacks);
	glTranslatef (0.0, 0.0, -length);	glRotatef(180, 0.0, 1.0, 0.0);
	gluDisk(disk, 0, radius, slices, stacks);

	//transform back to original
	glRotatef(-180, 0.0, 1.0, 0.0);
	glTranslatef (0.0, 0.0, length/2);	
}
void eye(GLdouble radius){
	GLdouble length = 1.0;
	GLint slices = 20, stacks = 20;

	glColor3f(0.8, 0.8, 0.3); //Color
	gluCylinder(quadratic, radius, radius, length, slices, stacks); // (GLUquadric* quad, GLdouble base, GLdouble top, GLdouble height, GLint slices, GLint stacks)
	gluCylinder(quadratic, radius-0.05, radius-0.05, length, slices, stacks);
	
	glTranslatef (0.0, 0.0, length);
	gluDisk(disk, radius-0.05, radius, slices, stacks); //(GLUquadric* quad, GLdouble inner, GLint slices, GLint loops)

	glColor3f(0, 0, 0); //Color
	glTranslatef (0.0, 0.0, -0.03);
	gluDisk(disk, 0, radius-0.05, slices, stacks);

	//transform back to original
	glTranslatef (0.0, 0.0, 0.03);
	glTranslatef (0.0, 0.0, -length);
}
void finger(float kepit){
	glRotatef(kepit, 0, 0, 1);
	cylinder(0.1, 0.3); //(radius, length)
	glTranslatef (0.2, 0.0, 0.0);
	cuboid (0.4, 0.2, 0.2, 0.1, 0.1, 0.1);
	
	glTranslatef (0.2, 0.0, 0.0);
	glRotatef(-45, 0, 0, 1);
	
	glTranslatef (0.15, 0.0, 0.0);
	cuboid (0.4, 0.15, 0.15, 1, 1, 0);

	//transform back to original
	glTranslatef (-0.15, 0.0, 0.0); 
	glRotatef(45, 0, 0, 1);
	glTranslatef (-0.4, 0.0, 0.0); 
	glRotatef(-kepit, 0, 0, 1);
}
void hand(float kepit){
	cuboid (0.24, 0.6, 0.24, 0.1, 0.1, 0.1);
	glTranslatef (0.0, 0.2, 0.0);
	finger(kepit);
	
	glTranslatef (0.0, -0.4, 0.0);
	glRotatef(180, 1, 0, 0); 
	finger(kepit);
	
	//transform back to original
	glRotatef(-180, 1, 0, 0);
}
// -- end of robot's part ----------------

void display(void)
{
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity(); 
	
	gluLookAt(0.0, 0.0, 7.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	
	// -- light --
	GLfloat DiffuseLight[] = {0.3, 0.3, 0.3}; //set color for GL_LIGHT2, GL_LIGHT3
	glLightfv (GL_LIGHT2, GL_DIFFUSE, DiffuseLight);
	glLightfv (GL_LIGHT3, GL_DIFFUSE, DiffuseLight);
	
	GLfloat AmbientLight[] = {0.0, 0.0, 0.0}; //AmbientLight[] = {r, g, b}
	glLightfv (GL_LIGHT1, GL_AMBIENT, AmbientLight);
	
	/*
	GLfloat LightPosition[] = {0, 0.0, 1.0, 0.0}; //Position of light
	glLightfv(GL_LIGHT0, GL_POSITION, LightPosition);
	*/
	
	GLfloat LightPosition2[] = {5.0, 10.0, 0.0, 0.0}; //set light position - above-right
	GLfloat LightPosition3[] = {-5.0, 10.0, 0.0, 0.0}; //set light position - above-left
	glLightfv(GL_LIGHT2, GL_POSITION, LightPosition2);
	glLightfv(GL_LIGHT3, GL_POSITION, LightPosition3);
	//============================== Robot =================================
	glPushMatrix();
	
	glTranslatef ((GLfloat) coordinateX, (GLfloat) coordinateY,  (GLfloat) coordinateZ); //Move robot up, down, left, right
	glRotatef ((GLfloat) turn, 0.0, 1.0, 0.0); //Turn robot left, right

	//---- Body ----
	sphere(1.0, 0.3, 0.3, 1.0);

	//---- Eyes ----
	glPushMatrix(); //---- Left Eye ----
		glTranslatef (-0.35, 0.3, 0.0);
		eye(0.22); //---- Right Eye ----
		glTranslatef (0.7, 0.0, 0.0);
		eye(0.22);
	glPopMatrix(); 
	//---- End Eyes ----
	
	glPushMatrix(); //---- Mouth ----
		glTranslatef (0.0, -0.2, 0.95);
		glScalef(1.0, 0.5, 1.0);
		glColor3f(1.0, 1.0, 0.0); //Color
		glutSolidSphere(0.5, 4.0, 2.0);
	glPopMatrix(); 

	glPushMatrix(); //---- Kipas ----
		glTranslatef (0.0, 1.1, 0.0); //Move fan upward
		glRotatef ((GLfloat) fan, 0.0, 1.0, 0.0); //Rotate fan
		
		//blades
		cuboid(1.8, 0.05, 0.30, 0.8, 0.8, 0.0); //(length, height, width, red, green, blue)
		cuboid(0.30, 0.05, 1.8, 0.8, 0.8, 0.0); //(length, height, width, red, green, blue)

		//fan's center
		glRotatef(90, 1.0, 0.0, 0.0);
		cylinder(0.1, 0.2);
		
	glPopMatrix(); //---- End Kipas ----

	//-------------- Right Arm --------------
	glPushMatrix();	//---- Upper Arm ----
		glTranslatef (1.1, 0.0, 0.0); //move shoulder to right
		glRotatef ((GLfloat) shoulderRy, 0.0, 1.0, 0.0); // foward/backward shoulder & elbow

		glPushMatrix();	
			glRotatef ((GLfloat) shoulderR, 0.0, 0.0, 1.0); // lift/lower shoulder & elbow
			sphere(0.3, 0.8, 0.8, 0.8); // shoulder - (radius, red, green,blue)
			
			glTranslatef (0.6, 0.0, 0.0); //move bicep to right
			cuboid(1.2, 0.25, 0.25, 0.1, 0.1, 0.1); //bicep - (length, height, width, red, green, blue)
			
			glPushMatrix();	//---- Lower Arm ----
				glTranslatef (0.6, 0.0, 0.0); //move elbow to right
				glRotatef ((GLfloat) elbowRy, 0.0, 1.0, 0.0); // foward/backward
				
				glPushMatrix();	
					glRotatef ((GLfloat) elbowR, 0.0, 0.0, 1.0); // lift/lower elbow
					sphere(0.2, 0.8, 0.8, 0.8); //elbow - (radius, red, green,blue)

					glTranslatef (0.6, 0.0, 0.0); //move forearm to right
					cuboid(1.25, 0.24, 0.24, 0.1, 0.1, 0.1); //forearm - (length, height, width, red, green, blue)
					
					glPushMatrix(); //---- Hand ----
						glTranslatef (0.6, 0.0, 0.0);
						hand(kepitR);
					glPopMatrix(); //---- End Hand ----

				glPopMatrix();

			glPopMatrix();	//-- End Lower Arm --
		glPopMatrix();
	glPopMatrix();	//--- End Upper Arm ---
	//-------------- End Right Arm --------------
	
	//--------------- Left Arm --------------
	glPushMatrix();	//---- Upper Arm ----
		glTranslatef (-1.1, 0.0, 0.0); //move shoulder to left
		glRotatef ((GLfloat) shoulderLy, 0.0, 1.0, 0.0); // foward/backward shoulder & elbow

		glPushMatrix();	
			glRotatef ((GLfloat) shoulderL, 0.0, 0.0, 1.0);	// lift/lower shoulder & elbow
			sphere(0.3, 0.8, 0.8, 0.8); //shoulder - (radius, red, green,blue)
	
			glTranslatef (-0.6, 0.0, 0.0); //move bicep to left
			cuboid(1.2, 0.25, 0.25, 0.1, 0.1, 0.1); //bicep - (length, height, width, red, green, blue)
			
			glPushMatrix();	//---- Lower Arm ----
				glTranslatef (-0.6, 0.0, 0.0); //move elbow to left
				glRotatef ((GLfloat) elbowLy, 0.0, 1.0, 0.0); // foward/backward elbow

				glPushMatrix();	
					glRotatef ((GLfloat) elbowL, 0.0, 0.0, 1.0); // lift/lower elbow
					sphere(0.2, 0.8, 0.8, 0.8); //elbow - (radius, red, green,blue)
	
					glTranslatef (-0.6, 0.0, 0.0); //move forearm to left
					cuboid(1.25, 0.24, 0.24, 0.1, 0.1, 0.1); //forearm - (length, height, width, red, green, blue)
				
					glPushMatrix(); //---- Hand ----
						glTranslatef (-0.6, 0.0, 0.0);
						glRotatef(180, 0, 0, 1);
						hand(kepitL);
					glPopMatrix(); //---- End Hand ----

				glPopMatrix();
				
			glPopMatrix();	//-- End Lower Arm --

		glPopMatrix();
	glPopMatrix();	//--- End Upper Arm ---
	//-------------- End Left Arm --------------
	
	glPopMatrix();	
	//============================== End Robot ==============================

	glutSwapBuffers();
	
	//fly or fall
	if (on == 1) {
		fly();
		fan = (fan + 2) % 360;
	}
	else fall();
}

void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//glOrtho(-3.0, 3.0, -3.0, 3.0, 0.0, 10.0);  // Set the view frustrum: left, right, bottom, top, near & far (orthographic)
	gluPerspective (60, (GLfloat)w / (GLfloat)h, 1.0, 100.0); // fovy, aspect ratio, near, far

	glMatrixMode(GL_MODELVIEW);
}
void input(int key, int x, int y)
{
	float k = 0.1;
	switch (key) {
		case GLUT_KEY_UP:
			coordinateY += k;
			flyPosition = coordinateY;
			glutPostRedisplay();
			break;
		case GLUT_KEY_DOWN:
			if (coordinateY > -2.5 + 0.5) { //set lowest position downward
				coordinateY -= k;
				flyPosition = coordinateY;
			}
			glutPostRedisplay();
			break;
		case GLUT_KEY_LEFT:
			coordinateX -= k;
			glutPostRedisplay();
			break;
		case GLUT_KEY_RIGHT:
			coordinateX += k;
			glutPostRedisplay();
			break;
		default:
			break;
	}
}
void keyboard (unsigned char key, int x, int y)
{
	switch (key) {
		// On/Off helicopter
		case 32: //Spacebar key
			if(on == 0) on = 1;
			else on = 0;
			break;
		// Turn body
		case 'g':
		case 'G':
			turn -= 1;
			break;
		case 'h':
		case 'H':
			turn += 1;
			break;
		case 'y':
			coordinateZ -= 0.5;
			glutPostRedisplay();
			break;
		case 'b':
			coordinateZ += 0.5;
			glutPostRedisplay();
			break;
		//=============== Right Arm ================
		case 'i': //lift right shoulder
			if(shoulderR < 90)
				shoulderR = shoulderR + 5;
			glutPostRedisplay();
			break;
		case 'k': //lower right shoulder
			if(shoulderR > -90)
				shoulderR = shoulderR - 5;
			glutPostRedisplay();
			break;
		case 'j': //foward right shoulder
			if(shoulderRy > -105)
				shoulderRy = shoulderRy - 5;
			glutPostRedisplay();
			break;
		case 'l': //backward right shoulder
			if(shoulderRy < 30)
				shoulderRy = shoulderRy + 5;
			glutPostRedisplay();
			break;
		case 'p': //lift right elbow
			if(elbowR < 160)
				elbowR = elbowR + 5;
			glutPostRedisplay();
			break;
		case ';': //lower right elbow
			if(elbowR > -160)
				elbowR = elbowR - 5;
			glutPostRedisplay();
			break;
		case 'u': //foward right elbow
			if(elbowRy > -160)
			elbowRy = elbowRy - 5;
			glutPostRedisplay();
			break;
		case 'o': //foward right elbow
			if(elbowRy < 0)
			elbowRy = elbowRy + 5;
			glutPostRedisplay();
			break;
		case 'n': //kepit kanan
			if (kepitR == 0) kepitR = 45;
			else kepitR = 0;
			glutPostRedisplay();
			break;
		//------------ End Right Arm ---------------
		//=============== Left Arm =================
		case 'e': //lift left shoulder
			if(shoulderL > -90)
				shoulderL = shoulderL - 5;
			glutPostRedisplay();
			break;
		case 'd': //lower left shoulder
			if(shoulderL < 90)
				shoulderL = shoulderL + 5;
			glutPostRedisplay();
			break;
		case 'f': //foward left shoulder
			if(shoulderLy < 105)
				shoulderLy = shoulderLy + 5;
			glutPostRedisplay();
			break;
		case 's': //backward left shoulder
			if(shoulderLy > -30)
				shoulderLy = shoulderLy - 5;
			glutPostRedisplay();
			break;
		case 'q': //lift left elbow
			if(elbowL > -160)
				elbowL = elbowL - 5;
			glutPostRedisplay();
			break;
		case 'a': //lower left elbow
			if(elbowL < 160)
				elbowL = elbowL + 5;
			glutPostRedisplay();
			break;
		case 'w': //backward left elbow
			if(elbowLy > 0)
				elbowLy = (elbowLy - 5) % 360;
			glutPostRedisplay();
			break;
		case 'r': //foward left elbow
			if(elbowLy < 160)
				elbowLy = (elbowLy + 5) % 360;
			glutPostRedisplay();
			break;
		case 'v': //kepit kiri
			if (kepitL == 0) kepitL = 45;
			else kepitL = 0;
			glutPostRedisplay();
			break;
		//------------- End Left Arm ---------------
		// Reset
		case 8: //Backspace key
			//coordinateX = 0.0; coordinateY = -2.5; flyPosition = 0.0; on = 0; floating = 1; fan = 0;
			shoulderR = 0; shoulderL = 0; shoulderRy = 0; shoulderLy = 0; 
			elbowR = 0; elbowL = 0; elbowRy = 0; elbowLy = 0; turn = 0.0;
			break;
		// Quit
		case 27: //Esc key
			exit(0);
			break;
		default:
			break;
	}
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(1200, 700);
	glutInitWindowPosition(5, 5);
	glutCreateWindow(argv[0]);
	init();
	glutDisplayFunc(display);
	glutIdleFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(input);

	glutMainLoop();
	return 0;
}