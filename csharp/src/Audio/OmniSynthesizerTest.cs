using System;

namespace BQt.Audio.Tests
{
    public class OmniSynthesizerTest
    {
        public static void Run()
        {
            TestSynthesizerPolyphony();
            TestSynthesizerNoteOnOff();
            TestSynthesizerProcessBlock();
            Console.WriteLine("OmniSynthesizer tests passed.");
        }

        private static void TestSynthesizerPolyphony()
        {
            var synth = new OmniSynthesizer();

            if (synth.Polyphony != 8)
            {
                throw new Exception($"Expected default polyphony to be 8, got {synth.Polyphony}");
            }

            synth.Polyphony = 16;
            if (synth.Polyphony != 16)
            {
                throw new Exception($"Expected polyphony to be 16 after setting, got {synth.Polyphony}");
            }
        }

        private static void TestSynthesizerNoteOnOff()
        {
            var synth = new OmniSynthesizer();
            synth.Polyphony = 2;

            synth.NoteOn(1, 60, 100);
            synth.NoteOn(1, 64, 100);
            synth.NoteOn(1, 67, 100); // Should be ignored

            if (synth.ActiveNotesCount != 2)
            {
                throw new Exception($"Expected 2 active notes, got {synth.ActiveNotesCount}");
            }

            synth.NoteOff(1, 60);

            if (synth.ActiveNotesCount != 1)
            {
                throw new Exception($"Expected 1 active note after NoteOff, got {synth.ActiveNotesCount}");
            }
        }

        private static void TestSynthesizerProcessBlock()
        {
            var synth = new OmniSynthesizer();
            float[] buffer = new float[256];

            synth.Process(buffer);
            for (int i = 0; i < buffer.Length; i++)
            {
                if (buffer[i] != 0.0f)
                {
                    throw new Exception($"Expected buffer to be empty, but got {buffer[i]} at index {i}");
                }
            }

            synth.NoteOn(1, 60, 100);
            synth.Process(buffer);

            bool nonZeroFound = false;
            foreach (float sample in buffer)
            {
                if (sample != 0.0f)
                {
                    nonZeroFound = true;
                    break;
                }
            }

            if (!nonZeroFound)
            {
                throw new Exception("Expected buffer to contain non-zero audio data after Process with active note");
            }
        }
    }
}
