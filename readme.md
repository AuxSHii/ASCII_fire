# ascii_fire

A doom-style fire effect that runs in the terminal, written in C. it draws with ANSI 256-color, resizes with the window, and asks before it quits.

i built it to learn C and the Windows console API, so the code is commented heavily and in my own words.
YOU WILL find a lot of comments here , i tend to write ANNOYINGLY large amount of comments when i am learning

**Windows only for now.** Built with MinGW gcc and run in plain cmd on Win10. A Linux version would need termios/ioctl in place of the Windows console calls, and i haven't done that yet.

## Build and run

```
gcc termctl.c -o fire.exe
fire.exe
```

No libraries beyond what ships with MinGW.

## Controls

- **Ctrl+C** pauses the fire and asks `quit? (y/n)` on the bottom row.
- **y** quits.
- **n** carries on.
- Resizing the window just works, and the fire rebuilds at the new size.

## How it works

Each cell in a grid holds a heat number. The bottom row is the fuel, set to near-max heat every frame with a little random flicker. Every other cell copies the cell below it, shifted one step left, right or straight (the wind drift), and loses 0 to 2 heat (the cooling). Heat climbs, gets patchy and fades, which gives the flame shape.

The heat number is an index into a 16-color palette, black to red to orange to yellow to white. A cell is drawn as a space with a colored background. Each frame is built as one string in memory and written with a single `fwrite`, which is what stops it flickering.

## Things I handle

- Hidden cursor and alternate screen while running, restored on exit, so your old cmd text comes back.
- i tiried - QuickEdit to be turned off while it runs (clicking the window used to swallow the first Ctrl+C) and the original console settings go back on exit.
- Ctrl+C only sets a flag in the handler. The prompt is shown by the main loop, because the handler runs on another thread.

## Known rough edges

- The frame delay is a fixed `Sleep(30)`, so the speed depends on how fast your machine renders a frame.
- Flame height, wind and flicker are hardcoded numbers (`rand() % 3`).
- Closing the window with the X button skips the cleanup, so the console modes don't get restored.

