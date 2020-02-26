#pragma once
// 1_5_62

// Patches

// E8 ? ? ? ? 84 DB 74 24 -> +0x230
constexpr uintptr_t g_FrameTimer_SlowTime_offset = 0x02F6B948;
constexpr uintptr_t g_FrameTimer_NoSlowTime_offset = 0x02F6B94C;

// BSLightingAmbientSpecular
// 41 F7 85 ? ? ? ? ? ? ? ? 74 0A 
constexpr uintptr_t AddAmbientSpecularToSetupGeometry_offset = 0x012F375D;
// 49 8B 47 28 0F 10 05 ? ? ? ? 
constexpr uintptr_t g_AmbientSpecularAndFresnel_offset = 0x1E0DFCC;
constexpr uintptr_t DisableSetupMaterialAmbientSpecular_offset = 0x12F2733;

// BSLightingShader Alpha
// E8 ? ? ? ? 49 8B 96 ? ? ? ? 40 B6 01 
constexpr uintptr_t BSBatchRenderer_SetupAndDrawPass_offset = 0x01308440;
constexpr uintptr_t BSLightingShader_vtbl_offset = 0x01864DE8;

// Disable Chargen Precache
// E8 ? ? ? ? E8 ? ? ? ? 48 8D 57 30  ->
constexpr uintptr_t ChargenCacheFunction_offset = 0x008B2BE0;
// 90 E8 ? ? ? ? 90 48 8B 15 ? ? ? ?  ->
constexpr uintptr_t ChargenCacheClearFunction_offset = 0x008B2D60;

// Enable Achievements With Mods
// 48 83 EC 28 C6 44 24 ? ? 
constexpr uintptr_t AchievementModsEnabledFunction_offset = 0x0016F2E0;

// Form Caching
constexpr uintptr_t LookupFormByID_offset = 0x00194230;
constexpr uintptr_t GlobalFormTableLock_offset = 0x01EC4150;
constexpr uintptr_t GlobalFormTable_offset = 0x01EC3CB8;

// E8 ? ? ? ? 90 83 05 ? ? ? ? ? ->
constexpr uintptr_t UnkFormFunc1_offset = 0x001947F0;
// E8 ? ? ? ? 84 C0 75 36 48 8B CD ->
constexpr uintptr_t UnkFormFunc2_offset = 0x00195EF0;
// E8 ? ? ? ? 40 B5 01 49 8B CD ->
constexpr uintptr_t UnkFormFunc3_offset = 0x00195C20;
// E8 ? ? ? ? 48 8D 84 24 ? ? ? ? 48 89 44 24 ? 48 8D 44 24 ? 48 89 44 24 ? 4C 8D 4F 14 -> +0xC4 ->
constexpr uintptr_t UnkFormFunc4_offset = 0x001967E0;

// Regular Quicksaves
// QuickSaveLoadHandler::HandleEvent  = vtbl 5
constexpr uintptr_t QuickSaveLoadHandler_HandleEvent_SaveType_offset = 0x008AA9A8; // F0000200
constexpr uintptr_t QuickSaveLoadHandler_HandleEvent_LoadType_offset = 0x008AA9DB; // D0000400

// Save Added Sound Categories
constexpr uintptr_t vtbl_BGSSoundCategory_offset = 0x01576FD0;
constexpr uintptr_t vtbl_BGSSoundCategory_BSISoundCategory_SetVolume_offset = 0x01577200; // vtbl 3
constexpr uintptr_t vtbl_INIPrefSettingCollection_Unlock_offset = 0x01535AB8; // vtbl 6

// Scrolling Doesn't Switch POV
// FirstPersonState::PlayerInputHandler::sub_4
// 48 39 08 75 0B 
constexpr uintptr_t FirstPersonState_DontSwitchPOV_offset = 0x00847E23;
// TPS
// 74 35 48 8B 0D ? ? ? ? E8 ? ? ? ? 84 C0 
constexpr uintptr_t ThirdPersonState_DontSwitchPOV_offset = 0x0084FDC8;

// Sleep Wait Time
// "getSliderValue" comiss
constexpr uintptr_t SleepWaitTime_Compare_offset = 0x008BDA8E;

// Tree LOD Reference Caching
// E8 ? ? ? ? EB 0F 48 8B 43 18 ->
constexpr uintptr_t UpdateBlockVisibility_orig_offset = 0x004A8090;
// E8 ? ? ? ? 66 89 47 04 ->
constexpr uintptr_t Float2Half_offset = 0x00D426F0;


// Waterflow 
// E8 ? ? ? ? 84 DB 74 24 -> +0x252
constexpr uintptr_t GameLoop_Hook_offset = 0x005B3242;
constexpr uintptr_t UnkGameLoopDword_offset = 0x02F6B950;
// 5th function in ??_7BSWaterShader@@6B@ vtbl
// F3 0F 10 0D ? ? ? ? F3 0F 11 4C 82 ?
constexpr uintptr_t WaterShader_ReadTimer_Hook_offset = 0x012F6879;


// Fixes

// Archery Downward Aiming
			// E8 ? ? ? ? 8B 83 CC 01 00 00 C1 E8 12
constexpr uintptr_t CalculateCollisionCall_offset = 0x00744DD0;    // 1_5_97

// Double Perk Apply
// 48 85 D2 74 7C 48 89 5C 24 ? 
constexpr uintptr_t Unknown_Add_Func_offset = 0x005C67A0;
// E8 ? ? ? ? B2 01 48 8B 0D ? ? ? ? E8 ? ? ? ? 48 8B 0D ? ? ? ? -> +0xA
constexpr uintptr_t Handle_Add_Rf_offset = 0x00682020;
// 44 0F B6 47 ? 48 8B D3 E8 ? ? ? ? 
constexpr uintptr_t Switch_Function_movzx_offset = 0x005C8B2E;
// 41 0F B6 F9 48 8B C2 
constexpr uintptr_t Unknown_Add_Function_movzx_offset = 0x005C67BA;
// +0x2C
constexpr uintptr_t Unknown_Add_Function_movzx2_offset = 0x005C67E6;
// E8 ? ? ? ? 48 8D 35 ? ? ? ? 48 89 5C 24 ? -> +0x12 -> +0x1B
constexpr uintptr_t Next_Formid_Get_Hook_offset = 0x00681FEB;
// called just after switch function movzx above, +0x1B
constexpr uintptr_t Do_Handle_Hook_offset = 0x003386BB;
// 74 1B 0F B6 42 08 
constexpr uintptr_t Do_Add_Hook_offset = 0x00338A61;

// Equip Shout Event Spam
// E8 ? ? ? ? F6 86 ? ? ? ? ? 74 0F 
constexpr uintptr_t Equip_Shout_Procedure_Function_offset = 0x006323C0;

// GHeap Leak Detection Crash
// E8 ? ? ? ? 48 8B 07 33 D2 48 8B CF FF 10

// DIFF:  1.00    0.99    -------    0000000140FFFEB0    sub_0000000140FFFEB0    000000014105CBB0    sub_000000014105CBB0
constexpr uintptr_t GHeap_Leak_Detection_Crash_offset = 0x105CBB0; // VR //0x00FFFEB0;

// Lip Sync Fix
// E8 ? ? ? ? 48 8D 8F 20 01 00 00 0F 28 CE
constexpr uintptr_t LipSync_FUNC_ADDR = 0x001F12A0;    // 1_5_80

// VR addresses for BGSShaderparticleGeometryData fix and snow shader fix
// Memory Access Errors
constexpr uintptr_t BSLightingShaderMaterialSnow_vtbl_offset = 0x18FEA78; //0x0185E0F0;
// 49 8B 4F 28 F3 0F 10 93 ? ? ? ? F3 0F 10 8B ? ? ? ? F3 0F 10 83 ? ? ? ? 

// DIFF: 0.97    0.99    GI---- - 00000001412F2020    BSLightingShader_vf_sub_1412F2020    0000000141338400    BSLightingShader_vf_sub_141338400    

// These are fixed for VR
constexpr uintptr_t BSLightingShader_SetupMaterial_Snow_Hook_offset = 0x013388E0; // VR //0x012F2500;
constexpr uintptr_t BSLightingShader_SetupMaterial_Snow_Exit_offset = 0x013389B6; // VR //0x012F25D6;

constexpr uintptr_t vtbl_BGSShaderParticleGeometryData_LoadForm_offset = 0x15D7BD8; // VR //0x0155FFA0; // vtbl[6]


// 48 8B C4 48 89 50 10 55  53 56 57 41 54 41 55 41  56 41 57 48 8D A8 38 FB
// Updated for VR
constexpr uintptr_t BadUseFuncBase_offset = 0x13574F0; //0x013251C0;

// MO5S Typo
// 3D ? ? ? ? 74 18 8B C8 
constexpr uintptr_t MO5STypo_offset = 0x0019B313;

// PerkFragmentIsRunning
// 48 89 5C 24 08 57 48 83  EC 20 33 FF 49 8B D9 49  89 39 48 85 C9 ?? ?? 80  79 1A 3E 48 0F 44 F9 48  8B CF ?? ?? ?? ?? ?? 84
constexpr uintptr_t GameFunc_Native_IsRunning_offset = 0x002DB610;

// RemovedSpellBook
constexpr uintptr_t TESObjectBook_vtbl_offset = 0x015592B8;

// Save Screenshots
// 84 C0 75 26 E8 ? ? ? ?  + 0x9
constexpr uintptr_t BGSSaveLoadManager_ProcessEvents_RequestScreenshot_hook_offset = 0x00589BC3;
constexpr uintptr_t MenuSave_RequestScreenshot_hook_offset = 0x005B158A;
// 41 89 5d 00 40 84 ff 0f 85 + 0x8
constexpr uintptr_t Screenshot_Jnz_hook_offset = 0x0129764A;
constexpr uintptr_t Screenshot_Render_Orig_jnz_offset = 0x012978E5;
// + 0x128 from screenshot_jnz
constexpr uintptr_t Render_Target_Hook_1_offset = 0x01297775;
// + 0x85
constexpr uintptr_t Render_Target_Hook_2_offset = 0x012977FA;
constexpr uintptr_t g_RequestSaveScreenshot_offset = 0x02F38968;

// Slow Time Camera Movement
// 40 53 48 83 EC 50 F3 0F 10 51 ? 48 8B D9 +0x2B, +0x92, +0x1F9
constexpr uintptr_t CameraMove_Timer1_offset = 0x008505CF; // +0x4
constexpr uintptr_t CameraMove_Timer2_offset = 0x00850636; // +0x4
constexpr uintptr_t CameraMove_Timer3_offset = 0x0085079D; // +0x4
// F3 0F 59 1D ? ? ? ? F3 0F 10 05 ? ? ? ?
constexpr uintptr_t CameraMove_Timer4_offset = 0x00850B3A; // +0x4
// E8 ? ? ? ? 48 8D 4B 4C -> +0x13
constexpr uintptr_t CameraMove_Timer5_offset = 0x00850C07; // +0x4

// Calendar Skipping
// E8 ? ? ? ? F6 87 DC 0B 00 00 01
constexpr uintptr_t Calendar_AdvanceTime_call_offset = 0x005A6230; 

// Tree Reflections
// ??_7BSDistantTreeShader@@6B@
// vfunc 3 -> +0x37
constexpr uintptr_t BSDistantTreeShader_hook_offset = 0x013014F7;

// Vertical Look Sensitivity
// ??_7ThirdPersonState@@6B@ vtbl last function + 0x71
constexpr uintptr_t ThirdPersonState_Vfunc_Hook_offset = 0x008508D1;
// ??_7DragonCameraState@@6B@ vtbl last function + 0x5F
constexpr uintptr_t DragonCameraState_Vfunc_Hook_offset = 0x004F971F;
// ??_7HorseCameraState@@6B@ vtbl last function + 0x5F
constexpr uintptr_t HorseCameraState_Vfunc_Hook_offset = 0x0084998F;

// ??_7EnchantmentItem@@6B@
constexpr uintptr_t offset_vtbl_EnchantmentItem = 0x015217E0;  

// Animation Load Sign
// SkyrimSE.exe+0x00B669C0    | 0FBF83 28010000          | movsx   eax, word ptr ds:[rbx + 0x128]            |
//-> SkyrimSE.exe+0x00B669C0  | 0FB783 28010000          | movzx   eax, word ptr ds:[rbx + 0x128]            |
constexpr uintptr_t offset_AnimationLoadSigned = 0x00B669C1;

// BSLightingShader::SetupGeometry Parallax Technique fix
// 8B C1 25 ? ? ? ? 41 0F 45 D0 
constexpr uintptr_t offset_BSLightingShader_SetupGeometry_ParallaxTechniqueFix = 0x012F3127;

// Warnings

// Dupe Addon Node index
constexpr uintptr_t vtbl_BGSAddonNode_LoadForm_offset = 0x0154D150;

// E8 ? ? ? ? E9 ? ? ? ? 4C 8D 9C 24 80 00 00 00
constexpr uintptr_t Unk_DataReload_Func_offset = 0x005B5490;    // 1_5_73

constexpr uintptr_t Call1_Unk_DataReload_func_offset = 0x005AF3D0;    // 1_5_73

// E8 ? ? ? ? 33 C9 E8 ? ? ? ? 84 C0
constexpr uintptr_t Call2_Unk_DataReload_func_offset = 0x005B5280;    // 1_5_73

// Refr Handle Limit
// LookupRefrPtrByHandle uses this
constexpr uintptr_t g_RefrHandleArray_offset = 0x01EC47C0;

// Experimental

// BB ? ? ? ? 4C 8B FA 
constexpr uintptr_t Win32FileType_CopyToBuffer_offset = 0x13420D4;
// C6 83 ? ? ? ? ? BA ? ? ? ? ->
constexpr uintptr_t Win32FileType_ctor_offset = 0x134113E;
// E8 ? ? ? ? 8B D0 4C 8B CB ->
constexpr uintptr_t ScrapHeap_GetMaxSize_offset = 0x0059B9A4;

// E8 ? ? ? ? 0F B6 D0 EB 02 
constexpr uintptr_t TESFile_IsMaster_offset = 0x0017E130;