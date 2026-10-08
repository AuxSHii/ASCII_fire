
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
