using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class EnemyController : MonoBehaviour
{
    GameObject _player;
    string _componentsName="";
    // Start is called before the first frame update
    void Start()
    {
        _player = GameObject.FindGameObjectWithTag("Player");
        Component[] components = gameObject.GetComponents(typeof(Component));
        foreach (Component component in components)
        {
            _componentsName += component.ToString();
            _componentsName += "\n";
        }
    }

    // Update is called once per frame
    void Update()
    {
        
    }
    private void OnGUI()
    {
        GUI.Label(new Rect(0, 0, 400, 20), _player.name);
        GUI.Label(new Rect(0, 20, 400, 200), _componentsName);
    }
}
