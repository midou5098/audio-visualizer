#ifndef HEADERS_H
#define HEADERS_H

#include <vector>
#include <iostream>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <fftw3.h>
#include <cmath>
#include <algorithm>

int sample_rate=44100,format=32,channels=1,buf_size=1024;





class SDLinit{
    private:
        SDL_Window* window;
        SDL_Renderer* renderer;
        TTF_Font* font;
    public:
        SDL_Renderer* getrenderer(){return renderer;}
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
class audiocap{
    private:
    SDL_AudioDeviceID device;
    fftw_plan plan=nullptr;
    double* fftw_input=nullptr;
    fftw_complex* fftw_output=nullptr;
    public:
        float buffer[1024];
        float bars[50];
        
        audiocap();
        void startmic();
        void processfft();
        static void callback(void* userdata,Uint8* stream,int len);
        void mode1();
        

};

class uinter{
    private:
        SDLinit& sdl;
        audiocap& cap;
         //  using 1 for live audio and 2 for file system
    public:
        uinter(SDLinit& s,audiocap& c):sdl(s),cap(c){}
        void handel(SDL_Event event,int* mode);
        void layout(int* mode);
        bool checkmouse(int x,int y,int lx,int rx,int upy,int dwy);

};
void uinter::layout(int* mode){
    if (*mode==-1){
        sdl.drawbut(150,150,400,150,107, 62, 46,"live audio");
        sdl.drawbut(600,150,400,150,245, 230, 211,"file?");

        
    }else if (*mode==1){
        int xb=50;
        for (int i=0;i<50;i++){
            float h=cap.bars[i];
            if(h > 720) h = 720;
            if(h < 0)   h = 0;
            SDL_Rect rect={xb,(int)(720-h),5,(int)(h)};
            SDL_Renderer* renderer=sdl.getrenderer();
            SDL_SetRenderDrawColor(renderer,0,0,0,255);
            SDL_RenderFillRect(renderer,&rect);
            xb+=24;
        }
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
                std::cout << "clicked at x=" << event.button.x << " y=" << event.button.y << std::endl;
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



audiocap::audiocap(){
    memset(bars,0,sizeof(bars));
    memset(buffer,0,sizeof(buffer));
    fftw_input=fftw_alloc_real(1024);
    fftw_output=fftw_alloc_complex(513);
}
void audiocap::startmic(){
    SDL_AudioSpec temp{};
    temp.freq= 44100;
    temp.format   = AUDIO_F32;
    temp.channels = 1;
    temp.samples  = 1024;
    temp.callback = audiocap::callback;
    temp.userdata = this;
    plan = fftw_plan_dft_r2c_1d(
            1024, 
            fftw_input,   
            fftw_output,  
            FFTW_ESTIMATE 
        );
    device=SDL_OpenAudioDevice(nullptr,1,&temp,nullptr,0);
    SDL_PauseAudioDevice(device,0);
    if(device == 0){
        std::cout << "mic failed: " << SDL_GetError() << std::endl;
    }else{
        std::cout << "mic opened ok, device id: " << device << std::endl;
    }

}



//peak
void audiocap::callback(void* userdata,Uint8* stream,int len){
    audiocap* self=(audiocap*)userdata;
    float* samples = (float*)stream;
    int count= len/sizeof(float);
    for(int i=0;i<std::min(count, 1024);i++){
        self->buffer[i]=samples[i];
        
    }
}

void audiocap::processfft(){
    if(!plan){return;}
    for(int i=0;i<1024;i++){
        double hann =0.5*(1.0-cos(2.0*M_PI*i/1023.0));
        fftw_input[i]=buffer[i]*hann;
    }
    fftw_execute(plan);
    for(int b=0;b<50;b++){
        float low_freq=20.0*pow(1000.0f,float(b)/50.0f);
        float high_freq=20.0*pow(1000.0f,float(b+1)/50.0f);
        int bin_low= (int)(low_freq/43.0f);
        int bin_high= (int)(high_freq/43.0f);
        bin_low  = std::max(1, std::min(bin_low,  512));
        bin_high = std::max(1, std::min(bin_high, 512));
        std::cout << "b=" << b << " bin_low=" << bin_low << " bin_high=" << bin_high << std::endl;
        float sum=0;
        for(int h=bin_low;h<=bin_high;h++){
            double real=fftw_output[h][0];
            double img=fftw_output[h][1];
            sum+=sqrt(real*real+img*img);
        }
        float raw=sum/(bin_high- bin_low +1);
        raw = log10f(raw + 150.0f) * 50.0f;
        bars[b]=bars[b]*0.8+raw*0.2f;

        }
    }






















#endif