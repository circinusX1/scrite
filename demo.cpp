
#include <stdio.h>
#include "demo.h"


extern Demo* PDemo;

Demo::Demo():_some(0)
{
    std::cout << "DEMO CTOR \n";
    _o.BindCppObject(this);
    PDemo = this;
}

Demo::Demo(int custom):_some(custom)
{
    std::cout << "DEMO CTOR 2\n";
    _o.BindCppObject(this);
}

Demo::~Demo()
{
    PDemo = nullptr;
    std::cout << "DEMO DTOR \n";
    if(!_o.IsNull()) { _o.AddRef(); } // some bug I could not pinpoint in the sqrat, but now the app exits flawless
}


int Demo::Method(int k, const char* s)
{
    printf(" Demo::Method called %d %s \n",k, s);
    return 0;
}

int Demo::Method2(int k)
{
    printf(" Demo::Method2 called %d \n",k);
    return 0;
}


int Demo::call_from_cpp(time_t now)
{
    if(!_o.GetSlot("Method2").IsNull())
    {
        Sqrat::Function f = Sqrat::Function(_o, "Method2");
        if(!f.IsNull())
        {
            try{
                f.Fcall<int>(now);
            }
            catch(Sqrat::Exception& ex)
            {
                std::cout << ex.Message() << "\n";
            }
        }
    }
    return 0;
}

