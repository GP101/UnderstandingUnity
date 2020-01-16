using System;
using System.Text;
using System.Reflection;

class Program1
{
    delegate void Calculate( int num );

    static void Multiply2( int num )
    {
        Console.WriteLine( "Multiply2: {0} * 2 = {1}", num, num * 2 );
    }

    static void Main( string[] args )
    {
        Calculate cal = Multiply2;
        cal( 3 );
        Calculate cal2 = delegate ( int num ){
            Console.WriteLine( "{0} * 2 = {1}", num, num * 2 ); };
        cal2( 3 );
    }
}
