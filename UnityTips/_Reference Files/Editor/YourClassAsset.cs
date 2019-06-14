using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor;

public class YourClassAsset : MonoBehaviour
{
    [MenuItem("Assets/Create/Create YourClass Asset")]
    public static void CreateAsset()
    {
        ScriptableObjectUtility.CreateAsset<YourClass>();
    }
}
