using System;
using System.Collections.Generic;

namespace BQt.Audio
{
    public class Voice
    {
        public float Velocity { get; set; }
        public double Phase { get; set; }

        public Voice(float velocity, double phase)
        {
            Velocity = velocity;
            Phase = phase;
        }
    }

    public class OmniSynthesizer : IAudioNode
    {
        private int _polyphony = 8;
        private readonly Dictionary<int, Voice> _activeNotes = new Dictionary<int, Voice>();
        private readonly double _sampleRate = 44100.0;
        private readonly object _lock = new object();

        public string Name => "OmniSynthesizer";

        public int Polyphony
        {
            get
            {
                lock (_lock)
                {
                    return _polyphony;
                }
            }
            set
            {
                lock (_lock)
                {
                    if (_polyphony != value)
                    {
                        _polyphony = value;
                        Console.WriteLine($"OmniSynthesizer C#: Polyphony set to {value}");
                    }
                }
            }
        }

        public void NoteOn(int channel, int note, int velocity)
        {
            lock (_lock)
            {
                if (_activeNotes.Count >= _polyphony)
                {
                    Console.WriteLine("OmniSynthesizer C#: Voice limit reached, ignoring NoteON");
                    return;
                }

                _activeNotes[note] = new Voice(velocity / 127.0f, 0.0);
                Console.WriteLine($"OmniSynthesizer C#: Native Synthesis NoteON -> CH: {channel} NOTE: {note}");
            }
        }

        public void NoteOff(int channel, int note)
        {
            lock (_lock)
            {
                _activeNotes.Remove(note);
                Console.WriteLine($"OmniSynthesizer C#: Native Synthesis NoteOFF -> CH: {channel} NOTE: {note}");
            }
        }

        public int ActiveNotesCount
        {
            get
            {
                lock (_lock)
                {
                    return _activeNotes.Count;
                }
            }
        }

        public void Process(float[] buffer)
        {
            lock (_lock)
            {
                if (_activeNotes.Count == 0)
                {
                    return;
                }

                for (int i = 0; i < buffer.Length; i++)
                {
                    float sample = 0.0f;

                    foreach (var kvp in _activeNotes)
                    {
                        int note = kvp.Key;
                        Voice voice = kvp.Value;

                        double freq = 440.0 * Math.Pow(2.0, (note - 69) / 12.0);
                        sample += voice.Velocity * (float)Math.Sin(2.0 * Math.PI * voice.Phase);

                        voice.Phase += freq / _sampleRate;
                        if (voice.Phase > 1.0)
                        {
                            voice.Phase -= 1.0;
                        }
                    }

                    buffer[i] += sample;
                }
            }
        }
    }
}
