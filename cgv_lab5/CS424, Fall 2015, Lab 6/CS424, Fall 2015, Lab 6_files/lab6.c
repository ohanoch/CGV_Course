
/**
 * CPSC 424, Fall 2015, Lab 6:  Light and Material in OpenGL 1.1.
 * This program shows a square "stage" with a variety of objects
 * arranged on it.  The objects use several shapes and materials
 * and include a wireframe object that is drawn with lighting
 * turned off.
 *
 * This program use GLU as well as GLUT, and it depends on polyhedron.c, 
 * which requires the math library.  It can be compiled with
 *
 *        gcc -o lab6 lab6.c polyhedron.c -lGL -lglut -lGLU -lm
 */

#include <GL/gl.h>
#include <GL/freeglut.h>
#include <stdio.h>      // (Can be used for debugging messages, with printf().)
#include "polyhedron.h" // For access to the regular polyhedra from polyhedron.c.

#include <math.h>
#include <string.h>
// --------------------------- Data for some materials ---------------------------------------------------

/**
 * One of the rows of this array corresponds to a set of material properties.  Items 0 to 4 in a row
 * specify an ambient color; items 4 through 7, a diffuse color; items 8 through 11, a specular color;
 * and item 12, a specular exponent (shininess value).  The data is adapted from the table on the page
 * http://devernay.free.fr/cours/opengl/materials.html
 */
#define SIZE_OF_MATERIALS 13
#define NUM_OF_MATERIALS 19
#define MATERIAL_NAME_MAX_SIZE 30

float materials[][SIZE_OF_MATERIALS] = {
	{ /* "emerald" */   0.0215f, 0.1745f, 0.0215f, 1.0f, 0.07568f, 0.61424f, 0.07568f, 1.0f, 0.633f, 0.727811f, 0.633f, 1.0f, 0.6f*128 },
	{ /* "jade" */   0.135f, 0.2225f, 0.1575f, 1.0f, 0.54f, 0.89f, 0.63f, 1.0f, 0.316228f, 0.316228f, 0.316228f, 1.0f, 0.1f*128 },
	{ /* "obsidian" */   0.05375f, 0.05f, 0.06625f, 1.0f, 0.18275f, 0.17f, 0.22525f, 1.0f, 0.332741f, 0.328634f, 0.346435f, 1.0f, 0.3f*128 },
	{ /* "pearl" */   0.25f, 0.20725f, 0.20725f, 1.0f, 1.0f, 0.829f, 0.829f, 1.0f, 0.296648f, 0.296648f, 0.296648f, 1.0f, 0.088f*128 },
	{ /* "ruby" */   0.1745f, 0.01175f, 0.01175f, 1.0f, 0.61424f, 0.04136f, 0.04136f, 1.0f, 0.727811f, 0.626959f, 0.626959f, 1.0f, 0.6f*128 },
	{ /* "turquoise" */   0.1f, 0.18725f, 0.1745f, 1.0f, 0.396f, 0.74151f, 0.69102f, 1.0f, 0.297254f, 0.30829f, 0.306678f, 1.0f, 0.1f*128 },
	{ /* "brass" */   0.329412f, 0.223529f, 0.027451f, 1.0f, 0.780392f, 0.568627f, 0.113725f, 1.0f, 0.992157f, 0.941176f, 0.807843f, 1.0f, 0.21794872f*128 },
	{ /* "bronze" */   0.2125f, 0.1275f, 0.054f, 1.0f, 0.714f, 0.4284f, 0.18144f, 1.0f, 0.393548f, 0.271906f, 0.166721f, 1.0f, 0.2f*128 },
	{ /* "chrome" */   0.25f, 0.25f, 0.25f, 1.0f, 0.4f, 0.4f, 0.4f, 1.0f, 0.774597f, 0.774597f, 0.774597f, 1.0f, 0.6f*128 },
	{ /* "copper" */   0.19125f, 0.0735f, 0.0225f, 1.0f, 0.7038f, 0.27048f, 0.0828f, 1.0f, 0.256777f, 0.137622f, 0.086014f, 1.0f, 0.1f*128 },
	{ /* "gold" */   0.24725f, 0.1995f, 0.0745f, 1.0f, 0.75164f, 0.60648f, 0.22648f, 1.0f, 0.628281f, 0.555802f, 0.366065f, 1.0f, 0.9f*128 },
	{ /* "silver" */   0.19225f, 0.19225f, 0.19225f, 1.0f, 0.50754f, 0.50754f, 0.50754f, 1.0f, 0.508273f, 0.508273f, 0.508273f, 1.0f, 0.4f*128 },
	{ /* "cyan plastic" */   0.0f, 0.1f, 0.06f, 1.0f, 0.0f, 0.50980392f, 0.50980392f, 1.0f, 0.50196078f, 0.50196078f, 0.50196078f, 1.0f, .25f*128 },
	{ /* "green plastic" */   0.0f, 0.0f, 0.0f, 1.0f, 0.1f, 0.35f, 0.1f, 1.0f, 0.45f, 0.55f, 0.45f, 1.0f, .25f*128 },
	{ /* "red plastic" */   0.0f, 0.0f, 0.0f, 1.0f, 0.5f, 0.0f, 0.0f, 1.0f, 0.7f, 0.6f, 0.6f, 1.0f, .25f*128 },
	{ /* "cyan rubber" */   0.0f, 0.05f, 0.05f, 1.0f, 0.4f, 0.5f, 0.5f, 1.0f, 0.04f, 0.7f, 0.7f, 1.0f, .078125f*128 },
	{ /* "green rubber" */   0.0f, 0.05f, 0.0f, 1.0f, 0.4f, 0.5f, 0.4f, 1.0f, 0.04f, 0.7f, 0.04f, 1.0f, .078125f*128 },
	{ /* "red rubber" */   0.05f, 0.0f, 0.0f, 1.0f, 0.5f, 0.4f, 0.4f, 1.0f, 0.7f, 0.04f, 0.04f, 1.0f, .078125f*128 },
	{ /* "custom pi" */   0.3f, 0.1f, 0.4f, 0.1f, 0.5f, 0.9f, 0.2f, 0.6f, 0.5f, 0.3f, 0.05f, 0.8f, .9f*128 },
};

char materials_names[][MATERIAL_NAME_MAX_SIZE] = {"emerald","jade","obsidian","pearl","ruby","turquoise","brass","bronze","chrome","copper","gold","silver","cyan plastic","green plastic","red plastic","cyan rubber","green rubber","red rubber","custom pi"};

int yRotation = 0;
int xRotation = 0;
int X_ROTATION_LIMIT = 15;

/**
 * This function accepts a string of a material type as an input.
 * If that string is found in the materials array then the current working gl material will be set to it.
 * This function has no output.
 */
void setMaterial(char* request){
	for(int i = 0; i < NUM_OF_MATERIALS; i++){
		if(!strcmp(materials_names[i],request)){
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, materials[i]);
			glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, materials[i]);
			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, &materials[i][8]);
			glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &materials[i][12]);
			printf("color changed to: %s\n", materials_names[i]);
			return;
		}
	}
	printf("Attempted changing material to: %s FAILED.\n", request);
} 

/**
 * This function accepts a polyhedron object and renders it on the stage.
 * 3 options: faces only, lines only, or both.
 * Face color only matters is glEnable(GL_COLOR_MATERIAL); is set. otherwise material takes preference
 * Input: polyhedron object, mode. mode = 0,1,2 for fill,wire,both respectivly
 * Output: No output
*/
void drawPolyhedron(Polyhedron poly, char* material, int mode, int useColor){
	createPolyhedra();
	glPolygonOffset(1,1);


	if (useColor){
		glEnable(GL_COLOR_MATERIAL);
	}

	//loop twice, k=0 for face, k=1 for wire
	for(int k=0; k<2; k++){
		if(k==0){
			//if mode = 1 we only want wire, thus we skip itteration 0 of for loop
			if (mode == 1){
				continue;
			}
			glEnable( GL_POLYGON_OFFSET_FILL );
		}
		
		//if mode = 0 we only want fill, thus we skip itteration 1 of for loop which does wire
		if(k == 1){
			if(mode == 0){
				break;
			}
			glDisable(GL_LIGHTING);
		}
		int currVertexIndex = 0;
		for(int faceIndex = 0; faceIndex<poly.faceCount; faceIndex++){
			//if drawing faces set colors according to color array otherwise set color to black
			if(k==0){
				glBegin(GL_TRIANGLE_FAN);
			} else {
				glBegin(GL_LINE_LOOP);
			}

			if (k == 1){
				//glColor3d(lineColors[0],lineColors[1],lineColors[2]); //set line number to black
			}
			else if(poly.faceColors && useColor){
				glColor3dv(&poly.faceColors[faceIndex * 3]);
			} else {
				setMaterial(material);
			}
			
			glNormal3dv(&poly.normals[faceIndex * 3]);

			//set vertecies of face
			while(poly.faces[currVertexIndex] != -1){
				int currFaceVertexNum = poly.faces[currVertexIndex];
				glVertex3dv(&poly.vertices[currFaceVertexNum * 3]);
				currVertexIndex++;
			}
			currVertexIndex++;

			glEnd();
		}

		//Disable gl options enabled specifically for this rendering
		if(k==0){
			glDisable( GL_POLYGON_OFFSET_FILL );
		} else {
			glEnable(GL_LIGHTING);
		}
	}
	if(useColor){
		glDisable(GL_COLOR_MATERIAL);
	}
}
// ------------------------ OpenGL rendering and  initialization -----------------------

/**
 * The display method is called when the panel needs to be drawn.
 * Here, it draws a stage and some objects on the stage.
 */
void display() {
        // called whenever the display needs to be redrawn

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
    glLoadIdentity();
    gluLookAt( 0,8,40, 0,1,0, 0,1,0 );  // viewing transform	
    
    float gray[] = { 0.6f, 0.6f, 0.6f, 1 };
    float zero[] = { 0, 0, 0, 1 };
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, gray);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, zero);
	
	glRotatef(yRotation,0,1,0);
	glRotatef(xRotation,1,0,0);

	glPushMatrix();
    glTranslatef(0,-1.5,0); // Move top of stage down to y = 0
    glScalef(1, 0.05, 1); // Stage will be one unit thick,
    glutSolidCube(20);
    glPopMatrix();
	
    // TODO draw some shapes!

	//wire sphere front right corner without lighting
	glPushMatrix();
	glDisable(GL_LIGHTING);
	glColor3b(43,129,233);
	glLineWidth(3);
	glTranslatef(7.0,2,7.0);
    glutWireSphere(3.0, 10, 10);
	glEnable(GL_LIGHTING);
	glPopMatrix();    
	
	//glutSolidIcosahedron front left corner in gold
	glPushMatrix();
	setMaterial("gold");
	glTranslatef(0.0,0.0,-7.0);
	glutSolidIcosahedron();
	glPopMatrix();

	//glutSolidTeapot back left corner using custom pi coloring
	glPushMatrix();
	setMaterial("custom pi");
	glTranslatef(-7.0,-0.15,-7.0);
	glutSolidTeapot(1);
	glPopMatrix();

	//Polyhedron filled house back right corner with colors defined by polyhedron
	glPushMatrix();
	glTranslatef(7.0,0,-7.0);
	drawPolyhedron(house, "none",0, 1);
	glPopMatrix();

	//Polyhedron wired stellatedDodecahedron left middle. line color = blue
	glPushMatrix();
	glColor3b(233,129,43);
	glTranslatef(-7.0,0,0);
	drawPolyhedron(stellatedDodecahedron, "none", 1, 0);
	glPopMatrix();

	//Polyhedron truncatedRhombicDodecahedron filled and wired back middle. fill color = pearl. line color = green
	glPushMatrix();
	glColor3b(129,43,233);
	glTranslatef(-7.0,1.1,7.0);
	drawPolyhedron(truncatedRhombicDodecahedron, "gold", 2, 0);
	glPopMatrix();
	

    glutSwapBuffers();  // (Required for double-buffered drawing, at the end of display().)
}

/**
 * initGL() is called just once, by main(), to do initialization of OpenGL state
 * and other global state. Here, it sets up a projection, configures some lighting, 
 * and enables the depth test.  It also calls createPolyhedra(), whcih is defined
 * in the included file, polyhedron.h.
 */
void initGL() {
    createPolyhedra();
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(20, 2, 1, 100);
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    // TODO configure better lighting!

	//configure light1
	glEnable(GL_LIGHT1);
	GLfloat light_ambient1[] = { 0.3, 0.3, 0.3, 1.0 };
	GLfloat light_diffuse1[] = { 0.3, 0.3, 0.3, 1.0 };
	GLfloat light_specular1[] = { 0.2, 0.2, 0.2, 1.0 };
	GLfloat light_position1[] = { 0.0, 50, 0.0, 0.0 };

	glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse1);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular1);
	glLightfv(GL_LIGHT1, GL_POSITION, light_position1);

	//configure light2
	
	glEnable(GL_LIGHT2);
	GLfloat light_ambient2[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat light_diffuse2[] = { 0.3, 0.3, 0.3, 1.0 };
	GLfloat light_specular2[] = { 0.4, 0.4, 0.4, 1.0 };
	GLfloat light_position2[] = { 0.0, 0.0, -50, 0.0 };

	glLightfv(GL_LIGHT2, GL_AMBIENT, light_ambient2);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, light_diffuse2);
	glLightfv(GL_LIGHT2, GL_SPECULAR, light_specular2);
	glLightfv(GL_LIGHT2, GL_POSITION, light_position2);

}  // end initGL()


// ------------------------------ mouse handling functions ----------------------------------

int dragging;        // 0 or 1 to indicate whether a drag operation is in progress
int dragButton;      // which button started the drag operation
int prevX, prevY;    // previous mouse position during drag

/*  mouseUpOrDown() is set up in main() to be called when the user presses or releases
 *  a button on the mouse.  The button paramter is one of the contants GLUT_LEFT_BUTTON,
 *  GLUT_MIDDLE_BUTTON, or GLUT_RIGHT_BUTTON.  The buttonState is GLUT_UP or GLUT_DOWN and
 *  tells whether this is a mouse press or a mouse release event.  x and y give the
 *  mouse position in pixel coordinates, with (0,0) at the UPPER LEFT.
 */
void mouseUpOrDown(int button, int buttonState, int x, int y) {
       // called to respond to mouse press and mouse release events
   if (buttonState == GLUT_DOWN) {  // a mouse button was pressed
       if (dragging)
          return;  // Ignore a second button press during a draw operation.
       dragging = 1;
       dragButton = button;
       prevX = x;
       prevY = y;
   }
   else {  // a mouse button was released
       if ( ! dragging || button != dragButton )
           return; // this mouse release does not end a drag operation.
       dragging = 0;
   }
}

/*  mouseDragged() is set up in main() to be called when the user moves the mouse,
 *  but only when one or more mouse buttons are pressed.  x and y give the position
 *  of the mouse in pixel coordinates.
 */
void mouseDragged(int x, int y) {
        // called to respond when the mouse moves during a drag
    if ( ! dragging )
        return;

    // Change roation variables according to mouse change when dragged
	yRotation += (x - prevX);
	xRotation += (y-prevY);

	//Limit x rotation degrees
	if(xRotation > X_ROTATION_LIMIT){
		xRotation = X_ROTATION_LIMIT;
	}
	else if (xRotation < -X_ROTATION_LIMIT){
		xRotation = -X_ROTATION_LIMIT;
	}

    glutPostRedisplay();  // make OpenGL redraw the scene
    prevX = x;
    prevY = y;
}


// ----------------- main routine -------------------------------------------------

int main(int argc, char** argv) {
    glutInit(&argc, argv); // Allows processing of certain GLUT command line options
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);  // Use double buffering and a depth buffer.
    glutInitWindowSize(1000,500);       // size of display area, in pixels
    glutInitWindowPosition(100,100);    // location in window coordinates
    glutCreateWindow("Stage");          // parameter is window title  
    initGL();                           // do OpenGL initialization for the window
    glutDisplayFunc(display);           // call display() to draw the scene
    glutMouseFunc(mouseUpOrDown);       // call mouseUpOrDown() for mousedown and mouseup events
    glutMotionFunc(mouseDragged);       // call mouseDragged() when mouse moves, only during a drag gesture
    glutMainLoop(); // Run the event loop!  This function does not return.
    return 0;
}

