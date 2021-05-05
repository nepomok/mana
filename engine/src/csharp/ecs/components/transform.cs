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