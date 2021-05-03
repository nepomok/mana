using System;
using System.Collections.Generic;

namespace Mana
{
    public class Node
    {
        private readonly List<Component> components = new List<Component>();

        public Component GetComponent(int index)
        {
            return components[index];
        }

        public int GetComponentCount()
        {
            return components.Count;
        }

        public Transform GetTransform()
        {
            foreach (var component in components)
            {
                if (component is Transform)
                    return component as Transform;
            }
            return null;
        }

        private void AddComponent(Component component)
        {
            components.Add(component);
        }
    }
}