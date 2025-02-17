/*
 * RISCV machine
 *
 * Copyright (c) 2016 Fabrice Bellard
 * Copyright (C) 2018,2019, Esperanto Technologies Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License")
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * THIS FILE IS BASED ON THE RISCVEMU SOURCE CODE WHICH IS DISTRIBUTED
 * UNDER THE FOLLOWING LICENSE:
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#ifndef RISCV_MACHINE_H
#define RISCV_MACHINE_H

#include "virtio.h"
#include "machine.h"
#include "riscv_cpu.h"

#ifdef LIVECACHE
#include "LiveCacheCore.h"
#endif

#define MAX_CPUS  8

/* Hooks */
typedef struct RISCVMachineHooks {
    /* Returns -1 if invalid CSR, 0 if OK. */
    int (*csr_read) (RISCVCPUState *s, uint32_t csr, uint64_t *pval);
    int (*csr_write)(RISCVCPUState *s, uint32_t csr, uint64_t   val);
} RISCVMachineHooks;

struct RISCVMachine {
    VirtMachine common;
    RISCVMachineHooks hooks;
    PhysMemoryMap *mem_map;
#ifdef LIVECACHE
    LiveCache *llc;
#endif
    RISCVCPUState *cpu_state[MAX_CPUS];
    int      ncpus;
    uint64_t ram_size;
    uint64_t ram_base_addr;
    /* PLIC */
    uint32_t plic_pending_irq;
    uint32_t plic_served_irq;
    IRQSignal plic_irq[32]; /* IRQ 0 is not used */

    /* HTIF */
    uint64_t htif_tohost_addr;

    VIRTIODevice *keyboard_dev;
    VIRTIODevice *mouse_dev;

    int virtio_count;

    /* MMIO range (for co-simulation only) */
    uint64_t mmio_start;
    uint64_t mmio_end;

    /* Extension state, not used by Dromajo itself */
    void *ext_state;
};

#define CLINT_BASE_ADDR 0x02000000
#define CLINT_SIZE      0x000c0000
#define RTC_FREQ_DIV 16 /* arbitrary, relative to CPU freq to have a
                           10 MHz frequency */

#endif
