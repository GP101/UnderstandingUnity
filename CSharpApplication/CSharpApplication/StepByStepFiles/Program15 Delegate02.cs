using System;
using System.Text;
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
        Test( hello1 );
    }
    static void SayHello()
    {
        Console.WriteLine( "Hello World" );
    }
    static void Test( HelloDelegate del )
    {
        del();
    }
}
