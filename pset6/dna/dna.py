from sys import argv
from csv import DictReader, reader

# ensure correct usage
if (len(argv) != 3):
    print('Usage: python dna.py data.csv sequence.txt')
    
else:
    
    # open txtfile
    with open(argv[2], 'r') as txtfile:
        DNA = txtfile.read()
        
    # open csvfile and create reader object
    with open(argv[1], newline='') as csvfile:
        data = reader(csvfile)
        strs = next(data)
        strs.remove(strs[0])
        
    # make sequences dict
    sequences = {}
    for item in strs:
        sequences[item] = 1

    # check the number of repetitions for each strand and add to the sequences dict
    for strand in sequences:
        
        l = len(strand)
        temp = 0
        tempMax = 0
        
        for i in range(len(DNA)):
            while temp > 0:
                temp -= 1
                continue

            if DNA[i:i + l] == strand:
                while DNA[i - l: i] == DNA[i: i + l]:
                    i += l
                    temp += 1
                    
                if temp > tempMax:
                    tempMax = temp
            
        sequences[strand] += tempMax
        
    # convert int values to str for comparing
    for key in sequences:
        sequences[key] = str(sequences[key])
            
    # open csv file to check for a match
    with open(argv[1], newline='') as csvfile:
        person = reader(csvfile)
        next(person)
        
        # list values from sequences dict for each STR
        values = []
        for key in (sequences):
            values.append(sequences[key])
        
        # check for a match, print name and exit
        for row in person:
            if (row[1:] == values):
                print(row[0])
                exit()
            else:
                continue
        # print no match if none found
        else:
            print("No match")