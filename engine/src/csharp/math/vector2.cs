namespace Mana
{
    public class Vector2
    {
        public Vector2()
        {
            this.x = 0;
            this.y = 0;
        }

        public Vector2(float v)
        {
            this.x = v;
            this.y = v;
        }

        public Vector2(float x, float y)
        {
            this.x = x;
            this.y = y;
        }

        public static Vector2 operator +(Vector2 vecA, Vector2 vecB)
        {
            return new Vector2(vecA.x + vecB.x, vecA.y + vecB.y);
        }

        public static Vector2 operator -(Vector2 vecA, Vector2 vecB)
        {
            return new Vector2(vecA.x - vecB.x, vecA.y - vecB.y);
        }

        public static Vector2 operator /(Vector2 vecA, Vector2 vecB)
        {
            return new Vector2(vecA.x / vecB.x, vecA.y / vecB.y);
        }

        public static Vector2 operator *(Vector2 vecA, Vector2 vecB)
        {
            return new Vector2(vecA.x * vecB.x, vecA.y * vecB.y);
        }

        public static Vector2 operator +(Vector2 vecA, float f)
        {
            return new Vector2(vecA.x + f, vecA.y + f);
        }

        public static Vector2 operator -(Vector2 vecA, float f)
        {
            return new Vector2(vecA.x - f, vecA.y - f);
        }

        public static Vector2 operator /(Vector2 vecA, float f)
        {
            return new Vector2(vecA.x / f, vecA.y / f);
        }

        public static Vector2 operator *(Vector2 vecA, float f)
        {
            return new Vector2(vecA.x * f, vecA.y * f);
        }

        public float x;
        public float y;
    }
}