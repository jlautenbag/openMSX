// $Id$

#include "WD2793BasedFDC.hh"
#include "WD2793.hh"
#include "DriveMultiplexer.hh"


WD2793BasedFDC::WD2793BasedFDC(MSXConfig::Device *config, const EmuTime &time)
	: MSXFDC(config, time), MSXDevice(config, time)
{
	multiplexer = new DriveMultiplexer(drives);
	controller = new WD2793(drives[0], time);
}

WD2793BasedFDC::~WD2793BasedFDC()
{
	delete controller;
	delete multiplexer;
}

void WD2793BasedFDC::reset(const EmuTime &time)
{
	
	controller->reset(time);
}
