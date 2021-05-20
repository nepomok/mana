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
    public class Transform : Component
    {
        public Transform()
        {
            base.type = ComponentType.TRANSFORM;
        }

        public Vector3 position;
        public Vector3 rotation;
        public Vector3 scale;

        public Transform parent;

        /**
            Returns the forward unit vector in world space.
        **/
        public Vector3 Forward()
        {
            Vector4 v = new Vector4(0, 0, -1, 1);
            Matrix m = MatrixMath.rotate(rotation.x, rotation.y, rotation.z);
            m = MatrixMath.inverse(m);
            Vector4 r = m * v;
            return new Vector3(r.x, r.y, r.z);
        }

        public Vector3 Left()
        {
            Vector4 v = new Vector4(-1, 0, 0, 1);
            Matrix m = MatrixMath.rotate(rotation.x, rotation.y, rotation.z);
            m = MatrixMath.inverse(m);
            Vector4 r = m * v;
            return new Vector3(r.x, r.y, r.z);
        }

        public Vector3 Up()
        {
            Vector4 v = new Vector4(0, 1, 0, 1);
            Matrix m = MatrixMath.rotate(rotation.x, rotation.y, rotation.z);
            m = MatrixMath.inverse(m);
            Vector4 r = m * v;
            return new Vector3(r.x, r.y, r.z);
        }
    }
}