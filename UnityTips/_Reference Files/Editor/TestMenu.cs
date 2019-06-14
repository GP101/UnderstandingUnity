using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor;

public class TestMenu : MonoBehaviour {

    [MenuItem("Test/Hello")]
    // Use this for initialization
    static void CreateHello() {
        //GameObject go = new GameObject("Hello");
        GameObject go = GameObject.CreatePrimitive(PrimitiveType.Cube);
        go.name = "Hello";
	}
}
