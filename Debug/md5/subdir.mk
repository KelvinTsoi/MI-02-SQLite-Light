################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../md5/md5.c 

OBJS += \
./md5/md5.o 

C_DEPS += \
./md5/md5.d 


# Each subdirectory must supply rules for building sources it contributes
md5/%.o: ../md5/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -Idl -Ipthread -O0 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


