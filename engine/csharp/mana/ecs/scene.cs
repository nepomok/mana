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
using System.Collections.Generic;

using Mana.IO;

namespace Mana
{
    public class Scene
    {
        internal static Scene scene_internal;

        internal static void setScene(Scene scene)
        {
            scene_internal = scene;
        }

        public static Scene scene
        {
            get { return scene_internal; }
        }

        public string name;

        public readonly Dictionary<string, Node> nodes = new Dictionary<string, Node>();

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
            Mana.Extern.SceneInterface.createNode(name);
            return nodes[name];
        }

        public void DestroyNode(string name)
        {
            nodes.Remove(name);
            Mana.Extern.SceneInterface.destroyNode(name);
        }

        public int GetNodesCount()
        {
            return nodes.Count;
        }
    }
}