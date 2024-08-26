class Point : public Color, public spaceTime
{
    public:
    int index;
    double p_mass;

    axis force;
    axis vel {0,0,0};
    axis acc {0,0,0};

    Point(int index,spaceTime point, double mass, Color p_Color)
    {
        updateColor(p_Color);
        update_spaceTime(point);
        p_mass = mass;
        this->index= index;

    }

    void printPt()
    {
        cout << "Pos:" << x << " " << y << " " << z << endl;
        cout << "Mass:" << p_mass << endl;
        cout << "Color:" << r << " " << g << " " << b << endl; 
    }

    int compare(Point* pt)
    {
        if(x == pt->x && y == pt->y && z == pt->z)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }

};