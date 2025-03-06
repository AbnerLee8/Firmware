#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif
endif

# Environment
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/left_samd51n20a_bootloader.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/left_samd51n20a_bootloader.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=-mafrlcsj
else
COMPARISON_BUILD=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=../src/config/default/bootloader/bootloader_serial_memory.c ../src/config/default/bootloader/bootloader_common.c ../src/config/default/driver/memory/src/drv_memory.c ../src/config/default/driver/memory/src/drv_memory_file_system.c ../src/config/default/driver/mx25l/src/drv_mx25l.c ../src/config/default/peripheral/clock/plib_clock.c ../src/config/default/peripheral/cmcc/plib_cmcc.c ../src/config/default/peripheral/evsys/plib_evsys.c ../src/config/default/peripheral/nvic/plib_nvic.c ../src/config/default/peripheral/nvmctrl/plib_nvmctrl.c ../src/config/default/peripheral/port/plib_port.c ../src/config/default/peripheral/qspi/plib_qspi.c ../src/config/default/stdio/xc32_monitor.c ../src/config/default/system/cache/sys_cache.c ../src/config/default/system/fs/fat_fs/file_system/ffunicode.c ../src/config/default/system/fs/fat_fs/file_system/ff.c ../src/config/default/system/fs/fat_fs/hardware_access/diskio.c ../src/config/default/system/fs/src/sys_fs.c ../src/config/default/system/fs/src/sys_fs_media_manager.c ../src/config/default/system/fs/src/sys_fs_fat_interface.c ../src/config/default/system/int/src/sys_int.c ../src/config/default/tasks.c ../src/config/default/initialization.c ../src/config/default/libc_syscalls.c ../src/config/default/interrupts.c ../src/config/default/exceptions.c ../src/config/default/startup_xc32.c ../src/main.c ../src/bootloader_app.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/_ext/1748409734/bootloader_serial_memory.o ${OBJECTDIR}/_ext/1748409734/bootloader_common.o ${OBJECTDIR}/_ext/992804092/drv_memory.o ${OBJECTDIR}/_ext/992804092/drv_memory_file_system.o ${OBJECTDIR}/_ext/312275821/drv_mx25l.o ${OBJECTDIR}/_ext/1984496892/plib_clock.o ${OBJECTDIR}/_ext/1865131932/plib_cmcc.o ${OBJECTDIR}/_ext/1986646378/plib_evsys.o ${OBJECTDIR}/_ext/1865468468/plib_nvic.o ${OBJECTDIR}/_ext/1593096446/plib_nvmctrl.o ${OBJECTDIR}/_ext/1865521619/plib_port.o ${OBJECTDIR}/_ext/1865555181/plib_qspi.o ${OBJECTDIR}/_ext/163028504/xc32_monitor.o ${OBJECTDIR}/_ext/1014039709/sys_cache.o ${OBJECTDIR}/_ext/411819097/ffunicode.o ${OBJECTDIR}/_ext/411819097/ff.o ${OBJECTDIR}/_ext/565198302/diskio.o ${OBJECTDIR}/_ext/1269487135/sys_fs.o ${OBJECTDIR}/_ext/1269487135/sys_fs_media_manager.o ${OBJECTDIR}/_ext/1269487135/sys_fs_fat_interface.o ${OBJECTDIR}/_ext/1881668453/sys_int.o ${OBJECTDIR}/_ext/1171490990/tasks.o ${OBJECTDIR}/_ext/1171490990/initialization.o ${OBJECTDIR}/_ext/1171490990/libc_syscalls.o ${OBJECTDIR}/_ext/1171490990/interrupts.o ${OBJECTDIR}/_ext/1171490990/exceptions.o ${OBJECTDIR}/_ext/1171490990/startup_xc32.o ${OBJECTDIR}/_ext/1360937237/main.o ${OBJECTDIR}/_ext/1360937237/bootloader_app.o
POSSIBLE_DEPFILES=${OBJECTDIR}/_ext/1748409734/bootloader_serial_memory.o.d ${OBJECTDIR}/_ext/1748409734/bootloader_common.o.d ${OBJECTDIR}/_ext/992804092/drv_memory.o.d ${OBJECTDIR}/_ext/992804092/drv_memory_file_system.o.d ${OBJECTDIR}/_ext/312275821/drv_mx25l.o.d ${OBJECTDIR}/_ext/1984496892/plib_clock.o.d ${OBJECTDIR}/_ext/1865131932/plib_cmcc.o.d ${OBJECTDIR}/_ext/1986646378/plib_evsys.o.d ${OBJECTDIR}/_ext/1865468468/plib_nvic.o.d ${OBJECTDIR}/_ext/1593096446/plib_nvmctrl.o.d ${OBJECTDIR}/_ext/1865521619/plib_port.o.d ${OBJECTDIR}/_ext/1865555181/plib_qspi.o.d ${OBJECTDIR}/_ext/163028504/xc32_monitor.o.d ${OBJECTDIR}/_ext/1014039709/sys_cache.o.d ${OBJECTDIR}/_ext/411819097/ffunicode.o.d ${OBJECTDIR}/_ext/411819097/ff.o.d ${OBJECTDIR}/_ext/565198302/diskio.o.d ${OBJECTDIR}/_ext/1269487135/sys_fs.o.d ${OBJECTDIR}/_ext/1269487135/sys_fs_media_manager.o.d ${OBJECTDIR}/_ext/1269487135/sys_fs_fat_interface.o.d ${OBJECTDIR}/_ext/1881668453/sys_int.o.d ${OBJECTDIR}/_ext/1171490990/tasks.o.d ${OBJECTDIR}/_ext/1171490990/initialization.o.d ${OBJECTDIR}/_ext/1171490990/libc_syscalls.o.d ${OBJECTDIR}/_ext/1171490990/interrupts.o.d ${OBJECTDIR}/_ext/1171490990/exceptions.o.d ${OBJECTDIR}/_ext/1171490990/startup_xc32.o.d ${OBJECTDIR}/_ext/1360937237/main.o.d ${OBJECTDIR}/_ext/1360937237/bootloader_app.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/1748409734/bootloader_serial_memory.o ${OBJECTDIR}/_ext/1748409734/bootloader_common.o ${OBJECTDIR}/_ext/992804092/drv_memory.o ${OBJECTDIR}/_ext/992804092/drv_memory_file_system.o ${OBJECTDIR}/_ext/312275821/drv_mx25l.o ${OBJECTDIR}/_ext/1984496892/plib_clock.o ${OBJECTDIR}/_ext/1865131932/plib_cmcc.o ${OBJECTDIR}/_ext/1986646378/plib_evsys.o ${OBJECTDIR}/_ext/1865468468/plib_nvic.o ${OBJECTDIR}/_ext/1593096446/plib_nvmctrl.o ${OBJECTDIR}/_ext/1865521619/plib_port.o ${OBJECTDIR}/_ext/1865555181/plib_qspi.o ${OBJECTDIR}/_ext/163028504/xc32_monitor.o ${OBJECTDIR}/_ext/1014039709/sys_cache.o ${OBJECTDIR}/_ext/411819097/ffunicode.o ${OBJECTDIR}/_ext/411819097/ff.o ${OBJECTDIR}/_ext/565198302/diskio.o ${OBJECTDIR}/_ext/1269487135/sys_fs.o ${OBJECTDIR}/_ext/1269487135/sys_fs_media_manager.o ${OBJECTDIR}/_ext/1269487135/sys_fs_fat_interface.o ${OBJECTDIR}/_ext/1881668453/sys_int.o ${OBJECTDIR}/_ext/1171490990/tasks.o ${OBJECTDIR}/_ext/1171490990/initialization.o ${OBJECTDIR}/_ext/1171490990/libc_syscalls.o ${OBJECTDIR}/_ext/1171490990/interrupts.o ${OBJECTDIR}/_ext/1171490990/exceptions.o ${OBJECTDIR}/_ext/1171490990/startup_xc32.o ${OBJECTDIR}/_ext/1360937237/main.o ${OBJECTDIR}/_ext/1360937237/bootloader_app.o

# Source Files
SOURCEFILES=../src/config/default/bootloader/bootloader_serial_memory.c ../src/config/default/bootloader/bootloader_common.c ../src/config/default/driver/memory/src/drv_memory.c ../src/config/default/driver/memory/src/drv_memory_file_system.c ../src/config/default/driver/mx25l/src/drv_mx25l.c ../src/config/default/peripheral/clock/plib_clock.c ../src/config/default/peripheral/cmcc/plib_cmcc.c ../src/config/default/peripheral/evsys/plib_evsys.c ../src/config/default/peripheral/nvic/plib_nvic.c ../src/config/default/peripheral/nvmctrl/plib_nvmctrl.c ../src/config/default/peripheral/port/plib_port.c ../src/config/default/peripheral/qspi/plib_qspi.c ../src/config/default/stdio/xc32_monitor.c ../src/config/default/system/cache/sys_cache.c ../src/config/default/system/fs/fat_fs/file_system/ffunicode.c ../src/config/default/system/fs/fat_fs/file_system/ff.c ../src/config/default/system/fs/fat_fs/hardware_access/diskio.c ../src/config/default/system/fs/src/sys_fs.c ../src/config/default/system/fs/src/sys_fs_media_manager.c ../src/config/default/system/fs/src/sys_fs_fat_interface.c ../src/config/default/system/int/src/sys_int.c ../src/config/default/tasks.c ../src/config/default/initialization.c ../src/config/default/libc_syscalls.c ../src/config/default/interrupts.c ../src/config/default/exceptions.c ../src/config/default/startup_xc32.c ../src/main.c ../src/bootloader_app.c

# Pack Options 
PACK_COMMON_OPTIONS=-I "${CMSIS_DIR}/CMSIS/Core/Include"



CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-default.mk ${DISTDIR}/left_samd51n20a_bootloader.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=ATSAMD51N20A
MP_LINKER_FILE_OPTION=,--script="..\src\config\default\ATSAMD51N20A.ld"
# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assembleWithPreprocess
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/1748409734/bootloader_serial_memory.o: ../src/config/default/bootloader/bootloader_serial_memory.c  .generated_files/flags/default/877021e62943eb9e39a76277f52442c327fc38f6 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1748409734" 
	@${RM} ${OBJECTDIR}/_ext/1748409734/bootloader_serial_memory.o.d 
	@${RM} ${OBJECTDIR}/_ext/1748409734/bootloader_serial_memory.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O2 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -I"../src/packs/ATSAMD51N20A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1748409734/bootloader_serial_memory.o.d" -o ${OBJECTDIR}/_ext/1748409734/bootloader_serial_memory.o ../src/config/default/bootloader/bootloader_serial_memory.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}/samd51a" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/1748409734/bootloader_common.o: ../src/config/default/bootloader/bootloader_common.c  .generated_files/flags/default/866ed09da062da15b6f5d5438d4e89b39b5eefe1 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1748409734" 
	@${RM} ${OBJECTDIR}/_ext/1748409734/bootloader_common.o.d 
	@${RM} ${OBJECTDIR}/_ext/1748409734/bootloader_common.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O2 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -I"../src/packs/ATSAMD51N20A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1748409734/bootloader_common.o.d" -o ${OBJECTDIR}/_ext/1748409734/bootloader_common.o ../src/config/default/bootloader/bootloader_common.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}/samd51a" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/992804092/drv_memory.o: ../src/config/default/driver/memory/src/drv_memory.c  .generated_files/flags/default/8b4c04662b2c911491b76d4f9f33e46d0ce6fbb7 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/992804092" 
	@${RM} ${OBJECTDIR}/_ext/992804092/drv_memory.o.d 
	@${RM} ${OBJECTDIR}/_ext/992804092/drv_memory.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O2 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -I"../src/packs/ATSAMD51N20A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/992804092/drv_memory.o.d" -o ${OBJECTDIR}/_ext/992804092/drv_memory.o ../src/config/default/driver/memory/src/drv_memory.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}/samd51a" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/992804092/drv_memory_file_system.o: ../src/config/default/driver/memory/src/drv_memory_file_system.c  .generated_files/flags/default/fd94e0d5779189721924237711c62105fe10e351 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/992804092" 
	@${RM} ${OBJECTDIR}/_ext/992804092/drv_memory_file_system.o.d 
	@${RM} ${OBJECTDIR}/_ext/992804092/drv_memory_file_system.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O2 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -I"../src/packs/ATSAMD51N20A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/992804092/drv_memory_file_system.o.d" -o ${OBJECTDIR}/_ext/992804092/drv_memory_file_system.o ../src/config/default/driver/memory/src/drv_memory_file_system.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}/samd51a" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/312275821/drv_mx25l.o: ../src/config/default/driver/mx25l/src/drv_mx25l.c  .generated_files/flags/default/b83869ecfbf42b4f0e79d5550fd6188e8aa6909b .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/312275821" 
	@${RM} ${OBJECTDIR}/_ext/312275821/drv_mx25l.o.d 
	@${RM} ${OBJECTDIR}/_ext/312275821/drv_mx25l.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O2 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -I"../src/packs/ATSAMD51N20A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/312275821/drv_mx25l.o.d" -o ${OBJECTDIR}/_ext/312275821/drv_mx25l.o ../src/config/default/driver/mx25l/src/drv_mx25l.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}/samd51a" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/1984496892/plib_clock.o: ../src/config/default/peripheral/clock/plib_clock.c  .generated_files/flags/default/c352b954e9631fe6ba49e7fd3bf1b09afe0e5e5e .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1984496892" 
	@${RM} ${OBJECTDIR}/_ext/1984496892/plib_clock.o.d 
	@${RM} ${OBJECTDIR}/_ext/1984496892/plib_clock.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O2 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -I"../src/packs/ATSAMD51N20A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1984496892/plib_clock.o.d" -o ${OBJECTDIR}/_ext/1984496892/plib_clock.o ../src/config/default/peripheral/clock/plib_clock.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}/samd51a" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/1865131932/plib_cmcc.o: ../src/config/default/peripheral/cmcc/plib_cmcc.c  .generated_files/flags/default/4b24f27257718c2a1b79f4c8101c49e447d3ed35 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1865131932" 
	@${RM} ${OBJECTDIR}/_ext/1865131932/plib_cmcc.o.d 
	@${RM} ${OBJECTDIR}/_ext/1865131932/plib_cmcc.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O2 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -I"../src/packs/ATSAMD51N20A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1865131932/plib_cmcc.o.d" -o ${OBJECTDIR}/_ext/1865131932/plib_cmcc.o ../src/config/default/peripheral/cmcc/plib_cmcc.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}/samd51a" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/1986646378/plib_evsys.o: ../src/config/default/peripheral/evsys/plib_evsys.c  .generated_files/flags/default/7d6febeaacfb0164b0e7cbb338318100017270de .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1986646378" 
	@${RM} ${OBJECTDIR}/_ext/1986646378/plib_evsys.o.d 
	@${RM} ${OBJECTDIR}/_ext/1986646378/plib_evsys.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O2 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -I"../src/packs/ATSAMD51N20A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1986646378/plib_evsys.o.d" -o ${OBJECTDIR}/_ext/1986646378/plib_evsys.o ../src/config/default/peripheral/evsys/plib_evsys.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}/samd51a" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/1865468468/plib_nvic.o: ../src/config/default/peripheral/nvic/plib_nvic.c  .generated_files/flags/default/2d710cb570501a539a05d1858a88462ca853d963 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1865468468" 
	@${RM} ${OBJECTDIR}/_ext/1865468468/plib_nvic.o.d 
	@${RM} ${OBJECTDIR}/_ext/1865468468/plib_nvic.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O2 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -I"../src/packs/ATSAMD51N20A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1865468468/plib_nvic.o.d" -o ${OBJECTDIR}/_ext/1865468468/plib_nvic.o ../src/config/default/peripheral/nvic/plib_nvic.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}/samd51a" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/1593096446/plib_nvmctrl.o: ../src/config/default/peripheral/nvmctrl/plib_nvmctrl.c  .generated_files/flags/default/9a5585a4fe4896b3a73670a30a407dbfe8a01e29 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1593096446" 
	@${RM} ${OBJECTDIR}/_ext/1593096446/plib_nvmctrl.o.d 
	@${RM} ${OBJECTDIR}/_ext/1593096446/plib_nvmctrl.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O2 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -I"../src/packs/ATSAMD51N20A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1593096446/plib_nvmctrl.o.d" -o ${OBJECTDIR}/_ext/1593096446/plib_nvmctrl.o ../src/config/default/peripheral/nvmctrl/plib_nvmctrl.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}/samd51a" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/1865521619/plib_port.o: ../src/config/default/peripheral/port/plib_port.c  .generated_files/flags/default/f84f3f968d62fc81dc472b61776b718a3aefc9be .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1865521619" 
	@${RM} ${OBJECTDIR}/_ext/1865521619/plib_port.o.d 
	@${RM} ${OBJECTDIR}/_ext/1865521619/plib_port.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O2 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -I"../src/packs/ATSAMD51N20A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1865521619/plib_port.o.d" -o ${OBJECTDIR}/_ext/1865521619/plib_port.o ../src/config/default/peripheral/port/plib_port.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}/samd51a" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/1865555181/plib_qspi.o: ../src/config/default/peripheral/qspi/plib_qspi.c  .generated_files/flags/default/6d9e27f506bea38ce19cf6fc29f628911ad2e5a0 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1865555181" 
	@${RM} ${OBJECTDIR}/_ext/1865555181/plib_qspi.o.d 
	@${RM} ${OBJECTDIR}/_ext/1865555181/plib_qspi.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O2 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -I"../src/packs/ATSAMD51N20A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1865555181/plib_qspi.o.d" -o ${OBJECTDIR}/_ext/1865555181/plib_qspi.o ../src/config/default/peripheral/qspi/plib_qspi.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}/samd51a" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/163028504/xc32_monitor.o: ../src/config/default/stdio/xc32_monitor.c  .generated_files/flags/default/ac0f055044853c09d7d6258de6cb6b6dae94667c .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/163028504" 
	@${RM} ${OBJECTDIR}/_ext/163028504/xc32_monitor.o.d 
	@${RM} ${OBJECTDIR}/_ext/163028504/xc32_monitor.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O2 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -I"../src/packs/ATSAMD51N20A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/163028504/xc32_monitor.o.d" -o ${OBJECTDIR}/_ext/163028504/xc32_monitor.o ../src/config/default/stdio/xc32_monitor.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}/samd51a" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/1014039709/sys_cache.o: ../src/config/default/system/cache/sys_cache.c  .generated_files/flags/default/8dc8c914742e6bb801bc81e9095a38a7529c9f64 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1014039709" 
	@${RM} ${OBJECTDIR}/_ext/1014039709/sys_cache.o.d 
	@${RM} ${OBJECTDIR}/_ext/1014039709/sys_cache.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O2 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -I"../src/packs/ATSAMD51N20A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1014039709/sys_cache.o.d" -o ${OBJECTDIR}/_ext/1014039709/sys_cache.o ../src/config/default/system/cache/sys_cache.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}/samd51a" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/411819097/ffunicode.o: ../src/config/default/system/fs/fat_fs/file_system/ffunicode.c  .generated_files/flags/default/b7b2d0c739a05671a81b9317ad347e5d8207ad71 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/411819097" 
	@${RM} ${OBJECTDIR}/_ext/411819097/ffunicode.o.d 
	@${RM} ${OBJECTDIR}/_ext/411819097/ffunicode.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O2 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -I"../src/packs/ATSAMD51N20A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/411819097/ffunicode.o.d" -o ${OBJECTDIR}/_ext/411819097/ffunicode.o ../src/config/default/system/fs/fat_fs/file_system/ffunicode.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}/samd51a" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/411819097/ff.o: ../src/config/default/system/fs/fat_fs/file_system/ff.c  .generated_files/flags/default/f3987bfe0cd168f4d397ad7d108d2b605510f4ea .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/411819097" 
	@${RM} ${OBJECTDIR}/_ext/411819097/ff.o.d 
	@${RM} ${OBJECTDIR}/_ext/411819097/ff.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O2 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -I"../src/packs/ATSAMD51N20A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/411819097/ff.o.d" -o ${OBJECTDIR}/_ext/411819097/ff.o ../src/config/default/system/fs/fat_fs/file_system/ff.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}/samd51a" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/565198302/diskio.o: ../src/config/default/system/fs/fat_fs/hardware_access/diskio.c  .generated_files/flags/default/c6727caef4914c387b9e8eea609f66377d1c1a53 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/565198302" 
	@${RM} ${OBJECTDIR}/_ext/565198302/diskio.o.d 
	@${RM} ${OBJECTDIR}/_ext/565198302/diskio.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O2 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -I"../src/packs/ATSAMD51N20A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/565198302/diskio.o.d" -o ${OBJECTDIR}/_ext/565198302/diskio.o ../src/config/default/system/fs/fat_fs/hardware_access/diskio.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}/samd51a" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/1269487135/sys_fs.o: ../src/config/default/system/fs/src/sys_fs.c  .generated_files/flags/default/286235fb40bb261cf4774d166bbe654c4456de8d .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1269487135" 
	@${RM} ${OBJECTDIR}/_ext/1269487135/sys_fs.o.d 
	@${RM} ${OBJECTDIR}/_ext/1269487135/sys_fs.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O2 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -I"../src/packs/ATSAMD51N20A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1269487135/sys_fs.o.d" -o ${OBJECTDIR}/_ext/1269487135/sys_fs.o ../src/config/default/system/fs/src/sys_fs.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}/samd51a" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/1269487135/sys_fs_media_manager.o: ../src/config/default/system/fs/src/sys_fs_media_manager.c  .generated_files/flags/default/771fc60d79023df5184698634da7c0052b53fe8c .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1269487135" 
	@${RM} ${OBJECTDIR}/_ext/1269487135/sys_fs_media_manager.o.d 
	@${RM} ${OBJECTDIR}/_ext/1269487135/sys_fs_media_manager.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O2 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -I"../src/packs/ATSAMD51N20A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1269487135/sys_fs_media_manager.o.d" -o ${OBJECTDIR}/_ext/1269487135/sys_fs_media_manager.o ../src/config/default/system/fs/src/sys_fs_media_manager.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}/samd51a" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/1269487135/sys_fs_fat_interface.o: ../src/config/default/system/fs/src/sys_fs_fat_interface.c  .generated_files/flags/default/a0129ddf1f0db58d8d592be632131707c8ecb3dd .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1269487135" 
	@${RM} ${OBJECTDIR}/_ext/1269487135/sys_fs_fat_interface.o.d 
	@${RM} ${OBJECTDIR}/_ext/1269487135/sys_fs_fat_interface.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O2 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -I"../src/packs/ATSAMD51N20A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1269487135/sys_fs_fat_interface.o.d" -o ${OBJECTDIR}/_ext/1269487135/sys_fs_fat_interface.o ../src/config/default/system/fs/src/sys_fs_fat_interface.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}/samd51a" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/1881668453/sys_int.o: ../src/config/default/system/int/src/sys_int.c  .generated_files/flags/default/97d8bd2827cf20ef49bfde3b2ae03f39f2429337 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1881668453" 
	@${RM} ${OBJECTDIR}/_ext/1881668453/sys_int.o.d 
	@${RM} ${OBJECTDIR}/_ext/1881668453/sys_int.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O2 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -I"../src/packs/ATSAMD51N20A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1881668453/sys_int.o.d" -o ${OBJECTDIR}/_ext/1881668453/sys_int.o ../src/config/default/system/int/src/sys_int.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}/samd51a" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/1171490990/tasks.o: ../src/config/default/tasks.c  .generated_files/flags/default/6afb6e2d88185d568b202dda86f5853a732a185e .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1171490990" 
	@${RM} ${OBJECTDIR}/_ext/1171490990/tasks.o.d 
	@${RM} ${OBJECTDIR}/_ext/1171490990/tasks.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O2 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -I"../src/packs/ATSAMD51N20A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1171490990/tasks.o.d" -o ${OBJECTDIR}/_ext/1171490990/tasks.o ../src/config/default/tasks.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}/samd51a" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/1171490990/initialization.o: ../src/config/default/initialization.c  .generated_files/flags/default/ddc15a9681849d8f53fdf76113e4505de5455358 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1171490990" 
	@${RM} ${OBJECTDIR}/_ext/1171490990/initialization.o.d 
	@${RM} ${OBJECTDIR}/_ext/1171490990/initialization.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O2 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -I"../src/packs/ATSAMD51N20A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1171490990/initialization.o.d" -o ${OBJECTDIR}/_ext/1171490990/initialization.o ../src/config/default/initialization.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}/samd51a" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/1171490990/libc_syscalls.o: ../src/config/default/libc_syscalls.c  .generated_files/flags/default/2310fd782d0d925bfd86f3fe1a1601cfc52c2bdf .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1171490990" 
	@${RM} ${OBJECTDIR}/_ext/1171490990/libc_syscalls.o.d 
	@${RM} ${OBJECTDIR}/_ext/1171490990/libc_syscalls.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O2 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -I"../src/packs/ATSAMD51N20A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1171490990/libc_syscalls.o.d" -o ${OBJECTDIR}/_ext/1171490990/libc_syscalls.o ../src/config/default/libc_syscalls.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}/samd51a" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/1171490990/interrupts.o: ../src/config/default/interrupts.c  .generated_files/flags/default/5c86e379447bc9a6bffaee678d6e0e105d8532c6 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1171490990" 
	@${RM} ${OBJECTDIR}/_ext/1171490990/interrupts.o.d 
	@${RM} ${OBJECTDIR}/_ext/1171490990/interrupts.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O2 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -I"../src/packs/ATSAMD51N20A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1171490990/interrupts.o.d" -o ${OBJECTDIR}/_ext/1171490990/interrupts.o ../src/config/default/interrupts.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}/samd51a" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/1171490990/exceptions.o: ../src/config/default/exceptions.c  .generated_files/flags/default/6ad308937dfbbed30992bbdd7b6a896faddfa02f .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1171490990" 
	@${RM} ${OBJECTDIR}/_ext/1171490990/exceptions.o.d 
	@${RM} ${OBJECTDIR}/_ext/1171490990/exceptions.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O2 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -I"../src/packs/ATSAMD51N20A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1171490990/exceptions.o.d" -o ${OBJECTDIR}/_ext/1171490990/exceptions.o ../src/config/default/exceptions.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}/samd51a" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/1171490990/startup_xc32.o: ../src/config/default/startup_xc32.c  .generated_files/flags/default/45887be20614546ea6ce83671458540c2c7de262 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1171490990" 
	@${RM} ${OBJECTDIR}/_ext/1171490990/startup_xc32.o.d 
	@${RM} ${OBJECTDIR}/_ext/1171490990/startup_xc32.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O2 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -I"../src/packs/ATSAMD51N20A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1171490990/startup_xc32.o.d" -o ${OBJECTDIR}/_ext/1171490990/startup_xc32.o ../src/config/default/startup_xc32.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}/samd51a" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/1360937237/main.o: ../src/main.c  .generated_files/flags/default/6fd2e9ee33443fd558d244791469d74eb7174e4 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/main.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/main.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O2 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -I"../src/packs/ATSAMD51N20A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/main.o.d" -o ${OBJECTDIR}/_ext/1360937237/main.o ../src/main.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}/samd51a" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/1360937237/bootloader_app.o: ../src/bootloader_app.c  .generated_files/flags/default/368de1318f46fd75fb66201b4b304ed0a0733ea6 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/bootloader_app.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/bootloader_app.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O2 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -I"../src/packs/ATSAMD51N20A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/bootloader_app.o.d" -o ${OBJECTDIR}/_ext/1360937237/bootloader_app.o ../src/bootloader_app.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}/samd51a" ${PACK_COMMON_OPTIONS} 
	
else
${OBJECTDIR}/_ext/1748409734/bootloader_serial_memory.o: ../src/config/default/bootloader/bootloader_serial_memory.c  .generated_files/flags/default/2ea063cf25c8093d7bf1b0c8014676dfe674d33e .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1748409734" 
	@${RM} ${OBJECTDIR}/_ext/1748409734/bootloader_serial_memory.o.d 
	@${RM} ${OBJECTDIR}/_ext/1748409734/bootloader_serial_memory.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O2 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -I"../src/packs/ATSAMD51N20A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1748409734/bootloader_serial_memory.o.d" -o ${OBJECTDIR}/_ext/1748409734/bootloader_serial_memory.o ../src/config/default/bootloader/bootloader_serial_memory.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}/samd51a" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/1748409734/bootloader_common.o: ../src/config/default/bootloader/bootloader_common.c  .generated_files/flags/default/ddf9432dd53b8ccc20d3c2655248f9913d07a7ff .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1748409734" 
	@${RM} ${OBJECTDIR}/_ext/1748409734/bootloader_common.o.d 
	@${RM} ${OBJECTDIR}/_ext/1748409734/bootloader_common.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O2 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -I"../src/packs/ATSAMD51N20A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1748409734/bootloader_common.o.d" -o ${OBJECTDIR}/_ext/1748409734/bootloader_common.o ../src/config/default/bootloader/bootloader_common.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}/samd51a" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/992804092/drv_memory.o: ../src/config/default/driver/memory/src/drv_memory.c  .generated_files/flags/default/64ecfa478363439858990e067f9f6b5583962990 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/992804092" 
	@${RM} ${OBJECTDIR}/_ext/992804092/drv_memory.o.d 
	@${RM} ${OBJECTDIR}/_ext/992804092/drv_memory.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O2 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -I"../src/packs/ATSAMD51N20A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/992804092/drv_memory.o.d" -o ${OBJECTDIR}/_ext/992804092/drv_memory.o ../src/config/default/driver/memory/src/drv_memory.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}/samd51a" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/992804092/drv_memory_file_system.o: ../src/config/default/driver/memory/src/drv_memory_file_system.c  .generated_files/flags/default/8860689876fb28fe6217cd8c693ee763645b9644 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/992804092" 
	@${RM} ${OBJECTDIR}/_ext/992804092/drv_memory_file_system.o.d 
	@${RM} ${OBJECTDIR}/_ext/992804092/drv_memory_file_system.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O2 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -I"../src/packs/ATSAMD51N20A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/992804092/drv_memory_file_system.o.d" -o ${OBJECTDIR}/_ext/992804092/drv_memory_file_system.o ../src/config/default/driver/memory/src/drv_memory_file_system.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}/samd51a" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/312275821/drv_mx25l.o: ../src/config/default/driver/mx25l/src/drv_mx25l.c  .generated_files/flags/default/ec30bb29c2c6fb0a07194c5446be18ae2d573b55 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/312275821" 
	@${RM} ${OBJECTDIR}/_ext/312275821/drv_mx25l.o.d 
	@${RM} ${OBJECTDIR}/_ext/312275821/drv_mx25l.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O2 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -I"../src/packs/ATSAMD51N20A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/312275821/drv_mx25l.o.d" -o ${OBJECTDIR}/_ext/312275821/drv_mx25l.o ../src/config/default/driver/mx25l/src/drv_mx25l.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}/samd51a" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/1984496892/plib_clock.o: ../src/config/default/peripheral/clock/plib_clock.c  .generated_files/flags/default/f8304256ebb2c0c984523daeb1499c3d6724a8fc .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1984496892" 
	@${RM} ${OBJECTDIR}/_ext/1984496892/plib_clock.o.d 
	@${RM} ${OBJECTDIR}/_ext/1984496892/plib_clock.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O2 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -I"../src/packs/ATSAMD51N20A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1984496892/plib_clock.o.d" -o ${OBJECTDIR}/_ext/1984496892/plib_clock.o ../src/config/default/peripheral/clock/plib_clock.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}/samd51a" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/1865131932/plib_cmcc.o: ../src/config/default/peripheral/cmcc/plib_cmcc.c  .generated_files/flags/default/8a20bd965de6db491f37413d45691d80899a2b0e .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1865131932" 
	@${RM} ${OBJECTDIR}/_ext/1865131932/plib_cmcc.o.d 
	@${RM} ${OBJECTDIR}/_ext/1865131932/plib_cmcc.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O2 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -I"../src/packs/ATSAMD51N20A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1865131932/plib_cmcc.o.d" -o ${OBJECTDIR}/_ext/1865131932/plib_cmcc.o ../src/config/default/peripheral/cmcc/plib_cmcc.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}/samd51a" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/1986646378/plib_evsys.o: ../src/config/default/peripheral/evsys/plib_evsys.c  .generated_files/flags/default/a9486f3e4ef409273c2c51545ce3de50c2869b16 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1986646378" 
	@${RM} ${OBJECTDIR}/_ext/1986646378/plib_evsys.o.d 
	@${RM} ${OBJECTDIR}/_ext/1986646378/plib_evsys.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O2 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -I"../src/packs/ATSAMD51N20A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1986646378/plib_evsys.o.d" -o ${OBJECTDIR}/_ext/1986646378/plib_evsys.o ../src/config/default/peripheral/evsys/plib_evsys.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}/samd51a" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/1865468468/plib_nvic.o: ../src/config/default/peripheral/nvic/plib_nvic.c  .generated_files/flags/default/a59953bdf45502dfacf064d7477c66a42c2cbbff .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1865468468" 
	@${RM} ${OBJECTDIR}/_ext/1865468468/plib_nvic.o.d 
	@${RM} ${OBJECTDIR}/_ext/1865468468/plib_nvic.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O2 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -I"../src/packs/ATSAMD51N20A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1865468468/plib_nvic.o.d" -o ${OBJECTDIR}/_ext/1865468468/plib_nvic.o ../src/config/default/peripheral/nvic/plib_nvic.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}/samd51a" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/1593096446/plib_nvmctrl.o: ../src/config/default/peripheral/nvmctrl/plib_nvmctrl.c  .generated_files/flags/default/ca3e118bb70257701833e95acd16b2294bc7e396 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1593096446" 
	@${RM} ${OBJECTDIR}/_ext/1593096446/plib_nvmctrl.o.d 
	@${RM} ${OBJECTDIR}/_ext/1593096446/plib_nvmctrl.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O2 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -I"../src/packs/ATSAMD51N20A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1593096446/plib_nvmctrl.o.d" -o ${OBJECTDIR}/_ext/1593096446/plib_nvmctrl.o ../src/config/default/peripheral/nvmctrl/plib_nvmctrl.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}/samd51a" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/1865521619/plib_port.o: ../src/config/default/peripheral/port/plib_port.c  .generated_files/flags/default/92feb7051ee131087a2a8658999d61caa2712cac .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1865521619" 
	@${RM} ${OBJECTDIR}/_ext/1865521619/plib_port.o.d 
	@${RM} ${OBJECTDIR}/_ext/1865521619/plib_port.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O2 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -I"../src/packs/ATSAMD51N20A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1865521619/plib_port.o.d" -o ${OBJECTDIR}/_ext/1865521619/plib_port.o ../src/config/default/peripheral/port/plib_port.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}/samd51a" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/1865555181/plib_qspi.o: ../src/config/default/peripheral/qspi/plib_qspi.c  .generated_files/flags/default/164cd7b98aebff900bf673697139715de7958a00 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1865555181" 
	@${RM} ${OBJECTDIR}/_ext/1865555181/plib_qspi.o.d 
	@${RM} ${OBJECTDIR}/_ext/1865555181/plib_qspi.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O2 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -I"../src/packs/ATSAMD51N20A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1865555181/plib_qspi.o.d" -o ${OBJECTDIR}/_ext/1865555181/plib_qspi.o ../src/config/default/peripheral/qspi/plib_qspi.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}/samd51a" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/163028504/xc32_monitor.o: ../src/config/default/stdio/xc32_monitor.c  .generated_files/flags/default/651fd676c592d11798e7c75f6aa6d425f29360d2 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/163028504" 
	@${RM} ${OBJECTDIR}/_ext/163028504/xc32_monitor.o.d 
	@${RM} ${OBJECTDIR}/_ext/163028504/xc32_monitor.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O2 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -I"../src/packs/ATSAMD51N20A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/163028504/xc32_monitor.o.d" -o ${OBJECTDIR}/_ext/163028504/xc32_monitor.o ../src/config/default/stdio/xc32_monitor.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}/samd51a" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/1014039709/sys_cache.o: ../src/config/default/system/cache/sys_cache.c  .generated_files/flags/default/17c62cae8ec4da898715c53fb403a409823b96c0 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1014039709" 
	@${RM} ${OBJECTDIR}/_ext/1014039709/sys_cache.o.d 
	@${RM} ${OBJECTDIR}/_ext/1014039709/sys_cache.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O2 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -I"../src/packs/ATSAMD51N20A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1014039709/sys_cache.o.d" -o ${OBJECTDIR}/_ext/1014039709/sys_cache.o ../src/config/default/system/cache/sys_cache.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}/samd51a" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/411819097/ffunicode.o: ../src/config/default/system/fs/fat_fs/file_system/ffunicode.c  .generated_files/flags/default/30b935302f91e7c06081c37cfe720c75a7b28530 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/411819097" 
	@${RM} ${OBJECTDIR}/_ext/411819097/ffunicode.o.d 
	@${RM} ${OBJECTDIR}/_ext/411819097/ffunicode.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O2 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -I"../src/packs/ATSAMD51N20A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/411819097/ffunicode.o.d" -o ${OBJECTDIR}/_ext/411819097/ffunicode.o ../src/config/default/system/fs/fat_fs/file_system/ffunicode.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}/samd51a" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/411819097/ff.o: ../src/config/default/system/fs/fat_fs/file_system/ff.c  .generated_files/flags/default/861e03594a58fe1c738ccd7dbca8e82b0decf73a .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/411819097" 
	@${RM} ${OBJECTDIR}/_ext/411819097/ff.o.d 
	@${RM} ${OBJECTDIR}/_ext/411819097/ff.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O2 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -I"../src/packs/ATSAMD51N20A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/411819097/ff.o.d" -o ${OBJECTDIR}/_ext/411819097/ff.o ../src/config/default/system/fs/fat_fs/file_system/ff.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}/samd51a" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/565198302/diskio.o: ../src/config/default/system/fs/fat_fs/hardware_access/diskio.c  .generated_files/flags/default/1baf4e1fca9e37892dd4830fb8080405140211b7 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/565198302" 
	@${RM} ${OBJECTDIR}/_ext/565198302/diskio.o.d 
	@${RM} ${OBJECTDIR}/_ext/565198302/diskio.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O2 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -I"../src/packs/ATSAMD51N20A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/565198302/diskio.o.d" -o ${OBJECTDIR}/_ext/565198302/diskio.o ../src/config/default/system/fs/fat_fs/hardware_access/diskio.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}/samd51a" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/1269487135/sys_fs.o: ../src/config/default/system/fs/src/sys_fs.c  .generated_files/flags/default/9979c35ee227de079fa71576eb26fb7ba9afe283 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1269487135" 
	@${RM} ${OBJECTDIR}/_ext/1269487135/sys_fs.o.d 
	@${RM} ${OBJECTDIR}/_ext/1269487135/sys_fs.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O2 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -I"../src/packs/ATSAMD51N20A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1269487135/sys_fs.o.d" -o ${OBJECTDIR}/_ext/1269487135/sys_fs.o ../src/config/default/system/fs/src/sys_fs.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}/samd51a" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/1269487135/sys_fs_media_manager.o: ../src/config/default/system/fs/src/sys_fs_media_manager.c  .generated_files/flags/default/ff5d88e33724e944dcc0d5276f64fddcefa03489 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1269487135" 
	@${RM} ${OBJECTDIR}/_ext/1269487135/sys_fs_media_manager.o.d 
	@${RM} ${OBJECTDIR}/_ext/1269487135/sys_fs_media_manager.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O2 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -I"../src/packs/ATSAMD51N20A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1269487135/sys_fs_media_manager.o.d" -o ${OBJECTDIR}/_ext/1269487135/sys_fs_media_manager.o ../src/config/default/system/fs/src/sys_fs_media_manager.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}/samd51a" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/1269487135/sys_fs_fat_interface.o: ../src/config/default/system/fs/src/sys_fs_fat_interface.c  .generated_files/flags/default/6c135bfe445fd5bdd4700064707ffe92e739fa82 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1269487135" 
	@${RM} ${OBJECTDIR}/_ext/1269487135/sys_fs_fat_interface.o.d 
	@${RM} ${OBJECTDIR}/_ext/1269487135/sys_fs_fat_interface.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O2 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -I"../src/packs/ATSAMD51N20A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1269487135/sys_fs_fat_interface.o.d" -o ${OBJECTDIR}/_ext/1269487135/sys_fs_fat_interface.o ../src/config/default/system/fs/src/sys_fs_fat_interface.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}/samd51a" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/1881668453/sys_int.o: ../src/config/default/system/int/src/sys_int.c  .generated_files/flags/default/523d914105dff795b017ac7a60897c9444e43e0b .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1881668453" 
	@${RM} ${OBJECTDIR}/_ext/1881668453/sys_int.o.d 
	@${RM} ${OBJECTDIR}/_ext/1881668453/sys_int.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O2 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -I"../src/packs/ATSAMD51N20A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1881668453/sys_int.o.d" -o ${OBJECTDIR}/_ext/1881668453/sys_int.o ../src/config/default/system/int/src/sys_int.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}/samd51a" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/1171490990/tasks.o: ../src/config/default/tasks.c  .generated_files/flags/default/cc2031e027a058a6094cf5e40678cf2f98f2b8d9 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1171490990" 
	@${RM} ${OBJECTDIR}/_ext/1171490990/tasks.o.d 
	@${RM} ${OBJECTDIR}/_ext/1171490990/tasks.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O2 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -I"../src/packs/ATSAMD51N20A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1171490990/tasks.o.d" -o ${OBJECTDIR}/_ext/1171490990/tasks.o ../src/config/default/tasks.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}/samd51a" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/1171490990/initialization.o: ../src/config/default/initialization.c  .generated_files/flags/default/c8a8d01f98307a16b69d1ee3d9c1527bb37678ec .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1171490990" 
	@${RM} ${OBJECTDIR}/_ext/1171490990/initialization.o.d 
	@${RM} ${OBJECTDIR}/_ext/1171490990/initialization.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O2 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -I"../src/packs/ATSAMD51N20A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1171490990/initialization.o.d" -o ${OBJECTDIR}/_ext/1171490990/initialization.o ../src/config/default/initialization.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}/samd51a" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/1171490990/libc_syscalls.o: ../src/config/default/libc_syscalls.c  .generated_files/flags/default/d2c3ac38b575e6e98559554a1af3612edf6b9518 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1171490990" 
	@${RM} ${OBJECTDIR}/_ext/1171490990/libc_syscalls.o.d 
	@${RM} ${OBJECTDIR}/_ext/1171490990/libc_syscalls.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O2 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -I"../src/packs/ATSAMD51N20A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1171490990/libc_syscalls.o.d" -o ${OBJECTDIR}/_ext/1171490990/libc_syscalls.o ../src/config/default/libc_syscalls.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}/samd51a" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/1171490990/interrupts.o: ../src/config/default/interrupts.c  .generated_files/flags/default/5a143a8bdde52660ea7172f6013b65534ca9db33 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1171490990" 
	@${RM} ${OBJECTDIR}/_ext/1171490990/interrupts.o.d 
	@${RM} ${OBJECTDIR}/_ext/1171490990/interrupts.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O2 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -I"../src/packs/ATSAMD51N20A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1171490990/interrupts.o.d" -o ${OBJECTDIR}/_ext/1171490990/interrupts.o ../src/config/default/interrupts.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}/samd51a" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/1171490990/exceptions.o: ../src/config/default/exceptions.c  .generated_files/flags/default/37a2c0f0f6267e01ae0cf389b5a90bfeccfb56b5 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1171490990" 
	@${RM} ${OBJECTDIR}/_ext/1171490990/exceptions.o.d 
	@${RM} ${OBJECTDIR}/_ext/1171490990/exceptions.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O2 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -I"../src/packs/ATSAMD51N20A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1171490990/exceptions.o.d" -o ${OBJECTDIR}/_ext/1171490990/exceptions.o ../src/config/default/exceptions.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}/samd51a" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/1171490990/startup_xc32.o: ../src/config/default/startup_xc32.c  .generated_files/flags/default/e24d00a1d079db1e0ee80d8d2cdd5a183a90bbb6 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1171490990" 
	@${RM} ${OBJECTDIR}/_ext/1171490990/startup_xc32.o.d 
	@${RM} ${OBJECTDIR}/_ext/1171490990/startup_xc32.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O2 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -I"../src/packs/ATSAMD51N20A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1171490990/startup_xc32.o.d" -o ${OBJECTDIR}/_ext/1171490990/startup_xc32.o ../src/config/default/startup_xc32.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}/samd51a" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/1360937237/main.o: ../src/main.c  .generated_files/flags/default/de8c653717937a7f53cecbc62f2822353609bd14 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/main.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/main.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O2 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -I"../src/packs/ATSAMD51N20A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/main.o.d" -o ${OBJECTDIR}/_ext/1360937237/main.o ../src/main.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}/samd51a" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/1360937237/bootloader_app.o: ../src/bootloader_app.c  .generated_files/flags/default/be98493755223f1dd1c5e9e71ab2620adc7a149f .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/bootloader_app.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/bootloader_app.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O2 -fno-common -I"../src" -I"../src/config/default" -I"../src/config/default/system/fs/fat_fs/file_system" -I"../src/config/default/system/fs/fat_fs/hardware_access" -I"../src/packs/ATSAMD51N20A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/bootloader_app.o.d" -o ${OBJECTDIR}/_ext/1360937237/bootloader_app.o ../src/bootloader_app.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}/samd51a" ${PACK_COMMON_OPTIONS} 
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compileCPP
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${DISTDIR}/left_samd51n20a_bootloader.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    ../src/config/default/ATSAMD51N20A.ld
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -g   -mprocessor=$(MP_PROCESSOR_OPTION)  -mno-device-startup-code -o ${DISTDIR}/left_samd51n20a_bootloader.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,-D=__DEBUG_D,--defsym=_min_heap_size=512,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,-DRAM_LENGTH=0x3fff0,-DRAM_ORIGIN=0x20000010,-DROM_LENGTH=0x6000,--memorysummary,${DISTDIR}/memoryfile.xml -mdfp="${DFP_DIR}/samd51a"
	
else
${DISTDIR}/left_samd51n20a_bootloader.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   ../src/config/default/ATSAMD51N20A.ld
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mprocessor=$(MP_PROCESSOR_OPTION)  -mno-device-startup-code -o ${DISTDIR}/left_samd51n20a_bootloader.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=_min_heap_size=512,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,-DRAM_LENGTH=0x3fff0,-DRAM_ORIGIN=0x20000010,-DROM_LENGTH=0x6000,--memorysummary,${DISTDIR}/memoryfile.xml -mdfp="${DFP_DIR}/samd51a"
	${MP_CC_DIR}\\xc32-bin2hex ${DISTDIR}/left_samd51n20a_bootloader.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} 
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${OBJECTDIR}
	${RM} -r ${DISTDIR}

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(wildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
