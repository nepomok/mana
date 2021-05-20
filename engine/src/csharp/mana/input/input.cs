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

using System.Collections.Generic;

namespace Mana
{
    public static class Input
    {
        private static readonly Dictionary<Key, KeyState> keys = new Dictionary<Key, KeyState>();
        private static readonly Mouse mouse = new Mouse();
        private static readonly List<InputListener> listeners = new List<InputListener>();

        public static KeyState GetKey(Key key)
        {
            if (keys.ContainsKey(key))
                return keys[key];
            else
                return KeyState.RELEASED;
        }

        public static bool GetKeyDown(Key key)
        {
            if (keys.ContainsKey(key))
                return keys[key] == KeyState.DOWN;
            else
                return false;
        }

        public static bool GetKeyUp(Key key)
        {
            if (keys.ContainsKey(key))
                return keys[key] == KeyState.UP;
            else
                return false;
        }

        public static KeyState GetMouseKey(MouseKey key)
        {
            return mouse.keys[key];
        }

        public static Mouse GetMouse()
        {
            return mouse;
        }

        public static bool GetMouseKeyDown(MouseKey key)
        {
            return mouse.keys[key] == KeyState.DOWN;
        }

        public static bool GetMouseKeyUp(MouseKey key)
        {
            return mouse.keys[key] == KeyState.UP;
        }

        public static void RegisterListener(InputListener listener)
        {
            if (listeners.Contains(listener))
                throw new System.ArgumentException("Listener already registered");
            listeners.Add(listener);
        }

        public static void UnregisterListener(InputListener listener)
        {
            if (!listeners.Contains(listener))
                throw new System.ArgumentException("Listener not registered");
            listeners.Add(listener);
        }

        private static void OnFrameEnd()
        {
            Dictionary<Key, KeyState> tmp = new Dictionary<Key, KeyState>();
            foreach (var k in keys)
            {
                if (k.Value == KeyState.DOWN)
                {
                    tmp[k.Key] = KeyState.HELD;
                }
                else if (k.Value == KeyState.UP)
                {
                    continue;
                }
                else
                {
                    tmp[k.Key] = k.Value;
                }
            }
            keys.Clear();
            foreach (var k in tmp)
            {
                keys[k.Key] = k.Value;
            }

            Dictionary<MouseKey, KeyState> mtmp = new Dictionary<MouseKey, KeyState>();
            foreach (var k in mouse.keys)
            {
                if (k.Value == KeyState.DOWN)
                {
                    mtmp[k.Key] = KeyState.HELD;
                }
                else if (k.Value == KeyState.UP)
                {
                    continue;
                }
                else
                {
                    mtmp[k.Key] = k.Value;
                }
            }
            mouse.keys.Clear();
            foreach (var k in mtmp)
            {
                mouse.keys[k.Key] = k.Value;
            }
        }

        private static void OnKeyDown(Key key)
        {
            keys[key] = KeyState.DOWN;
            foreach (var l in listeners)
                l.OnKeyDown(key);
        }

        private static void OnKeyUp(Key key)
        {
            keys[key] = KeyState.UP;
            foreach (var l in listeners)
                l.OnKeyUp(key);
        }

        private static void OnMouseMove(double xpos, double ypos)
        {
            mouse.positionX = xpos;
            mouse.positionY = ypos;
            foreach (var l in listeners)
                l.OnMouseMove(xpos, ypos);
        }

        private static void OnMouseWheelScroll(double ammount)
        {
            foreach (var l in listeners)
                l.OnMouseWheelScroll(ammount);
        }

        private static void OnMouseKeyDown(MouseKey key)
        {
            mouse.keys[key] = KeyState.DOWN;
            foreach (var l in listeners)
                l.OnMouseKeyDown(key);
        }

        private static void OnMouseKeyUp(MouseKey key)
        {
            mouse.keys[key] = KeyState.UP;
            foreach (var l in listeners)
                l.OnMouseKeyUp(key);
        }

        private static void OnTextInput(string text)
        {
            foreach (var l in listeners)
                l.OnTextInput(text);
        }
    }
}