#include "headers.h"





int main(){
    SDLinit sdl("fuckass visualizer" , 1280,720);
    bool running =true;
    SDL_Event event;
    while(running){
        while(SDL_PollEvent(&event)){
            if(event.type==SDL_QUIT){
                SDL_Quit();
            }
            sdl.clear();
            sdl.present();

        }
    }
    return 0;
}
