/**
 * =============================================================================
 * TF2 Items Extension
 * Copyright (C) 2009-2010 AzuiSleet, Asher Baker (asherkin).  All rights reserved.
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
 */

#ifndef _INCLUDE_SOURCEMOD_EXTENSION_PROPER_H_
#define _INCLUDE_SOURCEMOD_EXTENSION_PROPER_H_

/**
 * @file extension.hpp
 * @brief TF2Items extension code header.
 */

#include <cstddef>

#include "smsdk_ext.hpp"

//#include "iplayerinfo.h"
//#include "convar.h"

#define OVERRIDE_CLASSNAME		(1 << 0)
#define OVERRIDE_ITEM_DEF		(1 << 1)
#define OVERRIDE_ITEM_LEVEL		(1 << 2)
#define OVERRIDE_ITEM_QUALITY	(1 << 3)
#define OVERRIDE_ATTRIBUTES		(1 << 4)

#define PRESERVE_ATTRIBUTES		(1 << 5)
#define FORCE_GENERATION		(1 << 6)

class CBasePlayer;
class CEconItem;
class ITexture;
class ITextureCompositor;

template< class T, class I = int >
class CUtlMemoryTF2Items : public CUtlMemory< T, I >
{
public:
	CUtlMemoryTF2Items( int nGrowSize = 0, int nInitSize = 0 ) { CUtlMemory< T, I >( nGrowSize, nInitSize ); }
    CUtlMemoryTF2Items( T* pMemory, int numElements ) { CUtlMemory< T, I >( pMemory, numElements ); }
    CUtlMemoryTF2Items( const T* pMemory, int numElements ) { CUtlMemory< T, I >( pMemory, numElements ); }
    //~CUtlMemoryTF2Items() { ~CUtlMemory< T, I >(); }
    
	void Purge()
	{
		if ( !CUtlMemory< T, I >::IsExternallyAllocated() )
		{
			if (CUtlMemory< T, I >::m_pMemory)
			{
				UTLMEMORY_TRACK_FREE();
				//free( (void*)m_pMemory );
#ifdef TF2ITEMS_DEBUG_ITEMS
				META_CONPRINTF("CUtlMemory tried to be freed!\n");
#endif
				CUtlMemory< T, I >::m_pMemory = 0;
			}
			CUtlMemory< T, I >::m_nAllocationCount = 0;
		}
	}
};

class CEconItemAttribute
{
public:
	void *m_pVTable;

	uint16 m_iAttributeDefinitionIndex;
	float m_flValue;
	int32 m_nRefundableCurrency;
};

// TF2 uses 4-byte packing for the 32-bit item types.
#ifndef PLATFORM_64BITS
#pragma pack(push, 4)
#endif

class CEconItemHandle
{
public:
	void *m_pVTable;

	CEconItem *m_pItem;

	int64 m_ulItemID;
	uint64 m_SteamID;
};

class CAttributeList
{
public:
	void *m_pVTable;

	CUtlVector<CEconItemAttribute, CUtlMemoryTF2Items<CEconItemAttribute> > m_Attributes;
	void *m_pAttributeManager;


public:
	CAttributeList& operator=( const CAttributeList &other )
	{
		m_pVTable = other.m_pVTable;

		m_Attributes = other.m_Attributes;
		m_pAttributeManager = other.m_pAttributeManager;


		return *this;
	}
};

class CEconItemView
{
public:
	void *m_pVTable;

	uint16 m_iItemDefinitionIndex;
	
	int32 m_iEntityQuality;
	uint32 m_iEntityLevel;

	uint64 m_iItemID;
	uint32 m_iItemIDHigh;
	uint32 m_iItemIDLow;

	uint32 m_iAccountID;

	uint32 m_iInventoryPosition;
	
	CEconItemHandle m_ItemHandle;

	bool m_bColorInit;
	bool m_bPaintOverrideInit;
	bool m_bHasPaintOverride;


	float m_flOverrideIndex;
	uint32 m_unRGB;
	uint32 m_unAltRGB;

	int32 m_iTeamNumber;

	bool m_bInitialized;

	CAttributeList m_AttributeList;
	CAttributeList m_NetworkedDynamicAttributesForDemos;

	bool m_bDoNotIterateStaticAttributes;
};

#ifndef PLATFORM_64BITS
#pragma pack(pop)
#endif

// Match the dedicated server's item layout on each target.
constexpr bool kTF2Items64Bit = sizeof(void *) == 8;

static_assert(sizeof(CEconItemAttribute) == (kTF2Items64Bit ? 24 : 16));
static_assert(offsetof(CEconItemAttribute, m_iAttributeDefinitionIndex) == (kTF2Items64Bit ? 8 : 4));
static_assert(offsetof(CEconItemAttribute, m_flValue) == (kTF2Items64Bit ? 12 : 8));
static_assert(offsetof(CEconItemAttribute, m_nRefundableCurrency) == (kTF2Items64Bit ? 16 : 12));

static_assert(sizeof(CEconItemHandle) == (kTF2Items64Bit ? 32 : 24));
static_assert(offsetof(CEconItemHandle, m_pItem) == (kTF2Items64Bit ? 8 : 4));
static_assert(offsetof(CEconItemHandle, m_ulItemID) == (kTF2Items64Bit ? 16 : 8));
static_assert(offsetof(CEconItemHandle, m_SteamID) == (kTF2Items64Bit ? 24 : 16));

static_assert(sizeof(CAttributeList) == (kTF2Items64Bit ? 48 : 28));
static_assert(offsetof(CAttributeList, m_Attributes) == (kTF2Items64Bit ? 8 : 4));
static_assert(offsetof(CAttributeList, m_pAttributeManager) == (kTF2Items64Bit ? 40 : 24));

static_assert(sizeof(CEconItemView) == (kTF2Items64Bit ? 208 : 148));
static_assert(offsetof(CEconItemView, m_iItemDefinitionIndex) == (kTF2Items64Bit ? 8 : 4));
static_assert(offsetof(CEconItemView, m_iEntityQuality) == (kTF2Items64Bit ? 12 : 8));
static_assert(offsetof(CEconItemView, m_iEntityLevel) == (kTF2Items64Bit ? 16 : 12));
static_assert(offsetof(CEconItemView, m_iItemID) == (kTF2Items64Bit ? 24 : 16));
static_assert(offsetof(CEconItemView, m_ItemHandle) == (kTF2Items64Bit ? 48 : 40));
static_assert(offsetof(CEconItemView, m_bInitialized) == (kTF2Items64Bit ? 100 : 84));
static_assert(offsetof(CEconItemView, m_AttributeList) == (kTF2Items64Bit ? 104 : 88));
static_assert(offsetof(CEconItemView, m_NetworkedDynamicAttributesForDemos) == (kTF2Items64Bit ? 152 : 116));
static_assert(offsetof(CEconItemView, m_bDoNotIterateStaticAttributes) == (kTF2Items64Bit ? 200 : 144));

struct TScriptedItemOverride
{
	// Matches MAX_ATTRIBUTES_PER_ITEM in the TF2 SDK.
	static constexpr int MaxAttributes = 20;

	uint8 m_bFlags;									// Flags to what we should override.
	char m_strWeaponClassname[256];					// Classname to override the GiveNamedItem call with.
	uint32 m_iItemDefinitionIndex;					// New Item Def. Index.
	uint8 m_iEntityQuality;							// New Item Quality Level.
	uint8 m_iEntityLevel;							// New Item Level.
	uint8 m_iCount;									// Count of Attributes.
	CEconItemAttribute m_Attributes[MaxAttributes];			// The actual attributes.
};

class TScriptedItemOverrideTypeHandler : public IHandleTypeDispatch
{
public:
	void OnHandleDestroy(HandleType_t type, void *object);
};

/**
 * @brief Sample implementation of the SDK Extension.
 * Note: Uncomment one of the pre-defined virtual functions in order to use it.
 */
class TF2Items : public SDKExtension, public IConCommandBaseAccessor
{
public:
	/**
	 * @brief This is called after the initial loading sequence has been processed.
	 *
	 * @param error		Error message buffer.
	 * @param maxlength	Size of error message buffer.
	 * @param late		Whether or not the module was loaded after map load.
	 * @return			True to succeed loading, false to fail.
	 */
	virtual bool SDK_OnLoad(char *error, size_t maxlen, bool late);
	
	/**
	 * @brief This is called right before the extension is unloaded.
	 */
	virtual void SDK_OnUnload();

	/**
	 * @brief Called when the pause state is changed.
	 */
	//virtual void SDK_OnPauseChange(bool paused);

	/**
	 * @brief this is called when Core wants to know if your extension is working.
	 *
	 * @param error		Error message buffer.
	 * @param maxlength	Size of error message buffer.
	 * @return			True if working, false otherwise.
	 */
	//virtual bool QueryRunning(char *error, size_t maxlen);
public:
#if defined SMEXT_CONF_METAMOD
	/**
	 * @brief Called when Metamod is attached, before the extension version is called.
	 *
	 * @param error			Error buffer.
	 * @param maxlength		Maximum size of error buffer.
	 * @param late			Whether or not Metamod considers this a late load.
	 * @return				True to succeed, false to fail.
	 */
	virtual bool SDK_OnMetamodLoad(ISmmAPI *ismm, char *error, size_t maxlen, bool late);

	/**
	 * @brief Called when Metamod is detaching, after the extension version is called.
	 * NOTE: By default this is blocked unless sent from SourceMod.
	 *
	 * @param error			Error buffer.
	 * @param maxlength		Maximum size of error buffer.
	 * @return				True to succeed, false to fail.
	 */
	virtual bool SDK_OnMetamodUnload(char *error, size_t maxlen);

	/**
	 * @brief Called when Metamod's pause state is changing.
	 * NOTE: By default this is blocked unless sent from SourceMod.
	 *
	 * @param paused		Pause state being set.
	 * @param error			Error buffer.
	 * @param maxlength		Maximum size of error buffer.
	 * @return				True to succeed, false to fail.
	 */
	//virtual bool SDK_OnMetamodPauseChange(bool paused, char *error, size_t maxlen);
#endif
public: //IConCommandBaseAccessor
	bool RegisterConCommandBase(ConCommandBase *pCommand);
};

void CSCICopy(CEconItemView *olditem, CEconItemView *newitem);

static cell_t TF2Items_GiveNamedItem(IPluginContext *pContext, const cell_t *params);
static cell_t TF2Items_CreateItem(IPluginContext *pContext, const cell_t *params);

static cell_t TF2Items_SetFlags(IPluginContext *pContext, const cell_t *params);
static cell_t TF2Items_GetFlags(IPluginContext *pContext, const cell_t *params);
static cell_t TF2Items_SetClassname(IPluginContext *pContext, const cell_t *params);
static cell_t TF2Items_GetClassname(IPluginContext *pContext, const cell_t *params);
static cell_t TF2Items_SetItemIndex(IPluginContext *pContext, const cell_t *params);
static cell_t TF2Items_GetItemIndex(IPluginContext *pContext, const cell_t *params);
static cell_t TF2Items_SetQuality(IPluginContext *pContext, const cell_t *params);
static cell_t TF2Items_GetQuality(IPluginContext *pContext, const cell_t *params);
static cell_t TF2Items_SetLevel(IPluginContext *pContext, const cell_t *params);
static cell_t TF2Items_GetLevel(IPluginContext *pContext, const cell_t *params);
static cell_t TF2Items_SetNumAttributes(IPluginContext *pContext, const cell_t *params);
static cell_t TF2Items_GetNumAttributes(IPluginContext *pContext, const cell_t *params);
static cell_t TF2Items_SetAttribute(IPluginContext *pContext, const cell_t *params);
static cell_t TF2Items_GetAttributeId(IPluginContext *pContext, const cell_t *params);
static cell_t TF2Items_GetAttributeValue(IPluginContext *pContext, const cell_t *params);
static cell_t TF2Items_GetCurrentSlot(IPluginContext *pContext, const cell_t *params);

CBaseEntity * GetCBaseEntityFromIndex(int p_iEntity, bool p_bOnlyPlayers);
int GetIndexFromCBaseEntity(CBaseEntity * p_hEntity);
TScriptedItemOverride * GetScriptedItemOverrideFromHandle(cell_t cellHandle, IPluginContext *pContext=NULL);

extern HandleType_t g_ScriptedItemOverrideHandleType;
extern TScriptedItemOverrideTypeHandler g_ScriptedItemOverrideHandler;
extern sp_nativeinfo_t g_ExtensionNatives[];
extern IForward *g_pForwardGiveItem;
extern IForward *g_pForwardGiveItem_Post;

#endif // _INCLUDE_SOURCEMOD_EXTENSION_PROPER_H_
