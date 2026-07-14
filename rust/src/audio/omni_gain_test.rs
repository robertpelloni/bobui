use super::omni_gain::OmniGain;
use super::audio_graph::AudioNode;

#[test]
fn test_omni_gain() {
    let mut gain = OmniGain::new();
    assert_eq!(gain.name(), "OmniGain");

    gain.set_volume(0.5);

    let mut buffer = vec![1.0; 10];
    gain.process(&mut buffer);

    for sample in buffer.iter() {
        assert_eq!(*sample, 0.5);
    }
}
