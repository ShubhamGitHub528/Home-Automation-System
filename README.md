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

### Functional Simulation

![Screenshot from 2023-10-28 00-24-52](https://github.com/ShubhamGitHub528/Home-Automation-System/assets/140998623/513ee31f-b13d-4566-a2a0-b38e3d8d9152)

**GtkWaveform.**
By performing functional simulation we can verify our design through the verilog code which is processor.v and testbench.v. Here one trigger pin is there , input_wire is the sensorValue pin and output_wires is Door. So as shown in the gtkwave diagram when trigger is made zero and SensorValue pin(input_wire) is 0 , buzzer and led (output_wires) is 0. There is also shown the clk, write_done and ID_instruction in the gtkwave diagram below.
![Screenshot from 2023-10-28 23-30-29](https://github.com/ShubhamGitHub528/Home-Automation-System/assets/140998623/e9fd0fd0-f1cb-4e73-bb62-8852c6af1966)


![Screenshot from 2023-10-28 23-27-41](https://github.com/ShubhamGitHub528/Home-Automation-System/assets/140998623/dfc4997d-aa2c-4c04-be2e-c54e61c109a2)


### Instruction verification

![image](https://github.com/DINESHIIITB/iiitb_riscv_drip_irrigation_system/assets/140998565/acdadb8b-6161-44fa-b5ce-f58c90736ee7)

![image](https://github.com/DINESHIIITB/iiitb_riscv_drip_irrigation_system/assets/140998565/08ec655a-09d6-4846-a8e3-7127f011a1cc)




### Gate Level Simulation :

```

read_liberty -lib sky130_fd_sc_hd__tt_025C_1v80_256.lib 
read_verilog processor.v 
synth -top wrapper
dfflibmap -liberty sky130_fd_sc_hd__tt_025C_1v80_256.lib 
abc -liberty sky130_fd_sc_hd__tt_025C_1v80_256.lib
write_verilog synth_processor_test.v
```

command to run gls simulation

```

iverilog -o test synth_processor_test.v testbench.v sky130_sram_1kbyte_1rw1r_32x256_8.v sky130_fd_sc_hd.v primitives.v
```
![image](https://github.com/ShubhamGitHub528/Home-Automation-System/assets/140998623/62ba795b-cf65-4d4d-a02b-109837523cad)



### wrapper module after netlist created

```
show wrapper
```
![image](https://github.com/ShubhamGitHub528/Home-Automation-System/assets/140998623/d49c0fd8-e493-4902-b099-be2f6fc9ac05)




### To generate Processor.
Upload all.json and assembly code on the given url ```http://16.16.202.21/```


## Preparing the Design:

Preparing the design and including the lef files: The commands to prepare the design and overwite in a existing run folder the reports and results along with the command to include the lef files is given below:

```bash
## Update library files
sed -i 's/max_transition :0.04/max_transition :0.75/' *.lib

## Mount directories
make mount

## Run OpenLANE interactively
./flow.tcl -interactive

## Check OpenLANE version
% package require openlane 0.9

## Prepare design
% prep -design project -verbose 99
```
![image](https://github.com/ShubhamGitHub528/Home-Automation-System/assets/140998623/797c5fce-0c5f-44e5-a360-6dd30229c762)

## Synthesis

Logic synthesis transforms the RTL netlist through two key steps:

- GTECH Mapping:
    - Maps the HDL netlist to generic gates.
    - Enables logical optimization using AIGERs and other topologies derived from the generic mapped netlist.

- Technology Mapping:
    - Maps the post-optimized GTECH netlist to standard cells specified in the PDK.

To initiate synthesis, use:

```bash
run_synthesis
```


## Floorplan:

The floorplan stage aims to strategically organize the silicon area, establish a reliable power distribution network (PDN), and define macro placement and blockages for a legalized GDS file. Key considerations include achieving a balanced core utilization, setting aspect ratios, and incorporating power planning features.

Environment Variables / Switches:

    `FP_CORE_UTIL:` Specifies floorplan core utilization.
    `FP_ASPECT_RATIO:` Defines the floorplan aspect ratio.
    `FP_CORE_MARGIN:` Sets the core-to-die margin area.
    `FP_IO_MODE:` Configures pin arrangements (1 for equidistant, 0 for non-equidistant).
    `FP_CORE_VMETAL:` Specifies the vertical metal layer for the core.
    `FP_CORE_HMETAL:` Specifies the horizontal metal layer for the core.

Note: Metal layer values are typically 1 more than those specified.

Power Planning:

    Ring Formation: Connects to pads, enabling power distribution along the chip's edges.
    Power Straps: Utilizes higher metal layers to deliver power to the chip's middle, reducing IR drop and addressing electro-migration issues.

Floorplan Execution:
To execute the floorplan, use the following command:

```bash

% run_floorplan
```

This command initiates the floorplanning process, incorporating the specified environment variables and switches. Proper configuration ensures optimal silicon utilization, efficient power distribution, and a layout conducive to subsequent design stages.



- Post the floorplan run, a .def file will have been created within the results/floorplan directory. We may review floorplan files by checking the floorplan.tcl.
- To view the floorplan: Magic is invoked after moving to the results/floorplan directory,then use the floowing command:
- 
```bash
magic -T /home/akhilasati/vsdstdcelldesign/libs/sky130A.tech lef read /home/OpenLane/designs/touch_sensor/runs/RUN_2023.11.14_08.46.33/tmp merged.nom.lef def read wrapper.def &
```



Die area (after floorplan)

![Screenshot from 2023-11-16 00-50-27](https://github.com/akhiiasati/IIITB_Advanced_Physical_Design_using_OpenLANE_Sky130/assets/43675821/c2ced502-9a72-4d87-8d6f-7e4e1ebfe592)


Core area (after floorplan)

![Screenshot from 2023-11-16 00-50-18](https://github.com/akhiiasati/IIITB_Advanced_Physical_Design_using_OpenLANE_Sky130/assets/43675821/83ea8a39-2b2c-4f52-ac4d-d8ebe3e6666b)

## Placement Overview:

The placement stage in the OpenLANE ASIC flow involves arranging standard cells on floorplan rows, aligning them with sites specified in the technology LEF file. Placement is executed in two stages: Global and Detailed, aiming to optimize cell positions and ensure legality.

    - Global Placement:
        `Objective:` Finds optimal positions for all cells, allowing potential overlap and disregarding alignment to rows.
        `Optimization:` Focuses on reducing half parameter wire length to enhance overall performance.
        `Result:` Provides a preliminary arrangement, optimizing for wire length but not necessarily adhering to legal placement constraints.

    - Detailed Placement:
        `Objective:` Refines cell positions post-global placement to legalize and align them with floorplan rows.
        `Optimization:` Adjusts cell positions while respecting global placement constraints.
        `Result:` Yields a legally placed layout that aligns with the floorplan and satisfies design rules.

### Placement Execution:
To run the placement process, execute the following command:

```bash
run_placement
```
![image](https://github.com/ShubhamGitHub528/Home-Automation-System/assets/140998623/b0a7a831-ad24-4b25-983d-17f1e3eaf30e)
This command initiates both the Global and Detailed Placement stages, progressing the design towards a physically realizable layout. Proper placement is crucial for meeting performance and design rule specifications in the subsequent steps of the ASIC flow.



Post placement: the design can be viewed on magic within results/placement directory. Run the follwing command in that directory:

```bash
magic -T /home/shubham/.volare/sky130A/libs.tech/magic/sky130A.tech lef read ../../tmp/merged.nom.lef def read wrapper.def &
```


![image](https://github.com/ShubhamGitHub528/Home-Automation-System/assets/140998623/b8861276-5b02-4e77-810d-353d5dcae32b)



## Clock Tree Synthesis (CTS) Overview:

Clock Tree Synthesis is a critical step in the ASIC design flow aimed at creating an efficient clock distribution network for delivering the clock signal to all sequential elements. The primary objective is to minimize clock skew across the entire chip, ensuring synchronous operation of the design. H-trees are commonly employed as a network topology to achieve this goal.

### Key Objectives:

- Clock Distribution: Ensures the clock signal reaches every sequential element in the design.
- Clock Skew Minimization: Aims to achieve zero clock skew across the chip.
- H-tree Topology: Common methodology in CTS, providing an effective structure for distributing the clock signal evenly.

#### Command to Run CTS:
To perform Clock Tree Synthesis, execute the following command:

```bash
run_cts
```

This command triggers the CTS process, where the tool generates an optimized clock tree structure based on the design requirements and constraints. Achieving a balanced clock distribution is crucial for maintaining synchronization and meeting performance criteria in subsequent stages of the ASIC design flow.

### Timing Reports

![image](https://github.com/ShubhamGitHub528/Home-Automation-System/assets/140998623/967b5d7b-1f59-4629-9a1e-2e5452dee20a)


### AREA Reports

![image](https://github.com/ShubhamGitHub528/Home-Automation-System/assets/140998623/1eda3724-cad9-4257-ad86-67e0b1d65dca)



### Skew Reports
![image](https://github.com/ShubhamGitHub528/Home-Automation-System/assets/140998623/a178b9f4-096a-4f6e-ba0b-0b97b872d98a)



### Power Reports
![image](https://github.com/ShubhamGitHub528/Home-Automation-System/assets/140998623/40d365c0-e555-4032-b6f0-20845536fbc4)


# Routing:

Routing in the OpenLANE ASIC flow involves implementing the interconnect system between standard cells using the remaining available metal layers post Clock Tree Synthesis (CTS) and Power Distribution Network (PDN) generation. TritonRoute is the tool used for routing in OpenLANE, and the process is divided into two stages: Global Routing and Detailed Routing.

### Global Routing:
`Routing Grids:` The routing region is divided into rectangular grids, represented as coarse 3D routes using the Fastroute tool.
`Objective:` Establishes a high-level routing plan across the chip.
`Result:` Provides a global routing framework to guide the subsequent detailed routing stage.

### Detailed Routing:
`Routing Grids and Guides:` Utilizes finer grids and routing guides to implement the physical wiring, employing the TritonRoute tool.
`Features of TritonRoute:`
- Honors pre-processed route guides.
- Assumes that each net satisfies inter-guide connectivity.'
- Utilizes a Mixed-Integer Linear Programming (MILP) based panel routing scheme.
- Implements an intra-layer parallel and inter-layer sequential routing framework.

### Running the Routing Process:
To execute the routing process, use the following command:

```bash
% run_routing
```

This command initiates both the Global Routing and Detailed Routing stages, resulting in a well-defined interconnect system that adheres to design rules and minimizes design rule check (DRC) errors. Proper routing is essential for ensuring signal integrity and meeting performance requirements in the final chip design.

![image](https://github.com/ShubhamGitHub528/Home-Automation-System/assets/140998623/2afb1325-99cd-4031-9ac0-40594456bfea)
![image](https://github.com/ShubhamGitHub528/Home-Automation-System/assets/140998623/e9154a35-3420-498a-86b7-bf0a9c68e8e8)



View the post-routing design in Magic:

```bash
% magic -T /home/shubham/vsdstdcelldesign/libs/sky130A.tech lef read /home/OpenLane/designs/touch_sensor/runs/RUN_2023.11.14_08.46.33/tmp merged.nom.lef def read wrapper.def &

```


#### post_routing Timing Reports

![image](https://github.com/ShubhamGitHub528/Home-Automation-System/assets/140998623/6b6aa089-e566-4900-88b4-24ef369d42a3)



#### post_routing Area Reports

![image](https://github.com/ShubhamGitHub528/Home-Automation-System/assets/140998623/bf840015-99d1-48d3-9963-9d8d827332d8)


#### post_routing Power Reports
![image](https://github.com/ShubhamGitHub528/Home-Automation-System/assets/140998623/d76a3afb-9407-41f0-943f-76514b4f5bbf)



DRC violation is zero:
![image](https://github.com/ShubhamGitHub528/Home-Automation-System/assets/140998623/8bc49cda-a23b-4bf6-adb3-8b642ca96b2b)


Given a Clock period of 70ns in Json file , setup slack we got after routing is 19.13ns

                              1
Max Performance =  ------------------------
                     clock period - slack(setup)

Max Performance = 0.0196 Ghz




## Word of Thanks
I sciencerly thank **Mr. Kunal Gosh**(Founder/**VSD**) for helping me out to complete this flow smoothly.

## Acknowledgement
- Kunal Ghosh, VSD Corp. Pvt. Ltd.
- Skywater Foundry
- Chatgpt
- Amith Bharadwaj,Colleague,IIIT B
- Emil Jayanth Lal,Colleague,IIIT B
- Sumanto Kar,VSD Corp.
- Mayank Kabra (Founder, Chipcron Pvt. Ltd.)

  
  
## Reference 


- https://github.com/The-OpenROAD-Project/OpenSTA.git
- https://github.com/kunalg123
- https://www.vsdiat.com
- https://github.com/SakethGajawada/RISCV-GNU








