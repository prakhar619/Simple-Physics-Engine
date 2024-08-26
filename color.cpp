class Color
{
    public:
    int r,g,b,alpha;
    Color()
    {
        r = g= b = 0;
        alpha = 1;
    }
    Color(int rx,int gx,int bx,int alphax)
    {
        updateColor(rx,gx,bx,alphax);
    }
    Color(Color &Obj)
    {
        updateColor(Obj.r,Obj.g,Obj.b,Obj.alpha);
    }


    void updateColor(int rx,int gx,int bx,int alphax)
    {
        if(r > 255 || g > 255 || b > 255 || alpha > 255)
        {
           // cout << "Error. rgb cannot be greater than 255\n" << endl;
        }
        r = rx;
        g = gx;
        b = bx;
        alpha = alphax;

    }
    void updateColor(Color cpyObj)
    {
        r = cpyObj.r;
        g = cpyObj.g;
        b = cpyObj.b;
        alpha = cpyObj.alpha;
    }
};