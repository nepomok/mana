namespace Mana
{
    public class Camera : Component
    {
        public enum CameraType
        {
            PERSPECTIVE,
            ORTHOGRAPHIC
        }

        public Camera()
        {
            base.type = ComponentType.CAMERA;
        }

        CameraType cameraType;

        float nearClip;
        float farClip;

        float left;
        float top;
        float right;
        float bottom;

        float fov;
        float aspectRatio;
    }
}