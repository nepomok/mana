using System.IO;
using Newtonsoft.Json.Linq;

namespace Mana.IO
{
    public class JsonComponentSerializer : Serializer<Component>
    {
        public void serialize(Component data, Stream stream)
        {
            var str = JsonCommon.convertComponent(data).ToString();
            var writer = new StreamWriter(stream);
            writer.Write(str);
            writer.Flush();
        }
    }
}