namespace Mana.IO
{
    public class JsonSceneSerializer : Serializer<Scene>
    {
        public string serialize(Scene data)
        {
            var str = JsonCommon.convertScene(data).ToString();
            return str;
        }
    }
}