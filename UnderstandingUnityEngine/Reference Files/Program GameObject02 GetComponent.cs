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

    public abstract class MonoBehavior
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

    public class BoxCollider : Component
    {
        public void GetCollider() { }
    }
    public class MeshRenderer : Component
    {
        public void GetMaterial()
        {
            Console.WriteLine("GetMaterial");
        }
    }

    public class GameObject : MonoBehavior
    {
        public GameObject()
        {
            m_components.Add(new BoxCollider());
            m_components.Add(new MeshRenderer());
        }

        public void Start()
        {
        }

        public void Update()
        {
            MeshRenderer renderer = GetComponent<MeshRenderer>();
            renderer.GetMaterial();
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
