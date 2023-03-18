#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NONSTDC_NO_WARNINGS
#include "server.h"

/* Global Variables */
ST_accountsDB_t accountsDB[255] =
{
	{ 8000.0, RUNNING, "7896589674123014" },
	{ 9000.0, BLOCKED, "5849454654646465" },
	{ 6000.0, RUNNING, "9875466121654984" },
	{ 2000.0, BLOCKED, "8723045957293897" },
	{ 1000.0, RUNNING, "4161526162565205" },
	{ 4000.0, BLOCKED, "6473286584372378" },
	{ 5000.0, RUNNING, "3789456310753243" },
	{ 3000.0, BLOCKED, "2564561654856159" },
	{ 50000.0, RUNNING, "1987974987498490" },
	{ 50000.0, BLOCKED, "6546132161651616" }
};
ST_accountsDB_t* accountsDBptr = accountsDB; // to search in the accounts for a valid accaount
ST_accountsDB_t* valid_account_ptr = 0; //to store the valid account address to use it later in the app
ST_transaction_t transactionsDB[255] = { 0 };//initialiize the transaction array
ST_transaction_t* transactionsDBptr = transactionsDB;
ST_transaction_t* transaction_reference = 0; // the pointer used to access transactionsDB array

EN_transState_t transaction_state = 0;
uint32_t transactionsDB_index = 0; // index to be added on transactionDBptr to access transactionsDB array
EN_transState_t recieveTransactionData(ST_transaction_t* transData)
{
	EN_transState_t trans_error_type = 0;
	EN_serverError_t account_validity = 0;
	EN_serverError_t account_state = 0;
	EN_serverError_t amount_availability = 0;
	EN_serverError_t transaction_saved = 0;
	ST_transaction_t* save_transaction_ptr = transData;

	account_validity = isValidAccount(cardptr, accountsDBptr);
	account_state = isBlockedAccount(valid_account_ptr);
	amount_availability = isAmountAvailable(terminalptr, valid_account_ptr);

	if (account_validity == ACCOUNT_NOT_FOUND)
	{
		trans_error_type = FRAUD_CARD;
		transaction_state = FRAUD_CARD;
		printf("***invalid account ***\n");

	}
	else if (account_state == BLOCKED_ACCOUNT)
	{
		trans_error_type = DECLINED_STOLEN_CARD;
		transaction_state = DECLINED_STOLEN_CARD;
		printf("***invalid account ***\n");

	}
	else if (amount_availability == LOW_BALANCE)
	{
		trans_error_type = DECLINED_INSUFFECIENT_FUND;
		transaction_state = DECLINED_INSUFFECIENT_FUND;
		printf("***invalid account ***\n");
	}
	else
	{
		trans_error_type = APPROVED;
		transaction_state = APPROVED;
		valid_account_ptr->balance = valid_account_ptr->balance - terminalptr->transAmount;
		printf("***********approved account *********\n");

	}

	transaction_saved = saveTransaction(save_transaction_ptr);
	if (transaction_saved != SERVER_OK)
	{
		trans_error_type = INTERNAL_SERVER_ERROR;
		transaction_state = INTERNAL_SERVER_ERROR;
		printf("***trans error ***\n");

	}

	return trans_error_type;
}

EN_serverError_t isValidAccount(ST_cardData_t* cardData, ST_accountsDB_t* accountRefrence)
{
	EN_serverError_t server_error_type = 0;
	uint32_t PAN_cmp = 0;
	valid_account_ptr = 0;

	for (uint16_t i = 0; i < number_of_accounts; i++)
	{
		PAN_cmp = 0;
		PAN_cmp = strcmp(cardData->primaryAccountNumber, accountRefrence->primaryAccountNumber);

		if (PAN_cmp == 0)
		{
			server_error_type = SERVER_OK;
			valid_account_ptr = accountRefrence;
			/*printf("///valid account/////\n");*/

			break;
		}
		accountRefrence++;
	}

	if (valid_account_ptr == 0)
	{
		server_error_type = ACCOUNT_NOT_FOUND;
		/*printf("////invalid account/////\n");*/

	}


	return  server_error_type;

}
EN_serverError_t isBlockedAccount(ST_accountsDB_t* accountRefrence)
{
	EN_serverError_t server_error_type = 0;
	if (accountRefrence != 0)
	{
		if (accountRefrence->state == RUNNING)
		{
			server_error_type = SERVER_OK;
			/*printf("///ok account/////\n");*/
		}
		else if (accountRefrence->state == BLOCKED)
		{
			server_error_type = BLOCKED_ACCOUNT;
			/*printf("///blocked account/////\n");*/
		}
	}


	return  server_error_type;
}
EN_serverError_t isAmountAvailable(ST_terminalData_t* termData, ST_accountsDB_t* accountRefrence)
{
	EN_serverError_t server_error_type = 0;
	if (accountRefrence != 0)
	{
		if (termData->transAmount > accountRefrence->balance)
		{
			server_error_type = LOW_BALANCE;
			printf("///low balance account/////\n");
		}
		else
		{
			server_error_type = SERVER_OK;
			printf("///ok account/////\n");
		}
	}

	return  server_error_type;

}

EN_serverError_t saveTransaction(ST_transaction_t* transData)
{
	EN_serverError_t server_error_type = SERVER_OK;

	transData = transData + transactionsDB_index;
	transaction_reference = transData;

	strcpy(transData->cardHolderData.cardHolderName, cardptr->cardHolderName);
	strcpy(transData->cardHolderData.cardExpirationDate, cardptr->cardExpirationDate);
	strcpy(transData->cardHolderData.primaryAccountNumber, cardptr->primaryAccountNumber);

	strcpy(transData->terminalData.transactionDate, terminalptr->transactionDate);
	transData->terminalData.transAmount = terminalptr->transAmount;
	transData->terminalData.maxTransAmount = terminalptr->maxTransAmount;

	transData->transState = transaction_state;

	transData->transactionSequenceNumber = 1 + transactionsDB_index;
	printf("saved transaction\n");
	listSavedTransactions();
	transactionsDB_index++;

	return server_error_type;
}


void listSavedTransactions(void)
{
	printf("#########################");
	printf("\n");
	printf("Transaction Sequence Number: %ld", transaction_reference->transactionSequenceNumber);
	printf("\n");
	printf("Transaction Date: %s", transaction_reference->terminalData.transactionDate);
	printf("\n");
	printf("Transaction Amount: %f", transaction_reference->terminalData.transAmount);
	printf("\n");
	switch (transaction_reference->transState)
	{
	case APPROVED:
		printf("Transaction State: Approved");
		break;

	case DECLINED_INSUFFECIENT_FUND:
		printf("Transaction State: Declined -> Insufficient fund");
		break;

	case DECLINED_STOLEN_CARD:
		printf("Transaction State: Declined -> Blocked Account");
		break;

	case FRAUD_CARD:
		printf("Transaction State: Declined -> Fraud Card");
		break;

	case INTERNAL_SERVER_ERROR:
		printf("Transaction State: Declined -> Internal Server Error");
		break;
	}
	printf("\n");
	printf("Terminal Max Amount: %f", transaction_reference->terminalData.maxTransAmount);
	printf("\n");
	printf("Cardholder Name: %s", transaction_reference->cardHolderData.cardHolderName);
	printf("\n");
	printf("PAN: %s", transaction_reference->cardHolderData.primaryAccountNumber);
	printf("\n");
	printf("Card Expiration Date: %s", transaction_reference->cardHolderData.cardExpirationDate);
	printf("\n");
	printf(" account current balance: %f", valid_account_ptr->balance);
	printf("\n");
	printf("#########################");
	printf("\n");

}


void recieveTransactionDataTest(void) {


	printf("Tester Name: ahmed khaled abdellatif\n");
	printf("Function Name: recieveTransactionData \n");
	printf("Test Case 1:valid account  \n");
	printf("Expected Result:***approved account ***\n");
	

	getCardPAN(cardptr);

	getTransactionAmount(terminalptr);
	recieveTransactionData(transactionsDBptr);
	printf("********************************************\n");
}
void isAmountAvailabletest(void) {
	printf("Tester Name: ahmed khaled abdellatif\n");
	printf("Function Name:isAmountAvailable \n");
	printf("Test Case 2:invalid account  \n");
	printf("Expected Result:***low balance ***\n");
	
	getCardPAN(cardptr);

	getTransactionAmount(terminalptr);
	isValidAccount(cardptr, accountsDBptr);
	isAmountAvailable(terminalptr,valid_account_ptr);
}

void saveTransactionTest(void) {


	printf("Tester Name: ahmed khaled abdellatif\n");
	printf("Function Name: savetransaction \n");
	printf("Test Case 1:saved transaction  \n");
	printf("Expected Result:***saved transaction ***\n");


	getCardPAN(cardptr);

	getTransactionAmount(terminalptr);
	recieveTransactionData(transactionsDBptr);
	saveTransaction(valid_account_ptr);
	
	printf("********************************************\n");
}
void listsaveTransactionTest(void) {


	printf("Tester Name: ahmed khaled abdellatif\n");
	printf("Function Name: listsavetransaction \n");
	printf("Test Case 1:saved transaction viewing \n");
	printf("Expected Result:saved transaction information\n");


	getCardPAN(cardptr);

	getTransactionAmount(terminalptr);
	recieveTransactionData(transactionsDBptr);
	saveTransaction(valid_account_ptr);

	printf("********************************************\n");
}


//int main() {
//	
//	recieveTransactionDataTest();
// }