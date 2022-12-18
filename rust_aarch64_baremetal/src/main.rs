#![no_std]
#![no_main]

mod uart;

use core::arch::global_asm;
use core::fmt::Write;

// Include the minimal boot trampoline.
global_asm!(include_str!("boot.s"));

#[no_mangle]
pub extern "C" fn rs_main() {
    uart::write_str(
        "The string '0x41 == 65' should appear below, followed by a success message.\n\n>> ",
    );

    let mut uw = uart::UartWriter {};
    write!(uw, "{:x} == 65\n\n", 65).unwrap();

    uart::write_str("Success! Press 'C-a x' to exit.\n");
    loop {}
}

#[panic_handler]
fn panic(_info: &core::panic::PanicInfo) -> ! {
    loop {}
}
