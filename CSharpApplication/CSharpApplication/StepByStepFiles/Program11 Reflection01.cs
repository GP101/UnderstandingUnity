using System;
using System.Collections.Generic;
using System.Text;
using System.Reflection;

class Program1
{
    public class Sample
    {
        public string Name { get; set; }
        public int age;
        public void Update( float elapsedTime )
        {
            Console.WriteLine( "Sample.Update(), " + elapsedTime );
        }
    }

    static void Main( string[] args )
    {
        var assembly = Assembly.GetExecutingAssembly();
        var types = assembly.GetTypes();
        foreach( var type in types )
        {
            Console.WriteLine( "Type: " + type.Name + ", Base Type: " + type.BaseType );
            var props = type.GetProperties();
            foreach( var prop in props )
            {
                Console.WriteLine( "\tProp: " + prop.Name );
            }
        }
    }
}
/** output
    Type: Program1, Base Type: System.Object
    Type: Sample, Base Type: System.Object
            Prop: Name
    Press any key to continue . . .
*/
