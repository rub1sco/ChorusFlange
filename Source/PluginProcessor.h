/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once
#define MAX_DELAYTIME 2

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/**
*/
class ChorusFlangeAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    ChorusFlangeAudioProcessor();
    ~ChorusFlangeAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    
    float lin_inter(float sample_x, float sample_x1, float inPhase);

    private:
    //parameter declarations
    AudioParameterFloat* mDryWetParameter;
    AudioParameterFloat* mDepthParameter;
    AudioParameterFloat* mRateParameter;
    AudioParameterFloat* mPhaseOffsetParameter;
    AudioParameterFloat* mFeedbackParameter;
    AudioParameterInt* mTypeParameter;
    
    //lfo data
    float mLFOPhase;
    
    //feedback data
    float mFeedbackLeft;
    float mFeedbackRight;
    
    
    //circular buffer data
    float* mCircBuffLeft;
    float* mCircBuffRight;
    int mCircBuffWriteHead;
    int mCircBuffLength;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ChorusFlangeAudioProcessor)
};
