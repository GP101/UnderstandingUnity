using System;

class Program
{
    public class KBase
    {
        // interface members
        public void Start() { Console.WriteLine( "KBase::Start" ); }
        public virtual void Update() { Console.WriteLine( "KBase::Update" ); }
    }

    public class KDerived : KBase
    {
        public void Start() { Console.WriteLine( "KDerived::Start" ); }
        public override void Update() { Console.WriteLine( "KDerived::Update" ); }
    }

    static void Main()
    {
        dynamic o1 = new KDerived();
        KBase o2 = new KDerived();

        o1.Start(); // calls Start() of actual type
        o2.Start(); // calls Start() of variable type

        o1.Update(); // Update() is virtual function, so there is no difference with below
        o2.Update();
        /*  output:
            KDerived::Start
            KBase::Start
            KDerived::Update
            KDerived::Update
        */
    }
}