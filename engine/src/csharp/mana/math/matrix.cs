namespace Mana
{
    /**
    A 4 by 4 matrix of floats, stored row major.
    **/
    public class Matrix
    {
        public float this[int column, int row]
        {
            get => data[row * 4 + column];
            set => data[row * 4 + column] = value;
        }

        private readonly float[] data = new float[16];

        public void Set(int column, int row, float value)
        {
            this[column, row] = value;
        }

        public float Get(int column, int row)
        {
            return this[column, row];
        }

        public static Vector4 operator *(Matrix mat, Vector4 vec)
        {
            Vector4 ret = new Vector4();
            for (int row = 0; row < 4; row++)
            {
                float r = 0;
                switch (row)
                {
                    case 0:
                        r = ret.x;
                        break;
                    case 1:
                        r = ret.y;
                        break;
                    case 2:
                        r = ret.z;
                        break;
                    case 3:
                        r = ret.w;
                        break;
                    default:
                        break;
                }
                for (int column = 0; column < 4; column++)
                {
                    float f = 0;
                    switch (column)
                    {
                        case 0:
                            f = vec.x;
                            break;
                        case 1:
                            f = vec.y;
                            break;
                        case 2:
                            f = vec.z;
                            break;
                        case 3:
                            f = vec.w;
                            break;
                        default:
                            break;
                    }
                    r += mat[column, row] * f;
                }
                switch (row)
                {
                    case 0:
                        ret.x = r;
                        break;
                    case 1:
                        ret.y = r;
                        break;
                    case 2:
                        ret.z = r;
                        break;
                    case 3:
                        ret.w = r;
                        break;
                    default:
                        break;
                }
            }
            return ret;
        }
    }
}