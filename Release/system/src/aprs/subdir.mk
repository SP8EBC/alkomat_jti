################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../system/src/aprs/adc.c \
../system/src/aprs/afsk.c \
../system/src/aprs/ax25.c \
../system/src/aprs/crc.c \
../system/src/aprs/dac.c \
../system/src/aprs/raw.c 

OBJS += \
./system/src/aprs/adc.o \
./system/src/aprs/afsk.o \
./system/src/aprs/ax25.o \
./system/src/aprs/crc.o \
./system/src/aprs/dac.o \
./system/src/aprs/raw.o 

C_DEPS += \
./system/src/aprs/adc.d \
./system/src/aprs/afsk.d \
./system/src/aprs/ax25.d \
./system/src/aprs/crc.d \
./system/src/aprs/dac.d \
./system/src/aprs/raw.d 


# Each subdirectory must supply rules for building sources it contributes
system/src/aprs/%.o: ../system/src/aprs/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -Wall -Wextra  -g -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DSTM32F10X_MD_VL -DUSE_STDPERIPH_DRIVER -DHSE_VALUE=8000000 -I"../include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/stm32f1-stdperiph" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


