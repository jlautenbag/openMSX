// $Id$

#include "MidiInConnector.hh"
#include "MidiInDevice.hh"
#include "DummyMidiInDevice.hh"
#include "PluggingController.hh"


MidiInConnector::MidiInConnector(const string& name_, const EmuTime& time)
	: name(name_)
{
	dummy = new DummyMidiInDevice();
	PluggingController::instance()->registerConnector(this);

	unplug(time);
}

MidiInConnector::~MidiInConnector()
{
	PluggingController::instance()->unregisterConnector(this);
	delete dummy;
}

const string& MidiInConnector::getName() const
{
	return name;
}

const string& MidiInConnector::getClass() const
{
	static const string className("midi in");
	return className;
}

void MidiInConnector::plug(Pluggable* device, const EmuTime &time)
{
	Connector::plug(device, time);
}

void MidiInConnector::unplug(const EmuTime& time)
{
	Connector::unplug(time);
	plug(dummy, time);
}

