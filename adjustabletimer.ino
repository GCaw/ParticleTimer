#include "InternetButton/InternetButton.h"
#include "math.h"

InternetButton b = InternetButton();

int chronoSet = 4; //note on startup button 4 is depressed
long startTime = 0;
long flashtime = 0;
bool flashing = 0;
int onLEDs = 0;
int state = 0; /*
0 = startup, can adjsut
1 = running
2 = time up
3 = off
*/ 
bool buttons [] = {false, false, false, false, false};

void setup() {
    b.begin();
    b.setBrightness(50);
    b.playNote("G4",8);
}

void loop(){

    if (b.buttonOn(1)){
        buttons[1] = true;
    }
    if (b.buttonOn(2)){
        buttons[2] = true;
    }
    if (b.buttonOn(3)){
        buttons[3] = true;
    }
    if (b.buttonOn(4)){
        buttons[4] = true;
    }

    switch (state){
        case 0:
            b.ledOn (1,0,0,255); // status indicator
            for (int i=2; i<=chronoSet+1;i++)
            {
                b.ledOn (i,0,230,230);
            }
            if (!b.buttonOn(4) && buttons[4]){
                chronoSet++;
                buttons[4] = false;
            }
            if (!b.buttonOn(1) && buttons[1]){
                state = 3;
                buttons[1] = false;
                b.allLedsOff();
            }
            if (!b.buttonOn(2) && buttons[2]){
                chronoSet--;
                buttons[2] = false;
                b.allLedsOff();
            }
            if (!b.buttonOn(3) && buttons[3]){
                state = 1;
                b.allLedsOff();
                buttons[3] = false;
                startTime = millis();
                onLEDs = chronoSet;
                flashtime = 0;
                flashing = true;
            }
            break;
        case 1:
            b.ledOn (1,0,0,255); // status indicator            
            if ((startTime + (chronoSet*30000)) < millis()){
                state = 2;
                onLEDs = 0;
                b.setBrightness(100);
               // b.playNote("G4",16);// playing a note here causes the photon to crash. not sure why.
            }
            else{
                onLEDs = ceil(((startTime + (chronoSet*30000)) - millis())/30000)+1;
                if (onLEDs>1){    
                    for (int i = 2; i <= onLEDs;i++ ){
                        b.ledOn (i,120,120,120);
                    }
                }
                if (flashtime < millis())
                {
                    flashtime = millis() + 250;
                    if (flashing)
                    {
                        b.ledOn (onLEDs+1,120,120,120);
                        flashing = false;
                    }
                    else
                    {
                        b.allLedsOff();
                        flashing = true;
                    }
                }
                if (!b.buttonOn(1) && buttons[1]){
                    state = 0;
                    buttons[1] = false;
                    b.allLedsOff();
                }
            }

            break;
        case 2:
            if (flashtime < millis())
            {
                flashtime = millis() + 250;
                if (flashing)
                {
                    b.allLedsOn(255,0,0);
                    b.playNote("G4",16);
                    flashing = false;
                }
                else
                {
                    b.allLedsOff ();
                    flashing = true;
                }
            }
             
             if (!b.buttonOn(3) && buttons[3]){
                buttons[3] = false;
                b.allLedsOff();
                state= 0;
                b.setBrightness(50);
             }
            break;
        case 3:
            if (!b.buttonOn(1) && buttons[1]){
                state = 0;
                buttons[1] = false;
            }
            break;
        default:
            
            break;
    }
    
    if (chronoSet == 0){
        chronoSet = 1;
    }
    if (chronoSet == 11){
        chronoSet = 10;
    }

}
