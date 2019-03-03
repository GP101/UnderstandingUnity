using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

class Program
{
    static void Main( string[] args )
    {
        double d = 5673.74;
        int i;

        // cast double to int.
        i = ( int )d;
        Console.WriteLine( i );
        Console.WriteLine( "Hello" + i.ToString() );
        Console.ReadKey();
    }
}
