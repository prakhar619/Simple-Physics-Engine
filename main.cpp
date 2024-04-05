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
    axis vel {10,0,0};
    axis acc {0,0,0};

    Point(spaceTime point, double mass, Color p_Color)
    {
        updateColor(p_Color);
        update_spaceTime(point);
        p_mass = mass;

    }

    void update_vel(int currTime)
    {
        int time = currTime - t;
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

    void update_all(int currTime,axis force1)
    {
        update_acc(force1);
        update_vel(currTime);
        update_pos(currTime);
    }

    void printPt()
    {
        cout << "Pos:" << x << " " << y << " " << z << endl;
        cout << "Mass:" << p_mass << endl;
        cout << "Color:" << r << " " << g << " " << b << endl; 
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
        
    //SDL_RenderSetLogicalSize(renderer,50,50);
    //SDL_RenderSetScale(renderer,2,2);


    //Envirnoment
        vector<Point*> allPoints;
        int timeScale = 100;
        int boxWidth = 20;
        Color pColor(WHITE);
        Color currBoxColor(CELERIAN_BLUE);

        axis envForce(EMPTY);
        int gforce = 0;
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
                                    if(gforce == 0)
                                    {
                                        envForce.update_axis(GFORCE);
                                        gforce = 1;
                                    }
                                    else
                                    {
                                        envForce.update_axis(EMPTY);
                                        gforce = 0;
                                    }
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
                            int mouse_x,mouse_y;
                            SDL_GetMouseState(&mouse_x,&mouse_y);
                            switch(e.button.button)
                            {
                                case SDL_BUTTON_LEFT:
                                Point* p = new Point(spaceTime(last_x,last_y,0,SDL_GetTicks64()/timeScale),10,pColor);
                                p->vel = {mouse_x-last_x,mouse_y - last_y,0};
                                allPoints.push_back(p);
                                break;   
                            }
                        }

            }
            
            SDL_SetRenderDrawColor(renderer,0,0,0,0);
            SDL_RenderClear(renderer);

            box(renderer,screenWidth,screenLength,currBoxColor,boxWidth);
            for(Point* x : allPoints)
            {
                SDL_SetRenderDrawColor(renderer,x->r, x->g, x->b, x->alpha);
                SDL_RenderDrawPoint(renderer,x->x,x->y);
            }

            //Pos 
            for(Point* x: allPoints)
            {
                x->update_all(SDL_GetTicks64()/timeScale,envForce);

            }
        
            //Box interaction
            for(Point* x: allPoints)
            {
                if(x->x >= screenWidth - boxWidth)
                {
                    x->x = (screenWidth - boxWidth) - (x->x - (screenWidth - boxWidth));
                    x->vel.x = -(x->vel.x);
                }
                else if(x->x <= 0 + boxWidth)
                {
                    x->x = boxWidth - (x->x - boxWidth);
                    x->vel.x = -(x->vel.x);
                }

                if(x->y >= screenLength - boxWidth)
                {
                    x->y = (screenLength - boxWidth) - (x->y - (screenLength - boxWidth));
                    x->vel.y = -(x->vel.y);
                }
                else if(x->y <= 0 + boxWidth)
                {
                    x->y = boxWidth - (x->y - boxWidth);
                    x->vel.y = -(x->vel.y);
                }
            }

            SDL_RenderPresent(renderer);

            
        }




    return 0;
}