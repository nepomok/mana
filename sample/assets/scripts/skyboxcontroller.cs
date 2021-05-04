using System;
using Mana;

class SkyboxController : Script
{
    public override void OnEnable()
    {
        System.Console.WriteLine("OnEnable SkyboxController");
    }

    public override void OnUpdate()
    {
        Node cam = Scene.scene.GetNode("mainCamera");
        Node skybox = Scene.scene.GetNode("skybox");
        Transform camT = cam.GetTransform();
        Transform skyT = skybox.GetTransform();
        skyT.position = camT.position;
    }

    public override void OnDisable()
    {
        System.Console.WriteLine("OnDisable SkyboxController");
    }
}