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


**Code**
```

int main()
{
int sensorValue,i,j;
int GoutValue,Result1,Mask;
int Sensor_IN,Door;


while(1)
{

	asm volatile(
		"or x30, x30, %1\n\t"
		"andi %0, x30, 0x01\n\t"
		: "=r" (Sensor_IN)			// Input
		: "r" (GoutValue)			// Storing Input
		: "x30"
		);


if (GoutValue)
	{
	Mask=0xFFFFFFFD;

	Door=1;

	
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


	}
else
	{
	
	Mask=0xFFFFFFFD;
	
	Door=0;

	
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


	}
	
}

return 0;
}


```
### Assembly code conversion

The above C program is compiled using the RISC-V GNU toolchain and the assembly code is dumped into a text file.

Below codes are run on the terminal to get the assembly code.
```
riscv64-unknown-elf-gcc -mabi=ilp32 -march=rv32i -ffreestanding -nostdlib -o ./out Garage.c 
riscv64-unknown-elf-objdump -d -r out > Garage_assembly.txt

```

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
To find the number of unique instructions make sure to rename the filename as assembly.txt since the python script that we are using is opening the file name with assembly.txt and both files should be in the same directory. The python script I am using is already uploaded. Now follow the command to get the number of different instructions used.
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


### Spike Simulation
Now spike simulation is done using following commands.

```
riscv64-unknown-elf-gcc -march=rv64i -mabi=lp64 -ffreestanding -o out file.c
spike pk out
```


Here, We have one inputs and only one output, so there are only *two* test cases and out of them only one of them will result in the output being high, and in other the output is expected as low.For the sake of simulation in spice we are not using an infinite loop but just one iteration of it.
For spike simulation, the inputs is hard coded for the two test cases.

Case 1: When Gout Value is hard coded as 0. We get SensorValue = 0 and Door = 0(Closed)
![Screenshot from 2023-10-25 18-02-02](https://github.com/ShubhamGitHub528/Home-Automation-System/assets/140998623/c2f6f54f-3826-4553-beef-2b1237fb5dba)


Case 2: When Gout Value is hard coded as 1. We get SensorValue = Z(HighImpedance) and Door = 1(Open)
![Screenshot from 2023-10-25 18-02-52](https://github.com/ShubhamGitHub528/Home-Automation-System/assets/140998623/386dd660-2a35-485f-b391-30c448024ffa)




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








