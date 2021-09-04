/**
 *  Mana - 3D Game Engine
 *  Copyright (C) 2021  Julian Zampiccoli
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

using System;
using System.IO;
using System.Collections.Generic;

using Mana.IO;

namespace Mana
{
    public class Node
    {
        public string name;
        public bool enabled;
        public readonly List<Component> components = new List<Component>();

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

            var stream = new MemoryStream();
            new JsonComponentSerializer().serialize(component, stream);
            stream.Seek(0, SeekOrigin.Begin);
            var reader = new StreamReader(stream);

            Mana.Internal.SceneInterface.createComponent(name, reader.ReadToEnd());
        }

        public void RemoveComponent(Component component)
        {
            components.Remove(component);
            Mana.Internal.SceneInterface.destroyComponent(name, component.type);
        }
    }
}