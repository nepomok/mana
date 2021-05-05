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