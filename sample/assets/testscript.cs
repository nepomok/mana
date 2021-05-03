using System;
using Mana;

class TestScript : Script
{
    public override void OnEnable()
    {
        System.Console.WriteLine("OnEnable TestScript");
    }

    public override void OnUpdate()
    {
        Node n = Scene.scene.GetNode("plane");

        Transform t = n.GetTransform();

        t.position = t.position + new Vector3(1, 0, 0) * Time.deltaTime;

        t.rotation = t.rotation + new Vector3(0, 15, 0) * Time.deltaTime;
    }

    public override void OnDisable()
    {
        System.Console.WriteLine("OnDisable TestScript");
    }
}