#include "headers.h"





int main(){
    bool mon=false;
    SDLinit sdl("fuckass visualizer" , 1280,720);
    audiocap cap;
    uinter uic(sdl,cap);
    bool running =true;
    SDL_Event event;
    int mode=-1;
    while(running){
        if (mode==1 && mon==false){
            cap.startmic();
            mon=true;}
        while(SDL_PollEvent(&event)){
            if(event.type==SDL_QUIT){
                running=false;
            }
            uic.handel(event,&mode);
            

        }
        sdl.clear();
        if(mode==1){
            cap.processfft();
        }
        uic.layout(&mode);
        sdl.present();
    }
    return 0;
}
