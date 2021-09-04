using System.IO;

namespace Mana.IO
{
    public interface Serializer<T>
    {
        void serialize(T data, Stream stream);
    }
}