#include "myTimers.h"



void Clock_Tick(){

    myGlobalVariables.currentTime = millis(); //increment the current timer

    if((myGlobalVariables.currentTime - myGlobalVariables.previousTime) >= 10){ //1/100th of a second
        
        myGlobalVariables.SystemTime++; //increment the system time to keep up with the other units
        myGlobalVariables.previousTime = myGlobalVariables.currentTime;

        //Serial.println("Tick Tock?");

        //CheckForNewNtripPacket();
        

    }

}

/*
void hundredthSecond(){
    
    myGlobalVariables.hundredthSecond++;

    //Serial.println(myGlobalVariables.currentTime);

    if(myGlobalVariables.hundredthSecond >= 10){
        
        myGlobalVariables.hundredthSecond = 0;
        tenthSecond();

    }

}

void tenthSecond(){

    myGlobalVariables.tenthSecond++;

    if(myGlobalVariables.tenthSecond >= 10){
        
        myGlobalVariables.tenthSecond = 0;
        oneSecond();

    }

}


void oneSecond(){
    RefreshTimeOverNetwork();
}


void RefreshTimeOverNetwork(){

    myGlobalVariables.currentTime = millis();
    MyEspNowData.messageTimeStamp = myGlobalVariables.currentTime;

    SendOutNowBroadcast(1);

   

}
*/

