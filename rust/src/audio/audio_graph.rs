use std::collections::HashMap;



pub trait AudioNode: Send + Sync {
    fn name(&self) -> &str;
    fn process(&mut self, buffer: &mut [f32]);
}

pub struct AudioGraph {
    nodes: HashMap<String, Box<dyn AudioNode>>,
    links: HashMap<String, Vec<String>>,
    is_running: bool,
}

impl AudioGraph {
    pub fn new() -> Self {
        Self {
            nodes: HashMap::new(),
            links: HashMap::new(),
            is_running: false,
        }
    }

    pub fn add_node(&mut self, node: Box<dyn AudioNode>) {
        self.nodes.insert(node.name().to_string(), node);
    }

    pub fn connect(&mut self, src_name: &str, dest_name: &str) -> Result<(), String> {
        if !self.nodes.contains_key(src_name) {
            return Err(format!("node {} not found", src_name));
        }
        if !self.nodes.contains_key(dest_name) {
            return Err(format!("node {} not found", dest_name));
        }

        self.links.entry(src_name.to_string())
            .or_insert_with(Vec::new)
            .push(dest_name.to_string());

        Ok(())
    }

    pub fn process_block(&mut self, buffer: &mut [f32]) {
        for node in self.nodes.values_mut() {
            node.process(buffer);
        }
    }

    pub fn start(&mut self) {
        if self.is_running {
            return;
        }
        self.is_running = true;
        println!("AudioGraph: Audio processing thread STARTED. (Simulating JUCE DeviceManager)");
    }

    pub fn stop(&mut self) {
        if !self.is_running {
            return;
        }
        self.is_running = false;
        println!("AudioGraph: Audio processing thread STOPPED.");
    }
}
