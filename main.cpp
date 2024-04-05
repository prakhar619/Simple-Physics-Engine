#include <stdio.h>
#include <SDL.h>
#include <iostream>
#include <vector>
using namespace std;
// #include "object.cpp"

#define GREY Color{150,150,150,255}
#define RED Color{255,0,0,255}
#define GREEN Color{0,255,0,255}
#define BLUE Color{0,0,255,255}
#define WHITE Color{255,255,255,255}
#define BLACK Color{0,0,0,255}
#define CELERIAN_BLUE Color{40,40,40,255}

#define GFORCE axis{0,10,0}
#define EMPTY axis{0,0,0}
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
class Point : public Color, public spaceTime
{
    public:
    double p_mass;

    axis force;
    axis vel {0,0,0};
    axis acc {0,0,0};

    Point(spaceTime point, double mass, Color p_Color)
    {
        updateColor(p_Color);
        update_spaceTime(point);
        p_mass = mass;

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

class Physics
{
    public:
    int initialised = 0;

    int box_xW,box_yW,boxWidth;

    int timeScale;
    int flg_envForce = 0;
    axis envForce {0,0,0};

    void init(int timeScale,int box_xW,int box_yW,int boxWidth)
    {
        this->timeScale = timeScale;
        this->box_xW = box_xW;
        this->box_yW = box_yW;
        this->boxWidth = boxWidth;
        initialised = 1;
    }

    void toogleGForce()
    {
        if(flg_envForce == 0)
        envForce.update_axis(0,10,0);
        else
        envForce.update_axis(0,0,0);
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
        cout << "OLD VEL P1:" << pt1->vel.x << endl;
        cout << "OLD VEL P2:" << pt2->vel.x << endl;
        //v1_ = ((m1 - m2)v1 + 2m2v2)/m1+m2
        double v1_ = ((pt1->p_mass - pt2->p_mass)*pt1->vel.x + 2*pt2->p_mass*pt2->vel.x)/(pt1->p_mass + pt2->p_mass);
        //v2_ = (m1*v1 + m2*v2 - m1*v1_ )/m2
        pt2->vel.x = (pt1->p_mass*(pt1->vel.x - v1_) + pt2->p_mass*pt2->vel.x ) / pt2->p_mass;         
        pt1->vel.x = v1_;
        cout << "NEW VEL P1:" << pt1->vel.x << endl;
        cout << "NEW VEL P2:" << pt2->vel.x << endl;

    }

    void Update(vector<Point*> allPts,long long tick)
    {
        for(Point* x: allPts)
        {
                update_motion(x,tick/timeScale,envForce);
                wallCollision(x,box_xW,box_yW,boxWidth);
        }
    }
};

void box(SDL_Renderer* renderer,int screenWidth,int screenLength,Color boxColor,int width)
{
    Color boxBoundary {GREY};
    SDL_SetRenderDrawColor(renderer,boxBoundary.r,boxBoundary.g,boxBoundary.b,boxBoundary.alpha);
    SDL_Rect outer{0,0,screenWidth,screenLength};
    SDL_RenderFillRect(renderer, &outer);

    SDL_SetRenderDrawColor(renderer,boxColor.r,boxColor.g,boxColor.b,boxColor.alpha);
    SDL_Rect inner{width,width,screenWidth-(2*width),screenLength-(2*width)};
    SDL_RenderFillRect(renderer, &inner);

    return ;
}


int main(int argc, char* argv[])
{
    //initialising window, rendered, event
        SDL_Window* window = NULL;
        SDL_Renderer* renderer = NULL;
        SDL_Event e;
        bool running = true;


    //SDL_Surface* screenSurface = NULL;


    //SDL_Init working or not
        if( SDL_Init( SDL_INIT_VIDEO ) < 0)
        {
            printf("SDL could not initialize! SDL_Error:%s\n",SDL_GetError() );
            return 1;
        }

    //creating window 
        int screenWidth = 1400;
        int screenLength = (int)  screenWidth*(9.0/16.0);
        //Window Pos, size, showORnot
        window = SDL_CreateWindow("Physics Engine",SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenWidth, screenLength,SDL_WINDOW_SHOWN);
        renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);
        


    //Envirnoment
        int logicalScale = 5;
        int timeScale = 100;

        vector<Point*> allPoints;
        int boxWidth = 20/logicalScale;
        Color pColor(WHITE);
        Color currBoxColor(CELERIAN_BLUE);


        int box_xWidth = screenWidth/logicalScale;
        int box_yWidth = screenLength/logicalScale;

        Physics phy;
        phy.init(timeScale,box_xWidth,box_yWidth,boxWidth);
    //Scaling 
        //SDL_RenderSetLogicalSize(renderer,screenWidth/2,screenLength/2);
        SDL_RenderSetScale(renderer,logicalScale,logicalScale);

    //main looping
        while(running)
        {

            //Event handling //interaction
                while(SDL_PollEvent(&e))
                {
                    int last_x,last_y;

                    //close button implemented
                        if(e.type == SDL_QUIT)
                        {
                            running =  false;
                            // for(Point* x: allPoints)
                            // {
                            //     x->printPt();
                            // }
                        }

                    //Keyboard interaction
                        else if(e.type == SDL_KEYDOWN)
                        {
                            switch(e.key.keysym.sym)
                            {
                                case SDLK_r:
                                    pColor.updateColor(RED);
                                    break;
                                case SDLK_g:
                                    pColor.updateColor(GREEN);
                                    break;
                                case SDLK_b:
                                    pColor.updateColor(BLUE);
                                    break;
                            }
                        }
                        else if(e.type == SDL_KEYUP)
                        {
                            switch(e.key.keysym.sym)
                            {
                                case SDLK_RIGHT:
                                    cout <<"Right KEY was released" << endl;
                                    break;
                                case SDLK_r:
                                case SDLK_g:
                                case SDLK_b:
                                    pColor.updateColor(WHITE);
                                    break;
                                case SDLK_f:
                                    phy.toogleGForce();
                                    break;

                            }
                        }
                    //Mouse Interaction
                        else if(e.type == SDL_MOUSEBUTTONDOWN)
                        {   
                            switch(e.button.button)
                            {
                                case SDL_BUTTON_LEFT:
                                    SDL_GetMouseState(&last_x,&last_y);
                                    last_x /= logicalScale;
                                    last_y /= logicalScale;
                                    break;
                            }
                        }
                        else if(e.type == SDL_MOUSEBUTTONUP)
                        {
                            int mouse_x,mouse_y;
                            SDL_GetMouseState(&mouse_x,&mouse_y);
                            mouse_x /= logicalScale;
                            mouse_y /= logicalScale;
                            switch(e.button.button)
                            {
                                case SDL_BUTTON_LEFT:
                                Point* p = new Point(spaceTime(last_x,last_y,0,SDL_GetTicks64()/phy.timeScale),10,pColor);
                                p->vel = {mouse_x-last_x,mouse_y - last_y,0};
                                allPoints.push_back(p);
                                break;   
                            }
                        }

            }
            
            SDL_SetRenderDrawColor(renderer,0,0,0,0);
            SDL_RenderClear(renderer);

            box(renderer,box_xWidth,box_yWidth,currBoxColor,boxWidth);
            for(Point* x : allPoints)
            {
                SDL_SetRenderDrawColor(renderer,x->r, x->g, x->b, x->alpha);
                SDL_RenderDrawPoint(renderer,x->x,x->y);
            }

            //Physics 
            phy.Update(allPoints,SDL_GetTicks64());
        
            //Box interaction


            SDL_RenderPresent(renderer);

            
        }




    return 0;
}