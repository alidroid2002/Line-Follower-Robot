int pins[]={A0,A1,A2,A3,A4};
int threshold = 500;
int readings[5];
int lineValue[]={0,10,20,30,40};
int num = 0;
int sum;

void setup() {
 Serial.begin(9600);
}

void loop() {
 sum=0;
 num=0;
 for(int i=0;i<5;i++)
 {
  int a= analogRead(pins[i]);
  if(a>threshold)
 {
  a=0;
 }
  else
  {a=1;
  num++;
  }
  readings[i]=a*lineValue[i];
 }
 for(int i=0;i<5;i++)
 {
  sum+=readings[i];
 }
 if(num != 0)
 {
 sum/=num;
 Serial.println(20-sum);
 }
}
