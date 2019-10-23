using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
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
        t.Print2();

        KTest2 t2 = new KTest2();

        //var assembly = Assembly.GetExecutingAssembly();
        //var types = assembly.GetTypes();
        //foreach( var type in types )
        //{
        //    Console.WriteLine( "Type: " + type.Name + ", Base Type: " + type.BaseType );
        //}
        Console.ReadKey( true );
    }
    [AttributeUsage( AttributeTargets.Class | AttributeTargets.Method )]
    public class SampleAttributes : Attribute
    {
        public string Name { get; set; }
        public int Version { get; set; }
    }

    [SampleAttributes( Name = "Name", Version = 9 )]
    public class KTest3
    {
        public int IntValue { get; set; }
        [SampleAttributes]
        public void Method() { }
    }
}
