namespace Mana.IO
{
    class JsonNodeDeserializer : Deserializer<Node>
    {
        public Node deserialize(System.IO.Stream stream)
        {
            return new Node();
        }
    }
}