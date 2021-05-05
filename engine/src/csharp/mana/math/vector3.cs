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