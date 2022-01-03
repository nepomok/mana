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

namespace Mana
{
    public class Vector3
    {
        public Vector3()
        {
            this.x = 0;
            this.y = 0;
            this.z = 0;
        }

        public Vector3(float v)
        {
            this.x = v;
            this.y = v;
            this.z = v;
        }

        public Vector3(float x, float y, float z)
        {
            this.x = x;
            this.y = y;
            this.z = z;
        }

        public static Vector3 operator +(Vector3 vecA, Vector3 vecB)
        {
            return new Vector3(vecA.x + vecB.x, vecA.y + vecB.y, vecA.z + vecB.z);
        }

        public static Vector3 operator -(Vector3 vecA, Vector3 vecB)
        {
            return new Vector3(vecA.x - vecB.x, vecA.y - vecB.y, vecA.z - vecB.z);
        }

        public static Vector3 operator /(Vector3 vecA, Vector3 vecB)
        {
            return new Vector3(vecA.x / vecB.x, vecA.y / vecB.y, vecA.z / vecB.z);
        }

        public static Vector3 operator *(Vector3 vecA, Vector3 vecB)
        {
            return new Vector3(vecA.x * vecB.x, vecA.y * vecB.y, vecA.z * vecB.z);
        }

        public static Vector3 operator +(Vector3 vecA, float f)
        {
            return new Vector3(vecA.x + f, vecA.y + f, vecA.z + f);
        }

        public static Vector3 operator -(Vector3 vecA, float f)
        {
            return new Vector3(vecA.x - f, vecA.y - f, vecA.z - f);
        }

        public static Vector3 operator /(Vector3 vecA, float f)
        {
            return new Vector3(vecA.x / f, vecA.y / f, vecA.z / f);
        }

        public static Vector3 operator *(Vector3 vecA, float f)
        {
            return new Vector3(vecA.x * f, vecA.y * f, vecA.z * f);
        }

        public float x;
        public float y;
        public float z;
    }
}