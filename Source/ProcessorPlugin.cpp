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


ProcessorPlugin::ProcessorPlugin()
    : GenericProcessor("stm x fafa")
{
    //SerialPort port("COM8", 115200);

    addIntParameter(Parameter::GLOBAL_SCOPE, "stim freq (Hz)", "The frequency of pulse light and sound stimulation", 1, 1, 255);
    addIntParameter(Parameter::GLOBAL_SCOPE, "pitch (Hz)", "The pitch in Hz of sound ", 432, 1, 65535);
    addIntParameter(Parameter::GLOBAL_SCOPE, "duty cycle", "Duty cycle of stimulation", 50, 0, 100);
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


bool ProcessorPlugin::connect(string device, int baud) {
    _port.enumerateDevices();
    _port.setup(device.c_str(), baud);
    return true;
}

void ProcessorPlugin::disconnect() {
    _port.close();
}