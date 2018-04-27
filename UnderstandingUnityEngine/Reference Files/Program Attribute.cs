using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Reflection;
using System.Windows.Input;
using System.Threading;

namespace UnderstandingUnityEngine
{
    class Program
    {
        class Test
        {
            [Obsolete("Use Foo2()", true)]
            public void Foo()
            {
            }
            [Sample(Name = "Hello")]
            public void Foo2()
            {
            }
        }

        static void Main(string[] args)
        {
            Test t = new Test();
            t.Foo2();
        }

        //[AttributeUsage(AttributeTargets.Class)]
        public class SampleAttribute : Attribute
        {
            public string Name { get; set; }
        }
    }
}
