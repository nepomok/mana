namespace Mana
{
    class Transform : Component
    {
        public Vector3 position;
        public Vector3 rotation;
        public Vector3 scale;
        
        public Transform parent;
    }
}