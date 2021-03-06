/*
 * IDT CPS RapidIO switches support
 *
 * Copyright 2009 Integrated Device Technology, Inc.
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 */

#include <linux/rio.h>
#include <linux/rio_drv.h>
#include <linux/rio_ids.h>
#include "../rio.h"

#define CPS_NO_ROUTE 0xdf

static int
idtcps_route_add_entry(struct rio_mport *mport, u16 destid, u8 hopcount,
		       u16 table, u16 route_destid, u8 route_port)
{
	u32 result;

	if (table == RIO_GLOBAL_TABLE) {
		rio_mport_write_config_32(mport, destid, hopcount,
				RIO_STD_RTE_CONF_DESTID_SEL_CSR, route_destid);

		rio_mport_read_config_32(mport, destid, hopcount,
				RIO_STD_RTE_CONF_PORT_SEL_CSR, &result);

		result = (0xffffff00 & result) | (u32)route_port;
		rio_mport_write_config_32(mport, destid, hopcount,
				RIO_STD_RTE_CONF_PORT_SEL_CSR, result);
	}

	return 0;
}

static int
idtcps_route_get_entry(struct rio_mport *mport, u16 destid, u8 hopcount,
		       u16 table, u16 route_destid, u8 *route_port)
{
	u32 result;

	if (table == RIO_GLOBAL_TABLE) {
		rio_mport_write_config_32(mport, destid, hopcount,
				RIO_STD_RTE_CONF_DESTID_SEL_CSR, route_destid);

		rio_mport_read_config_32(mport, destid, hopcount,
				RIO_STD_RTE_CONF_PORT_SEL_CSR, &result);

		if (CPS_NO_ROUTE == (u8)result)
			result = RIO_INVALID_ROUTE;

		*route_port = (u8)result;
	}

	return 0;
}

static int
idtcps_route_clr_table(struct rio_mport *mport, u16 destid, u8 hopcount,
		       u16 table)
{
	u32 i;

	if (table == RIO_GLOBAL_TABLE) {
		for (i = 0x80000000; i <= 0x800000ff;) {
			rio_mport_write_config_32(mport, destid, hopcount,
				RIO_STD_RTE_CONF_DESTID_SEL_CSR, i);
			rio_mport_write_config_32(mport, destid, hopcount,
				RIO_STD_RTE_CONF_PORT_SEL_CSR,
				(RIO_INVALID_ROUTE << 24) |
				(RIO_INVALID_ROUTE << 16) |
				(RIO_INVALID_ROUTE << 8) | RIO_INVALID_ROUTE);
			i += 4;
		}
	}

	return 0;
}

DECLARE_RIO_ROUTE_OPS(RIO_VID_IDT, RIO_DID_IDTCPS6Q, idtcps_route_add_entry, idtcps_route_get_entry, idtcps_route_clr_table);
DECLARE_RIO_ROUTE_OPS(RIO_VID_IDT, RIO_DID_IDTCPS8, idtcps_route_add_entry, idtcps_route_get_entry, idtcps_route_clr_table);
DECLARE_RIO_ROUTE_OPS(RIO_VID_IDT, RIO_DID_IDTCPS10Q, idtcps_route_add_entry, idtcps_route_get_entry, idtcps_route_clr_table);
DECLARE_RIO_ROUTE_OPS(RIO_VID_IDT, RIO_DID_IDTCPS12, idtcps_route_add_entry, idtcps_route_get_entry, idtcps_route_clr_table);
DECLARE_RIO_ROUTE_OPS(RIO_VID_IDT, RIO_DID_IDTCPS16, idtcps_route_add_entry, idtcps_route_get_entry, idtcps_route_clr_table);
DECLARE_RIO_ROUTE_OPS(RIO_VID_IDT, RIO_DID_IDT70K200, idtcps_route_add_entry, idtcps_route_get_entry, idtcps_route_clr_table);
