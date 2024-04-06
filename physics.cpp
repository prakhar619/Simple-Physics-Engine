#u

class Physics
{
    public:

    int initialised = 0;
    map<pair<Point*,Point*>,int> collisionTime;

    int box_xW,box_yW,boxWidth;

    //physics env Variables
    int timeScale;
    int flg_envForce = 0;
    axis envForce {0,0,0};
    int collisionDelta;

    void init(int timeScale,int box_xW,int box_yW,int boxWidth)
    {
        this->timeScale = timeScale;
        this->box_xW = box_xW;
        this->box_yW = box_yW;
        this->boxWidth = boxWidth;
        initialised = 1;
        this->collisionDelta = 3;
    }

    void toogleGForce()
    {
        if(flg_envForce == 0)
        {
            envForce.update_axis(0,10,0);
            flg_envForce = 1;
        }
        else
        {
            envForce.update_axis(0,0,0);
            flg_envForce = 0;
        }
    }
    void update_vel(Point* pt1,int currTime)
    {
        int time = currTime - pt1->t;
        pt1->vel.x = pt1->vel.x + pt1->acc.x*time;
        pt1->vel.y = pt1->vel.y + pt1->acc.y*time;
        pt1->vel.z = pt1->vel.z + pt1->acc.z*time;
    }

    void update_acc(Point* pt1,axis force1)
    {
        pt1->force.update_axis(force1);
        pt1->acc.x = pt1->force.x/pt1->p_mass;
        pt1->acc.y = pt1->force.y/pt1->p_mass;
        pt1->acc.z = pt1->force.z/pt1->p_mass;
    }

    void update_pos(Point* pt1,int currTime)
    {
        int time = currTime - pt1->t;
        pt1->x = pt1->x + pt1->vel.x*time + (1/2.0)*pt1->acc.x*time*time;
        pt1->y = pt1->y + pt1->vel.y*time + (1/2.0)*pt1->acc.y*time*time;
        pt1->z = pt1->z + pt1->vel.z*time + (1/2.0)*pt1->acc.z*time*time;
        pt1->t = currTime;
    }

    void update_motion(Point* pt1,int currTime,axis force1)
    {
        update_acc(pt1,force1);
        update_vel(pt1,currTime);
        update_pos(pt1,currTime);
    }

    void wallCollision(Point* x,int box_xWidth,int box_yWidth,int boxWidth)
    {
            if(x->x >= box_xWidth - boxWidth)
            {
                x->x = (box_xWidth - boxWidth) - (x->x - (box_xWidth - boxWidth));
                x->vel.x = -(x->vel.x);
            }
            else if(x->x <= 0 + boxWidth)
            {
                x->x = boxWidth - (x->x - boxWidth);
                x->vel.x = -(x->vel.x);
            }

            if(x->y >= box_yWidth - boxWidth)
            {
                x->y = (box_yWidth- boxWidth) - (x->y - (box_yWidth - boxWidth));
                x->vel.y = -(x->vel.y);
            }
            else if(x->y <= 0 + boxWidth)
            {
                x->y = boxWidth - (x->y - boxWidth);
                x->vel.y = -(x->vel.y);
            }
    }
    
    void collisionHandler(Point* pt1, Point* pt2)
    {
        //v1_ = ((m1 - m2)v1 + 2m2v2)/m1+m2
        //v2_ = (m1*v1 + m2*v2 - m1*v1_ )/m2
        double v1_x = ((pt1->p_mass - pt2->p_mass)*pt1->vel.x + 2*pt2->p_mass*pt2->vel.x)/(pt1->p_mass + pt2->p_mass);
        pt2->vel.x = (pt1->p_mass*(pt1->vel.x - v1_x) + pt2->p_mass*pt2->vel.x ) / pt2->p_mass;         
        pt1->vel.x = v1_x;
        double v1_y = ((pt1->p_mass - pt2->p_mass)*pt1->vel.y + 2*pt2->p_mass*pt2->vel.y)/(pt1->p_mass + pt2->p_mass);
        pt2->vel.y = (pt1->p_mass*(pt1->vel.y - v1_y) + pt2->p_mass*pt2->vel.y ) / pt2->p_mass;         
        pt1->vel.y = v1_y;
        double v1_z = ((pt1->p_mass - pt2->p_mass)*pt1->vel.z + 2*pt2->p_mass*pt2->vel.z)/(pt1->p_mass + pt2->p_mass);
        pt2->vel.z = (pt1->p_mass*(pt1->vel.z - v1_z) + pt2->p_mass*pt2->vel.z ) / pt2->p_mass;         
        pt1->vel.z = v1_z;
    }

    void Update(vector<Point*> allPts,long long tick)
    {
        for(int i = 0; i < allPts.size(); i++)
        {
                update_motion(allPts[i],tick/timeScale,envForce);
                wallCollision(allPts[i],box_xW,box_yW,boxWidth);
                for(int j = i+1; j < allPts.size(); j++)
                {
                    if(allPts[i]->compare(allPts[j]) == 1)
                    {
                        pair<Point*,Point*> p1{allPts[i],allPts[j]};
                        if(collisionTime[p1] == 0)
                        {
                            //cout << "1st Collision Detected:" << tick/timeScale << endl;
                            collisionHandler(allPts[i],allPts[j]);
                            collisionTime[p1] = tick/timeScale;                            
                        }
                        else if(collisionTime[p1] + collisionDelta < tick/timeScale )
                        {
                            //cout << "Collision Detected:" << tick/timeScale << endl;
                            collisionHandler(allPts[i],allPts[j]);
                            collisionTime[p1] = tick/timeScale;
                        }
                        else
                        {
                            //cout << "Collision Factor:" <<collisionTime[p1] << " " << collisionDelta << " " << tick/timeScale << endl;
                        }
                    }
                }          
        }
    }
};