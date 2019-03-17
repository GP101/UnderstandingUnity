using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class TextureScroll : MonoBehaviour
{
    MeshRenderer renderer = null;
    public int materialIndex = 0;
    public Vector2 uvAnimationRate = new Vector2(1.0f, 0.0f);
    public string textureName = "_MainTex";

    Vector2 uvOffset = Vector2.zero;

    private void Start()
    {
        renderer = GetComponent<MeshRenderer>();
    }

    void LateUpdate()
    {
        uvOffset += (uvAnimationRate * Time.deltaTime);
        if (renderer.enabled)
        {
            renderer.materials[materialIndex].SetTextureOffset(textureName, uvOffset);
        }
    }
}
