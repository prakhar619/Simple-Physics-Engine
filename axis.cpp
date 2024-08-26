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
        update_axis(x,y,z);
    }
    axis(axis &Obj)
    {
        update_axis(Obj.x,Obj.y,Obj.z);
    }
    void update_axis(axis a)
    {
        x= a.x;
        y= a.y;
        z= a.z;
    }
    void update_axis(int x, int y, int z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }
};