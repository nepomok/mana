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
    public class Vector4
    {
        public Vector4()
        {
            this.x = 0;
            this.y = 0;
            this.z = 0;
            this.w = 0;
        }

        public Vector4(float v)
        {
            this.x = v;
            this.y = v;
            this.z = v;
            this.w = v;
        }

        public Vector4(float x, float y, float z, float w)
        {
            this.x = x;
            this.y = y;
            this.z = z;
            this.w = w;
        }

        public static Vector4 operator +(Vector4 vecA, Vector4 vecB)
        {
            return new Vector4(vecA.x + vecB.x, vecA.y + vecB.y, vecA.z + vecB.z, vecA.w + vecB.w);
        }

        public static Vector4 operator -(Vector4 vecA, Vector4 vecB)
        {
            return new Vector4(vecA.x - vecB.x, vecA.y - vecB.y, vecA.z - vecB.z, vecA.w - vecB.w);
        }

        public static Vector4 operator /(Vector4 vecA, Vector4 vecB)
        {
            return new Vector4(vecA.x / vecB.x, vecA.y / vecB.y, vecA.z / vecB.z, vecA.w / vecB.w);
        }

        public static Vector4 operator *(Vector4 vecA, Vector4 vecB)
        {
            return new Vector4(vecA.x * vecB.x, vecA.y * vecB.y, vecA.z * vecB.z, vecA.w * vecB.w);
        }

        public static Vector4 operator +(Vector4 vecA, float f)
        {
            return new Vector4(vecA.x + f, vecA.y + f, vecA.z + f, vecA.w + f);
        }

        public static Vector4 operator -(Vector4 vecA, float f)
        {
            return new Vector4(vecA.x - f, vecA.y - f, vecA.z - f, vecA.w - f);
        }

        public static Vector4 operator /(Vector4 vecA, float f)
        {
            return new Vector4(vecA.x / f, vecA.y / f, vecA.z / f, vecA.w / f);
        }

        public static Vector4 operator *(Vector4 vecA, float f)
        {
            return new Vector4(vecA.x * f, vecA.y * f, vecA.z * f, vecA.w * f);
        }

        public float x;
        public float y;
        public float z;
        public float w;
    }
}