using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace UnderstandingUnityEngine
{
    public class Component
    {
    }

    public class Behavior : Component
    {
    }

    public abstract class MonoBehavior : Behavior
    {
        public List<Component> m_components;
        public MonoBehavior()
        {
            m_components = new List<Component>();
        }
        public T GetComponent<T>() where T : Component
        {
            foreach (var comp in m_components)
            {
                if (comp.GetType() == typeof(T))
                {
                    return (T)comp;
                }
            }
            return default(T);
        }
    }

    public interface IBoxCollider
    {
        void GetCollider();
    }
    public interface IMeshRenderer
    {
        void GetMaterial();
    }

    public class Base
    {
    }

    public class GameObject : Base, IBoxCollider, IMeshRenderer
    {
        public void GetCollider() { }
        public void GetMaterial() { }

        // Use this for initialization
        public void Start()
        {
        }

        public void Update()
        {
        }
    }

    class Program
    {
        static void Main(string[] args)
        {
            GameObject e = new GameObject();
            e.Start();
            e.Update();
        }
    }
}
