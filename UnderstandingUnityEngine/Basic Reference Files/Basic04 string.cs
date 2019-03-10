using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

class Program
{
    static void Main( string[] args )
    {
        //from string literal and string concatenation
        string fname, lname;
        fname = "John";
        lname = "Seo";

        char[] letters= { 'H', 'e', 'l', 'l','o' };
        string[] sarray={ "Hello", "From", "Tutorials", "Point" };
        //string[] sarray=new string[4]{ "Hello", "From", "Tutorials", "Point" };

        string fullname = fname + lname;
        Console.WriteLine( "Full Name: {0}", fullname );

        //by using string constructor { 'H', 'e', 'l', 'l','o' };
        string greetings = new string(letters);
        Console.WriteLine( greetings );

        //methods returning string { "Hello", "From", "Tutorials", "Point" };
        string message = String.Join(" ", sarray);
        Console.WriteLine( message );
        //formatting method to convert a value
        message = String.Format( "Formatting {0} and {1}", fname, lname );
        Console.WriteLine( message );
    }
    /** Aliases in C#
    object:  System.Object
    string:  System.String
    bool:    System.Boolean
    byte:    System.Byte
    sbyte:   System.SByte
    short:   System.Int16
    ushort:  System.UInt16
    int:     System.Int32
    uint:    System.UInt32
    long:    System.Int64
    ulong:   System.UInt64
    float:   System.Single
    double:  System.Double
    decimal: System.Decimal
    char:    System.Char
    */
}
