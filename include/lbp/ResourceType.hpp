/*
	Copyright (C) 2026  Peyton Anderson

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

/**
 * @file ResourceType.hpp
 * @brief Defines ResourceType enum
 * @author Peyton Anderson (peyton@turnoffthetv.xyz)
 */

#ifndef RESOURCETYPE_HPP
#define RESOURCETYPE_HPP

#include <cstdint>

/**
 * @brief libobigplanet
 */
namespace LBP {

	/**
	 * @brief Resource type enum
	 */
	enum class ResourceType : uint32_t {
		/// @brief Invalid resource
		INVALID,
		/// @brief Texture
		TEX,
		/// @brief Mesh
		MOL,
		/// @brief Fragment shader
		FPO,
		/// @brief Vertex shader
		VPO,
		/// @brief Animation
		ANIM,
		/// @brief GUID substitution
		GSUB,
		/// @brief Graphics material
		GMAT,
		/// @brief SPU executable
		SBU,
		/// @brief Level
		LVL,
		/// @brief Media (FSB audio or BIK video)
		MEDIA,
		/// @brief FishFingers script
		FF,
		/// @brief Character settings
		CHA,
		/// @brief Raw bytes
		RAW,
		/// @brief Soft physics settings
		SPH,
		/// @brief Font
		FNT,
		/// @brief Physics material
		MAT,
		/// @brief DLC
		DLC,
		/// @brief Editor settings
		EDSET,
		/// @brief Joint settings
		JNT,
		/// @brief Game constants
		CON,
		/// @brief Popit settings
		POP,
		/// @brief Cached level data
		CLD,
		/// @brief Synced profile
		PRO,
		/// @brief Bevel
		BEV,
		/// @brief Game
		GAM,
		/// @brief Network settings
		NWS,
		/// @brief Pack
		PCK,
		/// @brief Big profile
		BPR,
		/// @brief Level slot list
		SLT,
		/// @brief Translation table
		TRANS,
		/// @brief Adventure create profile
		ADC,
		/// @brief Local profile
		IPR,
		/// @brief Limit settings
		LMT,
		/// @brief Tutorial
		TUT,
		/// @brief GUID list
		GLST,
		/// @brief Audio material
		AUM,
		/// @brief Fluid settings
		FLU,
		/// @brief Plan
		PLAN,
		/// @brief Texture list
		TXL,
		/// @brief Music settings
		MUS,
		/// @brief Mixer settings
		MIX,
		/// @brief Replay settings
		REP,
		/// @brief Palette
		PAL,
		/// @brief Static mesh
		SMH,
		/// @brief Animated texture
		ATX,
		/// @brief VOIP recording
		VOP,
		/// @brief Pin
		PIN,
		/// @brief Instrument
		INS,
		/// @brief Sample
		SMP,
		/// @brief Outfit list
		OFT,
		/// @brief Paintbrush
		PBR,
		/// @brief Recording
		REC,
		/// @brief Painting
		PTG,
		/// @brief Quest
		QST,
		/// @brief Animation bank
		ABK,
		/// @brief Animation set
		AST,
		/// @brief Skeleton map
		SMAP,
		/// @brief Skeleton registry
		SRG,
		/// @brief Skeleton animation style
		SAS,
		/// @brief Vita crossplay data
		CPV,
		/// @brief Streaming chunk
		CHK,
		/// @brief Adventure share data
		ADS,
		/// @brief Adventure play profile
		ADP,
		/// @brief Animation map
		AMP,
		/// @brief Cached costume data
		CCD,
		/// @brief Data labels
		DLA,
		/// @brief Adventure map
		ADM
	};

};

#endif