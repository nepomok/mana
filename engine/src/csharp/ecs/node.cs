using System;
using System.Collections.Generic;

namespace Mana
{
    public class Node
    {
        public string name;
        public readonly List<Component> components = new List<Component>();

        public void AddComponent(Component component)
        {
            components.Add(component);
        }

        public Component GetComponent(int index)
        {
            return components[index];
        }

        public int GetComponentCount()
        {
            return components.Count;
        }
    }
}