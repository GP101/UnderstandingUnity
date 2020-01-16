using System;
using System.Collections.Generic;
using System.Text;

namespace CsharpConsole
{
    class Program
    {
        //public interface IEnumerator
        //{
        //    bool MoveNext();
        //    object Current { get; }
        //    void Reset();
        //}

        static void Main(string[] args)
        {
            IEnumerator<int> enumerator = GetNextValue1().GetEnumerator();
            enumerator.MoveNext();
            object item = enumerator.Current;
            Console.WriteLine("value2 = {0}", item);
        }

        static IEnumerable<int> GetNextValue1()
        {
            Console.WriteLine("GetNextValue 1");
            yield return 1;
            Console.WriteLine("GetNextValue 3");
            yield return 3;
            Console.WriteLine("GetNextValue 5");
            yield return 5;
            Console.WriteLine("GetNextValue break");
            yield break;
        }
    }
}
