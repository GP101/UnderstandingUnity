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
            var person = new Person("John", tower);
            tower.ChimeFivePm();
        }
    }

    public class Person
    {
        private string _name;
        private ClockTower _tower;

        public Person(string name, ClockTower tower)
        {
            _name = name;
            _tower = tower;

            _tower.Chime += (object sender, ClockTowerEventArgs args) =>
            {
                Console.WriteLine("{0} heard the clock chime {1}.", _name, args.Time);
            };
        }

    }
    public class ClockTowerEventArgs : EventArgs
    {
        public int Time { get; set; }
    }

    public delegate void ChimeEventHandler(object sender, ClockTowerEventArgs args);
    public class ClockTower
    {
        public event ChimeEventHandler Chime;
        public void ChimeFivePm()
        {
            Chime(this, new ClockTowerEventArgs { Time = 5 });
        }

        public void ChimeSixPm()
        {
            Chime(this, new ClockTowerEventArgs { Time = 6 });
        }
    }
}
