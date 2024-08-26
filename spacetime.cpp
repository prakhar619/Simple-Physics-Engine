class spaceTime
{
    public:
    int x,y,z;
    long t;

    spaceTime()
    {
        x = y = z = t = 0;
    }
    spaceTime(int x,int y,int z,int t)
    {   
        update_spaceTime(x,y,z,t);
    }
    spaceTime(spaceTime& Obj)
    {
        update_spaceTime(Obj.x,Obj.y,Obj.z,Obj.t);
    }

    void update_spaceTime(int x,int y,int z,int t)
    {
        this->x = x;
        this->y = y;
        this->z = z;
        this->t = t;
    }
    void update_spaceTime(spaceTime cpyObj)
    {
        x = cpyObj.x;
        y = cpyObj.y;
        z = cpyObj.z;
        t = cpyObj.t;        
    }

};