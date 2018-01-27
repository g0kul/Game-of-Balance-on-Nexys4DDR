################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
LD_SRCS += \
../LCD/src/lscript.ld 

C_SRCS += \
../LCD/src/fonts.c \
../LCD/src/lcd.c \
../LCD/src/lcd_test.c 

OBJS += \
./LCD/src/fonts.o \
./LCD/src/lcd.o \
./LCD/src/lcd_test.o 

C_DEPS += \
./LCD/src/fonts.d \
./LCD/src/lcd.d \
./LCD/src/lcd_test.d 


# Each subdirectory must supply rules for building sources it contributes
LCD/src/%.o: ../LCD/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MicroBlaze gcc compiler'
	mb-gcc -Wall -O0 -g3 -c -fmessage-length=0 -MT"$@" -I../../mb-lcd-vol_bsp/microblaze_0/include -mlittle-endian -mcpu=v10.0 -mxl-soft-mul -Wl,--no-relax -ffunction-sections -fdata-sections -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


