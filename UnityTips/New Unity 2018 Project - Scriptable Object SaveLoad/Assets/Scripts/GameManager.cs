using UnityEngine;
using System.Collections;

public class GameManager : MonoBehaviour
{
    //Player player;
    public GameData[] gameData;
    [Range(1,5)]
    public int level;

	// Use this for initialization
	void Start () {
        if( level > gameData.Length ) {
            level = gameData.Length;
        }
        //player.data = playerData[level - 1];
	}
}
