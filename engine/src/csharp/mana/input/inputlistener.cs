namespace Mana
{
    public class InputListener
    {
        public virtual void OnKeyDown(Key key) { }
        public virtual void OnKeyUp(Key key) { }

        public virtual void OnMouseMove(double xPos, double yPos) { }
        public virtual void OnMouseWheelScroll(double amount) { }
        public virtual void OnMouseKeyDown(MouseKey key) { }
        public virtual void OnMouseKeyUp(MouseKey key) { }

        public virtual void OnTextInput(string text) { }
    }
}