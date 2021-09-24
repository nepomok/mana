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
using System.Runtime.InteropServices;

using Mana.IO;

namespace Mana.Internal
{
    internal static class SceneInterface
    {
        internal static void setSceneJson(string json)
        {
            // Calling Newtonsoft.Json.Linq.JObject.Parse here results in a memory leak.
            var o =  Newtonsoft.Json.Linq.JObject.Parse(json);

            Scene.scene_internal = JsonCommon.convertScene(o);
        }

        internal static string getSceneJson()
        {
            return JsonCommon.convertScene(Scene.scene_internal).ToString();
        }

        [DllImport("__Internal")]
        public static extern void createNode(string name, string json);

        [DllImport("__Internal")]
        public static extern void destroyNode(string name);

        [DllImport("__Internal")]
        public static extern void createComponent(string node, string json);

        [DllImport("__Internal")]
        public static extern void destroyComponent(string node, ComponentType type);
    }
}