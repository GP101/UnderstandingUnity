using System;
using System.Collections;
using System.Collections.Generic;

public class KComponent
{

}

public abstract class Base
{
    public List<KComponent> myComponents;
    public Base()
    {
        myComponents = new List<KComponent>();
    }
    public T GetKComponent<T>() where T : KComponent
    {
        foreach (var comp in myComponents)
        {
            if (comp.GetType() == typeof(T))
            {
                return (T)comp;
            }
        }
        return default(T);
    }
}

public class Health : KComponent
{
    public void Print()
    {
        Console.WriteLine("Health Component");
    }
}
public class Damage : KComponent
{

}

public class Enemy : Base
{
    Health h;
    Damage d;

    // Use this for initialization
    public void Start()
    {
        myComponents.Add( new Health() );
        myComponents.Add( new Damage() );
    }

    public void Update()
    {
        h = GetKComponent<Health>();
        h.Print();
    }

}

class Program
{
    static void Main(string[] args)
    {
        Enemy e = new Enemy();
        e.Start();
        e.Update();
    }
}
