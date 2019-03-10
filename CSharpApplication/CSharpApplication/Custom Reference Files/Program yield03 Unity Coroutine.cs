using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CsharpConsole
{

    class Program
    {
        static List<IEnumerator<object>> coroutines = new List<IEnumerator<object>>();
        static void Main(string[] args)
        {
            StartCoroutine(GetNextValue1());
            StartCoroutine(GetNextValue2());

            Update();
        }

        static void StartCoroutine(IEnumerator<object> routine)
        {
            coroutines.Add(routine);
        }

        static void Update()
        {
            int numRoutines = coroutines.Count;
            while (true)
            {
                int numMissing = 0;
                foreach (var routine in coroutines)
                {
                    if (routine.MoveNext())
                    {
                        object item = routine.Current;
                        Console.WriteLine("value = {0}", item);
                    }
                    else
                    {
                        numMissing += 1;
                    }
                }
                if (numMissing == numRoutines)
                    break;
            }
        }

        static IEnumerator<object> GetNextValue1()
        {
            yield return 1;
            yield return 3;
            yield return 5;
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

/** output
value = 1
value = hello
value = 3
value = wonderful
value = 5
value = world
계속하려면 아무 키나 누르십시오 . . .
*/