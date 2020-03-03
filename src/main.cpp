#include <Arduino.h>
// Código exemplo para teste de Micro Servo Motor SG90 
// Movimento do servo através de comandos especificados 
  
#include "Servo.h" // Inclui a Biblioteca Servo.h 
#include <Fuzzy.h>
#define pinLDR A0
Servo meuservo; // Cria o objeto servo para programação 
int angulo = 0; // Ajusta o ângulo inicial do Servo 
float teta=0;

Fuzzy *fuzzy = new Fuzzy();

void setup() {
  meuservo.attach(13); // Declara o pino do servo rotação horizontal
  meuservo.attach(12); // Declara pino do servo rotação vertical
  Serial.begin(9600);  // declara o serial do LDR

  // Instantiating a FuzzyInput object
  FuzzyInput *distance = new FuzzyInput(1);
  // Instantiating a FuzzySet object
  FuzzySet *small = new FuzzySet(0, 20, 20, 40);
  // Including the FuzzySet into FuzzyInput
  distance->addFuzzySet(small);
  // Instantiating a FuzzySet object
  FuzzySet *safe = new FuzzySet(30, 50, 50, 70);
  // Including the FuzzySet into FuzzyInput
  distance->addFuzzySet(safe);
  // Instantiating a FuzzySet object
  FuzzySet *big = new FuzzySet(60, 80, 80, 80);
  // Including the FuzzySet into FuzzyInput
  distance->addFuzzySet(big);
  // Including the FuzzyInput into Fuzzy
  fuzzy->addFuzzyInput(distance);

  // Instantiating a FuzzyOutput objects
  FuzzyOutput *speed = new FuzzyOutput(1);
  // Instantiating a FuzzySet object
  FuzzySet *slow = new FuzzySet(0, 10, 10, 20);
  // Including the FuzzySet into FuzzyOutput
  speed->addFuzzySet(slow);
  // Instantiating a FuzzySet object
  FuzzySet *average = new FuzzySet(10, 20, 30, 40);
  // Including the FuzzySet into FuzzyOutput
  speed->addFuzzySet(average);
  // Instantiating a FuzzySet object
  FuzzySet *fast = new FuzzySet(30, 40, 40, 50);
  // Including the FuzzySet into FuzzyOutput
  speed->addFuzzySet(fast);
  // Including the FuzzyOutput into Fuzzy
  fuzzy->addFuzzyOutput(speed);

  // Building FuzzyRule "IF distance = small THEN speed = slow"
  // Instantiating a FuzzyRuleAntecedent objects
  FuzzyRuleAntecedent *ifDistanceSmall = new FuzzyRuleAntecedent();
  // Creating a FuzzyRuleAntecedent with just a single FuzzySet
  ifDistanceSmall->joinSingle(small);
  // Instantiating a FuzzyRuleConsequent objects
  FuzzyRuleConsequent *thenSpeedSlow = new FuzzyRuleConsequent();
  // Including a FuzzySet to this FuzzyRuleConsequent
  thenSpeedSlow->addOutput(slow);
  // Instantiating a FuzzyRule objects
  FuzzyRule *fuzzyRule01 = new FuzzyRule(1, ifDistanceSmall, thenSpeedSlow);
  // Including the FuzzyRule into Fuzzy
  fuzzy->addFuzzyRule(fuzzyRule01);

  // Building FuzzyRule "IF distance = safe THEN speed = average"
  // Instantiating a FuzzyRuleAntecedent objects
  FuzzyRuleAntecedent *ifDistanceSafe = new FuzzyRuleAntecedent();
  // Creating a FuzzyRuleAntecedent with just a single FuzzySet
  ifDistanceSafe->joinSingle(safe);
  // Instantiating a FuzzyRuleConsequent objects
  FuzzyRuleConsequent *thenSpeedAverage = new FuzzyRuleConsequent();
  // Including a FuzzySet to this FuzzyRuleConsequent
  thenSpeedAverage->addOutput(average);
  // Instantiating a FuzzyRule objects
  FuzzyRule *fuzzyRule02 = new FuzzyRule(2, ifDistanceSafe, thenSpeedAverage);
  // Including the FuzzyRule into Fuzzy
  fuzzy->addFuzzyRule(fuzzyRule02);

  // Building FuzzyRule "IF distance = big THEN speed = high"
  // Instantiating a FuzzyRuleAntecedent objects
  FuzzyRuleAntecedent *ifDistanceBig = new FuzzyRuleAntecedent();
  // Creating a FuzzyRuleAntecedent with just a single FuzzySet
  ifDistanceBig->joinSingle(big);
  // Instantiating a FuzzyRuleConsequent objects
  FuzzyRuleConsequent *thenSpeedFast = new FuzzyRuleConsequent();
  // Including a FuzzySet to this FuzzyRuleConsequent
  thenSpeedFast->addOutput(fast);
  // Instantiating a FuzzyRule objects
  FuzzyRule *fuzzyRule03 = new FuzzyRule(3, ifDistanceBig, thenSpeedFast);
  // Including the FuzzyRule into Fuzzy
  fuzzy->addFuzzyRule(fuzzyRule03);
}
  
void loop() { 
  int LDR1 = analogRead(A0);
  int LDR2 = analogRead(A1);
  int LDR3 = analogRead(A2);
  int LDR4 = analogRead(A3);
  int LDR5 = analogRead(A4);
  Serial.print(LDR1);
  Serial.print(' ');
  Serial.print(LDR2);
  Serial.print(' ');
  Serial.print(LDR3);
  Serial.print(' ');
  Serial.print(LDR4);
  Serial.print(' ');
  //Serial.println(LDR5);
    
  // delay(50);
  if(LDR1<LDR2&&LDR1<LDR3){
    if(LDR2>LDR3) 
      fuzzy->setInput(1, abs(LDR2-LDR3));
      // teta=map(LDR2-LDR3,0,1023,45,90);
    else 
      fuzzy->setInput(1, abs(LDR3-LDR2));
      // map(LDR3-LDR2,0,1023,0,45);
  }
  else if(LDR3<LDR1&&LDR3<LDR2){
    if(LDR2>LDR1) 
      fuzzy->setInput(1, abs(LDR2-LDR1));
      // teta=map(LDR2-LDR1,0,1023,135,90);
    else 
      fuzzy->setInput(1, abs(LDR1-LDR2));
      // map(LDR1-LDR2,0,1023,180,135);
  }
  else Serial.println("erro");

  fuzzy->fuzzify();

  teta = fuzzy->defuzzify(1);

  meuservo.write(teta);
  delay(50);
}  