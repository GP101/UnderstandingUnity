using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Reflection;

namespace CsharpConsole
{
    class Program
    {
        static void Main(string[] args)
        {
            var types = from t in Assembly.GetExecutingAssembly().GetTypes()
                        where t.GetCustomAttribute<SampleAttributes>().Version == 9
                        select t;
            foreach (var t in types)
            {
                Console.WriteLine("{0}", t.Name);
            }
        }
    }

    [AttributeUsage(AttributeTargets.Class | AttributeTargets.Method)]
    public class SampleAttributes : Attribute
    {
        public string Name { get; set; }
        public int Version { get; set; }
    }

    [SampleAttributes(Name="Name", Version=9)]
    public class Test
    {
        public int IntValue { get; set; }
        [SampleAttributes]
        public void Method() { }
    }
}