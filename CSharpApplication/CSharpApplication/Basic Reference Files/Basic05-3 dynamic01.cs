using System;

class Program
{
    static void Main()
    {
        object MyTestVar = "test";
        dynamic MyTestVar2 = "Testing 123";

        Console.WriteLine( "{0}", MyTestVar.GetType() );
        Console.WriteLine( "{0}", MyTestVar2.GetType() );

        MyTestVar = 123;
        MyTestVar2 = 321;

        Console.WriteLine( "{0}", MyTestVar.GetType() );
        Console.WriteLine( "{0}", MyTestVar2.GetType() );
    }
}