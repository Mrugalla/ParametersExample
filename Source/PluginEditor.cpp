#include "PluginProcessor.h"
#include "PluginEditor.h"

ParametersExampleAudioProcessorEditor::ParametersExampleAudioProcessorEditor(ParametersExampleAudioProcessor& p) :
    AudioProcessorEditor(&p),
    audioProcessor(p),
    sliders
    {
        AttachedSlider(p, param::PID::GainWet),
		AttachedSlider(p, param::PID::Frequency)
    }
{
    for (auto i = 0; i < sliders.size(); ++i)
    {
        auto& slider = sliders[i];

       addAndMakeVisible(slider.slider);
    }

    setSize(800, 600);
}

void ParametersExampleAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);
}

void ParametersExampleAudioProcessorEditor::resized()
{
    auto w = static_cast<float>(getWidth());
	auto h = static_cast<float>(getHeight());

    auto x = 0.f;
    auto y = 0.f;
    auto sliderWidth = w / static_cast<float>(sliders.size());
    for (auto i = 0; i < sliders.size(); ++i)
    {
        sliders[i].slider.setBounds(juce::Rectangle<float>(x, y, sliderWidth, h).toNearestInt());
		x += sliderWidth;
    }
}
