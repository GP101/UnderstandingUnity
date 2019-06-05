using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Playables;

public class StartTimeline : MonoBehaviour
{
    public PlayableDirector _director;
    public Transform _player;

    private void OnTriggerEnter(Collider other)
    {
        Debug.Log("OnTriggerEnter named " + other.gameObject.name);
        if (_director != null && _director.state != PlayState.Playing)
        {
            Destroy(_player.gameObject.GetComponent<UnityStandardAssets.Characters.ThirdPerson.ThirdPersonUserControl>());
            Destroy(_player.gameObject.GetComponent<UnityStandardAssets.Characters.ThirdPerson.ThirdPersonCharacter>());
            _player.transform.rotation = Quaternion.identity;

            _director.Play();
        }
    }
}
