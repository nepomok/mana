namespace Mana.IO
{
    public class JsonNodeSerializer : Serializer<Node>
    {
        public string serialize(Node data)
        {
            var str = JsonCommon.convertNode(data).ToString();
            return str;
        }
    }
}