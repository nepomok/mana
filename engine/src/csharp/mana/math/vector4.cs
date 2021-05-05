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