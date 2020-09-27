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
	double x,y,z;
};

point pos,u,r,l;
double a,b,c;
double rot1,rot2,rot3,rot4;



void drawAxes()
{
	if(drawaxes==1)
	{
        glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_LINES);{
		    //glColor3f(1.0, 1.0, 1.0);
			glVertex3f( 350,0,0);
			glVertex3f(-300,0,0);

            //glColor3f(1.0, 0, 0);
			glVertex3f(0,-300,0);
			glVertex3f(0, 300,0);

            //glColor3f(0, 1, 0);
			glVertex3f(0,0, 400);
			glVertex3f(0,0,-400);
		}glEnd();
	}
}


void drawGrid()
{
	int i;
	if(drawgrid==1)
	{
		glColor3f(0.6, 0.6, 0.6);	//grey
		glBegin(GL_LINES);{
			for(i=-8;i<=8;i++){

				if(i==0)
					continue;	//SKIP the MAIN axes

				//lines parallel to Y-axis
				glVertex3f(i*10, -90, 0);
				glVertex3f(i*10,  90, 0);

				//lines parallel to X-axis
				glVertex3f(-90, i*10, 0);
				glVertex3f( 90, i*10, 0);
			}
		}glEnd();
	}
}

void drawSquare(double a)
{
    //glColor3f(1.0,0.0,0.0);
	glBegin(GL_QUADS);{
		glVertex3f( a, -800,a);
		glVertex3f( a,-800,-a);
		glVertex3f(-a,-800,-a);
		glVertex3f(-a, -800,a);
	}glEnd();
}


void drawCircle(double radius,int segments)
{
    int i;
    struct point points[100];
    glColor3f(0.7,0.7,0.7);
    //generate points
    for(i=0;i<=segments;i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
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

void drawCone(double radius,double height,int segments)
{
    int i;
    double shade;
    struct point points[100];
    //generate points
    for(i=0;i<=segments;i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
    }
    //draw triangles using generated points
    for(i=0;i<segments;i++)
    {
        //create shading effect
        if(i<segments/2)shade=2*(double)i/(double)segments;
        else shade=2*(1.0-(double)i/(double)segments);
        glColor3f(shade,shade,shade);

        glBegin(GL_TRIANGLES);
        {
            glVertex3f(0,0,height);
			glVertex3f(points[i].x,points[i].y,0);
			glVertex3f(points[i+1].x,points[i+1].y,0);
        }
        glEnd();
    }
}


void drawSphere(double radius,int slices,int stacks)
{
	struct point points[100][100];
	int i,j;
	double h,r;
	//generate points
	for(i=0;i<=stacks;i++)
	{
		h=radius*sin(((double)i/(double)stacks)*(pi/2));
		r=radius*cos(((double)i/(double)stacks)*(pi/2));
		for(j=0;j<=slices;j++)
		{
			points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
			points[i][j].y=r*sin(((double)j/(double)slices)*2*pi)-radius;
			points[i][j].z=h;
		}
	}
	//draw quads using generated points
	for(i=0;i<stacks;i++)
	{
        glColor3f((double)i/(double)stacks,(double)i/(double)stacks,(double)i/(double)stacks);
		for(j=0;j<slices;j++)
		{
			glBegin(GL_QUADS);{
			    //upper hemisphere
				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                //lower hemisphere
                glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
			}glEnd();
		}
	}
}

void drawCapsul(double radius,int slices,int stacks)
{
	point points[100][100];
	int i,j;
	double h,r;
	//generate points
	for(i=0;i<=stacks;i++)
	{
		h=radius*sin(((double)i/(double)stacks)*(pi/2));
		r=radius*cos(((double)i/(double)stacks)*(pi/2));
		for(j=0;j<=slices;j++)
		{
			points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
			points[i][j].y=h-radius;
			points[i][j].z=r*sin(((double)j/(double)slices)*2*pi);
		}
	}
	//draw quads using generated points
	for(i=0;i<stacks;i++)
	{
        glColor3f((double)i/(double)stacks,(double)i/(double)stacks,(double)i/(double)stacks);
		for(j=0;j<slices;j++)
		{
		    if(j%2==0){
                glColor3f(1.0,1.0,1.0);
		    }
		    else {
                glColor3f(0.3,0.3,0.3);
		    }
			glBegin(GL_QUADS);{
			    //upper hemisphere
				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                //lower hemisphere
                //glVertex3f(points[i][j].x,-points[i][j].y,points[i][j].z);
				//glVertex3f(points[i][j+1].x,-points[i][j+1].y,points[i][j+1].z);
				//glVertex3f(points[i+1][j+1].x,-points[i+1][j+1].y,-points[i+1][j+1].z);
				//glVertex3f(points[i+1][j].x,-points[i+1][j].y,-points[i+1][j].z);
			}glEnd();
		}
	}



	point points2[100][100];

	//generate points
	for(i=0;i<=stacks;i++)
	{
		h=radius*sin(((double)i/(double)stacks)*(pi/2));
		r=radius*cos(((double)i/(double)stacks)*(pi/2));
		for(j=0;j<=slices;j++)
		{
			points2[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
			points2[i][j].y=-(h+60);
			points2[i][j].z=r*sin(((double)j/(double)slices)*2*pi);
		}
	}
	//draw quads using generated points
	for(i=0;i<stacks;i++)
	{
        glColor3f((double)i/(double)stacks,(double)i/(double)stacks,(double)i/(double)stacks);
		for(j=0;j<slices;j++)
		{
		    if(j%2==0){
                glColor3f(1.0,1.0,1.0);
		    }
		    else {
                glColor3f(0.3,0.3,0.3);
		    }
			glBegin(GL_QUADS);{
			    //upper hemisphere
				glVertex3f(points2[i][j].x,points2[i][j].y,points2[i][j].z);
				glVertex3f(points2[i][j+1].x,points2[i][j+1].y,points2[i][j+1].z);
				glVertex3f(points2[i+1][j+1].x,points2[i+1][j+1].y,points2[i+1][j+1].z);
				glVertex3f(points2[i+1][j].x,points2[i+1][j].y,points2[i+1][j].z);
                //lower hemisphere
                //glVertex3f(points2[i][j].x,points2[i][j].y,-points2[i][j].z);
				//glVertex3f(points2[i][j+1].x,points2[i][j+1].y,-points2[i][j+1].z);
				//glVertex3f(points2[i+1][j+1].x,points2[i+1][j+1].y,-points2[i+1][j+1].z);
				//glVertex3f(points2[i+1][j].x,points2[i+1][j].y,-points2[i+1][j].z);
			}glEnd();
		}
	}

	for(j=0;j<slices;j++)
		{
		    if(j%2==0){
                glColor3f(1.0,1.0,1.0);
		    }
		    else {
                glColor3f(0.3,0.3,0.3);
		    }
			glBegin(GL_QUADS);{
			    //upper hemisphere
				glVertex3f(points[0][j].x,points[0][j].y,points[0][j].z);
				glVertex3f(points[0][j+1].x,points[0][j+1].y,points[0][j+1].z);

				glVertex3f(points2[0][j+1].x,points2[0][j+1].y,points2[0][j+1].z);
				glVertex3f(points2[0][j].x,points2[0][j].y,points2[0][j].z);

				//glVertex3f(points[0][j+1].x,points[0][j+1].y,points[0][j+1].z);
				//glVertex3f(points2[0][j+1].x,points2[0][j+1].y,points2[0][j+1].z);

				//glVertex3f(points[0][j].x,points[0][j].y,points[0][j].z);
				//glVertex3f(points2[0][j].x,points2[0][j].y,points2[0][j].z);
                //lower hemisphere
                //glVertex3f(points[i][j].x,-points[i][j].y,points[i][j].z);
				//glVertex3f(points[i][j+1].x,-points[i][j+1].y,points[i][j+1].z);
				//glVertex3f(points[i+1][j+1].x,-points[i+1][j+1].y,-points[i+1][j+1].z);
				//glVertex3f(points[i+1][j].x,-points[i+1][j].y,-points[i+1][j].z);
			}glEnd();
		}
}


void drawHead(double radius,int slices,int stacks)
{
	point points3[100][100],points5[100][100];
	int i,j;
	double h,r,r2;
	//generate points
	for(i=0;i<=stacks;i++)
	{
		h=radius*sin(((double)i/(double)stacks)*(pi/2));
		r=radius*cos(((double)i/(double)stacks)*(pi/2));
		r2=2*radius-r;
		for(j=0;j<=slices;j++)
		{
			points3[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
			points3[i][j].y=h;
			points3[i][j].z=r*sin(((double)j/(double)slices)*2*pi);

			points5[i][j].x=r2*cos(((double)j/(double)slices)*2*pi);
			points5[i][j].y=-(h+100);
			points5[i][j].z=r2*sin(((double)j/(double)slices)*2*pi);

		}
	}
	//draw quads using generated points
	for(i=0;i<stacks;i++)
	{
        glColor3f((double)i/(double)stacks,(double)i/(double)stacks,(double)i/(double)stacks);
		for(j=0;j<slices;j++)
		{
		    if(j%2==0){
                glColor3f(1.0,1.0,1.0);
		    }
		    else {
                glColor3f(0.3,0.3,0.3);
		    }
			glBegin(GL_QUADS);{
			    //upper hemisphere
				glVertex3f(points3[i][j].x,points3[i][j].y,points3[i][j].z);
				glVertex3f(points3[i][j+1].x,points3[i][j+1].y,points3[i][j+1].z);
				glVertex3f(points3[i+1][j+1].x,points3[i+1][j+1].y,points3[i+1][j+1].z);
				glVertex3f(points3[i+1][j].x,points3[i+1][j].y,points3[i+1][j].z);
                //lower hemisphere
                //glVertex3f(points[i][j].x,-points[i][j].y,points[i][j].z);
				//glVertex3f(points[i][j+1].x,-points[i][j+1].y,points[i][j+1].z);
				//glVertex3f(points[i+1][j+1].x,-points[i+1][j+1].y,-points[i+1][j+1].z);
				//glVertex3f(points[i+1][j].x,-points[i+1][j].y,-points[i+1][j].z);
			}glEnd();
		}
	}



	point points4[100][100];

	//generate points
	for(i=0;i<=stacks;i++)
	{
		h=radius*sin(((double)i/(double)stacks)*(pi/2));
		r=radius*cos(((double)i/(double)stacks)*(pi/2));
		for(j=0;j<=slices;j++)
		{
			points4[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
			points4[i][j].y=-(h+100);
			points4[i][j].z=r*sin(((double)j/(double)slices)*2*pi);
		}
	}


	for(j=0;j<slices;j++)
    {
		    if(j%2==0){
                glColor3f(1.0,1.0,1.0);
		    }
		    else {
                glColor3f(0.3,0.3,0.3);
		    }
			glBegin(GL_QUADS);{
			    //upper hemisphere
				glVertex3f(points3[0][j].x,points3[0][j].y,points3[0][j].z);
				glVertex3f(points3[0][j+1].x,points3[0][j+1].y,points3[0][j+1].z);

				glVertex3f(points4[0][j+1].x,points4[0][j+1].y,points4[0][j+1].z);
				glVertex3f(points4[0][j].x,points4[0][j].y,points4[0][j].z);

				//glVertex3f(points[0][j+1].x,points[0][j+1].y,points[0][j+1].z);
				//glVertex3f(points2[0][j+1].x,points2[0][j+1].y,points2[0][j+1].z);

				//glVertex3f(points[0][j].x,points[0][j].y,points[0][j].z);
				//glVertex3f(points2[0][j].x,points2[0][j].y,points2[0][j].z);
                //lower hemisphere
                //glVertex3f(points[i][j].x,-points[i][j].y,points[i][j].z);
				//glVertex3f(points[i][j+1].x,-points[i][j+1].y,points[i][j+1].z);
				//glVertex3f(points[i+1][j+1].x,-points[i+1][j+1].y,-points[i+1][j+1].z);
				//glVertex3f(points[i+1][j].x,-points[i+1][j].y,-points[i+1][j].z);
			}glEnd();
    }



	//draw quads using generated points
	for(i=0;i<stacks;i++)
	{
        glColor3f((double)i/(double)stacks,(double)i/(double)stacks,(double)i/(double)stacks);
		for(j=0;j<slices;j++)
		{
		    if(j%2==0){
                glColor3f(1.0,1.0,1.0);
		    }
		    else {
                glColor3f(0.3,0.3,0.3);
		    }
			glBegin(GL_QUADS);{
			    //upper hemisphere
				glVertex3f(points5[i][j].x,points5[i][j].y,points5[i][j].z);
				glVertex3f(points5[i][j+1].x,points5[i][j+1].y,points5[i][j+1].z);
				glVertex3f(points5[i+1][j+1].x,points5[i+1][j+1].y,points5[i+1][j+1].z);
				glVertex3f(points5[i+1][j].x,points5[i+1][j].y,points5[i+1][j].z);
                //lower hemisphere
                //glVertex3f(points[i][j].x,-points[i][j].y,points[i][j].z);
				//glVertex3f(points[i][j+1].x,-points[i][j+1].y,points[i][j+1].z);
				//glVertex3f(points[i+1][j+1].x,-points[i+1][j+1].y,-points[i+1][j+1].z);
				//glVertex3f(points[i+1][j].x,-points[i+1][j].y,-points[i+1][j].z);
			}glEnd();
		}
	}
}

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

      drawAxes();
      glColor3f(0.5,0.5,0.5);
      drawSquare(220);
      glRotatef(rot1,0,0,1);
      glRotatef(rot2,1,0,0);

      drawCapsul(8,24,20);
      glTranslatef(0,-74,0);

      glRotatef(rot3,1,0,0);
      glRotatef(rot4,0,1,0);
      drawHead(6,24,20);
}

void keyboardListener(unsigned char key, int x,int y){
	switch(key){

		case '1':
			//drawgrid=1-drawgrid;
			a=l.x;
			b=l.y;
			c=l.z;
			l.x=l.x*cos(0.05)-r.x*sin(0.05);
			r.x=r.x*cos(0.05)+a*sin(0.05);
			l.y=l.y*cos(0.05)-r.y*sin(0.05);
			r.y=r.y*cos(0.05)+b*sin(0.05);
			l.z=l.z*cos(0.05)-r.z*sin(0.05);
			r.z=r.z*cos(0.05)+c*sin(0.05);
			break;

        case '2':
			//drawgrid=1-drawgrid;
			a=l.x;
			b=l.y;
			c=l.z;
			l.x=l.x*cos(0.05)+r.x*sin(0.05);
			r.x=r.x*cos(0.05)-a*sin(0.05);
			l.y=l.y*cos(0.05)+r.y*sin(0.05);
			r.y=r.y*cos(0.05)-b*sin(0.05);
			l.z=l.z*cos(0.05)+r.z*sin(0.05);
			r.z=r.z*cos(0.05)-c*sin(0.05);
			break;

        case '3':
			//drawgrid=1-drawgrid;
			a=l.x;
			b=l.y;
			c=l.z;
			l.x=l.x*cos(0.05)+u.x*sin(0.05);
			u.x=u.x*cos(0.05)-a*sin(0.05);
			l.y=l.y*cos(0.05)+u.y*sin(0.05);
			u.y=u.y*cos(0.05)-b*sin(0.05);
			l.z=l.z*cos(0.05)+u.z*sin(0.05);
			u.z=u.z*cos(0.05)-c*sin(0.05);
			break;

        case '4':
			//drawgrid=1-drawgrid;
			a=l.x;
			b=l.y;
			c=l.z;
			l.x=l.x*cos(0.05)-u.x*sin(0.05);
			u.x=u.x*cos(0.05)+a*sin(0.05);
			l.y=l.y*cos(0.05)-u.y*sin(0.05);
			u.y=u.y*cos(0.05)+b*sin(0.05);
			l.z=l.z*cos(0.05)-u.z*sin(0.05);
			u.z=u.z*cos(0.05)+c*sin(0.05);
			break;

        case '5':
			//drawgrid=1-drawgrid;
			a=r.x;
			b=r.y;
			c=r.z;
			r.x=r.x*cos(0.05)-u.x*sin(0.05);
			u.x=u.x*cos(0.05)+a*sin(0.05);
			r.y=r.y*cos(0.05)-u.y*sin(0.05);
			u.y=u.y*cos(0.05)+b*sin(0.05);
			r.z=r.z*cos(0.05)-u.z*sin(0.05);
			u.z=u.z*cos(0.05)+c*sin(0.05);
			break;

		 case '6':
			//drawgrid=1-drawgrid;
			a=r.x;
			b=r.y;
			c=r.z;
			r.x=r.x*cos(0.05)+u.x*sin(0.05);
			u.x=u.x*cos(0.05)-a*sin(0.05);
			r.y=r.y*cos(0.05)+u.y*sin(0.05);
			u.y=u.y*cos(0.05)-b*sin(0.05);
			r.z=r.z*cos(0.05)+u.z*sin(0.05);
			u.z=u.z*cos(0.05)-c*sin(0.05);
			break;

         case 'q':
            if(rot1<70) rot1+=0.5;
            break;

        case 'w':
            if(rot1>-70) rot1-=0.5;
            break;

        case 'e':
            if(rot2>-70) rot2-=0.5;
            break;

        case 'r':
            if(rot2<70) rot2+=0.5;
            break;

        case 'a':
            if(rot3>-70) rot3-=0.5;
            break;

        case 's':
            if(rot3<70) rot3+=0.5;
            break;

        case 'd':
             if(rot4>-70) rot4-=0.5;
            break;

         case 'f':
            if(rot4<70) rot4+=0.5;
            break;

		default:
			break;
	}
}


void specialKeyListener(int key, int x,int y){
	switch(key){
		case GLUT_KEY_DOWN:		//down arrow key
			//cameraHeight -= 3.0;
			pos.y+=2;
			break;
		case GLUT_KEY_UP:		// up arrow key
			//cameraHeight += 3.0;
			pos.y-=2;
			break;

		case GLUT_KEY_RIGHT:
			//cameraAngle += 0.03;
			pos.x-=2;
			break;
		case GLUT_KEY_LEFT:
			//cameraAngle -= 0.03;
			pos.x+=2;
			break;

		case GLUT_KEY_PAGE_UP:
		    pos.z+=2;
			break;
		case GLUT_KEY_PAGE_DOWN:
		    pos.z-=2;
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

	//gluLookAt(50,50,50,	10,10,0,	0,0,1);
	//gluLookAt(200*cos(cameraAngle), 200*sin(cameraAngle), cameraHeight,		0,0,0,		0,0,1);
	gluLookAt(pos.x,pos.y,pos.z,	pos.x+l.x,pos.y+l.y,pos.z+l.z,	u.x,u.y,u.z);


	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);


	/****************************
	/ Add your objects from here
	****************************/
	//add objects


	//drawGrid();

    //glColor3f(1,0,0);
    //drawSquare(10);

    drawSS();

    //drawCircle(30,24);

    //drawCone(20,50,24);






	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}


void animate(){
	//angle+=0.05;
	//codes for any changes in Models, Camera
	glutPostRedisplay();
}

void init(){
	//codes for initialization
	pos.x=30;
	pos.y=100;
	pos.z=45;

	u.x=0;
	u.y=0;
	u.z=1;

	r.x=-1;
	r.y=0;
	r.z=0;

	l.x=0;
	l.y=-1;
	l.z=0;

    rot1=0;
    rot2=0;
    rot3=0;
    rot4=0;

	drawgrid=0;
	drawaxes=1;
	cameraHeight=150.0;
	cameraAngle=1.0;
	angle=0;


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
	glutInitWindowSize(700, 600);
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
