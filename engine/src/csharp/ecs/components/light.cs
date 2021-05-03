namespace Mana
{
    public class Light : Component
    {
        public enum LightType
        {
            DIRECTIONAL,
            POINT,
            SPOT
        }

        public Light()
        {
            base.type = ComponentType.LIGHT;
        }

        LightType lightType;

        Vector3 ambient;
        Vector3 diffuse;
        Vector3 specular;

        Vector3 direction;

        float cutOff;
        float outerCutOff;

        float constant;
        float linear;
        float quadratic;
    }
}