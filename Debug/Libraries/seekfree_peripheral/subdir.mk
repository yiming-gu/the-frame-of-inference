################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Libraries/seekfree_peripheral/SEEKFREE_18TFT.c \
../Libraries/seekfree_peripheral/SEEKFREE_FONT.c \
../Libraries/seekfree_peripheral/SEEKFREE_FUN.c \
../Libraries/seekfree_peripheral/SEEKFREE_IIC.c \
../Libraries/seekfree_peripheral/SEEKFREE_MT9V03X.c 

OBJS += \
./Libraries/seekfree_peripheral/SEEKFREE_18TFT.o \
./Libraries/seekfree_peripheral/SEEKFREE_FONT.o \
./Libraries/seekfree_peripheral/SEEKFREE_FUN.o \
./Libraries/seekfree_peripheral/SEEKFREE_IIC.o \
./Libraries/seekfree_peripheral/SEEKFREE_MT9V03X.o 

COMPILED_SRCS += \
./Libraries/seekfree_peripheral/SEEKFREE_18TFT.src \
./Libraries/seekfree_peripheral/SEEKFREE_FONT.src \
./Libraries/seekfree_peripheral/SEEKFREE_FUN.src \
./Libraries/seekfree_peripheral/SEEKFREE_IIC.src \
./Libraries/seekfree_peripheral/SEEKFREE_MT9V03X.src 

C_DEPS += \
./Libraries/seekfree_peripheral/SEEKFREE_18TFT.d \
./Libraries/seekfree_peripheral/SEEKFREE_FONT.d \
./Libraries/seekfree_peripheral/SEEKFREE_FUN.d \
./Libraries/seekfree_peripheral/SEEKFREE_IIC.d \
./Libraries/seekfree_peripheral/SEEKFREE_MT9V03X.d 


# Each subdirectory must supply rules for building sources it contributes
Libraries/seekfree_peripheral/%.src: ../Libraries/seekfree_peripheral/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: TASKING C/C++ Compiler'
	cctc -D__CPU__=tc37x "-fD:/FSKR/MyFirstCar/gMNIST/Debug/TASKING_C_C___Compiler-Include_paths.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc37x -o "$@"  "$<"  -cs --dep-file="$(@:.src=.d)" --misrac-version=2012 -N0 -Z0 -Y0 2>&1;
	@echo 'Finished building: $<'
	@echo ' '

Libraries/seekfree_peripheral/%.o: ./Libraries/seekfree_peripheral/%.src
	@echo 'Building file: $<'
	@echo 'Invoking: TASKING Assembler'
	astc -o  "$@" "$<" --list-format=L1 --optimize=gs
	@echo 'Finished building: $<'
	@echo ' '


