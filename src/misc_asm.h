/**
 * vim: set ts=4 :
 * =============================================================================
 * Misc ASM
 * Copyright (C) 2012 Michael "ProdigySim" Busby
 * =============================================================================
 *
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License, version 3.0, as published by the
 * Free Software Foundation.
 * 
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * As a special exception, the author(s) give you permission to link the
 * code of this program (as well as its derivative works) to "Half-Life 2," the
 * "Source Engine," the "SourcePawn JIT," and any Game MODs that run on software
 * by the Valve Corporation.  You must obey the GNU General Public License in
 * all respects for all other code used.  Additionally, the author(s) grant
 * this exception to all derivative works.  The author(s) define further
 * exceptions, found in LICENSE.txt (as of this writing, version JULY-31-2007),
 * or <http://www.sourcemod.net/license.php>.
 *
 * Version: $Id$
 */
#ifndef __MISC_ASM_H__
#define __MISC_ASM_H__

// tier0 basetypes.h
#include <basetypes.h>

// 6 is as big as I recognize...
#define MAX_MOV_INSTR_LEN 6
#define JMP_8_INSTR_LEN 2
#define MOV_R32_R32_INSTR_LEN 2

#define JA_8_OPCODE 0x77
#define JB_8_OPCODE 0x72
#define JBE_8_OPCODE 0x76
#define JMP_8_OPCODE 0xEB

#define MOV_R32_RM32_OPCODE 0x8B
#define MOV_R32_IMM32_OPCODE 0xB8
#define MODRM_REG_EAX_EDX 0xC2

#define MODRM_SRC_TO_DISP32(modrm) (( modrm & 0x38) | 0x05 )

const BYTE NOP_1[] = { 0x90 };
const BYTE NOP_2[] = { 0x66,0x90 };
const BYTE NOP_3[] = { 0x0f,0x1f,0x00 };
const BYTE NOP_4[] = { 0x0f,0x1f,0x40,0x00 };
const BYTE NOP_5[] = { 0x0f,0x1f,0x44,0x00,0x00 };
const BYTE NOP_6[] = { 0x66,0x0f,0x1f,0x44,0x00,0x00 };
const BYTE NOP_7[] = { 0x0f,0x1f,0x80,0x00,0x00,0x00,0x00 };
const BYTE NOP_8[] = { 0x0f,0x1f,0x84,0x00,0x00,0x00,0x00,0x00 };
const BYTE NOP_9[] = { 0x66,0x0f,0x1f,0x84,0x00,0x00,0x00,0x00,0x00 };


// Convert mov instruction of any type to mov from immediate address (disp32)
// @param instr: pointer to first byte of mov instruction
// @return true if instruction is now disp32 source
inline bool mov_to_disp32(BYTE* instr)
{
	switch (instr[0])
	{
	case 0x8B: // standard mov with modrm
		instr[1] = MODRM_SRC_TO_DISP32(instr[1]);
		return true;
	case 0xA1: // direct to eax mov
		return true;
	default: // unsupported or not mov
		return false;
	}
}

// offset of src operand in mov instruction
inline int mov_src_operand_offset(BYTE* instr)
{
	switch (instr[0])
	{
	case 0x8B:
		return 2;
	case 0xA1:
		return 1;
	default:
		return 0;
	}
}

#endif // __MISC_ASM_H__
