# Automatic Garage Door System.


### RISCV GNU tool chain

RISCV GNU tool chain is a C & C++ cross compiler. It has two modes: ELF/Newlib toolchain and Linux-ELF/glibc toolchain. We are using ELF/Newlib toolchain.

We are building a custom RISCV based application core for a specific application for 32 bit processor. 

Following are tools required to compile & execute the application:

1. RISCV GNU toolchain with dependent libraries as specified in [RISCV-GNU-Toolchain](https://github.com/riscv-collab/riscv-gnu-toolchain).

2. Spike simulator - Spike is a functional RISC-V ISA simulator that implements a functional model of one or more RISC-V harts. [RISCV-SPIKE](https://github.com/riscv-software-src/riscv-isa-sim.git).

### RISCV 32 bit compiler installation.

```
sudo apt install libc6-dev
git clone https://github.com/riscv/riscv-gnu-toolchain --recursive
mkdir riscv32-toolchain
cd riscv-gnu-toolchain
./configure --prefix=/home/bhargav/riscv32-toolchain/ --with-arch=rv32i --with-abi=ilp32
sudo apt-get install libgmp-dev
make
```

Access the riscv32-unknown-elf-gcc inside bin folder of riscv32-toolchain folder in home folder of user as shown.

```
/home/bhargav/riscv32-toolchain/bin/riscv32-unknown-elf-gcc --version
```

## ABSTRACT

This proposed system uses a IR sensor to sense the human body movement near to the door. A human body emits
infrared energy in the form of heat, which is detected by the PIR sensor from a particular distance. This project
proposes a system of automatic opening and closing of door by sensing any body movement near the door.

### Block Diagram

![image](https://github.com/ShubhamGitHub528/Home-Automation-System/assets/140998623/08f92f8b-1e33-4596-8894-a261c7bc76be)


### Register architecture of x30 for GPIOs:


![image](https://github.com/ShubhamGitHub528/Home-Automation-System/assets/140998623/dcb0d6b6-af63-40c2-8af7-d9ae39fa711a)


x30[3:0] is row pins of keypad.

x30[7:4] is column pins of keypad.

x30[14:8] is 7 segment display pins.

x30[25] is mode_led to indicate input / display mode of system. LED is ON if input mode else OFF for display mode.

x30[27] is next input which is used as enter button to store each character we enter.

x30[29] is delay pin where it accepts signal from 555 timer.

x30[31] is input/display mode input pin.

### C Code for the design.
```
int sensorValue;
int GoutValue;
int GoutValue_reg;

void initialize();
void delaytime(int);
int main()
{

GoutValue =0;
GoutValue_reg = GoutValue*2;

//asm code to initialize the garage door keep it 0 make it closed initialy
asm volatile(
	"or x30, x30, %0\n\t" 
	:
	:"r"(GoutValue_reg)
	:"x30"
	);



while (1) {


//  asm code to read sensor value
asm volatile(
	"andi %0, x30, 1\n\t"
	:"=r"(sensorValue)
	:
	:
	);




//if condition logic
if (sensorValue ==1)
	{
	GoutValue=1;
	GoutValue_reg = GoutValue*2;
	
	//asm code to set output reg
	asm volatile(
	"or x30, x30, %0\n\t" 
	:
	:"r"(GoutValue_reg)
	:"x30"
	);


	delaytime(1000);
	}
else
	{
	GoutValue=0;
	GoutValue_reg = GoutValue*2;
	
	//asm code to set output reg	
	asm volatile(
	"or x30, x30, %0\n\t" 
	:
	:"r"(GoutValue_reg)
	:"x30"
	);

	}

}
delaytime(500);
return 0;
}

void delaytime(int seconds) {
    int i, j;
    for (i = 0; i < seconds; i++) {
        for (j = 0; j < 1000000; j++) {
            //loop for delay
        }
    }
}
```
### Assembly code conversion.
```
shubham_proj.o:     file format elf32-littleriscv

Disassembly of section .text:

00010094 <main>:
   10094:	ff010113          	add	sp,sp,-16
   10098:	00112623          	sw	ra,12(sp)
   1009c:	00812423          	sw	s0,8(sp)
   100a0:	01010413          	add	s0,sp,16
   100a4:	000117b7          	lui	a5,0x11
   100a8:	1a07a023          	sw	zero,416(a5) # 111a0 <GoutValue>
   100ac:	000117b7          	lui	a5,0x11
   100b0:	1a07a783          	lw	a5,416(a5) # 111a0 <GoutValue>
   100b4:	00179713          	sll	a4,a5,0x1
   100b8:	80e1a423          	sw	a4,-2040(gp) # 111a4 <GoutValue_reg>
   100bc:	8081a783          	lw	a5,-2040(gp) # 111a4 <GoutValue_reg>
   100c0:	00ff6f33          	or	t5,t5,a5
   100c4:	001f7713          	and	a4,t5,1
   100c8:	000117b7          	lui	a5,0x11
   100cc:	18e7ae23          	sw	a4,412(a5) # 1119c <_DATA_BEGIN_>
   100d0:	000117b7          	lui	a5,0x11
   100d4:	19c7a703          	lw	a4,412(a5) # 1119c <_DATA_BEGIN_>
   100d8:	00100793          	li	a5,1
   100dc:	02f71a63          	bne	a4,a5,10110 <main+0x7c>
   100e0:	000117b7          	lui	a5,0x11
   100e4:	00100713          	li	a4,1
   100e8:	1ae7a023          	sw	a4,416(a5) # 111a0 <GoutValue>
   100ec:	000117b7          	lui	a5,0x11
   100f0:	1a07a783          	lw	a5,416(a5) # 111a0 <GoutValue>
   100f4:	00179713          	sll	a4,a5,0x1
   100f8:	80e1a423          	sw	a4,-2040(gp) # 111a4 <GoutValue_reg>
   100fc:	8081a783          	lw	a5,-2040(gp) # 111a4 <GoutValue_reg>
   10100:	00ff6f33          	or	t5,t5,a5
   10104:	3e800513          	li	a0,1000
   10108:	02c000ef          	jal	10134 <delaytime>
   1010c:	fb9ff06f          	j	100c4 <main+0x30>
   10110:	000117b7          	lui	a5,0x11
   10114:	1a07a023          	sw	zero,416(a5) # 111a0 <GoutValue>
   10118:	000117b7          	lui	a5,0x11
   1011c:	1a07a783          	lw	a5,416(a5) # 111a0 <GoutValue>
   10120:	00179713          	sll	a4,a5,0x1
   10124:	80e1a423          	sw	a4,-2040(gp) # 111a4 <GoutValue_reg>
   10128:	8081a783          	lw	a5,-2040(gp) # 111a4 <GoutValue_reg>
   1012c:	00ff6f33          	or	t5,t5,a5
   10130:	f95ff06f          	j	100c4 <main+0x30>

00010134 <delaytime>:
   10134:	fd010113          	add	sp,sp,-48
   10138:	02812623          	sw	s0,44(sp)
   1013c:	03010413          	add	s0,sp,48
   10140:	fca42e23          	sw	a0,-36(s0)
   10144:	fe042623          	sw	zero,-20(s0)
   10148:	0340006f          	j	1017c <delaytime+0x48>
   1014c:	fe042423          	sw	zero,-24(s0)
   10150:	0100006f          	j	10160 <delaytime+0x2c>
   10154:	fe842783          	lw	a5,-24(s0)
   10158:	00178793          	add	a5,a5,1
   1015c:	fef42423          	sw	a5,-24(s0)
   10160:	fe842703          	lw	a4,-24(s0)
   10164:	000f47b7          	lui	a5,0xf4
   10168:	23f78793          	add	a5,a5,575 # f423f <__global_pointer$+0xe28a3>
   1016c:	fee7d4e3          	bge	a5,a4,10154 <delaytime+0x20>
   10170:	fec42783          	lw	a5,-20(s0)
   10174:	00178793          	add	a5,a5,1
   10178:	fef42623          	sw	a5,-20(s0)
   1017c:	fec42703          	lw	a4,-20(s0)
   10180:	fdc42783          	lw	a5,-36(s0)
   10184:	fcf744e3          	blt	a4,a5,1014c <delaytime+0x18>
   10188:	00000013          	nop
   1018c:	00000013          	nop
   10190:	02c12403          	lw	s0,44(sp)
   10194:	03010113          	add	sp,sp,48
   10198:	00008067          	ret
```
   
### Number of different instructions: 15
```
List of unique instructions:
nop
lw
and
li
ret
j
sll
sw
bge
add
blt
jal
or
lui
bne
```

## Word of Thanks
I sciencerly thank **Mr. Kunal Gosh**(Founder/**VSD**) for helping me out to complete this flow smoothly.

## Acknowledgement
- Kunal Ghosh, VSD Corp. Pvt. Ltd.
- Skywater Foundry
- Chatgpt
- Amith Bharadwaj,Colleague,IIIT B
- Emil Jayanth Lal,Colleague,IIIT B
- Sumanto Kar,VSD Corp.
- Mayank Kabra,imtech

  
  
## Reference 


- https://github.com/The-OpenROAD-Project/OpenSTA.git
- https://github.com/kunalg123
- https://www.vsdiat.com
- https://github.com/SakethGajawada/RISCV-GNU








