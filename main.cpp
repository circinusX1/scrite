

#include <unistd.h>
#include <string>
#include <deque>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sqwrap.h"
#include "sqratConst.h"
#include "main.h"

#ifdef __linux
#   include <set>
#   include <sys/ioctl.h>
#   include <termios.h>
#   include <libgen.h>
#else
#   include <stdlib.h>
#   include <stdio.h>
#endif
#include "demo.h"

RunCtx*             PCTX = 0;
int                 _Debug = 0;
sq_api*                 SQ_PTRS;
Sqrat::Function*    RunFoo;
time_t              Interval = 1000;
Demo*               PDemo;   // this is built in script and we hold apointer here to call methid by name

inline time_t gtc(void)
{
    struct timespec now;
    if (clock_gettime(CLOCK_MONOTONIC, &now))
        return 0;
    return time_t(now.tv_sec * 1000.0 + now.tv_nsec / 1000000.0);
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// called from script, cache the script function to RunFoo, and save interval
static int run(Sqrat::Function& f, time_t interval)
{
    RunFoo = new Sqrat::Function(f);
    Interval = interval;
    return 0;
}

////////////////////////////////////////////////////////////////////////////
// call from C++ the Method2 on the script instance.
void call_script_object_method(Sqrat::Object sco)
{
    std::cout << __FUNCTION__ << "\n";
    Sqrat::Object   o = sco.GetObject();
    if(!o.IsNull())
    {
        Demo*     pp = o.Cast<Demo*>();
        pp->Method2(100);
    }
}

////////////////////////////////////////////////////////////////////////////
int main(int argc, char *argv[])
{
    SqEnv               sq;                 // INIT SCRIPT ENV............
    std::string         script;
    std::cout << "Scrite ver: 0.0.1-"<<__DATE__<<"\r\n";

    if(argc < 2)
    {
        std::cout << "pass a script name \n";
        return 1;
    }
    else
    {
        script = argv[1];
    }

    sq.acquire();

    sqrat_newapi(&SQ_PTRS);
    /**
      add some new constants definitons for script.................
   */
    Sqrat::ConstTable(sq.theVM())
            .Const("True", 1)
            .Const("False", 0);

    /**
      export 'GlobalCall' function for script.....................
    */
    Sqrat::RootTable(sq.theVM()).Functor("GlobalCall", &RunCtx::GlobalCall);
    Sqrat::RootTable(sq.theVM()).Functor("call_script_object_method", &call_script_object_method);

    /**
      export class and their methods for script...................
    */
    Sqrat::Class<Demo> cls(sq.theVM(), _SC("Demo"));
    cls.Ctor<int>(); // custom constructor
    cls.Ctor();
    cls.Functor(_SC("Method"), &Demo::Method);
    cls.Functor(_SC("Method2"), &Demo::Method2);
    Sqrat::RootTable().Bind(_SC("Demo"), cls);
    //............................................................

    // get the script run function, so we can call from cpp as well using run.Fcall()......
    Sqrat::RootTable(sq.theVM()).Functor("run", &run);


    try{
        MyScript scr = sq.compile_script(script.c_str()); // compile the script 'demo.js.txt'.................
        scr.run_script();                                 // run it. The script does nothing because we have a main function there

        // get script's main function ..................................................
        Sqrat::Function f = Sqrat::RootTable().GetFunction(_SC("main"));
        if(!f.IsNull())
        {
            Sqrat::SharedPtr<int>   srv;
            int                     rv;

            if(argc==2)
                srv = f.Fcall<int>(0);          // call main with param 0..........................
            else
                srv = f.Fcall<int>(argv[2]);    // if we pass a second arg to main pass it to the script main, so user can make use of it
            // take a look at main @ demo.js.txt, will call run @ C++
            // passing the loop function address and a time out, seee run() @ cpp
            if(srv.Get()==0)                    // get return code from main.......................
            {
                throw (Sqrat::Exception("function setup must return True or False "));
            }
            else
                rv = (*srv.Get());              //......................  main return code.........

            if(rv == 1)                         // if main returned 1, and run() saved the 'RunFoo' -> script::loop()
            {
                time_t now,then=0;
                while(rv)                       // as long loop returns 1, keep alive
                {
                    now = ::gtc();
                    if(now - then > Interval)   // call loop() into the script every 1000 ms
                    {
                        srv = RunFoo->Fcall<int>(now);  // call Fcall <- loop() @ script............
                        rv = (*srv.Get());
                        then = now;

                        PDemo->call_from_cpp(now);      // call a c++ method by name................
                    }
                }
            }

            // let's pass the demo C++ instance to script...............
            Sqrat::Function f2 = Sqrat::RootTable().GetFunction(_SC("take_obj"));
            if(!f2.IsNull())
            {
                Demo  local_instance(2);                    // local instance........
                f2.Fcall<int>(local_instance.object());     // pass it to script take_obj.....
            }

        }
        else
        {
            std::cout << "function 'setup(param2)' not found " << std::endl;
        }
        delete RunFoo;
    }
    catch(Sqrat::Exception ex)
    {
        std::cout <<  ex.Message() << " \n";
    }
    return 0;
}
