using System.IO;
using Newtonsoft.Json.Linq;

namespace Mana.IO
{
    public class JsonNodeDeserializer : Deserializer<Node>
    {
        public Node deserialize(Stream stream)
        {
            return JsonCommon.convertNode(JObject.Parse(new StreamReader(stream, System.Text.Encoding.UTF8).ReadToEnd()));
        }
    }
}