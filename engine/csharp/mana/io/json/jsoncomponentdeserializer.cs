using System.IO;
using Newtonsoft.Json.Linq;

namespace Mana.IO
{
    public class JsonComponentDeserializer : Deserializer<Component>
    {
        public Component deserialize(string data)
        {
            return JsonCommon.convertComponent(JObject.Parse(data));
        }
    }
}