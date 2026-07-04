package com.bqt.audio;

public class OmniGain implements AudioNode {
    private float volume = 1.0f;
    private final String name = "OmniGain";

    @Override
    public String getName() {
        return name;
    }

    public synchronized void setVolume(float vol) {
        this.volume = vol;
    }

    @Override
    public synchronized void process(float[] buffer) {
        if (Math.abs(this.volume - 1.0f) < 0.000001f) {
            return;
        }

        for (int i = 0; i < buffer.length; i++) {
            buffer[i] *= this.volume;
        }
    }
}
