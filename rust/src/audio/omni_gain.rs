use std::sync::{Arc, Mutex};
use super::audio_graph::AudioNode;

pub struct OmniGain {
    volume: Arc<Mutex<f32>>,
}

impl OmniGain {
    pub fn new() -> Self {
        Self {
            volume: Arc::new(Mutex::new(1.0)),
        }
    }

    pub fn set_volume(&self, vol: f32) {
        let mut v = self.volume.lock().unwrap();
        *v = vol;
    }
}

impl AudioNode for OmniGain {
    fn name(&self) -> &str {
        "OmniGain"
    }

    fn process(&mut self, buffer: &mut [f32]) {
        let vol = *self.volume.lock().unwrap();
        if (vol - 1.0).abs() < f32::EPSILON {
            return;
        }

        for sample in buffer.iter_mut() {
            *sample *= vol;
        }
    }
}
