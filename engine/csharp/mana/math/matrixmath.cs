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

using Mana.Internal;

namespace Mana
{
    public static class MatrixMath
    {
        public static Matrix perspective(float fovy, float aspectRatio, float zNear, float zFar)
        {
            return MarshalMatrix.convert(MathInterface.perspectiveMatrix(fovy, aspectRatio, zNear, zFar));
        }

        public static Matrix ortho(float left, float right, float bottom, float top, float zNear, float zFar)
        {
            return MarshalMatrix.convert(MathInterface.orthographicMatrix(left, right, bottom, top, zNear, zFar));
        }

        public static Matrix inverse(Matrix mat)
        {
            return MarshalMatrix.convert(MathInterface.inverse(MarshalMatrix.convert(mat)));
        }

        public static Matrix transpose(Matrix mat)
        {
            return MarshalMatrix.convert(MathInterface.transpose(MarshalMatrix.convert(mat)));
        }

        public static Matrix translate(float x, float y, float z)
        {
            return MarshalMatrix.convert(MathInterface.translate(x, y, z));
        }

        public static Matrix scale(float x, float y, float z)
        {
            return MarshalMatrix.convert(MathInterface.scale(x, y, z));
        }

        public static Matrix rotate(float x, float y, float z)
        {
            return MarshalMatrix.convert(MathInterface.rotate(x, y, z));
        }
    }
}