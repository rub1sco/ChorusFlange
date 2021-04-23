/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ChorusFlangeAudioProcessor::ChorusFlangeAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    //initilaze parameters
    addParameter(mDryWetParameter = new AudioParameterFloat("drywet","DryWet",0.0, 1, 0.5));
    addParameter(mDepthParameter = new AudioParameterFloat("depth", "Depth", 0.0,1.0,0.5));
    addParameter(mRateParameter = new AudioParameterFloat("rate", "Rate", 0.1f,20.f,10.f));
    addParameter(mPhaseOffsetParameter = new AudioParameterFloat("phaseoffset","PhaseOffset",0.0f,1.f,0.5f));
    addParameter(mFeedbackParameter = new AudioParameterFloat("feedback", "Feedback", 0,0.98,0.5));
    addParameter(mTypeParameter = new AudioParameterInt("type", "Type", 0,1,0));
    
    //initialize data default values
    mLFOPhase = 0;
    mCircBuffLeft = nullptr;
    mCircBuffRight = nullptr;
    mCircBuffWriteHead = 0;
    mCircBuffLength = 0;
    mFeedbackLeft = 0;
    mFeedbackRight = 0;
    
}

ChorusFlangeAudioProcessor::~ChorusFlangeAudioProcessor()
{
    
    //deletes resources
    if(mCircBuffLeft != nullptr){
        delete [] mCircBuffLeft;
        mCircBuffLeft = nullptr;
    }
    if(mCircBuffRight != nullptr){
        delete [] mCircBuffRight;
        mCircBuffRight = nullptr;
    }
    
}

//==============================================================================
const String ChorusFlangeAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool ChorusFlangeAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool ChorusFlangeAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool ChorusFlangeAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double ChorusFlangeAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int ChorusFlangeAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int ChorusFlangeAudioProcessor::getCurrentProgram()
{
    return 0;
}

void ChorusFlangeAudioProcessor::setCurrentProgram (int index)
{
}

const String ChorusFlangeAudioProcessor::getProgramName (int index)
{
    return {};
}

void ChorusFlangeAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void ChorusFlangeAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    //initialize LFOphase
    mLFOPhase = 0;
    
    //sets bufferlength
    mCircBuffLength = sampleRate * MAX_DELAYTIME;
    
    //iniliazes left buffer if nullptr
    if(mCircBuffLeft == nullptr){
        mCircBuffLeft = new float[mCircBuffLength];
    }
    
    //clears CirBuffLeft by BuffLength
    zeromem(mCircBuffLeft, mCircBuffLength*sizeof(float));

    //initializes CirBuffRight if nullptr
    if(mCircBuffRight == nullptr){
        mCircBuffRight = new float[mCircBuffLength];
    }
    
    //clears CircBuffRight
    zeromem(mCircBuffRight, mCircBuffLength*sizeof(float));
    
    //intializes writehead
    mCircBuffWriteHead = 0;
    
    
}

void ChorusFlangeAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool ChorusFlangeAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void ChorusFlangeAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    


    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

        float* leftChannel = buffer.getWritePointer(0);
        float* rightChannel = buffer.getWritePointer(1);
            
            // This is the place where you'd normally do the guts of your plugin's
            // audio processing...
            // Make sure to reset the state if your inner loop is processing
            // the samples and the outer loop is handling the channels.
            // Alternatively, you can process the samples with the channels
            // interleaved by keeping the same state.
            for(int i = 0; i < buffer.getNumSamples(); i++){
                
                //LFO Left function
                float lfoOutLeft = sin(2*M_PI *mLFOPhase);
                
                //calculates RightPhase
                float lfoPhaseRight = mLFOPhase + *mPhaseOffsetParameter;
                
                //error checking if greater than 1
                if (lfoPhaseRight > 1){
                    lfoPhaseRight -= 1;
                }
                
                //LFO Right function
                float lfoOutRight = sin(2* M_PI * lfoPhaseRight);
                
                //modifies left and right by DepthParameter
                lfoOutLeft *= *mDepthParameter;
                lfoOutRight *= *mDepthParameter;
                
                //initializes lfoMappedLeft and right
                float lfoOutMappedLeft = 0;
                float lfoOutMappedRight = 0;
                
                //if chorus type is selected
                if(*mTypeParameter == 0){
                    lfoOutMappedLeft = jmap(lfoOutMappedLeft, -1.f, 1.f, 0.005f, 0.03f);
                    lfoOutMappedRight = jmap(lfoOutMappedRight, -1.f, 1.f, 0.005f, 0.03f);
                }
                //else it is in flanger setting
                else{
                    lfoOutMappedLeft = jmap(lfoOutMappedLeft, -1.f, 1.f, 0.001f, 0.005f);
                    lfoOutMappedRight = jmap(lfoOutMappedRight, -1.f, 1.f, 0.001f, 0.005f);
                }
                
                //sets smoothness and gets the sample rate... smoothed makes sure no clicks are on output if jagged LFO used.
                float delayTimeSamplesLeft = getSampleRate() * lfoOutMappedLeft;
                float delayTimeSamplesRight = getSampleRate() * lfoOutMappedRight;
                
                //gets phase by multiplying sampleRate by mRateParameter
                mLFOPhase += *mRateParameter / getSampleRate();
                
                //if it's greater than 1, bring back in bounds
                if(mLFOPhase > 1){
                    mLFOPhase -= 1;
                }
                
                //writes the left and right channel and adds feedback at time i
                mCircBuffLeft[mCircBuffWriteHead] = leftChannel[i] + mFeedbackLeft;
                mCircBuffRight[mCircBuffWriteHead] = rightChannel[i] + mFeedbackRight;
                
                //calculates readhead from differences of writehead and delay time samples L
                float delayReadHeadLeft = mCircBuffWriteHead - delayTimeSamplesLeft;
                
                //checks if less than 0, if so add Buffer length
                if(delayReadHeadLeft < 0){
                    delayReadHeadLeft += mCircBuffLength;
                }
                
                //calculates Right readhead
                float delayReadHeadRight = mCircBuffWriteHead - delayTimeSamplesRight;
            
                //checks if less than 0, if so add bufferlength
                if (delayReadHeadRight < 0) {
                    delayReadHeadRight += mCircBuffLength;
                }
                
                
                //gets values for interpolation function
                int readHeadLeft_x = (int)delayReadHeadLeft;
                int readHeadLeft_x1 = readHeadLeft_x + 1;
                float readHeadFloatLeft = delayReadHeadLeft- readHeadLeft_x;
                
                
                //checks to make sure x1 is not greater than buffLength
                if(readHeadLeft_x1 >= mCircBuffLength){readHeadLeft_x1 -= mCircBuffLength;}
                
                //gets values for interpolation function
                int readHeadRight_x = (int)delayReadHeadRight;
                int readHeadRight_x1 = readHeadRight_x + 1;
                float readHeadFloatRight = delayReadHeadRight- readHeadRight_x;
                
                //checks to make sure x1 is not greater than buffLength
                if(readHeadLeft_x1 >= mCircBuffLength){readHeadLeft_x1 -= mCircBuffLength;}
                if(readHeadRight_x1 >= mCircBuffLength){readHeadRight_x1 -= mCircBuffLength;}
                
                //gets delay sample using linear interpolation... see wiki
                float delay_sample_left = lin_inter(mCircBuffLeft[readHeadLeft_x], mCircBuffLeft[readHeadLeft_x1], readHeadFloatLeft);
                float delay_sample_right =  lin_inter(mCircBuffRight[readHeadRight_x], mCircBuffRight[readHeadRight_x1], readHeadFloatRight);

                //combines feedback parameter with delay sample
                mFeedbackLeft = delay_sample_left * *mFeedbackParameter;
                mFeedbackRight = delay_sample_right * *mFeedbackParameter;
                
                //increments write head one
                mCircBuffWriteHead++;
                
                //checks if writehead has gone past buff length
                if(mCircBuffWriteHead >= mCircBuffLength){
                    mCircBuffWriteHead = 0;
                }
                
                //dry/wet var
                float dryAmount = 1 - *mDryWetParameter;
                float wetAmount = *mDryWetParameter;
                
                //combines the dry/wet signal with buffer signal at time i in left and right channel
                buffer.setSample(0, i,buffer.getSample(0, i) * dryAmount+ delay_sample_left * wetAmount);
                buffer.setSample(1, i, buffer.getSample(1, i) * dryAmount+ delay_sample_right * wetAmount);
                
            }
}

//==============================================================================
bool ChorusFlangeAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* ChorusFlangeAudioProcessor::createEditor()
{
    return new ChorusFlangeAudioProcessorEditor (*this);
}

//==============================================================================
void ChorusFlangeAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    //initialize xml as ChorusFlange
    std::unique_ptr<XmlElement> xml(new XmlElement("ChorusFlange"));
    
    //saves parameters as xml attributes
    xml -> setAttribute("DryWet", *mDryWetParameter);
    xml -> setAttribute("Depth", *mDepthParameter);
    xml -> setAttribute("Rate", *mRateParameter);
    xml -> setAttribute("PhaseOffset", *mPhaseOffsetParameter);
    xml -> setAttribute("Feedback", *mFeedbackParameter);
    xml -> setAttribute("Type", *mTypeParameter);
    
    //saves xml to binary
    copyXmlToBinary(*xml, destData);

}

void ChorusFlangeAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    //pointer for xml doc
    std::unique_ptr<XmlElement> xml(getXmlFromBinary(data, sizeInBytes));
    
    //if xml "ChorusFlange" is found, get attributes and save to current session.
    if(xml.get() != nullptr && xml ->hasTagName("ChorusFlange")){
        *mDryWetParameter = xml->getDoubleAttribute("DryWet");
        *mDepthParameter = xml-> getDoubleAttribute("Depth");
        *mRateParameter = xml -> getDoubleAttribute("Rate");
        *mPhaseOffsetParameter = xml -> getDoubleAttribute("PhaseOffset");
        *mFeedbackParameter = xml -> getDoubleAttribute("Feedback");
        *mTypeParameter = xml -> getIntAttribute("Type");
    }
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ChorusFlangeAudioProcessor();
}


//reference WIKI
float ChorusFlangeAudioProcessor::lin_inter(float sample_x, float sample_x1, float inPhase){
    return (1-inPhase) * sample_x + inPhase * sample_x1;
}
