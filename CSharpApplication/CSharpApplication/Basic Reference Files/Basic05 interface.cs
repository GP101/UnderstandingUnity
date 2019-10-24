using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

class Program
{
    public interface KBase
    {
        // interface members
        void Start();
        void Update();
    }

    public class KDerived : KBase
    {
        public void Start() { Console.WriteLine( "KDerived::Start" ); }
        public void Update() { Console.WriteLine( "KDerived::Update" ); }
    }
    class Tester
    {
        static void Main( string[] args )
        {
            KDerived d = new KDerived();
            d.Start();
            d.Update();
            KBase b = new KDerived();
            b.Start();
            b.Update();
            Console.ReadKey();
            /*
                KDerived::Start
                KBase::Update
                KDerived::Update
                KBase::Start
                KBase::Update
                KDerived::Update
            */
        }
    }
}
