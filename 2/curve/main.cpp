#include<stdio.h>
using namespace std;
#include<stdlib.h>
#include<math.h>
#include<iostream>

#include <windows.h>
#include <glut.h>

#define pi (2*acos(0.0))

typedef struct point2d
{
    double x, y;
};

point2d cp[200];
point2d cpa[200];
point2d hermit[110];
point2d trackhermit[5000];
int cpidx;
int cpaidx;
int dot;
int heridx;
int flag;
double ax,ay,bx,by,cx,cy,dx,dy;
point2d circle;
int cirflag;
int idx;
int toggle;
int update;
int detect;
double ux,uy;
int t;
int what;
void drawSquare(point2d p)
{
    glBegin(GL_QUADS);
    {
        glVertex3d( p.x+3,  p.y+3, 0);
        glVertex3d( p.x+3,  p.y-3, 0);
        glVertex3d( p.x-3,  p.y-3, 0);
        glVertex3d( p.x-3,  p.y+3, 0);
    }
    glEnd();
}



void drawCircle(point2d p,double radius,int segments)
{
    int i;
    struct point2d points[100];
    glColor3f(0,1,1);
    //generate points
    for(i=0;i<=segments;i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi)+p.x;
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi)+p.y;
    }
    //draw segments using generated points
    for(i=0;i<segments;i++)
    {
        glBegin(GL_TRIANGLES);
        {
			glVertex3f(points[i].x,points[i].y,0);
			glVertex3f(points[i+1].x,points[i+1].y,0);
			glVertex3f(p.x,p.y,0);
        }
        glEnd();
    }
}

void keyboardListener(unsigned char key, int x,int y){
	switch(key){

        case 'a':
            if(flag==1) cirflag++;

			break;
        case 'g':
            toggle++;
			break;
        case 'u':
            update=1;
			break;
		default:
			break;
	}
}

void specialKeyListener(int key, int x,int y){
	switch(key){
		case GLUT_KEY_DOWN:		//down arrow key
		    //int i;
		    //std::cout << ax << " " << bx << " "<<cx<<" "<<dx<< std::endl;
		    //for(i=0;i<=100;i++) {

                //std::cout << hermit[i].x << " " << hermit[i].y << std::endl;
		    //}
			break;
		case GLUT_KEY_UP:		// up arrow key
			break;

		case GLUT_KEY_RIGHT:
			break;
		case GLUT_KEY_LEFT:
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
			if(state == GLUT_DOWN && flag==0 && update==0) {		// 2 times?? in ONE click? -- solution is checking DOWN or UP
                dot++;
                if(dot%2==0) {
                    //std::cout << x << " " << y << std::endl;
                    //std::cout << dot << std::endl;
                    cp[cpidx].x = (double)x;
                    cp[cpidx].y = (double)(600 - y);
                    cpidx++;
                }
                else {
                    //std::cout << x << " " << y << std::endl;
                    //std::cout << dot << std::endl;
                    cpa[cpaidx].x = (double)x;
                    cpa[cpaidx].y = (double)(600 - y);
                    cpaidx++;
                }
			}
			else if(state == GLUT_DOWN && update==1 && detect==0) {
                ux=(double)x;
                uy=(double)(600 - y);
                double dis=INFINITE;
                int i;
                for(i=0;i<cpidx;i++) {
                    if(sqrt((ux-cp[i].x)*(ux-cp[i].x)+(uy-cp[i].y)*(uy-cp[i].y))<dis) {
                        dis=sqrt((ux-cp[i].x)*(ux-cp[i].x)+(uy-cp[i].y)*(uy-cp[i].y));
                        t=i;
                        what=1;
                    }
                }

                for(i=0;i<cpaidx;i++) {
                    if(sqrt((ux-cpa[i].x)*(ux-cpa[i].x)+(uy-cpa[i].y)*(uy-cpa[i].y))<dis) {
                        dis=sqrt((ux-cpa[i].x)*(ux-cpa[i].x)+(uy-cpa[i].y)*(uy-cpa[i].y));
                        t=i;
                        what=2;
                    }
                }
                detect=1;
			}

			else if(state == GLUT_DOWN && update==1 && detect==1 && what==1) {
                //drawCircle(cp[t],10,50);
                cp[t].x=ux;
                cp[t].y=uy;
                //cout<<t<<" "<<ux<<" "<<uy<< endl;
                update=0;
                cirflag=-1;
                detect=0;
			}

			else if(state == GLUT_DOWN && update==1 && detect==1 && what==2) {
                //drawCircle(cp[t],10,50);
                cpa[t].x=ux;
                cpa[t].y=uy;
                //cout<<t<<" "<<ux<<" "<<uy<< endl;
                update=0;
                cirflag=-1;
                detect=0;
			}
			break;

		case GLUT_RIGHT_BUTTON:
			//........
			if(state == GLUT_DOWN && dot%2!=0) flag=1;
			break;

		case GLUT_MIDDLE_BUTTON:
			//........
			break;

		default:
			break;
	}
}

void drawArrow(point2d c,point2d a) {
    double dis=sqrt((c.x-a.x)*(c.x-a.x)+(c.y-a.y)*(c.y-a.y));
    //std::cout << dis << std::endl;



    point2d vec;
    vec.x=a.x-c.x;
    vec.y=a.y-c.y;

    point2d temp;
    temp.x=a.x-(.3)*vec.x;
    temp.y=a.y-(.3)*vec.y;

    glColor3f(1, 1, 1);
    glBegin(GL_LINES);
        {
			glVertex3f(c.x,c.y,0);
			glVertex3f(temp.x,temp.y,0);
        }
    glEnd();

    point2d perp;
    perp.x=-vec.y;
    perp.y=vec.x;

    point2d r,l;
    r.x=temp.x+0.05*perp.x;
    r.y=temp.y+0.05*perp.y;
    l.x=temp.x-0.05*perp.x;
    l.y=temp.y-0.05*perp.y;

    glColor3f(1,0,0);  //arrow triangle
    glBegin(GL_TRIANGLES);
    {
          glVertex3f( a.x,a.y,0);
          glVertex3f( r.x,r.y,0);
          glVertex3f( l.x,l.y,0);
    }
    glEnd();
}


void Hermit(point2d p1,point2d p4,point2d r1,point2d r4) {
    ax=(2*p1.x-2*p4.x+r1.x+r4.x);
    ay=(2*p1.y-2*p4.y+r1.y+r4.y);
    bx=(-3*p1.x+3*p4.x-2*r1.x-r4.x);
    by=(-3*p1.y+3*p4.y-2*r1.y-r4.y);
    cx=(r1.x);
    cy=(r1.y);
    dx=p1.x;
    dy=p1.y;
    double delta=0.01;
    //cout<<delta<<endl;
    double fx=dx;
    double fy=dy;
    double dfx=ax*delta*delta*delta+bx*delta*delta+cx*delta;
    double dfy=ay*delta*delta*delta+by*delta*delta+cy*delta;
    double ddfx=6*ax*delta*delta*delta+2*bx*delta*delta;
    double ddfy=6*ay*delta*delta*delta+2*by*delta*delta;
    double dddfx=6*ax*delta*delta*delta;
    double dddfy=6*ay*delta*delta*delta;
    int i;

    for(i=0;i<101;i++) {
        hermit[i].x=fx;
        hermit[i].y=fy;
        trackhermit[heridx].x=fx;
        trackhermit[heridx].y=fy;
        heridx++;
        fx=fx+dfx;
        fy=fy+dfy;
        dfx=dfx+ddfx;
        dfy=dfy+ddfy;
        ddfx=ddfx+dddfx;
        ddfy=ddfy+dddfy;
        //cout<<fx<<endl;
    }

    //double j=0;
    //for(j=0;j<99999;j=j+0.5) {}
    for(i=0;i<100;i++) {
        glColor3f(1, 1, 1);
        glLineWidth(2.0);
        glBegin(GL_LINES);
        {
			glVertex3f(hermit[i].x,hermit[i].y,0);
			glVertex3f(hermit[i+1].x,hermit[i+1].y,0);
        }
        glEnd();
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
	//gluLookAt(150*cos(cameraAngle), 150*sin(cameraAngle), cameraHeight,		0,0,0,		0,0,1);
	gluLookAt(0,0,0,	0,0,-1,	0,1,0);


	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);


	/****************************
	/ Add your objects from here
	****************************/
	//add objects


	int i;

    if(toggle%2==0) {
        for (i = 0; i < cpidx; i++)
        {
            glColor3f(0, 1, 0);
            drawSquare(cp[i]);

            glColor3f(1, 1, 0);
            drawSquare(cpa[i]);
        }
    }
    if(toggle%2==0) {
        for(i=0;i<(dot+1)/2;i++) {
            //std::cout << cp[i].x << " " << cp[i].y << std::endl;
            //std::cout << cpa[i].x << " " << cpa[i].y << std::endl;
            drawArrow(cp[i],cpa[i]);
        }
    }
    if(flag==1) {
        for(i=0;i<cpidx-1;i++) {
            point2d r1;
            point2d r4;
            r1.x=cpa[i].x-cp[i].x;
            r1.y=cpa[i].y-cp[i].y;
            r4.x=cpa[i+1].x-cp[i+1].x;
            r4.y=cpa[i+1].y-cp[i+1].y;

            Hermit(cp[i],cp[i+1],r1,r4);
            //double j=0;
            //for(j=0;j<9999999;j=j+0.1) {}
        }
        point2d lr1;
        point2d lr4;
        lr1.x=cpa[cpaidx-1].x-cp[cpidx-1].x;
        lr1.y=cpa[cpaidx-1].y-cp[cpidx-1].y;
        lr4.x=cpa[0].x-cp[0].x;
        lr4.y=cpa[0].y-cp[0].y;

        Hermit(cp[cpidx-1],cp[0],lr1,lr4);

    }
    heridx=0;

    if(cirflag%2==0 && flag==1 && update==0) drawCircle(circle,5,50);
    if(detect==1 && what==1) drawCircle(cp[t],10,50);
    if(detect==1 && what==2) drawCircle(cpa[t],10,50);
	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}


void animate(){
    if(cirflag%2==0 && flag==1) {
        double j;
        for(j=0;j<100000;j=j+0.03) {
            //for(k=0;j<10;k++) {}
        }
        circle.x=trackhermit[idx].x;
        circle.y=trackhermit[idx].y;
        idx++;
        if(idx>=((cpidx)*101)) idx=0;
    }
    else if(cirflag%2!=0) idx=0;


	//codes for any changes in Models, Camera
	glutPostRedisplay();
}

void init(){
	//codes for initialization

	cpidx = 0;
	cpaidx = 0;
    dot=-1;
    heridx=0;
    flag=0;
    ax=0;
    bx=0;
    ay=0;
    by=0;
    cx=0;
    cy=0;
    dx=0;
    dy=0;
    ux=0;
    uy=0;
    //circle.x=trackhermit[0].x;
    //circle.y=trackhermit[0].y;
    cirflag=-1;
    idx=0;
    toggle=0;
    update=0;
    detect=0;
    t=0;
    what=0;
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
	gluOrtho2D(0, 800, 0, 600);
	//gluPerspective(80,	1,	1,	1000.0);
	//field of view in the Y (vertically)
	//aspect ratio that determines the field of view in the X direction (horizontally)
	//near distance
	//far distance
}

int main(int argc, char **argv){
	glutInit(&argc,argv);
	glutInitWindowSize(800, 600);
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
