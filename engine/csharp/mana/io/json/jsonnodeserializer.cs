using System.IO;
using Newtonsoft.Json.Linq;

namespace Mana.IO
{
    public class JsonNodeSerializer : Serializer<Node>
    {
        public void serialize(Node data, Stream stream)
        {
            var str = JsonCommon.convertNode(data).ToString();
            var writer = new StreamWriter(stream);
            writer.Write(str);
            writer.Flush();
        }
    }
}