namespace Mana.IO
{
    public class JsonSceneDeserializer : Deserializer<Scene>
    {
        public Scene deserialize(string data)
        {
            return JsonCommon.convertScene(Newtonsoft.Json.Linq.JObject.Parse(data));
        }
    }
}