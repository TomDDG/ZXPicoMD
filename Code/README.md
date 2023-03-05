## Notes on using the 2nd CORE

As all the main IO such as SD Card access, OLED and menu system are on core 1 and all the timing critical Microdrive elements are on core 2 there is a need to send commands between the two cores. As such my code makes use of the Pico inter-core FIFOs to communicate, sending commands to do things like telling the 1st core to get more data from the SD Card or to close an image file. The FIFO uses a 32bit integer `uint32_t` to communicate and I simply mask this into 4 individual bytes, a command a 3 "message" bytes.

````
#define CMD_MASK_CMD   0b11111111000000000000000000000000
#define CMD_MASK_VAL1  0b00000000000000000000000011111111
#define CMD_MASK_VAL2  0b00000000000000001111111100000000
#define CMD_MASK_VAL3  0b00000000111111110000000000000000
````

To launch the 2nd core from the 1st core I use the standard launch command and a very simple handshake:

```
multicore_launch_core1(core1main); // launch 2nd core to run the MicroDrive emulation
multicore_fifo_push_blocking(CMD_MASK_CMD); // push command to 2nd Core, simple handshake
while((multicore_fifo_pop_blocking()&CMD_MASK_CMD)!=CMD_MASK_CMD); // wait for response
```

On the 2nd core 

```
void __no_inline_not_in_flash_func(core1main)() {
    // sync cores
    while((multicore_fifo_pop_blocking()&CMD_MASK_CMD)!=CMD_MASK_CMD); // wait for message form 1st core
    multicore_fifo_push_blocking(CMD_MASK_CMD); // send a response
````

Using `__no_inline_not_in_flash_func` for the 2nd core means this code is always run from memory and not from flash. In theory this means timings are always the same and there is no delay in launching.

During normal operation the 1st core waits for a command from the 2nd

````
while(true) { // infinite loop
    rx=multicore_fifo_pop_blocking(); // wait for command from core 2
    cmd=(rx&CMD_MASK_CMD)>>24; // what is the command
    val3=(rx&CMD_MASK_VAL3)>>16; // message 1
    val2=(rx&CMD_MASK_VAL2)>>8; // message 2
    val1=(rx&CMD_MASK_VAL1); // message 3
````

When the menu is active (on core 1), I purposely close down the 2nd core to prevent any issues. This is because I use a shared memory area to copy data between the two cores and this coupled with some dynamic memory allocation resulted some memory corruption, in particular when using the memory heavy Z80 conversion routines. It was cleaner to shutdown the 2nd core and reset the shared memory again after the menu was exited and I knew the memory was only being used for data transfer.

````
switch (cmd) {
// Menu
    case 'M':
        multicore_reset_core1(); // reset core1 ready for re-launch 
````

## Notes on Memory Usage

The Raspberry Pico only has 256kB of memory which is actually a lot for a microcontroller. However when adding things like Z80 conversion fo 128kB snapshots it quickly gets used up as you need to store the uncompressed 128kB Spectrum memory and also have room for the Microdrive cartridge to write to. As such I make use of the SD Card for memory buffers, a bit like to good old disk cache. This is slow but still good enough to complete the tasks in seconds.

For the ZX PicoMD I use a small shared memory area, defined globally using `uint8_t mem_buffer[MEMSIZE];` for transfering data between the cores. I also use dynamic memory allocation within the menu system for some the conversion utilities and file sorts. 

In order to share memory between two cores you have to be very careful with contention, as in both cores accessing the same memory at the same time. I use a read ahead concept where the 1st core is always 12 Microdrive sectors (543*12=6516bytes) ahead of the 2nd core with some controls in place to ensure this is always the case. All data transfers are initiated by the 2nd core using the FIFO so the 2nd core always knows where the 1st core is.


