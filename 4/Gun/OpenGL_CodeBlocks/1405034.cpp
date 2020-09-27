#include<bits/stdc++.h>
using namespace std;
#include<stdlib.h>
#include<math.h>
#include "bitmap_image.hpp"

#include <windows.h>
#include <glut.h>

#define pi (2*acos(0.0))

double cameraHeight;
double cameraAngle;
int drawgrid;
int drawaxes;
double angle;
int texture=0;
int tH,tW;

typedef struct point
{
	double x,y,z;
};

class color {
public:
    double r, g, b;
    color(double r, double g, double b) {
        this->r = r;
        this->g = g;
        this->b = b;
    }
    color() {
    }
};

point normalize(point p) {
    point pp;
    double q=p.x*p.x+p.y*p.y+p.z*p.z;
    pp.x=p.x/q;
    pp.y=p.y/q;
    pp.z=p.z/q;

    return pp;
}

double minn(double x,double y,double z) {
    if(x<=y) {
        if (x<=z) {
            return x;
        }
        else return z;
    }
    else {
        if (y<=z) {
            return y;
        }
        else return z;
    }
}

point pos,u,r,l;
double a,b,c;
double rot1,rot2,rot3,rot4;

double ner,fr,fovy,aspect;
int recursion;
int pixels;

int objects;
string obj;

color **textpixel;

class sphere {
public:
    point center;
    double radius;
    double r,g,b;
    double am,dif,spec,refl,shin;
    sphere() {
    }
    sphere(point center,double radius,double r,double g,double b) {
        this->center=center;
        this->radius=radius;
        this->r=r;
        this->g=g;
        this->b=b;

    }
    double intersection(point p,point ray) {
        double x1,y1,z1;
        double aa=ray.x*ray.x+ray.y*ray.y+ray.z*ray.z;
        double bb=2*((ray.x*(p.x-center.x))+(ray.y*(p.y-center.y))+(ray.z*(p.z-center.z)));
        double cc=center.x*center.x+center.y*center.y+center.z*center.z+p.x*p.x+p.y*p.y+p.z*p.z-2*(center.x*p.x+center.y*p.y+center.z*p.z)-radius*radius;
        double t;
        if((bb*bb-4*aa*cc)>0) {
            double t1=(-bb+sqrt(bb*bb-4*aa*cc))/(2*aa);
            double t2=(-bb-sqrt(bb*bb-4*aa*cc))/(2*aa);

            if(t1<0 && t2<0) {
                x1=-1000;
                y1=-1000;
                z1=-1000;
                t=10000;
            }
            else if(t1>0 && t2>0) {
                if(t1>t2) t=t2;
                else t=t1;

                x1=p.x+ray.x*t;
                y1=p.y+ray.y*t;
                z1=p.z+ray.z*t;
            }
            else if(t1<0 && t2>0) {
                t=t2;
                x1=p.x+ray.x*t;
                y1=p.y+ray.y*t;
                z1=p.z+ray.z*t;
            }
            else if(t1>0 && t2<0) {
                t=t1;
                x1=p.x+ray.x*t;
                y1=p.y+ray.y*t;
                z1=p.z+ray.z*t;
            }

        }
        else {
            x1=-1000;
            y1=-1000;
            z1=-1000;
            t=10000;
        }
        point inter;
        inter.x=x1;
        inter.y=y1;
        inter.z=z1;
        //cout<<inter.x<<endl;
        //return inter;
        return t;


    }
};

class pyramid {
public:
    point lowest;
    double width,height;
    double r,g,b;
    double am,dif,spec,refl,shin;
    pyramid() {
    }
    pyramid(point lowest,double width,double height,double r,double g,double b) {
        this->lowest=lowest;
        this->width=width;
        this->height=height;
        this->r=r;
        this->g=g;
        this->b=b;

    }
};
class light{
public:
    point position;
    double falloff;
    point look;
    double ang;
    light() {
    }
    light(point position,double falloff) {
        this->position=position;
        this->falloff=falloff;
    }
};

class checkerboard{
public:

    double check_width;
    double ambient,diffuse,reflection;
    checkerboard() {
    }
    checkerboard(double check_width,double ambient,double diffuse,double reflection) {
        this->check_width=check_width;
        this->ambient=ambient;
        this->diffuse=diffuse;
        this->reflection=reflection;
    }

    void drawCheckerBoard(double check_width) {
        for (int i = -200; i < 200 ; ++i) {
            for (int j = -200; j < 200; ++j) {
                if((i + j)%2 == 0) // if i + j is even
                    glColor3f( 1.0,1.0, 1.0);
                else
                    glColor3f( 0.0, 0.0, 0.0 );
                glRecti(i*check_width, j*check_width, (i+1)*check_width, (j+1)*check_width);    // draw the rectangle
            }
        }
    }

    double intersection(point p,point ray) {
        double t=(-p.z)/(ray.z+0.0000001);
        point inter;
        if(t>0) {
            inter.x=p.x+t*ray.x;
            inter.y=p.y+t*ray.y;
            inter.z=p.z+t*ray.z;
            return t;
        }
        else {
            inter.x=-1000;
            inter.y=-1000;
            inter.z=-1000;
            t=10000;
        }

        //return inter;
        return t;
    }

    point for_tex(point p,point ray) {
        double t=(-p.z)/(ray.z+0.0000001);
        point inter;
        if(t>0) {
            inter.x=p.x+t*ray.x;
            inter.y=p.y+t*ray.y;
            inter.z=p.z+t*ray.z;
        }
        else {
            inter.x=-1000;
            inter.y=-1000;
            inter.z=-1000;
        }

        return inter;
    }

};

sphere s1,s2;
pyramid p;
checkerboard cb;
light normal,spot;

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

void drawPyramid(pyramid p) {
    glColor3f(p.r,p.g,p.b);

    glBegin(GL_QUADS);{
		glVertex3f(p.lowest.x,p.lowest.y,p.lowest.z);
		glVertex3f(p.lowest.x,(p.lowest.y+p.width),p.lowest.z);
		glVertex3f((p.lowest.x+p.width),p.lowest.y,p.lowest.z);
		glVertex3f((p.lowest.x+p.width),(p.lowest.y+p.width),p.lowest.z);

	}glEnd();

	point h;
	h.x=p.lowest.x+p.width/2;
	h.y=p.lowest.y+p.width/2;
	h.z=p.lowest.z+p.height;

	glBegin(GL_TRIANGLES);
        {
            glVertex3f(p.lowest.x,p.lowest.y,p.lowest.z);
			glVertex3f(p.lowest.x,(p.lowest.y+p.width),p.lowest.z);
			glVertex3f(h.x,h.y,h.z);

			glVertex3f(p.lowest.x,p.lowest.y,p.lowest.z);
			glVertex3f((p.lowest.x+p.width),(p.lowest.y+p.width),p.lowest.z);
			glVertex3f(h.x,h.y,h.z);

			glVertex3f(p.lowest.x,(p.lowest.y+p.width),p.lowest.z);
			glVertex3f((p.lowest.x+p.width),(p.lowest.y+p.width),p.lowest.z);
			glVertex3f(h.x,h.y,h.z);

			glVertex3f((p.lowest.x+p.width),p.lowest.y,p.lowest.z);
			glVertex3f((p.lowest.x+p.width),(p.lowest.y+p.width),p.lowest.z);
			glVertex3f(h.x,h.y,h.z);


        }
        glEnd();


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


void drawSphere(point center,double radius,double rr,double g,double b,int slices,int stacks)
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
			points[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
			points[i][j].z=h;
		}
	}
	//draw quads using generated points
	for(i=0;i<stacks;i++)
	{
        glColor3f(rr,g,b);
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

point pointBuffer[1000][1000];
void ray() {
    cout<<"hello"<<endl;
    color** pixel = new color*[pixels];
    for (int i = 0; i < pixels; i++) {
        pixel[i] = new color [pixels];
        for (int j = 0; j < pixels; j++) {
            pixel[i][j].r =0.0;
            pixel[i][j].g =0.0;
            pixel[i][j].b =0.0;
        }
    }


    int mid_i=pixels/2;
    int mid_j=pixels/2;
    //cout<<mid_i;
    l=normalize(l);
    u=normalize(u);
    r=normalize(r);

    pointBuffer[mid_i][mid_j].x=pos.x+ner*l.x;
    pointBuffer[mid_i][mid_j].y=pos.y+ner*l.y;
    pointBuffer[mid_i][mid_j].z=pos.z+ner*l.z;
    cout<<ner<<endl;
    cout<<l.x<<" "<<l.y<<" "<<l.z<<endl;
    cout<<pointBuffer[mid_i][mid_j].x<<" "<<pointBuffer[mid_i][mid_j].y<<" "<<pointBuffer[mid_i][mid_j].z<<endl;
    double vx,vy;
    vy=ner*tan((fovy/2)*pi/180);
    double fovx=aspect*fovy;
    vx=ner*tan((fovx/2)*pi/180);

    pointBuffer[0][0].x=pointBuffer[mid_i][mid_j].x+vy*u.x-r.x*vx;
    pointBuffer[0][0].y=pointBuffer[mid_i][mid_j].y+vy*u.y-r.y*vx;
    pointBuffer[0][0].z=pointBuffer[mid_i][mid_j].z+vy*u.z-r.z*vx;

    for (int j=1;j<pixels;j++) {
        pointBuffer[j][0].x=pointBuffer[j-1][0].x+(r.x*vx)/(pixels/2);
        pointBuffer[j][0].y=pointBuffer[j-1][0].y+(r.y*vx)/(pixels/2);
        pointBuffer[j][0].z=pointBuffer[j-1][0].z+(r.z*vx)/(pixels/2);
    }

    for (int i=1;i<pixels;i++) {
        pointBuffer[0][i].x=pointBuffer[0][i-1].x-(u.x*vy)/(pixels/2);
        pointBuffer[0][i].y=pointBuffer[0][i-1].y-(u.y*vy)/(pixels/2);
        pointBuffer[0][i].z=pointBuffer[0][i-1].z-(u.z*vy)/(pixels/2);
        for (int j=1;j<pixels;j++) {
            pointBuffer[j][i].x=pointBuffer[j-1][i].x+(r.x*vx)/(pixels/2);
            pointBuffer[j][i].y=pointBuffer[j-1][i].y+(r.y*vx)/(pixels/2);
            pointBuffer[j][i].z=pointBuffer[j-1][i].z+(r.z*vx)/(pixels/2);
        }
    }
//    for(int i=0;i<10;i++) {
//        for (int j=0;j<10;j++) {
//            cout<<pointBuffer[i][j].x<<" ";
//        }
//        cout<<endl;
//    }
    for (int i=0;i<pixels;i++) {
        for (int j=0;j<pixels;j++) {
            point ray;
            ray.x=pointBuffer[i][j].x-pos.x;
            ray.y=pointBuffer[i][j].y-pos.y;
            ray.z=pointBuffer[i][j].z-pos.z;

            double ts1=s1.intersection(pointBuffer[i][j],ray);
            double ts2=s2.intersection(pointBuffer[i][j],ray);
            double tcb=cb.intersection(pointBuffer[i][j],ray);

            //double ts1=1000;
            //double ts2=1000;
            //double tcb=1000;

            //if(inter1.x!=-1000 && inter1.y!=-1000 && inter1.z!=-1000) {
                //ts1=(inter1.x-pointBuffer[i][j].x)/ray.x;
            //}
            //if(inter2.x!=-1000 && inter2.y!=-1000 && inter2.z!=-1000) {
                //ts2=(inter2.x-pointBuffer[i][j].x)/ray.x;
            //}
            //if(inter3.x!=-1000 && inter3.y!=-1000 && inter3.z!=-1000) {
                //tcb=(-pointBuffer[i][j].z)/(ray.z+0.0000001);
            //}
            point inter3;
            inter3.x=pointBuffer[i][j].x+tcb*ray.x;
            inter3.y=pointBuffer[i][j].y+tcb*ray.y;
            inter3.z=pointBuffer[i][j].z+tcb*ray.z;

            double mm=minn(ts1,ts2,tcb);
            if(mm==ts1 && mm!=10000) {
                pixel[i][j].r=s1.r*255;
                pixel[i][j].g=s1.g*255;
                pixel[i][j].b=s1.b*255;
            }
            else if(mm==ts2 && mm!=10000) {
                pixel[i][j].r=s2.r*255;
                pixel[i][j].g=s2.g*255;
                pixel[i][j].b=s2.b*255;
            }
            else if(mm==tcb && mm!=10000) {
                int m=inter3.x/cb.check_width;
                int n=inter3.y/cb.check_width;
                if(inter3.x<0) m--;
                if(inter3.y<0) n--;

                if(texture%2==0) {
                    if((m+n)%2==0) {
                        pixel[i][j].r=255;
                        pixel[i][j].g=255;
                        pixel[i][j].b=255;
                    }
                    else {
                        pixel[i][j].r=0;
                        pixel[i][j].g=0;
                        pixel[i][j].b=0;
                    }
                }
                if(texture%2==1) {
                    double tx,ty,dx,dy;
                    tx=m*cb.check_width;
                    ty=(n+1)*cb.check_width;

                    dx=inter3.x-tx;
                    dy=ty-inter3.y;
                    int xc,yc;
                    //cout<<dx<<" "<<dy<<endl;
                    xc=(tW/cb.check_width)*dx;
                    yc=(tH/cb.check_width)*dy;
                    pixel[i][j].r=textpixel[yc][xc].r*255;
                    pixel[i][j].g=textpixel[yc][xc].g*255;
                    pixel[i][j].b=textpixel[yc][xc].b*255;
                    //cout<<xc<<" "<<yc<<endl<<endl;
                }
            }

        }

    }
    cout<<"sesh"<<endl;

    bitmap_image image(pixels, pixels);
    for (int i = 0; i < pixels; i++) {
        for (int j = 0; j < pixels; j++) {
            image.set_pixel(i, j, pixel[i][j].r, pixel[i][j].g, pixel[i][j].b);
        }
    }
    image.save_image("out.bmp");

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

//      drawAxes();
//      glColor3f(0.5,0.5,0.5);
//      drawSquare(220);
//      glRotatef(rot1,0,0,1);
//      glRotatef(rot2,1,0,0);
//
//      drawCapsul(8,24,20);
//      glTranslatef(0,-74,0);
//
//      glRotatef(rot3,1,0,0);
//      glRotatef(rot4,0,1,0);
//      drawHead(6,24,20);
}

void keyboardListener(unsigned char key, int x,int y){
	switch(key){

		case '1':
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

        case '2':
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
			r.x=r.x*cos(0.05)+u.x*sin(0.05);
			u.x=u.x*cos(0.05)-a*sin(0.05);
			r.y=r.y*cos(0.05)+u.y*sin(0.05);
			u.y=u.y*cos(0.05)-b*sin(0.05);
			r.z=r.z*cos(0.05)+u.z*sin(0.05);
			u.z=u.z*cos(0.05)-c*sin(0.05);
			break;

		 case '6':
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

         case '0':
             ray();
             break;
         case ' ':
             texture++;
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
	if(texture%2==0) cb.drawCheckerBoard(cb.check_width);
	glPushMatrix();
	{
	    glTranslatef(s1.center.x,s1.center.y,s1.center.z);
        drawSphere(s1.center,s1.radius,s1.r,s1.g,s1.b,90,24);
	} glPopMatrix();

    glPushMatrix();
	{
	    glTranslatef(s2.center.x,s2.center.y,s2.center.z);
        drawSphere(s2.center,s2.radius,s2.r,s2.g,s2.b,90,24);
	} glPopMatrix();

	drawPyramid(p);

	glPushMatrix();
	{
	    glTranslatef(normal.position.x,normal.position.y,normal.position.z);
        drawSphere(normal.position,5,1.0,1.0,1.0,90,24);
	} glPopMatrix();

	glPushMatrix();
	{
	    glTranslatef(spot.position.x,spot.position.y,spot.position.z);
        drawSphere(spot.position,5,1.0,1.0,1.0,90,24);
	} glPopMatrix();





    //glColor3f(1,0,0);
    //drawSquare(10);

    //drawSS();

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

	drawgrid=1;
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
	gluPerspective(fovy,	1,	1,	1000.0);
	//field of view in the Y (vertically)
	//aspect ratio that determines the field of view in the X direction (horizontally)
	//near distance
	//far distance
}

int main(int argc, char **argv){
    ifstream input;
    input.open("description.txt");
    input>>ner>>fr>>fovy>>aspect;
    input>>recursion;
    input>>pixels;
    input>>cb.check_width;
    input>>cb.ambient>>cb.diffuse>>cb.reflection;
    input>>objects;
    input>>obj;
    input>>s1.center.x>>s1.center.y>>s1.center.z;
    input>>s1.radius;
    input>>s1.r>>s1.g>>s1.b;
    input>>s1.am>>s1.dif>>s1.spec>>s1.refl>>s1.shin;

    input>>obj;
    input>>p.lowest.x>>p.lowest.y>>p.lowest.z;
    input>>p.width>>p.height;
    input>>p.r>>p.g>>p.b;
    input>>p.am>>p.dif>>p.spec>>p.refl>>p.shin;

    input>>obj;
    input>>s2.center.x>>s2.center.y>>s2.center.z;
    input>>s2.radius;
    input>>s2.r>>s2.g>>s2.b;
    input>>s2.am>>s2.dif>>s2.spec>>s2.refl>>s2.shin;

    double dx;
    input>>dx;
    input>>normal.position.x>>normal.position.y>>normal.position.z>>normal.falloff;

    input>>dx;
    input>>spot.position.x>>spot.position.y>>spot.position.z>>spot.falloff;
    input>>spot.look.x>>spot.look.y>>spot.look.z;
    input>>spot.ang;

    bitmap_image text ("texture.bmp");
    tH = text.height();
    tW = text.width();

    textpixel = new color*[tW];
    for (int i = 0; i < tW; i++) {
        textpixel[i] = new color [tH];
        for (int j = 0; j < tH; j++) {
            unsigned char r, g, b;
            text.get_pixel(i, j, r, g, b);
            color col(r/255.0, g/255.0, b/255.0);
            textpixel[i][j] = col;
            //cout<<col.r<<" "<<col.g<<" "<<col.b<<endl;
        }
    }

	glutInit(&argc,argv);
	glutInitWindowSize(pixels, pixels);
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
