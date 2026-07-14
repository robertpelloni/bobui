package com.bqt.audio;

import java.util.HashMap;
import java.util.Map;
import java.util.List;
import java.util.ArrayList;

public class AudioGraph {
    private final Map<String, AudioNode> nodes = new HashMap<>();
    private final Map<String, List<String>> links = new HashMap<>();
    private boolean isRunning = false;

    public synchronized void addNode(AudioNode node) {
        nodes.put(node.getName(), node);
    }

    public synchronized void connect(String srcName, String destName) throws IllegalArgumentException {
        if (!nodes.containsKey(srcName)) {
            throw new IllegalArgumentException("node " + srcName + " not found");
        }
        if (!nodes.containsKey(destName)) {
            throw new IllegalArgumentException("node " + destName + " not found");
        }

        links.computeIfAbsent(srcName, k -> new ArrayList<>()).add(destName);
    }

    public synchronized void processBlock(float[] buffer) {
        for (AudioNode node : nodes.values()) {
            node.process(buffer);
        }
    }

    public synchronized void start() {
        if (isRunning) return;
        isRunning = true;
        System.out.println("AudioGraph: Audio processing thread STARTED. (Simulating JUCE DeviceManager)");
    }

    public synchronized void stop() {
        if (!isRunning) return;
        isRunning = false;
        System.out.println("AudioGraph: Audio processing thread STOPPED.");
    }
}
