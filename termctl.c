#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING    //flag
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif




#include <stdlib.h>
#include <conio.h>
#include <stdio.h>
#include <windows.h>


/* 
what this code should do? 
  1.detect terminal size 
  2.rellocate buffer to new size
  3.process exiting by ctl+c and simple confirmation [n/y] 
*/

//1.struct to store terminal size
struct TermSize { int rows;  int cols; };
typedef struct TermSize TermSize;
  
//decalering handles to comm with os and get terminal window info
HANDLE hIn;       //id numbers os givs
HANDLE hOut;

//32 bit numbers to hold state of console scrn [o/i] 
//saved at startup restore at exit

DWORD originalInMode = 0;  //decalration
DWORD originalOutMode = 0;

//fxn to get termsize 
TermSize get_term_size(){
	CONSOLE_SCREEN_BUFFER_INFO info; //provide by os
	GetConsoleScreenBufferInfo(hOut, &info); //put it in info

    int cols = (info.srWindow.Right - info.srWindow.Left) + 1; //take out cols and row from info                                    
    int rows = (info.srWindow.Bottom - info.srWindow.Top) + 1; //info.srWindow = edges of terminal window
     
     //+1 because edges are inclusive

    //puting size info to our struct type

    TermSize result;

    result.cols=cols;
    result.rows=rows;

    //return result struct
    return result;

}


//fxn to restore terminal | void = only modif. bits=modes
void restore_terminal(){
	/* 
     1.show cursor
     2.reset colours
     3.leave alt screen
     4.fflush(stdout)
     5.restore console modes
	*/
    
    printf("\033[?25h"); //show cursor
    printf("\033[0m"); //reset colors
 
    printf("\033[?1049l");
    fflush(stdout);

  //restore the console modes to og
      SetConsoleMode(hOut , originalOutMode);
      SetConsoleMode(hIn , originalInMode);
}









volatile int quit_request_flag = 0;  //golbal flag decl,
 //volatile - var can change, dont cache it.
 // 0=not quiting , 1 = ctl+c pressed = quit



 //Quiting HANDLERS
 /* 
 BOOL = legacy windows 4byte int [ 0= false , 1 0r any non zero = true]
   BOOL WINAPI = 
  
 DWORD event = 32bit signal for actions by user [ex: ctrl+c , ctrl+break, window closed] 
 */ 
BOOL WINAPI ctrl_handler(DWORD event){
	if(event == CTRL_C_EVENT) {  
		quit_request_flag = 1; //rasise quit flag
	  return TRUE;
	}
	else return FALSE; //rest let win do whrvr it does
  }

  //register handler 
  void install_ctrl_handler() {
  	SetConsoleCtrlHandler(ctrl_handler ,TRUE);
  	//arg1=fxn to call
  	//arg2=true-add it, false=remove it [win api call]
  }
  //quit prompt to ask user
void check_quit_prompt() {
	if (quit_request_flag == 0)
	{
	  return;
	}

	TermSize size = get_term_size();  //get terminal size
     
    /* 1.move cursor to bottom
       2.erase line
       3.print msg
       4.fflush
    */
    printf("\033[%d;1H", size.rows);   // %d gets replaced by size.rows (e.g. 30)
    printf("\033[2K");

    printf("quit? (y/n)");
    fflush(stdout);

    while(1){  //indef loop
    	int key = getch(); //waits for 1 key
    	if (key == 'y' || key == 'Y')
    	{
    		restore_terminal();
    		exit(0);
    	}
    	if (key == 'n' || key == 'N')
    	{
    		break;  //leave the loop
    	}
     }
        
        printf("\033[2K");
        quit_request_flag = 0;  //reset , so ctrl c work next time
        } 
      
     


//fxn to init terminal
//job: fetch handles , remembr og settings

void init_terminal(){
	hIn = GetStdHandle(STD_INPUT_HANDLE); //id for current stdin provided by os
    hOut = GetStdHandle(STD_OUTPUT_HANDLE); //id for current stdout provided by os
    
    GetConsoleMode(hIn , &originalInMode); //input sett info store
    GetConsoleMode(hOut, &originalOutMode); //output sett info store    
}



//fxn to enable virt term processing etc
void enable_vt_and_raw_mode(){ 
    /*to enable ansi ensacpe codes = vtp
        MODES=bundles of on/off bits
           dont overwrite blindly ,will flip only the bits i want
              through | 
    */
   DWORD outMode = originalOutMode; //og val of o/p mode [32bit val]
   outMode = outMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING; //[flipped 32 bit val]
  // | = or : turn this bit on leve rest as it is.  
 

   DWORD inMode = originalInMode;  //og val for in/p mode [32 bit]
   inMode = inMode & ~(ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT); //flipped mode [32bit val]

   // ~ flip the bits, & then clear them
   //& -> turn this bit off , leave rest alone
   //hence , line buffering , echo are turned off
//APPLY MODIFIED in,out MODES

   SetConsoleMode(hOut , outMode);
   SetConsoleMode(hIn , inMode);


  //test print escape codes
   //hide cursor 
     printf("\033[?25l");
   //alt screen
     printf("\033[?1049h");
   //flush ouptut 
     fflush(stdout);
}



int main(){
  
  init_terminal();
  enable_vt_and_raw_mode();
  install_ctrl_handler();
  
  int counter = 0;
while(1){
    printf("\033[H");              // cursor to top-left
    printf("running... %d", counter);
    fflush(stdout);                // push it to the screen now
    counter++;
    check_quit_prompt();
    Sleep(100);
}

 return 0;
}

