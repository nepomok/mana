using System.IO;

namespace Mana.IO
{
    public interface Serializer<T>
    {
        string serialize(T data);
    }
}