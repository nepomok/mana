using Newtonsoft.Json;
using Newtonsoft.Json.Linq;

namespace Mana.IO
{
    static class JsonCommon
    {
        public static Component convertComponent(JObject component)
        {
            return new Component();
        }

        public static JObject convertComponent(Component component)
        {
            return new JObject();
        }

        public static Node convertNode(JObject node)
        {
            return new Node();
        }

        public static JObject convertNode(Node node)
        {
            return new JObject();
        }

        public static Scene convertScene(JObject scene)
        {
            return new Scene();
        }

        public static JObject convertScene(Scene scene)
        {
            return new JObject();
        }
    }
}