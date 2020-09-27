#include<bits/stdc++.h>
#include <iostream>
#include <fstream>
#include <cassert>
#include <iomanip>
#include <cmath>
#include <stack>
#include <queue>
#include "bitmap_image.hpp"
using namespace std;

#define pi (2*acos(0.0))
#define epsilon (1.0e-6)

class push_pop {
public:
    int pn;
    push_pop(int a) {
        pn=a;
    }
    push_pop() {
    }
};
class homogeneous_point
{
public:
    double x, y, z, w;

    // set the three coordinates, set w to 1
    homogeneous_point(double x, double y, double z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
        this->w = 1;
    }

    /*
    default constructor. does nothing. allows declarations like below:
        matrix m;
    therefore, usage is dangerous
    */
    homogeneous_point() {
    }

    // constructs a homogeneous point with given coordinates. forces w to be 1.0
    // if w is zero, raises error
    homogeneous_point(double x, double y, double z, double w)
    {
        assert (w != 0);
        this->x = x/w;
        this->y = y/w;
        this->z = z/w;
        this->w = 1;
    }

    // adds two points. returns a point forcing w to be 1.0
    homogeneous_point operator+ (const homogeneous_point& point)
    {
        double x = this->x + point.x;
        double y = this->y + point.y;
        double z = this->z + point.z;
        double w = this->w + point.w;
        homogeneous_point p(x, y, z, w);
        return p;
    }

    // subtracts one point from another. returns a point forcing w to be 1.0
    homogeneous_point operator- (const homogeneous_point& point)
    {
        double x = this->x - point.x;
        double y = this->y - point.y;
        double z = this->z - point.z;
        double w = this->w - point.w;
        homogeneous_point p(x, y, z, w);
    }

    // Print the coordinates of a point. exists for testing purpose.
    void print()
    {
        cout << "Point: " << endl;
        cout << x << " " << y << " " << z << " " << w << endl;
    }

};


class Vector
{
public:
    double x, y, z;

    // constructs a vector with given components
    Vector(double x, double y, double z) {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    // keeps the direction same. recalculates the vector to be unit.
    void normalize()
    {
        double r = sqrt(x*x + y*y + z*z);
        x = x / r;
        y = y / r;
        z = z / r;
    }

    // add two vectors
    Vector operator+(const Vector& v)
    {
        Vector v1(x+v.x, y+v.y, z+v.z);
        return v1;
    }

    // subtract one vector from another
    Vector operator-(const Vector& v)
    {
        Vector v1(x-v.x, y-v.y, z-v.z);
        return v1;
    }

    // scale a vector with a given coefficient
    Vector operator* (double m)
    {
        Vector v(x*m, y*m, z*m);
        return v;
    }

    // get the dot product of two vectors
    static double dot(Vector a, Vector b)
    {
        return a.x*b.x + a.y*b.y + a.z*b.z;
    }

    // get the cross product of two vectors
    static Vector cross(Vector a, Vector b)
    {
        Vector v(a.y*b.z - a.z*b.y, b.x*a.z - b.z*a.x, a.x*b.y - a.y*b.x);
        return v;
    }

    // print a vector. only for testing purposes.
    void print ()
    {
        cout << "Vector" << endl;
        cout << x << " " << y << " " << z << endl;
    }
};

Vector Rodrigues(Vector x,Vector a,double ang) {
    ang=(pi*ang)/180;
    Vector v1=x*cos(ang);
    double dot=Vector::dot(a,x);
    Vector cross=Vector::cross(a,x);
    dot=(1-cos(ang))*dot;
    Vector v2=a*dot;
    Vector v3=cross*sin(ang);

    Vector res=v1+v2;
    res=res+v3;

    return res;
}
/*
The matrices are forced to be 4x4. This is because in this assignment, we will deal with points in triangles.
Maximum # of points that we will deal with at once is 3. And all the standard matrices are 4x4 (i.e. scale, translation, rotation etc.)
*/
class matrix
{
public:
    double values[4][4];
    int num_rows, num_cols;

    // only set the number of rows and cols
    matrix(int rows, int cols)
    {
        assert (rows <= 4 && cols <= 4);
        num_rows = rows;
        num_cols = cols;
    }

    // prepare an nxn square matrix
    matrix(int n)
    {
        assert (n <= 4);
        num_rows = num_cols = n;
    }
    matrix(){
    }
    // prepare and return an identity matrix of size nxn
    static matrix make_identity(int n)
    {
        assert (n <= 4);
        matrix m(n);
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                if (i == j)
                    m.values[i][j] = 1;
                else
                    m.values[i][j] = 0;
            }
        }
        return m;
    }

    // print the matrix. exists for testing purposes
    void print()
    {
        cout << "Matrix:" << endl;
        for (int i = 0; i < num_rows; i++)
        {
            for (int j = 0; j < num_cols; j++)
            {
                cout << values[i][j] << "\t";
            }
            cout << endl;
        }
    }

    // add the two matrices. Raise error if dimension mismatches
    matrix operator+ (const matrix& m)
    {
        assert (this->num_rows == m.num_rows);
        assert (this->num_cols == m.num_cols);

        matrix m1(num_rows, num_cols);
        for (int i = 0; i < num_rows; i++)
        {
            for (int j = 0; j < num_cols; j++)
            {
                m1.values[i][j] = values[i][j] + m.values[i][j];
            }
        }
        return m1;
    }

    // subtract a matrix from another. raise error if dimension mismatches
    matrix operator- (const matrix& m)
    {
        assert (this->num_rows == m.num_rows);
        assert (this->num_cols == m.num_cols);

        matrix m1(num_rows, num_cols);
        for (int i = 0; i < num_rows; i++)
        {
            for (int j = 0; j < num_cols; j++)
            {
                m1.values[i][j] = values[i][j] - m.values[i][j];
            }
        }
        return m1;
    }

    // multiply two matrices. allows statements like m1 = m2 * m3; raises error is dimension mismatches
    matrix operator* (const matrix& m)
    {
        assert (this->num_cols == m.num_rows);
        matrix m1(this->num_rows, m.num_cols);

        for (int i = 0; i < m1.num_rows; i++) {
            for (int j = 0; j < m1.num_cols; j++) {
                double val = 0;
                for (int k = 0; k < this->num_cols; k++) {
                    val += this->values[i][k] * m.values[k][j];
                }
                m1.values[i][j] = val;
            }
        }
        return m1;
    }

    // multiply a matrix with a constant
    matrix operator* (double m)
    {
        matrix m1(this->num_rows, this->num_cols);
        for (int i = 0; i < num_rows; i++) {
            for (int j = 0; j < num_cols; j++) {
                m1.values[i][j] = m * this->values[i][j];
            }
        }
        return m1;
    }

    // multiply a 4x4 matrix with a homogeneous point and return the resulting point.
    // usage: homogeneous_point p = m * p1;
    // here, m is a 4x4 matrix, intended to be the transformation matrix
    // p1 is the point on which the transformation is being made
    // p is the resulting homogeneous point
    homogeneous_point operator* (const homogeneous_point& p)
    {
        assert (this->num_rows == this->num_cols && this->num_rows == 4);

        matrix m(4, 1);
        m.values[0][0] = p.x;
        m.values[1][0] = p.y;
        m.values[2][0] = p.z;
        m.values[3][0] = p.w;

        matrix m1 = (*this)*m;
        homogeneous_point p1(m1.values[0][0], m1.values[1][0], m1.values[2][0], m1.values[3][0]);
        return p1;
    }

    // return the transpose of a matrix
    matrix transpose()
    {
        matrix m(num_cols, num_rows);
        for (int i = 0; i < num_rows; i++) {
            for (int j = 0; j < num_cols; j++) {
                m.values[j][i] = values[i][j];
            }
        }
        return m;
    }

};

/*
A simple class to hold the color components, r, g, b of a certain shade.
*/
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

class triangle {
public:
    homogeneous_point p1,p2,p3;
    color c;
    homogeneous_point in[200];
    homogeneous_point out[200];
    homogeneous_point intersect[10];
    int inidx;
    triangle(homogeneous_point p1,homogeneous_point p2,homogeneous_point p3,color c) {
        this->p1 = p1;
        this->p2 = p2;
        this->p3 = p3;
        this->c = c;
        inidx=0;
    }
    triangle() {
    }
};

homogeneous_point intersection(double plane,homogeneous_point p,homogeneous_point v) {
    double t=(plane-p.z)/v.z;
    homogeneous_point V(t*v.x,t*v.y,t*v.z);
    homogeneous_point P=p+V;
    return P;
}

double scale(double x,double minn,double maxx,double a,double b) {
    return ((b-a)*(x-minn)/(maxx-minn))+a;
}


homogeneous_point maxx(triangle t) {
    if(t.p1.y>=t.p2.y) {
        if (t.p1.y>=t.p3.y) {
            return t.p1;
        }
        else return t.p3;
    }
    else {
        if (t.p2.y>=t.p3.y) {
            return t.p2;
        }
        else return t.p3;
    }
}

homogeneous_point minn(triangle t) {
    if(t.p1.y<=t.p2.y) {
        if (t.p1.y<=t.p3.y) {
            return t.p1;
        }
        else return t.p3;
    }
    else {
        if (t.p2.y<=t.p3.y) {
            return t.p2;
        }
        else return t.p3;
    }
}

homogeneous_point mid(triangle t) {
    if(t.p1.y>t.p2.y && t.p1.y<t.p3.y) return t.p1;
    else if(t.p1.y<t.p2.y && t.p1.y>t.p3.y) return t.p1;
    else if(t.p2.y>t.p1.y && t.p2.y<t.p3.y) return t.p2;
    else if(t.p2.y<t.p1.y && t.p2.y>t.p3.y) return t.p2;
    else if(t.p3.y>t.p1.y && t.p3.y<t.p2.y) return t.p3;
    else return t.p3;
}

homogeneous_point line_line_intersect(double y,homogeneous_point p1,homogeneous_point p2) {
    homogeneous_point inter(0,0,0);
    inter.y=y;
    double rx=(y-p1.y)*(p1.x-p2.x)/(p1.y-p2.y)+p1.x;
    inter.x=rx;
    return inter;
}



double eye_x, eye_y, eye_z;
double look_x, look_y, look_z;
double up_x, up_y, up_z;
double fov_x, fov_y, aspectRatio, near, far;
color backgroud;
int screen_x, screen_y;
matrix mat[1000];
int idx=0;
triangle tri[100];
int tdx=0;
int ttdx=0;
int tttdx=0;
triangle clip[200];
int cidx=0;



void scan_convert() {
    ifstream stage3;
    stage3.open("stage3.txt");

    color** pixels = new color*[screen_x];
    double** zs = new double*[screen_x];
    for (int i = 0; i < screen_x; i++) {
        pixels[i] = new color [screen_y];
        for (int j = 0; j < screen_y; j++) {
            pixels[i][j] = backgroud;
        }
        zs[i] = new double [screen_y];
        for (int j = 0; j < screen_y; j++) {
            zs[i][j] = +20; // a very large value intended as +INFINITY
        }
    }

    // perform scan conversion, populate the 2D array pixels
    // the array zs is the z-buffer.
    //cout<<tri[0].c.r<<" "<<tri[0].c.g<<tri[0].c.b;
    //cout<<ttdx<<endl;
    //cout<<screen_x<<" "<<screen_y<<endl;
    for (int i=0;i<ttdx;i++) {
        //tri[i].p1.print();
        homogeneous_point mx=maxx(tri[i]);
        homogeneous_point mn=minn(tri[i]);
        homogeneous_point md=mid(tri[i]);
        //cout<<mx.y<<" "<<mn.y<<" "<<md.y<<endl;
        double scl_mx=scale(mx.y,-1,1,0,screen_y);
        double scl_mn=scale(mn.y,-1,1,0,screen_y);
        double scl_md=scale(md.y,-1,1,0,screen_y);
        //cout<<(int)scl_mn<<" "<<(int)scl_mx<<endl<<endl;
        double j=scl_mx;
        while(j>=scl_mn) {
            //cout<<j<<endl;
            double scn_y=scale(j,0,screen_y,-1,1);
            //cout<<scn_y<<endl;
            if (j>scl_md) {
                homogeneous_point i1=line_line_intersect(scn_y,mx,md);
                homogeneous_point i2=line_line_intersect(scn_y,mx,mn);
                //i1.print();
                //cout<<scale(i1.x,-1,1,0,screen_x)<<" "<<scale(i2.x,-1,1,0,screen_x)<<endl;
                double i1s=scale(i1.x,-1,1,0,screen_x);
                double i2s=scale(i2.x,-1,1,0,screen_x);

                /*if(((int)scl_mx-j)==5) {
                    //cout<<i1.x<<" "<<i2.x<<endl;
                    cout<<scale_y(i1.x,-1,1,0,screen_y)<<" "<<scale_y(i2.x,-1,1,0,screen_y)<<endl;
                }*/
                double za,zb;
                double zmin,zmax;

                za=mx.z-((mx.z-md.z)*(mx.y-scn_y))/(mx.y-md.y);
                zb=mx.z-((mx.z-mn.z)*(mx.y-scn_y))/(mx.y-mn.y);

                //double zas=scale(za,-1,1,0,screen_x);
                //double zbs=scale(zb,-1,1,0,screen_x);
                double low=min(i1s,i2s);
                double high=max(i1s,i2s);
                //cout<<low<<" "<<high<<endl;
                for(double k=low;k<=high;k++) {
                    //pixels[k][j]=tri[i].c;
                    //cout<<k<<endl;
                    double xb,xa;
                    if(i1.x>=i2.x) {
                        xb=i1.x;
                        xa=i2.x;
                        zmin=zb;
                        zmax=za;
                    }
                    else {
                        xb=i2.x;
                        xa=i1.x;
                        zmin=za;
                        zmax=zb;
                    }
                    //cout<<xb;
                    //xa=scale_x(xa,-1,1,0,screen_x);
                    //xb=scale_x(xb,-1,1,0,screen_x);
                    //cout<<" "<<xb<<endl;

                    double zp=zmax-((zmax-zmin)*(xb-k))/(xb-xa);
                    //cout<<zp<<endl;
                    //int xp=int(zp);

                    if(zp<zs[(int)k][(int)j]) {
                        zs[(int)k][screen_y-(int)j]=zp;
                        pixels[(int)k][screen_y-(int)j]=tri[i].c;
                    }

                    //cout<<xp<<" "<<j<<endl;

                }
                //cout<<"up"<<endl;

            }
            //cout<<"h"<<endl;
            else {
                //cout<<"half"<<endl;
                homogeneous_point i1=line_line_intersect(scn_y,mn,md);
                homogeneous_point i2=line_line_intersect(scn_y,mn,mx);
                //cout<<scale(i1.x,-1,1,0,screen_x)<<" "<<scale(i2.x,-1,1,0,screen_x)<<endl;
                double i1s=scale(i1.x,-1,1,0,screen_x);
                double i2s=scale(i2.x,-1,1,0,screen_x);
                double za,zb;
                double zmin,zmax;

                za=mx.z-((mx.z-md.z)*(mx.y-scn_y))/(mx.y-md.y);
                zb=mx.z-((mx.z-mn.z)*(mx.y-scn_y))/(mx.y-mn.y);


                //double zas=scale(za,-1,1,0,screen_x);
                //double zbs=scale(zb,-1,1,0,screen_x);
                double low=min(i1s,i2s);
                double high=max(i1s,i2s);
                //cout<<low<<" "<<high<<endl;
                for(double k=low;k<=high;k++) {
                    //pixels[k][j]=tri[i].c;
                    double xb,xa;
                    if(i1.x>=i2.x) {
                        xb=i1.x;
                        xa=i2.x;
                        zmin=zb;
                        zmax=za;
                    }
                    else {
                        xb=i2.x;
                        xa=i1.x;
                        zmin=za;
                        zmax=zb;
                    }
                    //xa=scale_x(xa,-1,1,0,screen_x);
                    //xb=scale_x(xb,-1,1,0,screen_x);

                    double zp=zmax-((zmax-zmin)*(xb-k))/(xb-xa);
                    //cout<<zp<<endl;
                    //int xp=int(zp);
                    if(zp<zs[(int)k][(int)j]) {
                        zs[(int)k][screen_y-(int)j]=zp;
                        pixels[(int)k][screen_y-(int)j]=tri[i].c;
                    }

                }
                //cout<<"low"<<endl;
            }
            j--;
        }
    }
    // the following code generates a bmp image. do not change this.
    bitmap_image image(screen_x, screen_y);
    for (int x = 0; x < screen_x; x++) {
        for (int y = 0; y < screen_y; y++) {
            image.set_pixel(x, y, pixels[x][y].r, pixels[x][y].g, pixels[x][y].b);
        }
    }
    image.save_image("out.bmp");

    // free the dynamically allocated memory

}


void stage3()
{
    if (near == far) return;
    ifstream stage2;
    ofstream stage3;
    stage2.open ("stage2.txt");
    stage3.open ("stage3.txt");
    stage3 << std::fixed;
    stage3 << std::setprecision(7);

    //process input from stage2 and write to stage3
    //homogeneous_point in[200];
    //homogeneous_point out[200];
    //cout<<ttdx;
    //******NEAR
    for(int j=0;j<ttdx;j++) {
        /*int in_count=0;
        int out_count=0;

        //cout<<tri[j].p1.z<<" ";
        //cout<<tri[j].p2.z<<" ";
        //cout<<tri[j].p3.z<<" ";
        if (tri[j].p1.z<=-near && tri[j].p1.z>=-far) {
            tri[j].intersect[tri[j].inidx]=tri[j].p1;
            tri[j].inidx++;
        }

        if (tri[j].p2.z<=-near && tri[j].p2.z>=-far) {
            tri[j].intersect[tri[j].inidx]=tri[j].p2;
            tri[j].inidx++;
        }
        if (tri[j].p3.z<=-near && tri[j].p3.z>=-far) {
            tri[j].intersect[tri[j].inidx]=tri[j].p3;
            tri[j].inidx++;
        }
        //cout<<endl;

        if(tri[j].p1.z>-near) {
            tri[j].out[out_count]=tri[j].p1;
            out_count++;
        }
        else if(tri[j].p1.z<=-near) {
            tri[j].in[in_count]=tri[j].p1;
            in_count++;
        }

        else if(tri[j].p2.z>-near) {
            tri[j].out[out_count]=tri[j].p2;
            out_count++;
        }
        else if(tri[j].p2.z<=-near) {
            tri[j].in[in_count]=tri[j].p2;
            in_count++;
        }

        else if(tri[j].p3.z>-near) {
            tri[j].out[out_count]=tri[j].p3;
            out_count++;
        }
        else if(tri[j].p3.z<=-near) {
            tri[j].in[in_count]=tri[j].p3;
            in_count++;
        }
        if (out_count==3) continue;
        else if (out_count==1) {
            homogeneous_point i1=intersection(-near,tri[j].in[0],tri[j].out[0]);
            tri[j].intersect[tri[j].inidx]=i1;
            tri[j].inidx++;
            homogeneous_point i2=intersection(-near,tri[j].in[1],tri[j].out[0]);
            tri[j].intersect[tri[j].inidx]=i2;
            tri[j].inidx++;

        }
        else if (out_count==2) {
            homogeneous_point i1=intersection(-near,tri[j].in[0],tri[j].out[0]);
            tri[j].intersect[tri[j].inidx]=i1;
            tri[j].inidx++;
            homogeneous_point i2=intersection(-near,tri[j].in[1],tri[j].out[1]);
            tri[j].intersect[tri[j].inidx]=i2;
            tri[j].inidx++;
        }
        in_count=0;
        out_count=0;



        if(tri[j].p1.z<-far) {
            tri[j].out[out_count]=tri[j].p1;
            out_count++;
        }
        else if(tri[j].p1.z>=-far) {
            tri[j].in[in_count]=tri[j].p1;
            in_count++;
        }

        else if(tri[j].p2.z<-far) {
            tri[j].out[out_count]=tri[j].p2;
            out_count++;
        }
        else if(tri[j].p2.z>=-far) {
            tri[j].in[in_count]=tri[j].p2;
            in_count++;
        }

        else if(tri[j].p3.z<-far) {
            tri[j].out[out_count]=tri[j].p3;
            out_count++;
        }
        else if(tri[j].p3.z>=-far) {
            tri[j].in[in_count]=tri[j].p3;
            in_count++;
        }
        if (out_count==3) continue;
        else if (out_count==1) {
            homogeneous_point i11=intersection(-far,tri[j].in[0],tri[j].out[0]);
            tri[j].intersect[tri[j].inidx]=i11;
            tri[j].inidx++;
            homogeneous_point i22=intersection(-near,tri[j].in[1],tri[j].out[0]);
            tri[j].intersect[tri[j].inidx]=i22;
            tri[j].inidx++;

        }
        else if (out_count==2) {
            homogeneous_point i11=intersection(-near,tri[j].in[0],tri[j].out[0]);
            tri[j].intersect[tri[j].inidx]=i11;
            tri[j].inidx++;
            homogeneous_point i22=intersection(-near,tri[j].in[1],tri[j].out[1]);
            tri[j].intersect[tri[j].inidx]=i22;
            tri[j].inidx++;
        }

        if (tri[j].inidx==3) {
            triangle t(tri[j].intersect[0],tri[j].intersect[1],tri[j].intersect[2],tri[j].c);
            clip[cidx]=t;
            cidx++;
        }
        else if (tri[j].inidx==4) {
            triangle t(tri[j].intersect[0],tri[j].intersect[1],tri[j].intersect[2],tri[j].c);
            clip[cidx]=t;
            cidx++;
            triangle tt(tri[j].intersect[0],tri[j].intersect[2],tri[j].intersect[3],tri[j].c);
            clip[cidx]=tt;
            cidx++;
        }
        else if (tri[j].inidx==5) {
            triangle t(tri[j].intersect[0],tri[j].intersect[1],tri[j].intersect[2],tri[j].c);
            clip[cidx]=t;
            cidx++;
            triangle tt(tri[j].intersect[0],tri[j].intersect[2],tri[j].intersect[3],tri[j].c);
            clip[cidx]=tt;
            cidx++;
            triangle ttt(tri[j].intersect[0],tri[j].intersect[3],tri[j].intersect[4],tri[j].c);
            clip[cidx]=ttt;
            cidx++;
        }*/
        //----NEAR
        homogeneous_point input[3];
        homogeneous_point output[5];
        homogeneous_point output2[5];
        int cn=0,co=0,cf=0;
        input[cn]=tri[j].p1;
        cn++;
        input[cn]=tri[j].p2;
        cn++;
        input[cn]=tri[j].p3;
        cn++;
        homogeneous_point s=input[cn-1];
        homogeneous_point p=input[0];
        int k=0;
        while(k<cn) {
            if(s.z<=-near && p.z<=-near) {
                output[co]=p;
                co++;
            }
            else if (s.z<=-near && p.z>-near) {
                homogeneous_point i=intersection(-near,s,p);
                output[co]=i;
                co++;
            }
            else if (s.z>-near && p.z<=-near) {
                homogeneous_point i=intersection(-near,s,p);
                output[co]=i;
                co++;
                output[co]=p;
                co++;
            }
            s=input[k];
            p=input[k+1];
            k++;
        }

        s=output[co-1];
        p=output[0];
        k=0;
        while(k<co) {
            if(s.z>=-far && p.z>=-far) {
                output2[cf]=p;
                cf++;
            }
            else if (s.z>=-far && p.z<-far) {
                homogeneous_point ii=intersection(-far,s,p);
                output2[cf]=ii;
                cf++;
            }
            else if (s.z<-far && p.z>=-far) {
                homogeneous_point ii=intersection(-far,s,p);
                output2[cf]=ii;
                cf++;
                output2[cf]=p;
                cf++;
            }
            s=input[k];
            p=input[k+1];
            k++;
        }
        if (cf==3) {
            triangle t(output2[0],output2[1],output2[2],tri[j].c);
            clip[cidx]=t;
            cidx++;
        }
        else if(cf==4) {
            triangle t(output2[0],output2[1],output2[2],tri[j].c);
            clip[cidx]=t;
            cidx++;
            triangle tt(output2[0],output2[2],output2[3],tri[j].c);
            clip[cidx]=tt;
            cidx++;
        }
        else if(cf==5) {
            triangle t(output2[0],output2[1],output2[2],tri[j].c);
            clip[cidx]=t;
            cidx++;
            triangle tt(output2[0],output2[2],output2[3],tri[j].c);
            clip[cidx]=tt;
            cidx++;
            triangle ttt(output2[0],output2[3],output2[4],tri[j].c);
            clip[cidx]=ttt;
            cidx++;
        }



    }
    //cout<<cidx;
    fov_x=fov_y*aspectRatio;
    double ang1=(fov_y*pi)/180;
    double t=near*tan(ang1/2);
    double ang2=(fov_x*pi)/180;
    double r=near*tan(ang2/2);

    matrix P=matrix::make_identity(4);
    P.values[0][0]=near/r;
    P.values[1][1]=near/t;
    P.values[2][2]=-1*(far+near)/(far-near);
    P.values[2][3]=-1*(2*far*near)/(far-near);
    P.values[3][2]=-1;
    P.values[3][3]=0;
    //P.print();
    double dx,dy,dz;
    int i=0;
    //while (true) {
    for(i=0;i<cidx;i++) {
        /*stage2>>dx;
        if (stage2.eof()) break;
        stage2>>dy>>dz;
        homogeneous_point p(dx,dy,dz);
        homogeneous_point pp=P*p;
        stage3<<pp.x<<" "<<pp.y<<" "<<pp.z<<endl;
        i++;
        if(i%3==0) stage3<<endl;*/

        homogeneous_point a(clip[i].p1.x,clip[i].p1.y,clip[i].p1.z);
        homogeneous_point b(clip[i].p2.x,clip[i].p2.y,clip[i].p2.z);
        homogeneous_point c(clip[i].p3.x,clip[i].p3.y,clip[i].p3.z);
        homogeneous_point aa=P*a;
        stage3<<aa.x<<" "<<aa.y<<" "<<aa.z<<endl;
        homogeneous_point bb=P*b;
        stage3<<bb.x<<" "<<bb.y<<" "<<bb.z<<endl;
        homogeneous_point cc=P*c;
        stage3<<cc.x<<" "<<cc.y<<" "<<cc.z<<endl;
        stage3<<endl;
        triangle ttt(aa,bb,cc,clip[i].c);
        tri[tttdx]=ttt;
        tttdx++;
    }

    stage3.close();
    stage2.close();

}

void stage2()
{
    ifstream stage1;
    ofstream stage2;
    stage1.open ("stage1.txt");
    stage2.open ("stage2.txt");
    stage2 << std::fixed;
    stage2 << std::setprecision(7);

    // collect input from stage1 and process, write output to stage2
    Vector eye(eye_x,eye_y,eye_z);
    Vector look(look_x,look_y,look_z);
    Vector up(up_x,up_y,up_z);

    Vector l=look-eye;
    l.normalize();
    Vector r=Vector::cross(l,up);
    r.normalize();
    Vector u=Vector::cross(r,l);

    matrix T=matrix::make_identity(4);
    T.values[0][3]=-eye_x;
    T.values[1][3]=-eye_y;
    T.values[2][3]=-eye_z;
    //T.print();
    matrix R(4,4);
    R.values[0][0]=r.x;
    R.values[0][1]=r.y;
    R.values[0][2]=r.z;
    R.values[0][3]=0;

    R.values[1][0]=u.x;
    R.values[1][1]=u.y;
    R.values[1][2]=u.z;
    R.values[1][3]=0;

    R.values[2][0]=-l.x;
    R.values[2][1]=-l.y;
    R.values[2][2]=-l.z;
    R.values[2][3]=0;

    R.values[3][0]=0;
    R.values[3][1]=0;
    R.values[3][2]=0;
    R.values[3][3]=1;

    matrix V=R*T;

    double dx,dy,dz;
    int i=0;
    int point_count=0;
    homogeneous_point tri_trc[3];
    while (true) {
        stage1>>dx;
        if (stage1.eof()) break;
        stage1>>dy>>dz;
        homogeneous_point p(dx,dy,dz);
        homogeneous_point pp=V*p;
        stage2<<pp.x<<" "<<pp.y<<" "<<pp.z<<endl;
        tri_trc[point_count]=pp;
        i++;
        point_count++;
        if(i%3==0) {
            stage2<<endl;
            point_count=0;
            triangle t(tri_trc[0],tri_trc[1],tri_trc[2],tri[ttdx].c);
            tri[ttdx]=t;
            ttdx++;
        }
    }
    //tri[15].p3.print();

    stage1.close();
    stage2.close();

}

void stage1()
{
    ifstream scene;
    ofstream stage1;
    scene.open ("scene.txt");
    stage1.open ("stage1.txt");
    stage1 << std::fixed;
    stage1 << std::setprecision(7);

    string command;

    scene >> eye_x >> eye_y >> eye_z;
    scene >> look_x >> look_y >> look_z;
    scene >> up_x >> up_y >> up_z;
    scene >> fov_y >> aspectRatio >> near >> far;
    scene >> screen_x >> screen_y;
    scene >> backgroud.r >> backgroud.g >> backgroud.b;

    // take other commands as input from scene in a loop
    // process accordingly
    // write to stage1

    mat[idx]=matrix::make_identity(4);
    idx++;
    //mat[0].print();
    push_pop pp[100];
    int pdx=0;
    while (true) {
        scene>>command;
        if (command=="triangle") {
            double dx,dy,dz;
            color c;
            scene>>dx>>dy>>dz;
            homogeneous_point p1(dx,dy,dz);
            scene>>dx>>dy>>dz;
            homogeneous_point p2(dx,dy,dz);
            scene>>dx>>dy>>dz;
            homogeneous_point p3(dx,dy,dz);

            scene>>c.r>>c.g>>c.b;

            matrix m=mat[idx-1];
            homogeneous_point p11=m*p1;
            stage1<<p11.x<<" "<<p11.y<<" "<<p11.z<<endl;
            homogeneous_point p22=m*p2;
            stage1<<p22.x<<" "<<p22.y<<" "<<p22.z<<endl;
            homogeneous_point p33=m*p3;
            stage1<<p33.x<<" "<<p33.y<<" "<<p33.z<<endl;
            stage1<<endl;

            triangle t(p11,p22,p33,c);
            tri[tdx]=t;
            tdx++;
        }
        else if (command=="translate") {
            double tx,ty,tz;
            scene>>tx>>ty>>tz;
            matrix tm=matrix::make_identity(4);
            tm.values[0][3]=tx;
            tm.values[1][3]=ty;
            tm.values[2][3]=tz;
            //tm.print();
            matrix res=mat[idx-1]*tm;
            mat[idx]=res;
            idx++;
            if(pdx>0) pp[pdx-1].pn++;
        }
        else if (command=="scale") {
            double sx,sy,sz;
            scene>>sx>>sy>>sz;
            matrix sm=matrix::make_identity(4);
            sm.values[0][0]=sx;
            sm.values[1][1]=sy;
            sm.values[2][2]=sz;
            //sm.print();
            matrix res=mat[idx-1]*sm;
            mat[idx]=res;
            //mat[idx].print();
            idx++;
            if(pdx>0) pp[pdx-1].pn++;
        }
        else if (command=="rotate") {
            double ang,ax,ay,az;
            scene>>ang>>ax>>ay>>az;
            Vector a(ax,ay,az);
            a.normalize();
            Vector i(1.0,0.0,0.0);
            Vector j(0.0,1.0,0.0);
            Vector k(0.0,0.0,1.0);
            Vector c1=Rodrigues(i,a,ang);
            Vector c2=Rodrigues(j,a,ang);
            Vector c3=Rodrigues(k,a,ang);
            matrix rot(4,4);

            rot.values[0][0]=c1.x;
            rot.values[0][1]=c2.x;
            rot.values[0][2]=c3.x;
            rot.values[0][3]=0;

            rot.values[1][0]=c1.y;
            rot.values[1][1]=c2.y;
            rot.values[1][2]=c3.y;
            rot.values[1][3]=0;

            rot.values[2][0]=c1.z;
            rot.values[2][1]=c2.z;
            rot.values[2][2]=c3.z;
            rot.values[2][3]=0;

            rot.values[3][0]=0;
            rot.values[3][1]=0;
            rot.values[3][2]=0;
            rot.values[3][3]=1;

            //rot.print();
            matrix res=mat[idx-1]*rot;
            //res.print();
            mat[idx]=res;
            idx++;
            if(pdx>0) pp[pdx-1].pn++;
        }
        else if (command=="push") {
            push_pop p(0);
            pp[pdx]=p;
            pdx++;
        }
        else if (command=="pop") {
            idx=idx-pp[pdx-1].pn;
            pdx--;
        }
        else if (command=="end") {
            break;
        }
    }
    scene.close();
    stage1.close();

}

int main()
{
    cout << std::fixed;
    cout << std::setprecision(4);
    stage1();
    stage2();
    stage3();
    scan_convert();

    return 0;
}
