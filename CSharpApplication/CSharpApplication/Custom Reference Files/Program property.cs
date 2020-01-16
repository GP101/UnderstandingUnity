using System;
using System.Text;

namespace CsharpConsole
{
    public class Employee
    {
        private static int counter;
        private string name;

        // A read-write instance property:
        public string Name
        {
            get { return name; }
            set { name = value; }
        }

        // A read-only static property:
        public static int Counter
        {
            get { return counter; }
        }

        public Employee()
        {
            ++counter;
        }
        public int Value { get; set; }
    }

    class Console
    {
        static void Main()
        {
            Employee e1 = new Employee();
            e1.Name = "jintaeks";
            e1.Value = 50;

            System.Console.WriteLine("Employee number: {0}", Employee.Counter);
            System.Console.WriteLine("Employee name: {0}", e1.Name);
            /** output:
                Employee number: 1
                Employee name: jintaeks */
        }
    }
}
