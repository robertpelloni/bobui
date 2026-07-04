package com.bqt.audio;

public interface AudioNode {
    String getName();
    void process(float[] buffer);
}
