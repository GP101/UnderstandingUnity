using UnityEngine;
using System.Collections;
using UnityEngine.SceneManagement;

public class Player : MonoBehaviour {

	private StateManager stateManager;
    public GameData   gameData;
    public int level;
    public int health;

	// Use this for initialization
	void Start () {
		stateManager = GameObject.FindObjectOfType<StateManager> ();
	}
	
	// Update is called once per frame
	void Update () {
		if (Input.GetKeyDown (KeyCode.A)) {
			stateManager.Print ();
		}
		if (Input.GetKeyDown (KeyCode.L)) {
			stateManager.UpdateNextSceneId ();
			int iNextScene = stateManager.GetNextSceneId ();
			SceneManager.LoadScene (iNextScene);
            Debug.Log( gameData.playerName + Time.time.ToString() );
		}
	}

    public void Save()
    {
        SaveLoadManager.SavePlayer(this);
    }

    public void Load() {
        int[] loadedStats = SaveLoadManager.LoadPlayer();
        level = loadedStats[0];
        health = loadedStats[1];
    }
}
