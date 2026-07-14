use crate::audio::audio_graph::{AudioGraph, AudioNode};

struct DummyNode {
    name: String,
}

impl DummyNode {
    fn new(name: &str) -> Self {
        Self {
            name: name.to_string(),
        }
    }
}

impl AudioNode for DummyNode {
    fn name(&self) -> &str {
        &self.name
    }

    fn process(&mut self, _buffer: &mut [f32]) {
        // Dummy processing
    }
}

#[test]
fn test_audio_graph_lifecycle() {
    let mut graph = AudioGraph::new();
    graph.start();
    graph.stop();
}

#[test]
fn test_audio_graph_connect() {
    let mut graph = AudioGraph::new();
    graph.add_node(Box::new(DummyNode::new("src")));
    graph.add_node(Box::new(DummyNode::new("dest")));

    assert!(graph.connect("src", "dest").is_ok());
    assert!(graph.connect("unknown", "dest").is_err());
}
