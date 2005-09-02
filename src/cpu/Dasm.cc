// $Id$

#include "Dasm.hh"
#include "DasmTables.hh"
#include "MSXCPUInterface.hh"
#include "StringOp.hh"
#include <sstream>

namespace openmsx {

static char sign(unsigned char a)
{
	return (a & 128) ? '-' : '+';
}

static int abs(unsigned char a)
{
	return (a & 128) ? (256 - a) : a;
}

int dasm(const MSXCPUInterface& interf, word pc, byte buf[4], std::string& dest)
{
	const char* s;
	std::ostringstream tmp;
	int i = 0;
	const char* r = 0;

	buf[0] = interf.peekMem(pc);
	switch (buf[0]) {
		case 0xCB:
			buf[1] = interf.peekMem(pc + 1);
			s = mnemonic_cb[buf[1]];
			i = 2;
			break;
		case 0xED:
			buf[1] = interf.peekMem(pc + 1);
			s = mnemonic_ed[buf[1]];
			i = 2;
			break;
		case 0xDD:
			r = "ix";
			buf[1] = interf.peekMem(pc + 1);
			if (buf[1] != 0xcb) {
				s = mnemonic_xx[buf[1]];
				i = 2;
			} else {
				buf[2] = interf.peekMem(pc + 2);
				buf[3] = interf.peekMem(pc + 3);
				s = mnemonic_xx_cb[buf[3]];
				i = 4;
			}
			break;
		case 0xFD:
			r = "iy";
			buf[1] = interf.peekMem(pc + 1);
			if (buf[1] != 0xcb) {
				s = mnemonic_xx[buf[1]];
				i = 2;
			} else {
				buf[2] = interf.peekMem(pc + 2);
				buf[3] = interf.peekMem(pc + 3);
				s = mnemonic_xx_cb[buf[3]];
				i = 4;
			}
			break;
		default:
			s = mnemonic_main[buf[0]];
			i = 1;
	}

	for (int j = 0; s[j]; ++j) {
		switch (s[j]) {
		case 'B':
			buf[i] = interf.peekMem(pc + i);
			tmp << "#" << StringOp::toHexString((unsigned short) buf[i], 2); 
			i += 1;
			dest += tmp.str();
			break;
		case 'R':
			buf[i] = interf.peekMem(pc + i);
			tmp << "#" << StringOp::toHexString((pc + 2 + (signed char)buf[i]) & 0xFFFF, 4); 
			i += 1;
			dest += tmp.str();
			break;
		case 'W':
			buf[i + 0] = interf.peekMem(pc + i + 0);
			buf[i + 1] = interf.peekMem(pc + i + 1);
			tmp << "#" << StringOp::toHexString( buf[i] + buf[i + 1] * 256, 4); 
			i += 2;
			dest += tmp.str();
			break;
		case 'X':
			buf[i] = interf.peekMem(pc + i);
			tmp << r << sign(buf[i]) << "#" << StringOp::toHexString(abs(buf[i]), 2); 
			i += 1;
			dest += tmp.str();
			break;
		case 'Y':
			tmp << r << sign(buf[2]) << "#" << StringOp::toHexString(abs(buf[2]), 2); 
			dest += tmp.str();
			break;
		case 'I':
			dest += r;
			break;
		case '!':
			tmp << "db     #ED,#" << StringOp::toHexString(buf[1], 2); 
			dest = tmp.str();
			return 2;
		case '@':
			tmp << "db     #" << StringOp::toHexString(buf[0], 2); 
			dest = tmp.str();
			return 1;
		case '#':
			tmp << "db     #" << StringOp::toHexString(buf[0], 2) << ",#CB,#" << StringOp::toHexString(buf[2], 2);
			dest = tmp.str();
			return 2;
		case ' ': {
			dest.resize(7, ' ');
			break;
		}
		default:
			dest += s[j];
			break;
		}
	}
	dest.resize(18, ' ');
	return i;
}

} // namespace openmsx
