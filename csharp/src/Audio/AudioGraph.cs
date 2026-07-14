using System;
using System.Collections.Generic;

namespace BQt.Audio
{
    public class AudioGraph
    {
        private readonly Dictionary<string, IAudioNode> _nodes = new Dictionary<string, IAudioNode>();
        private readonly Dictionary<string, List<string>> _links = new Dictionary<string, List<string>>();
        private readonly object _lock = new object();
        private bool _isRunning = false;

        public void AddNode(IAudioNode node)
        {
            lock (_lock)
            {
                _nodes[node.Name] = node;
            }
        }

        public void Connect(string srcName, string destName)
        {
            lock (_lock)
            {
                if (!_nodes.ContainsKey(srcName))
                    throw new ArgumentException($"node {srcName} not found");
                if (!_nodes.ContainsKey(destName))
                    throw new ArgumentException($"node {destName} not found");

                if (!_links.ContainsKey(srcName))
                    _links[srcName] = new List<string>();

                _links[srcName].Add(destName);
            }
        }

        public void ProcessBlock(float[] buffer)
        {
            lock (_lock)
            {
                foreach (var node in _nodes.Values)
                {
                    node.Process(buffer);
                }
            }
        }

        public void Start()
        {
            lock (_lock)
            {
                if (_isRunning) return;
                _isRunning = true;
                Console.WriteLine("AudioGraph: Audio processing thread STARTED. (Simulating JUCE DeviceManager)");
            }
        }

        public void Stop()
        {
            lock (_lock)
            {
                if (!_isRunning) return;
                _isRunning = false;
                Console.WriteLine("AudioGraph: Audio processing thread STOPPED.");
            }
        }
    }
}
