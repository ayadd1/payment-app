#define _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_DEPRECATE  
#define _CRT_NONSTDC_NO_DEPRECATE

#include "terminal.h"
/* Global variables */
ST_terminalData_t terminal;
ST_terminalData_t* terminalptr = &terminal;

/* Terminal Functions */
EN_terminalError_t getTransactionDate(ST_terminalData_t* termData)
{
	EN_terminalError_t trans_date_error_type = 0; // OK or WRONG_DATE
	uint8_t transaction[20] = { 0 };//initialize the array for the transaction date 
	uint16_t p = 0;//initialize the counter
	uint8_t dd[3] = { 0 }; //intitialise the day array
	uint8_t mm[3] = { 0 };//intitialise the month array
	uint8_t yyyy[5] = { 0 };//intitialise the year array
	uint16_t dd_number = 0;//intitialise the day number 
	uint16_t mm_number = 0;//intitialise the month number
	uint16_t yyyy_number = 0;//intitialise the year number
	printf(" Please enter transaction date in the form of DD/MM/YYYY : "); // Asking the client to enter transaction date
	//printf("\n");
	fgets(transaction, sizeof(transaction), stdin);  // Take string input from the client

	while (transaction[p] != '\0')
	{
		p++;
	}

	dd[0] = transaction[0];      ////////copy the trans array to tthe day / month and year array
	dd[1] = transaction[1];
	dd[2] = '\0';
	dd_number = atoi(dd);

	mm[0] = transaction[3];
	mm[1] = transaction[4];
	mm[2] = '\0';
	mm_number = atoi(mm);

	yyyy[0] = transaction[6];
	yyyy[1] = transaction[7];
	yyyy[2] = transaction[8];
	yyyy[3] = transaction[9];
	yyyy[4] = '\0';
	yyyy_number = atoi(yyyy);

	if ((transaction[2] != '/') | (transaction[5] != '/'))
	{
		trans_date_error_type = WRONG_DATE;
		printf("Actual Result: WRONG\n");
	}
	else if (dd_number > 31)
	{
		trans_date_error_type = WRONG_DATE;
		printf(" Actual Result:WRONG\n");
	}
	else if (mm_number > 12)
	{
		trans_date_error_type = WRONG_DATE;
		printf("Actual Result: WRONG\n");
	}
	else if (yyyy_number < 2010)
	{
		trans_date_error_type = WRONG_DATE;
		printf("Actual Result: WRONG\n");
	}
	else if (p != 11)
	{
		trans_date_error_type = WRONG_DATE;
		printf(" Actual Result: WRONG\n");
	}
	else
	{
		trans_date_error_type = TERMINAL_OK;
		strcpy(termData->transactionDate, transaction);
		terminalptr->transactionDate[10] = '\0';
		printf("Actual Result: OK\n");
	}

	return trans_date_error_type;
}
EN_terminalError_t isCardExpired(ST_cardData_t* cardData, ST_terminalData_t* termData)
{
	EN_terminalError_t expired_card_error_type = 0;
	uint8_t mm_trans[3] = { 0 };
	uint8_t yy_trans[3] = { 0 };
	uint8_t mm_exp[3] = { 0 };
	uint8_t yy_exp[3] = { 0 };
	uint16_t mm_trans_number = 0;
	uint16_t yy_trans_number = 0;
	uint16_t mm_exp_number = 0;
	uint16_t yy_exp_number = 0;

	mm_trans[0] = termData->transactionDate[3];
	mm_trans[1] = termData->transactionDate[4];
	mm_trans[2] = '\0';
	mm_trans_number = atoi(mm_trans);

	yy_trans[0] = termData->transactionDate[8];
	yy_trans[1] = termData->transactionDate[9];
	yy_trans[2] = '\0';
	yy_trans_number = atoi(yy_trans);

	mm_exp[0] = cardData->cardExpirationDate[0];
	mm_exp[1] = cardData->cardExpirationDate[1];
	mm_exp[2] = '\0';
	mm_exp_number = atoi(mm_exp);

	yy_exp[0] = cardData->cardExpirationDate[3];
	yy_exp[1] = cardData->cardExpirationDate[4];
	yy_exp[2] = '\0';
	yy_exp_number = atoi(yy_exp);

	if (yy_trans_number > yy_exp_number)
	{
		expired_card_error_type = EXPIRED_CARD;
		printf("Actual Result:expired\n");
	}
	else if (yy_trans_number < yy_exp_number)
	{
		expired_card_error_type = TERMINAL_OK;
		printf("Actual Result:ok\n");
	}
	else if (yy_trans_number == yy_exp_number)
	{
		if (mm_trans_number <= mm_exp_number)
		{
			expired_card_error_type = TERMINAL_OK;
			printf("Actual Result:ok\n");
		}
		else if (mm_trans_number > mm_exp_number)
		{
			expired_card_error_type = EXPIRED_CARD;
			printf("Actual Result:expired\n");
			
		}
	}

	return expired_card_error_type;

}
EN_terminalError_t getTransactionAmount(ST_terminalData_t* termData)
{
	EN_terminalError_t trans_amount_error_type = 0; // OK or INVALID_AMOUNT
	float trans_amount = 0.0;

	printf("Enter Transaction Amount : ");
	//printf("\n");
	scanf_s("%f", &trans_amount);

	if (trans_amount <= 0.0)
	{
		trans_amount_error_type = INVALID_AMOUNT;
		printf("erorr\n");

	}
	else
	{
		trans_amount_error_type = TERMINAL_OK;
		termData->transAmount = trans_amount;
		printf("ok\n");
	}
	return trans_amount_error_type;
}
EN_terminalError_t setMaxAmount(ST_terminalData_t* termData, float maxAmount)
{
	EN_terminalError_t max_amount_error_type = 0;

	if (maxAmount <= 0.0)
	{
		max_amount_error_type = INVALID_MAX_AMOUNT;
		printf("erorr\n");
	}
	else
	{
		max_amount_error_type = TERMINAL_OK;
		termData->maxTransAmount = maxAmount;
		printf("ok\n");
	}

	return max_amount_error_type;

}
EN_terminalError_t isBelowMaxAmount(ST_terminalData_t* termData)
{
	EN_terminalError_t below_max_amout_error_type = 0; // OK or EXCEED_MAX_AMOUNT

	if (termData->transAmount > termData->maxTransAmount)
	{
		below_max_amout_error_type = EXCEED_MAX_AMOUNT;
		printf("exeed max\n");
	}
	else
	{
		below_max_amout_error_type = TERMINAL_OK;
		printf("ok\n");
	}
	return below_max_amout_error_type;
}


void getTransactionDatetest(void){
	getTransactionDate(terminalptr);
}
void isCardExpiredtest(void) {
	getCardExpiryDate(cardptr);
	getTransactionDate(terminalptr);
	isCardExpired(cardptr, terminalptr);
}
void getTransactionAmounttest(void) {

	printf("Tester Name: ahmed khaled abdellatif\n");
	printf("Function Name: get transactiona mount \n");
	printf("Test Case 1:ok  \n");
	printf("Expected Result:ok*\n");
	getTransactionAmount(terminalptr);
}
void isBelowmaxtest(void) {
	printf("Tester Name: ahmed khaled abdellatif\n");
	printf("Function Name: is below max Amount \n");
	printf("Test Case 1:ok  \n");
	printf("Expected Result:ok*\n");
	getTransactionAmount(terminalptr);
	setMaxAmount(terminalptr, 3000);
	isBelowMaxAmount(terminalptr);
}
void setMaxAmounttest(void) {
	setMaxAmount(terminalptr, 3000);

}
//void main() {
//	
//	
//	
//	
//	/*setMaxAmounttest();*/
//}