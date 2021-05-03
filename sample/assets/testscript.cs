using System;
using Mana;

class TestScript : Script
{
    public override void OnEnable()
    {
        System.Console.WriteLine("OnEnable " + Time.deltaTime);
    }

    public override void OnUpdate()
    {
        System.Console.WriteLine("OnUpdate " + Time.deltaTime + " NODE COUNT: " + Scene.scene.GetNodesCount());

        Node n = Scene.scene.GetNode("plane");

        Transform t = n.GetTransform();

        t.position = t.position + new Vector3(1, 0, 0) * Time.deltaTime;
    }

    public override void OnDisable()
    {
        System.Console.WriteLine("OnDisable " + Time.deltaTime);
    }
}