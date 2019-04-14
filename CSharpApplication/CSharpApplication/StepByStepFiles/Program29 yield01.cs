using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CsharpConsole
{
    class Program
    {
        static void Main( string[] args )
        {
            IEnumerator<object> enumerator = GetNextValue1().GetEnumerator();
            enumerator.MoveNext();
            object item = enumerator.Current;
            Console.WriteLine( "value2 = {0}", item );
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
    }
}
