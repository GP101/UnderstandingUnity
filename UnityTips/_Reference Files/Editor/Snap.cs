using System.Collections;
using System.Collections.Generic;
using UnityEditor;
using UnityEngine;

public class Snap : EditorWindow
{
    [MenuItem("Custom/Snap Tool/Open Snap Window")]
    static void Init()
    {
        Snap filter = (Snap)EditorWindow.GetWindow(typeof(Snap));
        filter.Show();
    }

    [MenuItem("Custom/Snap Tool/Snap hotkey _`")]
    static void ApplySnapToSelected()
    {
        var selected = Selection.activeTransform;
        //goTarget.position = RoundToInt( goTarget.position );
        Undo.RecordObject(selected, "Snap");
        _ApplyRoundToInt(selected);
    }

    static void _ApplyRoundToInt(Transform root)
    {
        if (root == null) return;
        root.position = RoundToInt(root.position);

        foreach (Transform child in root)
        {
            if (child.tag == "Untagged") continue;
            if (child.childCount > 0)
            {
                _ApplyRoundToInt(child);
            }
            else
            {
                child.position = RoundToInt(child.position);
            }
        }//foreach
    }//_ApplyRoundToInt()

    public static Vector3 RoundToInt(Vector3 pos)
    {
        Vector3 roundedPos = new Vector3((float)Mathf.RoundToInt(pos.x), (float)Mathf.RoundToInt(pos.y), (float)Mathf.RoundToInt(pos.z));
        return roundedPos;
    }//RoundToInt()

    private void OnGUI()
    {
        GUILayout.Space(5);
        if (GUILayout.Button("Snap All Positions", GUILayout.Width(120)))
        {
            ApplySnapToSelected();
        }//if
    }
}
