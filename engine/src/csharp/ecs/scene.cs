using System;
using System.Collections.Generic;

namespace Mana
{
    /**
        The scene and nodes are immutable for now only component values can be changed.
    */
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

        private void AddNode(string name, Node node)
        {
            if (nodes.ContainsKey(name))
                throw new ArgumentException("Node with name " + name + " already exists");

            Console.WriteLine("ADD NODE " + name + " " + node);

            for (int i = 0; i < node.GetComponentCount(); i++)
            {
                Console.WriteLine("Component: " + node.GetComponent(i).GetType());
            }

            Transform t = node.GetTransform();

            Console.WriteLine("TRANFORM " + t.position.x + " " + t.position.y + " " + t.position.z);

            nodes[name] = node;
        }

        public int GetNodesCount()
        {
            return nodes.Count;
        }
    }
}