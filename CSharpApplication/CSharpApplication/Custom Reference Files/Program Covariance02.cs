using System;
using System.Collections.Generic;
using System.Text;

namespace CsharpConsole
{
    class Program
    {
        public class KBase
        {
            public int _iData = 1;
        }

        public class KDerived : KBase
        {
            public float _fData = 2.3f;
        }

        //delegate T GetDataDelegate<T>();
        delegate T GetDataDelegate<out T>();

        static void Main(string[] args)
        {
            //KBase b = new KBase();
            //KDerived d = (KDerived)b;
            //Console.WriteLine("{0}", d._fData); // exception

            KDerived d2 = new KDerived();
            KBase b2 = d2;
            Console.WriteLine("{0}", b2._iData);
            KDerived d3 = (KDerived)b2;
            Console.WriteLine("{0}", d3._fData);

            GetDataDelegate<string> s0 = () => { return "Hello"; };
            GetDataDelegate<object> s1 = s0;

            string str0 = s0();
            Console.WriteLine("{0}", str0);
            string str1 = (string)s1();
            Console.WriteLine("{0}", str1);
        }
        /** output:
            1
            2.3
            Hello
            Hello
        */
    }
}
