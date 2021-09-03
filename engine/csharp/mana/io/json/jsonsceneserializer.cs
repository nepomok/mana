namespace Mana.IO
{
    class JsonSceneSerializer : Serializer<Scene>
    {
        public void serialize(Scene data, System.IO.Stream stream)
        {
            var str = JsonCommon.convertScene(data).ToString();
            var buf = System.Text.ASCIIEncoding.Default.GetBytes(str);
            stream.Write(buf, 0, buf.Length);
        }
    }
}