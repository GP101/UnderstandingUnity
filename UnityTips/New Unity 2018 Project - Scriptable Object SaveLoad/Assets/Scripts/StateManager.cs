using UnityEngine;
using System.Collections;
using UnityEngine.SceneManagement;

public class StateManager : MonoBehaviour
{
    private static StateManager stateManager;
    public GameObject myGameObject;
	private int nextSceneId = 1;

	public static StateManager Instance()
	{
		if (!stateManager) {
			stateManager = FindObjectOfType (typeof(StateManager)) as StateManager;
			if (!stateManager) {
				Debug.LogError ("It needs to be one StateManager script on a game object in your scene.");	
			}
		}//if

		return stateManager;
	}

	public void Awake()
	{
		DontDestroyOnLoad (this);
	}

	public void Start()
	{
		SceneManager.LoadScene (1);
	}

	public void Print()
	{
		Debug.Log ("StateManager.Print() called" + Time.time.ToString());
	}

	public void UpdateNextSceneId()
	{
		nextSceneId += 1;
		if (nextSceneId > 2) {
			nextSceneId = 1;
		}
	}

	public int GetNextSceneId()
	{
		return nextSceneId;
	}

    private void OnGUI()
    {
        GUILayout.TextField("Press L to load next scene");
    }
}
