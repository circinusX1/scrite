# scrite

## scripting engine template C++ / Squirrel + Sqrat.
## Project for QTCreator (or qmake . && make)

 * Shows 
    * export global or class static function to script 'GlobalFunction'.
    * export class and methods to script " class Demo".
    * instantiates the class Demo in script and calls it's methods.
    * pass a function pointer from script to C++ and C++ calls that function 'loop'
    * calls an exported function from C++ by name "Method2".
    * pass a C++ instance (a Demo class instance) to script and script calls it's methods.
    * pass a script instance to C++ and C++ calls methods on it.
    

```
./scrite ./demo.script 

CALLSTACK

LOCALS
Scrite ver: 0.0.1-Aug 25 2022
DEMO CTOR THIS=0x56152601a060
main in scriptdemo called global call 
 Demo::Method called 4 test some=0 0x56152601a060
call_script_object_method
 Demo::Method2 called 100  some=0
LOOP CALLED AT 55037466 - 5
 Demo::Method2 called 55037466  some=0
LOOP CALLED AT 55038467 - 4
 Demo::Method2 called 55038467  some=0
LOOP CALLED AT 55039468 - 3
 Demo::Method2 called 55039468  some=0
LOOP CALLED AT 55040469 - 2
 Demo::Method2 called 55040469  some=0
LOOP CALLED AT 55041470 - 1
 Demo::Method2 called 55041470  some=0
LOOP CALLED AT 55042471 - 0
 Demo::Method2 called 55042471  some=0
DEMO CTOR 2 THIS=0x7ffdc93fa860
take_obj called
 Demo::Method called 3 called from script some=2 0x7ffdc93fa860
DEMO DTOR 0x7ffdc93fa860
DEMO DTOR 0x56152601a060
```

Object menthod test
```            
            Sqrat::Object   ps =  script_object;
            Scr_Func        func;
            std::string     fname = "a_script_function"
            Sqrat::Function::getFoo(main_impl_t::_sq->theVM() , *ps, fname.c_str(), func);
            if(func.IsNull())
            {
                return;
            }
            func.Release();
```




