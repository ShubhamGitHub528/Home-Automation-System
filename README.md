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

![image](https://github.com/DINESHIIITB/iiitb_riscv_drip_irrigation_system/assets/140998565/ecccb444-7d79-4d52-a9fd-1176117a5463)


### wrapper module after netlist created

```
show wrapper
```

![image](https://github.com/DINESHIIITB/iiitb_riscv_drip_irrigation_system/assets/140998565/1be94917-691d-4aae-8603-e26ee64a6d6c)



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
![Screenshot from 2023-11-15 22-55-49](https://github.com/akhiiasati/IIITB_Advanced_Physical_Design_using_OpenLANE_Sky130/assets/43675821/b9a1c588-a1a3-4af6-92a5-45049fc282d8)

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

![Screenshot from 2023-11-15 23-02-02](https://github.com/akhiiasati/IIITB_Advanced_Physical_Design_using_OpenLANE_Sky130/assets/43675821/eaa78027-b9c0-4e5f-9501-f4b15f402bf6)

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

![Screenshot from 2023-11-15 23-07-23](https://github.com/akhiiasati/IIITB_Advanced_Physical_Design_using_OpenLANE_Sky130/assets/43675821/4fb43e7b-2386-46fe-8483-f5591cb86f38)


- Post the floorplan run, a .def file will have been created within the results/floorplan directory. We may review floorplan files by checking the floorplan.tcl.
- To view the floorplan: Magic is invoked after moving to the results/floorplan directory,then use the floowing command:
- 
```bash
magic -T /home/akhilasati/vsdstdcelldesign/libs/sky130A.tech lef read /home/OpenLane/designs/touch_sensor/runs/RUN_2023.11.14_08.46.33/tmp merged.nom.lef def read wrapper.def &
```
![Screenshot from 2023-11-15 23-57-12](https://github.com/akhiiasati/IIITB_Advanced_Physical_Design_using_OpenLANE_Sky130/assets/43675821/ac94b061-c3ab-4336-ade9-d48330f644b3)


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

This command initiates both the Global and Detailed Placement stages, progressing the design towards a physically realizable layout. Proper placement is crucial for meeting performance and design rule specifications in the subsequent steps of the ASIC flow.

![Screenshot from 2023-11-16 00-55-08](https://github.com/akhiiasati/IIITB_Advanced_Physical_Design_using_OpenLANE_Sky130/assets/43675821/209a6ede-3e95-48b0-94bb-e9257e4069ef)

Post placement: the design can be viewed on magic within results/placement directory. Run the follwing command in that directory:

```bash
magic -T /home/akhilasati/vsdstdcelldesign/libs/sky130A.tech lef read /home/OpenLane/designs/touch_sensor/runs/RUN_2023.11.14_08.46.33/tmp merged.nom.lef def read wrapper.def &
```

![Screenshot from 2023-11-16 00-57-02](https://github.com/akhiiasati/IIITB_Advanced_Physical_Design_using_OpenLANE_Sky130/assets/43675821/29a262a9-d3f6-4be5-bda1-38e65f6888fe)


![WhatsApp Image 2023-11-16 at 02 31 43_05327e13](https://github.com/akhiiasati/Touch_Bell_RISCV/assets/43675821/133416c4-aee6-4494-a31e-1bb281e4810f)


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

![Screenshot from 2023-11-16 01-04-38](https://github.com/akhiiasati/Touch_Bell_RISCV/assets/43675821/8f90dab2-1f53-47f1-8974-f546983bc747)

### Timing Reports

![Screenshot from 2023-11-16 01-32-14](https://github.com/akhiiasati/Touch_Bell_RISCV/assets/43675821/72960463-2357-4155-a206-28e7991503dc)


### AREA Reports

![Screenshot from 2023-11-16 01-32-32](https://github.com/akhiiasati/Touch_Bell_RISCV/assets/43675821/21c8d751-ba10-4ebf-86a1-b8ecdf2e7c80)


### Skew Reports

![Screenshot from 2023-11-16 01-33-04](https://github.com/akhiiasati/Touch_Bell_RISCV/assets/43675821/6b0c12ec-a203-4af0-9a58-8d61eb06d841)


### Power Reports

![Screenshot from 2023-11-16 01-33-17](https://github.com/akhiiasati/Touch_Bell_RISCV/assets/43675821/ef016302-b176-488b-bc7c-51cbed1fff13)

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

![Screenshot from 2023-11-16 01-41-08](https://github.com/akhiiasati/Touch_Bell_RISCV/assets/43675821/25dbec4b-f530-4750-8f1e-676023800b14)

![WhatsApp Image 2023-11-16 at 02 31 43_a72b96b9](https://github.com/akhiiasati/Touch_Bell_RISCV/assets/43675821/3bd17745-d375-4c22-ba3c-4b16a99bbec8)


View the post-routing design in Magic:

```bash
% magic -T /home/akhilasati/vsdstdcelldesign/libs/sky130A.tech lef read /home/OpenLane/designs/touch_sensor/runs/RUN_2023.11.14_08.46.33/tmp merged.nom.lef def read wrapper.def &

```

![Screenshot from 2023-11-16 01-51-58](https://github.com/akhiiasati/Touch_Bell_RISCV/assets/43675821/c0f30791-159a-4d6d-93b8-c7e785ef0705)


#### post_routing Timing Reports

![Screenshot from 2023-11-16 01-56-31](https://github.com/akhiiasati/Touch_Bell_RISCV/assets/43675821/46e10986-80ef-4424-8e0b-9cda7c3d9e97)


#### post_routing Area Reports

![Screenshot from 2023-11-16 01-53-21](https://github.com/akhiiasati/Touch_Bell_RISCV/assets/43675821/16635e3b-4798-4593-8803-f355ea689f18)

#### post_routing Power Reports

![Screenshot from 2023-11-16 01-57-19](https://github.com/akhiiasati/Touch_Bell_RISCV/assets/43675821/8475ba77-ac4f-448f-abd4-694bf9e854ec)


DRC violation is zero:
![Screenshot from 2023-11-16 02-01-48](https://github.com/akhiiasati/Touch_Bell_RISCV/assets/43675821/6654f7e2-edfd-455a-aac3-2ba5872ca7ee)

Given a Clock period of 70ns in Json file , setup slack we got after routing is 19.13ns

                              1
Max Performance =  ------------------------
                     clock period - slack(setup)

Max Performance = 0.0196 Ghz

## Some Extra Steps:

![Screenshot from 2023-11-14 15-53-31](https://github.com/akhiiasati/Touch_Bell_RISCV/assets/43675821/122ed158-798b-4eb3-814e-917c271fef64)




















## Placement 
/home/shubham/OpenLane/designs/Garage_Door/runs/RUN_2023.11.14_16.11.27/results/floorplan
magic -T /home/shubham/.volare/sky130A/libs.tech/magic/sky130A.tech lef read ../../tmp/merged.nom.lef def read wrapper.def &
![image](https://github.com/ShubhamGitHub528/Home-Automation-System/assets/140998623/b233d77d-bee0-43a2-9c70-d399a1e1911a)

















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








