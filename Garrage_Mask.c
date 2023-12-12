
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


