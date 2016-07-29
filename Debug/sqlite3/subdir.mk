################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../sqlite3/sqlite3.c 

OBJS += \
./sqlite3/sqlite3.o 

C_DEPS += \
./sqlite3/sqlite3.d 


# Each subdirectory must supply rules for building sources it contributes
sqlite3/%.o: ../sqlite3/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -Idl -Ipthread -O0 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


