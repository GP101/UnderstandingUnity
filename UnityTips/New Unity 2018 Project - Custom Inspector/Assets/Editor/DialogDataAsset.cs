using UnityEngine;
using UnityEditor;

public class DialogDataAsset
{
    [MenuItem( "Assets/Create/Dialog Data" )]
    public static void CreateAsset()
    {
        ScriptableObjectUtility.CreateAsset<DialogData>();
    }
}