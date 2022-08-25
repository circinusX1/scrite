# scrite

## scripting engine template C++ / Squirrel + Sqrat.
## Project for QTCreator

 * Demonstrate
    * export global functions to script "main"
    * export class and methods to script "Demo"
    * instantiates the class Demo in script and calls methods
    * callback from C++ to script "loop"
    * calls an exported function from C++ by name "Method2"
    
#### make the sq libraries using the make files    
#### build with QTcreator, or make a simple makefile with all cpp in the folder and link with pthred, dl and lsquirrel-x86_64-Linux
#### run
```
   scrite ./demo.js.txt
   ./scrite ./demo.js.txt 

CALLSTACK

LOCALS
Scrite ver: 0.0.1-Aug 25 2022
DEMO CTOR 
SETUP CALLED Demo::Method called 4 test 
LOOP CALLED AT41803615
 Demo::Method2 called 41803615 
LOOP CALLED AT41804616
 Demo::Method2 called 41804616 
LOOP CALLED AT41805617
 Demo::Method2 called 41805617 
LOOP CALLED AT41806618
 Demo::Method2 called 41806618 

```




