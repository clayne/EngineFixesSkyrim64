#include <intrin.h>

#ifndef SKYRIMVR

#include "RE/Skyrim.h"
#include "SKSE/API.h"
#include "SKSE/CodeGenerator.h"
#include "SKSE/Trampoline.h"

#include "patches.h"
#include "utils.h"

namespace patches
{
    RelocPtr<float> FrameTimer_WithSlowTime(g_FrameTimer_SlowTime_offset);

    // +0x252
    RelocAddr<uintptr_t> GameLoop_Hook(GameLoop_Hook_offset);
    RelocPtr<uint32_t> UnkGameLoopDword(UnkGameLoopDword_offset);

    // 5th function in??_7BSWaterShader@@6B@ vtbl
    // F3 0F 10 0D ? ? ? ? F3 0F 11 4C 82 ?
    // loads TIMER_DEFAULT which is a timer representing the GameHour in seconds
    RelocAddr<uintptr_t> WaterShader_ReadTimer_Hook(WaterShader_ReadTimer_Hook_offset);

    float timer = 8 * 3600; // Game timer inits to 8 AM

    void update_timer()
    {
        timer = timer + *FrameTimer_WithSlowTime * config::waterflowSpeed;
        if (timer > 86400) // reset timer to 0 if we go past 24 hours
            timer = timer - 86400;
    }

    bool PatchWaterflowAnimation()
    {
        _VMESSAGE("- waterflow timer -");

        _VMESSAGE("hooking new timer to the game update loop...");
        {
            struct GameLoopHook_Code : SKSE::CodeGenerator
            {
                GameLoopHook_Code() : SKSE::CodeGenerator()
                {
                    Xbyak::Label retnLabel;
                    Xbyak::Label funcLabel;
                    Xbyak::Label unkDwordLabel;

                    // enter 5B36F2
                    // some people were crashing and while I'm pretty sure the registers dont need to be saved looking at the function in question, this was just a check to see if that was the problem
                    // (it wasnt)
                    /*
                    sub(rsp, 0x20);
                    vmovdqu(ptr[rsp], xmm0);
                    vmovdqu(ptr[rsp + 0x10], xmm1);
                    push(rax);*/
                    sub(rsp, 0x20);
                    call(ptr[rip + funcLabel]);
                    add(rsp, 0x20);
                    /*
                    pop(rax);
                    vmovdqu(xmm1, ptr[rsp + 0x10]);
                    vmovdqu(xmm0, ptr[rsp]);
                    add(rsp, 0x20);*/
                    // .text:00000001405B36F2                 mov     edx, cs : dword_142F92950
                    mov(rdx, ptr[rip + unkDwordLabel]);
                    mov(edx, dword[rdx]);

                    // exit 5B36F8
                    jmp(ptr[rip + retnLabel]);

                    L(funcLabel);
                    dq(uintptr_t(update_timer));

                    L(retnLabel);
                    dq(GameLoop_Hook.GetUIntPtr() + 0x6);

                    L(unkDwordLabel);
                    dq(UnkGameLoopDword.GetUIntPtr());
                }
            };

            GameLoopHook_Code code;
            code.finalize();

            auto trampoline = SKSE::GetTrampoline();
            trampoline->Write6Branch(GameLoop_Hook.GetUIntPtr(), uintptr_t(code.getCode()));
        }
        _VMESSAGE("replacing water flow timer with our timer...");
        {
            struct WaterFlowHook_Code : SKSE::CodeGenerator
            {
                WaterFlowHook_Code() : SKSE::CodeGenerator()
                {
                    Xbyak::Label retnLabel;
                    Xbyak::Label timerLabel;

                    // enter 130DFD9
                    // .text:000000014130DFD9                 movss   xmm1, cs:TIMER_DEFAULT
                    // .text:000000014130DFE1                 movss   dword ptr[rdx + rax * 4 + 0Ch], xmm1
                    mov(r9, ptr[rip + timerLabel]); // r9 is safe to use, unused again until .text:000000014130E13C                 mov     r9, r12
                    movss(xmm1, dword[r9]);
                    movss(dword[rdx + rax * 4 + 0xC], xmm1);

                    // exit 130DFE7
                    jmp(ptr[rip + retnLabel]);

                    L(retnLabel);
                    dq(WaterShader_ReadTimer_Hook.GetUIntPtr() + 0xE);

                    L(timerLabel);
                    dq(uintptr_t(&timer));
                }
            };

            WaterFlowHook_Code code;
            code.finalize();

            auto trampoline = SKSE::GetTrampoline();
            trampoline->Write6Branch(WaterShader_ReadTimer_Hook.GetUIntPtr(), uintptr_t(code.getCode()));
        }
        _VMESSAGE("success");

        return true;
    }

    decltype(&fopen_s) VC140_fopen_s;
    errno_t hk_fopen_s(FILE **File, const char *Filename, const char *Mode)
    {
        errno_t err = VC140_fopen_s(File, Filename, Mode);

        if (err != 0)
            _MESSAGE("WARNING: Error occurred trying to open file: fopen_s(%s, %s), errno %d", Filename, Mode, err);

        return err;
    }

    decltype(&_wfopen_s) VC140_wfopen_s;
    errno_t hk_wfopen_s(FILE **File, const wchar_t *Filename, const wchar_t *Mode)
    {
        errno_t err = VC140_wfopen_s(File, Filename, Mode);

        if (err != 0)
            _MESSAGE("WARNING: Error occurred trying to open file: _wfopen_s(%p, %p), errno %d", Filename, Mode, err);

        return err;
    }

    decltype(&fopen) VC140_fopen;
    FILE *hk_fopen(const char *Filename, const char *Mode)
    {
        FILE *f = VC140_fopen(Filename, Mode);

        if (!f)
            _MESSAGE("WARNING: Error occurred trying to open file: fopen(%s, %s)", Filename, Mode);

        return f;
    }

    bool PatchMaxStdio()
    {
        _VMESSAGE("- max stdio -");

        const HMODULE crtStdioModule = GetModuleHandleA("API-MS-WIN-CRT-STDIO-L1-1-0.DLL");

        if (!crtStdioModule)
        {
            _VMESSAGE("crt stdio module not found, failed");
            return false;
        }

        const auto maxStdio = reinterpret_cast<decltype(&_setmaxstdio)>(GetProcAddress(crtStdioModule, "_setmaxstdio"))(2048);

        _VMESSAGE("max stdio set to %d", maxStdio);

        *(void **)&VC140_fopen_s = (uintptr_t *)PatchIAT(GetFnAddr(hk_fopen_s), "API-MS-WIN-CRT-STDIO-L1-1-0.DLL", "fopen_s");
        *(void **)&VC140_wfopen_s = (uintptr_t *)PatchIAT(GetFnAddr(hk_wfopen_s), "API-MS-WIN-CRT-STDIO-L1-1-0.DLL", "wfopen_s");
        *(void **)&VC140_fopen = (uintptr_t *)PatchIAT(GetFnAddr(hk_fopen), "API-MS-WIN-CRT-STDIO-L1-1-0.DLL", "fopen");

        return true;
    }

    RelocAddr<uintptr_t> QuickSaveLoadHandler_HandleEvent_SaveType(QuickSaveLoadHandler_HandleEvent_SaveType_offset);
    RelocAddr<uintptr_t> QuickSaveLoadHandler_HandleEvent_LoadType(QuickSaveLoadHandler_HandleEvent_LoadType_offset);

    bool PatchRegularQuicksaves()
    {
        const uint32_t regular_save = 0xF0000080;
        const uint32_t load_last_save = 0xD0000100;

        _VMESSAGE("- regular quicksaves -");
        SafeWrite32(QuickSaveLoadHandler_HandleEvent_SaveType.GetUIntPtr(), regular_save);
        SafeWrite32(QuickSaveLoadHandler_HandleEvent_LoadType.GetUIntPtr(), load_last_save);
        _VMESSAGE("success");
        return true;
    }

    RelocAddr<uintptr_t> AchievementModsEnabledFunction(AchievementModsEnabledFunction_offset);

    bool PatchEnableAchievementsWithMods()
    {
        _VMESSAGE("- enable achievements with mods -");
        // Xbyak is used here to generate the ASM to use instead of just doing it by hand
        struct Patch : SKSE::CodeGenerator
        {
            Patch() : SKSE::CodeGenerator(100)
            {
                mov(al, 0);
                ret();
            }
        };

        Patch patch;
        patch.finalize();

        for (UInt32 i = 0; i < patch.getSize(); ++i)
        {
            SafeWrite8(AchievementModsEnabledFunction.GetUIntPtr() + i, patch.getCode()[i]);
        }

        _VMESSAGE("success");
        return true;
    }

    RelocAddr<uintptr_t> ChargenCacheFunction(ChargenCacheFunction_offset);
    RelocAddr<uintptr_t> ChargenCacheClearFunction(ChargenCacheClearFunction_offset);

    bool PatchDisableChargenPrecache()
    {

        _VMESSAGE("- disable chargen precache -");
        SafeWrite8(ChargenCacheClearFunction.GetUIntPtr(), 0xC3);
        SafeWrite8(ChargenCacheClearFunction.GetUIntPtr(), 0xC3);
        _VMESSAGE("success");
        return true;
    }

    bool loadSet = false;

    bool hk_TESFile_IsMaster(RE::TESFile * modInfo)
    {
        if (loadSet)
            return true;

        uintptr_t returnAddr = (uintptr_t)(_ReturnAddress()) - RelocationManager::s_baseAddr;

        if (returnAddr == 0x16E11E)
        {
            loadSet = true;
            _MESSAGE("load order finished");
            auto dhnl = RE::TESDataHandler::GetSingleton();
            for (auto& mod : dhnl->compiledFileCollection.files)
            {
                mod->recordFlags |= RE::TESFile::RecordFlag::kMaster;
            }
            return true;
        }

		return (modInfo->recordFlags & RE::TESFile::RecordFlag::kMaster) != RE::TESFile::RecordFlag::kNone;
    }

    RelocAddr<uintptr_t> TESFile_IsMaster(TESFile_IsMaster_offset);

    bool PatchTreatAllModsAsMasters()
    {
        _VMESSAGE("- treat all mods as masters -");
        MessageBox(nullptr, TEXT("WARNING: You have the treat all mods as masters patch enabled. I hope you know what you're doing!"), TEXT("Engine Fixes for Skyrim Special Edition"), MB_OK);
        auto trampoline = SKSE::GetTrampoline();
        trampoline->Write6Branch(TESFile_IsMaster.GetUIntPtr(), GetFnAddr(hk_TESFile_IsMaster));
        _VMESSAGE("success");

        return true;
    }

    RelocAddr<uintptr_t> FirstPersonState_DontSwitchPOV(FirstPersonState_DontSwitchPOV_offset);
    RelocAddr<uintptr_t> ThirdPersonState_DontSwitchPOV(ThirdPersonState_DontSwitchPOV_offset);

    bool PatchScrollingDoesntSwitchPOV()
    {
        _VMESSAGE("- scrolling doesnt switch POV -");
        SafeWrite8(FirstPersonState_DontSwitchPOV.GetUIntPtr(), 0xEB);
        SafeWrite8(ThirdPersonState_DontSwitchPOV.GetUIntPtr(), 0xEB);
        _VMESSAGE("- success -");
        return true;
    }

    RelocAddr<uintptr_t> SleepWaitTime_Compare(SleepWaitTime_Compare_offset);

    bool PatchSleepWaitTime()
    {
        _VMESSAGE("- sleep wait time -");
        {
            struct SleepWaitTime_Code : SKSE::CodeGenerator
            {
                SleepWaitTime_Code() : SKSE::CodeGenerator()
                {
                    push(rax);
                    mov(rax, (size_t)&config::sleepWaitTimeModifier);
                    comiss(xmm0, ptr[rax]);
                    pop(rax);
                    jmp(ptr[rip]);
                    dq(SleepWaitTime_Compare.GetUIntPtr() + 0x7);
                }
            };

            SleepWaitTime_Code code;
            code.finalize();

            auto trampoline = SKSE::GetTrampoline();
            trampoline->Write6Branch(SleepWaitTime_Compare.GetUIntPtr(), uintptr_t(code.getCode()));
        }
        _VMESSAGE("success");
        return true;
    }

	RelocAddr<uintptr_t> Win32FileType_CopyToBuffer(Win32FileType_CopyToBuffer_offset);
	RelocAddr<uintptr_t> Win32FileType_ctor(Win32FileType_ctor_offset);
	RelocAddr<uintptr_t> ScrapHeap_GetMaxSize(ScrapHeap_GetMaxSize_offset);

	bool PatchSaveGameMaxSize()
	{
		_VMESSAGE("- save game max size -");
		SafeWrite8(Win32FileType_CopyToBuffer.GetUIntPtr(), 0x08);
		SafeWrite8(Win32FileType_ctor.GetUIntPtr(), 0x08);
		SafeWrite8(ScrapHeap_GetMaxSize.GetUIntPtr(), 0x08);
		_VMESSAGE("success");
		return true;
	}
}

#endif