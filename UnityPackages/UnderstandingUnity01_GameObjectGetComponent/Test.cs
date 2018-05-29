using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Test : MonoBehaviour {

    private MeshRenderer m_renderer;

    // Use this for initialization
    void Start () {
        m_renderer = GetComponent<MeshRenderer>();
        m_renderer.material.color = Color.red;
    }

    // Update is called once per frame
    void Update () {
        Debug.Log("elapsed time" + Time.deltaTime);
	}
}
