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


double eye_x, eye_y, eye_z;
double look_x, look_y, look_z;
double up_x, up_y, up_z;
double fov_x, fov_y, aspectRatio, near, far;
color backgroud;
int screen_x, screen_y;



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

    // process input from stage2 and write to stage3

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
    matrix mat[1000];
    int idx=0;
    mat[idx]=matrix::make_identity(4);
    idx++;
    //mat[0].print();
    while (true) {
        scene>>command;
        if (command=="triangle") {
            homogeneous_point p1,p2,p3;
            color c;
            scene>>p1.x>>p1.y>>p1.z;
            scene>>p2.x>>p2.y>>p2.z;
            scene>>p3.x>>p3.y>>p3.z;
            scene>>c.r>>c.g>>c.b;

            stage1<<p1.x<<" "<<p1.y<<" "<<p1.z<<endl;
            stage1<<p2.x<<" "<<p2.y<<" "<<p2.z<<endl;
            stage1<<p3.x<<" "<<p3.y<<" "<<p3.z<<endl;
            stage1<<endl;
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
