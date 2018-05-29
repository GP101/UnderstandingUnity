using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Test : MonoBehaviour {

    private MeshRenderer m_renderer;

    // Use this for initialization
    void Start () {
        m_renderer = GetComponent<MeshRenderer>();
        m_renderer.material.color = Color.red;
        StartCoroutine(Fade());
    }

    // Update is called once per frame
    void Update () {
        Debug.Log("elapsed time" + Time.deltaTime);
	}

    IEnumerator Fade()
    {
        for (float f = 1f; f >= 0; f -= 0.1f)
        {
            Color c = m_renderer.material.color;
            m_renderer.material.color = new Color(f, 0, 0);
            yield return new WaitForSeconds(0.1f);
        }
        m_renderer.material.color = new Color(0, 0, 0);
    }
}
