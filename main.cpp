#include "headers.h"





int main(){
    SDLinit sdl("fuckass visualizer" , 1280,720);
    uinter uic(sdl);
    bool running =true;
    SDL_Event event;
    int mode=-1;
    while(running){
        while(SDL_PollEvent(&event)){
            if(event.type==SDL_QUIT){
                running=false;
                sdl.~SDLinit();
            }
            
            uic.handel(event,&mode);
        }
        sdl.clear();
        uic.layout(&mode);
        sdl.present();
    }
    return 0;
}
