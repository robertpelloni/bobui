use std::collections::HashMap;
use std::sync::{Arc, Mutex};
use super::audio_graph::AudioNode;

pub struct Voice {
    pub velocity: f32,
    pub phase: f64,
}

pub struct Synthesizer {
    polyphony: usize,
    active_notes: HashMap<usize, Voice>,
    sample_rate: f64,
}

pub struct OmniSynthesizer {
    inner: Arc<Mutex<Synthesizer>>,
}

impl OmniSynthesizer {
    pub fn new() -> Self {
        Self {
            inner: Arc::new(Mutex::new(Synthesizer {
                polyphony: 8,
                active_notes: HashMap::new(),
                sample_rate: 44100.0,
            })),
        }
    }

    pub fn polyphony(&self) -> usize {
        self.inner.lock().unwrap().polyphony
    }

    pub fn set_polyphony(&self, voices: usize) {
        let mut inner = self.inner.lock().unwrap();
        if inner.polyphony != voices {
            inner.polyphony = voices;
            println!("OmniSynthesizer Rust: Polyphony set to {}", voices);
        }
    }

    pub fn note_on(&self, channel: usize, note: usize, velocity: usize) {
        let mut inner = self.inner.lock().unwrap();
        if inner.active_notes.len() >= inner.polyphony {
            println!("OmniSynthesizer Rust: Voice limit reached, ignoring NoteON");
            return;
        }

        inner.active_notes.insert(note, Voice {
            velocity: (velocity as f32) / 127.0,
            phase: 0.0,
        });
        println!("OmniSynthesizer Rust: Native Synthesis NoteON -> CH: {} NOTE: {}", channel, note);
    }

    pub fn note_off(&self, channel: usize, note: usize) {
        let mut inner = self.inner.lock().unwrap();
        inner.active_notes.remove(&note);
        println!("OmniSynthesizer Rust: Native Synthesis NoteOFF -> CH: {} NOTE: {}", channel, note);
    }

    // Test helper
    pub fn active_notes_count(&self) -> usize {
        self.inner.lock().unwrap().active_notes.len()
    }
}

impl AudioNode for OmniSynthesizer {
    fn name(&self) -> &str {
        "OmniSynthesizer"
    }

    fn process(&mut self, buffer: &mut [f32]) {
        let mut inner = self.inner.lock().unwrap();

        if inner.active_notes.is_empty() {
            return;
        }

        let sample_rate = inner.sample_rate;

        for sample in buffer.iter_mut() {
            let mut mixed_sample = 0.0_f32;

            for (note, voice) in inner.active_notes.iter_mut() {
                let freq = 440.0 * 2.0_f64.powf((*note as f64 - 69.0) / 12.0);
                mixed_sample += voice.velocity * (2.0 * std::f64::consts::PI * voice.phase).sin() as f32;

                voice.phase += freq / sample_rate;
                if voice.phase > 1.0 {
                    voice.phase -= 1.0;
                }
            }

            *sample += mixed_sample;
        }
    }
}
