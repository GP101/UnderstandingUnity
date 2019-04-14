using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
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
        var nameProperty = sampleType.GetProperty( "Name" );
        Console.WriteLine( "Property: " + nameProperty.GetValue( sample ) );

        var updateMethod = sampleType.GetMethod( "Update" );
        object[] paramArray = new object[1] { 1.2f };
        updateMethod.Invoke( sample, paramArray );
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
