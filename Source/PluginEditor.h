#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"
#include <array>
#include <functional>

struct AttachedSlider
{
    using Slider = juce::Slider;
    using Attachment = juce::SliderParameterAttachment;
	using Component = juce::Component;

    AttachedSlider(ParametersExampleAudioProcessor& processor, param::PID pID) :
        slider(),
        attachment(*processor.params[static_cast<int>(pID)], slider, nullptr)
    {
		slider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    }

    Slider slider;
    Attachment attachment;
};

struct ParametersExampleAudioProcessorEditor :
    public juce::AudioProcessorEditor
{
    using AttachedSliderArray = std::array<AttachedSlider, param::NumParams>;

    ParametersExampleAudioProcessorEditor(ParametersExampleAudioProcessor&);

    void paint (juce::Graphics&) override;
    void resized() override;

    ParametersExampleAudioProcessor& audioProcessor;
    AttachedSliderArray sliders;
};
