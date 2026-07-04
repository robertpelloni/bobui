use super::synthesizer::OmniSynthesizer;
use super::audio_graph::AudioNode;

#[test]
fn test_synthesizer_polyphony() {
    let synth = OmniSynthesizer::new();
    assert_eq!(synth.polyphony(), 8);

    synth.set_polyphony(16);
    assert_eq!(synth.polyphony(), 16);
}

#[test]
fn test_synthesizer_note_on_off() {
    let synth = OmniSynthesizer::new();
    synth.set_polyphony(2);

    synth.note_on(1, 60, 100);
    synth.note_on(1, 64, 100);
    synth.note_on(1, 67, 100); // Should be ignored

    assert_eq!(synth.active_notes_count(), 2);

    synth.note_off(1, 60);

    assert_eq!(synth.active_notes_count(), 1);
}

#[test]
fn test_synthesizer_process_block() {
    let mut synth = OmniSynthesizer::new();
    let mut buffer = vec![0.0; 256];

    synth.process(&mut buffer);
    for sample in buffer.iter() {
        assert_eq!(*sample, 0.0);
    }

    synth.note_on(1, 60, 100);
    synth.process(&mut buffer);

    let mut non_zero_found = false;
    for sample in buffer.iter() {
        if *sample != 0.0 {
            non_zero_found = true;
            break;
        }
    }

    assert!(non_zero_found, "Expected buffer to contain non-zero audio data");
}
