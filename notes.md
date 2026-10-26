
NOTES i will dump here , while i code this project


what is a "Handle" in this context?

think of os (Windows or Linux) as a giant office. The terminal window is a physical filing cabinet inside it. our Code isn't allowed to just walk over and measure the cabinet itself
instead, our program must ask the operating system: "can you give me a temporary access pass to the screen's output?" that access pass is the handle (hout , hin etc)

logic to get terminal size:


1. grab the Handle:tell the OS to hand over the active "Standard Output" handle. This establishes a direct line of communication to the specific terminal window running your program.
2. request the "Buffer" Info: we don't actually ask for the "size" directly. instead, we hand that tool/handle back to the OS and say, "Hey, look at the window attached to this handle, and tell me its current screen buffer information."
3. read the Coordinates: The OS will hand us back a specialized package (a structure): the columns (width) and the rows (height). [TermSize]

dword = 32 bit unsigned number

• originalInMode (Input): Stores the rules for the keyboard and mouse. By default, Windows automatically prints keys as you type them (echoing) and waits for you to press Enter. If you want to detect single keypresses instantly (like for a game or command-line menu), you have to turn those rules off.

• originalOutMode (Output): Stores the rules for the screen layout. By default, standard terminal screens do not process specialized color codes or text animations. If you want to use custom formatting, you have to turn those capabilities on.


Ctrl+C handler with the y/n prompt

Theory: on Windows, when Ctrl+C is pressed, the OS runs handler function on a separate thread. If the handler does heavy work (printing, reading keys) while  main loop is also running, the two collide. 
So the handler does one tiny thing, 
raise a flag, and the main loop checks that flag at a safe moment and does the real work. That's why the flag is volatile: it tells the compiler "another thread can change this, don't cache it."



WINAPI (The Calling Convention)

WINAPI is a preprocessor macro that resolves to __stdcall on x86 architectures.
• The calling convention tells the compiler how to pass arguments to the function (pushed onto the stack from right to left) and who cleans up the stack (the called function itself, rather than the caller).
• It ensures strict compatibility and interoperability across different compilers and programming languages linking against the Windows operating system libraries.
HEAT BUFFER ARRAY-
each frame, every cell copies the heat of a cell below it, shifted randomly left or right (the wind flicker) and cooled by a random amount. Heat dies out as it climbs, and the fuel row at the bottom keeps feeding it.




...so basically, instead of printing characters one by one and making the screen flash like crazy, i decided to build the whole screen in memory first... i use a giant string buffer called frame, write everything into it using sprintf(), and then dump the whole thing to stdout in one clean shot with fwrite()... plus, i added a tiny check if(color != last_color) so i only write the color change code when the color actually changes, which saves a ton of bytes and keeps things fast...

these are pretty standard things uh can google out.. to find more out [atleast thats what i did... 'alot'!]

..the actual fire physics come from a cellular automata trick... the bottom row gets seeded with max heat minus a random bit so it looks jagged... then every row above looks at the cell beneath it, shifts left or right randomly by (rand() % 3) - 1 to mimic wind, and cools down by a random decay amount... as it climbs up, it loses heat until it completely , dies out into black space...

to make it look cool, i use ansi escape sequences... \033[H moves the cursor back to the top left without clearing the whole screen, which stops flickering... \033[48;5;%dm sets the 256-color background... and \033[?1049h switches to an alternate screen buffer so when i quit, my normal terminal history is untouched...

to catch single keypresses instantly without waiting for the user to hit enter or having text echo back on screen, i mess with the console input modes... i turn off line buffering and echo flags, and also kill ENABLE_QUICK_EDIT_MODE so clicking around doesn't freeze the program...

breakdown of the windows apis and functions i used...
GetStdHandle(...)...
...this talks to the os to grab handles (hIn and hOut) so my program has an open line of communication to read inputs and write outputs...

GetConsoleMode(...) and SetConsoleMode(...)...
..these let me read the current 32-bit configuration flags of the terminal and flip specific bits on or ofF... i use them to turn on ansi escape processing and turn off line echoing and line buffering...

GetConsoleScreenBufferInfo(...)...
..,.i pass my output handle into this, and the os fills a struct with the window's exact width and height edges so my program knows how big to make the fire grid...

SetConsoleCtrlHandler(...)...
..this registers my custom ctrl_handler function so when someone presses ctrl+c, it safely triggers a volatile flag instead of crashing the app or letting windows do whatever it wants...

calloc(...) and malloc(...)...
..,.these allocate raw memory blocks on the heap for my fire grid (fire) and my string buffer (frame), with error checks to make sure the memory didn't fail to allocate...

fwrite(...) and fflush(...)...
...fwrite pushes my massive frame buffer straight to standard output all at once, and fflush(stdout) forces the stream to clear and display immediately without delay...

getch()...
...this pauses execution and waits for a single key press (like y or n) instantly without needing the user to press enter...