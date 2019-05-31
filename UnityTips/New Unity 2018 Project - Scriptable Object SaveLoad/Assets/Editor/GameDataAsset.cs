using UnityEngine;
using UnityEditor;

public class GameDataAsset
{
    [MenuItem( "Assets/Create/Game Data" )]
    public static void CreateAsset() {
        ScriptableObjectUtility.CreateAsset<GameData>();
    }
}