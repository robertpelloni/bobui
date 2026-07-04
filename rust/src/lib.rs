pub mod audio {
    pub mod audio_graph;
    pub mod omni_gain;
    pub mod synthesizer;

    #[cfg(test)]
    mod audio_graph_test;
    #[cfg(test)]
    mod omni_gain_test;
    #[cfg(test)]
    mod synthesizer_test;
}
