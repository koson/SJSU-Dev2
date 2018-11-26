#pragma once
#include <cstring>
// Define an alias for the Isr function pointer.
using IsrPointer = void (*)(void);

#if defined HOST_TEST
#include "L5_Testing/testing_frameworks.hpp"
// During host tests, we don't want to call these functions since they write to
// memory locations directly.
// The defines below will replace every instance of the functions names with
// <name>_original.
// Which means that, unless the code in this file explicitly makes a call to
// <name>_original(), these functions will not be called below
#define NVIC_EnableIRQ NVIC_EnableIRQ_original
#define NVIC_DisableIRQ NVIC_DisableIRQ_original
#define NVIC_SetPriority NVIC_SetPriority_original
#endif  // defined HOST_TEST

#include "L0_LowLevel/LPC40xx.h"
#include "L2_Utilities/macros.hpp"

#if defined HOST_TEST
// Remove the text replacement used to replace the variable names above.
#undef NVIC_EnableIRQ
#undef NVIC_DisableIRQ
#undef NVIC_SetPriority

DECLARE_FAKE_VOID_FUNC(NVIC_EnableIRQ, IRQn_Type);
DECLARE_FAKE_VOID_FUNC(NVIC_DisableIRQ, IRQn_Type);
DECLARE_FAKE_VOID_FUNC(NVIC_SetPriority, IRQn_Type, uint32_t);
#endif  // defined HOST_TEST

constexpr size_t kInterruptsCount = 56;
// NOLINTNEXTLINE(readability-identifier-naming)
extern IsrPointer dynamic_isr_vector_table[kInterruptsCount];

void RegisterIsr(IRQn_Type irq, IsrPointer isr, bool enable_interrupt = true,
                 int32_t priority = -1);

void DeregisterIsr(IRQn_Type irq);
// External declaration for the pointer to the stack top from the linker script
extern "C" void StackTop(void);
// These are defined after the compilation of the FreeRTOS port for Cortex M4F
// These will link to those definitions.
extern "C" void xPortPendSVHandler(void);   // NOLINT
extern "C" void vPortSVCHandler(void);      // NOLINT
extern "C" void xPortSysTickHandler(void);  // NOLINT
// Forward declaration of the default handlers. These are aliased.
// When the application defines a handler (with the same name), this will
// automatically take precedence over these weak definitions
extern "C" SJ2_IGNORE_STACK_TRACE(void ResetIsr(void));
extern "C" SJ2_IGNORE_STACK_TRACE(void NmiHandler(void));
extern "C" SJ2_IGNORE_STACK_TRACE(void HardFaultHandler(void));
extern "C" SJ2_IGNORE_STACK_TRACE(void MemManageHandler(void));
extern "C" SJ2_IGNORE_STACK_TRACE(void BusFaultHandler(void));
extern "C" SJ2_IGNORE_STACK_TRACE(void UsageFaultHandler(void));
extern "C" SJ2_IGNORE_STACK_TRACE(void SvcHandler(void));
extern "C" SJ2_IGNORE_STACK_TRACE(void DebugMonHandler(void));
extern "C" SJ2_IGNORE_STACK_TRACE(void PendSVHandler(void));
extern "C" SJ2_IGNORE_STACK_TRACE(void SysTickHandler(void));
extern "C" SJ2_IGNORE_STACK_TRACE(SJ2_WEAK void IntDefaultHandler(void));
