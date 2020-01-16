using System;
using System.Collections.Generic;
using System.Text;

namespace CsharpConsole
{
    class Program
    {
        static void Main( string[] args )
        {
            foreach( int value in GetNextValue1() )
            {
                Console.WriteLine( "value = {0}", value );
            }
            IEnumerator<object> enumerator2 = GetNextValue2();
            while( enumerator2.MoveNext() )
            {
                object item = enumerator2.Current;
                Console.WriteLine( "value3 = {0}", item );
                if( item.ToString() == "hello" )
                    Console.WriteLine( "First case" );
                else if( item.ToString() == "wonderful" )
                    Console.WriteLine( "Second case" );
                else if( item.ToString() == "world" )
                    Console.WriteLine( "Third case" );
            }
        }

        static IEnumerable<object> GetNextValue1()
        {
            Console.WriteLine( "GetNextValue 1" );
            yield return 1;
            Console.WriteLine( "GetNextValue 3" );
            yield return 3;
            Console.WriteLine( "GetNextValue 5" );
            yield return 5;
            Console.WriteLine( "GetNextValue break" );
            yield break;
        }
        static IEnumerator<object> GetNextValue2()
        {
            yield return "hello";
            yield return "wonderful";
            yield return "world";
            yield break;
        }
    }
}
