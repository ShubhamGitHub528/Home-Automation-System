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

### C Code for the design.
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
### Assembly code conversion

Compile the c program using RISCV-V GNU Toolchain and dump the assembly code into C_code.txt using the below commands.

```
riscv64-unknown-elf-gcc -march=rv32i -mabi=ilp32 -ffreestanding -nostdlib -o ./out C_code.c
riscv64-unknown-elf-objdump -d  -r out > C_code.txt
```



### Assembly code conversion.
```

out:     file format elf32-littleriscv


Disassembly of section .text:

00010074 <main>:
   10074:	ff010113          	addi	sp,sp,-16
   10078:	00112623          	sw	ra,12(sp)
   1007c:	00812423          	sw	s0,8(sp)
   10080:	01010413          	addi	s0,sp,16
   10084:	000117b7          	lui	a5,0x11
   10088:	1807a423          	sw	zero,392(a5) # 11188 <__DATA_BEGIN__>
   1008c:	000117b7          	lui	a5,0x11
   10090:	1887a783          	lw	a5,392(a5) # 11188 <__DATA_BEGIN__>
   10094:	00179713          	slli	a4,a5,0x1
   10098:	000117b7          	lui	a5,0x11
   1009c:	18e7a623          	sw	a4,396(a5) # 1118c <GoutValue_reg>
   100a0:	000117b7          	lui	a5,0x11
   100a4:	18c7a783          	lw	a5,396(a5) # 1118c <GoutValue_reg>
   100a8:	00ff6f33          	or	t5,t5,a5
   100ac:	001f7713          	andi	a4,t5,1
   100b0:	80e1a423          	sw	a4,-2040(gp) # 11190 <sensorValue>
   100b4:	8081a703          	lw	a4,-2040(gp) # 11190 <sensorValue>
   100b8:	00100793          	li	a5,1
   100bc:	02f71e63          	bne	a4,a5,100f8 <main+0x84>
   100c0:	000117b7          	lui	a5,0x11
   100c4:	00100713          	li	a4,1
   100c8:	18e7a423          	sw	a4,392(a5) # 11188 <__DATA_BEGIN__>
   100cc:	000117b7          	lui	a5,0x11
   100d0:	1887a783          	lw	a5,392(a5) # 11188 <__DATA_BEGIN__>
   100d4:	00179713          	slli	a4,a5,0x1
   100d8:	000117b7          	lui	a5,0x11
   100dc:	18e7a623          	sw	a4,396(a5) # 1118c <GoutValue_reg>
   100e0:	000117b7          	lui	a5,0x11
   100e4:	18c7a783          	lw	a5,396(a5) # 1118c <GoutValue_reg>
   100e8:	00ff6f33          	or	t5,t5,a5
   100ec:	3e800513          	li	a0,1000
   100f0:	034000ef          	jal	ra,10124 <delaytime>
   100f4:	fb9ff06f          	j	100ac <main+0x38>
   100f8:	000117b7          	lui	a5,0x11
   100fc:	1807a423          	sw	zero,392(a5) # 11188 <__DATA_BEGIN__>
   10100:	000117b7          	lui	a5,0x11
   10104:	1887a783          	lw	a5,392(a5) # 11188 <__DATA_BEGIN__>
   10108:	00179713          	slli	a4,a5,0x1
   1010c:	000117b7          	lui	a5,0x11
   10110:	18e7a623          	sw	a4,396(a5) # 1118c <GoutValue_reg>
   10114:	000117b7          	lui	a5,0x11
   10118:	18c7a783          	lw	a5,396(a5) # 1118c <GoutValue_reg>
   1011c:	00ff6f33          	or	t5,t5,a5
   10120:	f8dff06f          	j	100ac <main+0x38>

00010124 <delaytime>:
   10124:	fd010113          	addi	sp,sp,-48
   10128:	02812623          	sw	s0,44(sp)
   1012c:	03010413          	addi	s0,sp,48
   10130:	fca42e23          	sw	a0,-36(s0)
   10134:	fe042623          	sw	zero,-20(s0)
   10138:	0340006f          	j	1016c <delaytime+0x48>
   1013c:	fe042423          	sw	zero,-24(s0)
   10140:	0100006f          	j	10150 <delaytime+0x2c>
   10144:	fe842783          	lw	a5,-24(s0)
   10148:	00178793          	addi	a5,a5,1
   1014c:	fef42423          	sw	a5,-24(s0)
   10150:	fe842703          	lw	a4,-24(s0)
   10154:	000f47b7          	lui	a5,0xf4
   10158:	23f78793          	addi	a5,a5,575 # f423f <__global_pointer$+0xe28b7>
   1015c:	fee7d4e3          	bge	a5,a4,10144 <delaytime+0x20>
   10160:	fec42783          	lw	a5,-20(s0)
   10164:	00178793          	addi	a5,a5,1
   10168:	fef42623          	sw	a5,-20(s0)
   1016c:	fec42703          	lw	a4,-20(s0)
   10170:	fdc42783          	lw	a5,-36(s0)
   10174:	fcf744e3          	blt	a4,a5,1013c <delaytime+0x18>
   10178:	00000013          	nop
   1017c:	02c12403          	lw	s0,44(sp)
   10180:	03010113          	addi	sp,sp,48
   10184:	00008067          	ret
```
   
### Number of different instructions: 15
```
Number of different instructions: 15
List of unique instructions:
bge
sw
jal
or
addi
lui
lw
ret
bne
blt
j
li
andi
slli
nop
shu
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








