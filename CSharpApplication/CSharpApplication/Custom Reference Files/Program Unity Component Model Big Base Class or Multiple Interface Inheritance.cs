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
    public abstract void GetHealth();
}

// multiple inheritance is not possible. _20180417_jintaeks
public abstract class IHealth
{
    public abstract void GetHealth();
}
public interface IDamage
{
    void GetDamage();
}

public class Enemy : Base, IDamage
{
    // Use this for initialization
    public void Start()
    {
    }

    public void Update()
    {
        GetHealth();
    }

    public override void GetHealth()
    {
        Console.WriteLine("GetHealth");
    }
    public void GetDamage()
    {
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
