#include <stdio.h>
#include <time.h>

extern "C" char get_pixel(int row,int col,int colour);
extern "C" int init(int d_lev);
extern "C" int read_analog(int ch_adc);
extern "C" int take_picture();
extern "C" int Sleep(int sec, int usec);
extern "C" int set_motor(int motor , int speed );

int main(){

    init(0);

    signed int leftSensor;
    signed int rightSensor;
    char c;
    int whiteWall;
    bool noLeftWall, noRightWall, noWallAhead;
    int THRESHOLD = 300;
    int totalWidth;
    signed int leftMotor, rightMotor;


    while(true){
	//initialise
	whiteWall = 0;
        //returns true if there isnt a wall
	noLeftWall =false;
	noRightWall = false;
	noWallAhead = false;

	//get data from sensors
        leftSensor = read_analog(0);
	rightSensor = read_analog(1);
	//printf("left sensor: %d\nright sensor: %d\n", leftSensor, rightSensor);
	//get data from camera
	take_picture();

	for(int i = 1; i<8; i++){

	    c = get_pixel(0,i*25, 3);
 	    if(c>120){                                    //change white threshold
            	whiteWall++;
            	printf("c is: %d\n\n",c);
	    }
    	}
    	
	//printf("whiteWall: %d", whiteWall);
	if(whiteWall < 5){                                 //Change threshold if theres problems
	    noWallAhead = true; //rename
	  //  printf("No wall ahead!!!!!!!!!\n\n\n");
	}
	

	if(leftSensor<THRESHOLD){
	     noLeftWall = true;
	}
	if(rightSensor<THRESHOLD){
	     noRightWall = true;
	}
	if(noRightWall){
	    set_motor(1, 37);//right motor
	    set_motor(2, -60);//left motor//CHANGE THRESHOLD
	    Sleep(0,10000);//CHANGE THRESHOLD
	    printf("turning right");
	}
	else if(noWallAhead){				//stay in the center of the maze
	    rightMotor = (leftSensor/10*1.1);      //change threshold
	    leftMotor = -(rightSensor/10);
	    set_motor(1, rightMotor);
	    set_motor(2,leftMotor);
	    
	    Sleep(0,1);
	    //rotate back to centre
	    leftMotor = -(leftSensor/10);      //change threshold
	    rightMotor = (rightSensor/10*1.1);
	    set_motor(1, rightMotor);
	    set_motor(2, leftMotor);
	    printf("forwarsdgsdjksdgr");
	    Sleep(0, 1);
	}
	else if(noLeftWall){
	    set_motor(1, 60);//right motor
	    set_motor(2, -37);//left motor               //Change thresholds
	    Sleep(0,10000);
	   printf("left left left left");
	}
	else //pop a u turn
	{
	   printf("pop a u turn");
	    set_motor(1, -50);
	    set_motor(2, -60);                            //bigger so the back doesn't hit the wall
	    Sleep(0,100000);               		  //Change thresholds
	}
    }
    return 0;
}
