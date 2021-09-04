using System.IO;
using Newtonsoft.Json.Linq;

namespace Mana.IO
{
    public class JsonComponentDeserializer : Deserializer<Component>
    {
        public Component deserialize(Stream stream)
        {
            return JsonCommon.convertComponent(JObject.Parse(new StreamReader(stream, System.Text.Encoding.UTF8).ReadToEnd()));
        }
    }
}