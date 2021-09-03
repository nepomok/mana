namespace Mana.IO
{
    class JsonNodeSerializer : Serializer<Node>
    {
        public void serialize(Node data, System.IO.Stream stream)
        {
            var str = JsonCommon.convertNode(data).ToString();
            var buf = System.Text.ASCIIEncoding.Default.GetBytes(str);
            stream.Write(buf, 0, buf.Length);
        }
    }
}