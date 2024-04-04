#include <stdio.h>
#include <SDL.h>
#include <iostream>
#include <vector>
using namespace std;
// #include "object.cpp"

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
        updateColor(Obj);
    }


    void updateColor(int rx,int gx,int bx,int alphax)
    {
        if(r > 255 || g > 255 || b > 255 || alpha > 255)
        {
            cout << "Error. rgb cannot be greater than 255\n" << endl;
        }
        r = rx;
        g = gx;
        b = bx;
        alpha = alphax;

    }
    void updateColor(Color &cpyObj)
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
        update_spaceTime(Obj);
    }

    void update_spaceTime(int x,int y,int z,int t)
    {
        this->x = x;
        this->y = y;
        this->z = z;
        this->t = t;
    }
    void update_spaceTime(spaceTime &cpyObj)
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
    void update_axis(axis &a)
    {
        x= a.x;
        y= a.y;
        z= a.z;
    }
};
class Point : public Color, public spaceTime
{
    public:
    double p_mass;

    axis force;
    axis vel {10,0,0};
    axis acc {0,0,0};

    Point(spaceTime point, double mass, Color p_Color)
    {
        updateColor(p_Color);
        update_spaceTime(point);
        p_mass = mass;

    }

    void update_vel(int time)
    {
        vel.x = vel.x + acc.x*time;
        vel.y = vel.y + acc.y*time;
        vel.z = vel.z + acc.z*time;
    }

    void update_acc(axis force1)
    {
        force.update_axis(force1);
        acc.x = force.x/p_mass;
        acc.y = force.y/p_mass;
        acc.z = force.z/p_mass;
    }

    void update_pos(int currTime)
    {
        int time = currTime - t;
        x = x + vel.x*time + (1/2.0)*acc.x*time*time;
        y = y + vel.y*time + (1/2.0)*acc.y*time*time;
        z = z + vel.z*time + (1/2.0)*acc.z*time*time;
        t = currTime;
    }

    void printPt()
    {
        cout << "Pos:" << x << " " << y << " " << z << endl;
        cout << "Mass:" << p_mass << endl;
        cout << "Color:" << r << " " << g << " " << b << endl; 
    }

};


void box(SDL_Renderer* renderer,int screenWidth,int screenLength)
{
    SDL_SetRenderDrawColor(renderer,255,255,255,255);
    SDL_Rect outer{0,0,screenWidth,screenLength};
    SDL_RenderFillRect(renderer, &outer);

    SDL_SetRenderDrawColor(renderer,0,0,0,255);
    SDL_Rect inner{20,20,screenWidth-20,screenLength-20};
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
        renderer = SDL_CreateRenderer(window,-1,0);
        
        
    //SDL_RenderSetScale(renderer,4,4);


    //Envirnoment
    vector<Point*> allPoints;
    int timeScale = 100;

    box(renderer,screenWidth,screenLength);


    //main looping
        while(running)
        {

            //Event handling //interaction
                while(SDL_PollEvent(&e))
                {
                    int last_x,last_y;
                    int mouse_x,mouse_y;
                    SDL_GetMouseState(&mouse_x,&mouse_y);
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
                                case SDLK_RIGHT:
                                    cout << "Right KEY was pressed" << endl;
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
                            }
                        }
                    //Mouse Interaction
                        else if(e.type == SDL_MOUSEBUTTONDOWN)
                        {   
                            switch(e.button.button)
                            {
                                case SDL_BUTTON_LEFT:
                                    SDL_GetMouseState(&last_x,&last_y);
                                    break;
                            }
                        }
                        else if(e.type == SDL_MOUSEBUTTONUP)
                        {
                            switch(e.button.button)
                            {
                                case SDL_BUTTON_LEFT:
                                Point* p = new Point(spaceTime(last_x,last_y,0,SDL_GetTicks64()/timeScale),10,Color(244,244,244,244));
                                p->vel = {mouse_x-last_x,mouse_y - last_y,0};
                                allPoints.push_back(p);
                                break;   
                            }
                        }

            }
                SDL_SetRenderDrawColor(renderer,0,0,0,0);
                SDL_RenderClear(renderer);

            for(Point* x : allPoints)
            {
                SDL_SetRenderDrawColor(renderer,x->r, x->g, x->b, x->alpha);
                SDL_RenderDrawPoint(renderer,x->x,x->y);
            }
            for(Point* x: allPoints)
            {
                x->update_pos(SDL_GetTicks64()/timeScale);
            }
            box(renderer,screenWidth,screenLength);
            \
            SDL_RenderPresent(renderer);
        }




    return 0;
}