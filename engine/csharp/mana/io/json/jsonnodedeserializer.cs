namespace Mana.IO
{
    public class JsonNodeDeserializer : Deserializer<Node>
    {
        public Node deserialize(string data)
        {
            return JsonCommon.convertNode(Newtonsoft.Json.Linq.JObject.Parse(data));
        }
    }
}