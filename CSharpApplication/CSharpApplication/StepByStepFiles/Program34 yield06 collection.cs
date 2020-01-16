using System;
using System.Collections;
using System.Collections.Generic;
using System.Text;

namespace CsharpConsole
{
    class Program
    {
        static void Main()
        {
            DaysOfTheWeek days = new DaysOfTheWeek();

            foreach (string day in days)
            {
                Console.Write(day + " ");
            }
            Console.ReadKey();
        }
        /** output:
            Sun Mon Tue Wed Thu Fri Sat
        */

        public class DaysOfTheWeek : IEnumerable
        {
            private string[] days = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };

            public IEnumerator GetEnumerator()
            {
                for (int index = 0; index < days.Length; index++)
                {
                    // Yield each day of the week.
                    yield return days[index];
                }
            }
        }
        //public class DaysOfTheWeek : IEnumerable<string>
        //{
        //    private string[] days = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };

        //    public IEnumerator<string> GetEnumerator()
        //    {
        //        for (int index = 0; index < days.Length; index++)
        //        {
        //            // Yield each day of the week.
        //            yield return days[index];
        //        }
        //    }
        //    IEnumerator IEnumerable.GetEnumerator()
        //    {
        //        return GetEnumerator();
        //    }
        //}
    }
}
