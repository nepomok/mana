namespace Mana.IO
{
    public class JsonComponentSerializer : Serializer<Component>
    {
        public string serialize(Component data)
        {
            var str = JsonCommon.convertComponent(data).ToString();
            return str;
        }
    }
}