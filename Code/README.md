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

For the ZX PicoMD I use a small shared memory area, defined globally using `uint8_t mem_buffer[MEMSIZE];` for transfering data between the cores. I also use dynamic memory allocation within the menu system for some the conversion utilities and file sorts. An example set-up from the Z80 conversion routine:

````
if ((main48k = (uint8_t*)malloc(49152 * sizeof(uint8_t))) == NULL) { // cannot create space for copy of main memory
    f_close(&fpIn);
    f_close(&fpOut);    
    return false;
}
````

In order to share memory between two cores you have to be very careful with contention, as in both cores accessing the same memory at the same time. In order to prevent this core 2 and core 1 use different parts of the shared memory unless a specific copy is being made at which point the cores wait for this copy operation to complete before moving on. This is all controlled by the inter-core FIFO.

A quick example, there are 3 key areas of the shared memory.

1. SD Card Read Buffer
2. SD Card Write Buffer
3. Microdrive Buffer

When a read is requested from Core 1 (as it needs more data) whatever is in 1 gets copied to 3 and control handed back to core 2. Once control is handed back core 1 grabs the next 12 sectors from the SD Card read for the next read request. As the copy takes micro seconds to complete there is easily enough time before the Interface 1 needs more data.

During a write the Microdrive will overwrite the Microdrive buffer with up to 12 sectors and once the 12th sector is passed core 2 will send a write command to core 1. This will simply tell core 1 to copy data from the Microdrive buffer to the SD Card Write buffer ready for the next SD card access. As the Microdrive now needs more data (it finished all 12 sectors0 it requests a read to get the next 12 sectors loaded and this trigger the SD Card write.

The following two pieces of code handle this read/write operation:

````
case 'R': // read operation requested from core 2
    for(i=0;i<MDR_SECTOR_SIZE*SECTOR_READAHEAD;i++) { // 12*543 = 6516bytes
        sectorBuffer_Read[i]=sectorBuffer_In[i]; // copy 12 sectors to Microdrive buffer
    }                
    multicore_fifo_push_blocking('X'<<24); // all done message               
    if(writeSector) {                                        
        readFilepos=f_tell(&fpRW); // move to correct place in file
        writeBack(sectorBuffer_Out,&fpRW); // write 12 sectors to file
        writeSector=false; // no more writes
    } 
    readAhead(sectorBuffer_In,&currentSector[driveNumber],&fpRW); // read next 12 sectors
break;
//     
case 'W': // write operation requested from core 2
    for(i=0;i<MDR_SECTOR_SIZE*SECTOR_READAHEAD;i++) { // 12*543 = 6516bytes
        sectorBuffer_Out[i]=sectorBuffer_Read[i]; // copy 12 sectors from Microdrive buffer to write buffer
    }
    multicore_fifo_push_blocking('X'<<24); // all done message         
    writeSector=true; // set to true so on next read it also writes
break;
````

## Notes on use of FATFS_SPI library

As part of using this library you need to customise the `ffconf.h` file. The version I use has:

````
#define FFCONF_DEF	80196
#define FF_FS_READONLY	0
#define FF_FS_MINIMIZE	1 // removes un-needed operations such as mkdir in order to reduce memory footpint
#define FF_USE_STRFUNC	0
#define FF_USE_FIND		0
#define FF_USE_MKFS		0
#define FF_USE_FASTSEEK	1 // fast seek speeds up some operations which is useful due to the limited time available from an IF1 request and when data is needed to be sent
#define FF_USE_EXPAND	0
#define FF_USE_CHMOD	0
#define FF_USE_LABEL	0
#define FF_USE_FORWARD	0
#define FF_CODE_PAGE	437
#define FF_USE_LFN		3
#define FF_MAX_LFN		255
#define FF_LFN_UNICODE	2
#define FF_LFN_BUF		255
#define FF_SFN_BUF		12
#define FF_STRF_ENCODE	3
#define FF_FS_RPATH		0
#define FF_VOLUMES		1 // only 1 SD Card connected
#define FF_STR_VOLUME_ID	0
#define FF_VOLUME_STRS		"RAM","NAND","CF","SD","SD2","USB","USB2","USB3"
#define FF_MULTI_PARTITION	0
#define FF_MIN_SS		512
#define FF_MAX_SS		512
#define FF_LBA64		1
#define FF_MIN_GPT		0x10000000
#define FF_USE_TRIM		0
#define FF_FS_TINY		0 // I did play around with Tiny, to reduce memory footprint, but as only 1 file is ever open at one time it didn't make any difference
#define FF_FS_EXFAT		1 // yes please, FAT32 isn't great with larger SD Cards and Windows
#define FF_FS_NORTC		1
#define FF_NORTC_MON	1
#define FF_NORTC_MDAY	1
#define FF_NORTC_YEAR	2020
#define FF_FS_NOFSINFO	0
#define FF_FS_LOCK		16
#define FF_FS_REENTRANT	0
#define FF_FS_TIMEOUT	1000
#define FF_SYNC_t		HANDLE
````

As you can see I use fast seek to speed up the seek operations. It is not a massive improvement but when time is at a premium every milli-seccond counts. To use fast seek you need to allocate a small buffer to the routine based on the cluster size of the file system. My basic maths showed this needed to be around 70 as the smallest FAT32 cluster size is 4096bytes (256MB–8GB) so 34 clusters for a MDR file tops (MDR size is 137923bytes, 137923/4096=33.67), the equation is then (34+1)*2=70. This is based on nobody using <256MB SD Card and when using >8GB the cluster size only gets larger which decreases the buffer size. exFAT cluster sizes are larger than FAT32, smallest being 4096bytes for <256MB, 32kB for <32GB and 128kB for >32GB.

The code used to set-up fast seek is as follows:

````
FIL fpRW;
DWORD clmt[70]; // cluster link map table buffer for fast seek
//
f_open(&fpRW,cartName,FA_READ|FA_WRITE);
fpRW.cltbl=clmt; // Enable fast seek mode (cltbl != NULL)
clmt[0]=SZ_TBL; // Set table size                     
f_lseek(&fpRW,CREATE_LINKMAP);
````

I also changed the `spi.h` in order to flash the correct LED when accessing the SD Card, changing it to pin 13 from 25.

Finally a `hw_config.c` file needs to be created (in the root folder) to specify the hardware configuration required. The following is an extract of the important parts:

````
#include <string.h>
#include "my_debug.h"
#include "hw_config.h"
#include "ff.h" /* Obtains integer types */
#include "diskio.h" /* Declarations of disk functions */

void spi0_dma_isr();

// Hardware Configuration of SPI "objects"
static spi_t spis[] = {  // One for each SPI.
    {
        .hw_inst = spi0,  // SPI component
        .miso_gpio = 16, // GPIO number (not pin number)
        .mosi_gpio = 19,
        .sck_gpio = 18,
        .set_drive_strength = true,
        .mosi_gpio_drive_strength = GPIO_DRIVE_STRENGTH_2MA,
        .sck_gpio_drive_strength = GPIO_DRIVE_STRENGTH_2MA,
        .baud_rate = 12500 * 1000,  // A safe value, some cards can go faster but have to cater for slower cards as well. Also don't need mega speeds
        .dma_isr = spi0_dma_isr
    }
};

// Hardware Configuration of the SD Card "objects"
static sd_card_t sd_cards[] = {  // One for each SD card
    {
        .pcName = "0:",           // Name used to mount device
        .spi = &spis[0],          // Pointer to the SPI driving this card
        .ss_gpio = 17,            // GPIO pin 17 for the SPI CS connection
        .set_drive_strength = true,
        .ss_gpio_drive_strength = GPIO_DRIVE_STRENGTH_2MA,
        .use_card_detect = true, // The Micro SD Card reader I use has this feature, so this is turned on
        .card_detect_gpio = 22,   // Card detect GPIO
        .card_detected_true = 1, // What the GPIO read returns when a card is present. Use -1 if there is no card detect
        .m_Status = STA_NOINIT
    }
};

void spi0_dma_isr() { spi_irq_handler(&spis[0]); }
size_t sd_get_num() { return count_of(sd_cards); }
sd_card_t *sd_get_by_num(size_t num) {
    if (num <= sd_get_num()) {
        return &sd_cards[num];
    } else {
        return NULL;
    }
}
size_t spi_get_num() { return count_of(spis); }
spi_t *spi_get_by_num(size_t num) {
    if (num <= sd_get_num()) {
        return &spis[num];
    } else {
        return NULL;
    }
}
````

To use this file you need to add it to the Pico `CMakeLists.txt` file within the `add_executable` add `hw_config.c`. If you do put it in a directory make sure you add the path.
