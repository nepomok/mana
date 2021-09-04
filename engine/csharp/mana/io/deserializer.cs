using System.IO;

namespace Mana.IO
{
    public interface Deserializer<T>
    {
        T deserialize(Stream stream);
    }
}