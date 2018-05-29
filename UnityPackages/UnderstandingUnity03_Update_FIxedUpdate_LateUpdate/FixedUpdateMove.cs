using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.Threading;

public class FixedUpdateMove : MonoBehaviour {

    public float m_speed = 2f;
    bool bUpdateTransform = false;

	// Use this for initialization
	void Start () {
        StartCoroutine(MoveRight());
	}

	// Update is called once per frame
	void Update () {
        int threadId = Thread.CurrentThread.ManagedThreadId;
        if (bUpdateTransform)
        {
            gameObject.transform.Translate(Time.deltaTime * m_speed, 0, 0);
            bUpdateTransform = false;
        }
    }

    IEnumerator MoveRight()
    {
        while (true)
        {
            //gameObject.transform.Translate(Time.deltaTime * m_speed, 0, 0);
            bUpdateTransform = true;
            yield return new WaitForFixedUpdate();
        }
    }
}
