/* This files provides address values that exist in the system */
#define SDRAM_BASE            0xC0000000
#define FPGA_ONCHIP_BASE      0xC8000000
#define FPGA_CHAR_BASE        0xC9000000

/* Cyclone V FPGA devices */
#define LEDR_BASE             0xFF200000
#define HEX3_HEX0_BASE        0xFF200020
#define HEX5_HEX4_BASE        0xFF200030
#define SW_BASE               0xFF200040
#define KEY_BASE              0xFF200050
#define TIMER_BASE            0xFF202000
#define PIXEL_BUF_CTRL_BASE   0xFF203020
#define CHAR_BUF_CTRL_BASE    0xFF203030

/* VGA colors */
#define WHITE 0xFFFF
#define YELLOW 0xFFE0
#define RED 0xF800
#define GREEN 0x07E0
#define BLUE 0x001F
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define GREY 0xC618
#define PINK 0xFC18
#define ORANGE 0xFC00


#define ABS(x) (((x) > 0) ? (x) : -(x))

/* Screen size. */
#define RESOLUTION_X 320
#define RESOLUTION_Y 240

/* Constants for animation */
#define BOX_LEN 2
#define NUM_BOXES 8

#define FALSE 0
#define TRUE 1

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

//Funtion Prototypes
void swap(int*, int*);
void clear_screen();
void draw_green_borders();
void draw_road(int y);
void draw_line(int, int, int,short int);
void plot_pixel(int, int, short int);
void draw_chicken(int x, int y);
void blank_chicken(int x, int y, short int color , bool box_erase);
void draw_car(int x, int y);
void blank_car(int x, int y);
void draw_word(int x, int y, char word[]);
void draw_char(int x, int y, char character);
void draw_police(int x, int y);
void draw_cake(int x, int y);


//global variables
volatile int pixel_buffer_start; 
volatile int charBuffer = 0xc9000000;
volatile int* PS2 = (int*)0xff200100;
int* key_addy = 0xFF200050;
	bool collide = false; //checks for collisions
	bool  win = false;    //checks for wins
    bool pink_power = false; //makes pink powered chicken



//main starts here
int main(void)
{
     *PS2 = 0xff;
	 volatile int * pixel_ctrl_ptr = (int *)0xFF203020;
     pixel_buffer_start = *pixel_ctrl_ptr;
	register int status;
		erase_words();
		clear_screen();
	
	draw_word(34, 20, "CROSSY ROAD");
	draw_word(18, 26, "Rules: || Help The Chicken Cross The Road    ||");
	draw_word(24, 28, " || Dont Get Hit By Incoming Vechicles ||");
	draw_word(24, 30, " || Dont Get Caught By The Police      ||");
	draw_word(24, 32, " || Eat Cake To Enable Pink Power      ||");
	draw_word(24, 34, " || Use Keys To Move Chicken           ||");
	draw_word(24, 36, " || Use P Button To Pause Game         ||");
	draw_word(29, 41, " PRESS SPACE TO START ");
	
	draw_green_borders();
	
	for (int i = 20 ; i < 320 ; i = i + 25 ){
		if (pink_power == false){
			  pink_power = true;
		}
			else{
				pink_power = false;
			}
		draw_chicken(i , 20 );
	}
	
	for (int i = 20 ; i < 320 ; i = i + 25 ){
		if (pink_power == false){
			  pink_power = true;
		}
			else{
				pink_power = false;
			}
		draw_chicken(i , 220 );
	}
	
		pink_power = false;
	
	
	for (int i = 20 ; i < 320 ; i = i + 30 ){
		draw_police(i , 55 );
	}
	for (int i = 20 ; i < 320 ; i = i + 35 ){
		draw_car(i , 190 );
	}
	
	for (int i = 77 ; i < 180 ; i = i + 24 ){
		draw_cake(20 , i );
	}
	
	for (int i = 77 ; i < 180 ; i = i + 24 ){
		draw_cake(300 , i );
	}
	
	
	
	volatile int value;
	while (1){	
	value = *PS2;
		value = value & 0xff;
		if (value == 0x29){
			break;
		}	
		
	}
	*PS2 = 0xff;
	
	
   //everytime game over occurs, we come to this loop
	while(1){
		
	pink_power = false;	
    volatile int * pixel_ctrl_ptr = (int *)0xFF203020;
    /* Read location of the pixel buffer from the pixel buffer controller */
    pixel_buffer_start = *pixel_ctrl_ptr;
	register int status;
		erase_words();
		clear_screen();
		
		//draw initial level and delay
		draw_word(36, 30, "LEVEL 1");
		int delay = 500000000;
		while(delay != 0){
			delay = delay-1;
		}
		erase_words();
		
		
	collide = false; //set initial bools
    win = false;  
    int car_curr_x[4]; //x coordinates of 4 cars
	int car_curr_y[4]; //y coordinates of 4 cars
	int dx[4];  // direction 4 cars (only horizontal)
	int old_x[4]; //used to keep track of the previous  x coordinates of cars
	int old_y[4]; //used to keep track of previous y coordinates of cars
	int level = 0; //holds the level
		
	int arr_police[ 4 ] = { 48, 112, 176 ,240 };
	int police_x[6]; //x coordinates of 3 police
	int police_y[6]; //y coordinates of 3 police
		
	
		
		
	draw_word(70 , 1, "LEVEL 01"); //showing initial level on top right corner
		
		
	//everytime a level is won, we come to this loop
	while(1){
		
	//if we came here due to a collision we exit to the outermost loop as game is over
		//we also display the gameover screen
	if (collide){
		clear_screen();
		draw_word(36, 30, "GAME OVER");
		int delay = 500000000;
		while(delay != 0){
			delay = delay-1;
		}
		break;
	}
		
	//increment level and display it in the screen and on the top right corner
	level = level +1;
	if (win){
			clear_screen();
		draw_word(36, 30, "LEVEL");
	      char c = level+ '0' ;
		draw_char(42, 30, c );
		
		draw_word(70, 1, "LEVEL 0");
		 draw_char(77, 1, c);
		int delay = 500000000;
		while(delay != 0){
			delay = delay-1;
		}
		erase_words();
		
		draw_word(70, 1, "LEVEL 0");
		 draw_char(77, 1, c);
	}
		
	
		
	//setting intial coordinates for cars
	car_curr_x[0] = 25;
	car_curr_y[0] = 60;
    car_curr_x[1] = 295;
	car_curr_y[1] = 100;
    car_curr_x[2] = 25;
	car_curr_y[2] = 140;
    car_curr_x[3] = 295;
	car_curr_y[3] = 180;
	
	for (int i = 0 ; i < 4 ; ++i){
		old_x[i] = car_curr_x[i];
		old_y[i] = car_curr_y[i];
	}
	//setting initial directions for cars
	dx[0] = 1 * level;
	dx[1] = -1 * level;
	dx[2] = 1 * level;
	dx[3] = -1 * level;
	
		
		
	//setting random values for police positions
		

	for (int i=0; i < 6 ; i++){
		police_x[i] = arr_police[rand()%4]; //pick random x value for each police
	}
		
		police_y[0] = 80;
		police_y[1] = 120;
		police_y[2] = 160;
		police_y[3] = 80;
		police_y[4] = 120;
		police_y[5] = 160;
		
	int	cake_x = 30;
	int	cake_y = 220;
		
   //first we draw the roads and greenspaces
    clear_screen();
	draw_green_borders();
	draw_road(40);
	draw_road(80);
	draw_road(120);
	draw_road(160);
	draw_road(200);
	
	int toggle = -1; // for togelling which color is drawn back after erasure
	int ystep = 0;
	int curr_y = 220;
	int curr_x = 175;
	int xstep = 0;
	short int line_color = 0x07E0; //green
    bool box_erase=0;
	
	//draw the basic chicken behind the start line
	draw_chicken(curr_x,curr_y);
	
		//draw start police
		if (level > 2){
				 for (int i = 0 ; i < 6 ; i++){
			draw_police(police_x[i], police_y[i]);
		}
		}
		else{
	   for (int i = 0 ; i < 3 ; i++){
			draw_police(police_x[i], police_y[i]);
		}
		}
	//draw start cars
	   for (int i = 0 ; i < 4 ; i++){
			draw_car(car_curr_x[i], car_curr_y[i]);
		}
		
		if (level == 4){
			draw_cake(cake_x, cake_y);
		}
	
	while (1){
		
		
		 value = *PS2;
		value = value & 0xff;
		
		if (value == 0x4D ){
			
		
			draw_word(36 , 31, "PAUSED");
			
			
		   draw_word(28 , 33, "PRESS SPACE TO CONTINUE");
			
			while(1){
		  value = *PS2;
		   value = value & 0xff;
		if (value == 0x29){
			break;
			
		}	
				}
			
				draw_word(36 , 31, "      ");
			
			
		   draw_word(28 , 33, "                       ");
		}
		xstep = 0;
		ystep = 0;
		
		win = false;
		
			//checking for collisions
		
		//if level is more than 2, cars are very fast so different bounds for collision checking
		if (level > 2) {
		for  (int i = 0 ; i < 4; i++){
		if (curr_y -15 <= car_curr_y[i] + 5 && curr_y + 10 >= car_curr_y[i] -5 ){
			
			if (curr_x -10 <= car_curr_x[i] + 25 && curr_x +10 >= car_curr_x[i] - 25){
				collide = true;
				break;
			}
			
		}
			}	
			
		}
		
		//if level is less than 2 , simple collision check
			else{
		 for  (int i = 0 ; i < 4; i++){
		if (curr_y -15 <= car_curr_y[i] + 5 && curr_y + 10 >= car_curr_y[i] -5 ){
			
			if (curr_x -10 <= car_curr_x[i] + 15 && curr_x +10 >= car_curr_x[i] -15){
				collide = true;
				break;
			}
			
		}
			}
			}
		
		
		if (level > 2 ) {
		for  (int i = 0 ; i < 6; i++){
		if (curr_y -15 <= police_y[i] + 12 && curr_y + 10 >= police_y[i] - 8){
			
			if (curr_x -10 <= police_x[i] + 13 && curr_x +10 >= police_x[i] -13){
				collide = true;
				break;
			}
		}
		}
		}
			else{
		//simple police collision check
		 for  (int i = 0 ; i < 3; i++){
		if (curr_y -15 <= police_y[i] + 12 && curr_y + 10 >= police_y[i] - 8){
			
			if (curr_x -10 <= police_x[i] + 13 && curr_x +10 >= police_x[i] -13){
				collide = true;
				break;
			}
		}
		 }
			}
		
		//checking if cake ate
		if (level == 4){
		if(curr_y -15 <= cake_y + 3 && curr_y + 10 >= cake_y - 7){
			if (curr_x -10 <= cake_x + 10 && curr_x +10 >= cake_x -10){
				pink_power = true;
				draw_word(5, 58, "PINK POWER");
			}
		}
		}
			
		
		//checking for win
		if (curr_y + 10 <= 40){
			win = true;
		}
		
		if (collide || win){
			break;
		}
		
	   // Erase any boxes and lines that were drawn in the last iteration 
        for (int i = 0 ; i < 4 ; i++){
			blank_car(old_x[i], old_y[i] );
		}
		
		
	for (int i=0; i < 4 ; i++){
		old_x[i] = car_curr_x[i];
		old_y[i] = car_curr_y[i];
	}
		
		 // code for updating the locations of cars 
		
		if (level > 2) {
		for (int i=0 ; i <4 ; i ++){
			if (car_curr_x[i] - 15  <= 0 || car_curr_x[i] + 15 >= 320){
				dx[i] *= -1;
			}
			car_curr_x[i] += dx[i];
		}
		}
		
		else{
			
			for (int i=0 ; i <4 ; i ++){
			if (car_curr_x[i] -15  == 0 || car_curr_x[i] + 15 == 320){
				dx[i] *= -1;
			}
			car_curr_x[i] += dx[i];
		}
		}
		
	
		
		//draw the new cars
			for (int i = 0 ; i < 4 ; i++){
			draw_car(car_curr_x[i], car_curr_y[i]);
		}
		
		
		
			//then we do the wait for vsync part , switching buffers and shit
		*pixel_ctrl_ptr = 1;
			
		status = *(pixel_ctrl_ptr+3);
		
		while ( (status & 0x01) != 0){
			status = *(pixel_ctrl_ptr+3);
		}
		
	
		
    
		
		
		volatile int key_val = *key_addy;
		
		if (key_val == 1){
			if(pink_power){
				xstep = 2;
			}
			else{
			xstep = 1;
			}
		}
		else if (key_val == 2){
		  if(pink_power){
				ystep = -2;
			}
			else{
			ystep = -1;
			}
		}
		else if (key_val == 4){
			if(pink_power){
				ystep = 2;
			}
			else{
			ystep = 1;
			}
		}
		else if (key_val == 8 ){
			if(pink_power){
				xstep = -2;
			}
			else{
			xstep = -1;
			}
		}
		else{
			xstep = 0;
			ystep = 0;
		}
	
		
	if (key_val > 0 ){
			
			//these if statement are to erase green when in the start and finish and erase black when in the road
		if (ystep == -1 || ystep == -2){
			toggle = 1;
		}
		else{
			toggle = -1;
		}
		
		if (toggle == -1 ){
		 if (curr_y == 217 || (pink_power && ( curr_y ==216 || curr_y == 218)) ){
			line_color= 0x07E0;
            box_erase = 1;
		}
		else if (curr_y == 58  || (pink_power && ( curr_y == 57 || curr_y == 59))){
			line_color = 0x0000;
            box_erase = 1;
		}
	   }
		
	  if (toggle == 1){
		if (curr_y == 190 || (pink_power && ( curr_y == 189 || curr_y == 191))){
			line_color= 0x0000;
            box_erase = 0;
		}
		else if (curr_y == 30  || (pink_power && ( curr_y == 29 || curr_y == 31))){
			line_color = 0x07E0;
            box_erase = 0;
		}  
	   }
	  //till here 
		
		
		
		//next redrawing roads 
		
		if (curr_y+10 >= 40 || curr_y-18 <= 40){
			draw_road(40);
		}
		if (curr_y+10 >= 80 || curr_y-18 <= 80){
			draw_road(80);
			draw_police(police_x[0],police_y[0]);
			if(level >2 ){
			draw_police(police_x[3],police_y[3]);
			}
		}
		if (curr_y+10 >= 120 || curr_y-18 <= 120){
			draw_road(120);
			draw_police(police_x[1],police_y[1]);
				if(level >2 ){
			draw_police(police_x[4],police_y[4]);
				}
		}
		if (curr_y+10 >= 160 || curr_y-18 <= 160){
			draw_road(160);
			draw_police(police_x[2],police_y[2]);
				if(level >2 ){
			draw_police(police_x[5],police_y[5]);
				}
		}
		if (curr_y+10 >= 200 || curr_y-18 <= 200){
			draw_road(200);
		}
		
			//we erase old chicken 
			blank_chicken(curr_x, curr_y , line_color, box_erase);
		
		//increment x and y with ystep
		curr_y += ystep;
		curr_x += xstep;
		
	
		
		//draw new chicken
			draw_chicken(curr_x,curr_y);
		
		
		if (key_val == 0){
				 value = *PS2;
		value = value & 0xff;
		
		if (value == 0x4D ){
			
	
			draw_word(36 , 31, "PAUSED");
			
			
		   draw_word(28 , 33, "PRESS SPACE TO CONTINUE");
			
			while(1){
		  value = *PS2;
		   value = value & 0xff;
		if (value == 0x29){
			break;
			
		}	
				}
			
				draw_word(36 , 31, "      ");
			
			
		   draw_word(28 , 33, "                       ");
		}
		}


		//then we do the wait for vsync part , switching buffers and shit
		*pixel_ctrl_ptr = 1;
			
		status = *(pixel_ctrl_ptr+3);
		
		while ( (status & 0x01) != 0){
			status = *(pixel_ctrl_ptr+3);
		}
		
		
		}
	}
	}
} 
}	





//this function clears screen to black
void clear_screen(){
	for (int x=0; x< 320; ++x){
		for (int y=0; y<240 ; ++y){
			plot_pixel(x,y,0x0000);
		}
	}
}

//this function draws green start and finish
void draw_green_borders(){
	for (int x=0; x< 320; ++x){
		for (int y=0; y<40 ; ++y){
			plot_pixel(x,y,0x07E0);
}
	}		
	for (int x=0; x < 320; ++x){
		for (int y=200; y<240 ; ++y){
			plot_pixel(x,y,0x07E0);
}
}
}

	//this function draws a road	
void draw_road(int y){
	int toggle = 1;
	for (int x=0; x < 320 ; x=x+32){
		if (toggle == 1) {
	draw_line(x, y , x+32, 0xFFFF);
		}
		else {
	draw_line(x, y , x+32, 0x0000);
		}
		toggle = toggle * -1;
	}
 }

//this fucntion will draw chicken 

void draw_chicken(int x,int y){
	short int chick_color = 0xFFFF;
	short int beak_color =	0xFC00;
	if (pink_power){
		chick_color = 0xF81F;
		beak_color = 0x07FF;
	}
	for (int x1 = x-10 ; x1 < x+10 ; ++x1){
		for (int y1 = y-10 ; y1 < y+10 ; ++y1){
			plot_pixel(x1,y1,chick_color);
		}
	}
	
	for (int x1 = x-5 ; x1 < x+5 ; ++x1){
		for (int y1 = y-15 ; y1 < y ; ++y1){
			plot_pixel(x1,y1,chick_color);
		}
	}
	
		for (int x1 = x-2 ; x1 < x+2 ; ++x1){
		for (int y1 = y-17 ; y1 < y-15 ; ++y1){
			plot_pixel(x1,y1,beak_color);
		}
	}
	
		if (pink_power){
			plot_pixel(x-2,y-13,0xFFFF);
	plot_pixel(x-3,y-13,0xFFFF);
	
	plot_pixel(x+2,y-13,0xFFFF);
	plot_pixel(x+3,y-13,0XFFFF);
			
		for (int y1 = y-8 ; y1 < y+8 ; ++y1){
			plot_pixel(x-9,y1,0xFFFF);
		}
		for (int x1 = x-9; x1 < x-2 ; ++x1){
			plot_pixel(x1,y-8,0xFFFF);
	    }
		for (int y1 = y-8 ; y1 < y-1 ; ++y1){
			plot_pixel(x-2,y1,0xFFFF);
		}
		for (int x1 = x-9; x1 < x-2 ; ++x1){
			plot_pixel(x1, y-2 ,0xFFFF);
	    }
			
		for (int y1 = y-8 ; y1 < y+8 ; ++y1){
			plot_pixel(x+1,y1,0xFFFF);
		}
		for (int x1 = x+1; x1 < x+8 ; ++x1){
			plot_pixel(x1,y-8,0xFFFF);
	    }
		for (int y1 = y-8 ; y1 < y-1 ; ++y1){
			plot_pixel(x+8,y1,0xFFFF);
		}
		for (int x1 = x+1; x1 < x+8 ; ++x1){
			plot_pixel(x1, y-2 ,0xFFFF);
	    }
	}
			
	else{
	plot_pixel(x-2,y-13,0x0000);
	plot_pixel(x-3,y-13,0x0000);
	
	plot_pixel(x+2,y-13,0x0000);
	plot_pixel(x+3,y-13,0x0000);
	}
	
}


//this function will erase that chicken

void blank_chicken(int x,int y, short int color, bool box_erase){


if (box_erase){
    for (int x1 = x-10 ; x1 < x+10 ; ++x1){
		for (int y1 = y-17 ; y1 < y+10 ; ++y1){
			plot_pixel(x1,y1,color);
		}
	}
}

else{
	
		for (int x1 = x-10 ; x1 < x+10 ; ++x1){
		for (int y1 = y-10 ; y1 < y+10 ; ++y1){
			plot_pixel(x1,y1,color);
		}
	}
	
	for (int x1 = x-5 ; x1 < x+5 ; ++x1){
		for (int y1 = y-15 ; y1 < y ; ++y1){
			plot_pixel(x1,y1,color);
		}
	}
	
		for (int x1 = x-2 ; x1 < x+2 ; ++x1){
		for (int y1 = y-17 ; y1 < y-15 ; ++y1){
			plot_pixel(x1,y1,color);
		}
	}
}
}

//this function draws the cars
void draw_car(int x, int y){
		for (int x1 = x-15 ; x1 < x+15 ; ++x1){
		for (int y1 = y-5 ; y1 < y+5 ; ++y1){
			plot_pixel(x1,y1,0xF800);
		}
	}
	
	
		for (int x1 = x-7 ; x1 < x+7 ; ++x1){
		for (int y1 = y-4 ; y1 < y+4 ; ++y1){
			plot_pixel(x1,y1,0xC618);
		}
	}

}

//this function erases the cars
void blank_car(int x, int y){
		for (int x1 = x-15 ; x1 < x+15 ; ++x1){
		for (int y1 = y-5 ; y1 < y+5 ; ++y1){
			plot_pixel(x1,y1,0x0000);
		}
	}
	
	
}

//this function draws lines using given algorithm
	
	
void draw_line(int x0, int y , int x1, short int colour){
for (int x = x0 ; x < x1 ; x++){
	plot_pixel(x, y, colour);
}
}

//this function plots a pixel

void plot_pixel(int x, int y, short int line_color)
{
    *(short int *)(pixel_buffer_start + (y << 10) + (x << 1)) = line_color;
}

//this function draws a string
void draw_word(int x, int y, char word[]) {
	int len = strlen(word);
	for (int i = 0; i < len ; ++i) {
		draw_char(x+i, y, word[i]);
	}
}
//this function plots a character onto the board
void draw_char(int x, int y, char character) {
	*(char *)(charBuffer + (y << 7) + x) = character;
}

//this function erases all the words in the screen
void erase_words(){
		for (int x=0; x< 80; ++x){
		for (int y=0; y<60 ; ++y){
			draw_char(x,y,' ');
		}
	}
	
}

//draw the police men
void draw_police(int x, int y){
	
	
	for (int x1 = x-5 ; x1 < x+5 ; ++x1){
		for (int y1 = y-13 ; y1 < y ; ++y1){
			plot_pixel(x1,y1,0xFC00);
		}
	}
	
	plot_pixel(x-2,y-10,0x0000);
	plot_pixel(x-3,y-10,0x0000);
	
	plot_pixel(x+2,y-10,0x0000);
	plot_pixel(x+3,y-10,0x0000);
	
	
		for (int x1 = x-7 ; x1 < x-3 ; ++x1){
		for (int y1 = y+5 ; y1 < y+12; ++y1){
			plot_pixel(x1,y1,0xFC00);
		}
	}
	
	for (int x1 = x+3 ; x1 < x+7 ; ++x1){
		for (int y1 = y+5 ; y1 < y+12; ++y1){
			plot_pixel(x1,y1,0xFC00);
		}
	}
	
	for (int x1 = x-10 ; x1 < x+10 ; ++x1){
		for (int y1 = y-8 ; y1 < y+8 ; ++y1){
			plot_pixel(x1,y1,0x001F);
		}
	}
	
	for (int x1 = x-13 ; x1 < x+13 ; ++x1){
		for (int y1 = y-4 ; y1 < y+4 ; ++y1){
			plot_pixel(x1,y1,0x001F);
		}
	}
	
		for (int x1 = x-1 ; x1 < x+1 ; ++x1){
		for (int y1 = y-8 ; y1 < y+6 ; ++y1){
			plot_pixel(x1,y1,0x0000);
		}
	}
	
	for (int x1 = x+6 ; x1 < x+10 ; ++x1){
		for (int y1 = y-4 ; y1 < y+1 ; ++y1){
			plot_pixel(x1,y1,0xFFE0);
		}
	}
	
}

void draw_cake(int x, int y){
	for (int x1 = x-10 ; x1 < x+10 ; ++x1){
		for (int y1 = y-2 ; y1 < y+3 ; ++y1){
			plot_pixel(x1,y1,0xF81F); 
		}
	}
	for (int x1 = x-10 ; x1 < x+10 ; ++x1){
		for (int y1 = y-5 ; y1 < y-2 ; ++y1){
			plot_pixel(x1,y1,0x07FF); 
		}
	}
		for (int x1 = x-8 ; x1 < x+10 ; x1=x1+4){
		for (int y1 = y-2 ; y1 < y+1 ; ++y1){
			plot_pixel(x1,y1,0x07FF); 
		}
	}
	
		for (int x1 = x-7 ; x1 < x+8 ; x1=x1+4){
		for (int y1 = y-2 ; y1 < y ; ++y1){
			plot_pixel(x1,y1,0x07FF); 
		}
	}
	
	plot_pixel( x, y-5, 0xFFFF);
		plot_pixel( x, y-6, 0xFFFF);
	plot_pixel( x, y-7, 0xFC00);
	
	
}


	
	
	
