using System;

namespace BQt.Audio
{
    public class OmniGain : IAudioNode
    {
        private float _volume = 1.0f;
        private readonly object _lock = new object();

        public string Name => "OmniGain";

        public void SetVolume(float vol)
        {
            lock (_lock)
            {
                _volume = vol;
            }
        }

        public void Process(float[] buffer)
        {
            lock (_lock)
            {
                if (Math.Abs(_volume - 1.0f) < float.Epsilon)
                {
                    return;
                }

                for (int i = 0; i < buffer.Length; i++)
                {
                    buffer[i] *= _volume;
                }
            }
        }
    }
}
