package com.bqt.audio;

public class OmniGainTest {
    public static void main(String[] args) {
        testOmniGain();
        System.out.println("OmniGain tests passed.");
    }

    private static void testOmniGain() {
        OmniGain gain = new OmniGain();
        if (!"OmniGain".equals(gain.getName())) {
            throw new RuntimeException("Expected name OmniGain, got " + gain.getName());
        }

        gain.setVolume(0.5f);

        float[] buffer = new float[10];
        for (int i = 0; i < buffer.length; i++) {
            buffer[i] = 1.0f;
        }

        gain.process(buffer);

        for (int i = 0; i < buffer.length; i++) {
            if (Math.abs(buffer[i] - 0.5f) > 0.000001f) {
                throw new RuntimeException("Expected 0.5 at index " + i + ", got " + buffer[i]);
            }
        }
    }
}
