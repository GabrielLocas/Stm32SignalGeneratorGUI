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

#ifndef PROCESSORPLUGINEDITOR_H_DEFINED
#define PROCESSORPLUGINEDITOR_H_DEFINED

#include <EditorHeaders.h>
#include <SerialLib.h>
#include "ProcessorPlugin.h"

class ProcessorPluginEditor : public GenericEditor,
							  public ComboBox::Listener,
							  public Button::Listener
{
public:

	/** Constructor */
	ProcessorPluginEditor(GenericProcessor* parentNode);

	/** Destructor */
	~ProcessorPluginEditor() { }

	/** Called when selected device is changed.*/
	void comboBoxChanged(ComboBox* comboBoxThatHasChanged);

	/** Gets the latest device from the processor*/
	void updateDevice(String deviceName);

	/** Called when configure button is clicked */
	void buttonClicked(Button* button) override;

private:

	ofSerial serial;

	std::unique_ptr<ComboBox> deviceSelector;

	std::unique_ptr<ComboBox> waveSelector;

	std::unique_ptr<UtilityButton> startButton;

	std::unique_ptr<UtilityButton> stopButton;

	/** Generates an assertion if this class leaks */
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ProcessorPluginEditor);

};

#endif // PROCESSORPLUGINEDITOR_H_DEFINED