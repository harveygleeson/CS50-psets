from cs50 import get_int, get_string

# get credit card number
cardnum_i = get_int("Number: ")

# convert the num to string
cardnum_s = str(cardnum_i)

# define the checksum function, return true if valid


def checksum(card_number):

    counter = 0
    
    # even number
    if int(len(card_number)) % 2 == 0:
        for i in range(len(card_number)):
            if i % 2 == 0:                      
                x = int(card_number[i]) * 2
                if len(str(x)) > 1:
                    counter += int(str(x)[0])
                    counter += int(str(x)[1])
                else:
                    counter += x
            else:
                counter += int(card_number[i])

   # odd number
    else: 
        for i in range(len(card_number)):
            if i % 2 != 0:
                x = int(card_number[i]) * 2
                if len(str(x)) > 1:
                    counter += int(str(x)[0])
                    counter += int(str(x)[1])
                else:
                    counter += x
            else:
                counter += int(card_number[i])

    return counter % 10 == 0
    

# card maker check
if checksum(cardnum_s):
    if cardnum_s[0] == "4" and (len(cardnum_s) == 13 or len(cardnum_s) == 16):      # card criteria
        print("VISA") 
    elif cardnum_s[0:2] == "37" or cardnum_s[0:2] == "34":
        if len(cardnum_s) == 15:
            print("AMEX")
        else:
            print("INVALID")
    elif len(cardnum_s) == 16:
        if cardnum_s[0:2] in ["51", "52", "52", "54", "55"]:
            print("MASTERCARD")
        else:
            print("INVALID")
else:
    print("INVALID")
