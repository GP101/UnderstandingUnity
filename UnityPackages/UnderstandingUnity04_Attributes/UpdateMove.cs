using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class UpdateMove : MonoBehaviour {

    [Range(0,10)]
    public float m_speed = 2f;
    [SerializeField]
    private string m_name;

    [System.Obsolete("Do not use Foo",true)]
    void Foo()
    {
    }

    // Use this for initialization
    void Start () {
        StartCoroutine(MoveRight());
        //Foo();
	}
	
	// Update is called once per frame
	void Update () {
    }

    IEnumerator MoveRight()
    {
        while (true)
        {
            gameObject.transform.Translate(Time.deltaTime * m_speed, 0, 0);
            yield return null;
        }
    }
}
