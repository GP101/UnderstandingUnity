using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor;

[CustomEditor(typeof(ExampleClass))]
public class TestOnInspector : Editor {

	// Use this for initialization
	void Start () {
		
	}
	
	// Update is called once per frame
	void Update () {
		
	}

    public override void OnInspectorGUI()
    {
        GUILayout.Label("This is a Label in a Custom Editor");
    }
}
