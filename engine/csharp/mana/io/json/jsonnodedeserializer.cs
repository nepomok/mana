namespace Mana.IO
{
    class JsonNodeDeserializer : Deserializer<Node>
    {
        public Node deserialize(System.IO.Stream stream)
        {
            return JsonCommon.convertNode(Newtonsoft.Json.Linq.JObject.Parse(new System.IO.StreamReader(stream).ReadToEnd()));
        }
    }
}