using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor;

public class SelectAllOfTag : ScriptableWizard {

    public string searchTag = "tag here";

	// Use this for initialization
    [MenuItem ("My Tools/Select All of Tag...")]
    void SelectAllOfTagWizard() {
        ScriptableWizard.DisplayWizard<SelectAllOfTag>( "Select All of Tag...", "Make Selection" );
	}
    void OnWizardCreate() {
        GameObject[] gameObjects = GameObject.FindGameObjectsWithTag( searchTag );
        Selection.objects = gameObjects;
    }
}
