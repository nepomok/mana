namespace Mana.IO
{
    class JsonSceneDeserializer : Deserializer<Scene>
    {
        public Scene deserialize(System.IO.Stream stream)
        {
            return JsonCommon.convertScene(Newtonsoft.Json.Linq.JObject.Parse(new System.IO.StreamReader(stream).ReadToEnd()));
        }
    }
}