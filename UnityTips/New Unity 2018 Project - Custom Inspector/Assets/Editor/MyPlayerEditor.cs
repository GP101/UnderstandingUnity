using UnityEngine;
using UnityEditor;
using System.Collections;

[CustomEditor( typeof( MyPlayer ) )]
[CanEditMultipleObjects]
public class MyPlayerEditor : Editor
{
    SerializedProperty  damageProp;
    SerializedProperty  armorProp;
    SerializedProperty  gunProp;
    SerializedProperty  dialogIndexProp;

    DialogData          dialogData;

    void OnEnable()
    {
        // Setup the SerializedProperties
        damageProp = serializedObject.FindProperty( "damage" );
        armorProp = serializedObject.FindProperty( "armor" );
        gunProp = serializedObject.FindProperty( "gun" );
        dialogIndexProp = serializedObject.FindProperty( "dialogIndex" );

        dialogData = ( DialogData )Resources.Load( "NewDialogData" ) as DialogData;

        // test
        //dialog = dialogData.dialog;
        //dialog = new string[3];
        //dialog[0] = "Hello";
        //dialog[1] = "World";
        //dialog[2] = "Another";
    }//OnEnable()

    public override void OnInspectorGUI()
    {
        // Update the serializedProperty - always do this in the beginning of OnInspectorGUI.
        serializedObject.Update();

        // Show the custom GUI controls
        EditorGUILayout.IntSlider( damageProp, 0, 100, new GUIContent( "Damage" ) );

        // Only show the damage progress bar if all the objects have the same damage value:
        //if( !damageProp.hasMultipleDifferentValues )
        //    ProgressBar( damageProp.intValue / 100.0f, "Damage" );

        EditorGUILayout.IntSlider( armorProp, 0, 100, new GUIContent( "Armor" ) );

        // Only show the armor progress bar if all the objects have the same armor value:
        //if( !armorProp.hasMultipleDifferentValues )
        //    ProgressBar( armorProp.intValue / 100.0f, "Armor" );

        EditorGUILayout.PropertyField( gunProp, new GUIContent( "Gun Object" ) );
        // Apply changes to the serializedProperty - always do this in the end of OnInspectorGUI.

        string dialogIndexString;
        dialogIndexString = dialogIndexProp.intValue.ToString();
        EditorGUILayout.TextField( "Dialog: ", dialogIndexString );

        dialogIndexProp.intValue = EditorGUILayout.Popup( dialogIndexProp.intValue, dialogData.dialog, GUILayout.Width( 200 ) );

        serializedObject.ApplyModifiedProperties();
    }//OnInspectorGUI()

    // Custom GUILayout progress bar.
    void ProgressBar( float value, string label )
    {
        // Get a rect for the progress bar using the same margins as a textfield:
        Rect rect = GUILayoutUtility.GetRect( 18, 18, "TextField" );
        EditorGUI.ProgressBar( rect, value, label );
        EditorGUILayout.Space();
    }//ProgressBar()
}//class MyPlayerEditor