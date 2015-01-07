//  Copyright (c) 2014 Jakub Filipowicz <jakubf@gmail.com>
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc.,
//  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

const char *_NA_ = "N/A";
#define _UNK_ "unknown"

const char *crk5_disk_type_names[] = {
	"MERA 9425 (connectet to mem channel)",
	"winchester disk",
	"floppy disk",
	"MERA 9425 or EC 5061 (connected to PLIX)"
};

const char *mx_flop5_type_names[] = {
	"SD",
	"DD",
	"HD"
};

const char *mx_disk_type_names[] = {
	"EC 5061",
	"MERA 9425 (removable)",
	"MERA 9425 (fixed)",
	"MERA 9425 (whole disk)"
};

const char *mx_pline_direction_names[] = {
	_UNK_,
	_UNK_,
	"input",
	_UNK_,
	"output",
	"half duplex",
	"full duplex"
};

const char *mx_pline_type_names[] = {
	"serial async",
	"parallel",
	"serial sync",
	"winchester disk",
	"magnetic tape",
	"floppy disk"
};

const char *mx_protocol_names[] = {
	"punched tape reader",
	"printer, tape puncher",
	"terminal",
	"SOM-3 punched tape reader",
	"SOM-3 printer, tape puncher",
	"SOM-3 terminal",
	"winchester",
	"magnetic tape",
	"floppy disk"
};

const char *px_device_type_names[] = {
	"EC 5061",
	"MERA 9425",
	"winchester",
	"floppy disk"
};


