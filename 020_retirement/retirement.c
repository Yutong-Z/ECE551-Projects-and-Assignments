#include <stdio.h>
#include <stdlib.h>

struct _retire_info {
  int month;
  double contribution;
  double rate_of_return;
};
typedef struct _retire_info retire_info;

/* A function print out current balance and current age every month
and return final balance
with input of initial blance and age as well as working/retriement info */
double balance_statement(int age, double balance, retire_info StateInfo) {
  int m = StateInfo.month;
  double c = StateInfo.contribution;
  double r = StateInfo.rate_of_return;
  for (int j = 0; j < m; j++) {
    printf("Age %3d month %2d you have $%.2lf\n", age / 12, age % 12, balance);
    balance = balance * (1 + r) + c;
    age++;
  }
  return balance;
}

void retirement(int startAge,         //in months
                double initial,       //initial savings in dollars
                retire_info working,  //info about working
                retire_info retired)  //info about being retired
{
  double balance = balance_statement(startAge, initial, working);
  int age = startAge + working.month;
  balance = balance_statement(age, balance, retired);
}

int main(void) {
  retire_info working;
  working.month = 489;
  working.contribution = 1000;
  working.rate_of_return = 0.045 / 12;
  retire_info retired;
  retired.month = 384;
  retired.contribution = -4000;
  retired.rate_of_return = 0.01 / 12;
  int startAge = 327;
  double initial = 21345;
  retirement(startAge, initial, working, retired);
  return EXIT_SUCCESS;
}
