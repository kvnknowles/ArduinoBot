

/* Blink*/

void setup(){
 pinMode(13, OUTPUT);
}


void test_success(){
        digitalWrite(13, HIGH);
        delay(5000);
        digitalWrite(13, LOW);

}

void test_fail(int x){
	int x;

	delay(5000);


        for(i=0;i<x;i++){
	   digitalWrite(13, HIGH);
           delay(1000);
           digitalWrite(13, LOW);
           delay(1000); 
        }
} 

void loop(){
	test_fail(7);
}

