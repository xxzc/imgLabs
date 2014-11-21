#include <stdio.h>
#include <SDL.h>
#include <math.h>

#define WIDTH 1023
#define HEIGHT 500
#define BPP 4
#define DEPTH 32
#define fun(a,x) (a)*((x)-(x)*(x))
#define derr(a,x) ((a)-2*a*(x))
//#define fun(a,x) (a)*sin((x)*3.14159)/4
SDL_Surface *screen;

#define col(r,g,b) SDL_MapRGB(screen->format,(r),(g),(b))
#define WHITE col(0xFF,0xFF,0xFF)
#define BLACK col(0x0,0x0,0x0)

#define NMAXCELL 100*HEIGHT
double origPos[NMAXCELL],afterPos[NMAXCELL];
double afterDerr[NMAXCELL];
int nPeriod[NMAXCELL],nxcell;
int nacell;
 
void origInit()
{
	int i;
	for(i=0;i<nxcell;i++)
		origPos[i] = (double)i/(double)nxcell;/*0..1*/
}
void periodCalc(double na)
{
	const int MAXPERO = 500;
	
	int p,t;
	for(p=0;p<nxcell;p++){
		afterPos[p] = origPos[p];
		nPeriod[p]=0;
		afterDerr[p]=1.0;
	}
	for(t=1;t<=MAXPERO;t++){
		for(p=0;p<nxcell;p++){
			afterPos[p] = fun(na,afterPos[p]);
			afterDerr[p] = afterDerr[p]*derr(na,afterPos[p]);
		}
		for(p=0;p<nxcell-1;p++)
			if(nPeriod[p]==0&&
			   (afterPos[p]-origPos[p])>0 ^
			   (afterPos[p+1]-origPos[p+1])>0
			   //(afterPos[p+1]>origPos[p])
			   //(afterPos[p]<origPos[p+1])
			   ){
				nPeriod[p] = t;
				if(fabs(afterDerr[p])>1) nPeriod[p] = -1;
			}
	}
	for(p=0;p<nxcell;p++) if(nPeriod[p]==-1) nPeriod[p]=0;
}
void putPixel(int x, int y, int color)
{
    
    *((Uint32*) screen->pixels + y*screen->pitch/BPP + x)=color;
}

int norm(double p,double lo,double hi)
{return (int)((p-lo)/(hi-lo)*255);}
void DrawScreen(SDL_Surface* screen)
{ 
	static int done = 0;
	if(done) return;
	
	int nxsub=30;
	double as = 1;
	nacell = nxsub*WIDTH; nxcell = nxsub*HEIGHT;
	origInit();
	int x,y;
    for(x=0;x<WIDTH;x++){
		for(y=0;y<HEIGHT;y++)
					putPixel(x,y,WHITE);
		int xsub;
		for(xsub=0;xsub<nxsub;xsub++){
			periodCalc(as+(4.0-as)*(float)(x*nxsub+xsub)/(float)nacell);/*0...4*/
			for(y=0;y<HEIGHT;y++){
					int psub;
					for(psub=0;psub<nxsub;psub++)
						if(nPeriod[y*nxsub+psub]){
							putPixel(x,HEIGHT-1-y,BLACK);
							break;
						}
			}
			SDL_Flip(screen); 
		}
    }
    done = 1;
    SDL_SaveBMP(screen, "chaos.bmp");
    printf("BMP Saved Succesful.\n");
}

int main(int argc, char* argv[])
{
    SDL_Event event;  
    int keypress = 0;
    int h=0; 
    if (SDL_Init(SDL_INIT_VIDEO) < 0 ) return 1;
    if (!(screen = SDL_SetVideoMode(WIDTH, HEIGHT, DEPTH,SDL_HWSURFACE)))
    {
        SDL_Quit();
        return 1;
    }
    while(!keypress) 
    {
         DrawScreen(screen);
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
