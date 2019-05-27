using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Move : MonoBehaviour {
    public float speed = 3.0f;
	// Use this for initialization
    Material redMaterial;
	void Start () {
        //redMaterial = GetComponent<Material>();
        Color newColor = GetComponent<MeshRenderer>().material.color;
        newColor.a = 0.5f;
        GetComponent<MeshRenderer>().material.color = newColor;
	}
	
	// Update is called once per frame
	void Update () {
        transform.Translate( Input.GetAxis( "Horizontal" ) * speed * Time.deltaTime, 0, 0 );
	}
}
