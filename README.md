# Circus Elephants
A simple arcade style game written using SDL2 where elephants have escaped the circus and you - the mouse - need to get them back.

## Dependencies
* SDL2
* SDL2 IMG
* SDL2 TTF

## Compilation
Go to program directory and execute:
```console
$ cd src

$ make
``` 
Compilation uses clang compiler.

To clean up after compilaton:
```console
$ make clean
```
## Game rules
Elephants are scared from the mouse so they flee from it when it is close to them.
If the mouse gets too close elephants die from fear.
You have to get as much elephants as you can to the circus before the time runs out.
