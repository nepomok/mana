using System.IO;
using Newtonsoft.Json.Linq;

namespace Mana.IO
{
    class JsonComponentDeserializer : Deserializer<Component>
    {
        public Component deserialize(Stream stream)
        {
            return JsonCommon.convertComponent(JObject.Parse(new StreamReader(stream).ReadToEnd()));
        }
    }
}