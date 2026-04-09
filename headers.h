#ifndef HEADERS_H
#define HEADERS_H

#include <vector>
#include <iostream>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>








class SDLinit{
    private:
        SDL_Window* window;
        SDL_Renderer* renderer;
        TTF_Font* font;
    public:
        SDLinit(const std::string title,int w,int h);
        ~SDLinit();
        void clear();
        void present();
        void drawbut(int x,int y,int w,int h,int r,int g,int b,const std::string &text);


};

SDLinit::SDLinit(const std::string title,int w,int h){
    SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO);
    window=SDL_CreateWindow(title.c_str(),SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,w,h,SDL_WINDOW_SHOWN);
    renderer=SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);
    TTF_Init();
    font=TTF_OpenFont("font.ttf",50);
    

}
SDLinit::~SDLinit(){
    if(renderer){SDL_DestroyRenderer(renderer);}
    if(window){SDL_DestroyWindow(window);}
    if(font){TTF_CloseFont(font);}
    TTF_Quit();
    SDL_Quit();
    
}
void SDLinit::clear(){
    if (renderer){
        SDL_SetRenderDrawColor(renderer,156,90,60,255);
        SDL_RenderClear(renderer);
    }
}
void SDLinit::present(){
    if(renderer){
        SDL_RenderPresent(renderer);
    }
}


void SDLinit::drawbut(int x,int y,int w,int h,int r,int g,int b,const std::string &text){
    
    SDL_SetRenderDrawColor(renderer,r,g,b,255);
    SDL_Rect rect6={x,y,w,h};
    SDL_RenderFillRect(renderer,&rect6);
    SDL_SetRenderDrawColor(renderer,0,0,0,255);
    SDL_RenderDrawRect(renderer,&rect6);
    SDL_Color white = {120,120,120,255};
    SDL_Surface* surf=TTF_RenderText_Solid(font,text.c_str(),white);
    SDL_Texture* tex=SDL_CreateTextureFromSurface(renderer,surf);
    SDL_Rect rect5={x+27,y+10,w-50,h-30};
    SDL_RenderCopy(renderer,tex,NULL,&rect5);
    SDL_FreeSurface(surf);
    SDL_DestroyTexture(tex);
    
}


class uinter{
    private:
        SDLinit& sdl;
         //  using 1 for live audio and 2 for file system
    public:
        uinter(SDLinit& s):sdl(s){}
        void handel(SDL_Event event,int* mode);
        void layout(int* mode);
        bool checkmouse(int x,int y,int lx,int rx,int upy,int dwy);

};
void uinter::layout(int* mode){
    if (*mode==-1){
        sdl.drawbut(150,150,400,150,107, 62, 46,"live audio");
        sdl.drawbut(600,150,400,150,245, 230, 211,"file?");

        
    }
}
bool uinter::checkmouse(int x,int y,int lx,int rx,int upy,int dwy){
    if(x>lx && x<rx && y>upy && y<dwy){
        return true;
    }
    return false;
}
void uinter::handel(SDL_Event event,int* mode){
    if(*mode==-1){
        if (event.type==SDL_MOUSEBUTTONDOWN){
            int mousex=event.button.x;
            int mousey=event.button.y;
            if(checkmouse(mousex,mousey,150,550,150,300)){
                *mode=1;
            }else if (checkmouse(mousex,mousey,600,1000,150,300)){
                *mode=2;
            }
        }
    }
    if(*mode==1 || *mode==2){
        if(event.type==SDL_KEYDOWN){
            SDL_Keycode key=event.key.keysym.sym;
            if (key==SDLK_ESCAPE){
                *mode=-1;
            }

        }
    }
}

























#endif