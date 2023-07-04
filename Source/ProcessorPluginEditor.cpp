/*
------------------------------------------------------------------

This file is part of the Open Ephys GUI
Copyright (C) 2022 Open Ephys

------------------------------------------------------------------

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "ProcessorPluginEditor.h"
#include <stdio.h>

//std::thread stimThread;

ProcessorPluginEditor::ProcessorPluginEditor(GenericProcessor* parentNode) 
    : GenericEditor(parentNode)
{

    desiredWidth = 410;

    //==============================================================================

    // DEVICE SELECTOR
    vector <ofSerialDeviceInfo> devices = serial.getDeviceList();

    deviceSelector = std::make_unique<ComboBox>();
    deviceSelector->setBounds(5, 40, 110, 20);
    deviceSelector->addListener(this);
    deviceSelector->addItem("Device", 1);

    for (int i = 0; i < devices.size(); i++)
    {
        deviceSelector->addItem(devices[i].getDevicePath(), i + 2);
    }

    deviceSelector->setSelectedId(1, dontSendNotification);
    addAndMakeVisible(deviceSelector.get());

    // START AND STOP BUTTONS
    startButton = std::make_unique<UtilityButton>("START", titleFont);
    startButton->addListener(this);
    startButton->setBounds(5, 65, 50, 30);
    addAndMakeVisible(startButton.get());

    stopButton = std::make_unique<UtilityButton>("STOP", titleFont);
    stopButton->addListener(this);
    stopButton->setBounds(5, 95, 50, 30);
    addAndMakeVisible(stopButton.get());

    // Random checkbox
    addCheckBoxParameterEditor("random", 55, 65);

    //==============================================================================

    // TIME PARAMETERS
    addTextBoxParameterEditor("stim_time_s", 120, 20); // 125 20
    addTextBoxParameterEditor("rest_time_s", 120, 55); // 125 55
    addTextBoxParameterEditor("repetitions", 120, 90); // 125 90

    //==============================================================================

    // SOUND PARAMETERS

    // WAVE SELECTOR
    waveSelector = std::make_unique<ComboBox>();
    waveSelector->setBounds(220, 35, 80, 20); // 230 20
    waveSelector->addListener(this);
    waveSelector->addItem("Wave_form", 1);
    waveSelector->addItem("sine", 2);
    waveSelector->addItem("tri", 3);
    waveSelector->addItem("square", 4);
    waveSelector->addItem("saw", 5);
    waveSelector->setSelectedId(1, dontSendNotification);
    addAndMakeVisible(waveSelector.get());

    addTextBoxParameterEditor("pitch_Hz", 220, 55);
    addTextBoxParameterEditor("volume", 220, 90);

    //==============================================================================

    // LIGHT PARAMETERS
    addTextBoxParameterEditor("stim_freq_Hz", 310, 20);
    addTextBoxParameterEditor("duty_cycle", 310, 55);
    addTextBoxParameterEditor("light", 310, 90);
}

void ProcessorPluginEditor::comboBoxChanged(ComboBox* comboBoxThatHasChanged)
{
    if (comboBoxThatHasChanged == waveSelector.get())
    {
        ProcessorPlugin* processor = (ProcessorPlugin*)getProcessor();
        processor->stimThread.wave_type = (char)waveSelector->getSelectedItemIndex() - 1;
    }
    if (comboBoxThatHasChanged == deviceSelector.get())
    {
        ProcessorPlugin* processor = (ProcessorPlugin*)getProcessor();
        processor->stimThread.device = deviceSelector->getText().toStdString();
    }
}

void ProcessorPluginEditor::updateDevice(String deviceName)
{
    for (int i = 0; i < deviceSelector->getNumItems(); i++)
    {
        if (deviceSelector->getItemText(i).equalsIgnoreCase(deviceName))
            deviceSelector->setSelectedId(deviceSelector->getItemId(i), dontSendNotification);
    }
}

void ProcessorPluginEditor::buttonClicked(Button* button)
{
    ProcessorPlugin* processor = (ProcessorPlugin*)getProcessor();
    if (button == startButton.get())
    {
        processor->stimThread.startThread();
        CoreServices::updateSignalChain(this);
    }
    else if (button == stopButton.get()) {
        processor->stimThread.stopThread(100);
        processor->stimThread.stopStimulationCycle(deviceSelector->getText().toStdString(), (char)waveSelector->getSelectedItemIndex() - 1);
        CoreServices::updateSignalChain(this);
    }

}
