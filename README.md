# Automatic Garage Door System.

### ABSTRACT

This proposed system uses a IR sensor to sense the human body movement near to the door. A human body emits
infrared energy in the form of heat, which is detected by the PIR sensor from a particular distance. This project
proposes a system of automatic opening and closing of door by sensing any body movement near the door.

### Block Diagram

![image](https://github.com/ShubhamGitHub528/Home-Automation-System/assets/140998623/0cb74d1c-50d3-482b-8799-66d291921b45)

### Assumed Schematic.

![image](https://github.com/ShubhamGitHub528/Home-Automation-System/assets/140998623/bff81d81-9270-4947-9827-d41de1f9d28b)


### Register architecture of x30 for GPIOs:


![image](https://github.com/ShubhamGitHub528/Home-Automation-System/assets/140998623/dcb0d6b6-af63-40c2-8af7-d9ae39fa711a)


x30[3:0] is row pins of keypad.

x30[7:4] is column pins of keypad.

x30[14:8] is 7 segment display pins.

x30[25] is mode_led to indicate input / display mode of system. LED is ON if input mode else OFF for display mode.

x30[27] is next input which is used as enter button to store each character we enter.

x30[29] is delay pin where it accepts signal from 555 timer.

x30[31] is input/display mode input pin.

**C Code for the design**
```

int main()
{
int sensorValue;
int GoutValue;
int GoutValue_reg;

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

	for (i = 0; i < 3000; i++) {
        	for (j = 0; j < 1000000; j++) {
            	// Adding a loop inside to approximate seconds
        	}
    	    }


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

return 0;
}


```
**Assembly code command**

Compile the c program using RISCV-V GNU Toolchain and dump the assembly code into C_code.txt using the below commands.

```
riscv64-unknown-elf-gcc -march=rv32i -mabi=ilp32 -ffreestanding -nostdlib -o ./out C_code.c
riscv64-unknown-elf-objdump -d  -r out > C_code.txt
```



**Assembly code conversion**
```

out:     file format elf32-littleriscv


Disassembly of section .text:

00010054 <main>:
   10054:	fd010113          	addi	sp,sp,-48
   10058:	02812623          	sw	s0,44(sp)
   1005c:	03010413          	addi	s0,sp,48
   10060:	fe042223          	sw	zero,-28(s0)
   10064:	fe442783          	lw	a5,-28(s0)
   10068:	00179793          	slli	a5,a5,0x1
   1006c:	fef42023          	sw	a5,-32(s0)
   10070:	fe042783          	lw	a5,-32(s0)
   10074:	00ff6f33          	or	t5,t5,a5
   10078:	001f7793          	andi	a5,t5,1
   1007c:	fcf42e23          	sw	a5,-36(s0)
   10080:	fdc42703          	lw	a4,-36(s0)
   10084:	00100793          	li	a5,1
   10088:	06f71663          	bne	a4,a5,100f4 <main+0xa0>
   1008c:	00100793          	li	a5,1
   10090:	fef42223          	sw	a5,-28(s0)
   10094:	fe442783          	lw	a5,-28(s0)
   10098:	00179793          	slli	a5,a5,0x1
   1009c:	fef42023          	sw	a5,-32(s0)
   100a0:	fe042783          	lw	a5,-32(s0)
   100a4:	00ff6f33          	or	t5,t5,a5
   100a8:	fe042623          	sw	zero,-20(s0)
   100ac:	0340006f          	j	100e0 <main+0x8c>
   100b0:	fe042423          	sw	zero,-24(s0)
   100b4:	0100006f          	j	100c4 <main+0x70>
   100b8:	fe842783          	lw	a5,-24(s0)
   100bc:	00178793          	addi	a5,a5,1
   100c0:	fef42423          	sw	a5,-24(s0)
   100c4:	fe842703          	lw	a4,-24(s0)
   100c8:	000f47b7          	lui	a5,0xf4
   100cc:	23f78793          	addi	a5,a5,575 # f423f <__global_pointer$+0xe292f>
   100d0:	fee7d4e3          	bge	a5,a4,100b8 <main+0x64>
   100d4:	fec42783          	lw	a5,-20(s0)
   100d8:	00178793          	addi	a5,a5,1
   100dc:	fef42623          	sw	a5,-20(s0)
   100e0:	fec42703          	lw	a4,-20(s0)
   100e4:	000017b7          	lui	a5,0x1
   100e8:	bb778793          	addi	a5,a5,-1097 # bb7 <main-0xf49d>
   100ec:	fce7d2e3          	bge	a5,a4,100b0 <main+0x5c>
   100f0:	f89ff06f          	j	10078 <main+0x24>
   100f4:	fe042223          	sw	zero,-28(s0)
   100f8:	fe442783          	lw	a5,-28(s0)
   100fc:	00179793          	slli	a5,a5,0x1
   10100:	fef42023          	sw	a5,-32(s0)
   10104:	fe042783          	lw	a5,-32(s0)
   10108:	00ff6f33          	or	t5,t5,a5
   1010c:	f6dff06f          	j	10078 <main+0x24>
```
   
***Number of different instructions: 15***
```
Number of different instructions: 11
List of unique instructions:
bne
j
lw
andi
addi
li
lui
or
bge
sw
slli

```

### Spike Simulation.

**Code**
```
#include<stdio.h>

int main()
{
int sensorValue,i,j;
int GoutValue,Result1,Mask;
int Sensor_IN,Door;


// GoutValue =0;
// GoutValue_reg = GoutValue*2;

//asm code to initialize the garage door keep it 0 make it closed initialy
/*
asm volatile(
	"or x30, x30, %0\n\t" 
	:
	:"r"(GoutValue_reg)
	:"x30"
	);
*/


for (int j=0; j<15;j++) 
//while(1)
{

if(j<10)
			GoutValue = 1;
	else
			GoutValue =0;
			
			
//  asm code to read sensor value
/*
asm volatile(
	"andi %0, x30, 1\n\t"
	:"=r"(sensorValue)
	:
	:
	);
*/
	asm volatile(
		"or x30, x30, %1\n\t"
		"andi %0, x30, 0x01\n\t"
		: "=r" (Sensor_IN)			// Input
		: "r" (GoutValue)			// Storing Input
		: "x30"
		);



//if condition logic
if (GoutValue)
	{
	Mask=0xFFFFFFFD;
	
	// printf(" \n");
	
	Door=1;
	// GoutValue_reg = GoutValue*2;
	
	//asm code to set output reg
	/*
	asm volatile(
	"or x30, x30, %0\n\t" 
	:
	:"r"(GoutValue_reg)
	:"x30"
	);
	*/
	
	asm volatile(
            "and x30,x30, %0\n\t"     // Load immediate 1 into x30
            "ori x30, x30,2"                 // output at 2nd bit , that switches on the motor
            :
            :"r"(Mask)
            :"x30"
            );
            
            asm volatile(
	    	"addi %0, x30, 0\n\t"
	    	:"=r"(Result1)
	    	:
	    	:"x30"
	    	);
    	printf("Result1 = %d\n",Result1);
    	
	/*
	for (i = 0; i < 3000; i++) {
        	for (j = 0; j < 1000000; j++) {
            	// Adding a loop inside to approximate seconds
        	}
    	    }
	*/

	}
else
	{
	
	Mask=0xFFFFFFFD;
	
	Door=0;
	// GoutValue_reg = GoutValue*2;
	
	/*
	//asm code to set output reg	
	asm volatile(
	"or x30, x30, %0\n\t" 
	:
	:"r"(GoutValue_reg)
	:"x30"
	);
	*/
	
	asm volatile( 
            "and x30,x30, %0\n\t"     
            "ori x30, x30,0"            
            :
            :"r"(Mask)
            :"x30"
        );
        asm volatile(
	    	"addi %0, x30, 0\n\t"
	    	:"=r"(Result1)
	    	:
	    	:"x30"
	    	);
	 printf("Result1 = %d\n",Result1);

	}
	printf("Door=%d \n", GoutValue); 
}

return 0;
}

```


*The simulation commands and outputs are as follows*

```
riscv64-unknown-elf-gcc -march=rv64i -mabi=lp64 -ffreestanding -o out file.c
spike pk out
```
### Results

![image](https://github.com/ShubhamGitHub528/Home-Automation-System/assets/140998623/9222f9de-4949-411f-bc9e-c4e3a8d3b77d)



**Assembly Code**
```
out:     file format elf32-littleriscv


Disassembly of section .text:

00010054 <main>:
   10054:	fd010113          	addi	sp,sp,-48
   10058:	02812623          	sw	s0,44(sp)
   1005c:	03010413          	addi	s0,sp,48
   10060:	fec42783          	lw	a5,-20(s0)
   10064:	00ff6f33          	or	t5,t5,a5
   10068:	001f7793          	andi	a5,t5,1
   1006c:	fef42423          	sw	a5,-24(s0)
   10070:	fec42783          	lw	a5,-20(s0)
   10074:	02078663          	beqz	a5,100a0 <main+0x4c>
   10078:	ffd00793          	li	a5,-3
   1007c:	fef42223          	sw	a5,-28(s0)
   10080:	00100793          	li	a5,1
   10084:	fef42023          	sw	a5,-32(s0)
   10088:	fe442783          	lw	a5,-28(s0)
   1008c:	00ff7f33          	and	t5,t5,a5
   10090:	002f6f13          	ori	t5,t5,2
   10094:	000f0793          	mv	a5,t5
   10098:	fcf42e23          	sw	a5,-36(s0)
   1009c:	fc5ff06f          	j	10060 <main+0xc>
   100a0:	ffd00793          	li	a5,-3
   100a4:	fef42223          	sw	a5,-28(s0)
   100a8:	fe042023          	sw	zero,-32(s0)
   100ac:	fe442783          	lw	a5,-28(s0)
   100b0:	00ff7f33          	and	t5,t5,a5
   100b4:	000f6f13          	ori	t5,t5,0
   100b8:	000f0793          	mv	a5,t5
   100bc:	fcf42e23          	sw	a5,-36(s0)
   100c0:	fa1ff06f          	j	10060 <main+0xc>
```

***Number of different instructions: 11***
```
List of unique instructions:
lw
addi
ori
andi
sw
li
beqz
or
and
j
mv

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








