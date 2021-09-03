namespace Mana.IO
{
    class JsonSceneDeserializer : Deserializer<Scene>
    {
        public Scene deserialize(System.IO.Stream stream)
        {
            return new Scene();
        }
    }
}