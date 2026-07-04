using System;

namespace BQt.Audio.Tests
{
    public class OmniGainTest
    {
        public static void Run()
        {
            TestOmniGain();
            Console.WriteLine("OmniGain tests passed.");
        }

        private static void TestOmniGain()
        {
            var gain = new OmniGain();
            if (gain.Name != "OmniGain")
            {
                throw new Exception($"Expected name OmniGain, got {gain.Name}");
            }

            gain.SetVolume(0.5f);

            float[] buffer = new float[10];
            for (int i = 0; i < buffer.Length; i++)
            {
                buffer[i] = 1.0f;
            }

            gain.Process(buffer);

            for (int i = 0; i < buffer.Length; i++)
            {
                if (Math.Abs(buffer[i] - 0.5f) > 0.000001f)
                {
                    throw new Exception($"Expected 0.5 at index {i}, got {buffer[i]}");
                }
            }
        }
    }
}
