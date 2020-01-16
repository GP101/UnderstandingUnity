using System;
using System.Text;

namespace CsharpConsole
{
    public class KTest
    {
        string[] _name = new string[] { "Hello", "Unity", "Programming" };
        public string this[int index]
        {
            get { return _name[index]; }
            set { _name[index] = value; }
        }
    }

    class Console
    {
        static void Main()
        {
            KTest e = new KTest();
            e[1] = "World";

            System.Console.WriteLine("{0} {1} {2}", e[0], e[1], e[2]);
            /** output:
                Hello World Programming */
        }
    }
}
