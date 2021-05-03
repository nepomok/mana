using System;
using System.Collections.Generic;

namespace Mana
{
    public class Node
    {
        internal string name;
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

        public void AddComponent(Component component)
        {
            components.Add(component);
            Mana.Internal.SceneInterface.createComponent(name, component.type);
        }

        public void RemoveComponent(Component component)
        {
            components.Remove(component);
            Mana.Internal.SceneInterface.destroyComponent(name, component.type);
        }

        internal void _AddComponent(Component component)
        {
            components.Add(component);
        }
    }
}