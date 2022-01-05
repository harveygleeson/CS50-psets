from cs50 import get_string

letters = 0
sentences = 0
words = 1

# get input from user
text = get_string("Text: ")

# loop through each character and update letters, words, sentences
for i in text:
    if i.isalpha():
        letters += 1
    elif i == '.' or i == '!' or i == '?':
        sentences += 1
    elif i == ' ':
        words += 1
    else:
        continue

# S and L per 100 W
L = (letters / words) * 100
S = (sentences / words) * 100

# CL index
index = 0.0588 * L - 0.296 * S - 15.8
grade_i = round(index)
grade_s = str(grade_i)

# print results
if grade_i < 1:
    print('Before Grade 1')
elif grade_i > 16:
    print('Grade 16+')
else:
    print('Grade ' + grade_s)
