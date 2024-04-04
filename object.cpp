#include <iostream>
using namespace std;
class color
{
    public:
    int r,g,b,alpha;
    color()
    {
        r = g= b = 0;
        alpha = 1;
    }
    color(int r,int g,int b,int alpha)
    {
        if(r > 255 || g > 255 || b > 255 || alpha > 255)
        {
            cout << "Error. rgb cannot be greater than 255\n" << endl;
        }
        this->r = r;
        this->g = g;
        this->b = b;
        this->alpha = alpha;
    }
    color(color &cpyObj)
    {
        r = cpyObj.r;
        g = cpyObj.g;
        b = cpyObj.b;
        alpha = cpyObj.alpha;
    }
};
class spaceTime
{
    public:
    int x,y,z;
    double t;

    spaceTime()
    {
        x = y = z = t = 0;
    }
    spaceTime(int x,int y,int z,int t)
    {
        this->x = x;
        this->y = y;
        this->z = z;
        this->t = t;
    }
    spaceTime(spaceTime &cpyObj)
    {
        x = cpyObj.x;
        y = cpyObj.y;
        z = cpyObj.z;
        t = cpyObj.t;        
    }
};
class axis
{
    public:
    double x,y,z;
    axis()
    {
        x = 0;
        y = 0;
        z= 0;
    }
    axis(int x, int y, int z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }
    axis(axis &a)
    {
        x= a.x;
        y= a.y;
        z= a.z;
    }
};
class Point
{
    public:
    double p_mass;
    color p_color;
    spaceTime p_spaceTime;
    axis force;
    axis vel;
    axis acc;

    Point(spaceTime point, double mass, color p_color)
    {
        p_spaceTime = spaceTime(point);
        p_color = color(p_color);
        p_mass = mass;
    }

    void update_pos(int time)
    {
        vel.x = vel.x + acc.x*time;
        vel.y = vel.y + acc.y*time;
        vel.z = vel.z + acc.z*time;
    }

    void update_acc(axis force1)
    {
        force = axis(force1);
        acc.x = force.x/p_mass;
        acc.y = force.y/p_mass;
        acc.z = force.z/p_mass;
    }

};
