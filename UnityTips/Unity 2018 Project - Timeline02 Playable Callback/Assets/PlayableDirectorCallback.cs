using UnityEngine;
using UnityEngine.Playables;

public class PlayableDirectorCallback : MonoBehaviour
{
    public Vector3 _position;
    public PlayableDirector _director;

    void OnEnable()
    {
        _director.stopped += OnPlayableDirectorStopped;
    }

    void OnPlayableDirectorStopped(PlayableDirector aDirector)
    {
        if (_director == aDirector)
        {
            Debug.Log("PlayableDirector named " + aDirector.name + " is now stopped.");
            gameObject.AddComponent<UnityStandardAssets.Characters.ThirdPerson.ThirdPersonUserControl>();
            //gameObject.AddComponent<UnityStandardAssets.Characters.ThirdPerson.ThirdPersonCharacter>();
        }
    }

    void OnDisable()
    {
        _director.stopped -= OnPlayableDirectorStopped;
    }

    private void OnGUI()
    {
        string text = string.Format("Pos=({0},{1},{2})"
            , transform.position.x, transform.position.y, transform.position.z);
        int y = 0;
        GUI.Label(new Rect(0, y, 200, 20), text);
        y += 20;
        //if (GUI.Button(new Rect(0, y, 200, 20), "Play"))
        //{
        //    transform.rotation = Quaternion.identity;
        //    _director.Play();
        //}
    }
}