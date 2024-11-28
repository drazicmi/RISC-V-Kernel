.extern _ZN5Riscv20handleSupervisorTrapEv
// everything starts with _Z than N for namesapce its in class RiscV and the funtion is named handleSupervisorTrap)
// 5 and 20 are the lenghts of class and fucntion
// E is for end
// v is for void; the function didn't have arguments so we put void

// We have to have the align 4 for the trap function, so that when we register it as our main supervisorTrap we can put the 0 in last 4 bits
// which in RISC-V architecture signlas that we have only 1 trap routine and that all the interrupts will redirect to this exact routine
.align 4
.global _ZN5Riscv14supervisorTrapEv // Function has global scope
.type _ZN5Riscv14supervisorTrapEv, @function // Symbol type is function
_ZN5Riscv14supervisorTrapEv:
    // push all registers to stack
    addi sp, sp, -256 // Allocate space on stack; Stack pointer is not moving throughout the saving process
    .irp index, 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31 // Macro is for repeatition of instructions (the code we write is shorter)
    sd x\index, \index * 8(sp)
    .endr

    // We are calling the C function that is used to handle our trap routine (We are doing this so we can write trap rouitne in C++ instead of assembly)
    call _ZN5Riscv20handleSupervisorTrapEv

    // pop all registers from stack
    .irp index, 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31
    ld x\index, \index * 8(sp)
    .endr
    addi sp, sp, 256 // Delete space on stack; Stack pointer is not moving throughout the restoring process

    // Return from the function differs from normal function because we need to change the mode we are working in besides
    // the normal return process. sret takes the previuous value from the sstatus register and puts the program back in the mode
    // it ran previously. Also if interrupts were previuously enabled they get enabled again.
    // The thing that also differes is that sret takes sepc (supervisor exception pc) and uses that as return adress instead of ra register.
    sret
