using System;
using System.Collections.Generic;

namespace Mana
{
    public class Scene
    {
        public readonly static Scene scene;

        private readonly Dictionary<string, Node> nodes = new Dictionary<string, Node>();

        public Node GetNode(string name)
        {
            if (!nodes.ContainsKey(name))
                throw new ArgumentException("Node not found " + name);
            return nodes[name];
        }

        public Node CreateNode(string name)
        {
            if (nodes.ContainsKey(name))
                throw new ArgumentException("Node with name " + name + " already exists");
            Mana.Internal.SceneInterface.createNode(name);
            return nodes[name];
        }

        public void DestroyNode(string name)
        {
            nodes.Remove(name);
            Mana.Internal.SceneInterface.destroyNode(name);
        }

        public int GetNodesCount()
        {
            return nodes.Count;
        }

        private void _AddNode(string name, Node node)
        {
            if (nodes.ContainsKey(name))
                throw new ArgumentException("Node with name " + name + " already exists");

            node.name = name;
            nodes[name] = node;
        }
    }
}