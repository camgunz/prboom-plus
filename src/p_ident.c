/* Emacs style mode select   -*- C++ -*-
 *-----------------------------------------------------------------------------
 *
 *
 *  PrBoom: a Doom port merged with LxDoom and LSDLDoom
 *  based on BOOM, a modified and improved DOOM engine
 *  Copyright (C) 1999 by
 *  id Software, Chi Hoang, Lee Killough, Jim Flynn, Rand Phares, Ty Halderman
 *  Copyright (C) 1999-2000 by
 *  Jess Haas, Nicolas Kalkhof, Colin Phipps, Florian Schulze
 *  Copyright 2005, 2006 by
 *  Florian Schulze, Colin Phipps, Neil Stevens, Andrey Budko
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation; either version 2
 *  of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
 *  02111-1307, USA.
 *
 *-----------------------------------------------------------------------------*/

#include "z_zone.h"
#include "doomtype.h"

#include <stdint.h>

#include "khash.h"

#include "doomstat.h"
#include "d_ticcmd.h"
#include "lprintf.h"
#include "p_ident.h"
#include "p_mobj.h"

KHASH_MAP_INIT_INT(ident, mobj_t*)

khash_t(ident) *id_hash = NULL;
static unsigned int max_id = 0;

void P_IdentInit(void) {
  id_hash = kh_init(ident);
}

void P_IdentGetID(void *obj, unsigned int *obj_id) {
  khiter_t iter;
  unsigned int id;
  int rv = 0;

  max_id++;

  if (max_id == 0) {
    I_Error("P_IdentGetID: ID number rolled over");
  }

  id = max_id;

  iter = kh_put(ident, id_hash, id, &rv);

  if (rv == 0) {
    I_Error("P_IdentGetID: ID %u already assigned", id);
  }

  kh_value(id_hash, iter) = obj;

  *obj_id = id;
}

void P_IdentAssignID(void *obj, unsigned int obj_id) {
  khiter_t iter;
  int rv = 0;

  if (obj_id == 0) {
    I_Error("P_IdentAssignID: ID is 0");
  }

  max_id = MAX(max_id, obj_id);

  iter = kh_put(ident, id_hash, obj_id, &rv);

  if (rv == 0) {
    I_Error("P_IdentAssignID: ID %u already assigned", obj_id);
  }

  kh_value(id_hash, iter) = obj;
}

void P_IdentReleaseID(unsigned int *obj_id) {
  khiter_t iter;
  unsigned int id = *obj_id;

  if (id == 0) {
    I_Error("P_IdentReleaseID: ID %d not assigned", id);
  }

  iter = kh_get(ident, id_hash, id);

  if (iter != kh_end(id_hash)) {
    kh_del(ident, id_hash, iter);
  }

  *obj_id = 0;
}

void* P_IdentLookup(unsigned int id) {
  khiter_t iter = kh_get(ident, id_hash, id);

  if (iter == kh_end(id_hash)) {
    return NULL;
  }

  return kh_value(id_hash, iter);
}

void P_IdentReset(void) {
  kh_destroy(ident, id_hash);
  id_hash = kh_init(ident);
  max_id = 0;
}

unsigned int P_IdentGetMaxID(void) {
  return max_id;
}

void P_IdentSetMaxID(unsigned int new_max_id) {
  max_id = new_max_id;
}

/* vi: set et ts=2 sw=2: */

