using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using UnityEditor;

/** @desc   Editor extension to select game object(s) by tag.
    @Url    original source code link:
            https://nevzatarman.com/2015/01/10/unity-editor-scripting-filtering-hierarchy-by-tag-or-layer/
    @date   jintaeks on 2017-08-08_11-27
            Code merged into 'Story Of KakeMake' project
*/

enum FilterOptions
{
    Tag,
    Layer,
    Both
}

public class SelectByTag : EditorWindow
{
    // selection by tag
    //
    FilterOptions       filterOptions = FilterOptions.Tag;
    string              selectedTag = "Untagged";
    int                 layer = 0;
    bool                isFilterInactive = true; // select all check box, 20190326_jintaeks
    List<int>           objectIndex = new List<int>();

    // selection by component(script) name
    //
    string              componentName = "unknown";

    [MenuItem( "Custom/Select by Tag" )]
    static void Init()
    {
        SelectByTag filter = ( SelectByTag )EditorWindow.GetWindow( typeof( SelectByTag ) );
        filter.Show();
    }
    void OnGUI()
    {
        EditorGUILayout.PrefixLabel( "Filtering Options:" );
        isFilterInactive = EditorGUILayout.Toggle( "Include all", isFilterInactive );
        filterOptions = ( FilterOptions )EditorGUILayout.EnumPopup( "Select by", filterOptions );
        if( filterOptions == FilterOptions.Tag ) {
            selectedTag = EditorGUILayout.TagField( "Select Tag", selectedTag );
            if( GUILayout.Button( "Select by Tag" ) ) {
                FilterSelected( filterOptions );
                //GameObject[] objects = GameObject.FindGameObjectsWithTag( selectedTag );
                //Selection.objects = objects;
            }
        }
        else if( filterOptions == FilterOptions.Layer ) {
            layer = EditorGUILayout.LayerField( "Select Layer", layer );
            if( GUILayout.Button( "Select by Layer" ) ) {
                FilterSelected( filterOptions );
            }
        }
        else {
            selectedTag = EditorGUILayout.TagField( "Select Tag", selectedTag );
            layer = EditorGUILayout.LayerField( "Select Layer", layer );
            if( GUILayout.Button( "Select by All" ) ) {
                FilterSelected( filterOptions );
            }
        }//if.. else if.. else..

        EditorGUILayout.Separator();
        EditorGUILayout.PrefixLabel( "Component Options:" );
        GUILayout.BeginHorizontal();
        GUILayout.Label( "Component Name" );
        componentName = GUILayout.TextField( componentName, 80 );
        GUILayout.EndHorizontal();
        if( GUILayout.Button( "Select by Component Name" ) ) {
            SelectObjectByScriptComponent( componentName );
        }//if

        EditorGUILayout.Separator();
        EditorGUILayout.PrefixLabel( "Save/Load Options:" );
        string selectionInfoText;
        selectionInfoText = string.Format( "{0} object(s) selected.", Selection.objects.Length );
        //selectionInfoText = "Objects selected : " + Selection.objects.Length;
        EditorGUILayout.TextArea( selectionInfoText );

        //EditorGUILayout.PrefixLabel("Save and Load Options");
        EditorGUILayout.BeginHorizontal();
        if( Selection.objects.Length >= 1 ) {
            if( GUILayout.Button( "Save Selection" ) ) {
                int[] selectionIDs = Selection.instanceIDs;
                var saveStr = string.Empty;
                foreach( int i in selectionIDs )
                    saveStr += i.ToString() + ";";
                saveStr = saveStr.TrimEnd( char.Parse( ";" ) ); // qff, intaeks on 2017-08-08_16-31
                EditorPrefs.SetString( "SelectedIDs", saveStr );
            }
        }
        if( EditorPrefs.HasKey( "SelectedIDs" ) ) {
            if( GUILayout.Button( "Load Selection" ) ) {
                string[] strIDs= EditorPrefs.GetString( "SelectedIDs" ).Split( char.Parse( ";" ) );
                int[] ids = new int[strIDs.Length];
                for( var i = 0; i < strIDs.Length; i++ )
                    ids[i] = int.Parse( strIDs[i] );
                Selection.instanceIDs = ids;
            }
        }
        EditorGUILayout.EndHorizontal();
    }//OnGUI()

    void OnInspectorUpdate()
    {
        Repaint();
    }

    Object[] SelectAll()
    {
        // 'FindObjectsOfTypeAll' also list disabled objects.
        return Resources.FindObjectsOfTypeAll( typeof( GameObject ) ) as Object[];
    }
    Object[] SelectActive()
    {
        return Object.FindObjectsOfType( typeof( GameObject ) ) as Object[];
    }

    void FilterSelected( FilterOptions ops )
    {
        Object[] selected = isFilterInactive ? SelectAll() : SelectActive();

        objectIndex = new List<int>();
        for( int i = 0; i < selected.Length; i++ ) {
            GameObject g = ( GameObject )selected[i] as GameObject;

            if( ops == FilterOptions.Tag && g.tag == selectedTag ) {
                objectIndex.Add( i );
            }
            else if( ops == FilterOptions.Layer && g.layer == layer ) {
                objectIndex.Add( i );
            }
            else if( ops == FilterOptions.Both && g.layer == layer && g.tag == selectedTag ) {
                objectIndex.Add( i );
            }
        }
        Object[] newSelected = new Object[objectIndex.Count];
        for( int i = 0; i < objectIndex.Count; i++ ) {
            newSelected[i] = selected[objectIndex[i]];
        }
        Selection.objects = newSelected;
    }//FilterSelected()

    void SelectObjectByScriptComponent( string componentName )
    {
        Object[] selected = isFilterInactive ? SelectAll() : SelectActive();

        objectIndex = new List<int>();

        for( int i = 0; i < selected.Length; ++i ) {
            GameObject obj = ( GameObject )selected[i] as GameObject;
            Component[] cs = ( Component[] )obj.GetComponents( typeof( Component ) );
            foreach( Component c in cs ) {
                //Debug.Log( "name " + c.name + " type " + c.GetType() + " basetype " + c.GetType().BaseType );
                //this part is where I want to check with the class that was passed to the function as a parameter
                //if( c.name == componentName ) {
                if (c.GetType().ToString().Contains(componentName))
                {
                    objectIndex.Add( i );
                    break;
                }//if
            }//foreach
        }//for
        Object[] newSelected = new Object[objectIndex.Count];
        for( int i = 0; i < objectIndex.Count; i++ ) {
            newSelected[i] = selected[objectIndex[i]];
        }
        Selection.objects = newSelected;
    }//SelectObjectByScriptComponent()
}