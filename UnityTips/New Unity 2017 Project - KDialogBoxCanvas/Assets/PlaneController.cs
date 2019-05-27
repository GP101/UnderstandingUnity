using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PlaneController : MonoBehaviour
{
    public static int counter = 0;

	// Use this for initialization
	void Start () {
		
	}
	
	// Update is called once per frame
	void Update () {
		
	}

    private static void OnButtonCallback(int buttonId)
    {
        // 1 : left button
        // 2 : right button
        Debug.Log("My Button " + buttonId as string);
        if (buttonId == 1)
        {
            counter += 1;
            KDialogBoxCanvas.DialogBox("Inner Caption" + counter as string
                , "Inner Message" + counter as string
                , "Yes99", "No99"
                , OnButtonCallback);
        }
    }

    private void OnGUI()
    {
        if (GUI.Button(new Rect(0, 0, 200, 40), "Test"))
        {
            counter += 1;

            KDialogBoxCanvas.DialogBox("My Caption" + counter as string
                , "My Message" + counter as string
                , "Yes1", "No2"
                , OnButtonCallback);
                //, (int buttonId) =>
                //{
                //    // 1 : left button, 2 : right button
                //    Debug.Log("My Button " + buttonId as string );
                //} );
        }
    }
}
