ENTRY(entry)

SECTIONS
{
	. = 0x00100000;
	code = .;
	
	.text  . : { 
		*(.text*) 	
	}
	
	.rdata . :
    {   *(.rdata*)    }
	. = ALIGN(4096);
	
	
	. = ALIGN(4096);
	data = .;
	.data . :  { 
		*(.*data*)  
		
	}
	
	bss = .;
	.bss :  {  
		*(COMMON)
		*(.bss*)   
		. = ALIGN(4096);
		*(.stack)
	}
end = .; _end = .; __end = .;

/DISCARD/ :{
                *(.note*)
                *(.indent)
                *(.comment)
                *(.stab)
                *(.stabstr)
        }
}
