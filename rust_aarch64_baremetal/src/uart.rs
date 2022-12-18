pub const UART_PORT: *mut u8 = 0x9000000 as *mut u8;

/// Write a single byte to UART.
pub fn write_byte(byte: u8) {
    unsafe {
        *UART_PORT = byte;
    }
}

/// Write a message to UART.
pub fn write_str(msg: &str) {
    for c in msg.chars() {
        write_byte(c as u8);
    }
}

/// Wrapper interface for formatted printing to UART.
pub struct UartWriter;

impl core::fmt::Write for UartWriter {
    fn write_str(&mut self, s: &str) -> core::fmt::Result {
        write_str(s);
        Ok(())
    }
}
