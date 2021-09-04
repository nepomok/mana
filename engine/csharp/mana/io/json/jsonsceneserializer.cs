using System.IO;
using Newtonsoft.Json.Linq;

namespace Mana.IO
{
    public class JsonSceneSerializer : Serializer<Scene>
    {
        public void serialize(Scene data, Stream stream)
        {
            var str = JsonCommon.convertScene(data).ToString();
            var writer = new StreamWriter(stream);
            writer.Write(str);
            writer.Flush();
        }
    }
}