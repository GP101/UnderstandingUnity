using UnityEngine;
using System.Collections;
using System;
using System.Runtime.Serialization.Formatters.Binary;
using System.IO;

public static class SaveLoadManager
{
    public static void SavePlayer( Player player ) {
        BinaryFormatter bf = new BinaryFormatter();
        FileStream stream = new FileStream( Application.persistentDataPath + "/player.sav", FileMode.Create );

        PlayerData data = new PlayerData( player );
        bf.Serialize( stream, data );
        stream.Close();
    }

    public static int[] LoadPlayer() {
        if( File.Exists( Application.persistentDataPath + "/player.sav" ) ) {
            BinaryFormatter bf = new BinaryFormatter();
            FileStream stream = new FileStream( Application.persistentDataPath + "/player.sav", FileMode.Create );
            PlayerData data = bf.Deserialize( stream ) as PlayerData;
            stream.Close();
            return data.stats;
        }
        Debug.LogError( "File does not exist" );
        return new int[2];
    }
}

[Serializable]
public class PlayerData
{
    public int[] stats;

    public PlayerData( Player player ) {
        stats = new int[2];
        stats[0] = player.level;
        stats[1] = player.health;
    }
}