using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Reflection;

class Program1
{
    delegate void HelloDelegate();

    static void Main( string[] args )
    {
        HelloDelegate hello1 = new HelloDelegate( SayHello );
        HelloDelegate hello2 = SayHello;
        hello1.Invoke();
        hello2.Invoke();
        hello1();
    }
    static void SayHello()
    {
        Console.WriteLine( "Hello World" );
    }
}
