using System;
using Mana;

class InputController : Script
{
    public float movementSpeed = 5; //Units per second
    
    public float rotationSpeed = 40; //Degrees per second

    public override void OnEnable()
    {
        System.Console.WriteLine("OnEnable TestScript");
    }

    public override void OnUpdate()
    {
        Node cam = Scene.scene.GetNode("MainCamera");
        Transform camT = cam.GetTransform();

        Vector3 inputMovement = new Vector3(0);

        if (Input.GetKey(Key.KEY_A) != KeyState.RELEASED) {
            inputMovement.x = 1;
        } else if (Input.GetKey(Key.KEY_D) != KeyState.RELEASED) {
            inputMovement.x = -1;
        }

        if (Input.GetKey(Key.KEY_W) != KeyState.RELEASED) {
            inputMovement.z = 1;
        } else if (Input.GetKey(Key.KEY_S) != KeyState.RELEASED) {
            inputMovement.z = -1;
        }

        if (Input.GetKey(Key.KEY_E) != KeyState.RELEASED) {
            inputMovement.y = 1;
        } else if (Input.GetKey(Key.KEY_Q) != KeyState.RELEASED) {
            inputMovement.y = -1;
        }

        camT.position = camT.position + camT.Forward() * inputMovement.z * movementSpeed * Time.deltaTime;
        camT.position = camT.position + camT.Left() * inputMovement.x * movementSpeed * Time.deltaTime;
        camT.position = camT.position + camT.Up() *inputMovement.y  * movementSpeed * Time.deltaTime;

        float rspeed = rotationSpeed * Time.deltaTime;

        if (Input.GetKey(Key.KEY_UP) != KeyState.RELEASED) {
            camT.rotation.x += rspeed;
        } else if (Input.GetKey(Key.KEY_DOWN) != KeyState.RELEASED) {
            camT.rotation.x -= rspeed;
        }

        if (Input.GetKey(Key.KEY_LEFT) != KeyState.RELEASED) {
            camT.rotation.y -= rspeed;
        } else if (Input.GetKey(Key.KEY_RIGHT) != KeyState.RELEASED) {
            camT.rotation.y += rspeed;
        }
    }

    public override void OnDisable()
    {
        System.Console.WriteLine("OnDisable TestScript");
    }
}