using System;
using System.Text;
using System.Reflection;
using System.Diagnostics;

class Program1
{

    public class KTest
    {
        public void Print() { Console.WriteLine( "Print" ); }
        [Conditional( "DEBUG" )]
        public void Print2() { Console.WriteLine( "Print2" ); }
    }

    [Obsolete]
    public class KTest2
    {
        public void Print() { Console.WriteLine( "KTest2.Print" ); }
    }

    static void Main( string[] args )
    {
        KTest t = new KTest();
        t.Print();
        t.Print2(); // will not called in 'Release' configuration

        KTest2 t2 = new KTest2(); // generate compile-time warning

        bool isDefined = false;
        System.Type t3 = typeof(KTest3);
        MethodInfo m = t3.GetMethod("Method");

        isDefined = Attribute.IsDefined( t3, typeof( SampleAttribute ) );
        Console.WriteLine( "isDefined = {0}", isDefined );

        isDefined = Attribute.IsDefined( m, typeof( SampleAttribute ) );
        Console.WriteLine( "isDefined = {0}", isDefined );

        Console.ReadKey( true );
        /*  output in 'Debug' run.
            Print
            Print2
            isDefined = True
            isDefined = False
        */
    }
    [AttributeUsage( AttributeTargets.Class | AttributeTargets.Method )]
    public class SampleAttribute : Attribute
    {
        public string Name { get; set; }
        public int Version { get; set; }
    }

    [SampleAttribute( Name = "Name", Version = 9 )]
    public class KTest3
    {
        public int IntValue { get; set; }
        //[Sample]
        public void Method() { }
    }
}
