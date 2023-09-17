#define RAW_DATA_LEN 100
uint16_t rawData[RAW_DATA_LEN]={
  5962, 1954, 510, 1930, 510, 1934, 506, 686, 
  510, 686, 510, 1934, 506, 686, 510, 686, 
  506, 1934, 510, 1934, 510, 1930, 506, 686, 
  510, 686, 510, 686, 506, 686, 510, 686, 
  506, 1934, 510, 686, 510, 686, 506, 690, 
  506, 1934, 506, 690, 506, 1934, 506, 1934, 
  510, 686, 506, 686, 510, 1934, 506, 1934, 
  506, 1934, 506, 690, 506, 686, 506, 690, 
  506, 690, 506, 686, 510, 682, 510, 686, 
  510, 686, 506, 690, 506, 686, 510, 686, 
  506, 690, 506, 686, 510, 686, 506, 690, 
  506, 686, 506, 690, 506, 686, 506, 690, 
  506, 690, 506, 1000};

void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
Serial.print(sizeof(rawData[2]));
Serial.println(" bytes");
Serial.println(rawData[1],HEX);
Serial.println(rawData[0],BIN);
Serial.println(rawData[0],BIN);
}

void loop() {
  // put your main code here, to run repeatedly:
}
