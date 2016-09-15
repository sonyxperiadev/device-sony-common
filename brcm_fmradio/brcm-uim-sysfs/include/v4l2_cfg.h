/*
 *  User Mode Init manager - For shared transport
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program;if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

 *  Copyright (C) 2009-2015 Broadcom Corporation
 */


/************************************************************************************
*
*  Filename:      v4l2_cfg.h
*
*  Description:   Configure v4l2 related settings.
*
***********************************************************************************/

#define TRUE 1
#define FALSE 0

/* Set this to false for production release */
#ifndef V4L2_SNOOP_ENABLE
#define V4L2_SNOOP_ENABLE TRUE
#endif

/* Set this to true for debugging uim */
#ifndef UIM_DEBUG
#define UIM_DEBUG FALSE
#endif

/* Set this to false for production release */
#ifndef DBG_V4L2_DRIVERS
#define DBG_V4L2_DRIVERS TRUE
#endif


