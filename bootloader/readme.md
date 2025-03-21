# Project Configuration for left side MCU.
	## Configuration:
		### QSPI
			- QSPI works at Baud rate 1 MHz.
			- Operating mode as "Serial Memory"
	    ### MX25L	
			- All are default configuration
		### Memory	
			- All are default configuration
		    #### Instance 0
				- All are default configuration
		### File system
			- Media type : SPI Flash
			- File system : FAT
		### File system bootloader
			- Bootloader Media type : Serial Memory
			- Bootloader size : 49152
		### Pin Configuration
			- PA08 - QSPI_DATA0
			- PA09 - QSPI_DATA1
			- PA10 - QSPI_DATA2
			- PA11 - QSPI_DATA3
			- PB10 - QSPI_SCK
			- PB11 - QSPI_CS
	## Component Connection:
		- QSPI component connected with MX25L driver as SQI connection
		- MX25L driver component with Memory component as MEMORY connection
		- Memory driver component with File system component as DRV_MEDIA
		- NVMCTRL component with File system bootloader component as MEMORY(NVM)
	## Linker File configuration
		### ROM ( Flash )
			- For bootloader ROM is allocated as 48 KB. Application should start from after the 0xC000 address.
				- ROM_ORIGIN : 0x0
				- ROM_LENGTH : 0xC000
		### RAM
			- For bootloader first 16 byte, is used to store the signature value by application to inform the bootloader that 
			  new application binary file are available into the external flash memory.
			- Same 16 byte should be allocated from to write the signature at application side as well.