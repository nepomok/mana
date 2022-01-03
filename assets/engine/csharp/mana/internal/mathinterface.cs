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

using System.Runtime.InteropServices;

namespace Mana.Internal
{
    internal struct MarshalMatrix
    {
        float x0, y0, z0, w0;
        float x1, y1, z1, w1;
        float x2, y2, z2, w2;
        float x3, y3, z3, w3;

        internal static MarshalMatrix convert(Matrix mat)
        {
            MarshalMatrix ret = new MarshalMatrix();

            ret.x0 = mat[0, 0];
            ret.y0 = mat[1, 0];
            ret.z0 = mat[2, 0];
            ret.w0 = mat[3, 0];

            ret.x1 = mat[0, 1];
            ret.y1 = mat[1, 1];
            ret.z1 = mat[2, 1];
            ret.w1 = mat[3, 1];

            ret.x2 = mat[0, 2];
            ret.y2 = mat[1, 2];
            ret.z2 = mat[2, 2];
            ret.w2 = mat[3, 2];

            ret.x3 = mat[0, 3];
            ret.y3 = mat[1, 3];
            ret.z3 = mat[2, 3];
            ret.w3 = mat[3, 3];

            return ret;
        }

        internal static Matrix convert(MarshalMatrix mat)
        {
            Matrix ret = new Matrix();

            ret[0, 0] = mat.x0;
            ret[1, 0] = mat.y0;
            ret[2, 0] = mat.z0;
            ret[3, 0] = mat.w0;

            ret[0, 1] = mat.x1;
            ret[1, 1] = mat.y1;
            ret[2, 1] = mat.z1;
            ret[3, 1] = mat.w1;

            ret[0, 2] = mat.x2;
            ret[1, 2] = mat.y2;
            ret[2, 2] = mat.z2;
            ret[3, 2] = mat.w2;

            ret[0, 3] = mat.x3;
            ret[1, 3] = mat.y3;
            ret[2, 3] = mat.z3;
            ret[3, 3] = mat.w3;

            return ret;
        }
    };

    internal static class MathInterface
    {

        [DllImport("__Internal")]
        public static extern MarshalMatrix perspectiveMatrix(float fovy, float aspectRatio, float zNear, float zFar);

        [DllImport("__Internal")]
        public static extern MarshalMatrix orthographicMatrix(float left,
                                             float right,
                                             float bottom,
                                             float top,
                                             float zNear,
                                             float zFar);

        [DllImport("__Internal")]
        public static extern MarshalMatrix inverse(MarshalMatrix mat);

        [DllImport("__Internal")]
        public static extern MarshalMatrix transpose(MarshalMatrix mat);

        [DllImport("__Internal")]
        public static extern MarshalMatrix translate(float x, float y, float z);

        [DllImport("__Internal")]
        public static extern MarshalMatrix scale(float x, float y, float z);

        [DllImport("__Internal")]
        public static extern MarshalMatrix rotate(float x, float y, float z);
    }

}