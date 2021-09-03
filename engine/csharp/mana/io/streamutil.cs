using System.IO;

namespace Mana.IO
{
    public static class StreamUtil
    {
        public static Stream GetStringStream(string str)
        {
            var stream = new MemoryStream();
            var writer = new StreamWriter(stream);
            writer.Write(str);
            writer.Flush();
            stream.Position = 0;
            return stream;
        }
    }
}