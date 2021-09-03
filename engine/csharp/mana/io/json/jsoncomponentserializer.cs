namespace Mana.IO
{
    class JsonComponentSerializer : Serializer<Component>
    {
        public void serialize(Component data, System.IO.Stream stream)
        {
            var str = JsonCommon.convertComponent(data).ToString();
            var buf = System.Text.ASCIIEncoding.Default.GetBytes(str);
            stream.Write(buf, 0, buf.Length);
        }
    }
}