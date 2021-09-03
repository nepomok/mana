

namespace Mana.IO
{
    class JsonComponentDeserializer : Deserializer<Component>
    {
        public Component deserialize(System.IO.Stream stream)
        {
            return new Component();
        }
    }
}