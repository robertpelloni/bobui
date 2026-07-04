namespace BQt.Audio
{
    public interface IAudioNode
    {
        string Name { get; }
        void Process(float[] buffer);
    }
}
