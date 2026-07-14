package com.bqt.audio;

import java.util.HashMap;
import java.util.Map;

public class OmniSynthesizer implements AudioNode {
    private int polyphony = 8;
    private final Map<Integer, Voice> activeNotes = new HashMap<>();
    private final double sampleRate = 44100.0;
    private final String name = "OmniSynthesizer";

    @Override
    public String getName() {
        return name;
    }

    public synchronized int getPolyphony() {
        return polyphony;
    }

    public synchronized void setPolyphony(int voices) {
        if (this.polyphony != voices) {
            this.polyphony = voices;
            System.out.println("OmniSynthesizer Java: Polyphony set to " + voices);
        }
    }

    public synchronized void noteOn(int channel, int note, int velocity) {
        if (activeNotes.size() >= polyphony) {
            System.out.println("OmniSynthesizer Java: Voice limit reached, ignoring NoteON");
            return;
        }

        activeNotes.put(note, new Voice(velocity / 127.0f, 0.0));
        System.out.println("OmniSynthesizer Java: Native Synthesis NoteON -> CH: " + channel + " NOTE: " + note);
    }

    public synchronized void noteOff(int channel, int note) {
        activeNotes.remove(note);
        System.out.println("OmniSynthesizer Java: Native Synthesis NoteOFF -> CH: " + channel + " NOTE: " + note);
    }

    // For testing purposes
    public synchronized int getActiveNotesCount() {
        return activeNotes.size();
    }

    @Override
    public synchronized void process(float[] buffer) {
        if (activeNotes.isEmpty()) {
            return;
        }

        for (int i = 0; i < buffer.length; i++) {
            float sample = 0.0f;

            for (Map.Entry<Integer, Voice> entry : activeNotes.entrySet()) {
                int note = entry.getKey();
                Voice voice = entry.getValue();

                double freq = 440.0 * Math.pow(2.0, (note - 69) / 12.0);
                sample += voice.velocity * (float) Math.sin(2.0 * Math.PI * voice.phase);

                voice.phase += freq / sampleRate;
                if (voice.phase > 1.0) {
                    voice.phase -= 1.0;
                }
            }

            buffer[i] += sample;
        }
    }
}
