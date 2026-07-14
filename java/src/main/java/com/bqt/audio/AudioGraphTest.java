package com.bqt.audio;

public class AudioGraphTest {

    static class DummyNode implements AudioNode {
        private final String name;

        public DummyNode(String name) {
            this.name = name;
        }

        @Override
        public String getName() {
            return name;
        }

        @Override
        public void process(float[] buffer) {
            // Dummy processing
        }
    }

    public static void main(String[] args) {
        testAudioGraphLifecycle();
        testAudioGraphConnect();
        System.out.println("All tests passed.");
    }

    private static void testAudioGraphLifecycle() {
        AudioGraph graph = new AudioGraph();
        graph.start();
        graph.stop();
    }

    private static void testAudioGraphConnect() {
        AudioGraph graph = new AudioGraph();
        graph.addNode(new DummyNode("src"));
        graph.addNode(new DummyNode("dest"));

        try {
            graph.connect("src", "dest");
        } catch (IllegalArgumentException e) {
            throw new RuntimeException("Test failed: Should be able to connect existing nodes");
        }

        boolean threw = false;
        try {
            graph.connect("unknown", "dest");
        } catch (IllegalArgumentException e) {
            threw = true;
        }

        if (!threw) {
            throw new RuntimeException("Test failed: Should throw when connecting unknown nodes");
        }
    }
}
