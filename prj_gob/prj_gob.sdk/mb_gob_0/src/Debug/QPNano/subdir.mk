################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../QPNano/qepn.c \
../QPNano/qfn.c \
../QPNano/qfn_init.c 

OBJS += \
./QPNano/qepn.o \
./QPNano/qfn.o \
./QPNano/qfn_init.o 

C_DEPS += \
./QPNano/qepn.d \
./QPNano/qfn.d \
./QPNano/qfn_init.d 


# Each subdirectory must supply rules for building sources it contributes
QPNano/%.o: ../QPNano/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MicroBlaze gcc compiler'
	mb-gcc -Wall -O0 -g3 -c -fmessage-length=0 -MT"$@" -I../../mb-lcd-vol_bsp/microblaze_0/include -mlittle-endian -mcpu=v10.0 -mxl-soft-mul -Wl,--no-relax -ffunction-sections -fdata-sections -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


