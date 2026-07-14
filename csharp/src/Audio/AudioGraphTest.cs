using System;

namespace BQt.Audio.Tests
{
    public class AudioGraphTest
    {
        private class DummyNode : IAudioNode
        {
            public string Name { get; }

            public DummyNode(string name)
            {
                Name = name;
            }

            public void Process(float[] buffer)
            {
            }
        }

        public static void Main()
        {
            TestAudioGraphLifecycle();
            TestAudioGraphConnect();
            Console.WriteLine("All tests passed.");
            OmniGainTest.Run();
            OmniSynthesizerTest.Run();
        }

        private static void TestAudioGraphLifecycle()
        {
            var graph = new AudioGraph();
            graph.Start();
            graph.Stop();
        }

        private static void TestAudioGraphConnect()
        {
            var graph = new AudioGraph();
            graph.AddNode(new DummyNode("src"));
            graph.AddNode(new DummyNode("dest"));

            try
            {
                graph.Connect("src", "dest");
            }
            catch (Exception)
            {
                throw new Exception("Test failed: Should be able to connect existing nodes");
            }

            bool threw = false;
            try
            {
                graph.Connect("unknown", "dest");
            }
            catch (ArgumentException)
            {
                threw = true;
            }

            if (!threw)
            {
                throw new Exception("Test failed: Should throw when connecting unknown nodes");
            }
        }
    }
}
