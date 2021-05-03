using System.Runtime.InteropServices;

namespace Mana.Internal
{
    internal static class SceneInterface
    {
        [DllImport("__Internal")]
        public static extern void createNode(string name);

        [DllImport("__Internal")]
        public static extern void destroyNode(string name);

        [DllImport("__Internal")]
        public static extern void createComponent(string nodeName, ComponentType type);

        [DllImport("__Internal")]
        public static extern void destroyComponent(string nodeName, ComponentType type);
    }
}