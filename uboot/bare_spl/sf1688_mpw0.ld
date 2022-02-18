
/***********************************************************************
 *
 *  sim_SPRam.ld for interAptiv
 *
 * ######################################################################

Copyright (c) 2015, Imagination Technologies Limited and/or its affiliated group companies

All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are
permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of
conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list
of conditions and the following disclaimer in the documentation and/or other materials
provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be
used to endorse or promote products derived from this software without specific prior
written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/


OUTPUT_ARCH(mips)

/**** Start point ****/
ENTRY(__reset_vector)

SECTIONS
{
  _monitor_flash = 0xbc004000 ;

  /**** Init Code (executed from reset.) ****/

  .text_init 0xbc004000 :   /* Runs uncached (from 0x9fc00000) till I$ is initialized. */
  AT( _monitor_flash )      /* (Load address is start of Monitor Flash.) */

  {
    _ftext_init = ABSOLUTE(.) ;	/* Start of init code. */
    ./arch/mips/cpu/start.o(.text)		    /* Reset entry point		    */
    ./arch/mips/cpu/init_cp0.o(.text)
    ./arch/mips/cpu/copy_c2_ram.o(.text)
    . = ALIGN(8);
    _etext_init = ABSOLUTE(.);	/* End of init code. */
  } = 0

  /**** "C" Code and read-only data ****/
  _zap1 = _etext_init - _ftext_init + _monitor_flash;
  _start_rom_text = _zap1;

  .text_rom _start_rom_text :    /* Need to copy code and read-only data to ram... */
  AT( _zap1 )               /* (Load address is after init code in Monitor Flash.) */

  {
    _ftext_rom = ABSOLUTE(.) ;	/* Start of code and read-only data */
    *(.text)*(.text.*)
    *(.rodata)
    *(.rodata.*)
    . = ALIGN(8);
    _etext_rom = ABSOLUTE(.);	/* End of code and read-only data   */
  } = 0

  /**** Initialised data ****/
  _zap2 = _etext_rom - _ftext_rom + _zap1 ;
  _start_ram_text = _zap2;
  .data _start_ram_text :
  AT( _zap2 )               /* (Load address is after "C" code in Monitor Flash.) */
  {
    _fdata_ram = ABSOLUTE(.);	/* Start of initialised data	    */
    *(.data)
    *(.data.*)
    . = ALIGN(8);
    _gp = ABSOLUTE(. + 0x7ff0); /* Base of small data		    */
    *(.lit8)
    *(.lit4)
    *(.sdata)
    . = ALIGN(8);
    _edata_ram  = ABSOLUTE(.);	/* End of initialised data	    */
  }


  /**** Uninitialised data ****/
  _fbss = .;			/* Start of uninitialised data	    */

  .sbss :
  {
    *(.sbss)
    *(.scommon)
  }

  .bss :
  {
    *(.bss)
    *(COMMON)
  }
  . = ALIGN(8);
  _end = . ;		/* End of uninitialised data	    */

  _heap_start = .;

  .gptab.sdata : { *(.gptab.data) *(.gptab.sdata) }
  .gptab.sbss : { *(.gptab.bss) *(.gptab.sbss) }

  .init : { KEEP (*(.init)) }
  .fini : { KEEP (*(.fini)) }

  .eh_frame : { KEEP (*(.eh_frame)) }

  /DISCARD/ :
  {
    *(.reginfo)
    *(.MIPS.abiflags)
  }
  _freemem = .;
}
