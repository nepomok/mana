using System.IO;
using Newtonsoft.Json.Linq;

namespace Mana.IO
{
    public class JsonSceneDeserializer : Deserializer<Scene>
    {
        public Scene deserialize(Stream stream)
        {
            return JsonCommon.convertScene(JObject.Parse(new StreamReader(stream, System.Text.Encoding.UTF8).ReadToEnd()));
        }
    }
}