using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor;
using UnityEngine.AI;

[CustomEditor(typeof(CapsuleController))]
public class CharacterNavigationEditor : Editor
{
    static bool _visualizePath = false;

    public override void OnInspectorGUI()
    {
        DrawDefaultInspector();
        _visualizePath = EditorGUILayout.Toggle("Visualize Path", _visualizePath);
    }

    [DrawGizmo(GizmoType.Selected | GizmoType.NonSelected)]
    static void DrawLinesForNavAgent(NavMeshAgent agent, GizmoType gizmoType)
    {
        if (_visualizePath == false || agent.path == null)
            return;

        Gizmos.color = Color.red;
        Vector3[] points = agent.path.corners;
        for (int i = 0; i < points.Length - 1; ++i)
        {
            Gizmos.DrawLine(points[i], points[i + 1]);
        }
    }
}
