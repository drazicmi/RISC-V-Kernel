.global _ZN3Thr13contextSwitchEPNS_7ContextES1_ // Function has global scope
.type _ZN3Thr13contextSwitchEPNS_7ContextES1_, @function // Symbol type is function
// P -> pointer
// S_ -> TCB
// S1_ -> PNS_7Context
_ZN3Thr13contextSwitchEPNS_7ContextES1_:
    // a0 - &old->context
    // a1 - &running->context
    sd ra, 0 * 8(a0)
    sd sp, 1 * 8(a0)

    ld ra, 0 * 8(a1)
    ld sp, 1 * 8(a1)

    ret