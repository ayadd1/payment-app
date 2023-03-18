#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NONSTDC_NO_WARNINGS
#include "card.h"

ST_cardData_t card;
ST_cardData_t* cardptr = &card;

EN_cardError_t getCardHolderName(ST_cardData_t* cardData)//
{
    EN_cardError_t name_err_type = 0;
    uint8_t name[28] = { 0 };
    uint32_t u = 0;
    printf(" enter your name : "); //asking for name
    fgets(name, sizeof(name), stdin);//taking array of characters 
    while (name[u] != '\0')//the end of the  array is represented by NULL as long as its not null keep going 
    {
        u++;
    }
    if (u > 20 && u < 28) // setting boundries to the name that must be more that 28 letters and more than 20 letters 
    {
        name_err_type = CARD_OK;
        strcpy(cardData->cardHolderName, name);
        cardData->cardHolderName[u - 1] = NULL;//set thhe last aarray element to null 
        /*printf(" OK\n");*/
    }
    else
    {
        name_err_type = WRONG_NAME;
        /*printf(" WRONG\n");*/
    }

    fflush(stdin);

    return name_err_type;// return 0 or 1 0is ok and 1 means that there is ana error
}

EN_cardError_t getCardExpiryDate(ST_cardData_t* cardData)
{
    EN_cardError_t exp_error_type = 0;
    uint8_t exp[10] = { 0 };// expiery date array
    uint16_t j = 0;
    uint8_t MM[3] = { 0 };// month array
    uint8_t YY[3] = { 0 };//year array
    uint16_t MM_number = 0;
    uint16_t YY_number = 0;

    printf("Please enter card exp date in form MM/YY : "); // Asking for expiry date
    // printf("\n");
    fgets(exp, sizeof(exp), stdin);  // Taking string input from the client

    while (exp[j] != '\0') //the end of the  array is represented by NULL as long as its not null keep going 
    {
        j++;
    }

    MM[0] = exp[0]; //coping the data of the month from the exp array to the month array
    MM[1] = exp[1];
    MM[2] = NULL;
    MM_number = atoi(MM); // this function converts strings to inegers to store them in the mm_number variable to compare it with our boundries 

    YY[0] = exp[3];
    YY[1] = exp[4];
    YY[2] = NULL;
    YY_number = atoi(YY);

    if (exp[2] != '/')
    {
        exp_error_type = WRONG_EXP_DATE;
       /* printf(" WRONG\n");*/
       
    }
    else if (MM_number > 12)
    {
        exp_error_type = WRONG_EXP_DATE;
        /*printf(" WRONG\n");*/
      
    }
    else if (YY_number < 22)
    {
        exp_error_type = WRONG_EXP_DATE;
        /*printf(" WRONG\n");*/
    }
    else if (j != 6)
    {
        exp_error_type = WRONG_EXP_DATE;
       /* printf(" WRONG\n");*/
    }
    else
    {
        exp_error_type = CARD_OK;
        strcpy(cardData->cardExpirationDate, exp); //store the data in the card exiration date element in the card data struct 
        cardData->cardExpirationDate[5] = NULL; // set the last element of the array to null
       /* printf(" OK\n");*/
    }

    return exp_error_type;
}

EN_cardError_t getCardPAN(ST_cardData_t* cardData)
{
    EN_cardError_t pan_error_type = 0;
    uint8_t pan[40] = { 0 };
    uint16_t k = 0;

    printf("Please enter your card's PAN : "); 
    fgets(pan, sizeof(pan), stdin); 

    while (pan[k] != '\0')
    {
        k++;
    }

    if (k > 16 && k < 21)
    {
        pan_error_type = CARD_OK;
        strcpy(cardData->primaryAccountNumber, pan);
        cardData->primaryAccountNumber[k - 1] = NULL;
        /*printf(" OK\n");*/
    }
    else
    {
        pan_error_type = WRONG_PAN;
       /* printf(" wrong\n");*/
    }

    return pan_error_type;
    
}
