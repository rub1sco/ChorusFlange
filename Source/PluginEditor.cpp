/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ChorusFlangeAudioProcessorEditor::ChorusFlangeAudioProcessorEditor (ChorusFlangeAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
    
    auto& params = processor.getParameters();
     
     
     //Dry wet mix
     AudioParameterFloat* dryWetParameter = (AudioParameterFloat*)params.getUnchecked(0);
     mDryWetSlider.setBounds(0, 0, 100, 100);
     mDryWetSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
     mDryWetSlider.setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
     mDryWetSlider.setRange(dryWetParameter -> range.start, dryWetParameter -> range.end);
     mDryWetSlider.setValue(*dryWetParameter);
    
     addAndMakeVisible(mDryWetSlider);
     
     mDryWetSlider.onValueChange = [this, dryWetParameter]{*dryWetParameter = mDryWetSlider.getValue();};
     mDryWetSlider.onDragStart = [dryWetParameter]{dryWetParameter -> beginChangeGesture();};
     mDryWetSlider.onDragEnd = [dryWetParameter]{dryWetParameter -> endChangeGesture();};
     
     //Depth
     AudioParameterFloat* depthParameter = (AudioParameterFloat*)params.getUnchecked(1);
     
     mDepthSlider.setBounds(100, 0, 100, 100);
     mDepthSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
     mDepthSlider.setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
     mDepthSlider.setRange(depthParameter -> range.start, depthParameter -> range.end);
     mDepthSlider.setValue(*depthParameter);
     addAndMakeVisible(mDepthSlider);
     
     mDepthSlider.onValueChange = [this, depthParameter]{*depthParameter = mDepthSlider.getValue();};
     mDepthSlider.onDragStart = [depthParameter]{depthParameter -> beginChangeGesture();};
     mDepthSlider.onDragEnd = [depthParameter]{depthParameter -> endChangeGesture();};

     //Rate
     AudioParameterFloat* rateParameter = (AudioParameterFloat*)params.getUnchecked(2);
     
     mRateSlider.setBounds(200, 0, 100, 100);
     mRateSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
     mRateSlider.setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
     mRateSlider.setRange(rateParameter -> range.start, rateParameter -> range.end);
     mRateSlider.setValue(*rateParameter);
     addAndMakeVisible(mRateSlider);
     
     mRateSlider.onValueChange = [this, rateParameter]{*rateParameter = mRateSlider.getValue();};
     mRateSlider.onDragStart = [rateParameter]{rateParameter -> beginChangeGesture();};
     mRateSlider.onDragEnd = [rateParameter]{rateParameter -> endChangeGesture();};
     
     //PhaseOffset
     AudioParameterFloat* phaseParameter = (AudioParameterFloat*)params.getUnchecked(3);
        
     mPhaseOffsetSlider.setBounds(300, 0, 100, 100);
     mPhaseOffsetSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
     mPhaseOffsetSlider.setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
     mPhaseOffsetSlider.setRange(phaseParameter -> range.start, phaseParameter -> range.end);
     mPhaseOffsetSlider.setValue(*phaseParameter);
    addAndMakeVisible(mPhaseOffsetSlider);
     
     mPhaseOffsetSlider.onValueChange = [this, phaseParameter]{*phaseParameter = mPhaseOffsetSlider.getValue();};
     mPhaseOffsetSlider.onDragStart = [phaseParameter]{phaseParameter -> beginChangeGesture();};
     mPhaseOffsetSlider.onDragEnd = [phaseParameter]{phaseParameter -> endChangeGesture();};
     
     //Feedback Parameter
     AudioParameterFloat* FeedbackParam = (AudioParameterFloat*) params.getUnchecked(4);
     
     mFeedbackSlider.setBounds(0, 100, 100, 100);
     mFeedbackSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
     mFeedbackSlider.setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
     mFeedbackSlider.setRange(FeedbackParam -> range.start, FeedbackParam -> range.end);
     mFeedbackSlider.setValue(*FeedbackParam);
     addAndMakeVisible(mFeedbackSlider);
     
     mFeedbackSlider.onValueChange = [this, FeedbackParam]{*FeedbackParam = mFeedbackSlider.getValue();};
     mFeedbackSlider.onDragStart = [FeedbackParam]{FeedbackParam -> beginChangeGesture();};
     mFeedbackSlider.onDragEnd = [FeedbackParam]{FeedbackParam -> endChangeGesture();};
     
     //comboBox type
     AudioParameterInt* typeParameter = (AudioParameterInt*)params.getUnchecked(5);
     mType.setBounds(100, 100, 100, 30);
     mType.addItem("Chorus", 1);
     mType.addItem("Flange", 2);

     addAndMakeVisible(mType);

     mType.onChange = [this,typeParameter] {
         typeParameter -> beginChangeGesture();
         *typeParameter = mType.getSelectedItemIndex();
         typeParameter -> endChangeGesture();
     };

     mType.setSelectedItemIndex(*typeParameter);
    
}

ChorusFlangeAudioProcessorEditor::~ChorusFlangeAudioProcessorEditor()
{
}

//==============================================================================
void ChorusFlangeAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(Colours::black);
    g.setColour (Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("Chorus/Flanger", getLocalBounds(), Justification::centred, 1);
}

void ChorusFlangeAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
