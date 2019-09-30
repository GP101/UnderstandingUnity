using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

class Program
{
    class KBase
    {
    }
    class KDerived : KBase
    {
        public int _iData;
    }

    static void Main(string[] args)
    {
        double d = 5673.74;
        int i;

        // cast double to int.
        i = (int)d;
        Console.WriteLine(i);
        Console.WriteLine("Hello" + i.ToString());

        KDerived a;
        KBase b = new KBase();
        //a = (KDerived)b; // exception
        a = b as KDerived; // a is null
        Console.ReadKey();
    }
}
