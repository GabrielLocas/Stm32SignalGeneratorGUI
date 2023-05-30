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

ProcessorPluginEditor::ProcessorPluginEditor(GenericProcessor* parentNode) 
    : GenericEditor(parentNode)
{

    desiredWidth = 400;

    //DEVICE SELECTOR
    vector <ofSerialDeviceInfo> devices = serial.getDeviceList();

    deviceSelector = std::make_unique<ComboBox>();
    deviceSelector->setBounds(15, 50, 110, 20);
    deviceSelector->addListener(this);
    deviceSelector->addItem("Device", 1);

    for (int i = 0; i < devices.size(); i++)
    {
        deviceSelector->addItem(devices[i].getDevicePath(), i + 2);
    }

    deviceSelector->setSelectedId(1, dontSendNotification);
    addAndMakeVisible(deviceSelector.get());

    //WAVE SELECTOR
    waveSelector = std::make_unique<ComboBox>();
    waveSelector->setBounds(15, 90, 110, 20);
    waveSelector->addListener(this);
    waveSelector->addItem("Wave form", 1);
    waveSelector->addItem("sine", 2);
    waveSelector->addItem("tri", 3);
    waveSelector->addItem("square", 4);
    waveSelector->addItem("saw 3", 5);

    waveSelector->setSelectedId(1, dontSendNotification);
    addAndMakeVisible(waveSelector.get());

    //OTHER PARAMETERS
    addTextBoxParameterEditor("stim freq (Hz)", 144, 30);
    addTextBoxParameterEditor("pitch (Hz)", 144, 70);
    addSliderParameterEditor("duty cycle", 260, 40);

    //START AND STOP BUTTONS
    startButton = std::make_unique<UtilityButton>("START", titleFont);
    startButton->addListener(this);
    //startButton->setRadius(3.0f);
    startButton->setBounds(350, 40, 50, 30);
    addAndMakeVisible(startButton.get());

    stopButton = std::make_unique<UtilityButton>("STOP", titleFont);
    stopButton->addListener(this);
    //stopButton->setRadius(3.0f);
    stopButton->setBounds(350, 80, 50, 30);
    addAndMakeVisible(stopButton.get());

}

void ProcessorPluginEditor::comboBoxChanged(ComboBox* comboBoxThatHasChanged)
{
    if (comboBoxThatHasChanged == waveSelector.get())
    {
        
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

    if (button == startButton.get())
    {
        ProcessorPlugin* processor = (ProcessorPlugin*)getProcessor();
        processor->startSignal(deviceSelector->getText().toStdString(), (char)waveSelector->getSelectedItemIndex() - 1);
        CoreServices::updateSignalChain(this);
    }
    else if (button == stopButton.get()) {
        ProcessorPlugin* processor = (ProcessorPlugin*)getProcessor();
        processor->stopSignal(deviceSelector->getText().toStdString(), (char)waveSelector->getSelectedItemIndex() - 1);
        CoreServices::updateSignalChain(this);
    }

}
