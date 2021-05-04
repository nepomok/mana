using System;
using Mana;

class InputController : Script
{
    public override void OnEnable()
    {
        System.Console.WriteLine("OnEnable TestScript");
    }

    public override void OnUpdate()
    {
        Node cam = Scene.scene.GetNode("mainCamera");
        Transform camT = cam.GetTransform();
        if (Input.GetKey(Key.KEY_W) != KeyState.RELEASED)
        {
            camT.position = camT.position + new Vector3(0, 0, -1) * Time.deltaTime;
        }
    }

    public override void OnDisable()
    {
        System.Console.WriteLine("OnDisable TestScript");
    }
}