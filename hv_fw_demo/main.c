#include <Hypervisor/hv_vcpu.h>
#include <Hypervisor/hv_vm.h>
#include <Hypervisor/hv_vm_allocate.h>
#include <Hypervisor/hv_vm_config.h>
#include <Hypervisor/hv_vm_types.h>

#include <pthread.h>

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define log(fmt, ...)                                     \
    do {                                                  \
        printf("%s: " fmt "\n", __func__, ##__VA_ARGS__); \
    } while (0)

static uint32_t const code[] = {
    0xaa1f03e0, // mov    x0, xzr
    0x580000a1, // ldr    x1, 0x18
    0x580000c2, // ldr    x2, 0x20
    0x580000e3, // ldr    x3, 0x28
    0x58000104, // ldr    x4, 0x30
    0xd4000c62, // hvc    #0x63

    0x11111111, 0x00000000, 0x22222222, 0x00000000, 0x33333333, 0x00000000,
    0x44444444, 0x00000000
};

#define GUEST_PAGE_ADDR 0x80000

static void *vcpu_thread(void *opaque)
{
    hv_vcpu_t vcpu;
    hv_vcpu_exit_t *exit;

    log("Creating vCPU...");
    hv_return_t err = hv_vcpu_create(&vcpu, &exit, NULL);
    if (err != HV_SUCCESS) {
        log("Failed to create vCPU. (%#x)", err);
        return NULL;
    }

    log("Setting initial PC...");
    err = hv_vcpu_set_reg(vcpu, HV_REG_PC, GUEST_PAGE_ADDR);
    if (err != HV_SUCCESS) {
        log("Failed to set initial PC. (%#x)", err);
        return NULL;
    }

    log("Running vCPU until exit...");
    err = hv_vcpu_run(vcpu);
    if (err != HV_SUCCESS) {
        log("Failed to run vCPU. (%#x)", err);
        return NULL;
    }

    uint64_t regs[5] = {};
    hv_vcpu_get_reg(vcpu, HV_REG_X0, &regs[0]);
    hv_vcpu_get_reg(vcpu, HV_REG_X1, &regs[1]);
    hv_vcpu_get_reg(vcpu, HV_REG_X2, &regs[2]);
    hv_vcpu_get_reg(vcpu, HV_REG_X3, &regs[3]);
    hv_vcpu_get_reg(vcpu, HV_REG_X4, &regs[4]);

    log("Exited with register state: ");
    log("  X0 = %#llx", regs[0]);
    log("  X1 = %#llx", regs[1]);
    log("  X2 = %#llx", regs[2]);
    log("  X3 = %#llx", regs[3]);
    log("  X4 = %#llx", regs[4]);

    log("Destroying vCPU...");
    err = hv_vcpu_destroy(vcpu);
    if (err != HV_SUCCESS) {
        log("Failed to destroy vCPU. (%#x)", err);
        return NULL;
    }

    return NULL;
}

int main(int argc, char const **argv)
{
    log("Creating VM...");
    hv_return_t err = hv_vm_create(NULL);
    if (err != HV_SUCCESS) {
        log("Failed to create VM. (%#x)", err);
        return EXIT_FAILURE;
    }

    log("Allocating guest page...");
    void *guest_page;
    err = hv_vm_allocate(&guest_page, 0x4000, HV_ALLOCATE_DEFAULT);
    if (err != HV_SUCCESS) {
        log("Failed to allocate guest page. (%#x)", err);
    }

    log("Copying stub code to guest page... (%p)", guest_page);
    memcpy(guest_page, code, sizeof(code));

    log("Mapping guest page at %#x as RX...", GUEST_PAGE_ADDR);
    err = hv_vm_map(guest_page, GUEST_PAGE_ADDR, 0x4000, HV_MEMORY_READ | HV_MEMORY_EXEC);
    if (err != HV_SUCCESS) {
        log("Failed to map guest page. (%#x)", err);
    }

    pthread_t vcpu_pt;
    pthread_create(&vcpu_pt, NULL, vcpu_thread, NULL);

    pthread_join(vcpu_pt, NULL);

    log("Unmapping guest page...");
    err = hv_vm_unmap(GUEST_PAGE_ADDR, 0x4000);
    if (err != HV_SUCCESS) {
        log("Failed to unmap guest page. (%#x)", err);
    }

    log("Destroying VM...");
    err = hv_vm_destroy();
    if (err != HV_SUCCESS) {
        log("Failed to destroy VM. (%#x)", err);
    }

    log("Done.");

    return EXIT_SUCCESS;
}
