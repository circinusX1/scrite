#ifndef DEMO_H
#define DEMO_H

#include <sqrat.h>
#include <iostream>

class Demo
{
    int _some;
public:
    Demo();
    Demo(int custom);
    virtual ~Demo();

    int Method(int k, const char* s);
    int Method2(int k);
    int call_from_cpp(time_t);

private:
    Sqrat::Object   _o;           // this is the object of this we return to script if we need the object 
                                  // out of the scole (not the case because the Demo is global in the script)  
};

#endif // DEMO_H
