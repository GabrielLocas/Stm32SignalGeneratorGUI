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

#include "ProcessorPlugin.h"

#include "ProcessorPluginEditor.h"

#include <stdio.h>

bool signalActive = false;

ProcessorPlugin::ProcessorPlugin()
    : GenericProcessor("stm32 - version 2.1")
{

    addIntParameter(Parameter::GLOBAL_SCOPE, "stim freq (Hz)", "The frequency of pulse light and sound stimulation", 20, 1, 255);
    addIntParameter(Parameter::GLOBAL_SCOPE, "pitch (Hz)", "The pitch in Hz of sound ", 10000, 1, 65535);
    addIntParameter(Parameter::GLOBAL_SCOPE, "duty cycle", "Duty cycle of stimulation", 50, 0, 100);
    addIntParameter(Parameter::GLOBAL_SCOPE, "stim time (s)", "The time that stimulation is on (seconds)", 5, 1, 3600); //Secondes ou minutes???
    addIntParameter(Parameter::GLOBAL_SCOPE, "rest time (s)", "The time that stimulation is off", 3, 0, 3600);
    addIntParameter(Parameter::GLOBAL_SCOPE, "repetitions", "The number of repetitions of Stim and Rest time", 3, 1, 100);
    addBooleanParameter(Parameter::GLOBAL_SCOPE, "random", "Activation of random duty cycle", 0);
}


ProcessorPlugin::~ProcessorPlugin()
{
    disconnect();
}


AudioProcessorEditor* ProcessorPlugin::createEditor()
{
    editor = std::make_unique<ProcessorPluginEditor>(this);
    return editor.get();
}


void ProcessorPlugin::updateSettings()
{


}


void ProcessorPlugin::process(AudioBuffer<float>& buffer)
{

    checkForEvents(true);

}


void ProcessorPlugin::handleTTLEvent(TTLEventPtr event)
{

}


void ProcessorPlugin::handleSpike(SpikePtr spike)
{

}


void ProcessorPlugin::handleBroadcastMessage(String message)
{

}


void ProcessorPlugin::saveCustomParametersToXml(XmlElement* parentElement)
{

}


void ProcessorPlugin::loadCustomParametersFromXml(XmlElement* parentElement)
{

}

bool ProcessorPlugin::startStimulationCycle(string device, char wave_type) {

    Time timer;
    uint32 currentTime;
    //Iterate for the number of desired cycles
    LOGC("The stimulation starts now...");
    signalActive = true;
    for (int i = 0; i < (int)(getParameter("repetitions")->getValue()); i++) {
        if(signalActive){
            //Activate stimulation
            LOGC("Activating signal");
            sendStartSignal(device, wave_type);
            currentTime = timer.getMillisecondCounter();
            timer.waitForMillisecondCounter(currentTime + 1000 * (int)(getParameter("stim time (s)")->getValue()));

            //Deactivate stimulation
            LOGC("Deactivating signal");
            sendStopSignal(device, wave_type);
            currentTime = timer.getMillisecondCounter();
            timer.waitForMillisecondCounter(currentTime + 1000 * (int)(getParameter("rest time (s)")->getValue()));
        }
        else {
            break;
        }
    }
    signalActive = false;
    LOGC("... the simulation has stopped");
    return true;
}

bool ProcessorPlugin::stopStimulationCycle(string device, char wave_type) {
    signalActive = false;
    sendStopSignal(device, wave_type);
    return true;
}

bool ProcessorPlugin::sendStartSignal(string device, char wave_type, int baud) {
    //Create connection
    _port.enumerateDevices();
    _port.setup(device.c_str(), baud);

    unsigned char sendArray[PACKET_SIZE];

    //wave type
    sendArray[0] = wave_type;

    //pitch frequency
    sendArray[2] = ((int)getParameter("pitch (Hz)")->getValue() & 0x000000ff);
    sendArray[1] = ((int)getParameter("pitch (Hz)")->getValue() & 0x0000ff00) >> 8;

    //stimulation frequency
    sendArray[3] = (int)getParameter("stim freq (Hz)")->getValue();

    //duty cycle
    if ((int)(getParameter("duty cycle")->getValue()) == 100) {
        sendArray[4] = 0xFF;
    }
    else {
        sendArray[4] = (int)((float)(getParameter("duty cycle")->getValue()) * 2.55);
    }

    sendArray[5] = (int)(getParameter("random")->getValue());

    //Send packet
    _port.writeBytes(sendArray, PACKET_SIZE);

    _port.close();

    return true;
}

bool ProcessorPlugin::sendStopSignal(string device, char wave_type, int baud) {
    //Create connection
    _port.enumerateDevices();
    _port.setup(device.c_str(), baud);

    unsigned char sendArray[PACKET_SIZE];

    //Arbitrary values except the duty cycle of zero to cancel all stimulation
    sendArray[0] = 0x01;
    sendArray[2] = 0x00;
    sendArray[1] = 0x10;
    sendArray[3] = 0x10;
    sendArray[4] = 0x00; //This one
    sendArray[5] = 0x00;

    //Send packet
    _port.writeBytes(sendArray, PACKET_SIZE);

    _port.close();

    return true;
}

void ProcessorPlugin::disconnect() {
    _port.close();
}