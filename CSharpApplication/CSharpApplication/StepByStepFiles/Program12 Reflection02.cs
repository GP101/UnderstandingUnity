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
            var fields = type.GetFields();
            foreach( var field in fields )
            {
                Console.WriteLine( "\tField: " + field.Name );
            }
            var methods = type.GetMethods();
            foreach( var method in methods )
            {
                Console.WriteLine( "\tMethod: " + method.Name );
            }
        }
    }
}
/** output
    Type: Program1, Base Type: System.Object
            Method: ToString
            Method: Equals
            Method: GetHashCode
            Method: GetType
    Type: Sample, Base Type: System.Object
            Prop: Name
            Field: age
            Method: get_Name
            Method: set_Name
            Method: Update
            Method: ToString
            Method: Equals
            Method: GetHashCode
            Method: GetType
    Press any key to continue . . .
*/
