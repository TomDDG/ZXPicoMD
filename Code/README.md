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

When the menu is active (on core 1), I purposely close down the 2nd core to prevent any issues. This is because I used a shared memory are and found in some cases it could get corrupted, in particular when using the memory heavy Z80 conversion routines. It was cleaner to shutdown the 2nd core and reset the shared memory again after the menu was exited.

````
switch (cmd) {
// Menu
    case 'M':
        multicore_reset_core1(); // reset core1 ready for re-launch 
````



