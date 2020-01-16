#define DEBUG
//#define IOS
using System;

public class TestClass
{
    public static void Main()
    {
#if( DEBUG && !IOS )
        Console.WriteLine("DEBUG is defined");
#elif ( !DEBUG && IOS )
         Console.WriteLine("IOS is defined");
#elif ( DEBUG && IOS )
        Console.WriteLine( "DEBUG and IOS are defined" );
#else
         Console.WriteLine("DEBUG and IOS are not defined");
#endif
        Console.ReadKey();
    }
}