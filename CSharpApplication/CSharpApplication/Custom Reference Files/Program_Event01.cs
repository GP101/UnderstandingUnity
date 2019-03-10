using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CsharpConsole
{
    class Program
    {
        static void Main(string[] args)
        {
            var tower = new ClockTower();
            var person = new Person( "John", tower );
            tower.ChimeFivePm();
        }
    }

    public class Person
    {
        private string _name;
        private ClockTower _tower;

        public Person( string name, ClockTower tower )
        {
            _name = name;
            _tower = tower;

            _tower.Chime += () => { Console.WriteLine("{0} heard the clock chime.", _name);  };
            // there is no operator=() for event.
            // operator +=() and operator -=() is defined for events.
            // jintaeks 2017-12-11_22-52
        }

    }
    public delegate void ChimeEventHandler();
    public class ClockTower
    {
        public event ChimeEventHandler Chime;
        public void ChimeFivePm()
        {
            Chime();
        }

        public void ChimeSixPm()
        {
            Chime();
        }
    }
}
