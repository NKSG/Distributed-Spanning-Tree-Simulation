################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../LList.c \
../Network-Topology.c \
../Simulation.c 

OBJS += \
./LList.o \
./Network-Topology.o \
./Simulation.o 

C_DEPS += \
./LList.d \
./Network-Topology.d \
./Simulation.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)"  -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Simulation.o: ../Simulation.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -o "$@" "$<" -Ipthread -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"Simulation.d"
	@echo 'Finished building: $<'
	@echo ' '


