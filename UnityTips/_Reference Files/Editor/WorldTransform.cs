using System.Collections;
using System.Collections.Generic;
#if UNITY_EDITOR
using UnityEditor;
#endif
using UnityEngine;

/** @class  SnapTool
   @desc   snapping tool for Story of Noah
           extends 'Transform' component in 'Inspector' window
   @date   jintaeks on 2017-05-30_09-09 */
[CustomEditor(typeof(Transform))]
public class WorldTransform : Editor
{
   public static Vector3 RoundToInt(Vector3 pos)
   {
       Vector3 roundedPos = new Vector3(Mathf.RoundToInt(pos.x), Mathf.RoundToInt(pos.y), Mathf.RoundToInt(pos.z));
       return roundedPos;
   }//RoundToInt()

   private void _ApplyRoundToInt(Transform root)
   {
       root.position = RoundToInt(root.position);

       foreach (Transform child in root)
       {
           if (child.childCount > 0)
           {
               _ApplyRoundToInt(child);
           }
           else
           {
               child.position = RoundToInt(child.position);
           }
       }//foreach
   }//_GetMinMax()


   public void OnEnable()
   {
   }//OnEnable()

   public override void OnInspectorGUI()
   {
       //base.OnInspectorGUI();

       Transform goTarget = target as Transform;

       EditorGUILayout.Space();
       GUILayout.BeginHorizontal();
       GUILayout.Label("Local Position");
       goTarget.localPosition = EditorGUILayout.Vector3Field("", goTarget.localPosition);
       GUILayout.EndHorizontal();

       GUILayout.BeginHorizontal();
       GUILayout.Label("Local Scale");
       goTarget.localScale = EditorGUILayout.Vector3Field("", goTarget.localScale);
       GUILayout.EndHorizontal();

       GUILayout.BeginHorizontal();
       GUILayout.Label("Local Rotation");
       Vector3 localEulerAngles = Quaternion.ToEulerAngles(goTarget.localRotation);
       //Vector3 localEulerAngles = goTarget.localRotation.eulerAngles;
       localEulerAngles *= Mathf.Rad2Deg;

       localEulerAngles = EditorGUILayout.Vector3Field("", localEulerAngles);
       goTarget.localRotation = Quaternion.Euler(localEulerAngles);

       GUILayout.EndHorizontal();

       EditorGUILayout.Separator();
       GUILayout.BeginHorizontal();
       GUILayout.Label("World Position");
       goTarget.position = EditorGUILayout.Vector3Field("", goTarget.position);
       GUILayout.EndHorizontal();

       GUILayout.BeginHorizontal();
       GUILayout.Label("World Rotation");
       Vector3 worldEulerAngles = Quaternion.ToEulerAngles(goTarget.rotation);
       //Vector3 worldEulerAngles = goTarget.rotation.eulerAngles;
       worldEulerAngles *= Mathf.Rad2Deg;

       worldEulerAngles = EditorGUILayout.Vector3Field("", worldEulerAngles);
       goTarget.rotation = Quaternion.Euler(worldEulerAngles);

       GUILayout.EndHorizontal();

       GUILayout.BeginHorizontal();
       if (GUILayout.Button("Snap All Positions", GUILayout.Width(120)))
       {
           //goTarget.position = RoundToInt( goTarget.position );
           Undo.RecordObject(goTarget, "Snap");
           _ApplyRoundToInt(goTarget);
       }//if

       EditorGUILayout.Separator();
       if (GUILayout.Button("Rotate 90 CCW", GUILayout.Width(100)))
       {
           worldEulerAngles.y -= 90.0f;
           goTarget.rotation = Quaternion.Euler(worldEulerAngles);
       }//if

       if (GUILayout.Button("Rotate 90 CW", GUILayout.Width(100)))
       {
           worldEulerAngles.y += 90.0f;
           goTarget.rotation = Quaternion.Euler(worldEulerAngles);
       }//if

       GUILayout.EndHorizontal();
       EditorGUILayout.Space();
   }//OnInspectorGUI()
}
