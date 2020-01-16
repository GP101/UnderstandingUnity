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

    static float GetMultiply2( int num )
    {
        Console.WriteLine( "GetMultiply2: {0} * 2 = {1}", num, num * 2 );
        return num * 2;
    }

    static void Main( string[] args )
    {
        Calculate cal = Multiply2;
        cal( 3 );
        Calculate cal2 = delegate ( int num ){
            Console.WriteLine( "{0} * 2 = {1}", num, num * 2 ); };
        cal2( 3 );
        Calculate cal3 = ( int num ) => {
            Console.WriteLine( "{0} * 2 = {1}", num, num * 2 ); };
        cal3( 3 );
        Calculate cal4 = ( num ) => {
            Console.WriteLine( "{0} * 2 = {1}", num, num * 2 ); };
        cal4( 3 );
        Calculate cal5 = num => {
            Console.WriteLine( "{0} * 2 = {1}", num, num * 2 ); };
        cal5( 3 );
        Action<int> cal6 = num => { Console.WriteLine( "Action {0} * 2 = {1}", num, num * 2 ); };
        cal6( 3 );
        Func<int, float> cal7 = num => { return num * 2; };
        Console.WriteLine( "Func {0} * 2 = {1}", 3, cal7( 3 ) );
        cal7 = GetMultiply2;
        cal7( 3 );
    }
}
