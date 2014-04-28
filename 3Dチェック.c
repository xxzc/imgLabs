#include <stdio.h>
#include <SDL.h>
#include <math.h>

#define FP 4
/*>......FIXED POINT DEF..........<*/
typedef int fixed;

#define NFIX(a,b) (a)<<FP>>(b)
#define INT2FIX(a) (a)<<FP
#define FIX2INT(a) (a)>>FP
#define DOUBLE2FIX(a) (fixed)((a)*(1<<FP))
#define FIX2DOUBLE(a) ((double)(a))/(1<<FP)

#define FADD(a,b) (a)+(b)
#define FSUB(a,b) (a)-(b)
#define FMUL(a,b) (a)*(b)>>FP
#define FDIV(a,b) ((a)<<FP)/(b)
/*.....*>_<*......FIN...............*/

#define WIDTH 1023
#define HEIGHT 719
#define BPP 4
#define DEPTH 32

SDL_Surface *screen;
int t=0;
#define col(r,g,b) SDL_MapRGB(screen->format,(r),(g),(b))

void putPixel(int x, int y, int color)
{
    
    *((Uint32*) screen->pixels + y*screen->pitch/BPP + x)=color;
}

int norm(fixed p,fixed lo,fixed hi)
{return FIX2INT(FDIV((p-lo),(hi-lo))*255);}
void DrawScreen(SDL_Surface* screen)
{ 
	int white = col(255,255,255),
		black = col(0,0,0);
	int x,y;
	int lev;
	fixed k=DOUBLE2FIX(0.1),w=DOUBLE2FIX(0.1);
	fixed h=DOUBLE2FIX(3.0);
	fixed xs,ys;
	fixed xv,yv;
    for(y=0;y<HEIGHT;y++)
        for(x=0;x<WIDTH;x++){
        	xs=(double)x/(double)WIDTH*2.0-1.0;
        	ys=(double)y/(double)HEIGHT*2.0-1.0;
			h=3.0+3.0*cos(0.1*t);
			xv=h*xs/ys;yv=h/ys;
            if(ys>0) lev=norm(sin(xv*cos(w*t)-yv*sin(w*t))
							 *sin(xv*sin(w*t)+yv*cos(w*t)),-1.0,1.0);
            else lev=0;
            putPixel(x,y,col(lev,lev,lev));
        }
    SDL_Flip(screen); 
}

int main(int argc, char* argv[])
{
    SDL_Event event;  
    int keypress = 0;
    int h=0; 
    if (SDL_Init(SDL_INIT_VIDEO) < 0 ) return 1;
    if (!(screen = SDL_SetVideoMode(WIDTH, HEIGHT, DEPTH, SDL_HWSURFACE)))
    {
        SDL_Quit();
        return 1;
    }
    while(!keypress) 
    {
         DrawScreen(screen);t++;
         while(SDL_PollEvent(&event)) 
         {      
              switch (event.type) 
              {
                  case SDL_QUIT:
	              keypress = 1;
	              break;
                  case SDL_KEYDOWN:
                  break;
              }
         }
    }
    SDL_Quit();
    return 0;
}
