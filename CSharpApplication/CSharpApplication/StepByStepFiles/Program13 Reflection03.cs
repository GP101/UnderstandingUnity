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
        Sample sample = new Sample { Name = "Hello", age = 49 };
        var sampleType = typeof( Sample );
        //System.Type sampleType = typeof(Sample);
        var nameProperty = sampleType.GetProperty( "Name" );
        Console.WriteLine( "Property: " + nameProperty.GetValue( sample ) );

        var updateMethod = sampleType.GetMethod( "Update" );
        //System.Reflection.MethodInfo updateMethod = sampleType.GetMethod("Update");
        object[] paramArray = new object[1] { 1.2f };
        updateMethod.Invoke( sample, paramArray );
    }
}
/** output
    Property: Hello
    Sample.Update(), 1.2
    Press any key to continue . . .
*/
