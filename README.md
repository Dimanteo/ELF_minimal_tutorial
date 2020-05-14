# ELF files generation tutorial  
This example was written on Linux x86_64.  
To generate file use C standard header `<elf.h>`. Executable requires two headers **ELF format header**, represented by structure `Elf64_Ehdr` and **program header** represented by `Elf64_Phdr`. Valid field values defined in `<elf.h>` like this: 

```C
/* Legal values for e_type (object file type).  */

#define ET_NONE   0		/* No file type */
#define ET_REL    1		/* Relocatable file */
#define ET_EXEC   2		/* Executable file */
#define ET_DYN    3		/* Shared object file */
#define ET_CORE   4		/* Core file */
#define ET_NUM    5		/* Number of defined types */
#define ET_LOOS   0xfe00		/* OS-specific range start */
#define ET_HIOS   0xfeff		/* OS-specific range end */
#define ET_LOPROC 0xff00		/* Processor-specific range start */
#define ET_HIPROC 0xffff		/* Processor-specific range end */
```  

More detailed information about each field can be found in [MAN](https://linux.die.net/man/5/elf) or [here](https://cirosantilli.com/elf-hello-world#elf-header).  
However, some fields haven't got predefined values, so their values need to be received with ELF reading tools.
### Tools 
* Shell command `readelf`. Key `-h` prints info about ELF header. `-l` prints info about program header. Also useful to check, if your generated header is correct.  
* Shell command `strip` removes unnecessary information from ELF file.
* Radare - super powerfull tool for file analysis. Can be installed from [Repository](https://github.com/radareorg/radare2) and learned in [Book](https://radare.gitbooks.io/radare2book/content/). Here on brief overview of basic functions.  
`r2 -n <file>` - open file, align it to zero.  
`px` - hex dump  
`pd` - print disasm  
`s` - seek  
`VV` - enables visual view
## So how do I make executable elf
Lets write simple programm that prints `Hello` on NASM. Note that if use GCC, there will be a lot of unnecessary (for minimal example) information.

```
global _start

section .text

_start:         nop
                nop
                nop
                sub rsp, 6
                mov byte [rsp], 'H'
                mov byte [rsp + 1], 'e'
                mov byte [rsp + 2], 'l'
                mov byte [rsp + 3], 'l'
                mov byte [rsp + 4], 'o'
                mov byte [rsp + 5], 0x0A    ; LF
                mov rax, 1                  ; syscall Write
                mov rdi, 1
                mov rsi, rsp
                mov rdx, 6
                syscall
                mov rax, 0x3C               ; syscall Exit
                mov rdi, 0
                syscall
                nop
                nop
                nop
```

The code is surrounded by nop instructions to make it easier to find it in HEX dump. After compilation and linking we get executable. `strip` it and then use `readelf -h`. You will see information about each field of ELF header. This is how you can get values to fill your own header.

```
ELF Header:
  Magic:   7f 45 4c 46 02 01 01 00 00 00 00 00 00 00 00 00 
  Class:                             ELF64
  Data:                              2's complement, little endian
  Version:                           1 (current)
  OS/ABI:                            UNIX - System V
  ABI Version:                       0
  Type:                              EXEC (Executable file)
  Machine:                           Advanced Micro Devices X86-64
  Version:                           0x1
  Entry point address:               0x400080
  Start of program headers:          64 (bytes into file)
  Start of section headers:          216 (bytes into file)
  Flags:                             0x0
  Size of this header:               64 (bytes)
  Size of program headers:           56 (bytes)
  Number of program headers:         1
  Size of section headers:           64 (bytes)
  Number of section headers:         3
  Section header string table index: 2
```

 The same way you can get values to fill program header. Use `readelf -l`.

 ```
 Elf file type is EXEC (Executable file)
Entry point 0x400080
There is 1 program header, starting at offset 64

Program Headers:
  Type           Offset             VirtAddr           PhysAddr
                 FileSiz            MemSiz              Flags  Align
  LOAD           0x0000000000000000 0x0000000000400000 0x0000000000400000
                 0x00000000000000c7 0x00000000000000c7  R E    0x200000

 Section to Segment mapping:
  Segment Sections...
   00     .text 
 ```

 Note tha FileSiz and MemSize are calculated from size of your programm, don't copy them.  

 Now we have all values we need. Next step is to make C program, where fill structures and write them to file. After those headers we should put program that we want to execute.  

 **Note that program body must start from adress `0x80`.**  
 
 In the example program body is surrounded by bytes `909090` - three nop instructions.  
 Hex dump of generated file:   

```
00000000: 7f45 4c46 0201 0100 0900 0000 0000 0000  .ELF............
00000010: 0200 3e00 0100 0000 8000 4000 0000 0000  ..>.......@.....
00000020: 4000 0000 0000 0000 0000 0000 0000 0000  @...............
00000030: 0000 0000 4000 3800 0100 0000 0000 0000  ....@.8.........
00000040: 0100 0000 0500 0000 0000 0000 0000 0000  ................
00000050: 0000 4000 0000 0000 0000 4000 0000 0000  ..@.......@.....
00000060: c700 0000 0000 0000 c700 0000 0000 0000  ................
00000070: 0000 2000 0000 0000 0000 0000 0000 0000  .. .............
00000080: 9090 9048 83ec 06c6 0424 48c6 4424 0165  ...H.....$H.D$.e
00000090: c644 2402 6cc6 4424 036c c644 2404 6fc6  .D$.l.D$.l.D$.o.
000000a0: 4424 050a b801 0000 00bf 0100 0000 4889  D$............H.
000000b0: e6ba 0600 0000 0f05 b83c 0000 00bf 0000  .........<......
000000c0: 0000 0f05 9090 90                        .......
```  

 Example of program that generates executable is in file elfmaker.cpp. After you created file give it execution permission via `chmod +x <file>` and it's done.

### Resources:

Man page: https://linux.die.net/man/5/elf  
ELF detailed tutorial: https://cirosantilli.com/elf-hello-world#elf-header  
Radare repository: https://github.com/radareorg/radare2  
Radare book: https://radare.gitbooks.io/radare2book/content/