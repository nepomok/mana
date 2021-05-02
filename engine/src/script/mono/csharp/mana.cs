namespace Mana
{
    public class Time
    {
        public static float deltaTime;

        public static void _internal_setDeltaTime(float deltaTime){ System.Console.WriteLine("DeltaTime: " + deltaTime); Time.deltaTime = deltaTime; }
    }

    public class Script
    {
        public virtual void OnEnable() {}

        public virtual void OnDisable() {}

        public virtual void OnUpdate() {}
    }
}