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
    //code to enable vt and settings i want
}





int main(){
  
  init_terminal();

  TermSize size = get_term_size(); //current
 
  printf("rows=%d cols=%d\n",size.rows , size.cols); //to check

 return 0;
}

