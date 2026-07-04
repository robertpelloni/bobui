package com.bqt.audio;

public class OmniSynthesizerTest {
    public static void main(String[] args) {
        testSynthesizerPolyphony();
        testSynthesizerNoteOnOff();
        testSynthesizerProcessBlock();
        System.out.println("OmniSynthesizer tests passed.");
    }

    private static void testSynthesizerPolyphony() {
        OmniSynthesizer synth = new OmniSynthesizer();

        if (synth.getPolyphony() != 8) {
            throw new RuntimeException("Expected default polyphony to be 8, got " + synth.getPolyphony());
        }

        synth.setPolyphony(16);
        if (synth.getPolyphony() != 16) {
            throw new RuntimeException("Expected polyphony to be 16 after setting, got " + synth.getPolyphony());
        }
    }

    private static void testSynthesizerNoteOnOff() {
        OmniSynthesizer synth = new OmniSynthesizer();
        synth.setPolyphony(2);

        synth.noteOn(1, 60, 100);
        synth.noteOn(1, 64, 100);
        synth.noteOn(1, 67, 100); // Should be ignored

        if (synth.getActiveNotesCount() != 2) {
            throw new RuntimeException("Expected 2 active notes, got " + synth.getActiveNotesCount());
        }

        synth.noteOff(1, 60);

        if (synth.getActiveNotesCount() != 1) {
            throw new RuntimeException("Expected 1 active note after NoteOff, got " + synth.getActiveNotesCount());
        }
    }

    private static void testSynthesizerProcessBlock() {
        OmniSynthesizer synth = new OmniSynthesizer();
        float[] buffer = new float[256];

        synth.process(buffer);
        for (int i = 0; i < buffer.length; i++) {
            if (buffer[i] != 0.0f) {
                throw new RuntimeException("Expected buffer to be empty, but got " + buffer[i] + " at index " + i);
            }
        }

        synth.noteOn(1, 60, 100);
        synth.process(buffer);

        boolean nonZeroFound = false;
        for (float sample : buffer) {
            if (sample != 0.0f) {
                nonZeroFound = true;
                break;
            }
        }

        if (!nonZeroFound) {
            throw new RuntimeException("Expected buffer to contain non-zero audio data after Process with active note");
        }
    }
}
