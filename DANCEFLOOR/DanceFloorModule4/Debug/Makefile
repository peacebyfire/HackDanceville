################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL := cmd.exe
RM := rm -rf

USER_OBJS :=

LIBS := 
PROJ := 

O_SRCS := 
C_SRCS := 
S_SRCS := 
S_UPPER_SRCS := 
OBJ_SRCS := 
ASM_SRCS := 
PREPROCESSING_SRCS := 
OBJS := 
OBJS_AS_ARGS := 
C_DEPS := 
C_DEPS_AS_ARGS := 
EXECUTABLES := 
OUTPUT_FILE_PATH :=
OUTPUT_FILE_PATH_AS_ARGS :=
AVR_APP_PATH :=$$$AVR_APP_PATH$$$
QUOTE := "
ADDITIONAL_DEPENDENCIES:=
OUTPUT_FILE_DEP:=

# Every subdirectory with source files must be described here
SUBDIRS :=  \
../eth


# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS +=  \
../DanceFloorModule3.c \
../delay.c \
../df.c \
../dfcontrol.c \
../eth/arp.c \
../eth/dhcp.c \
../eth/enc28j60.c \
../eth/eth.c \
../eth/icmp.c \
../eth/in.c \
../eth/ipv4.c \
../eth/net.c \
../eth/packetType.c \
../eth/udp.c \
../idlePattern.c \
../systime.c \
../temp.c \
../tlc5941.c


PREPROCESSING_SRCS += 


ASM_SRCS += 


OBJS +=  \
DanceFloorModule3.o \
delay.o \
df.o \
dfcontrol.o \
eth/arp.o \
eth/dhcp.o \
eth/enc28j60.o \
eth/eth.o \
eth/icmp.o \
eth/in.o \
eth/ipv4.o \
eth/net.o \
eth/packetType.o \
eth/udp.o \
idlePattern.o \
systime.o \
temp.o \
tlc5941.o


OBJS_AS_ARGS +=  \
DanceFloorModule3.o \
delay.o \
df.o \
dfcontrol.o \
eth/arp.o \
eth/dhcp.o \
eth/enc28j60.o \
eth/eth.o \
eth/icmp.o \
eth/in.o \
eth/ipv4.o \
eth/net.o \
eth/packetType.o \
eth/udp.o \
idlePattern.o \
systime.o \
temp.o \
tlc5941.o


C_DEPS +=  \
DanceFloorModule3.d \
delay.d \
df.d \
dfcontrol.d \
eth/arp.d \
eth/dhcp.d \
eth/enc28j60.d \
eth/eth.d \
eth/icmp.d \
eth/in.d \
eth/ipv4.d \
eth/net.d \
eth/packetType.d \
eth/udp.d \
idlePattern.d \
systime.d \
temp.d \
tlc5941.d


C_DEPS_AS_ARGS +=  \
DanceFloorModule3.d \
delay.d \
df.d \
dfcontrol.d \
eth/arp.d \
eth/dhcp.d \
eth/enc28j60.d \
eth/eth.d \
eth/icmp.d \
eth/in.d \
eth/ipv4.d \
eth/net.d \
eth/packetType.d \
eth/udp.d \
idlePattern.d \
systime.d \
temp.d \
tlc5941.d


OUTPUT_FILE_PATH +=DanceFloorModule3.elf

OUTPUT_FILE_PATH_AS_ARGS +=DanceFloorModule3.elf

ADDITIONAL_DEPENDENCIES:=

OUTPUT_FILE_DEP:= ./makedep.mk

# AVR32/GNU C Compiler





































./%.o: .././%.c
	@echo Building file: $<
	@echo Invoking: AVR/GNU C Compiler : (AVR_8_bit_GNU_Toolchain_3.4.0_663) 4.6.2
	$(QUOTE)C:\Program Files (x86)\Atmel\Atmel Studio 6.0\extensions\Atmel\AVRGCC\3.4.0.65\AVRToolchain\bin\avr-gcc.exe$(QUOTE) -funsigned-char -funsigned-bitfields -O1 -fpack-struct -g2 -Wall -c -std=gnu99 -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"  -mmcu=atmega328p   -o"$@" "$<" 
	@echo Finished building: $<

eth/%.o: ../eth/%.c
	@echo Building file: $<
	@echo Invoking: AVR/GNU C Compiler : (AVR_8_bit_GNU_Toolchain_3.4.0_663) 4.6.2
	$(QUOTE)C:\Program Files (x86)\Atmel\Atmel Studio 6.0\extensions\Atmel\AVRGCC\3.4.0.65\AVRToolchain\bin\avr-gcc.exe$(QUOTE) -funsigned-char -funsigned-bitfields -O1 -fpack-struct -g2 -Wall -c -std=gnu99 -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"  -mmcu=atmega328p   -o"$@" "$<" 
	@echo Finished building: $<



# AVR32/GNU Preprocessing Assembler



# AVR32/GNU Assembler




ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: $(OUTPUT_FILE_PATH) $(ADDITIONAL_DEPENDENCIES)

$(OUTPUT_FILE_PATH): $(OBJS) $(USER_OBJS) $(OUTPUT_FILE_DEP)
	@echo Building target: $@
	@echo Invoking: AVR/GNU Linker : (AVR_8_bit_GNU_Toolchain_3.4.0_663) 4.6.2
	$(QUOTE)C:\Program Files (x86)\Atmel\Atmel Studio 6.0\extensions\Atmel\AVRGCC\3.4.0.65\AVRToolchain\bin\avr-gcc.exe$(QUOTE) -o$(OUTPUT_FILE_PATH_AS_ARGS) $(OBJS_AS_ARGS) $(USER_OBJS) $(LIBS) -Wl,-Map="DanceFloorModule3.map" -Wl,--start-group -Wl,-lm  -Wl,--end-group -Wl,--gc-sections  -mmcu=atmega328p  
	@echo Finished building target: $@
	"C:\Program Files (x86)\Atmel\Atmel Studio 6.0\extensions\Atmel\AVRGCC\3.4.0.65\AVRToolchain\bin\avr-objcopy.exe" -O ihex -R .eeprom -R .fuse -R .lock -R .signature  "DanceFloorModule3.elf" "DanceFloorModule3.hex"
	"C:\Program Files (x86)\Atmel\Atmel Studio 6.0\extensions\Atmel\AVRGCC\3.4.0.65\AVRToolchain\bin\avr-objcopy.exe" -j .eeprom  --set-section-flags=.eeprom=alloc,load --change-section-lma .eeprom=0  --no-change-warnings -O ihex "DanceFloorModule3.elf" "DanceFloorModule3.eep" || exit 0
	"C:\Program Files (x86)\Atmel\Atmel Studio 6.0\extensions\Atmel\AVRGCC\3.4.0.65\AVRToolchain\bin\avr-objdump.exe" -h -S "DanceFloorModule3.elf" > "DanceFloorModule3.lss"
	"C:\Program Files (x86)\Atmel\Atmel Studio 6.0\extensions\Atmel\AVRGCC\3.4.0.65\AVRToolchain\bin\avr-size.exe" "DanceFloorModule3.elf"
	





# Other Targets
clean:
	-$(RM) $(OBJS_AS_ARGS)$(C_DEPS_AS_ARGS) $(EXECUTABLES) 
	rm -rf "DanceFloorModule3.elf" "DanceFloorModule3.a" "DanceFloorModule3.hex" "DanceFloorModule3.lss" "DanceFloorModule3.eep" "DanceFloorModule3.map"
	