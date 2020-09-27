#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#include <windows.h>
#include <glut.h>

#define pi (2*acos(0.0))

double cameraHeight;
double cameraAngle;
int drawgrid;
int drawaxes;
double angle;

typedef struct point
{
	double x,y;
};

point p1,p2,v1,v2;
point p_large;
point perp;


//void drawAxes()
//{
//	if(drawaxes==1)
//	{
//		glColor3f(1.0, 1.0, 1.0);
//		glBegin(GL_LINES);{
//			glVertex3f( 100,0,0);
//			glVertex3f(-100,0,0);
//
//			glVertex3f(0,-100,0);
//			glVertex3f(0, 100,0);
//
//			glVertex3f(0,0, 100);
//			glVertex3f(0,0,-100);
//		}glEnd();
//	}
//}
//
//
//void drawGrid()
//{
//	int i;
//	if(drawgrid==1)
//	{
//		glColor3f(0.6, 0.6, 0.6);	//grey
//		glBegin(GL_LINES);{
//			for(i=-8;i<=8;i++){
//
//				if(i==0)
//					continue;	//SKIP the MAIN axes
//
//				//lines parallel to Y-axis
//				glVertex3f(i*10, -90, 0);
//				glVertex3f(i*10,  90, 0);
//
//				//lines parallel to X-axis
//				glVertex3f(-90, i*10, 0);
//				glVertex3f( 90, i*10, 0);
//			}
//		}glEnd();
//	}
//}
//
//void drawSquare(double a)
//{
//    //glColor3f(1.0,0.0,0.0);
//	glBegin(GL_QUADS);{
//		glVertex3f( a, a,2);
//		glVertex3f( a,-a,2);
//		glVertex3f(-a,-a,2);
//		glVertex3f(-a, a,2);
//	}glEnd();
//}


void drawCircle(point p,double red,double green,double blue,double radius,int segments)
{
    int i;
    point points[200];
    glColor3f(red,green,blue);
    //generate points
    for(i=0;i<=segments;i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi)+p.x;
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi)+p.y;
    }
    //draw segments using generated points
    for(i=0;i<segments;i++)
    {
        glBegin(GL_LINES);
        {
			glVertex3f(points[i].x,points[i].y,0);
			glVertex3f(points[i+1].x,points[i+1].y,0);
        }
        glEnd();
    }
}

void drawArrow(point p,point v) {
    point pt;
    pt.x=p.x+12*v.x;
    pt.y=p.y+12*v.y;

    glBegin(GL_LINES);
        {
			glVertex3f(p.x,p.y,0);
			glVertex3f(pt.x,pt.y,0);
        }
    glEnd();

    point vec;
    vec.x=pt.x-p.x;
    vec.y=pt.y-p.y;

    point temp;
    temp.x=p.x+8*v.x;
    temp.y=p.y+8*v.y;

    point perp;
    perp.x=-vec.y;
    perp.y=vec.x;

    point r,l;
    r.x=temp.x+0.4*perp.x;
    r.y=temp.y+0.4*perp.y;
    l.x=temp.x-0.4*perp.x;
    l.y=temp.y-0.4*perp.y;

    //printf("%lf %lf\n",vec_x,vec_y);

   glColor3f(1,0,0);  //arrow triangle
      glBegin(GL_TRIANGLES);{
          glVertex3f( pt.x,pt.y,0);
          glVertex3f( r.x,r.y,0);
          glVertex3f( l.x,l.y,0);
      }glEnd();


}

//void drawCone(double radius,double height,int segments)
//{
//    int i;
//    double shade;
//    struct point points[100];
//    //generate points
//    for(i=0;i<=segments;i++)
//    {
//        points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
//        points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
//    }
//    //draw triangles using generated points
//    for(i=0;i<segments;i++)
//    {
//        //create shading effect
//        if(i<segments/2)shade=2*(double)i/(double)segments;
//        else shade=2*(1.0-(double)i/(double)segments);
//        glColor3f(shade,shade,shade);
//
//        glBegin(GL_TRIANGLES);
//        {
//            glVertex3f(0,0,height);
//			glVertex3f(points[i].x,points[i].y,0);
//			glVertex3f(points[i+1].x,points[i+1].y,0);
//        }
//        glEnd();
//    }
//}
//
//
//void drawSphere(double radius,int slices,int stacks)
//{
//	struct point points[100][100];
//	int i,j;
//	double h,r;
//	//generate points
//	for(i=0;i<=stacks;i++)
//	{
//		h=radius*sin(((double)i/(double)stacks)*(pi/2));
//		r=radius*cos(((double)i/(double)stacks)*(pi/2));
//		for(j=0;j<=slices;j++)
//		{
//			points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
//			points[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
//			points[i][j].z=h;
//		}
//	}p1
//	//draw quads using generated points
//	for(i=0;i<stacks;i++)
//	{
//        glColor3f((double)i/(double)stacks,(double)i/(double)stacks,(double)i/(double)stacks);
//		for(j=0;j<slices;j++)
//		{
//			glBegin(GL_QUADS);{
//			    //upper hemisphere
//				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
//				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
//				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
//				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
//                //lower hemisphere
//                glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
//				glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
//				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
//				glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
//			}glEnd();
//		}
//	}
//}


void drawSS()
{
//    glColor3f(1,0,0);
//    drawSquare(20);
//
//    glRotatef(angle,0,0,1);
//    glTranslatef(110,0,0);
//    glRotatef(2*angle,0,0,1);
//    glColor3f(0,1,0);
//    drawSquare(15);
//
//    glPushMatrix();
//    {
//        glRotatef(angle,0,0,1);
//        glTranslatef(60,0,0);
//        glRotatef(2*angle,0,0,1);
//        glColor3f(0,0,1);
//        drawSquare(10);
//    }
//    glPopMatrix();
//
//    glRotatef(3*angle,0,0,1);
//    glTranslatef(40,0,0);
//    glRotatef(4*angle,0,0,1);
//    glColor3f(1,1,0);
//    drawSquare(5);


      drawCircle(p_large,0.7,0.7,0.7,120,195);

      drawCircle(p1,1,1,0,12,195);

      drawCircle(p2,0,1,0,12,195);

      glColor3f(0.7,0.7,0.7);
      drawArrow(p1,v1);

      glColor3f(0.7,0.7,0.7);
      drawArrow(p2,v2);
//      glTranslatef(p1.x,p1.y,0);
//      drawCircle(1,1,0,12,190);
//
//      glColor3f(0.7,0.7,0.7);  //arrow straight line
//      glBegin(GL_LINES);{
//          glVertex3f( 0,0,0);
//          glVertex3f( 0,12,0);
//      }glEnd();
//
//      glColor3f(1,0,0);  //arrow triangle
//      glBegin(GL_TRIANGLES);{
//          glVertex3f( 0,12,0);
//          glVertex3f( 4,8,0);
//          glVertex3f( -4,8,0);
//      }glEnd();
//
//      glTranslatef(-80,20,0);
//      drawCircle(0,1,0,12,190);
//
//      glColor3f(0.7,0.7,0.7);  //arrow straight line
//      glBegin(GL_LINES);{
//          glVertex3f( 0,0,0);
//          glVertex3f( 12,0,0);
//      }glEnd();
//
//      glColor3f(1,0,0);  //arrow triangle
//      glBegin(GL_TRIANGLES);{
//          glVertex3f( 12,0,0);
//          glVertex3f( 8,4,0);
//          glpoint perp;Vertex3f( 8,-4,0);
//      }glEnd();


}

void keyboardListener(unsigned char key, int x,int y){
	switch(key){

		case '1':
			drawgrid=1-drawgrid;
			break;

		default:
			break;
	}
}


void specialKeyListener(int key, int x,int y){
	switch(key){
		case GLUT_KEY_DOWN:		//down arrow key
			cameraHeight -= 3.0;
			break;
		case GLUT_KEY_UP:		// up arrow key
			cameraHeight += 3.0;
			break;

		case GLUT_KEY_RIGHT:
			//cameraAngle += 0.03;
			perp.x=-v1.y;
			perp.y=v1.x;
			v1.x=v1.x*cos(0.5)-perp.x*sin(0.5);
			v1.y=v1.y*cos(0.5)-perp.y*sin(0.5);
			break;
		case GLUT_KEY_LEFT:
			//cameraAngle -= 0.03;
			perp.x=-v1.y;
			perp.y=v1.x;
			v1.x=v1.x*cos(0.5)+perp.x*sin(0.5);
			v1.y=v1.y*cos(0.5)+perp.y*sin(0.5);
			break;

		case GLUT_KEY_PAGE_UP:
			break;
		case GLUT_KEY_PAGE_DOWN:
			break;

		case GLUT_KEY_INSERT:
			break;

		case GLUT_KEY_HOME:
			break;
		case GLUT_KEY_END:
			break;

		default:
			break;
	}
}


void mouseListener(int button, int state, int x, int y){	//x, y is the x-y of the screen (2D)
	switch(button){
		case GLUT_LEFT_BUTTON:
			if(state == GLUT_DOWN){		// 2 times?? in ONE click? -- solution is checking DOWN or UP
				drawaxes=1-drawaxes;
			}
			break;

		case GLUT_RIGHT_BUTTON:
			//........
			break;

		case GLUT_MIDDLE_BUTTON:
			//........
			break;

		default:
			break;
	}
}



void display(){

	//clear the display
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0,0,0,0);	//color black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/********************
	/ set-up camera here
	********************/
	//load the correct matrix -- MODEL-VIEW matrix
	glMatrixMode(GL_MODELVIEW);

	//initialize the matrix
	glLoadIdentity();

	//now give three info
	//1. where is the camera (viewer)?
	//2. where is the camera looking?
	//3. Which direction is the camera's UP direction?

	//gluLookAt(100,100,100,	0,0,0,	0,0,1);
	//gluLookAt(200*cos(cameraAngle), 200*sin(cameraAngle), cameraHeight,		0,0,0,		0,0,1);
	gluLookAt(0,0,200,	0,0,0,	0,1,0);


	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);


	/****************************
	/ Add your objects from here
	****************************/
	//add objects

	//drawAxes();
	//drawGrid();

    //glColor3f(1,0,0);
    //drawSquare(10);

    drawSS();


    //drawCircle(30,24);

    //drawCone(20,50,24);

	//drawSphere(30,24,20);




	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}


void animate(){
	//angle+=0.05;
	//v1.x=1;
	//v1.y=1;
//	if((p1.x*p1.x+p1.y*p1.y)<108) {
//        p1.x+=0.05*v1.x;
//        p1.y+=0.05*v1.y;
//	}
//	else if((p1.x*p1.x+p1.y*p1.y)==108) {
//        point n1;
//        double a=p1.x*p1.x+p1.y*p1.y;
//        n1.x=p1.x;
//        n1.x=(double)(n1.x)/(double) sqrt(a);
//        n1.y=p1.y;
//        n1.y=(double)(n1.y)/(double) sqrt(a);
//
//        n1.x=(v1.x*n1.x)*n1.x;
//        v1.x=v1.x-2*n1.x;
//        n1.y=(v1.y*n1.y)*n1.y;
//        v1.y=v1.y-2*n1.y;
//
//	}
//
//	if((p2.x*p2.x+p2.y*p2.y)<108) {
//        p2.x+=0.05*v2.x;
//        p2.y+=0.05*v2.y;
//	}
//	else if((p2.x*p2.x+p2.y*p2.y)==108) {
//        point n2;
//        double b=p2.x*p2.x+p2.y*p2.y;
//        n2.x=p2.x;
//        n2.x=(double)(n2.x)/(double) sqrt(b);
//        n2.y=p2.y;
//        n2.y=(double)(n2.y)/(double) sqrt(b);
//
//        n2.x=(v2.x*n2.x)*n2.x;
//        v2.x=v2.x-2*n2.x;
//        n2.y=(v2.y*n2.y)*n2.y;
//        v2.y=v2.y-2*n2.y;
//
//	}
//
//	if (sqrt((p1.x-p2.x)*(p1.x-p2.x)+(p1.y-p2.y)*(p1.y-p2.y))==24) {
//        point n3;
//        double c=p1.x*p1.x+p1.y*p1.y;
//        n3.x=p1.x;
//        n3.x=(double)(n3.x)/(double) sqrt(c);
//        n3.y=p1.y;
//        n3.y=(double)(n3.y)/(double) sqrt(c);
//
//        n3.x=(v1.x*n3.x)*n3.x;
//        v1.x=v1.x-2*n3.x;
//        n3.y=(v1.y*n3.y)*n3.y;
//        v1.y=v1.y-2*n3.y;
//
//        point n4;
//        double d=p2.x*p2.x+p2.y*p2.y;
//        n4.x=p2.x;
//        n4.x=(double)(n4.x)/(double) sqrt(d);
//        n4.y=p2.y;
//        n4.y=(double)(n4.y)/(double) sqrt(d);
//
//        n4.x=(v2.x*n4.x)*n4.x;
//        v2.x=v2.x-2*n4.x;
//        n4.y=(v2.y*n4.y)*n4.y;
//        v2.y=v2.y-2*n4.y;
//
//	}

    p1.x+=0.2*v1.x;
    p1.y+=0.2*v1.y;

    if(sqrt(p1.x*p1.x+p1.y*p1.y)>=108) {
        point n1;
        double a=p1.x*p1.x+p1.y*p1.y;
        n1.x=p1.x;
        n1.x=(double)(n1.x)/(double) sqrt(a);
        n1.y=p1.y;
        n1.y=(double)(n1.y)/(double) sqrt(a);

        double dot1=v1.x*n1.x+v1.y*n1.y;
        n1.x=dot1*n1.x;
        v1.x=v1.x-2*n1.x;
        n1.y=dot1*n1.y;
        v1.y=v1.y-2*n1.y;
	}

    p2.x+=0.2*v2.x;
    p2.y+=0.2*v2.y;

    if(sqrt(p2.x*p2.x+p2.y*p2.y)>=108) {
        point n2;
        double b=p2.x*p2.x+p2.y*p2.y;
        n2.x=p2.x;
        n2.x=(double)(n2.x)/(double) sqrt(b);
        n2.y=p2.y;
        n2.y=(double)(n2.y)/(double) sqrt(b);

        double dot2=v2.x*n2.x+v2.y*n2.y;
        n2.x=dot2*n2.x;
        v2.x=v2.x-2*n2.x;
        n2.y=dot2*n2.y;
        v2.y=v2.y-2*n2.y;
	}

	if (sqrt((p1.x-p2.x)*(p1.x-p2.x)+(p1.y-p2.y)*(p1.y-p2.y))<24) {
        point coli;
        coli.x=(p1.x+p2.x)/2;
        coli.y=(p1.y+p2.y)/2;

        point n_1;
        n_1.x=p1.x-coli.x;
        n_1.y=p1.y-coli.y;
        double c=n_1.x*n_1.x+n_1.y*n_1.y;
        n_1.x=(double)(n_1.x)/(double) sqrt(c);
        n_1.y=(double)(n_1.y)/(double) sqrt(c);

        double dot3=n_1.x*v1.x+n_1.y*v1.y;
        n_1.x=dot3*n_1.x;
        v1.x=v1.x-2*n_1.x;
        n_1.y=dot3*n_1.y;
        v1.y=v1.y-2*n_1.y;



        point n_2;
        n_2.x=p2.x-coli.x;
        n_2.y=p2.y-coli.y;
        double d=n_2.x*n_2.x+n_2.y*n_2.y;
        n_2.x=(double)(n_2.x)/(double) sqrt(d);
        n_2.y=(double)(n_2.y)/(double) sqrt(d);

        double dot4=n_2.x*v2.x+n_2.y*v2.y;
        n_2.x=dot4*n_2.x;
        v2.x=v2.x-2*n_2.x;
        n_2.y=dot4*n_2.y;
        v2.y=v2.y-2*n_2.y;


	}


	//codes for any changes in Models, Camera
	glutPostRedisplay();
}

void init(){
	//codes for initialization
	drawgrid=0;
	drawaxes=1;
	cameraHeight=150.0;
	cameraAngle=1.0;
	angle=0;
	v1.x=0;
    v1.y=1;

    v2.x=-1;
    v2.y=0;


    p_large.x=0;
    p_large.y=0;

    p1.x=40;
    p1.y=-40;

    p2.x=70;
    p2.y=+30;

	//clear the screen
	glClearColor(0,0,0,0);

	/************************
	/ set-up projection here
	************************/
	//load the PROJECTION matrix
	glMatrixMode(GL_PROJECTION);

	//initialize the matrix
	glLoadIdentity();

	//give PERSPECTIVE parameters
	gluPerspective(80,	1,	1,	1000.0);
	//field of view in the Y (vertically)
	//aspect ratio that determines the field of view in the X direction (horizontally)
	//near distance
	//far distance
}

int main(int argc, char **argv){
	glutInit(&argc,argv);
	glutInitWindowSize(600, 600);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

	glutCreateWindow("My OpenGL Program");

	init();

	glEnable(GL_DEPTH_TEST);	//enable Depth Testing

	glutDisplayFunc(display);	//display callback function
	glutIdleFunc(animate);		//what you want to do in the idle time (when no drawing is occuring)

	glutKeyboardFunc(keyboardListener);
	glutSpecialFunc(specialKeyListener);
	glutMouseFunc(mouseListener);

	glutMainLoop();		//The main loop of OpenGL

	return 0;
}
