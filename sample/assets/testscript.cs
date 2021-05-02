using System;
using Mana;

class TestScript  : Script
{
    public override void OnEnable()
    {
        System.Console.WriteLine("OnEnable " + Time.deltaTime);
    }

    public override void OnUpdate()
    {
        System.Console.WriteLine("OnUpdate " + Time.deltaTime);
    }

    public override void OnDisable()
    {
        System.Console.WriteLine("OnDisable " + Time.deltaTime);
    }
}