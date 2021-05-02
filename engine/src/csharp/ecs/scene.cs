using System.Collections.Generic;

namespace Mana
{
    public class Scene
    {
        public readonly static Scene scene;

        public readonly Dictionary<string, Node> nodes = new Dictionary<string, Node>();

        public void SetNode(string name, Node node)
        {
            nodes[name] = node;
        }

        public Node GetNode(string name)
        {
            return nodes[name];
        }
    }
}