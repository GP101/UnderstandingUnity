using UnityEngine;
using UnityEditor;
using System.IO;

public static class ScriptableObjectUtility
{
    /// <summary>
    //	This makes it easy to create, name and place unique new ScriptableObject asset files.
    /// </summary>
    public static void CreateAsset<T>() where T : ScriptableObject
    {
        T asset = ScriptableObject.CreateInstance<T>();
        string assetPathAndName;
        string path = AssetDatabase.GetAssetPath(Selection.activeObject);
        if (path == "")
        {
            path = "Assets/LevelEditorTool/Resources/";
        }
        else if (Path.GetExtension(path) != "")
        {
            path = path.Replace(Path.GetFileName(AssetDatabase.GetAssetPath(Selection.activeObject)), "");
        }
        string lastchar = path.Substring(path.Length - 1);
        if (lastchar != "/" || lastchar != "\\")
            path += "/";

        if (typeof(T).ToString() == "BuildingPrefabs.BuildingBlocksManager")
        {
            assetPathAndName = AssetDatabase.GenerateUniqueAssetPath(path + "BuildingBlocksList" + ".asset");   
        }
        else
        {
            assetPathAndName = AssetDatabase.GenerateUniqueAssetPath(path + "Settings" + ".asset");
        }
        AssetDatabase.CreateAsset(asset, assetPathAndName);

        AssetDatabase.SaveAssets();
        AssetDatabase.Refresh();
        EditorUtility.FocusProjectWindow();
        Selection.activeObject = asset;
    }
}