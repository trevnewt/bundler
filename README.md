## Synopsis

This is a simple (and very messy!) offline asset bundler program written from scratch in C. It's not particularly immaculate code, but it will never end up on the user's machine - it is merely used on my own dev machine to pack assets into neat binary files for the game to use. No external libraries are used, including the C runtime libary, so all of the code in the final executable is my own - with the exception of a handful of Win32 functions needed to interact with the operating system.

## Building

You can compile the program by running the "build.bat" batch file from the command prompt. Visual Studio's x64 C compiler must be installed. The program's entry point is defined in win32_main.c.
