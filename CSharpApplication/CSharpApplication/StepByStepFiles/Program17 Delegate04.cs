using System;
using System.Text;
using System.Reflection;

class Program1
{
    delegate void HelloDelegate();

    class Sample
    {
        public void SayHello()
        {
            Console.WriteLine( "Sample Hello World" );
        }
    }

    static void Main( string[] args )
    {
        HelloDelegate hello1 = new HelloDelegate( SayHello );
        hello1 += SayHello2;
        hello1();
        Sample sam = new Sample();
        HelloDelegate hello3 = sam.SayHello;
        Test( hello3 );
    }
    static void SayHello()
    {
        Console.WriteLine( "Hello World" );
    }
    static void SayHello2()
    {
        Console.WriteLine( "Hello World2" );
    }
    static void Test( HelloDelegate del )
    {
        del();
    }
}
