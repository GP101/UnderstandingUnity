using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class UpdateMove : MonoBehaviour {

    public float m_speed = 2f;

	// Use this for initialization
	void Start () {
        StartCoroutine(MoveRight());
	}
	
	// Update is called once per frame
	void Update () {
        //gameObject.transform.Translate(Time.deltaTime * m_speed, 0, 0);
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
